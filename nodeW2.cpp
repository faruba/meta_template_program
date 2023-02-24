/*
 * =====================================================================================
 *
 *       Filename:  nodeW2.cpp
 *
 *    Description:  这是第4版本的实现了，
 *                  大题流程如下：
 *                    0编译期生成object->struct代码
 *                    1从nodejs 层获得object数据，
 *                    2组装数据并发送， 
 *                    3获得erating服务器返回，
 *                    4解析,根据commandId 解构，
 *                    5生成nodejs object
 *
 *        Version:  1.0
 *        Created:  2021年01月16日 18时11分24秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  faruba, 
 *   Organization:  
 *
 * =====================================================================================
 */
#define NAPI_VERSION 4
#include <stdlib.h>
#include <assert.h>
#include <napi.h>
#include <map>
#include "SysAlgorithmCRC32.h"
#include "./dep/EProtocol/include/AGIPBind.h"
#include "static_reflection.h"
#include "./dep/EBase/include/SysMemory.h"
#include "myEvt.h"
#include "addon.h"


socketor::ConnectionManager* pcm = NULL;
socketor::Connection* pc = NULL;
MyEvt* pMyEvt = NULL;
Addon* pAddon;
uint32_t gameId;
uint32_t gateWayId;
uint32_t seq;




 
/** 调试报错用的 */
bool catch_err(napi_env env, napi_status status) {
    if (status != napi_ok) {
        const napi_extended_error_info* error_info = NULL;
        napi_get_last_error_info(env, &error_info);
        printf("%s\n", error_info->error_message);
		return false;
    }
	return true;
}

static void startNetLoop(napi_env env, void* data) {
	Addon* pAddon = (Addon*) data;
	if(S_SUCCESS == pcm->createConnection(pAddon->ip.c_str(), pAddon->port, pc)) {
    	pMyEvt = new MyEvt(pAddon);
		pc->set_event_handler(pMyEvt);
		pc->connect();
		pcm->run();
	}else{
      Napi::TypeError::New(env, "create connection failed ").ThrowAsJavaScriptException();
	}

}
void onCleanup(void* args) {
	Napi::Env* pEnv = (Napi::Env*) args;
	printf("===========onExit");
  if(pAddon != NULL) {
	  napi_delete_async_work(*pEnv, pAddon->work);
      pAddon->work = NULL;
	  //delete pAddon;
 	  pAddon = NULL;
  }

  if(pcm != NULL) {
    pcm->release();
    //delete pMyEvt;
 	//pMyEvt = NULL;
    delete pcm;
	pcm = NULL;
  }


}


static void whenStopNetLoop(napi_env env, napi_status status, void* data) {
 	//onCleanup(&env);
	pcm->stop();
    delete pMyEvt;
 	pMyEvt = NULL;
}


void afterInitCallJs(napi_env env, napi_value js_cb, void* context, void* data) {
}
/*
 */
Napi::Value AsyncConnectSDK(const Napi::CallbackInfo& info) {
	Napi::Env env = info.Env();
  if (info.Length() <= 1 || !info[0].IsObject()|| !info[1].IsObject()) {
      Napi::TypeError::New(env, "Two Object expected{port, ip},{gameId,gateWayId}").ThrowAsJavaScriptException();
	return env.Null();
  }

  // 目前根据渠道的说法，gameId,gateWayId 是不变的。在这里
  // 初始化全局信息。用于发送
  Napi::Object obj2 = info[1].ToObject();
  gameId = obj2.Get("gameId").ToNumber().Uint32Value();
  seq = obj2.Get("seq").ToNumber().Uint32Value();
  gateWayId = obj2.Get("gateWayId").ToNumber().Uint32Value();

  // 网络库是一个死循环。如果不创建异步线程，就会导致代码没办法执行
  napi_status sts;
  pAddon = new Addon(info);
  sts = napi_create_async_work(env,
		  NULL, //如果有async_hooks 传进去的数据
          Napi::String::New(env, "__CreateSDKWorker"), 
 		  startNetLoop,
		  whenStopNetLoop,
		  pAddon,
		  &(pAddon->work));
  if(!catch_err(env, sts)){
	  delete pAddon;
	  return env.Null();
  }
  sts = napi_queue_async_work(env, pAddon->work);
  if(!catch_err(env, sts)){
	  delete pAddon;
	  return env.Null();
  }
  return env.Null();
}

void printProtocal(agip::SysProtocol& proto, const uint8_t*pdu, int32_t data_len) {
  proto.showInfo(); 
  printf("\n"); 
  printBuf(pdu, data_len); 
}

//FUNCTION_BIND(AGIPBind)


Napi::Value quit(const Napi::CallbackInfo& info) {
	Napi::Env env = info.Env();
    onCleanup(NULL);
    return env.Null();
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
	if(pcm == NULL) {
		pcm = new socketor::ConnectionManager();
    	pcm->initialize();
	}
	napi_add_env_cleanup_hook(env, onCleanup, &env);

	registFunc(env, exports);
  manualRegisFunc(env, exports);
    exports.Set(Napi::String::New(env, "AsyncConnectSDK"), Napi::Function::New(env,  AsyncConnectSDK));
    exports.Set(Napi::String::New(env, "Quit"), Napi::Function::New(env,  quit));
 
  return exports;
}

NODE_API_MODULE(addon, Init)

