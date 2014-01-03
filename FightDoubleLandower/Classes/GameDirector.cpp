#include "GameDirector.h"
#include<time.h>
static GameDirector* p = NULL;
GameDirector* GameDirector::getInstall()
{
	
	if(p == NULL)
		p = new GameDirector();
	return p;
}
GameDirector::GameDirector()
{
	//memset(winner,-1,4);
	isDoubleLandower = false;
	four = false;
	player = new Player[4];
	resetCardSequeue();
	dealCardSequeue();

}
void GameDirector::dealCardSequeue()
{
	int position;
	position = this->sort(cardSequeue,1,13);
	player[0].boundCardAndItem(cardSequeue,1,13,position);
	position = this->sort(cardSequeue,14,26);
	player[1].boundCardAndItem(cardSequeue,14,26,position);
	position = this->sort(cardSequeue,27,39);
	player[2].boundCardAndItem(cardSequeue,27,39,position);
	position = this->sort(cardSequeue,40,52);
	player[3].boundCardAndItem(cardSequeue,40,52,position);
}
void GameDirector::resetCardSequeue()
{
	for (int i = 1; i <= allCardNum; i++)
	{
		cardSequeue[i] = i;
	}
	int washTime = 10;
	for(int j = 0; j < washTime; j++ )
		for (int i = allCardNum; i > 1; i--)
		{
		    srand((int)time(0));//随机种子使随机数真正随机
			int ran = rand()%i + 1;
			int test = cardSequeue[ran];
			cardSequeue[ran] = cardSequeue[i];
			cardSequeue[i] = test;
		}

}
int GameDirector::getCardNum()
{
	int i = 0;
	for ( ; i < 5; i++)
	{
		if (cards[i] == 0)
		{
			break;
		}
	}
	return i;//返回0---5
}
bool GameDirector::isNoOneWant(int a)
{
	bool free = true;
	for (int i = 0;i < 5; i++)
	{
		if (cards[i] != player[a].getCardNum(i))
		{
			free = false;
			break;
		}
	}
	return free;
}
bool GameDirector::isFree(int a)
{
	int last = a-1;
	if (0 == a)
	{
		last = 3;
	}
	if (player[last].getRemainCardNum()== 0 && this->isNoOneWant(last))//借风
	{
		return true;
	}
	if (player[a].getRemainCardNum() != 0 && this->isNoOneWant(a))//大比我
	{
		return true;
	}
	
}
int GameDirector::autoPlayAI(Player* p,int k)
{
	int num =  this->getCardNum();
	if (p->getRemainCardNum() < num && 1)//上首牌不是我打的 
	{
		p->setWantStaus(false);
	}else
	{
		int menuItemCard[5] = {0};
		bool staus = false;
		int n = 0;
		if (num == 0 || isFree(k))//\我要出个四或大 或借风给我
		{
			//这里想出什么牌就出什么牌  吹啊
			setPlayerCardsFree(menuItemCard,n,staus,k);
		}
		switch (num)
		{
		case 1:
			for (int i = 0;i < 13; i++)
			{
				if (p->getMenuItemCardByNum(i) && whichOneLarge( p->getMenuItemCardByNum(i)->getCardNum(),cards[0]))
				{
					//p->setCardToPlay()
					menuItemCard[0] = i;
					n = 1;
					staus = true;
					break;
				}
			}
			break;
		case 2:
			setPlayerTwoCards(menuItemCard,n,staus);
			break;
		case 3:
			setPlayerThreeCards(menuItemCard,n,staus);
			break;
		case 5:
			setPlayerFiveCards(menuItemCard,n,staus);
			break;


		default:
			break;

		p->setCardToPlay(menuItemCard,n,staus);

		}

	}
	return 0;
}

int GameDirector::sort(int cardSequeue[],int start,int end)
{
	int position = eNOLANDOWER;
	int num = end - start + 1;
	for (int i = start;i < end; i++)
	{
		for (int j = start; j <= end - i; j++ )
		{
			if (whichOneLarge(cardSequeue[j],cardSequeue[j+1]))
			{
				int test = cardSequeue[j];
				cardSequeue[j] = cardSequeue[j+1];
				cardSequeue[j+1] = test;
			}			
		}
	}

	//设定玩家身份
	for (int i = start; i <= end; i++)
	{
		if (cardSequeue[i] == 4)//最小
		{
			squeue = start / 13;
		}
		if (cardSequeue[i] == 40 && 0 == position)
		{
			position = eLANDOWER_ONE;
		}
		if(cardSequeue[i] == 42 && 0 == position)
		{
			position = eLANDOWER_THREE;				
		}
		if((cardSequeue[i] == 42 || cardSequeue[i] == 40)  && 0 != position)
		{
			position = eDOUBLELANDOWER;			
			isDoubleLandower = true;
		}
	}
	return position;
}

bool GameDirector::whichOneLarge(int a,int b)//a>b return true
{
	int i = a % 13 + 1;
	int j = b % 13 + 1;
	if (i == j)
	{
		return a > b;	
	}
	if (4 > j && 4 > i)
	{
		return i > j;

	}
	if (4 <= j && 4 <= i)
	{
		return i > j;
	}
	if (4 <= j && i < 4)
	{
		return true;
	}else
	{
		return false;
	}


}
 
void GameDirector::control()
{
	int playingSequeue = this->squeue;//轮到那个玩家
	bool isGameOver = false;
	do 
	{

		int  a = playingSequeue % 4;

		autoPlayAI(&player[a],a);//玩家针对上个玩家出的牌作了策略准备
		if (player[a].getWantStaus())
		{
			this->updatePlayerCard(&player[a]);
			if (player[a].getRemainCardNum() == 0)//打完这局
			{
				this->judgeGameOver(&player[a],a);
			}
		}
		playingSequeue++;//轮到下一玩家

	} while (!isGameStaus());
	
}

void GameDirector::updatePlayerCard(Player* p)
{
	for (int i = 0; i < 5; i++)
	{
		cards[i] = p->getCardNum(i);
	}
}

bool GameDirector::judgeGameOver(Player* p,int num)
{
	static int i = 0;
	playerGameResult[i].ID = num;
	if(this->isDoubleLandowerType())
	{
		if (eDOUBLELANDOWER == p->getPosition())
		{
			switch (i)
			{
			case 0:
				playerGameResult[0].staus = eWINALL;
				for (int j = 1; j < 4; j++)
				{
					playerGameResult[j].ID = j;
					playerGameResult[j].staus = eLOSEALL;
				}
				break;
			case 1:	
				for (int j = 1; j < 4; j++)
				{
					playerGameResult[j].ID = j;
					playerGameResult[j].staus = eLOSE;
				}
				playerGameResult[0].staus = eWIN;
				playerGameResult[0].ID = num;
				break;
			case 2:
				for (int j = 1; j < 4; j++)
				{
					playerGameResult[j].ID = j;
					playerGameResult[j].staus = eWIN;
				}
				playerGameResult[0].staus = eLOSE;
				playerGameResult[0].ID = num;
				break;
			default:
				break;
			}
			isGameOver = true;
		} 
		else
		{
			if (i == 2)
			{
				for (int j = 1; j < 4; j++)
				{
					playerGameResult[j].ID = j;
					playerGameResult[j].staus = eWINALL;
				}
				playerGameResult[0].staus = eLOSEALL;
				playerGameResult[0].ID = num;
				isGameOver = true;
			}
		}
	}
	else{//这局不存在双主
		if (i == 1)
		{
			if (p->judgePosition() == player[playerGameResult[0].ID].judgePosition())
			{
				isGameOver = true;
				playerGameResult[1].staus = playerGameResult[0].staus = eWINALL;
				playerGameResult[2].staus = playerGameResult[3].staus = eLOSEALL;
				int k = 2;
				for (int j = 0; j < 4; j++)
				{
					if (j != playerGameResult[0].ID && j != playerGameResult[1].ID )
					{
						playerGameResult[k++].ID = j;
					}
				}
			}
		}
		if (i == 2)
		{
			isGameOver = true;
			if (player[playerGameResult[1].ID].judgePosition() == p->judgePosition() )
			{
				for (int j = 0; j < 4; j++)
				{
					playerGameResult[j].staus = eTIE;
					playerGameResult[j].ID = j;
				}
			} 
			else
			{
				playerGameResult[0].staus = playerGameResult[2].staus = eWIN;
				playerGameResult[1].staus = playerGameResult[3].staus = eLOSE;
				int k = 0;
				for(; k < 4; k++)
				{
					if ((k != playerGameResult[0].ID) && (k != playerGameResult[1].ID) && (k != playerGameResult[2].ID))
					{
						break;
					}
				}
				playerGameResult[3].ID = k;
			}
		}

	}

	i++;
	return isGameOver;
}

void GameDirector::setPlayerTwoCards(int* card,int &num, bool &want)
{

}
//void GameDirector::setGameStaus(int* card,int &num, bool &want)

void GameDirector::setPlayerFiveCards(int* card,int &num, bool &want)
{

}
void GameDirector::setPlayerThreeCards(int* card,int &num, bool &want)
{

}

void GameDirector::setPlayerCardsFree(int* card,int &num, bool &want,int k)
{
	int n = 0;
	if (!four)
	{
		four = true;//四没打出去要打含4的牌
		want = true;
		int a,b,c,d,e;
		a = b = c = d = e = -1;
		if (player[k].hasNum(5,a) && player[k].hasNum(6,b) && player[k].hasNum(7,c) && player[k].hasNum(8,d))//45678
		{
			card[0] = player[k].getNum(0);
			card[1] = player[k].getNum(a);
			card[2] = player[k].getNum(b);
			card[3] = player[k].getNum(c);
			card[4] = player[k].getNum(d);
			n = 5;
			want = true;
			return;
		}
		if (player[k].hasNum(3,a) && player[k].hasNum(5,b) && player[k].hasNum(6,c) && player[k].hasNum(7,d))//34567
		{
			card[0] = player[k].getNum(a);
			card[1] = player[k].getNum(0);
			card[2] = player[k].getNum(b);
			card[3] = player[k].getNum(c);
			card[4] = player[k].getNum(d);
			n = 5;
			return;
		}

	}else
	{
			//想怎样打就怎样打  吹我
	}
}