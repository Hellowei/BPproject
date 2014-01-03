#ifndef GAMEDIRECTOR_H
#define GAMEDIRECTOR_H
#include "player.h"
#include "LoadAllCardPic.h"
//游戏控制 导演类 ，控制游戏的开始 结束 打牌规则 获胜等
class Player; 

enum _result
{
	eLOSEALL = -2,
	eLOSE = -1,
	eTIE,
	eWIN,
	eWINALL 
};
struct _playerGameResult
{

	int ID;
	int staus;
};

class GameDirector
{
public:
	static GameDirector* getInstall();
	bool startGame();
	bool reStartGame();
	void resetCardSequeue();//洗牌
	void dealCardSequeue();//发牌
	int getCardNum();//判断上一手牌打的是单张 对 三张 五张
	int autoPlayAI(Player* p,int i);//托管AI
	int getCardTypeNum(int start,int end,int type);//在范围内寻找牌型为type的数量
	void control();//游戏控制方法  主导游戏的进行
	bool isDoubleLandowerType(){return isDoubleLandower;}
	void setGameStaus(int *,int &,bool& staus){ isGameOver = staus;}
	bool isGameStaus(){return isGameOver;}
		Player  *player;
private:
	GameDirector();
	int sort(int cardSequeue[],int start,int end);//对得到的牌进行排序，并返回玩家的身份信息
	void setPlayerOneCard();
	bool isFree(int a);
	bool four;
	bool isNoOneWant(int a);
	void setPlayerCardsFree(int* card,int &num, bool &want,int k);
	void setPlayerTwoCards(int* card,int &num, bool &want);
	void setPlayerThreeCards(int* card,int &num, bool &want);
	void setPlayerFourCards(int* card,int &num, bool &want);
	void setPlayerFiveCards(int* card,int &num, bool &want);
	bool whichOneLarge(int a,int b);
	bool isLargest();
	void updatePlayerCard(Player* p);//游戏主导演获取玩家的出牌信息 
	bool judgeGameOver(Player* p,int num);	//第num个玩家P过了就要判断游戏是否应该结束

	int cardSequeue[53];//只用下标1到52
	int squeue;// 0 1 2 3 分别对应该哪个玩家出牌
	int cards[5];//上个玩家的出牌
	_playerGameResult playerGameResult[4];
	bool isDoubleLandower;
	bool isGameOver;
};
#endif