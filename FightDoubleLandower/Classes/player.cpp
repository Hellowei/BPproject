#include "player.h"
#include "LoadAllCardPic.h"
Player::Player(int cardSequeue[],int start,int end)//cardSequeue可能值 1--52
{
///	boundCardAndItem(cardSequeue,start,end);
	trusteeship = true;
	isLandower = false;
	remainCardNum = 13;

}
void Player::boundCardAndItem(int cardSequeue[],int start,int end,int position)
{
	playerPosition = position;
	if (position != 0)
	{
		isLandower = true;
	}
	int j;
	for (int i = start, j = 0; i <= end && j < 13; j++,i++)
	{
		playCard[j] = LoadCardPic::getInstace()->getCCMenuItemCardByNum(cardSequeue[i]);//数组下标可能值 1--52
		cardOnHand[j] = eHAND;
		num[j] = cardSequeue[i];
	
	}
}
void Player::playOneHand(int cardOut[],int five)
{
	int num = 0;
	for (num = 0; num <= five; num++)
	{
		if (cardOut[num] == 0)
		{
			break;
		}else{
			int subScript = this->getCCMenuItemCardNum(cardOut[num]);
			playCard[subScript] = NULL; 
			cardOnHand[subScript] = eDESK;
		}
	}
	remainCardNum -= num;
}
void Player::setCardToPlay(int a[],int num, bool wantStaus)
{
	setWantStaus(wantStaus);
	if(!getWantStaus())
		return;
	remainCardNum -= num;
	for (int i = 0; i < num; i++)
	{
		card[i] = playCard[a[i]]->getCardNum();
		playCard[i] = NULL;
	}
	
}
int Player::getCCMenuItemCardNum(int cardNum)
{
	for (int i = 0; i < 13; i++)
	{
		if (playCard[i]->getCardNum() == cardNum)
		{
			return i;
		}
	}
}
