/*---------------------------------------------------------------------------------*/
/*--------------------	     �˵���Ϣ��ӡ��         -------------------------------*/
/*--------------------			ͨ��bill.json�ļ���ȡ������Ϣ     -----------------*/
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

	// ��ӡ����֧����Ϣ
	// filename		�����������ļ�
	bool Print( const char* filename );
};

#endif