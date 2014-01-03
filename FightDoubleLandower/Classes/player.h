#ifndef PLAYER_H
#define PLAYER_H

#include "CCMenuItemCard.h"
enum Landower //玩家身份信息
{
	eNOLANDOWER = 0,	//农民
	eLANDOWER_ONE = 1,  //大魅A
	eLANDOWER_THREE = 3,//大魁3
	eDOUBLELANDOWER = 2 //双主
};

enum cardStaus
{
	eDESK = false,
	eHAND = true
};
class Player
{
public:
	Player(int cardSequeue[],int start,int end);
	Player(){trusteeship = true;}
	void boundCardAndItem(int cardSequeue[],int start,int end,int position);
	void playOneHand(int cardOut[],int five);
	bool getIsTrusteeship(){return trusteeship;}
	void updateTrusteeshipStaus(){trusteeship = !trusteeship;}
	int getRemainCardNum(){return remainCardNum;}
	void setWantStaus(bool staus){want = staus;}
	bool getWantStaus(){return want;}
	void setCardZero(){memset(card,0,5);}
	CCMenuItemCard* getMenuItemCardByNum(int num){ return playCard[num];};//输入0--12 返回
	void setCardToPlay(int a[],int num, bool wantStaus);
	int getCardNum(int i){ return card[i];}
	int getPosition(){return playerPosition;}
	bool judgePosition(){return isLandower;}
	int getNum(int i){return num[i];}
	bool hasNum(int i,int &j){return true;};//是否有某个牌  
	
private:
	int remainCardNum;
	CCMenuItemCard* playCard[13]; 
	bool cardOnHand[13];
	int getCCMenuItemCardNum(int cardNum);
	int card[5];
	bool want;//
	bool trusteeship;//托管
	int playerPosition;
	int num[13];// 1--52
	bool isLandower;
};

#endif