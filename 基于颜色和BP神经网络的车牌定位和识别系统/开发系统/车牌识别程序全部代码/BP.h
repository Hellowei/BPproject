/////////////////////////////////////////////////////////////////////
typedef int INT;
typedef int BOOL;
typedef double REAL;
typedef char CHAR;

#define FALSE 0
#define TRUE 1
#define NOT !
#define AND &&
#define OR ||

#define MIN(x,y)	((x)>(y)?(y):(x))
#define MAX(x,y)	((x)>(y)?(x):(y))
#define sqr(x)		((x)*(x))

#define LO 0.1
#define HI 0.9
#define BIAS 0.5

#define NUM_LAYERS 3	//神经网络层数
#define N  2	//输入层神经元个数
#define M 2	//输出层神经元个数
#define ONECOLORBIT 256


typedef struct
{
	INT Units;		//神经元个数
	REAL* Output; //输出层
	REAL* Activation; //激活值
	REAL* Error;  //本层误差
	REAL** Weight; //连接权
	REAL** dWeight; //调整值
	REAL** WeightSave; //保存连接权
}LAYER; //神经层结构

typedef struct
{
	LAYER**  Layer;	//神经层各指针
	LAYER*  OutputLayer; //指向输出层指针
	LAYER*  InputLayer; //指向输入层
	REAL Eta;     //冲量参数
	REAL Alpha;   //学习率
	REAL Error;   //总误差
	REAL Epsilon; //控制精度
 	//INT Numlayers;		//网络层数
}NET;   //神经网络结构


////////////	各函数声明    ////
//REAL Simioid(REAL input);		//返回simioid值

void InitializeRandoms();		//产生随机数种子
INT RandomEqualINT(INT low,INT high);	//产生一随机区间内整数
REAL RandomEqualREAL(REAL Low,REAL High); //产生一随机区间大小的小数
void RandomWeights(NET* Net);		//随机产生连接权
void SaveWeights(NET* Net);			//保存连接权
void RestoreWeights(NET* Net);		//恢复连接权
void SaveCharWeights(NET* Net);			//保存识别字符连接权
void RestoreCharWeights(NET* Net);		//恢复识别字符连接权

void GenerateNetwork(NET* Net,int Unit[]);			//分配网络空间,数组表示每个层神经元数目
void SimulateNet(NET* Net,REAL* Input,REAL* Target,BOOL Training,BOOL Protocoling);	//将样本投入运作
void SetInput(NET* Net,REAL* Input,BOOL Protocoling);		//设置输入层输出
void PropagateLayer(NET* Net,LAYER* Lower,LAYER* Upper);	//计算当前层输出，lower为上一层，upper为当前层
void PropagateNet(NET* Net);	//计算整个网络输出
void GetOutput(NET* Net,REAL* Output,BOOL Protocoling);	//获得输出层的输出
void ComputeOutputError(NET *Net,REAL* Target);		//计算输出层的误差
void ComputeNetError(NET *Net);		//计算整个网络各层各结点误差
void BackpropagateLayer(NET* Net,LAYER* Upper,LAYER* Lower);	//下一层误差传到上层，HIGH为下层（其每结点误差已求出），LOWER为上层
void BackpropagateNet(NET* Net);	//整个误差后传
void AdjustWeights(NET* Net);		//调整网络连接权，获得特征
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MAXIMAGEWIDTH	256
#define MAXIMAGEHEIGHT	256
extern HDC hWinDC;
void ReadImage(char *, char *, INT, INT);	//读图像系数
void GetPatternColor();	
void TrainingNet();		//训练网络
void TrainCharNet();    //训练识别字符网络 
BOOL RecognizateColor(int,int ,int,BOOL);	//是否蓝色
void GetPixelBlueColor(HWND);		//		获得的像素颜色
void GetPatternColor();		//从样本文件中得到蓝色样本
void DetectPlateColor(unsigned char A[480][640]);		//对车牌蓝色进行定位,并把精定位后的显示的效果保存到BinaryImage中
BOOL ReadBmpFileHead(HFILE, char *);
void OpenImageFileDlg(char *OPDLTitle);
void Accurate(int* );		//对车牌精定位后信息存在Rectangle中
void Derivative(int *,int *,int);		//求数组的导数	
void CharDevision(int *,int);			//对车牌进行字符分割
void CharImage(int num);
void WriteImage(LPSTR ImageFileName,  char *Image, int wImage, int hImage) ;
int smallest(float*,int);		//返回数组中最小值下标
LRESULT CALLBACK Renamechar(HWND, UINT, WPARAM, LPARAM);
void ReadRaw(LPSTR ImageFileName, char *oImage, int wImage, int hImage); //读取图像信息并保存在Image[][]中
void Recochar();
void ShowLicense(int* lice);


BOOL LicenseBPMessageProc(HWND, UINT, WPARAM, LPARAM);
