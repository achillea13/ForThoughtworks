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

	// ��ȡ����
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
				// û��ָ������
				id = strValue;
				num = 1;

			}
			else
			{
				// ��ָ������
				id = strValue.substr(0, pos);
				num = atoi(strValue.substr(pos + 1).c_str());
			}

			map<string, int>::iterator it = mapPurchase.find(id);
			if (it == mapPurchase.end())
			{
				// ��δ�����id������
				mapPurchase[id] = num;
			}
			else
			{
				// �������
				it->second += num;
			}
		}
	}

	// ��ʼ��ӡ
	string strInfo1 = "\n\n***<ûǮ׬�̵�>�����嵥***\n";
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

		// ��Ʒ��Ϣ
		Item* pItem = s_ItemManager.GetItem(id);
		if (pItem == NULL)
		{
			printf("error item id %s\n", id);
			continue;
		}

		bool bPromotion = s_PromotionManager.Pay(id, num, pItem->fPrice, payment);
		if ( bPromotion == false)
		{
			// û�вμ��Żݻ
			float fPay = pItem->fPrice * num;
			strInfo1 = strInfo1 + "���� : " + pItem->strName + ", ���� : " + to_string(num) + pItem->strType + ", ���� : " + FLOAT_TO_P2STRING(pItem->fPrice) + "(Ԫ), С�� : " + FLOAT_TO_P2STRING(fPay) + "(Ԫ)\n";
			fTotalPay += fPay;
		
		}
		else
		{
			// ���Żݻ
			switch (payment.eType)
			{
				case FREE:
				{

					strInfo1 = strInfo1 + "���� : " + pItem->strName + ", ���� : " + to_string(num) + pItem->strType + ", ���� : " + FLOAT_TO_P2STRING(pItem->fPrice) + "(Ԫ), С�� : " + FLOAT_TO_P2STRING(payment.fPayment) + "(Ԫ)\n";
					
					if (strInfo2 == "")
					{
						strInfo2 = "--------------------------------------\n" + payment.strReserveInfo + "��Ʒ:\n";
					}
					strInfo2 = strInfo2 + "���� : " + pItem->strName + ", ���� : " + to_string(payment.iFreeNum) + pItem->strType + "\n";

					fTotalPay += payment.fPayment;
					fSave += payment.iFreeNum * pItem->fPrice;
				}
				break;

				case DISCOUNT:
				{
					float fSaveThis = payment.fPrice - payment.fPayment ;
					strInfo1 = strInfo1 + "���� : " + pItem->strName + ", ���� : " + to_string(num) + pItem->strType + ", ���� : " + FLOAT_TO_P2STRING(pItem->fPrice) + "(Ԫ), С�� : " + FLOAT_TO_P2STRING(payment.fPayment) + "(Ԫ), ��ʡ" + FLOAT_TO_P2STRING(fSaveThis) + "(Ԫ)\n";

					fTotalPay += payment.fPayment;
					fSave += fSaveThis;
				}
				break;;

				default:
					break;
			}
			
		}
		
	}

	strInfo3 = strInfo3 + "�ܼ� : " + FLOAT_TO_P2STRING(fTotalPay) + "(Ԫ)\n" + "��ʡ : " + FLOAT_TO_P2STRING(fSave) + "(Ԫ)\n";

	printf("%s%s%s%s\n", strInfo1.c_str(), strInfo2.c_str(), strInfo3.c_str(), strEnd.c_str());

	return true;
}