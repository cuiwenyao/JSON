#pragma once
#include "json.h"
unsigned int TEST_NUM;
unsigned int PASS_NUM;

using namespace JSON;

/*
void test_null()
{
	TEST_NUM++;
	Json js;
	js.parse("C:/Users/admin/source/repos/cjson/cjson/test/null.json");
	if (js.root->type == JSON_TYPE_NULL)
	{
		PASS_NUM++;
	}
	std::cout << "pass null" << std::endl;
	return;
}

void test_true()
{
	TEST_NUM++;
	Json js;
	js.parse("C:/Users/admin/source/repos/cjson/cjson/test/true.json");
	if (js.root->type == JSON_TYPE_TRUE)
	{
		PASS_NUM++;
	}
	std::cout << "pass true" << std::endl;
	return;
}
void test_false()
{
	TEST_NUM++;
	Json js;
	js.parse("C:/Users/admin/source/repos/cjson/cjson/test/false.json");
	if (js.root->type == JSON_TYPE_FALSE)
	{
		PASS_NUM++;
	}
	std::cout << "pass false" << std::endl;
	return;
}
void test_number()
{
	TEST_NUM++;
	Json js;
	js.parse("C:/Users/admin/source/repos/cjson/cjson/test/number.json");
	if (js.root->type == JSON_TYPE_NUMBER)
	{
		if (js.root->number == -123.098732)
			PASS_NUM++;
	}
	std::cout << "pass number" << std::endl;
	return;
}
void test_string()
{
	TEST_NUM++;
	Json js;
	js.parse("C:/Users/admin/source/repos/cjson/cjson/test/string.json");
	if (js.root->type == JSON_TYPE_STRING)
	{
		if (!(js.root->string.compare("this is a string test")))
			PASS_NUM++;
	}
	std::cout << "pass string" << std::endl;
	return;
}
void test_array()
{
	TEST_NUM++;
	Json js;
	js.parse("C:/Users/admin/source/repos/cjson/cjson/test/array.json");
	if (js.root->type == JSON_TYPE_ARRAY)
	{
		PASS_NUM++;
	}
	std::cout << "pass array" << std::endl;
	return;
}
void test_object()
{
	TEST_NUM++;
	Json js;
	js.parse("C:/Users/admin/source/repos/cjson/cjson/test/object.json");
	if (js.root->type == JSON_TYPE_OBJECT)
	{
		PASS_NUM++;
	}
	std::cout << "pass object" << std::endl;
	return;
}
void test_stringify()
{
	Json js;
	js.parse("C:/Users/admin/source/repos/cjson/cjson/test/stringify.json");
	js.stringify("C:/Users/admin/source/repos/cjson/cjson/test/stringify.json");
}

*/

void test_get_value()
{
	JSON::Json js;
	js.parse("C:/Users/admin/source/repos/cjson/cjson/test/object.json");
	JSON::Operator op = js.get_operator();
	//ʹ�� [] ��ȡ������������е�ֵ
	std::string name = op["array"][1].get_string();
	double number = op["array"][0].get_number();
	//ʹ�ú�����ȡ bool nullֵ
	bool bool_res = op["4ddd"].get_bool();
	bool null_res = op["2sadsad"].is_null();

	std::vector<double> array_double_res = op["aarray_double"].get_array_double();
	//size���ص���size_t���Ե�size=0��ʹ�� i<=size-1�ͻᱨ����Ϊ��ʱ size-1Ϊ��-1�����ֵ
	for (int i = 0; i < array_double_res.size(); i++)
	{
		std::cout << array_double_res.at(i) << std::endl;
	}
	return;
}

void test_set_value()
{
	JSON::Json js;
	JSON::Operator op = js.get_operator();
	//����ʹ�� ()�ſ��Բ�����ֵ���߸��ľ�ֵ []�����޸��Ѵ��ڵ�ֵ 
	/*	op("obj_string") = "this is a new string";
	op("obj_number") = 1212.0;
	op("obj_true") = true;
	op("obj_fasle") = false;
	op("obj_null") = (void*)nullptr;
	//array
	std::vector<double> value{ 1,2,3,4 };
	op("obj_array") = value;
	//����=ʹ�ó�ʼ���б�
	op("obj_array_2") = { 11,22,33,44};
	//object
	op("obj_obj") = { {"obj_1",{1,2}},{"obj_2",{2,3}} };
	*/

	op("pi") = 3.14;
	op("happy") = true;
	op("name") = "niels";
	op("nothing") = (void*)nullptr;
	//op("answer") =  {"everything",42} ;

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
	op("array")(1) = { {"1",1.0}, {"2",true},{"3",(void*)nullptr},{"4","string"}};
	//Ƕ��
	op("array")(1)("1") = { {"1 in 1",1.0}, {"2",true},{"3",(void*)nullptr},{"4","string"},{"5",{1,2,3,4}} };
	//�޸�
	op("array")(1)("1")("1 in 1") =  "1 in 1 ��ֵ��Ϊ�� 9.0" ;
	//����
	//op("array")(1)("1") = { 1,2,3,4 };
	//����
	//op("array") = (void*) nullptr;

	//object  
	op("object") = { {"obj_1",1.0},{"obj_2",(void*)nullptr},{"obj_3",true},{"obj_4","this is obj_4"},{"obj_5",{1.0,2.0,3.0,4.0}} };

	js.stringify("C:/Users/admin/source/repos/cjson/cjson/test/set_value.json");

}

void test_deep_copy()
{
	JSON::Json co;
	JSON::Json py;
	co.parse("C:/Users/admin/source/repos/cjson/cjson/test/copy.json");
	py.root = new Json_data();
	*py.root = *co.root;
	delete(co.root);
	py.stringify("C:/Users/admin/source/repos/cjson/cjson/test/paste.json");
}
void test()
{
	//test_null();
	//test_true();
	//test_false();
	//test_number();
	//test_array();
	//test_object();
	//test_stringify();
	test_get_value();
	test_set_value();
	test_deep_copy();
}