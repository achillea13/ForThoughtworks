#include "stdafx.h"

#include "PromotionalItems.h"

#include "json/include/json/json.h"

#include <iostream>
#include <fstream>



bool IPromotion::IsValid(string id)
{
	map<string, int>::iterator it = m_mapItems.find(id);
	if (it == m_mapItems.end())
		return false;
	else
		return true;
}


bool IPromotion::AddItem(string id)
{
	m_mapItems[id] = 0;
	return true;
}

FreePromotion::FreePromotion() : IPromotion(FREE)
{

}

bool FreePromotion::Pay(string id, int num, float price, Payment &res)
{
	// 赠送活动
	int iBaseNum = (int)m_vecParams[0];
	int iFreeNum = (int)m_vecParams[1];
	int iTNum = iBaseNum + iFreeNum;

	int iTimes = num / iTNum;

	if (iTimes <= 0)
	{
		printf("不满足赠送条件\n");
		return false;
	}

	res.iFreeNum = iTimes * iFreeNum;
	res.fPayment = price * (num - res.iFreeNum);
	res.fPrice = price * num;
	res.iNum = num;
	res.strId = id;
	res.eType = m_eType;
	res.strReserveInfo = "买" + to_string(iBaseNum) + "送" + to_string(iFreeNum);

	return true;
}


DiscountPromotion::DiscountPromotion() : IPromotion(DISCOUNT)
{

}

bool DiscountPromotion::Pay(string id, int num, float price, Payment &res)
{
	// 折扣活动
	float fDiscountRate = m_vecParams[0];

	res.fPrice = num * price;
	res.fPayment = res.fPrice * fDiscountRate;
	res.iFreeNum = 0;
	res.iNum = num;
	res.strId = id;

	res.eType = m_eType;

	res.strReserveInfo = to_string(fDiscountRate * 10) + "折";

	return true;
}





PromotionManager::PromotionManager()
{
}

PromotionManager::~PromotionManager()
{
	for (int i = 0; i < (int)m_vecPromotion.size(); i++)
	{
		SAFE_DELETE(m_vecPromotion[i]);
	}
	
}


bool PromotionManager::Read(const char* filename)
{

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
			int type = root[i]["type"].asInt();

			IPromotion* pProm = NULL;


			switch (type)
			{
				case (int)FREE:
				{
					FreePromotion* pTtemp = new FreePromotion();
					pProm = dynamic_cast<IPromotion*>(pTtemp);
				}
				break;

				case (int)DISCOUNT:
				{
					DiscountPromotion* pTemp = new DiscountPromotion();
					pProm = dynamic_cast<IPromotion*>(pTemp);
				}
				break;

				default:
					break;
			}

			if (pProm == NULL)
			{
				printf("没有当前类型的促销活动，促销配置有问题\n");
				continue;
			}

			vector<float> params;
			int paramSize = root[i]["param"].size();
			for (int j = 0; j < paramSize; j++)
			{
				params.push_back(root[i]["param"][j].asFloat());
			}
			pProm->SetParams(params);

			int itemsSize = root[i]["items"].size();
			for (int k = 0; k < itemsSize; k++ )
			{
				string itemID = root[i]["items"][k].asString();
				pProm->AddItem(itemID);

				map<string, vector<IPromotion*> >::iterator it = m_mapPromotionalItems.find(itemID);
				if (it == m_mapPromotionalItems.end())
				{
					// 还没有这条商品的优惠数据
					vector<IPromotion*> vecTemp;
					vecTemp.push_back(pProm);
					m_mapPromotionalItems[itemID] = vecTemp;
				}
				else
				{
					// 已存在此商品优惠数据，继续添加
					it->second.push_back(pProm);
				}
			}


			// 存入优惠配置数据备份
			m_vecPromotion.push_back(pProm);
		}
		return true;
	}

	return false;
}


bool PromotionManager::Pay(string id, int num, float price, Payment& res)
{
	map<string, vector<IPromotion*> >::iterator it = m_mapPromotionalItems.find(id);

	// 无促销活动
	if (it == m_mapPromotionalItems.end())
		return false;
	
	for (int i = 0; i < (int)it->second.size(); i++)
	{
		if (it->second[i]->Pay(id, num, price, res) == true)
			return true;
	}

	// 促销条件都不达标
	return false;

}