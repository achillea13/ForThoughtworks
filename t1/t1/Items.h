/*---------------------------------------------------------------------------------------------------*/
/*----------------		商品信息管理类，超市所有在售商品信息数据	   ------------------------------*/
/*----------------			通过items.json文件读取					   ------------------------------*/
/*---------------------------------------------------------------------------------------------------*/


#ifndef __ITEMS__
#define  __ITEMS__



#include <string>
#include <map>


using namespace std;

struct Item
{
	string strId;

	string strName;

	string strType;

	float fPrice;

	Item(){}
	Item(string id, string name, string type, float price)
	{
		strId = id;
		strName = name;
		strType = type;
		fPrice = price;
	}
};


class ItemManager 
{

protected:
	map<string, Item*> m_mapItems;

public:

	ItemManager();
	virtual ~ItemManager();

	virtual bool Read( const char* filename);

	Item* GetItem(string id);
};


#endif