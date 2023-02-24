/*
 * =====================================================================================
 *
 *       Filename:  addon.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2021年02月04日 15时54分10秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author: zhaoshu, 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "addon.h"
#include "connection_manager.h"

extern uint32_t gameId;
extern uint32_t gateWayId;
extern uint32_t seq;
extern socketor::Connection* pc;
extern Addon* pAddon;
extern bool catch_err(napi_env env, napi_status status);

static const int BUFF_LEN = sizeof(uint8_t)* agip::AGIP_MAX_BUFFER_SIZE;

//这个函数用来计算 js_cb(也就是js中的回调函数的传入参数）
//
void call_js(napi_env env, napi_value js_cb, void* context, void* data) {
	napi_value undefined;

    napi_get_undefined(env, &undefined); // 创建一个 js 的 undefined
	napi_value obj[1]; //napi_value 其实是个指针
	napi_create_object(env, obj); //这个宏要求传入一个 指针的指针，而obj实际是个数组，就相当于一个地址，其实等同于 &(obj[0]),因此这句话就相当于创建了一个object，并把句柄赋值给obj第一个元素
	Napi::Value v(env, obj[0]); //句柄就是一个指针，想对它进行操作，就要先包装成一个Value
	Napi::Object o = v.ToObject();//再转化为object 包装类
	//上面几行就是为了创建一个只有一个object元素的数组，作为回调的参数,

	//data 就是我们收到的信息，强转为消息头，获得消息类型
	agip::PSAGIPHeader header = (agip::PSAGIPHeader)data;
	//处理大小端
	const uint32_t cmdId =  htonl(header->un_Command_ID);
	//buffFunctor 的注册就是 宏REGIST_MAP_AND_EXPORT 的职责，
	//根据cmdid，知道如何处理data
	auto it = buffFunctor.find(cmdId);
	if(it !=  buffFunctor.end()){
		//找到了之后就调用函数指针，对header 进行处理，具体处理
		//主要的逻辑就是根据DEFINE_STRUCT_SCHEMA宏表达的结构体信息，调用object.Set(key,value)方法给o 赋值。而我是用模板元编程动态生成，逻辑见 manualRegisFunc 中的注释,
		it->second(header, o);
	}
    //data 实在 myevt.cpp onRead 动态申请的。一开始我是想用
	//内存池的。但是会导致非法内存访问。这里就释放，以防止内存泄露
	//还是希望可以用内存池，免得有碎片，但是问题很奇怪，明明地址是对的，但是就是不能访问，不论是在主线程还是socket线程创建内存池都不行。可能是创建异步函数有其他限制。
	free(data);
	//调用回调， obj 就是穿进去的参数
	//用js的代码相当于 js_cb.apply(obj) obj是个数组
    napi_call_function(env,
        undefined, // js 回调的 this 对象
        js_cb,     // js 回调函数句柄
        1,         // js 回调函数接受参数个数
        obj,      // js 回调函数参数数组
        NULL);    // js 回调函数中如果有 retrun，将会被 result 接受到，NULL 代表忽略
}


#define REGIST_MAP(ID, TP) \
	printf("registFunc %d  \n", CMD_##ID); \
  buffFunctor.insert(std::make_pair(CMD_##ID,  BuffToObject<agip::AGIP##TP, agip::SAGIP##TP>))


#define REGIST_MAP_AND_EXPORT(T, ST) \
	REGIST_MAP(T, ST##Res);   \
    exports.Set(Napi::String::New(env, "SAGIP"#ST), Napi::Function::New(env,  SAGIP##ST##Func))

std::map<uint32_t, BuffToObjectFunctor> buffFunctor;

#include "defGen.h"

//有些协议没按套路出牌，主要是他们没有封装成结构体，就要手动绑定了
//


//这里的函数其实 算是 static_reflection.h FUNCTION_BIND 的展开，
Napi::Value SAGIPJointAuthenExFunc(const Napi::CallbackInfo& info) {
	//info 可以理解为js函数的参数，通过[]可以获得对应的参数
	//这里检查了一下参数的类型，
  	Napi::Env env = info.Env();
    if (info.Length() <= 0 || !info[0].IsObject() ||!info[1].IsFunction()) {
      Napi::TypeError::New(env, "Object and function expected").ThrowAsJavaScriptException();
  	  return env.Null();
    }
	//验证通过，就可以获得传入参数
    Napi::Object obj = info[0].ToObject();
	////// 上面是通用代码,不用改，如果需求要求改，FUNCTION_BIND 对应的代码记得也要改，否则自动生成的那部分逻辑不会受到影响
	

	// 实例化对应的协议，设定通用信息。gamid 和gatewayid 是全局数据。在nodeW2.cpp  AsyncConnectSDK 中设定的。 
  	agip::AGIPJointAuthenEx proto;
    proto.setGameID(gameId);
    proto.setGatewayID(gateWayId);
	//在这里写object到c类的转化代码,这个协议比较特殊，只有一对set/getParameter,所以遍历object的key和val 循环调用这个函数就解决了
	//有些类，会提供各种set方法，代码会是类似这样的
	/* 
	 * proto.setUserClass(obj.Get("userClass").ToString().Utf8Value().c_str());
	 * proto.setXXX(obj.Get("xxxx").As<someType>());
	 * ......
	 * proto.setXXX(obj.Get("xxxx").As<someType>());
	 * */
	Napi::Array keys = obj.GetPropertyNames();	
	uint32_t len = keys.Length();
	for(uint32_t idx = 0; idx < len; idx++){
		const char* key = keys.Get(idx).ToString().Utf8Value().c_str();
		const char* val = obj.Get(key).ToString().Utf8Value().c_str();
		proto.setParameter(key, val);
		//printf("setParam %s: %s\n", key, val);
	}
	// 这句是为了方便看生成的对不对，解开注释运行一下就知道了，
	// 打印的信息很漂亮
	//proto.showInfo();
	

	//下面是通用代码,不用改，如果需求要求改，FUNCTION_BIND 对应的代码记得也要改，否则自动生成的那部分逻辑不会受到影响
	//唯一的id，主要是用来对应每一个请求和请求返回回调的
    seq ++;
	//他们提供的接口，会设置头，日本渠道会返回这个SequenceID
    proto.setSequenceID(seq);
  	int result = E_ERROR;
    int32_t data_len = 0;
  	uint8_t pdu[BUFF_LEN];
	//上面的各种set方法，要通过这个函数得到二进制数据
    result = proto.getDecodedPDU(pdu, BUFF_LEN, &data_len);
  	if (S_SUCCESS != result){
  		return env.Null();
    }
	Napi::Function js_cb = info[1].As<Napi::Function>();
    int real_write = 0;
	//把二进制数据写入到socket, 用于发送
    if(S_SUCCESS != pc->write((const int8_t*)pdu, data_len, real_write)) {
      	Napi::TypeError::New(env,"send Error").ThrowAsJavaScriptException();
		return env.Null();
	}
	//如果写成功了， 就可以把js_cb（js传进来的回调）包装成一个异步方法。当 myEvt.cpp onRead 收到信息之后，将用来调用，详见 onread的实现。
	napi_status sts;
 	napi_threadsafe_function tsfn;
	sts = napi_create_threadsafe_function(env,
			js_cb, NULL,
			Napi::String::New(env, "__SendAGIP"),//给hook_api 用的，暂时用不到
			0, 1,
			NULL, NULL, NULL,
			call_js,
			&tsfn);
	if(!catch_err(env, sts)){
      	Napi::TypeError::New(env,"send Error").ThrowAsJavaScriptException();
		return env.Null();
	}
	//请求回调暂时存在tsfns, 收到回调后 见 myEvt.cpp toObject 函数
	pAddon->tsfns[seq] = tsfn;
	return env.Null();
}

//这里是 定义服务器返回结构的地方。虽然发送没按套路来，
//但是接受还是没变， 因此接受处理可以沿用通用逻辑。
//这里先是把agip::SAGIPJointAuthenExRes的成员变量和对应的字符串名字建立联系，（技术细节见 static_reflection.h),然后在  manualRegisFunc 函数中,通过buffFunctor.insert(std::make_pair(agip::CMD_JOINT_AUTHEN_EX_RES,  BuffToObject<agip::AGIPJointAuthenEx, agip::SAGIPJointAuthenExRes>)); 把协议cmdId 和 转换函数绑定在一起。
//这样运行期就可以根据cmdId 来决定如何生成js object了。
DEFINE_STRUCT_SCHEMA(agip::SAGIPJointAuthenExRes,
	DEFINE_STRUCT_FIELD(header, "@header"),
	DEFINE_STRUCT_FIELD(n_Result_Code, "resultCode"),
    DEFINE_STRUCT_FIELD(un_User_ID,"userId"),
    DEFINE_STRUCT_FIELD(c_User_Type,"userType"),
    DEFINE_STRUCT_FIELD(c_Adult_Flag,"adultFlag"),
    DEFINE_STRUCT_FIELD(c_User_Class, "userClass"),
    DEFINE_STRUCT_FIELD(c_User_Role_Count, "userRoleCount"),
    DEFINE_STRUCT_FIELD(n_User_Point, "userPoint"),
    DEFINE_STRUCT_FIELD(un_Promoter_ID,"promoterId"),
    DEFINE_STRUCT_FIELD(un_User_Flag,"userFlag"),
    DEFINE_STRUCT_FIELD(n_CP_Ret_Value,"cpRetValue"),
    DEFINE_STRUCT_FIELD(str_CP_Err_Msg,"cpErrMsg"),
    DEFINE_STRUCT_FIELD(str_Ret_User_Name,"retUserName"),
    DEFINE_STRUCT_FIELD(us_Appendix1,"appendix1"),
    DEFINE_STRUCT_FIELD(us_Appendix2,"appendix2"),
	DEFINE_STRUCT_FIELD(checksum, "&crc"));

//这里是手动注册函数的地方。
void manualRegisFunc(Napi::Env& env, Napi::Object& exports){
	//把 CMD_**RES 和对应的应答结构体关联上。这样就可以通过
	//socket收到的信息来解析为对应的结构体。
	//至于如何解析。其实是用了模板偏特化，把类型信息传递进函数
	//调用 PSAGIPHerder的模板方法PDUAS<>获得对应的结构体，
	//然后利用编译期DEFINE_STRUCT_FIELD声明的结构体信息（而这个信息又是通过extract.js 生成出来的）调用ToObjectFunctor 声明的一系列重载的函数，根据类型调用对应的转义函数，最终完成结构体到object的转变,在细节的实现讲解，见 static_reflection.h的讲解
    buffFunctor.insert(std::make_pair(agip::CMD_JOINT_AUTHEN_EX_RES,  BuffToObject<agip::AGIPJointAuthenEx, agip::SAGIPJointAuthenExRes>));
	//把  SAGIPJointAuthenExFunc函数导入到js中 名为SAGIPJointAuthenEx
    exports.Set(Napi::String::New(env, "SAGIPJointAuthenEx"), Napi::Function::New(env,  SAGIPJointAuthenExFunc));
}
