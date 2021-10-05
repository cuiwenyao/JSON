#pragma once
//#include "test.hpp"
#include <iostream>
extern void test();
extern unsigned int TEST_NUM;
extern unsigned int PASS_NUM;

int main()
{
	test();
	std::cout << "TESTNUM: " << TEST_NUM << " PASS_NUM: " << PASS_NUM << " pass_rate: " << (double)PASS_NUM / (double)TEST_NUM << std::endl;
}