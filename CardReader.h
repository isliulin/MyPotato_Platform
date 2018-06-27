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

	// ��ȡ���֤�������� 
	CString GetCardBirth() ;

	// ��ȡ���֤�Ա� 
	CString GetCardSex() ;

	// ��ȡ���֤����
	CString GetCardNation() ;

	// ��ȡ���֤סַ
	CString GetCardAddress() ;

	// ��ȡ���֤���� 
	CString GetCardID() ;

	// ��ȡ��֤����
	CString GetIssuingOrgan() ;

	// ��ȡ��Ч���� 
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

