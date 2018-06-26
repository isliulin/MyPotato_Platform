extern HINSTANCE hModule2;

//1 版本信息获取
typedef int (__stdcall  *pFP_GetVersion)(unsigned char code[4]);
//2	初始化操作
typedef int (__stdcall *pFP_Begin)(); 

//3	1枚指纹图像特征提取
typedef int (__stdcall  *pFP_FeatureExtract)(unsigned char   cScannerType, //输入参数 指纹采集器类型代码
	unsigned char cFingerCode,	 // 指位代码。输入参数。
	unsigned char * pFingerImgBuf,//输入参数 指纹图像数据指针 图像格式为256(width)*360(height) 8位灰度图像 
	unsigned char * pFeatureData); //输出参数 指纹图像特征数据指针


//5	对2个指纹特征数据进行比对，得到匹配相似度值
typedef int (__stdcall  *pFP_FeatureMatch)(unsigned char * pFeatureData1,	//输入参数 指纹特征数据1
	unsigned char * pFeatureData2,	//输入参数 指纹特征数据2
	float * pfSimilarity);			//输出参数 匹配相似度值0.00-1.00


//6	对指纹图像和指纹特征进行比对，得到匹配相似度值
typedef int (__stdcall  *pFP_ImageMatch)(unsigned char * pFingerImgBuf,	//输入参数 指纹图像数据指针 图像格式为256(width)*360(height)
	unsigned char * pFeatureData,	//输入参数 指纹特征数据
	float * pfSimilarity);			//输出参数 指纹图像特征数据指针

//7	对指纹图像数据进行压缩
typedef int (__stdcall  *pFP_Compress)(  unsigned char cScannerType,	//输入参数 指纹采集器类型代码
	unsigned char cEnrolResult,	
	unsigned char cFingerCode,		 // 指位代码。输入参数。
	unsigned  char * pFingerImgBuf,   //输入参数 指纹图像数据指针 图像格式为256(width)*360(height)
	int nCompressRatio,				//输入参数 压缩倍数
	unsigned char * pCompressedImgBuf,//输出参数 指纹图像压缩数据指针 空间为20K字节
	unsigned char strBuf[256]);		//输出参数 返回-4时的错误信息

//8	对指纹图像压缩数据进行复现
typedef int (__stdcall  *pFP_Decompress)(unsigned char * pCompressedImgBuf,//输入参数 指纹图像压缩数据指针 已分配好20K字节空间
	unsigned char * pFingerImgBuf,	//输入参数 指纹图像数据指针 大小256*360 字节
	unsigned char strBuf[256]);		//输出参数 返回-4时的错误信息

//9	获取指纹图像的质量值
typedef int (__stdcall  *pFP_GetQualityScore)(unsigned char * pFingerImgBuf,//输入参数 指纹图像数据指针 图像格式为256(width)*360(height)
	unsigned char  * pnScore);//输出参数 图像质量值 00H - 64H


//10 生成"未注册"指纹特征数据
typedef int (__stdcall  *pFP_GenFeatureFromEmpty1)(
	unsigned char   cScannerType,  //输入参数 指纹采集器类型代码
	unsigned char	cFingerCode,	 // 指位代码。输入参数。
	unsigned char * pFeatureData  //输出参数 已由调用者分配512字节空间
	);

//10 生成"未注册"指纹特征数据
typedef int (__stdcall  *pFP_GenFeatureFromEmpty2)(unsigned char cFingerCode, unsigned char * pFeatureData);//输出参数 已由调用者分配512字节空间

//11 生成"未注册"指纹图像压缩数据
//typedef int (* pFP_GenCompressDataFromEmpty)(unsigned char * pCompressedImgBuf);//输出参数 已由调用者分配20K字节空间
//

//12 结束操作
typedef int (__stdcall  *pFP_End)();

extern pFP_GetVersion FP_GetVersion; 
extern pFP_Begin FP_Begin;
extern pFP_FeatureExtract FP_FeatureExtract;
extern pFP_FeatureMatch FP_FeatureMatch;
extern pFP_ImageMatch FP_ImageMatch;
extern pFP_Compress FP_Compress;
extern pFP_Decompress FP_Decompress;
extern pFP_GetQualityScore FP_GetQualityScore;
extern pFP_GenFeatureFromEmpty1 FP_GenFeatureFromEmpty1;
extern pFP_GenFeatureFromEmpty2 FP_GenFeatureFromEmpty2;
extern pFP_End FP_End;
extern BOOL LoadDll();
extern void FreeDll();