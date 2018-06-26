#pragma once
#include "sdtapi.h"
#include "WltRS.h"
#pragma comment(lib,"sdtapi.lib")
#pragma comment(lib,"WltRS.lib")


class CCardReader
{
public:
	CCardReader(void);
	~CCardReader(void);
	CString OpenCardPort();
	CString CloseCardPort() ;

	void SetHeadPicPath(CString path) ;
	CString ReadIDCard() ;

	CString GetCardName();

	// 获取身份证出生日期 
	CString GetCardBirth() ;

	// 获取身份证性别 
	CString GetCardSex() ;

	// 获取身份证民族
	CString GetCardNation() ;

	// 获取身份证住址
	CString GetCardAddress() ;

	// 获取身份证号码 
	CString GetCardID() ;

	// 获取发证机关
	CString GetIssuingOrgan() ;

	// 获取有效期限 
	CString GetAllottedTime() ;

private:
	int iRet ;	
	unsigned char pucSN[1024];
	unsigned char pucIIN[1024];

	unsigned char pucCHMsg[1024];
	unsigned char pucPHMsg[1024];
	unsigned int iCHMsgLen,iPHMsgLen;

	CString m_ImagePath;
	CString name;
	CString sex;
	CString nation;
	CString birthdate;
	CString addr;
	CString id;
	CString office;
	CString beginDate;
	CString endDate;

	CString GetNationByString(CString str);

	

};

