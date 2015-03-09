#include "stdafx.h"
#include "resource.h"
#include <stdlib.h>       
#include<stdio.h>
#include<math.h>
#include <conio.h> 
#include<string.h>
#include <direct.h>
#include <commdlg.h>

#include "BP.h"
int *A;
int *B;
	
#define PIXELH 10
#define PIXELW 10
#define THRESHH 192	//中有车牌大于192*39才被检测
#define THRESHW 39
#define DEVISION 6
#define HDE  25
#define WDE  50
NET Net;		//	网络声明
NET CharNet;	//字符网络
INT CharUnits[NUM_LAYERS]={72,50,34};	//字符网络结点数
char standard[12*6];	//12表示图像的高度，即通常所说的行,对应坐标中的Y
HINSTANCE hInst;	

INT Units[NUM_LAYERS]={N,4,M};	//各神经层神经元结点数
REAL Input[N];
REAL InputTest[N];
REAL Inputchar[12*6];			//字符输入像素点
char BPstring[1024];
BOOL HasTrainNet=FALSE;
//char license[72];
/////////////////////////////////////////////////////////
//		导师信号，从上到下表示蓝色，非蓝色				/
/////////////////////////////////////////////////////////
REAL Target[2][M]={
	{HI,LO},
	{LO,HI}};
	//	从A到9
	REAL CharTarget[34][34]={
		{HI,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO},
		{LO,HI,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO},
		{LO,LO,HI,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO},
		{LO,LO,LO,HI,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO},
		{LO,LO,LO,LO,HI,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO},
		{LO,LO,LO,LO,LO,HI,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO},
		{LO,LO,LO,LO,LO,LO,HI,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO},
		{LO,LO,LO,LO,LO,LO,LO,HI,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO},
		{LO,LO,LO,LO,LO,LO,LO,LO,HI,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO},
		{LO,LO,LO,LO,LO,LO,LO,LO,LO,HI,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO},
		{LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,HI,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO},
		{LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,HI,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO},
		{LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,HI,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO},
		{LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,HI,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO},
		{LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,HI,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO},
		{LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,HI,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO},
		{LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,HI,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO},
		{LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,HI,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO},
		{LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,HI,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO},
		{LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,HI,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO},
		{LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,HI,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO},
		{LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,HI,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO},
		{LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,HI,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO},
		{LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,HI,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO},
		{LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,HI,LO,LO,LO,LO,LO,LO,LO,LO,LO},
		{LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,HI,LO,LO,LO,LO,LO,LO,LO,LO},
		{LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,HI,LO,LO,LO,LO,LO,LO,LO},
		{LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,HI,LO,LO,LO,LO,LO,LO},
		{LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,HI,LO,LO,LO,LO,LO},
		{LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,HI,LO,LO,LO,LO},
		{LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,HI,LO,LO,LO},
		{LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,HI,LO,LO},
		{LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,HI,LO},
		{LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,LO,HI},
		};
int ColorPattern[ONECOLORBIT][ONECOLORBIT];
int ImageWidth,ImageHeight,ImageColor;
extern char *OrgImage;
unsigned char BinaryImage[480][640];//
char ImgDlgFileName[128];
char ImgDlgFileDir[128];
FILE *f;
int Rect[4];
int IsDevision=0;
	int devision[30];
int start,end;
int CHarImage[20][8][16];
float smallerror[34];
int total;
#define LICENUM  20
int license[LICENUM];
int numchar;
//车牌字符
char charnum[34]={'0','1','2','3','4','5','6','7','8','9',
				  'A','B','C','D','E','F','G','H','J','K',
				  'L','M','N','P','Q','R','S','T','U','V',
				  'W','X','Y','Z'};
/************************************************************************************************
*																								*
*   神经网络消息函数																			*
*																								*
************************************************************************************************/
RECT WinRect;
BOOL GetPlateColorID=FALSE;
int iyShowPos=160;
BOOL  LicenseBPMessageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
	int i;
	switch (LOWORD(wParam))
	{
	case IDM_TRAINNET:			//训练网络
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
			GetPatternColor();
 			TrainingNet();
			break;

	case IDM_HERE:		//车牌定位	
	    	OpenImageFileDlg("选择车辆进行定位");
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
			ReadImage(ImgDlgFileName, OrgImage,ImageWidth, ImageHeight);
			DetectPlateColor(BinaryImage);
				break;

	case IDM_ACCURATE:			//精确定位
			Accurate( Rect);
			break;

	case IDM_CHARDEVISION:	//对车牌进行字符分割
			CharDevision(Rect,IsDevision);
			
			break;
	case IDM_CHOSECAR:			//选择车辆进行识别
				OpenImageFileDlg("选择车辆进行定位");
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
			ReadImage(ImgDlgFileName, OrgImage,ImageWidth, ImageHeight);
			DetectPlateColor(BinaryImage);
			Accurate( Rect);
			CharDevision(Rect,IsDevision);
					for( i=2;i < 29; i=i+2){
			if(devision[i] == -1 || devision[i+1] == -1)
				break;
			CharImage(i);
			Recochar();
			}
			ShowLicense(license);

		
		break;

	case IDM_SAVECHARBPWEIGHT:		//保存字符权值
			SaveCharWeights(&CharNet);
			break;
	
	case IDM_RESTORECHARBPWEIGHT:	//恢复字符BP权值
			GenerateNetwork(&CharNet,CharUnits);
			RestoreCharWeights(&CharNet);
			break;
	case IDM_SAVEBPWEIGHT:		//保存权值
			SaveWeights(&Net);
			break;
	
	case IDM_RESTOREBPWEIGHT:	//恢复BP权值
			GenerateNetwork(&Net,Units);
			RestoreWeights(&Net);
			break;
	case IDM_GETCHAR:	//切割的字符作为样本
		OpenImageFileDlg("选择车辆进行定位");
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
			ReadImage(ImgDlgFileName, OrgImage,ImageWidth, ImageHeight);
			DetectPlateColor(BinaryImage);

			Accurate( Rect);

			CharDevision(Rect,IsDevision);
		
		for( i=0;i < 29; i=i+2){
			if(devision[i] == -1 || devision[i+1] == -1)
				break;
			CharImage(i);	//第i个字符
			DialogBox(hInst, (LPCTSTR)IDD_DIALOG1, hWnd, (DLGPROC)Renamechar);

		}
			break;
	case IDM_CHARTRA:
		TrainCharNet() ;
		break;
	case IDM_LICENSE:		//对字符进行识别
		for( i=2;i < 29; i=i+2){
			if(devision[i] == -1 || devision[i+1] == -1)
				break;
			CharImage(i);
			Recochar();

		}
	 ShowLicense(license);


		break;
		default: return(FALSE);
	}
	return(TRUE);
}
//将文件名保存于数组OPdltitle中
void OpenImageFileDlg(char *OPDLTitle)
{
	char FileTitle[100];
	OPENFILENAME ofn;

	memset(&ofn,0,sizeof(ofn));
	ofn.lStructSize=sizeof(OPENFILENAME);
	ofn.hwndOwner=NULL;
	ofn.hInstance=NULL;
	ofn.lpstrFilter=TEXT("bmp files\0*.bmp\0raw files\0*.raw\0All File\0*.*\0\0");
	ofn.lpstrCustomFilter=NULL;
	ofn.nMaxCustFilter=0;
	ofn.nFilterIndex=1;
	ofn.lpstrFile=ImgDlgFileName;
	ofn.nMaxFile=MAX_PATH;
	ofn.lpstrFileTitle=FileTitle;
	ofn.nMaxFileTitle=99;
	ofn.lpstrInitialDir=ImgDlgFileDir;
	ofn.lpstrTitle=OPDLTitle;
	ofn.Flags=OFN_FILEMUSTEXIST;
	ofn.lpstrDefExt="raw";
	ofn.lCustData=NULL;
	ofn.lpfnHook=NULL;
	ofn.lpTemplateName=NULL;
	ImgDlgFileName[0]='\0';
	GetOpenFileName(&ofn); 

	getcwd(ImgDlgFileDir, MAX_PATH);
}

/************************************************************************************************
*																								*
*   读图像文件																					*
*																								*
************************************************************************************************/
void ReadImage(LPSTR ImageFileName, char *oImage, INT wImage, INT hImage) //读取图像信息并保存在Image[][]中， wImage为图像的宽，himage为高
{
	OFSTRUCT of;
	HFILE Image_fp;

	Image_fp = OpenFile(ImageFileName, &of, OF_READ);
	if (Image_fp == HFILE_ERROR) 
	{
		MessageBox(NULL, "打开读图像文件出错！", "出错信息", MB_OK);
		return ;
	}

	if (ReadBmpFileHead(Image_fp, oImage)) return;

	_llseek(Image_fp, 0, 0);
	_lread(Image_fp, oImage, wImage*hImage);
	_lclose(Image_fp);
	ImageWidth = MAXIMAGEWIDTH;
	ImageHeight= MAXIMAGEHEIGHT;
//	ImageColor = GRAYIMAGE;

}
/************************************************************************************************
*																								*
*   从BMP图像中，读出图像头信息，主要包括图像长度和宽度											*
*																								*
************************************************************************************************/
BOOL ReadBmpFileHead(HFILE Image_fp, char *oImage)
{ 
	BITMAPFILEHEADER bfImage;
	BITMAPINFOHEADER biImage;
	INT i, j;
	char tmpPixel;

	_llseek(Image_fp, 0, 0);
	_lread(Image_fp, &bfImage, sizeof(BITMAPFILEHEADER));

	if ((bfImage.bfType != 0x4d42)) {		// "BM"
//		MessageBox(NULL, NULL, "打开bmp文件出错信息", MB_OK);
		_llseek(Image_fp, 0, 0);
		return FALSE;
	}

	_lread(Image_fp, &biImage, sizeof(BITMAPINFOHEADER));

	ImageWidth = biImage.biWidth;
	ImageHeight= biImage.biHeight;
	ImageColor = 1;
	if (biImage.biBitCount == 24) ImageColor = 3;

	_llseek(Image_fp, bfImage.bfOffBits, 0);
	_lread(Image_fp, oImage, biImage.biWidth*biImage.biHeight*ImageColor);
	_lclose(Image_fp);

	if (biImage.biBitCount == 24) {
		for (i=0; i<ImageHeight/2; i++) {
			for (j=0; j<ImageWidth; j++) {
				tmpPixel					 = oImage[i*ImageWidth*3+j*3];
				oImage[i*ImageWidth*3+j*3]	 = oImage[(ImageHeight-i-1)*ImageWidth*3+j*3+2];
				oImage[(ImageHeight-i-1)*ImageWidth*3+j*3+2] = tmpPixel;

				tmpPixel					 = oImage[i*ImageWidth*3+j*3+1];
				oImage[i*ImageWidth*3+j*3+1] = oImage[(ImageHeight-i-1)*ImageWidth*3+j*3+1];
				oImage[(ImageHeight-i-1)*ImageWidth*3+j*3+1] = tmpPixel;

				tmpPixel					 = oImage[i*ImageWidth*3+j*3+2];
				oImage[i*ImageWidth*3+j*3+2] = oImage[(ImageHeight-i-1)*ImageWidth*3+j*3];
				oImage[(ImageHeight-i-1)*ImageWidth*3+j*3] = tmpPixel;
			}
		}
	}

	if (biImage.biBitCount == 8) {
		for (i=0; i<ImageHeight/2; i++) {
			for (j=0; j<ImageWidth; j++) {
				tmpPixel				 = oImage[i*ImageWidth+j];
				oImage[i*ImageWidth+j]	 = oImage[(ImageHeight-i-1)*ImageWidth+j];
				oImage[(ImageHeight-i-1)*ImageWidth+j] = tmpPixel;
			}
		}
	}

	return TRUE;
}

void ReadRaw(LPSTR ImageFileName, char *oImage, int wImage, int hImage) //读取图像信息并保存在Image[][]中raw格式
{
	OFSTRUCT of;
	HFILE Image_fp;

	Image_fp = OpenFile(ImageFileName, &of, OF_READ);
	if (Image_fp == HFILE_ERROR) 
	{
//	MessageBox(NULL, "打开读图像文件出错！", ImageFileName, MB_OK);
		return ;
	}

	_llseek(Image_fp, 0, 0);
	_lread(Image_fp, oImage, wImage*hImage);
	_lclose(Image_fp);

}
//////////////////////////////////////////////////////////////
//		样本颜色中所有蓝色        ////////////////////////////
/////////////////////////////////////////////////////////////
void GetPatternColor()
{
	int i,j,m,Cr,Cb,r,g,b;
	char PatternFileName[128];

	for(i=0;i<ONECOLORBIT;i++)
		for(j=0;j<ONECOLORBIT;j++)
				ColorPattern[i][j]=0;	//0表示非蓝色

		for(m=1;m<=24;m++)
		{
			wsprintf(PatternFileName,"%02d.bmp",m);		//将每个样本读进
			ReadImage(PatternFileName, OrgImage, ImageWidth, ImageHeight);

			for(i=0;i<ImageHeight;i++)
				for(j=0;j<ImageWidth;j++)
				{
					r=(int)((unsigned char)OrgImage[i*3*ImageWidth+j*3+0]);
					g=(int)((unsigned char)OrgImage[i*3*ImageWidth+j*3+1]);
					b=(int)((unsigned char)OrgImage[i*3*ImageWidth+j*3+2]);

					if(((abs(r-g))<50)&&((abs(r-b))<50)&&((abs(g-b))<50))//扩大蓝色范围
						r=g=b=0;
					SetPixel(hWinDC, j+10, i+10, RGB(r, g, b));

					Cr=(int)(128-37.797*r/255-74.203*g/255+112*b/255);
					Cb=(int)(128+112*r/255-93.768*g/255-18.214*b/255);
					if(!(Cr==Cb))
							ColorPattern[Cr][Cb]=1;	//代表蓝色
				}
		}

}
	
////////////////////////////////////////////////////
//	对车牌进行蓝色定位			//////////////////
////////////////////////////////////////////////////
void DetectPlateColor(unsigned char BinaryImage[480][640])
{
	int i,j;
	int r,g,b;
	if(!HasTrainNet){
		MessageBox(NULL,"没有有效权值，无法定位车牌","BP权值",MB_OK);
		 return;
	}
		for(i=0;i<ImageHeight;i++)
				for(j=0;j<ImageWidth;j++)
				{
					r=(int)((unsigned char)OrgImage[i*3*ImageWidth+j*3+0]);
					g=(int)((unsigned char)OrgImage[i*3*ImageWidth+j*3+1]);
					b=(int)((unsigned char)OrgImage[i*3*ImageWidth+j*3+2]);
					if(!RecognizateColor(r,g,b,FALSE))
					{
					SetPixel(hWinDC,j+10,i+10,RGB(0,0,0));
					
					BinaryImage[i][j]=0;		//标志为非蓝色
					}
					else
					{
					BinaryImage[i][j]=255;		//标志为蓝色
					}
					
				}

}
///////////////////////////////////////////////
//   训练BP神经网络，提取样本特征			//
//////////////////////////////////////////////
void TrainingNet()
{
	int PatternID;
	int Cr,Cb,m;
	INT count;
	BOOL Stop;
	REAL Error;

		InitializeRandoms();		//产生随机数种子
		GenerateNetwork(&Net,Units);
	RandomWeights(&Net);
	count=0;
		do{
			 Error=0;
			 Stop=TRUE;
		
			for(Cr=0;Cr<ONECOLORBIT;Cr++)
				for(Cb=0;Cb<ONECOLORBIT;Cb++)
			{
					Input[0]=Cr/300.0+0.1;
					Input[1]=Cb/300.0+0.1;
					PatternID=1;
					if(0!=ColorPattern[Cr][Cb])
						PatternID=0;
			
				
					SimulateNet(&Net, Input,Target[PatternID],FALSE,FALSE);	// 计算误差
				 Error=MAX(Error,Net.Error);
				 Stop=Stop AND (Net.Error<Net.Epsilon);
			}

			 Error=MAX(Error,Net.Epsilon);
			sprintf(BPstring,"训练了%d 次，精度%f , 误差 %f ,完成了%0.0f %%",count,Net.Epsilon,Error,(Net.Epsilon/Error)*100);
				TextOut(hWinDC,120,600,BPstring,strlen(BPstring));
				if(count++>10)
					break;
			if(NOT Stop)
			{
				for(m=0;m<ONECOLORBIT*ONECOLORBIT*5;m++){
					Cr=RandomEqualINT(0,ONECOLORBIT-1);		//随机选取训练
					Cb=RandomEqualINT(0,ONECOLORBIT-1);
						Input[0]=Cr/300.0+0.1;
						Input[1]=Cb/300.0+0.1;
						PatternID=1;

						if(0!=ColorPattern[Cr][Cb])
							PatternID=0;
				
						SimulateNet(&Net, Input,Target[PatternID],TRUE,FALSE);	//将样本投入运作
				}	
		
			}
		}while(NOT Stop);

		MessageBox(NULL,"神经网络已训练完成","神经网络",MB_OK);
		HasTrainNet=TRUE;
}
int he=0;
///////////////////////////////////////////////
//   训练字符库神经网络，提取样本特征			//
//////////////////////////////////////////////
void TrainCharNet()   //训练识别字符网络 
{	
	char* charImage=(char*)malloc(12*6*sizeof(char));
	if(charImage == NULL)
	{
		MessageBox(NULL,"CharImage no memery","charimage",MB_OK);
		return ;
	}
	char strname[100];
    int i,j,m,k;
	int whichchar,charlarge;
	
	InitializeRandoms();		//产生随机数种子
	GenerateNetwork(&CharNet,CharUnits);			
	RandomWeights(&CharNet);	
		
	for(m=0;m<10000;m++){		//
 charlarge=RandomEqualINT(0,33);		//最大33吧
		//		charlarge=m%34;		//最大33吧

	 /*	if(m>9999)
			charlarge=9;//m%34;//RandomEqualINT(0,33);		//最大33吧
			else
			 charlarge=RandomEqualINT(0,33);		//最大33吧*/
		 for(k=0; k < 10; k++){
				 if(k!=0)
					 whichchar=charlarge*10+RandomEqualINT(0,9);		//随机选取训练
				 else
				 	whichchar=charlarge*10;		//随机选取训练
								
					wsprintf(strname,"%03d.raw",whichchar);
					ReadRaw(strname, charImage, 6,12);	
				
					for(i=0; i < 12; i++)
						for(j=0; j < 6; j++)
						{
							if((unsigned char)charImage[i*6+j] > 100)
								Inputchar[i*6+j]=HI;
							else
								Inputchar[i*6+j]=LO;
									
						//	SetPixel(hWinDC, j+720,i+720,RGB((unsigned char)charImage[i*6+j] , (unsigned char)charImage[i*6+j] ,(unsigned char)charImage[i*6+j] ));

						}
						if(0 == m %1000)
						{
						char st[10];
						sprintf(st,"%d ",m);
						TextOut(hWinDC,600,120,st,strlen(st));}
			 	SimulateNet(&CharNet, Inputchar,CharTarget[charlarge],TRUE,FALSE);	//将样本投入运作
					//	MessageBox(NULL,"CH","网",MB_OK);

				}	
		
			}
		

		MessageBox(NULL,"CHAR已训练完成","网络",MB_OK);
		//HasTrainNet=TRUE;
}
/////////////////////////////////////////////
//			识别是否蓝					////
////////////////////////////////////////////
BOOL RecognizateColor(int r,int g,int b,BOOL cShow)
{
	int i,Cr,Cb;

	BOOL IsPlateColor;
	if(FALSE==HasTrainNet){
			TextOut(hWinDC,120,700,"BP未训练",30);
			return FALSE;
	}
		Cr=(int)(128-37.797*r/255-74.203*g/255+112*b/255);
		Cb=(int)(128+112*r/255-93.768*g/255-18.214*b/255);
		InputTest[0]=Cr/300.0+0.1;
		InputTest[1]=Cb/300.0+0.1;
				
		SimulateNet(&Net, InputTest,Target[0],FALSE,FALSE);	//将样本投入运作
		IsPlateColor=TRUE;

			for( i=1;i<=Net.OutputLayer->Units;i++)
			{
				if(fabs(Target[0][i-1]-Net.OutputLayer->Output[i])>0.1)//////////////这里原来是0.1的，我改严格点了
				{	
					IsPlateColor=FALSE;
					break;
				}
			}
			if(cShow)
			{
				sprintf(BPstring,"o1=%f,  o2=%f   r=%d,g=%d ,b=%d   ",Net.OutputLayer->Output[1],Net.OutputLayer->Output[2],r,g,b);
				TextOut(hWinDC,600,120,BPstring,strlen(BPstring));

			}
		
	return IsPlateColor;
}

//对车牌精定位后信息存在Rec中
void Accurate(int* Rec)
{
 	int i,j;
	int a,b,c,d;

	int flag1,flag2;
	a=b=c=d=flag1=flag2=0;
	A=(int *)calloc(ImageHeight,sizeof(int));
	for(i=0;i<ImageHeight;i++)
		 A[i]=0;

	if(NULL==A){
				MessageBox(NULL,"精定位时内存不足","出错",MB_OK);
				return ;
				}
	for(i=0;i<ImageHeight;i++)
				for(j=0;j<ImageWidth;j++)
				{
					if(255==BinaryImage[i][j])
					{
						A[i]++;  
					}
				}
	//显示水平投影	///////////////////////////////////

		for(i=0;i<ImageHeight;i++)
				for(j=0;j<A[i];j++)
				{
					SetPixel(hWinDC,j+10+640,i+10, RGB(250, 0, 255));
				}


	for(i=PIXELH;i<ImageHeight-PIXELH;i++)
	{
			if(A[i] < int(THRESHH/2.5))
				continue;
			else
			{
				for(j = i+1; j < ImageHeight-PIXELH; j++)
				{
					if(A[j] >= int(THRESHH/3.5))// && abs(A[j]-A[j-1]) < THRESHH/2)
						continue;
					else
						if(A[j+4] < int(THRESHH/3.5))
							break;
				}
				if(j-i >= THRESHW)//找到车牌结束
				{
					flag1=1;
					a=i;
					b=j;
					break;
				}
				else
					i=j-1;		//没找到继续
			}
		
		}




		B=(int *)calloc(ImageWidth,sizeof(int));

		if(NULL==B){
				MessageBox(NULL,"精定位时内存不足","出错",MB_OK);
				return ;
				}
					for(j=0;j<ImageWidth;j++)
						B[j]=0;

				for(j=0;j<ImageWidth;j++)
					for(i=a;i<b;i++)
										//	for(i=0;i<ImageHeight;i++)

				{
					if(255==BinaryImage[i][j])
						B[j]++;
					
					
					}

		//竖直投影
		for(i=0;i<ImageWidth;i++)
				for(j=0;j<B[i];j++)
				{
					SetPixel(hWinDC,i+10,j+10+480, RGB(250, 0,255));
				}
					
	for(i=PIXELW;i<ImageWidth-PIXELW;i++)
	{
		if(B[i] < int(THRESHW/1.5))
			continue;
		else
		{
			for(j=i+1; j < ImageWidth-PIXELW; j++)
			{
				if(B[j] >= int(THRESHW/3.5))// && abs(B[j-1]-B[j])<THRESHW/2)
					continue;
				else
					if(B[j+6]< int(THRESHW/4))// && B[j+5] < THRESHW/3 )
						break;
			}
			if(j-i >= int(THRESHH/1.2))
			{
				flag2=1;
				c=i;
				d=j;
				IsDevision=1;		//车牌可分割
				break;
			}
			else
				i=j-1;
		}
	}

	if(b-a >= 101)
	{
		a+=15;
		b-=15;
	}else if(b-a >= 85)
	{  a+=12;
		b-=13;
	}else if(b-a >= 78)
	{
		a+=10;
		b-=10;
	}else if(b-a >= 70){
	a+=8;
	b-=8;}
	else if(b-a >= 55){
	a+=5;
	b-=6;}
	else if(b-a >= 40) {
		a+=2;
		b-=3;
	}else{
		a+=1;
		b-=2;
	}
	b-=1;

	if(1==flag1){
					for(i=0;i<ImageWidth;i++)
					{
						SetPixel(hWinDC, i+10, a+10, RGB(0, 255, 180));
						SetPixel(hWinDC, i+10, b+10, RGB(0, 255, 180));
					}
				}		
	if(1==flag2){
					for(i=0;i<ImageHeight;i++)
					{
						SetPixel(hWinDC,  c+10+5, i+10,RGB(120, 255, 0));
						SetPixel(hWinDC,  d+10,i+10, RGB(120, 255, 0));

					}
				}
	Rect[0]=a;
	Rect[1]=b;
	Rect[2]=c;
	Rect[3]=d;
}
/************************************************************************************************
*																								*
*   将wImage图像写入文件中																		*
*																								*
************************************************************************************************/
void WriteImage(LPSTR ImageFileName,  char *Image, int wImage, int hImage) 
{
	OFSTRUCT of;
	HFILE Image_fp;

	Image_fp = OpenFile(ImageFileName, &of, OF_CREATE);
	_llseek(Image_fp, 0, 0);
	_lwrite(Image_fp, Image, wImage*hImage);
	
	_lclose(Image_fp);

	return ;
}

void CharDevision(int *A,int flag)			//对车牌进行字符分割
{
	int chardevision;

	if(0 == flag)
	{
    	MessageBox(NULL,"无法分割","出错",MB_OK);
		return;
	}
	int a,b,c,d;
	start=	a=A[0];
	end = b=A[1];
	c=A[2]+5;
	d=A[3]-3;
	if(b-a <= 45)
		chardevision = DEVISION -2;
	else
		chardevision = DEVISION;
	int *B;
	B=(int *)malloc((d-c+1)*sizeof(int));
	int i,j, k;
	for(i=0; i <= d-c; i++)
		B[i]=0;

	for(i=c; i <= d; i++)
			for(j=a; j <= b; j++)
			{
				if(0 == BinaryImage[j][i])
						B[i-c]++;
			}

			for(i=0; i < 30; i++)	//字符分割线
				devision[i]=-1;
		for(i=c,k=0; i <= d; i++)
		{
		
			if(B[i-c] < chardevision)
				continue;
			j=i;
			while(j <= d-1 && (B[j-c+1] >= DEVISION*2 || B[j-c] >= DEVISION))//&& B[i-c] >= DEVISION)
				j++;
			if(j-i >= chardevision-2)
			{
				devision[k++]=i;
				devision[k++]=j;
				i=j-1;
			}

		}
				//竖直投影
		for(i=0; i <= d-c; i++)
		{	
			for(j=B[i];j>=0;j--)
					SetPixel(hWinDC,c+i+10,10+480-j, RGB(0,255 ,0));
				}
		total=0;
		for(k=0; k < 30; k++)
		{
			if(-1 != devision[k])	//可分
			{				
				total++;
				for(i=a; i < b; i++)
					SetPixel(hWinDC,  devision[k]+10, i+10,RGB( 255, 0,0));

			}

		}
		total=total/2;
		for(i=0; i < LICENUM; i++)
			 license[i]=0;	//清空其车牌	
		numchar=0;
}
		
//////////////////////////////////////////////////////////////
//			字符缩放								/////////
////////////////////////////////////////////////////////////
void CharImage(int num)
{
	int i,j;
	unsigned char pixelnum;
	int p=num;
float nxtime,nytime;		//新图像是原来的多少倍
if(devision[1+p] == -1 || -1 == devision[0+p])
	return;

nxtime=6.0/(devision[1+p]-devision[0+p]+1);
nytime=12.0/(end-start+1);
int ox,oy;
		for(i = 0; i < 12; i++)
			for(j = 0; j < 6; j++)
			{
				ox=(int)((float)j/nxtime+devision[0+p]);
				oy=(int)((float)i/nytime+start);
				standard[i*6+j]=BinaryImage[oy][ox];
			}
		for(i = 0; i < 12; i++)
			for(j = 0; j < 6; j++)
			{
				pixelnum=(unsigned char)standard[i*6+j];
				SetPixel(hWinDC, j+720,i+720,RGB(pixelnum , pixelnum ,pixelnum ));
			}

}

LRESULT CALLBACK Renamechar(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	char CharName[32];

	switch (message)
	{
	

		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case IDOK:
					GetDlgItemText(hDlg, IDC_CHARNAME, CharName, 32);
				WriteImage(CharName, standard, 6, 12);		
				case IDCANCEL: 
					EndDialog(hDlg, LOWORD(wParam));
					return TRUE;
			}
			break;
	}
    return FALSE;
}
////////////////////////////////
int smallest(float* error,int num)		//返回数组中最小值下标
{
	int i,small=0;
	float min=error[0];
	for(i=1; i < num; i++)
	{
		if(min > error[i])
		{
			min = error[i];
			small = i;
		}
	}
	if(error[small]<0.15)
		return small;
	else
		return -1;
}
void Recochar()
{
   int i,j;
   for(i=0; i < 12; i++)
		for(j=0; j < 6; j++)
		{
			if((unsigned char)standard[i*6+j] > 100)
								Inputchar[i*6+j]=HI;
							else
								Inputchar[i*6+j]=LO;
		}

		for(i=0; i < 34; i++)
		{
	 	SimulateNet(&CharNet, Inputchar,CharTarget[i],FALSE,FALSE);	//将样本投入运作
		smallerror[i]=CharNet.Error;
		}
		int num=smallest(smallerror,34);
		char s[20];
		if(num!=-1)
		license[numchar++]=num;		//字符数目统计

	//	wsprintf(s,"is %dth %c",num,charnum[num]);
	//	MessageBox(NULL,s,"识别",MB_OK);                     逐个字符识别视觉过程
}
/////////////////////
//显示车牌函数  ////
///////////////////////
void ShowLicense(int* lice)
{
	int i;
	char* show;
	show=(char*)malloc(numchar*sizeof(char));
	if(NULL == show)
	{
		MessageBox(NULL,"显示车牌分配内存失败","显示",MB_OK);
		return;
	}
	for(i=0; i < numchar; i++)
	{
		show[i]=charnum[lice[i]];
	}
//wsprintf(s,"is %dth %c",num,charnum[num]);
		MessageBox(NULL,show,"识别结果",MB_OK);

}

/////////////////////////////////////////////////////////////////////////////////////
///////			各函数实现		///////

//产生随机数种子
void InitializeRandoms()	
{
	srand(4711);
}
//产生一随机区间内整数
INT  RandomEqualINT(INT Low,INT High)
{
	return	rand()%(High-Low+1)+Low;
}

//产生一随机区间大小的小数
REAL RandomEqualREAL(REAL Low,REAL High)
{
	return ((REAL)rand()/RAND_MAX)*(High-Low)+Low;
}


//随机产生连接权
void RandomWeights(NET* Net)
{
	INT l,i,j;		
		for(l=1;l<NUM_LAYERS;l++)
			for(i=1;i<=Net->Layer[l]->Units;i++)
				for(j=0;j<=Net->Layer[l-1]->Units;j++)
					{
					Net->Layer[l]->Weight[i][j]=RandomEqualREAL(-0.5,0.5);//随机结点间连接权
					}
}


	
//保存连接权
void SaveWeights(NET* Net)
{
 	INT l,i,j;
	char BPWeightFileName[128];
	char BPWeight[16],BPWeightKeyName[16];
	if(!HasTrainNet)
	{
		MessageBox(NULL,"BP网络没训练，没有权值保存","保存失败",MB_OK);
		return ;
	}
	
	f=fopen("BPNet.txt","a+");	//以追加的写方式打开文件
		if(NULL==f)
		{
			MessageBox(NULL,"创建保存文件失败，BP值没保存","保存失败",MB_OK);
			return;
		}

		int k;
		double s;
		for(i=1;i<NUM_LAYERS;i++)
			for(j=1;j<=Net->Layer[i]->Units;j++)
				for(k=0;k<=Net->Layer[i-1]->Units;k++)
					{
							s=Net->Layer[i]->WeightSave[j][k]=Net->Layer[i]->Weight[j][k];//保存结点间连接权
							fprintf(f,"%lf ",s);
					}
		fclose(f);	//关闭文件
	
	MessageBox(NULL,"宝贵的BP值已保存到BPNet.txt","保存成功",MB_OK);


}


//恢复连接权以便网络运作起来
void RestoreWeights(NET* Net)
{
	f=fopen("BPNet.txt","r");	//以读方式打开文件
	
	if(NULL==f)
	{
			MessageBox(NULL,"没有BP文件，无法恢复","恢复BP权值",MB_OK);
			return;
	}
	int i,j,k;
		double s;		
		for(i=1;i<NUM_LAYERS;i++)
			for(j=1;j<=Net->Layer[i]->Units;j++)
				for(k=0;k<=Net->Layer[i-1]->Units;k++)
					{
							fscanf(f,"%lf ",&s);
							Net->Layer[i]->WeightSave[j][k]=Net->Layer[i]->Weight[j][k]=s;//恢复结点间连接权
					}
		fclose(f); //关闭文件
	HasTrainNet=TRUE;
	MessageBox(NULL,"BP权值已恢复","恢复BP权值",MB_OK);
}

//保存识别字符连接权
void SaveCharWeights(NET* Net)	
{
 	INT l,i,j;
	char BPWeightFileName[128];
	char BPWeight[16],BPWeightKeyName[16];
	f=fopen("CharBPNet.txt","a+");	//以追加的写方式打开文件
		if(NULL==f)
		{
			MessageBox(NULL,"创建保存文件失败，BP值没保存","保存失败",MB_OK);
			return;
		}

		int k;
		double s;
		for(i=1;i<NUM_LAYERS;i++)
			for(j=1;j<=Net->Layer[i]->Units;j++)
				for(k=0;k<=Net->Layer[i-1]->Units;k++)
					{
							s=Net->Layer[i]->WeightSave[j][k]=Net->Layer[i]->Weight[j][k];//保存结点间连接权
							fprintf(f,"%lf ",s);
					}
		fclose(f);	//关闭文件
	
	MessageBox(NULL,"宝贵的BP值已保存到CHARBPNet.txt","保存成功",MB_OK);


}	

//恢复识别字符连接权
void RestoreCharWeights(NET* Net)	
{
	f=fopen("CharBPNet.txt","r");	//以读方式打开文件
	
	if(NULL==f)
	{
			MessageBox(NULL,"没有BP文件，无法恢复","恢复BP权值",MB_OK);
			return;
	}
	int i,j,k;
		double s;		
		for(i=1;i<NUM_LAYERS;i++)
			for(j=1;j<=Net->Layer[i]->Units;j++)
				for(k=0;k<=Net->Layer[i-1]->Units;k++)
					{
							fscanf(f,"%lf ",&s);
							Net->Layer[i]->WeightSave[j][k]=Net->Layer[i]->Weight[j][k]=s;//恢复结点间连接权
					}
		fclose(f); //关闭文件
	MessageBox(NULL,"charBP权值已恢复","恢复BP权值",MB_OK);
}
		
//分配网络空间
void GenerateNetwork(NET* Net,int Unit[])
{
	INT l,i;
	Net->Layer=(LAYER**)calloc(NUM_LAYERS,sizeof(LAYER*));
	for(l=0;l<NUM_LAYERS;l++)
	{
		Net->Layer[l]=(LAYER*)malloc(sizeof(LAYER));

		Net->Layer[l]->Units=Unit[l];
		Net->Layer[l]->Output=(REAL*)calloc(Unit[l]+1,sizeof(REAL));
		Net->Layer[l]->Activation=(REAL*)calloc(Unit[l]+1,sizeof(REAL));
		Net->Layer[l]->Error=(REAL*)calloc(Unit[l]+1,sizeof(REAL));
		Net->Layer[l]->Weight=(REAL**)calloc(Unit[l]+1,sizeof(REAL*));
		Net->Layer[l]->WeightSave=(REAL**)calloc(Unit[l]+1,sizeof(REAL*));
		Net->Layer[l]->dWeight=(REAL**)calloc(Unit[l]+1,sizeof(REAL*));
		Net->Layer[l]->Output[0]=BIAS;             
		if(0!=l)
		{
			for(i=1;i<=Unit[l];i++)
			{
			Net->Layer[l]->Weight[i]=(REAL*)calloc(Unit[l-1]+1,sizeof(REAL));	//此层结点J连接着上一层所有结点
			Net->Layer[l]->WeightSave[i]=(REAL*)calloc(Unit[l-1]+1,sizeof(REAL));
			Net->Layer[l]->dWeight[i]=(REAL*)calloc(Unit[l-1]+1,sizeof(REAL));
			}
		}
	}
	Net->Alpha=0.8;		//冲量参数
	Net->Epsilon=0.005;	//控制精度
	Net->Eta=0.5;		//学习率
	Net->InputLayer=Net->Layer[0];		//输入层分配指针
	Net->OutputLayer=Net->Layer[NUM_LAYERS-1];	//为输出层分配指针

}


//将样本投入运作
void SimulateNet(NET* Net,REAL* Input,REAL* Target,BOOL Training,BOOL Protocoling)
{
	REAL	Output[34];
	SetInput(Net,Input,Protocoling);//获取输入层输出
	PropagateNet(Net);	//计算整个网络输出
	GetOutput(Net,Output,Protocoling);	//获得输出层的输出

	ComputeOutputError(Net,Target);		//计算输出层的误差
	if(Training)
	{
	
		BackpropagateNet(Net);	//整个误差后传
		AdjustWeights(Net);		//调整网络连接权，获得特征
	}
}


//设置输入层输出
void SetInput(NET* Net,REAL* input,BOOL Protocoling)
{
	INT i;
	for(i=1;i<=Net->InputLayer->Units;i++)
	{
		Net->InputLayer->Output[i]=input[i-1];//获得输入层每个结点的输出
	}
}




//计算当前层输出，lower为上一层，upper为当前层
void PropagateLayer(NET* Net,LAYER* Lower,LAYER* Upper)
{
	INT i,j;
	REAL sum;
	for(i=1;i<=Upper->Units;i++)
	{	sum=0;				//下一结点输入重置0
		for(j=0;j<=Lower->Units;j++)
		{
			sum+=Upper->Weight[i][j]*Lower->Output[j];//获得当前结点输入
		}
		Upper->Activation[i]=sum;		//保留激活值,
		Upper->Output[i]=1/(1+exp(-sum));//Simioid(sum);		//返回simioid值
	
	}
}
//计算整个网络输出
void PropagateNet(NET* Net)
{
	INT i;
	for(i=0;i<NUM_LAYERS-1;i++)
	{
		PropagateLayer(Net,Net->Layer[i],Net->Layer[i+1]);	//计算当前层输出
	}

}


//获得输出层的输出
void GetOutput(NET* Net,REAL* Output,BOOL Protocoling)
{
	INT i;
	for(i=1;i<=Net->OutputLayer->Units;i++)
	{
		Output[i-1]=Net->OutputLayer->Output[i];
	}
}


//计算输出层结点输出的误差
void ComputeOutputError(NET *Net,REAL* Target)
{
	INT i;
	REAL Err,Out;
	Net->Error=0;
	for(i=1;i<=Net->OutputLayer->Units;i++)
	{
		Out=Net->OutputLayer->Output[i];
		Err=Target[i-1]-Out;
		Net->OutputLayer->Error[i]=Out*(1-Out)*Err;
		Net->Error+=0.5*sqr(Err);		
	}
}
//下一层误差传到上层，HIGH为下层（其每结点误差已求出），LOWER为上层
void BackpropagateLayer(NET* Net,LAYER* Upper,LAYER* Lower)	
{
	INT i,j;
	REAL Err,Out;
	for(i=1;i<=Lower->Units;i++)
	{
		Out=Lower->Output[i];
		Err=0;		//误差重置
		for(j=1;j<=Upper->Units;j++)
			Err+=Upper->Error[j]*Upper->Weight[j][i];
		Lower->Error[i]=Out*(1-Out)*Err;
	}
	
}


//整个误差后传
void BackpropagateNet(NET* Net)
{
	INT i;
	for(i=NUM_LAYERS-1;i>1;i--)//////////////////////////////////////////////////////////////////////////////////////////////////////////
		BackpropagateLayer(Net,Net->Layer[i],Net->Layer[i-1]);	//当前层误差反向传导
}

//调整网络连接权，获得特征
void AdjustWeights(NET* Net)
{
	INT l,i,j;
	REAL dWeight,Out,Err;
	for(l=1;l<NUM_LAYERS;l++)
		for(i=1;i<=Net->Layer[l]->Units;i++)
			for(j=0;j<=Net->Layer[l-1]->Units;j++)
			{
				
				Out=Net->Layer[l-1]->Output[j];
				Err=Net->Layer[l]->Error[i];			
				dWeight=Net->Layer[l]->dWeight[i][j];
				Net->Layer[l]->Weight[i][j]+=(Net->Alpha*dWeight+Net->Eta*Err*Out);
				Net->Layer[l]->dWeight[i][j]=Net->Eta*Err*Out;
			}	
}

