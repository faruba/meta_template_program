/*
 * =====================================================================================
 *
 *       Filename:  static_reflection.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2021年01月16日 16时21分42秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

// Static Reflection by BOT Man, 2018
// https://bot-man-jl.github.io/articles/?post=2018/Cpp-Struct-Field-Reflection
// 这里是我仿照的地方。很多地方都是原样copy的。只做了很小的修改。下面是我自己的注释，把我研究他的实现时遇到的问题的理解加了进来。

//模板元编程，如果用函数式编程去理解会容易很多。
//偏特化对应函数调用,而函数调用和prolog中的合一很像。
//类型计算对应函数体
//类型计算的结果对应返回值
//明白了这一层。元编程就比较简单了，剩下的就是c++晦涩的语法。
//最直观的感觉就是写模板的代码容易，编译成功难。
//比如 调用类的成员模板函数template <T> T As() 正常来说 obj.As<int>()这样就可以了，但是如果是偏特化的函数里，就要 obj.template As<int>()。 不写这个template就会报错。

/*
 * 用prolog 计算一个列表的和
 * sum_list([]) -> 0;
 * sum_list([first|rest]) ->
 *   first + sum_list(rest);
 *
 * 是不是很熟悉？下面看c++实现，看StructSchema的实现
 */
#ifndef STATIC_REFLECTION_H_
#define STATIC_REFLECTION_H_

#include <tuple>
#include <type_traits>


// 这时一个模板函数，用来匹配默认情况，相当于
// sum_list([]) -> 0; 这里的0 对应下面 空的tuple
// 然后展开宏 DEFINE_STRUCT_SCHEMA 对应的就是 
// sum_list([first|rest]) ->
//    first + sum_list(rest);
// 这个宏利用偏特化，定义了一个 类型和 结构体对应关系的tuple
template <typename T>
inline constexpr auto StructSchema() {
  return std::make_tuple();
}

namespace detail {
/*
 * 以下是erlang版
 make_index_sequence<N> 会得到一个 0~N-1的序列
 make_index_sequence(MaxNumber) ->lists:reverse(helper(MaxNumber)).
 helper(N) when N > 0 -> [N|helper(N-1)];
 helper(0) -> [].

 ForEachTuple(Tuple, Fn,[Idx|Rest]) ->
   Fn(nth(Idx,Tuple);
   ForEachTuple(Tuple, Fn, Rest)

 ForEachTuple(Tuple, Fn) ->
   ForEachTuple(Tuple, Fn, make_index_sequence(sizeOf(Tuple)))

   std::get<I> 对应 nth
   std::index_sequence 和elang没办法一一对应，
   相当于 解构 外加函数调用。
 */
template <typename Fn, typename Tuple, std::size_t... I>
inline constexpr void ForEachTuple(Tuple&& tuple,
                                   Fn&& fn,
                                   std::index_sequence<I...>) {
  using Expander = int[];
  (void)Expander{0, ((void)fn(std::get<I>(std::forward<Tuple>(tuple))), 0)...};
}

template <typename Fn, typename Tuple>
inline constexpr void ForEachTuple(Tuple&& tuple, Fn&& fn) {
  ForEachTuple(
      std::forward<Tuple>(tuple), std::forward<Fn>(fn),
      std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>{});
}

//一些静态验证函数，也是利用模板的机制，
//在编译期做检查，如果对应的东西没有实现，
//就会因为偏特化失败而报错，
//从而实现了静态检查
template <typename T>
struct is_field_pointer : std::false_type {};

template <typename C, typename T>
struct is_field_pointer<T C::*> : std::true_type {};

template <typename T>
constexpr auto is_field_pointer_v = is_field_pointer<T>::value;


//这里根据穿进来的类型信息 T，获得对应的field 信息
//然后调用lambda 函数，而这个lambda函数也是模板函数，
//会根据偏特化自动匹配。
template <typename T, typename Fn>
inline constexpr void ForEachField(T&& value, Fn&& fn) {
	// 见 DEFINE_STRUCT_SCHEMA 的注释，通过这句，就可以得到对应类型的结构描述的tuple
  constexpr auto struct_schema = StructSchema<std::decay_t<T>>();
  //做了一下静态检验，检查tuple的大小。从使用上看。tuple 应该也是个通过模板元编程实现的。
  static_assert(std::tuple_size<decltype(struct_schema)>::value != 0,
                "StructSchema<T>() for type T should be specialized to return "
                "FieldSchema tuples, like ((&T::field, field_name), ...)");

  //循环遍历tuple中的数据，很像Array.forEach 
  detail::ForEachTuple(struct_schema, [&value, &fn](auto&& field_schema) {
    using FieldSchema = std::decay_t<decltype(field_schema)>;
    static_assert(
        std::tuple_size<FieldSchema>::value >= 2 &&
            detail::is_field_pointer_v<std::tuple_element_t<0, FieldSchema>>,
        "FieldSchema tuple should be (&T::field, field_name)");

	//这个lambda函数通过std::get<int> 获得tuple 中的数据，穿个 fn这个匿名函数，函数接受3个参数，分别是结构体的field，hield的名字，和大小(大小是我加的，方便copy 字符串)
    fn(value.*(std::get<0>(std::forward<decltype(field_schema)>(field_schema))),
       std::get<1>(std::forward<decltype(field_schema)>(field_schema)),
	   sizeof(value.*(std::get<0>(std::forward<decltype(field_schema)>(field_schema)))));
  });
}

}  // namespace detail


/* 
 using _Struct = Struct;                      
 Struct 是宏的参数，所以展开后是个变化的符号，
 通过using 为 这个符号提供了一个临时的名字 _Struct
 这样 DEFINE_STRUCT_FIELD 就可以通过_Struct:: 的方式访问
 结构体的成员函数，
  DEFINE_STRUCT_FIELD 将结构体字段 和一个字符串包装成 tuple
  再通过 DEFINE_STRUCT_SCHEMA 将这个tuple 和 类型绑定起来（偏特化）
  这样当我声明 StructSchema<SAGIPBind> 的时候，就会得到这个结构体结构的tuple
*/
#define DEFINE_STRUCT_SCHEMA(Struct, ...)        \
  template <>                                    \
  inline constexpr auto StructSchema<Struct>() { \
    using _Struct = Struct;                      \
    return std::make_tuple(__VA_ARGS__);         \
  }

#define DEFINE_STRUCT_FIELD(StructField, FieldName) \
  std::make_tuple(&_Struct::StructField, FieldName)

//这个算是 addon.cpp 中  SAGIPJointAuthenExFunc 的通用版 具体含义见 SAGIPJointAuthenExFunc 的注释。宏有连字符，不好写注释

#define FUNCTION_BIND(St) \
  Napi::Value SAGIP##St##Func(const Napi::CallbackInfo& info) {  \
  	Napi::Env env = info.Env();                  \
    if (info.Length() <= 0 || !info[0].IsObject() ||!info[1].IsFunction()) {                                     \
      Napi::TypeError::New(env, "Object and function expected").ThrowAsJavaScriptException();                    \
  	  return env.Null();                         \
    }                                            \
    Napi::Object obj = info[0].ToObject();       \
  	agip::AGIP##St proto;                        \
    proto.setGameID(gameId);                     \
    proto.setGatewayID(gateWayId);               \
  	agip::SAGIP##St& _instance = proto.PDUAs<agip::SAGIP##St>();  \
  	detail::ForEachField(_instance,              \
        GenericFunctor{obj}                      \
    );                                           \
    seq ++;                                      \
    proto.setSequenceID(seq);                    \
  	int result = E_ERROR;                        \
    int32_t data_len = 0;                        \
  	uint8_t pdu[BUFF_LEN];                       \
    result = proto.getDecodedPDU(pdu, BUFF_LEN, &data_len);  \
  	if (S_SUCCESS != result){                    \
  		return env.Null();                       \
    }                                            \
	Napi::Function js_cb = info[1].As<Napi::Function>();     \
    int real_write = 0;                          \
    if(S_SUCCESS != pc->write((const int8_t*)pdu, data_len, real_write)) {                                       \
      	Napi::TypeError::New(env,"send Error").ThrowAsJavaScriptException();                                     \
		return env.Null();                       \
	}                                            \
	napi_status sts;                             \
 	napi_threadsafe_function tsfn;               \
	sts = napi_create_threadsafe_function(env,   \
			js_cb, NULL,                         \
			Napi::String::New(env, "__SendAGIP"),\
			0, 1,                                \
			NULL, NULL, NULL,                    \
			call_js,                             \
			&tsfn);                              \
	if(!catch_err(env, sts)){                    \
      	Napi::TypeError::New(env,"send Error").ThrowAsJavaScriptException();                                     \
		return env.Null();                       \
	}                                            \
	pAddon->tsfns[seq] = tsfn;                    \
	return env.Null();                           \
  } 


//打印二进制数据的，头文件做函数声明，方便其他cpp 使用
void printBuf(const uint8_t* buff, size_t length);



#endif  // STATIC_REFLECTION_H_
