#ifndef CCMenuItemCard_h
#define CCMenuItemCard_h

#include "cocos2d.h"
#include "HelloWorldScene.h"
USING_NS_CC;//不用在cocos2d类前加cocos2d::
class CCMenuItemCard:public CCNode
{
public:
	CCMenuItemCard(int num):cardNum(num),hadChosed(false){
	char normalImage[16] = {0};
	char selectImage[16] = {0};
		int a = 1;
		int b = 13;
		sprintf(normalImage,"%d%02d.png",a,b);
		PMenuItemImage = CCMenuItemImage::create(normalImage,normalImage,this,menu_selector(CCMenuItemCard::closed));
	}
	~CCMenuItemCard(){if(PMenuItemImage) delete PMenuItemImage; PMenuItemImage = NULL; }
	bool getHadChosed(){return hadChosed;}
	void setChosedStatus(bool status){ hadChosed = status;}
	int getCardNum(){return cardNum;}
	CCMenuItemImage* getMenuItemImage(){return PMenuItemImage;}
	void closed(CCObject* Psender)
	{
		CCMenuItemImage* p = (CCMenuItemImage*)Psender;
		int num = this->getCardNum();	
		if(this->getHadChosed())
		{
			p->setPositionY(p->getPositionY() / 2);
			this->setChosedStatus(false);
		}
		else
		{
			p->setPositionY(p->getPositionY() * 2);
			this->setChosedStatus(true);
		}

//		CCDirector::sharedDirector()->end();
	}

private:
	bool hadChosed;
	int cardNum;
	CCMenuItemImage*  PMenuItemImage;
};


#endif