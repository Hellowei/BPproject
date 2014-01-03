#ifndef LOADALLCARDPIC_H
#define LOADALLCARDPIC_H
#include"CCMenuItemCard.h"
const int allCardNum = 52;//不要大小鬼
class LoadCardPic
{
public:
	static LoadCardPic* getInstace();
	CCMenuItemCard* getCCMenuItemCardByNum(int i){return PCard[i];} 
private:
	LoadCardPic();
	~LoadCardPic();
	CCMenuItemCard * PCard[allCardNum+1];
};

#endif