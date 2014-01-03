#ifndef CCMenuItemCard_h
#define CCMenuItemCard_h

#include "cocos2d.h"

class CCMenuItemCard: public cocos2d::CCMenuItem
{
	CCMenuItemCard(int num):cardNum(num),hadChosed(false){}
	~CCMenuItemCard(){}
private:
	bool hadChosed
	int cardNum;
};


#endif