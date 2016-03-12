/*-------------------------------------------------------------------------------------*/
/*----------------		促销商品信息表，包括促销商品及促销方式	   --------------------*/
/*----------------			通过promotion.json文件读取促销数据     --------------------*/
/*-------------------------------------------------------------------------------------*/


#ifndef  __PROMOTIONALITEMS__
#define __PROMOTIONALITEMS__


#include <string.h>
#include <vector>
#include <map>

using namespace std;



enum PromotionType
{
	NONE = 0,					// 无优惠
	FREE = 1,					// 赠送
	DISCOUNT					// 打折
};

struct Payment
{
	string strId;							// ID
	PromotionType eType;					// 优惠方式
	int iNum;								// 商品数量
	int iFreeNum;							// 活动赠送数量
	float fPrice;							// 原价支付
	float fPayment;							// 实际支付

	string strReserveInfo;					// 保留的字符串字段

	Payment()
	{
		eType = NONE;
	}
};


// 促销活动基类
class IPromotion
{
public:
	IPromotion(PromotionType eType){ m_eType = eType; };
	virtual ~IPromotion(){};


	// 折扣方式实现接口
	virtual bool Pay(string id, int num, float price, Payment &res) = 0;

	// 判断某个id的物品是否在促销名单中
	virtual bool IsValid(string id);

	PromotionType GetType(){ return m_eType; }

	// 设置参数
	void SetParams(vector<float> vecParams){ m_vecParams = vecParams; }

	// 添加享受优惠的商品
	bool AddItem(string id);

protected:

	PromotionType m_eType;			// 优惠类型

	vector<float> m_vecParams;		// 参数集

	map<string, int> m_mapItems;			// 参与活动的物品		first:item id		second: 预留

};


// 赠送活动
class FreePromotion : public IPromotion
{
public:
	FreePromotion();
	virtual ~FreePromotion(){};

	virtual bool Pay(string id, int num, float price, Payment &res);
};

// 折扣活动
class DiscountPromotion : public IPromotion
{
public:
	DiscountPromotion();
	virtual ~DiscountPromotion(){};

	virtual bool Pay(string id, int num, float price, Payment &res);
};




// 管理器
class PromotionManager
{
public:
	PromotionManager();
	~PromotionManager();

	virtual bool Read( const char* filename);

	bool Pay(string id, int num, float price, Payment& res);

protected:
	vector<IPromotion*> m_vecPromotion;							// 促销数据

	map<string, vector<IPromotion*> > m_mapPromotionalItems;		// 参与促销的商品列表		first:商品ID		second:商品包含的打折数组，按顺序为打折方式优先级
};



#endif