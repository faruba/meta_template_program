/*
 * =====================================================================================
 *
 *       Filename:  myEvt.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2021年01月29日 20时58分21秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "myEvt.h"
#include "AGIP.h"
#include "static_reflection.h"

//网络使用epoll实现的，onRead 是有收到数据的时候的回调。
//这里的模式是生产者消费者的模式，缓存是个ringbuffer.
//有数据进来，就会往ringbuffer里面写，ringbuffer 有数据
//就会调用这个回调。这个回调负责处理数据。
//所以如果数据已经处理完，一定要通过read 把处理的数据吞掉，
//否则下次回调，数据依旧在。
int32_t MyEvt::onRead(socketor::Connection* connection){
  uint8_t* pdu = (uint8_t*)malloc(sizeof(uint8_t)*1024);
  int32_t realSize = 0;
  //先看看能不能读一个头的数据
  connection->peek(pdu, 20, realSize);
  if(realSize < 20) {
    //如果头没有读到，说明还没收全
    return S_SUCCESS;
  }
  //读到头，直接强转即可。
  agip::PSAGIPHeader pHeader = (agip::PSAGIPHeader) pdu;
  
  //大小端转化。
  uint16_t size = htons(pHeader->us_Total_Length);

  //知道了整个包的大小就试着读。成功的话，ringbuffer对应大小的
  //缓存就会被吞掉。没成功什么都不影响，继续等下一次回调
  int32_t ret = connection->read(pdu, size);
  if(S_SUCCESS != ret) {
    //头读到了，但是整个包还没有读出来
    return S_SUCCESS;
  }

  toObject(pdu, pHeader);
  //用这个函数可以打印buff 检查收到的数据是否正确，
  //用来和日本渠道定位问题。
  //printBuf(pdu, size);
  return S_SUCCESS;
}

void MyEvt::toObject(uint8_t* buff,agip::PSAGIPHeader header){
  //根据addon.cpp  SAGIPJointAuthenExFunc 或者是 static_reflection.h FUNCTION_BIND宏中 绑定的回调，查询到对应的回调函数
  uint32_t seq = htonl(header->un_Sequence_ID);
  auto it = mp_addon->tsfns.find(seq);
  if(it == mp_addon->tsfns.end()) {
    printf("no such SequenceID %d\n", seq);
    return;
  }
  //下面这3步就是调用回调的标准流程。而执行的逻辑看 addon.cpp call_js, 这个是执行回调前用来计算传给js回调函数参数的函数
  napi_acquire_threadsafe_function(it->second);

  //napi_call_threadsafe_function 第一个参数是回调函数的handler，可以在任何线程调用，第二个参数将成为 call_js 第4个参数
  napi_call_threadsafe_function(it->second, buff ,napi_tsfn_blocking);
  napi_release_threadsafe_function(it->second, napi_tsfn_release);
  mp_addon->tsfns.erase(it);
}

