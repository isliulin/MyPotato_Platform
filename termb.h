extern HINSTANCE hModule;

//卡认证;
typedef int (* pAuthenticate)();
typedef int (* pAuthenticateExt)();

//关闭串口
typedef int (* pCloseComm)(); 

//获取设备编号
typedef int (* pGetDeviceID)(char * pMsg);		

//获取bmp照片
typedef int (* pGetPhoto)(char * Wlt_File);

//获取SAM编号
typedef char* (*pGetSAMID)();

// 初始化串口;
typedef  int (*pInitComm)(int port);

// 初始化串口(扩展);
typedef int  (*pInitCommExt)();

//获取厂商和产品信息
typedef void (*pMfrInfo)(char * cDeviceType,char * cDeviceCategory,char * cDeviceName,char * cMfr);

//读卡操作,信息文件存储在应用程序当前目录下;
typedef int  (*pRead_Content)( int active);

//	读卡操作,信息文件存储在指定目录下。
typedef int  (*pRead_Content_Path)(char* cPath, int active);

extern pAuthenticate Authenticate; 
extern pAuthenticateExt  AuthenticateExt;
extern pCloseComm CloseComm;
extern pGetDeviceID  GetDeviceID;
extern pGetPhoto GetPhoto;
extern pGetSAMID  GetSAMID;
extern pInitComm InitComm;
extern pInitCommExt InitCommExt;
extern pMfrInfo MfrInfo;
extern pRead_Content Read_Content;
extern pRead_Content_Path Read_Content_Path;
extern BOOL LoadTermbDll();
extern void FreeTermbDll();