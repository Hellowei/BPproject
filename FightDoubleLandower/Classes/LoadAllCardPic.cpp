#include"LoadAllCardPic.h"
#include"cocos2d.h"
USING_NS_CC;

LoadCardPic* LoadCardPic::getInstace()
{
	static LoadCardPic* p = NULL;
	if(!p){
		p = new LoadCardPic;
	}
	return p;
}
LoadCardPic:: LoadCardPic()

{
	
	for(int i = 1;i <= allCardNum; i++)
	{
		PCard[i] = new CCMenuItemCard(i); 
	}

}
LoadCardPic:: ~LoadCardPic()
{	
	for(int i = 1;i <= allCardNum; i++)
	{
		if(PCard[i])
		{
			delete PCard[i];
			PCard[i] = NULL;
		}
	}
}

