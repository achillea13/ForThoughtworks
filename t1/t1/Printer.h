/*---------------------------------------------------------------------------------*/
/*--------------------	     账单信息打印机         -------------------------------*/
/*--------------------			通过bill.json文件读取购物信息     -----------------*/
/*---------------------------------------------------------------------------------*/

#ifndef __PRINTER__
#define __PRINTER__

#include <string>
#include <map>
#include <fstream>

#include "PromotionalItems.h"

using namespace  std;


class Printer
{
public:
	Printer();
	~Printer();

	// 打印购买及支付信息
	// filename		：购买数据文件
	bool Print( const char* filename );
};

#endif