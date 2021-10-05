# cjson

即崔文耀的json库，之前跟着Milo Yip使用c语言写了一个json解析的库，这段时间学习c++，顺便使用c++实现一个json解析库。为方便起见，
本库默认根为object类型。


## feature

good to learn c++ &&
good to operator json


## api

* void parse(const char* path);
* void stringify(const char* path);
* Operator get_operator();
* ()
* []
* -
* bool get_bool()
* bool is_null()
* double get_number()
* std::string& get_string()
* std::vector<double>& get_array_double()


## 使用

```c++
#include "json.h"
void test_use()
{
	JSON::Json js;
	//解析
	js.parse("C:/Users/admin/source/repos/cjson/cjson/test/object.json");
	//获取操作句柄
	JSON::Operator& op = js.get_operator();
	//获取string
	std::cout << op["name"].get_string() << std::endl;;
	//获取bool
	if (op["bool"].get_bool())
		std::cout << "true" << std::endl;
	else
		std::cout << "false" << std::endl;
	//获取null
	if (op["null"].is_null())
		std::cout << "this is a null" << std::endl;
	else 		
		std::cout << "this is not a null" << std::endl;
	//获取number
	std::cout << op["number"].get_number() << std::endl;
	//获取数组 只支持全部为double的数组
	for (double i : op["array_double"].get_array_double())
		std::cout << i << std::endl;
	//获取数组中的某元素 使用索引
	std::cout << op["array"][1].get_string() << std::endl;
	
	//测试进行写入
	//使用 () 代表代表进行写入，此时可以对json进行增改查
	op("pi") = 3.14;
	op("happy") = true;
	op("name") = "niels";
	op("nothing") = (void*)nullptr;
	op("answer") =  {"everything",42.0} ;

	//将初始化列表先解析为 Json_data 再以Json_data为初始化参数 
	op("array") = { 1,2,3 };
	//加入新值
	op("array")(3) = 4.0;
	//修改值
	op("array")(1) = 5.0;
	//直接修改数组
	op("array") = { 1,2,3 };
	//数组添加任意类型数据
	op("array")(1) = (void*)nullptr;
	op("array")(2) = true;
	op("array")(3) = false;
	op("array")(4) = (double)100;
	op("array")(5) = "string";
	//数组也可以当作数组添加进去
	op("array")(1) = { 1,2,3 };
	//同理可修改这个数组的值  有时候会编译器会将搞不清，所以最好自己显式地表示出来
	op("array")(1)((unsigned int)0) = (double)99;
	op("array")(1)((unsigned int)1) = true;
	op("array")(1)((unsigned int)2) = "array in array";
	op("array")(1)((unsigned int)5) = "array in array";
	//数组中的对象
	//  生成一个键值对 
	op("array")(1) = { "1",1.0 };
	op("array")(2) = { "2",true };
	op("array")(1) = { {"1",1.0}, {"2",true},{"3",(void*)nullptr},{"4","string"} };
	//嵌套
	op("array")(1)("1") = { {"1 in 1",1.0}, {"2",true},{"3",(void*)nullptr},{"4","string"},{"5",{1,2,3,4}} };
	//修改
	op("array")(1)("1")("1 in 1") = "1 in 1 的值变为了 9.0";
	//覆盖
	//op("array")(1)("1") = { 1,2,3,4 };
	//覆盖
	//op("array") = (void*) nullptr;

	//object  
	op("object") = { {"obj_1",1.0},{"obj_2",(void*)nullptr},{"obj_3",true},{"obj_4","this is obj_4"},{"obj_5",{1.0,2.0,3.0,4.0}} };

	//删除某一个对象中的某一个键值对 object::obj_1
	op ("object")-("obj_1");
	//删除根对象上的object键值对
	//op - ("object");
	
	//删除数组元素 0 为索引
	op("object")("obj_5") - (0);
	op("object")("obj_5") - (0);
	op("object")("obj_5") - (0);
	op("object")("obj_5") - (0);

	js.stringify("C:/Users/admin/source/repos/cjson/cjson/test/set_value.json");

}
```



		
		

		

		

		



