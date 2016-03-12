/*-------------------------------------------------------------------------------------*/
/*----------------		������Ʒ��Ϣ������������Ʒ��������ʽ	   --------------------*/
/*----------------			ͨ��promotion.json�ļ���ȡ��������     --------------------*/
/*-------------------------------------------------------------------------------------*/


#ifndef  __PROMOTIONALITEMS__
#define __PROMOTIONALITEMS__


#include <string.h>
#include <vector>
#include <map>

using namespace std;



enum PromotionType
{
	NONE = 0,					// ���Ż�
	FREE = 1,					// ����
	DISCOUNT					// ����
};

struct Payment
{
	string strId;							// ID
	PromotionType eType;					// �Żݷ�ʽ
	int iNum;								// ��Ʒ����
	int iFreeNum;							// ���������
	float fPrice;							// ԭ��֧��
	float fPayment;							// ʵ��֧��

	string strReserveInfo;					// �������ַ����ֶ�

	Payment()
	{
		eType = NONE;
	}
};


// ���������
class IPromotion
{
public:
	IPromotion(PromotionType eType){ m_eType = eType; };
	virtual ~IPromotion(){};


	// �ۿ۷�ʽʵ�ֽӿ�
	virtual bool Pay(string id, int num, float price, Payment &res) = 0;

	// �ж�ĳ��id����Ʒ�Ƿ��ڴ���������
	virtual bool IsValid(string id);

	PromotionType GetType(){ return m_eType; }

	// ���ò���
	void SetParams(vector<float> vecParams){ m_vecParams = vecParams; }

	// ��������Żݵ���Ʒ
	bool AddItem(string id);

protected:

	PromotionType m_eType;			// �Ż�����

	vector<float> m_vecParams;		// ������

	map<string, int> m_mapItems;			// ��������Ʒ		first:item id		second: Ԥ��

};


// ���ͻ
class FreePromotion : public IPromotion
{
public:
	FreePromotion();
	virtual ~FreePromotion(){};

	virtual bool Pay(string id, int num, float price, Payment &res);
};

// �ۿۻ
class DiscountPromotion : public IPromotion
{
public:
	DiscountPromotion();
	virtual ~DiscountPromotion(){};

	virtual bool Pay(string id, int num, float price, Payment &res);
};




// ������
class PromotionManager
{
public:
	PromotionManager();
	~PromotionManager();

	virtual bool Read( const char* filename);

	bool Pay(string id, int num, float price, Payment& res);

protected:
	vector<IPromotion*> m_vecPromotion;							// ��������

	map<string, vector<IPromotion*> > m_mapPromotionalItems;		// �����������Ʒ�б�		first:��ƷID		second:��Ʒ�����Ĵ������飬��˳��Ϊ���۷�ʽ���ȼ�
};



#endif