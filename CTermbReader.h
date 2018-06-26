#pragma once

#include "termb.h"

class CTermbReader
{
public:
	CTermbReader(void);
	~CTermbReader(void);
	CString OpenCardPort();
	CString CloseCardPort() ;
	void SetHeadPicPath(CString path) ;
	CString ReadIDCard(CString savepath) ;

	CString GetCardName();

	// 获取身份证出生日期 [3/2/2018 Leo]
	CString GetCardBirth() ;

	// 获取身份证性别 [3/2/2018 Leo]
	CString GetCardSex() ;

	// 获取身份证民族 [3/2/2018 Leo]
	CString GetCardNation() ;

	// 获取身份证住址 [3/2/2018 Leo]
	CString GetCardAddress() ;

	// 获取身份证号码 [3/2/2018 Leo]
	CString GetCardID() ;

	// 获取发证机关 [3/2/2018 Leo]
	CString GetIssuingOrgan() ;

	// 获取有效期限 [3/2/2018 Leo]
	CString GetAllottedTime() ;

	BOOL FindFinger();

	CString GetFingerDat();

private:
	int iRet ;	
	unsigned char pucSN[1024];
	unsigned char pucIIN[1024];

	unsigned char pucCHMsg[1024];
	unsigned char pucPHMsg[1024];
	unsigned char pucFPMsg[1024];
	unsigned int iCHMsgLen,iPHMsgLen,puiFPMsgLen;
	
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
	bool findfinger;
	CString GetNationByString(CString str);

};

