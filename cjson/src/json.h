#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

namespace JSON
{
	typedef enum
	{
		JSON_TYPE_NULL,
		JSON_TYPE_TRUE,
		JSON_TYPE_FALSE,
		JSON_TYPE_NUMBER,
		JSON_TYPE_ARRAY,
		JSON_TYPE_STRING,
		JSON_TYPE_OBJECT
	}JSON_TYPE;

	typedef struct JSON_DATA
	{
		struct JSON_DATA()
		{
			this->type=JSON_TYPE_NULL;
		}
		//初始化为一个数组
		struct JSON_DATA(std::initializer_list<double> value)
		{
			//注意这里用到了重载的 = 
			std::vector<double> v(value);
			*this = v;
		}
		// { {"1", 1}, { "2",true }, { "3",(void*)nullptr }};
		//key:number 初始化为一个键值对(以object的方式存储，即size为一的map)  
		//op("answer") = { {"1", 1}, { "2",true }, { "3",(void*)nullptr }};
		struct JSON_DATA( std::string key, double number)
		{
			this->type = JSON_TYPE_OBJECT;
			this->object.clear();
			std::string str = key;
			struct JSON_DATA* new_json_data = new struct JSON_DATA;
			new_json_data->type = JSON_TYPE_NUMBER;
			new_json_data->number = number;
			this->object.insert(std::pair<std::string, struct JSON_DATA*>(str, new_json_data));
		}
		// key:bool
		struct JSON_DATA(std::string key, bool value)
		{
			this->type = JSON_TYPE_OBJECT;
			this->object.clear();
			std::string str = key;
			struct JSON_DATA* new_json_data = new struct JSON_DATA;
			if (value)
				new_json_data->type = JSON_TYPE_TRUE;
			else
				new_json_data->type = JSON_TYPE_FALSE;
			this->object.insert(std::pair<std::string, struct JSON_DATA*>(str, new_json_data));
		}
		// key:null
		struct JSON_DATA(std::string key, void*ptr)
		{
			this->type = JSON_TYPE_OBJECT;
			this->object.clear();
			std::string str = key;
			struct JSON_DATA* new_json_data = new struct JSON_DATA;
			if (ptr==nullptr)
				new_json_data->type = JSON_TYPE_NULL;
			this->object.insert(std::pair<std::string, struct JSON_DATA*>(str, new_json_data));
		}
		// key:string
		struct JSON_DATA(std::string key, const char * value)
		{
			this->type = JSON_TYPE_OBJECT;
			this->object.clear();
			std::string str = key;
			struct JSON_DATA* new_json_data = new struct JSON_DATA;
			new_json_data->type = JSON_TYPE_STRING;
			new_json_data->string = value;
			this->object.insert(std::pair<std::string, struct JSON_DATA*>(str, new_json_data));
		}
		// key: Json_data
		struct JSON_DATA(std::string key, struct JSON_DATA value)
		{
			this->type = JSON_TYPE_OBJECT;
			this->object.clear();
			std::string str = key;
			struct JSON_DATA* new_json_data = new struct JSON_DATA;
			*new_json_data = value;
			this->object.insert(std::pair<std::string, struct JSON_DATA*>(str, new_json_data));
		}
		JSON_TYPE type;
		double number;
		std::vector<struct JSON_DATA*> array;
		std::string string;
		std::map<std::string, struct JSON_DATA*> object;
		struct JSON_DATA& operator[](const char* key)
		{
			//获取对象中的某个值
			if (this->type == JSON_TYPE_OBJECT && object.count(key))
			{
				return *(object.at(key));
			}
			else
			{
				Json_data* res = new Json_data();
				res->type = JSON_TYPE_NULL;
				return *res;
			}
		}
		struct JSON_DATA& operator[](int index)
		{
			//获取数组中的值
			if (this->type == JSON_TYPE_ARRAY && index >= 0 && index < array.size())
			{
				return *(array.at(index));
			}
			else
			{
				Json_data* res = new Json_data();
				res->type = JSON_TYPE_NULL;
				return *res;
			}
		}
		//插入一个键值对 key 并返回这个键值对的value的引用（以使用重载的 = 进行赋值） 
		struct JSON_DATA& operator()(const char* key)
		{
			if (this->type == JSON_TYPE_OBJECT)
			{
				if (this->object.count(key))
				{
					return *this->object.at(key);
				}
				else
				{
					Json_data* value = new Json_data();
					//this->object.insert_or_assign(key, value);
					this->object.insert(std::pair<std::string, Json_data*>(key, value));
					return *value;
				}
			}
			else
			{
				Json_data* res = new Json_data();
				res->type = JSON_TYPE_NULL;
				return *res;
			}
		}
		// 插入数组时用于获得下标
		struct JSON_DATA& operator()(unsigned index)
		{
			//'this' is a array
			if (this->type == JSON_TYPE_ARRAY)
			{
				//保证足够的空间
				for (unsigned int i = this->array.size(); i <= index; i++)
				{
					struct JSON_DATA *new_json_data = new struct JSON_DATA;
					this->array.push_back(new_json_data);
				}
				return *this->array.at(index);
			}
		}

		bool get_bool()
		{
			if (type == JSON_TYPE_TRUE)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		bool is_null()
		{
			if (type == JSON_TYPE_NULL)
			{
				return true;
			}
			return false;
		}
		double get_number()
		{
			if (this && this->type == JSON_TYPE_NUMBER)
			{
				return this->number;
			}
			else
			{
				return 0;
			}
		}
		std::string& get_string()
		{
			if (this && this->type == JSON_TYPE_STRING)
			{
				return this->string;
			}
			else
			{
				std::string* str = new std::string("");
				return *str;
			}
		}
		std::vector<double>& get_array_double()
		{
			std::vector<double>* res = new std::vector<double>;
			res->clear();
			bool flag = true;
			for (size_t i = 0; i < array.size(); i++)
			{
				if (array.at(i)->type != JSON_TYPE_NUMBER)
				{
					flag = false;
					return *res;
				}
			}
			for (size_t i = 0; i < array.size(); i++)
			{
				(*res).push_back(array.at(i)->number);
			}
			return *res;

		}

		// operator["1"]= nullptr
		void operator=(void* ptr)
		{
			if (ptr == NULL)
			{
				this->type = JSON_TYPE_NULL;
			}
		}
		// operator["1"]=true;
		void operator=(bool value)
		{
			if (value)
			{
				this->type = JSON_TYPE_TRUE;
			}
			else
			{
				this->type = JSON_TYPE_FALSE;
			}
		}
		// operator["1"]=1.0 
		void operator=(double number)
		{
			this->type = JSON_TYPE_NUMBER;
			this->number = number;
		}
		// operator["1"]="string";
		void operator=(const char* str)
		{
			this->type = JSON_TYPE_STRING;
			this->string.clear();
			this->string.assign(str);
		}
		// perator["array"]=std::vector<double>
		void operator=(std::vector<double>& value)
		{
			//this 为一个 array 为this 赋值
			this->type = JSON_TYPE_ARRAY;
			this->array.clear();
			for (int i = 0; i < value.size(); i++)
			{
				Json_data* new_json_data = new Json_data;
				new_json_data->type = JSON_TYPE_NUMBER;
				new_json_data->number = value.at(i);
				this->array.push_back(new_json_data);
			}
		}
		// perator["array"]=std::initializer_list<double> value
		void operator=(std::initializer_list<double> value)
		{
			this->type = JSON_TYPE_ARRAY;
			this->array.clear();
			for (double number : value)
			{
				Json_data* new_json_data = new Json_data;
				new_json_data->type = JSON_TYPE_NUMBER;
				new_json_data->number = number;
				this->array.push_back(new_json_data);
			}
		}

		//将一系列的键值对插入object中  每一个键值对以object的形式存储，但是只存储一个
		void operator=(std::initializer_list < struct JSON_DATA > value)
		{
			this->type = JSON_TYPE_OBJECT;
			this->object.clear();
			for ( struct JSON_DATA v : value)
			{
				//其实就一个
				for (std::pair< std::string, struct JSON_DATA*> p : v.object)
				{
					std::string str = p.first;
					struct JSON_DATA* new_json_data = new struct JSON_DATA;
					*new_json_data = *p.second;//深拷贝
					this->object.insert(std::pair<std::string, struct JSON_DATA*>(str, new_json_data));
				}
			}
		}

		//深拷贝
		void operator=(struct JSON_DATA* value)
		{
			this->type = value->type;
			this->number = value->number;
			this->string = value->string;
			for (int i = 0; i < value->array.size(); i++)
			{
				struct JSON_DATA* new_json_data = new struct JSON_DATA();
				*new_json_data = value[i];
			}
			for (std::pair< std::string, struct JSON_DATA*> p : value->object)
			{
				std::string str = p.first;
				struct JSON_DATA* new_json_data = new struct JSON_DATA();
				*new_json_data = *p.second;
			}
		}
		//删除运算符
		void operator-(std::string key)
		{
			;
		}


	}Json_data, & Operator;




	class Json
	{
	public:
		void parse(const char* path);
		void stringify(const char* path);
		Operator get_operator();
		Json_data* root = NULL;
		size_t line_num = 0;

	private:


		void skip_white_space(size_t& index, std::string& str);
		void skip_row(size_t& index, std::string& str);
		void parse_token(Json_data* root, size_t& index, std::string& str);
		void parse_token_null(Json_data* root, size_t& index, std::string& str);
		void parse_token_true(Json_data* root, size_t& index, std::string& str);
		void parse_token_false(Json_data* root, size_t& index, std::string& str);
		void parse_token_number(Json_data* root, size_t& index, std::string& str);
		void parse_token_string(Json_data* root, size_t& index, std::string& str);
		void parse_token_array(Json_data* root, size_t& index, std::string& str);
		void parse_token_object(Json_data* root, size_t& index, std::string& str);

		void stringify_token(const Json_data* root, std::string& str);
		void stringify_null(const Json_data* root, std::string& str);
		void stringify_true(const Json_data* root, std::string& str);
		void stringify_false(const Json_data* root, std::string& str);
		void stringify_munber(const Json_data* root, std::string& str);
		void stringify_string(const Json_data* root, std::string& str);
		void stringify_array(const Json_data* root, std::string& str);
		void stringify_object(const Json_data* root, std::string& str);
	};
}