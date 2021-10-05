#include "json.h"


void JSON::Json::parse(const char* path)
{
	std::ifstream ifile;
	std::string str;
	ifile.exceptions(std::ifstream::badbit | std::ifstream::failbit);
	try
	{
		ifile.open(path);
		std::stringstream str_stream;
		str_stream << ifile.rdbuf();
		str = str_stream.str();
		ifile.close();
	}
	catch(std::ifstream::failure e)
	{
		std::cout << "error to open file " << path << std::endl;
		return;
	}
	size_t index = 0;
	Json_data* root=new Json_data();
	while (index < str.length())
	{
		skip_white_space(index,str);
		parse_token(root, index, str);
	}
	skip_white_space(index, str);
	//现在index指向哪里？
	if (index < str.length())
	{
		std::cout << "syntax error: numlty root" << std::endl;
		exit(EXIT_SUCCESS);
	}
	delete(this->root);
	this->root = root;
}
void JSON::Json::skip_white_space(size_t& index, std::string& str)
{
	while (index < str.length())
	{
		char c = str.at(index);
		if (c == '\n' || c == '\t' || c == '\r' || c == ' ')
		{
			if (c == '\n' || c == '\r')
			{
				this->line_num++;
			}
			index++;
		}
		else
		{
			return;
		}
	}
}
void JSON::Json::skip_row(size_t& index, std::string& str)
{
	while (index < str.length())
	{
		char c = str.at(index);
		index++;
		if (c == '\n')
		{
			this->line_num++;
			break;
		}
	}
}
void JSON::Json::parse_token(Json_data* root, size_t& index, std::string& str)
{
	skip_white_space(index, str);
	if ((index >= str.length()) || (!root))
	{
		return;
	}
	switch (str.at(index))
	{
	case 'n':
		parse_token_null(root, index, str);
		break;
	case 't':
		parse_token_true(root, index, str);
		break;
	case 'f':
		parse_token_false(root, index, str);
		break;
	case '\"':
		parse_token_string(root, index, str);
		break;
	case '[':
		parse_token_array(root, index, str);
		break;
	case '{':
		parse_token_object(root, index, str);
		break;
	default:
		parse_token_number(root, index, str);
		break;
	}
}
void JSON::Json::parse_token_null(Json_data* root, size_t& index, std::string& str)
{
	//index应当指向null，否则报错
	if (index + 3 >= str.length())
	{
		std::cout << "error to parse null" << std::endl;
		exit(EXIT_SUCCESS);
	}
	if (str.compare(index, 4, "null") != 0)
	{
		std::cout << "error to parse null syntax error at line "<<this->line_num << std::endl;
		exit(EXIT_SUCCESS);
	}
	index += 4;
	root->type = JSON_TYPE_NULL;
	//std::cout << "解析出一个 null" << std::endl;
	return;
}
void JSON::Json::parse_token_true(Json_data* root, size_t& index, std::string& str)
{
	//index应当指向true，否则报错
	if (index + 3 >= str.length())
	{
		std::cout << "error to parse true" << std::endl;
		exit(EXIT_SUCCESS);
	}
	if (str.compare(index, 4, "true") != 0)
	{
		std::cout << "error to parse true syntax error at line " << this->line_num << std::endl;
		exit(EXIT_SUCCESS);
	}
	index += 4;
	root->type = JSON_TYPE_TRUE;
	//std::cout << "解析出一个 true" << std::endl;
	return;
}
void JSON::Json::parse_token_false(Json_data* root, size_t& index, std::string& str)
{
	//index应当指向fasle，否则报错
	if (index + 4 >= str.length())
	{
		std::cout << "error to parse false" << std::endl;
		exit(EXIT_SUCCESS);
	}
	if (str.compare(index, 5, "false") != 0)
	{
		std::cout << "error to parse false syntax error at line " << this->line_num << std::endl;
		exit(EXIT_SUCCESS);
	}
	index += 5;
	root->type = JSON_TYPE_FALSE;
	//std::cout << "解析出一个 false" << std::endl;
	return;
}
void JSON::Json::parse_token_number(Json_data* root, size_t& index, std::string& str)
{
	skip_white_space(index, str);
	char* end = NULL;
	double res = strtod(str.c_str()+index, &end);
	if (!end)
	{
		std::cout << "error to parse number syntax error at line " << this->line_num << std::endl;
		exit(EXIT_SUCCESS);
	}
	index = end - str.data();
	root->type = JSON_TYPE_NUMBER;
	root->number = res;
	//std::cout << "解析出一个 number: " << res <<std::endl;
	return;
}
void JSON::Json::parse_token_string(Json_data* root, size_t& index, std::string& str)
{
	if (!(str.at(index) == '\"'))
	{
		std::cout << "error to parse string syntax error at line " << this->line_num << std::endl;
		exit(EXIT_SUCCESS);
	}
	index++;
	size_t string_end = index;
	while (string_end<str.length())
	{
		if (str.at(string_end++) == '\"')
			break;
	}
	if ((string_end >= str.length()) && (str.at(string_end-1) != '\"'))
	{
		std::cout << "error to parse string, expect a \", syntax error at line  " << this->line_num << std::endl;
		exit(EXIT_SUCCESS);
	}
	root->type = JSON_TYPE_STRING;
	root->string.assign(str,index, string_end-index-1);
	index = string_end;
	//std::cout << "解析出一个 string: " << root->string << std::endl;
	return;
}

void JSON::Json::parse_token_array(Json_data* root, size_t& index, std::string& str)
{
	// [1,"str",null,[2,"str2",true],{"3":[1,2,3],"4":false}]
	//首先去除首  [ 
	if (!(str.at(index) == '['))
	{
		std::cout << "error to parse array, expect a [, syntax error at line " << this->line_num << std::endl;
		exit(EXIT_SUCCESS);
	}
	index++;
	root->type = JSON_TYPE_ARRAY;
	skip_white_space(index, str);
	while (index < str.length())
	{
		Json_data* array_element=new Json_data();
		parse_token(array_element, index, str);
		root->array.push_back(array_element);
		//解析出数组中的一个元素，在加入一个逗号即可
		skip_white_space(index, str);
		if (index >= str.length())
		{
			std::cout << "error to parse array, expect a ], syntax error at line " << this->line_num << std::endl;
			exit(EXIT_SUCCESS);
		}
		if (str.at(index) == ']')
		{
			index++;
			break;
		}
		if (str.at(index) == ',')
		{
			index++;
			if (index >= str.length())
			{
				std::cout << "error to parse array,expect a ], syntax error at line " << this->line_num << std::endl;
				exit(EXIT_SUCCESS);
			}
			if ( str.at(index) == ']')
			{
				std::cout << "error to parse array,comma overflow, syntax error at line " << this->line_num << std::endl;
				exit(EXIT_SUCCESS);
			}
		}
	}
	//std::cout << "解析出一个 array: " << std::endl;
	return;
}
void JSON::Json::parse_token_object(Json_data* root, size_t& index, std::string& str)
{
	//首先去除首  {
	if (!(str.at(index) == '{'))
	{
		std::cout << "error to parse object, expect a {, syntax error at line " << this->line_num << std::endl;
		exit(EXIT_SUCCESS);
	}
	index++;
	root->type = JSON_TYPE_OBJECT;
	skip_white_space(index, str);
	while (index < str.length())
	{
		//std::pair < std::string, Json_data*> object_elemet;
		Json_data* key = new Json_data();
		Json_data* value = new Json_data();
		//解析key 
		skip_white_space(index, str);
		parse_token(key, index, str);
		// :
		skip_white_space(index, str);
		if (index >= str.length() || str.at(index) != ':')
		{
			std::cout << "error to parse object, expect a : , syntax error at line " << this->line_num << std::endl;
			exit(EXIT_SUCCESS);
		}
		index++;
		//解析value
		skip_white_space(index, str);
		parse_token(value, index, str);
		//object_elemet.first = key->string;
		//object_elemet.second = value;
		//root->object.insert(object_elemet);
		root->object.insert(std::pair<std::string, Json_data*>(key->string,value));
		//解析出数组中的一个元素，在加入一个逗号即可
		skip_white_space(index, str);
		if (index >= str.length())
		{
			std::cout << "error to parse object, expect a ], syntax error at line " << this->line_num << std::endl;
			exit(EXIT_SUCCESS);
		}
		if (str.at(index) == '}')
		{
			index++;
			break;
		}
		if (str.at(index) == ',')
		{
			index++;
			if (index >= str.length())
			{
				std::cout << "error to parse object,expect a }, syntax error at line " << this->line_num << std::endl;
				exit(EXIT_SUCCESS);
			}
			if (str.at(index) == '}')
			{
				std::cout << "error to parse object,comma overflow, syntax error at line " << this->line_num << std::endl;
				exit(EXIT_SUCCESS);
			}
		}
	}

	//std::cout << "解析出一个 object: " << std::endl;
	return;
}
void JSON::Json::stringify(const char* path)
{
	//将Json数据存储在文本中
//根据root的type进行不同的stringify
	std::string str;
	Json_data* root = this->root;
	this->stringify_token(root, str);
	//将结果放入文本中
	std::fstream ofile;
	ofile.open(path, std::ios::out | std::ios::trunc);
	ofile << str;
}
void JSON::Json::stringify_token(const Json_data* root, std::string& str)
{
	if (root == NULL)
	{
		std:: cout<< "error to stringfy an NULL" << std::endl;
		return;
	}
	switch (root->type)
	{
	case JSON_TYPE_NULL:
		this->stringify_null(root, str);
		break;
	case JSON_TYPE_TRUE:
		this->stringify_true(root, str);
		break;
	case JSON_TYPE_FALSE:
		this->stringify_false(root, str);
		break;
	case JSON_TYPE_NUMBER:
		this->stringify_munber(root, str);
		break;
	case JSON_TYPE_STRING:
		this->stringify_string(root, str);
		break;
	case JSON_TYPE_ARRAY:
		this->stringify_array(root, str);
		break;
	case JSON_TYPE_OBJECT:
		this->stringify_object(root, str);
		break;
	default:
		break;
	}
}
void JSON::Json::stringify_null(const Json_data* root, std::string& str)
{
	str.append("null");
}
void JSON::Json::stringify_true(const Json_data* root, std::string& str)
{
	str.append("true");
}
void JSON::Json::stringify_false(const Json_data* root, std::string& str)
{
	str.append("false");
}
void JSON::Json::stringify_munber(const Json_data* root, std::string& str)
{
	str.append(std::to_string( root->number));
}
void JSON::Json::stringify_string(const Json_data* root, std::string& str)
{
	str.append("\"");
	str.append(root->string);
	str.append("\"");
}
void JSON::Json::stringify_array(const Json_data* root, std::string& str)
{
	// [ 1 , 2 ]
	str.append("[ ");
	for (size_t i = 0; i < root->array.size(); i++)
	{	
		this->stringify_token(root->array.at(i), str);
		str.append(", ");
	}
	str.at(str.size() - 1) = ']';
}
void JSON::Json::stringify_object(const Json_data* root, std::string& str)
{
	str.append("{\n");
	std::map<std::string, Json_data*>::iterator it;

	for (std::pair<std::string, Json_data*>i : root->object)
	{
		str.append("\"");
		str.append(i.first);
		str.append("\" : ");
		this->stringify_token(i.second, str);
		str.append(",\n");
	}
	str.at(str.size() - 2) = '\n';
	str.at(str.size() - 1) = '}';
}


JSON::Json_data& JSON::Json::get_operator()
{
	if (this->root == NULL)
	{
		//默认构造为一个object
		this->root = new Json_data();
		this->root->type = JSON_TYPE_OBJECT;
	}
	return *(this->root);
}

