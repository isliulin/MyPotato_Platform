#include "StdAfx.h"
#include "termb.h"

HINSTANCE hModule;

pAuthenticate Authenticate; 
pAuthenticateExt  AuthenticateExt;
pCloseComm CloseComm;
pGetDeviceID  GetDeviceID;
pGetPhoto GetPhoto;
pGetSAMID  GetSAMID;
pInitComm InitComm;
pInitCommExt InitCommExt;
pMfrInfo MfrInfo;
pRead_Content Read_Content;
pRead_Content_Path Read_Content_Path;
BOOL LoadTermbDll();
void FreeTermbDll();

BOOL LoadTermbDll()
{
	TCHAR szbuf[MAX_PATH] = {0};
	GetModuleFileName(AfxGetStaticModuleState()->m_hCurrentInstanceHandle,szbuf,MAX_PATH);
	CString _dllPath = CString(szbuf);
	_dllPath = _dllPath.Left(_dllPath.ReverseFind('\\')) + TEXT("\\termb.dll");
	hModule=LoadLibrary(_dllPath);
	if(hModule ==NULL )
	{
		//AfxMessageBox(_T("º”‘ÿ∂ØÃ¨ø‚ ß∞‹"));
		return FALSE;
	};

    Authenticate    =(pAuthenticate)GetProcAddress(hModule,"Authenticate");
    AuthenticateExt     =(pAuthenticateExt)GetProcAddress(hModule,"AuthenticateExt");
    CloseComm    =(pCloseComm)GetProcAddress(hModule,"CloseComm");
    GetDeviceID   =(pGetDeviceID)GetProcAddress(hModule,"GetDeviceID");
    GetPhoto    =(pGetPhoto)GetProcAddress(hModule,"GetPhoto");
    GetSAMID   =(pGetSAMID)GetProcAddress(hModule,"GetSAMID");
    InitComm     =(pInitComm)GetProcAddress(hModule,"InitComm");
    InitCommExt   =(pInitCommExt)GetProcAddress(hModule,"InitCommExt");
    MfrInfo     =(pMfrInfo)GetProcAddress(hModule,"MfrInfo");
    Read_Content =(pRead_Content)GetProcAddress(hModule,"Read_Content");
    Read_Content_Path =(pRead_Content_Path)GetProcAddress(hModule,"Read_Content_Path");
}

void FreeTermbDll()
{
	if(hModule !=NULL )
	{
		FreeLibrary(hModule);
		hModule=NULL;
	}	

}
