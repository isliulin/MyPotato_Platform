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
	return L"打开端口失败";
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
		OutputDebugString(L"----启动找卡失败----\n");
		return L"启动找卡失败";
	}

	iRet = SDT_SelectIDCard(1001,pucSN,0);
	if (iRet != 144)
	{
		OutputDebugString(L"----选卡失败----\n");
		return L"选卡失败";
	}

	iRet = SDT_ReadBaseMsg(1001,pucCHMsg,&iCHMsgLen,pucPHMsg,&iPHMsgLen,0);
	if (iRet != 144)
	{
		OutputDebugString(L"----读卡失败----\n");
		return L"读卡失败";
	}


	CFile file;
	CString wltFile;
	int dotIndex = m_ImagePath.ReverseFind('.');
	wltFile = m_ImagePath.Left(dotIndex) + L".wlt";
	//写加密后的文件
	file.Open(wltFile,CFile::modeReadWrite | CFile::modeCreate);
	file.Write(pucPHMsg, iPHMsgLen);
	file.Close();

	//解密文件
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
		nation = L"汉";
		break;
	case 2:
		nation = L"蒙古";
		break;
	case 3:
		nation = L"回";
		break;
	case 4:
		nation = L"藏";
		break;
	case 5:
		nation = L"维吾尔";
		break;
	case 6:
		nation = L"苗";
		break;
	case 7:
		nation = L"彝";
		break;
	case 8:
		nation = L"壮";
		break;
	case 9:
		nation = L"布依";
		break;
	case 10:
		nation = L"朝鲜";
		break;
	case 11:
		nation = L"满";
		break;
	case 12:
		nation = L"侗";
		break;
	case 13:
		nation = L"瑶";
		break;
	case 14:
		nation = L"白";
		break;
	case 15:
		nation = L"土家";
		break;
	case 16:
		nation = L"哈尼";
		break;
	case 17:
		nation = L"哈萨克";
		break;
	case 18:
		nation = L"傣";
		break;
	case 19:
		nation = L"黎";
		break;
	case 20:
		nation = L"傈僳";
		break;
	case 21:
		nation = L"佤";
		break;
	case 22:
		nation = L"畲";
		break;
	case 23:
		nation = L"高山";
		break;
	case 24:
		nation = L"拉祜";
		break;
	case 25:
		nation = L"水";
		break;
	case 26:
		nation = L"东乡";
		break;
	case 27:
		nation = L"纳西";
		break;
	case 28:
		nation = L"景颇";
		break;
	case 29:
		nation = L"柯尔克孜";
		break;
	case 30:
		nation = L"土";
		break;
	case 31:
		nation = L"达翰尔";
		break;
	case 32:
		nation = L"仫佬";
		break;
	case 33:
		nation = L"羌";
		break;
	case 34:
		nation = L"布朗";
		break;
	case 35:
		nation = L"撒拉";
		break;
	case 36:
		nation = L"毛南";
		break;
	case 37:
		nation = L"仡佬";
		break;
	case 38:
		nation = L"锡伯";
		break;
	case 39:
		nation = L"阿昌";
		break;
	case 40:
		nation = L"普米";
		break;
	case 41:
		nation = L"塔吉克";
		break;
	case 42:
		nation = L"怒";
		break;
	case 43:
		nation = L"乌孜别克";
		break;
	case 44:
		nation = L"俄罗斯";
		break;
	case 45:
		nation = L"鄂温克";
		break;
	case 46:
		nation = L"德昂";
		break;
	case 47:
		nation = L"保安";
		break;
	case 48:
		nation = L"裕固";
		break;
	case 49:
		nation = L"京";
		break;
	case 50:
		nation = L"塔塔尔";
		break;
	case 51:
		nation = L"独龙";
		break;
	case 52:
		nation = L"鄂伦春";
		break;
	case 53:
		nation = L"赫哲";
		break;
	case 54:
		nation = L"门巴";
		break;
	case 55:
		nation = L"珞巴";
		break;
	case 56:
		nation = L"基诺";
		break;
	case 57:
		nation = L"其它";
		break;
	case 98:
		nation = L"外国人入籍";
		break;
	}
	return nation;
}

CString CCardReader::GetCardName()
{
	return name;
}

// 获取身份证出生日期 
CString CCardReader::GetCardBirth() 
{
	CString year = birthdate.Left(4);
	CString month = birthdate.Mid(4,2);
	CString day = birthdate.Mid(6,2);
	return year + L"年" + month + L"月" + day + L"日";
}

// 获取身份证性别
CString CCardReader::GetCardSex()
{
	if (sex == L"1")
	{
		return L"男";
	}
	else
	{
		return L"女";
	}
}

// 获取身份证民族
CString CCardReader::GetCardNation()
{
	return nation;
}

// 获取身份证住址 
CString CCardReader::GetCardAddress()
{
	return addr;
}

// 获取身份证号码
CString CCardReader::GetCardID() 
{
	return id;
}

// 获取发证机关
CString CCardReader::GetIssuingOrgan() 
{
	return office;
}

// 获取有效期限
CString CCardReader::GetAllottedTime() 
{
	beginDate.Insert(4,L".");
	beginDate.Insert(7,L".");
	endDate.Insert(4,L".");
	endDate.Insert(7,L".");
	return beginDate + L"-" + endDate;
}
