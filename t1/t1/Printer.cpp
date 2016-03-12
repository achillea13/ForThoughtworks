#include "stdafx.h"
#include "Printer.h"

#include "Items.h"


#include "json/include/json/json.h"

ItemManager s_ItemManager;
PromotionManager s_PromotionManager;


string FLOAT_TO_P2STRING(float f)
{
	char arr[20];
	sprintf_s(arr, "%.2f", (f));
	return string(arr); 
}


Printer::Printer()
{

}


Printer::~Printer()
{
}

bool Printer::Print(const char* filename)
{
	map<string, int> mapPurchase;

	// 读取数据
	Json::Reader reader;
	Json::Value root;
	ifstream is;
	is.open(filename, ios::binary);
	if (reader.parse(is, root))
	{
		int iSize = root.size();
		for (int i = 0; i < iSize; i++)
		{
			string strValue = root[i].asString();
			int pos = strValue.find('-');
			string id;
			int num = 0;
			if (pos < 0)
			{
				// 没有指定数量
				id = strValue;
				num = 1;

			}
			else
			{
				// 有指定数量
				id = strValue.substr(0, pos);
				num = atoi(strValue.substr(pos + 1).c_str());
			}

			map<string, int>::iterator it = mapPurchase.find(id);
			if (it == mapPurchase.end())
			{
				// 还未加入此id的数据
				mapPurchase[id] = num;
			}
			else
			{
				// 添加数量
				it->second += num;
			}
		}
	}

	// 开始打印
	string strInfo1 = "\n\n***<没钱赚商店>购物清单***\n";
	string strInfo2 = "";
	string strInfo3 = "--------------------------\n";
	string strEnd = "***********************\n\n\n";
	float fTotalPay = 0;
	float fSave = 0;
	map<string, int>::iterator iter = mapPurchase.begin();
	for (; iter != mapPurchase.end(); iter++ )
	{
		string id = iter->first;
		int num = iter->second;

		Payment payment;

		// 商品信息
		Item* pItem = s_ItemManager.GetItem(id);
		if (pItem == NULL)
		{
			printf("error item id %s\n", id);
			continue;
		}

		bool bPromotion = s_PromotionManager.Pay(id, num, pItem->fPrice, payment);
		if ( bPromotion == false)
		{
			// 没有参加优惠活动
			float fPay = pItem->fPrice * num;
			strInfo1 = strInfo1 + "名称 : " + pItem->strName + ", 数量 : " + to_string(num) + pItem->strType + ", 单价 : " + FLOAT_TO_P2STRING(pItem->fPrice) + "(元), 小计 : " + FLOAT_TO_P2STRING(fPay) + "(元)\n";
			fTotalPay += fPay;
		
		}
		else
		{
			// 有优惠活动
			switch (payment.eType)
			{
				case FREE:
				{

					strInfo1 = strInfo1 + "名称 : " + pItem->strName + ", 数量 : " + to_string(num) + pItem->strType + ", 单价 : " + FLOAT_TO_P2STRING(pItem->fPrice) + "(元), 小计 : " + FLOAT_TO_P2STRING(payment.fPayment) + "(元)\n";
					
					if (strInfo2 == "")
					{
						strInfo2 = "--------------------------------------\n" + payment.strReserveInfo + "商品:\n";
					}
					strInfo2 = strInfo2 + "名称 : " + pItem->strName + ", 数量 : " + to_string(payment.iFreeNum) + pItem->strType + "\n";

					fTotalPay += payment.fPayment;
					fSave += payment.iFreeNum * pItem->fPrice;
				}
				break;

				case DISCOUNT:
				{
					float fSaveThis = payment.fPrice - payment.fPayment ;
					strInfo1 = strInfo1 + "名称 : " + pItem->strName + ", 数量 : " + to_string(num) + pItem->strType + ", 单价 : " + FLOAT_TO_P2STRING(pItem->fPrice) + "(元), 小计 : " + FLOAT_TO_P2STRING(payment.fPayment) + "(元), 节省" + FLOAT_TO_P2STRING(fSaveThis) + "(元)\n";

					fTotalPay += payment.fPayment;
					fSave += fSaveThis;
				}
				break;;

				default:
					break;
			}
			
		}
		
	}

	strInfo3 = strInfo3 + "总计 : " + FLOAT_TO_P2STRING(fTotalPay) + "(元)\n" + "节省 : " + FLOAT_TO_P2STRING(fSave) + "(元)\n";

	printf("%s%s%s%s\n", strInfo1.c_str(), strInfo2.c_str(), strInfo3.c_str(), strEnd.c_str());

	return true;
}