# cjson

������ҫ��json�⣬֮ǰ����Milo Yipʹ��c����д��һ��json�����Ŀ⣬���ʱ��ѧϰc++��˳��ʹ��c++ʵ��һ��json�����⡣Ϊ���������
����Ĭ�ϸ�Ϊobject���͡�


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


## ʹ��

```c++
#include "json.h"
void test_use()
{
	JSON::Json js;
	//����
	js.parse("C:/Users/admin/source/repos/cjson/cjson/test/object.json");
	//��ȡ�������
	JSON::Operator& op = js.get_operator();
	//��ȡstring
	std::cout << op["name"].get_string() << std::endl;;
	//��ȡbool
	if (op["bool"].get_bool())
		std::cout << "true" << std::endl;
	else
		std::cout << "false" << std::endl;
	//��ȡnull
	if (op["null"].is_null())
		std::cout << "this is a null" << std::endl;
	else 		
		std::cout << "this is not a null" << std::endl;
	//��ȡnumber
	std::cout << op["number"].get_number() << std::endl;
	//��ȡ���� ֻ֧��ȫ��Ϊdouble������
	for (double i : op["array_double"].get_array_double())
		std::cout << i << std::endl;
	//��ȡ�����е�ĳԪ�� ʹ������
	std::cout << op["array"][1].get_string() << std::endl;
	
	//���Խ���д��
	//ʹ�� () ����������д�룬��ʱ���Զ�json�������Ĳ�
	op("pi") = 3.14;
	op("happy") = true;
	op("name") = "niels";
	op("nothing") = (void*)nullptr;
	op("answer") =  {"everything",42.0} ;

	//����ʼ���б��Ƚ���Ϊ Json_data ����Json_dataΪ��ʼ������ 
	op("array") = { 1,2,3 };
	//������ֵ
	op("array")(3) = 4.0;
	//�޸�ֵ
	op("array")(1) = 5.0;
	//ֱ���޸�����
	op("array") = { 1,2,3 };
	//�������������������
	op("array")(1) = (void*)nullptr;
	op("array")(2) = true;
	op("array")(3) = false;
	op("array")(4) = (double)100;
	op("array")(5) = "string";
	//����Ҳ���Ե���������ӽ�ȥ
	op("array")(1) = { 1,2,3 };
	//ͬ����޸���������ֵ  ��ʱ���������Ὣ�㲻�壬��������Լ���ʽ�ر�ʾ����
	op("array")(1)((unsigned int)0) = (double)99;
	op("array")(1)((unsigned int)1) = true;
	op("array")(1)((unsigned int)2) = "array in array";
	op("array")(1)((unsigned int)5) = "array in array";
	//�����еĶ���
	//  ����һ����ֵ�� 
	op("array")(1) = { "1",1.0 };
	op("array")(2) = { "2",true };
	op("array")(1) = { {"1",1.0}, {"2",true},{"3",(void*)nullptr},{"4","string"} };
	//Ƕ��
	op("array")(1)("1") = { {"1 in 1",1.0}, {"2",true},{"3",(void*)nullptr},{"4","string"},{"5",{1,2,3,4}} };
	//�޸�
	op("array")(1)("1")("1 in 1") = "1 in 1 ��ֵ��Ϊ�� 9.0";
	//����
	//op("array")(1)("1") = { 1,2,3,4 };
	//����
	//op("array") = (void*) nullptr;

	//object  
	op("object") = { {"obj_1",1.0},{"obj_2",(void*)nullptr},{"obj_3",true},{"obj_4","this is obj_4"},{"obj_5",{1.0,2.0,3.0,4.0}} };

	//ɾ��ĳһ�������е�ĳһ����ֵ�� object::obj_1
	op ("object")-("obj_1");
	//ɾ���������ϵ�object��ֵ��
	//op - ("object");
	
	//ɾ������Ԫ�� 0 Ϊ����
	op("object")("obj_5") - (0);
	op("object")("obj_5") - (0);
	op("object")("obj_5") - (0);
	op("object")("obj_5") - (0);

	js.stringify("C:/Users/admin/source/repos/cjson/cjson/test/set_value.json");

}
```



		
		

		

		

		



