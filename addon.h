/*
 * =====================================================================================
 *
 *       Filename:  addon.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2021年01月31日 10时50分07秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  zhaoshu, 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef ADDONE_H
#define ADDONE_H
#include <napi.h>
#include <map>
#include <iostream>
#include "AGIP.h"
#include "static_reflection.h"

struct Addon{
  Addon(const Napi::CallbackInfo& info){
    optInitCB = NULL;
    Napi::Object obj = info[0].ToObject();
	ip = obj.Get("ip").ToString().Utf8Value();
    port = (uint16_t)obj.Get("port").ToNumber().Uint32Value();
  }
  uint16_t port;
  std::string ip;
  napi_async_work work;          // 保存线程任务的
  std::map<uint32_t, napi_threadsafe_function> tsfns; // 保存回调函数的
};

/////////////////////////object -> struct  
struct GenericFunctor {
  // ... context data
  Napi::Object& obj;
  void operator()(char* field, const char* name, size_t size) {
    //printf("============2 %s: %s [%p]\n", name, field, &field);
    memcpy(field,obj.Get(name).ToString().Utf8Value().c_str(), size);
  }
  template <typename Field, typename Name, typename Size>
  void operator()(Field&& field, Name&& name, Size&& size) {
    //printf("============4 %s [%p]\n", name, &field);
    field = (Field)obj.Get(name).ToNumber().Int32Value();
  }
  void operator()(uint16_t field, const char* name, size_t size) {
    //printf("============3 %s [%p]\n", name, &field);
    field = (uint16_t)obj.Get(name).ToNumber().Uint32Value();
  }
  void operator()(int16_t field, const char* name, size_t size) {
    //printf("============3 %s [%p]\n", name, &field);
    field = (int16_t)obj.Get(name).ToNumber().Int32Value();
  }

  void operator()(int8_t field, const char* name, size_t size) {
    //printf("============1 %s [%p]\n", name, &field);
    field = (int8_t)obj.Get(name).ToNumber().Int32Value();
  }
  void operator()(uint8_t field, const char* name, size_t size) {
    //printf("============1 %s [%p]\n", name, &field);
    field = (uint8_t)obj.Get(name).ToNumber().Int32Value();
  }

  void operator()(uint32_t field, const char* name, size_t size) {
    //printf("============6 %s [%p]\n", name, &field);
    field = (uint32_t)obj.Get(name).ToNumber().Uint32Value();
  }
  void operator()(int32_t field, const char* name, size_t size) {
    //printf("============6 %s [%p]\n", name, &field);
    field = (int32_t)obj.Get(name).ToNumber().Int32Value();
  }



  //这时对头的解析。原本打算返回头的信息比如长度，类型。
  //不过好像没必要。如果需要。在这里写就可以了
  void operator()(agip::_AGIP_HEADER_& field, const char* name, size_t size) {
  }
  void operator()(agip::_AGIP_CHECKSUM_& field, const char* name, size_t size) {
  }

};

/////////////////////////struct -> object 
//这边的数据还是网络的大端 。要在这里改成小端
//其实他们提供了对应的类，但我是用元编程动态生成代码， 成员函数
//和结构体的字段对应关系不是很稳定。在这里我就直接自己转化了
struct ToObjectFunctor {
  // ... context data
  Napi::Object& obj;
  template <typename Field, typename Name, typename Size>
  void operator()(Field&& field, Name&& name, Size&& size) {
    //printf("============4 %s [%p]\n", name, &field);
    std::cout << typeid(field).name() << name << "  " << field << "  " << &field <<std::endl;
    obj.Set(name,field);
    //field = (Field)obj.Get(name).ToNumber().Int32Value();
  }
  void operator()(uint16_t field, const char* name, size_t size) {
    obj.Set(name,(uint16_t)htons(field));
  }

  void operator()(uint32_t field, const char* name, size_t size) {
    obj.Set(name,(uint32_t)htonl(field));
  }
  void operator()(int32_t field, const char* name, size_t size) {
    obj.Set(name,htonl(field));
  }



  void operator()(agip::_AGIP_HEADER_& field, const char* name, size_t size) {
    //printf("============5.1 %s [%p]\n", name, &field);
  }
  void operator()(agip::_AGIP_CHECKSUM_& field, const char* name, size_t size) {
    //printf("============5.2 %s [%p]\n", name, &field);
  }

};

typedef void (*BuffToObjectFunctor)(void* pdata, Napi::Object obj);

extern std::map<uint32_t, BuffToObjectFunctor> buffFunctor;

template<typename T, typename S>
inline constexpr void BuffToObject(void* pdata, Napi::Object obj) {
	agip::PSAGIPHeader header = (agip::PSAGIPHeader)pdata;
  T res;
  res.setDecodedPDU((uint8_t*)pdata, htons(header->us_Total_Length));
  //res.showInfo();
  S& _instance = res.template PDUAs<S> (); 
  detail::ForEachField(_instance, 
      ToObjectFunctor { obj } 
  ); 
}




void registFunc(Napi::Env& env, Napi::Object& exports);
void manualRegisFunc(Napi::Env& env, Napi::Object& exports);

#endif //ADDONE_H
