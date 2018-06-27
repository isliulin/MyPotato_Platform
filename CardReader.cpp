#include "StdAfx.h"
#include "CardReader.h"


CCardReader::CCardReader(void)
{
	iRet = 0;
	m_ImagePath = L"";
	name;
	sex = L"";
	nation = L"";
	birthdate = L"";
	addr = L"";
	id = L"";
	office = L"";
	beginDate = L"";
	endDate = L"";
}


CCardReader::~CCardReader(void)
{
}


CString CCardReader::OpenCardPort()
{
	iRet = SDT_OpenPort(1001);
	if (iRet == 0x90)
	{
		return L"";
	}
	return L"�򿪶˿�ʧ��";
}

CString CCardReader::CloseCardPort()
{
	SDT_ClosePort(1001);
	return L"";

}

void CCardReader::SetHeadPicPath(CString path)
{
	m_ImagePath = path;
}

CString CCardReader::ReadIDCard()
{
	iRet = 0;
	iRet = SDT_StartFindIDCard(1001,pucIIN,0);
	if (iRet != 159)
	{
		OutputDebugString(L"----�����ҿ�ʧ��----\n");
		return L"�����ҿ�ʧ��";
	}

	iRet = SDT_SelectIDCard(1001,pucSN,0);
	if (iRet != 144)
	{
		OutputDebugString(L"----ѡ��ʧ��----\n");
		return L"ѡ��ʧ��";
	}

	iRet = SDT_ReadBaseMsg(1001,pucCHMsg,&iCHMsgLen,pucPHMsg,&iPHMsgLen,0);
	if (iRet != 144)
	{
		OutputDebugString(L"----����ʧ��----\n");
		return L"����ʧ��";
	}


	CFile file;
	CString wltFile;
	int dotIndex = m_ImagePath.ReverseFind('.');
	wltFile = m_ImagePath.Left(dotIndex) + L".wlt";
	//д���ܺ���ļ�
	file.Open(wltFile,CFile::modeReadWrite | CFile::modeCreate);
	file.Write(pucPHMsg, iPHMsgLen);
	file.Close();

	//�����ļ�
	int filePathLength = ::WideCharToMultiByte(CP_ACP, NULL, wltFile, wltFile.GetLength(), NULL, 0, NULL, NULL); 
	char* cfilePath = new char[filePathLength + 1]; 
	ZeroMemory(cfilePath,filePathLength + 1);
	::WideCharToMultiByte(CP_ACP, NULL, wltFile, wltFile.GetLength(), cfilePath, filePathLength, NULL, NULL); 
	iRet = GetBmp(cfilePath,2);
	delete cfilePath;

	//CodeFile(m_ImagePath.Left(dotIndex) + L".bmp");




	int ansiLen = ::WideCharToMultiByte(CP_ACP, NULL, (LPCWSTR)pucCHMsg, iCHMsgLen, NULL, 0, NULL, NULL); 
	char* szAnsi = new char[ansiLen + 1]; 
	::WideCharToMultiByte(CP_ACP, NULL, (LPCWSTR)pucCHMsg, wcslen((LPCWSTR)pucCHMsg), szAnsi, ansiLen, NULL, NULL); 
	CString msg(szAnsi);
	delete szAnsi;

	name = msg.Left(15).Trim();
	sex = msg.Mid(15,1).Trim();
	nation = GetNationByString(msg.Mid(16,2)).Trim();
	birthdate = msg.Mid(18,8).Trim();
	addr = msg.Mid(26,35).Trim();
	id = msg.Mid(61,18).Trim();
	office = msg.Mid(79,15).Trim();
	beginDate = msg.Mid(94,8).Trim();
	endDate = msg.Mid(102,8).Trim();

	return L"";



}

CString CCardReader::GetNationByString(CString str)
{
	CString nation;
	int i = _ttoi(str);
	switch (i)
	{
	case 1:
		nation = L"��";
		break;
	case 2:
		nation = L"�ɹ�";
		break;
	case 3:
		nation = L"��";
		break;
	case 4:
		nation = L"��";
		break;
	case 5:
		nation = L"ά���";
		break;
	case 6:
		nation = L"��";
		break;
	case 7:
		nation = L"��";
		break;
	case 8:
		nation = L"׳";
		break;
	case 9:
		nation = L"����";
		break;
	case 10:
		nation = L"����";
		break;
	case 11:
		nation = L"��";
		break;
	case 12:
		nation = L"��";
		break;
	case 13:
		nation = L"��";
		break;
	case 14:
		nation = L"��";
		break;
	case 15:
		nation = L"����";
		break;
	case 16:
		nation = L"����";
		break;
	case 17:
		nation = L"������";
		break;
	case 18:
		nation = L"��";
		break;
	case 19:
		nation = L"��";
		break;
	case 20:
		nation = L"����";
		break;
	case 21:
		nation = L"��";
		break;
	case 22:
		nation = L"�";
		break;
	case 23:
		nation = L"��ɽ";
		break;
	case 24:
		nation = L"����";
		break;
	case 25:
		nation = L"ˮ";
		break;
	case 26:
		nation = L"����";
		break;
	case 27:
		nation = L"����";
		break;
	case 28:
		nation = L"����";
		break;
	case 29:
		nation = L"�¶�����";
		break;
	case 30:
		nation = L"��";
		break;
	case 31:
		nation = L"�ﺲ��";
		break;
	case 32:
		nation = L"����";
		break;
	case 33:
		nation = L"Ǽ";
		break;
	case 34:
		nation = L"����";
		break;
	case 35:
		nation = L"����";
		break;
	case 36:
		nation = L"ë��";
		break;
	case 37:
		nation = L"����";
		break;
	case 38:
		nation = L"����";
		break;
	case 39:
		nation = L"����";
		break;
	case 40:
		nation = L"����";
		break;
	case 41:
		nation = L"������";
		break;
	case 42:
		nation = L"ŭ";
		break;
	case 43:
		nation = L"���α��";
		break;
	case 44:
		nation = L"����˹";
		break;
	case 45:
		nation = L"���¿�";
		break;
	case 46:
		nation = L"�°�";
		break;
	case 47:
		nation = L"����";
		break;
	case 48:
		nation = L"ԣ��";
		break;
	case 49:
		nation = L"��";
		break;
	case 50:
		nation = L"������";
		break;
	case 51:
		nation = L"����";
		break;
	case 52:
		nation = L"���״�";
		break;
	case 53:
		nation = L"����";
		break;
	case 54:
		nation = L"�Ű�";
		break;
	case 55:
		nation = L"���";
		break;
	case 56:
		nation = L"��ŵ";
		break;
	case 57:
		nation = L"����";
		break;
	case 98:
		nation = L"������뼮";
		break;
	}
	return nation;
}

CString CCardReader::GetCardName()
{
	return name;
}

// ��ȡ���֤�������� 
CString CCardReader::GetCardBirth() 
{
	CString year = birthdate.Left(4);
	CString month = birthdate.Mid(4,2);
	CString day = birthdate.Mid(6,2);
	return year + L"��" + month + L"��" + day + L"��";
}

// ��ȡ���֤�Ա�
CString CCardReader::GetCardSex()
{
	if (sex == L"1")
	{
		return L"��";
	}
	else
	{
		return L"Ů";
	}
}

// ��ȡ���֤����
CString CCardReader::GetCardNation()
{
	return nation;
}

// ��ȡ���֤סַ 
CString CCardReader::GetCardAddress()
{
	return addr;
}

// ��ȡ���֤����
CString CCardReader::GetCardID() 
{
	return id;
}

// ��ȡ��֤����
CString CCardReader::GetIssuingOrgan() 
{
	return office;
}

// ��ȡ��Ч����
CString CCardReader::GetAllottedTime() 
{
	beginDate.Insert(4,L".");
	beginDate.Insert(7,L".");
	endDate.Insert(4,L".");
	endDate.Insert(7,L".");
	return beginDate + L"-" + endDate;
}
