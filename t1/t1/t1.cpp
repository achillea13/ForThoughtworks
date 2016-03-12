// t1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


#include "Printer.h"
#include <iostream>

using namespace std;

extern PromotionManager s_PromotionManager;

int _tmain(int argc, _TCHAR* argv[])
{
	Printer printer;

	char promname[256];
	cout << "请选择【促销数据配置】的json文件，或手动输入自定义促销测试数据文件名，以回车结束\n【1】 promotion.json     【2】 promotion1.json    【3】 promotion2.json" << endl;
	cin >> promname;

	if (promname[0] == '1' && promname[1] == '\0')
		s_PromotionManager.Read("promotion.json");
	else if (promname[0] == '2' && promname[1] == '\0')
		s_PromotionManager.Read("promotion1.json");
	else if (promname[0] == '3' && promname[1] == '\0')
		s_PromotionManager.Read("promotion2.json");
	else
		s_PromotionManager.Read(promname);
	

	char billname[256];

	
	while (true)
	{
		cout << "请选择【测试账单】的json文件，或手动输入自定义测试数据文件，以回车结束，键入q退出\n【1】 bill.json     【2】 bill1.json    【3】 bill2.json" << endl;
		cin >> billname;



		if (billname[0] == '1' && billname[1] == '\0')
			printer.Print("bill.json");
		else if (billname[0] == '2' && billname[1] == '\0')
			printer.Print("bill1.json");
		else if (billname[0] == '3' && billname[1] == '\0')
			printer.Print("bill2.json");
		else if ((billname[0] == 'q' || billname[0] == 'Q') && billname[1] == '\0')
			break;
		else
			printer.Print(billname);
	}

	



	return 0;
}

