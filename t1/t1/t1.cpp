// t1.cpp : �������̨Ӧ�ó������ڵ㡣
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
	cout << "��ѡ�񡾴����������á���json�ļ������ֶ������Զ���������������ļ������Իس�����\n��1�� promotion.json     ��2�� promotion1.json    ��3�� promotion2.json" << endl;
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
		cout << "��ѡ�񡾲����˵�����json�ļ������ֶ������Զ�����������ļ����Իس�����������q�˳�\n��1�� bill.json     ��2�� bill1.json    ��3�� bill2.json" << endl;
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

