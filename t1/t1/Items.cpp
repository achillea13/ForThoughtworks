#include "stdafx.h"
#include "Items.h"

#include "json/include/json/json.h"
#include <iostream>
#include <fstream>

ItemManager::ItemManager()
{
	Read("items.json");

}

ItemManager::~ItemManager()
{
	map<string, Item*>::iterator it = m_mapItems.begin();
	for (; it != m_mapItems.end(); it++ )
	{
		SAFE_DELETE(it->second);
	}
}

bool ItemManager::Read(const char* filename)
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
			string id = root[i]["id"].asString();
			string name = root[i]["name"].asString();
			float price = root[i]["price"].asFloat();
			string type = root[i]["type"].asString();

			map<string, Item*>::iterator it = m_mapItems.find(id);
			if (it != m_mapItems.end())
			{
				printf("该ID商品已存在，取消录入\n");
				continue;
			}
			
			Item* pItem = new Item();
			pItem->strId = id;
			pItem->strName = name;
			pItem->strType = type;
			pItem->fPrice = price;

			m_mapItems[id] = pItem;
		}
	}

	return true;
}

Item* ItemManager::GetItem(string id)
{
	map<string, Item*>::iterator it = m_mapItems.find(id);
	if (it == m_mapItems.end())
		return NULL;
	else
		return it->second;
}