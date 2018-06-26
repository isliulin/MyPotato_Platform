#include "StdAfx.h"
#include "ID_Fpr.h"

HINSTANCE hModule2;    //修改为hModule2,因为在termb.h中有同名变量定义，否则会出现重定义问题

pFP_GetVersion FP_GetVersion; 
pFP_Begin FP_Begin;
pFP_FeatureExtract FP_FeatureExtract;
pFP_FeatureMatch FP_FeatureMatch;
pFP_ImageMatch FP_ImageMatch;
pFP_Compress FP_Compress;
pFP_Decompress FP_Decompress;
pFP_GetQualityScore FP_GetQualityScore;
pFP_GenFeatureFromEmpty1 FP_GenFeatureFromEmpty1;
pFP_GenFeatureFromEmpty2 FP_GenFeatureFromEmpty2;
pFP_End FP_End;
BOOL LoadDll();
void FreeDll();

BOOL LoadDll()
{
	hModule2=LoadLibrary(_T("ID_Fpr.dll"));
	if(hModule2 ==NULL )
	{
		//AfxMessageBox(_T("加载动态库失败"));
		return FALSE;
	}

    FP_GetVersion    =(pFP_GetVersion)GetProcAddress(hModule2,"FP_GetVersion");
    FP_Begin     =(pFP_Begin)GetProcAddress(hModule2,"FP_Begin");
    FP_FeatureExtract    =(pFP_FeatureExtract)GetProcAddress(hModule2,"FP_FeatureExtract");
    FP_FeatureMatch   =(pFP_FeatureMatch)GetProcAddress(hModule2,"FP_FeatureMatch");
    FP_ImageMatch    =(pFP_ImageMatch)GetProcAddress(hModule2,"FP_ImageMatch");
    FP_Compress   =(pFP_Compress)GetProcAddress(hModule2,"FP_Compress");
    FP_Decompress     =(pFP_Decompress)GetProcAddress(hModule2,"FP_Decompress");
    FP_GetQualityScore   =(pFP_GetQualityScore)GetProcAddress(hModule2,"FP_GetQualityScore");
    FP_GenFeatureFromEmpty1     =(pFP_GenFeatureFromEmpty1)GetProcAddress(hModule2,"FP_GenFeatureFromEmpty1");
    FP_GenFeatureFromEmpty2 =(pFP_GenFeatureFromEmpty2)GetProcAddress(hModule2,"FP_GenFeatureFromEmpty2");
    FP_End =(pFP_End)GetProcAddress(hModule2,"FP_End");
}

void FreeDll()
{
	if(hModule2 !=NULL )
	{
		FreeLibrary(hModule2);
		hModule2=NULL;
	}	

}
