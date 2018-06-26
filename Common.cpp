#include "StdAfx.h"
#include "Common.h"

BYTE Get8bitBmpGray(BYTE *buf, DWORD bufSize)//获取8位灰度图像的灰度均值
{
	DWORD sum = 0;
	for (int i = 0; i < bufSize; i++)
	{
		sum += buf[i];
	}
	float avg = 1.0 * sum / bufSize + 0.5;
	return (BYTE)avg;
}

int WriteHead(unsigned char* Output,unsigned char *Input,int nWidth,int nHeight)
{
	int IMAGE_X=nWidth,IMAGE_Y=nHeight;
	unsigned char head[1078]={
		/***************************/
		//file header
		0x42,0x4d,//file type 
			//0x36,0x6c,0x01,0x00, //file size***
			0x0,0x0,0x0,0x00, //file size***
			0x00,0x00, //reserved
			0x00,0x00,//reserved
			0x36,0x4,0x00,0x00,//head byte***
			/***************************/
			//infoheader
			0x28,0x00,0x00,0x00,//struct size
			
			//0x00,0x01,0x00,0x00,//map width*** 
			0x00,0x00,0x0,0x00,//map width*** 
			//0x68,0x01,0x00,0x00,//map height***
			0x00,0x00,0x00,0x00,//map height***
			
			0x01,0x00,//must be 1
			0x08,0x00,//color count***
			0x00,0x00,0x00,0x00, //compression
			//0x00,0x68,0x01,0x00,//data size***
			0x00,0x00,0x00,0x00,//data size***
			0x00,0x00,0x00,0x00, //dpix
			0x00,0x00,0x00,0x00, //dpiy
			0x00,0x00,0x00,0x00,//color used
			0x00,0x00,0x00,0x00,//color important
			
	};
	
	long num;
	num=IMAGE_X; head[18]= num & 0xFF;
	num=num>>8;  head[19]= num & 0xFF;
	num=num>>8;  head[20]= num & 0xFF;
	num=num>>8;  head[21]= num & 0xFF;
	
	
	num=IMAGE_Y; head[22]= num & 0xFF;
	num=num>>8;  head[23]= num & 0xFF;
	num=num>>8;  head[24]= num & 0xFF;
	num=num>>8;  head[25]= num & 0xFF;
	
	int i,j;
	
	j=0;
	for (i=54;i<1078;i=i+4)
	{
		head[i]=head[i+1]=head[i+2]=j; 
		head[i+3]=0;
		j++;
	} 
	
	memcpy(Output,head,sizeof(byte)*1078);
	for(  i=0;i<=IMAGE_Y-1; i++ )
	{
		memcpy(Output+1078*sizeof(char)+(i)*(IMAGE_X),Input+i*IMAGE_X,IMAGE_X*sizeof(char));
	}
	
	return 1;
}
 


BITMAPFILEHEADER  bf;

BITMAPINFOHEADER bi;
#define WIDTHBYTES(i)    ((i+31)/32*4)
BOOL LoadBmpFile (HWND hWnd,unsigned char *BMPBuf,int nWidth,int nHeight)

{   	
	HFILE hf=NULL; //文件句柄
	
	//指向BITMAPINFOHEADER结构的指针	
	LPBITMAPINFOHEADER    lpImgData; 
	
	LOGPALETTE *pPal; //指向逻辑调色板结构的指针	
	LPRGBQUAD lpRGB; //指向RGBQUAD结构的指针	
	HPALETTE hPrevPalette; //用来保存设备中原来的调色板	
	HDC hDc; //设备句柄	
	HLOCAL hPal; //存储调色板的局部内存句柄	
	DWORD LineBytes;  //每一行的字节数	
	DWORD ImgSize;   //实际的图象数据占用的字节数
	
	//实际用到的颜色数 ，即调色板数组中的颜色个数
	DWORD NumColors; 
	
	DWORD i;
	void* hImgData;
	HPALETTE hPalette;
 	
	//将BITMAPFILEHEADER结构从文件中读出，填写到bf中	
	memcpy((LPSTR)&bf, BMPBuf, sizeof(BITMAPFILEHEADER)); 
	
	//将BITMAPINFOHEADER结构从文件中读出，填写到bi中
	memcpy((LPSTR)&bi, BMPBuf+sizeof(BITMAPFILEHEADER),
		sizeof(BITMAPINFOHEADER));	 	
	LineBytes=(DWORD)WIDTHBYTES(bi.biWidth*bi.biBitCount);
	
	//ImgSize为实际的图象数据占用的字节数	
	ImgSize=(DWORD)LineBytes*bi.biHeight;
	
	//NumColors为实际用到的颜色数 ，即调色板数组中的颜色个数
	if(bi.biClrUsed!=0)//如果bi.biClrUsed不为零，即为实际用到的颜色数
	{			
		NumColors=(DWORD)bi.biClrUsed; 	
	}
	else //否则，用到的颜色数为2biBitCount。		
	{
		switch(bi.biBitCount)
		{		
		case 1:			
			NumColors=2;			
			break;			
		case 4:			
			NumColors=16;			
			break;			
		case 8:			
			NumColors=256;			
			break;			
		case 24:			
			NumColors=0; //对于真彩色图，没用到调色板			
			break;			
		default: //不处理其它的颜色数，认为出错。			
			MessageBox(hWnd,TEXT("Invalid color numbers!"),
				TEXT("Error Message"), MB_OK|MB_ICONEXCLAMATION);			
			_lclose(hf);			
			return FALSE; //关闭文件，返回FALSE			
		}
	}
	
	if(bf.bfOffBits!=(DWORD)(NumColors*sizeof(RGBQUAD)
		+sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)))
		
	{		
		//计算出的偏移量与实际偏移量不符，一定是颜色数出错		
		MessageBox(hWnd,TEXT("Invalid color numbers!"),
			TEXT("Error Message"), MB_OK|MB_ICONEXCLAMATION);		
		return FALSE; //关闭文件，返回FALSE		
	}
	
	bf.bfSize=sizeof(BITMAPFILEHEADER)
		+sizeof(BITMAPINFOHEADER)+		
		NumColors*sizeof(RGBQUAD)+ImgSize;
	
	//分配内存，大小为BITMAPINFOHEADER结构长度加调色板+实际位图
	
	if((hImgData=GlobalAlloc(GHND,(DWORD)		
		(sizeof(BITMAPINFOHEADER)+		
		NumColors*sizeof(RGBQUAD)+		
		ImgSize)))==NULL)		
	{		
		//分配内存错误		
		MessageBox(hWnd, TEXT("Error alloc memory!"),
			TEXT("ErrorMessage"), MB_OK|MB_ICONEXCLAMATION);		
		return FALSE; //关闭文件，返回FALSE		
	}
	
	//指针lpImgData指向该内存区	
	lpImgData=(LPBITMAPINFOHEADER)GlobalLock(hImgData); 
		
	//将文件内容读入lpImgData	
	memcpy((char *)lpImgData,BMPBuf+sizeof(BITMAPFILEHEADER),
		(long)sizeof(BITMAPINFOHEADER)		
		+(long)NumColors*sizeof(RGBQUAD)+ImgSize); 
	
	if(NumColors!=0) //NumColors不为零，说明用到了调色板		
	{		
		//为逻辑调色板分配局部内存，大小为逻辑调色板结构长度加		
		//NumColors个PALETTENTRY		
		hPal=LocalAlloc(LHND,sizeof(LOGPALETTE)+			
			NumColors* sizeof(PALETTEENTRY));
		
		//指针pPal指向该内存区		
		pPal =(LOGPALETTE *)LocalLock(hPal);
		
		//填写逻辑调色板结构的头		
		pPal->palNumEntries = (unsigned short)NumColors;	
		pPal->palVersion = 0x300;
		
		//lpRGB指向的是调色板开始的位置		
		lpRGB = (LPRGBQUAD)((LPSTR)lpImgData +			
			(DWORD)sizeof(BITMAPINFOHEADER));
		
		//填写每一项
		
		for (i = 0; i < NumColors; i++)			
		{			
			pPal->palPalEntry[i].peRed=lpRGB->rgbRed;			
			pPal->palPalEntry[i].peGreen=lpRGB->rgbGreen;			
			pPal->palPalEntry[i].peBlue=lpRGB->rgbBlue;			
			pPal->palPalEntry[i].peFlags=(BYTE)0;			
			lpRGB++; //指针移到下一项			
		}
		
		//产生逻辑调色板，hPalette是一个全局变量		
		hPalette=CreatePalette(pPal);
		
		//释放局部内存		
		LocalUnlock(hPal);		
		LocalFree(hPal);		
	}
	
	//获得设备上下文句柄	
	hDc=GetDC(hWnd);
	
	if(hPalette) //如果刚才产生了逻辑调色板		
	{		
		//将新的逻辑调色板选入DC，将旧的逻辑调色板句柄保存在//hPrevPalette		
		hPrevPalette=SelectPalette(hDc,hPalette,FALSE);		
		RealizePalette(hDc);		
	}
	
	//产生位图句柄	
	HBITMAP hBitmap = CreateDIBitmap(hDc, 
		(LPBITMAPINFOHEADER)lpImgData, (LONG)CBM_INIT,		
		(LPSTR)lpImgData+sizeof(BITMAPINFOHEADER)
		+NumColors*sizeof(RGBQUAD),		
		(LPBITMAPINFO)lpImgData, DIB_RGB_COLORS);
	
	HDC hMemDC = CreateCompatibleDC(hDc);// 建立内存
	SelectObject(hMemDC, hBitmap);
	
	StretchBlt(hDc, 0, 0, nWidth, nHeight, hMemDC, 0, 0,
		nWidth,nHeight,SRCCOPY);//放大两倍
	
	//将原来的调色板(如果有的话)选入设备上下文句柄	
	if(hPalette && hPrevPalette)		
	{		
		SelectPalette(hDc,hPrevPalette,FALSE);		
		RealizePalette(hDc);		
	}

	DeleteDC(hMemDC);//释放内存设备上下文	
	ReleaseDC(hWnd,hDc); //释放设备上下文		
	GlobalUnlock(hImgData); //解锁内存区
	GlobalFree(hImgData);
	DeleteObject(hBitmap);
	return TRUE; //成功返回	
}
