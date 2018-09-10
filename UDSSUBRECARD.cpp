// UDSSUBRECARD.cpp : 实现文件
//

#include "stdafx.h"
#include "UDSPlatformV1.7.h"
#include "UDSSUBRECARD.h"
#include "afxdialogex.h"
#include "ID_Fpr.h"        //指纹识别
#include "ID_FprCap_Protocol.h"
#include "Common.h"


// CUDSSUBRECARD 对话框

IMPLEMENT_DYNAMIC(CUDSSUBRECARD, CDialogEx)

CUDSSUBRECARD::CUDSSUBRECARD(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUDSSUBRECARD::IDD, pParent)
{

	m_editAddress = _T("");
	m_editBirth = _T("");
	m_editCardnum = _T("");
	m_editName = _T("");
	m_editNation = _T("");
	m_editOffice = _T("");
	m_editSex = _T("");
	m_editValidity = _T("");
	m_StaInfo = _T("");

	m_nOriX   = 0;
	m_nOriY   = 0;
	m_nWidth  = 0;
	m_nHeight = 0;
}

CUDSSUBRECARD::~CUDSSUBRECARD()
{
}

void CUDSSUBRECARD::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ADDRESS, m_editAddress);
	DDX_Text(pDX, IDC_EDIT_BITRTH, m_editBirth);
	DDX_Text(pDX, IDC_EDIT_CARDNUM, m_editCardnum);
	DDX_Text(pDX, IDC_EDIT_NAME, m_editName);
	DDX_Text(pDX, IDC_EDIT_NATION, m_editNation);
	DDX_Text(pDX, IDC_EDIT_OFFICE, m_editOffice);
	DDX_Text(pDX, IDC_EDIT_SEX, m_editSex);
	DDX_Text(pDX, IDC_EDIT_VALIDITY, m_editValidity);
	DDX_Control(pDX, IDC_BTN_READCARD, m_btnReadCard);
	DDX_Control(pDX, IDC_BTN_BEGIN, m_btnBegin);
	DDX_Control(pDX, IDC_STA_CARDIMG, m_conCardImg);
	DDX_Control(pDX, IDC_STA_COPYIMG, m_conCopyImg);
	DDX_Control(pDX, IDC_SLID_PERIOD, m_conPeriod);
	DDX_Text(pDX, IDC_STA_INFO, m_StaInfo);
	DDX_Control(pDX, IDC_BTN_FINGER, m_btnReadFinger);
}


BEGIN_MESSAGE_MAP(CUDSSUBRECARD, CDialogEx)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_READCARD, &CUDSSUBRECARD::OnBnClickedBtnReadcard)
	ON_BN_CLICKED(IDC_CHK_AUTO, &CUDSSUBRECARD::OnClickedChkAuto)
//	ON_BN_CLICKED(IDC_BTN_BEGIN, &CUDSSUBRECARD::OnBnClickedBtnBegin)
//	ON_BN_CLICKED(IDC_BTN_END, &CUDSSUBRECARD::OnBnClickedBtnEnd)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_BEGIN, &CUDSSUBRECARD::OnBnClickedBtnBegin)
	ON_BN_CLICKED(IDC_CHK_SAVE, &CUDSSUBRECARD::OnBnClickedChkSave)
	ON_STN_DBLCLK(IDC_STA_COPYIMG, &CUDSSUBRECARD::OnDblclkStaCopyimg)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLID_PERIOD, &CUDSSUBRECARD::OnCustomdrawSlidPeriod)
	ON_BN_CLICKED(IDC_BTN_FINGER, &CUDSSUBRECARD::OnBnClickedBtnFinger)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_TESTDIFFER, &CUDSSUBRECARD::OnBnClickedBtnTestdiffer)
END_MESSAGE_MAP()


// CUDSSUBRECARD 消息处理程序


BOOL CUDSSUBRECARD::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	/************************************
	*
	* 1、检测缓存目录以及配置文件
	* 2、启动界面首次读卡,首次读卡及显示放在OP中，init中首次无法正常显示
	* 3、窗口背景
	* 4、Button
	* 5、CheckBox
	*
	**************************************/
//	m_iCard = new CCardReader;
	m_iCard = new CTermbReader;

	m_strInfoBuffer  = _T("");
	m_strPicBuffer   = _T("");
	m_strCardIniPath = _T("");

	m_editName     = _T("");
	m_editSex      = _T("");
	m_editNation   = _T("");
	m_editBirth    = _T("");
	m_editAddress  = _T("");
	m_editCardnum  = _T("");
	m_editOffice   = _T("");
	m_editValidity = _T("");
	m_strCopyImg   = _T("");

	m_nFingerNum   = 0;
	m_strFingerIndex = _T("");
	m_nFigerIndex  = -1;

	m_BFirstOpen   = TRUE;
	m_BSaveCopy    = TRUE;

	m_BAutoRead    = FALSE;

	//1、----------------------------------
	TCHAR     tem_tcExepath[MAX_PATH+1] = {0};
	GetModuleFileName(NULL, tem_tcExepath, MAX_PATH);
	(_tcsrchr(tem_tcExepath, _T('\\')))[1] = 0;

	m_strProntImg   = tem_tcExepath;
	m_strBackImg    = tem_tcExepath;
	m_strProntImg  += _T("res\\");
	m_strBackImg   += _T("res\\");
	m_strProntImg  += _T("PrintFront.jpg");
	m_strBackImg   += _T("PrintBack.jpg");


	CString    tem_strScanTemp = Self_GetMyDocument();
	tem_strScanTemp += _T("\\");
	tem_strScanTemp += _T("UDSData");
	CFileFind  tem_fFileFind;
	if (!tem_fFileFind.FindFile(tem_strScanTemp))
	{
		CreateDirectory(tem_strScanTemp, NULL);
	}
	//指纹存放路径
	m_strFingerPath  = tem_strScanTemp;
	m_strFingerPath += _T("\\身份证信息\\Finger\\");

	//判断配置文件是否存在
	m_strCardIniPath  = tem_strScanTemp + _T("\\");
	m_strCardIniPath += _T("PersonInfo.INI");

	CString tem_strSoftIniPath;
	tem_strSoftIniPath  = tem_strScanTemp + _T("\\");
	tem_strSoftIniPath += _T("config.ini");
	m_strIniPath        = tem_strSoftIniPath;

	CString  tem_strReadTime = _T("");
	::GetPrivateProfileString(_T("Resource"), _T("ReadSens"), _T("没有找到ReadSens信息"), tem_strReadTime.GetBuffer(MAX_PATH), MAX_PATH, tem_strSoftIniPath);
	tem_strReadTime.ReleaseBuffer();
	m_nReadTime = _ttoi(tem_strReadTime);

	if (m_nReadTime<1000 || m_nReadTime>10000)
	{
		m_nReadTime = 3000;
	}


	BOOL     tem_BFileFind = PathFileExists(m_strCardIniPath);
	if (!tem_BFileFind)
	{
		CString   tem_strExePath = _T("");
		//文件不存在，将原配置文件复制到指定目录
		TCHAR        tem_tcFilepath[MAX_PATH+1] = {0};
		GetModuleFileName(NULL, tem_tcFilepath, MAX_PATH);
		(_tcsrchr(tem_tcFilepath, _T('\\')))[1] = 0;
		tem_strExePath   = tem_tcFilepath;
		tem_strExePath  += _T("PersonInfo.INI");
//		tem_strExePath.Replace(_T("\\"), _T("\\\\"));
		CopyFile(tem_strExePath, m_strCardIniPath, TRUE);
	}

	m_strInfoBuffer  = tem_strScanTemp;
	m_strInfoBuffer += _T("\\身份证信息");
	if (!tem_fFileFind.FindFile(m_strInfoBuffer))
	{
		CreateDirectory(m_strInfoBuffer, NULL);
	}

	m_strPicBuffer  = m_strInfoBuffer;
	m_strPicBuffer += _T("\\Image");
	if (!tem_fFileFind.FindFile(m_strPicBuffer))
	{
		CreateDirectory(m_strPicBuffer, NULL);
	}

	m_strFingerDoc  = m_strInfoBuffer;
	m_strFingerDoc += _T("\\Finger");
	if (!tem_fFileFind.FindFile(m_strFingerDoc))
	{
		CreateDirectory(m_strFingerDoc, NULL);
	}
	m_strFingerDoc += _T("\\");
	//2、---------------------------------------------------------

	//3、---------------------------------------------------------
	CDialogEx::SetBackgroundColor(RGB(72, 77, 91));

	//4、---------------------------------------------------------
	m_btnReadCard.Init(IDB_PNG_READCARD, BTN_IMG_3, BTN_TYPE_NORMAL);
	m_btnBegin.Init(IDB_PNG_LL2, BTN_IMG_3, BTN_TYPE_NORMAL);
	m_btnReadFinger.Init(IDB_PNG_FINGER, BTN_IMG_4, BTN_TYPE_NORMAL);

	m_btnSubTips.Create(this);
	m_btnSubTips.AddTool(GetDlgItem(IDC_BTN_READCARD), _T("读卡"));
	m_btnSubTips.AddTool(GetDlgItem(IDC_BTN_BEGIN), _T("打开目录"));
	m_btnSubTips.AddTool(GetDlgItem(IDB_PNG_FINGER), _T("采集指纹"));

	m_btnSubTips.SetDelayTime(TTDT_INITIAL, 100);
	m_btnSubTips.SetDelayTime(TTDT_AUTOPOP, 5000);
	m_btnSubTips.SetDelayTime(TTDT_RESHOW, 100);
	m_btnSubTips.SetTipTextColor(RGB(0, 0, 0));
	m_btnSubTips.SetTipBkColor(RGB(255, 255, 255));
	m_btnSubTips.Activate(TRUE);

	//5、---------------------------------------------------------
	((CButton*)GetDlgItem(IDC_CHK_SAVE))->SetCheck(m_BSaveCopy);
	
	//6、自动读卡时间间隔------------------
	m_conPeriod.SetRange(1000, 10000, TRUE);
	m_conPeriod.SetPos(m_nReadTime);
	m_conPeriod.SetPageSize(500);

	float    tem_fPeriod   = (float)m_nReadTime*1.0/1000;
	CString  tem_strPeriod = _T("");
	tem_strPeriod.Format(_T("%.1f"), tem_fPeriod);
	GetDlgItem(IDC_STA_PERIODV)->SetWindowText(tem_strPeriod);
	CRect    tem_rcRect;; 
	GetDlgItem(IDC_STA_PERIODV)->GetWindowRect(&tem_rcRect); 
	ScreenToClient(&tem_rcRect); 
	InvalidateRect(&tem_rcRect);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CUDSSUBRECARD::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	
	if (m_BFirstOpen)
	{
		m_BFirstOpen = FALSE;
		CString  tem_strPicPath = Self_GetSystemTime(m_strPicBuffer);
		tem_strPicPath = Self_ReadCard(tem_strPicPath);
		if (!tem_strPicPath.IsEmpty())
		{
			Self_ShowPic(tem_strPicPath);
		}
		Self_ShowFingerImg(_T("res\\FingerPrint.bmp"));
	}
	
	

	// 不为绘图消息调用 CDialogEx::OnPaint()
}


void CUDSSUBRECARD::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}


HBRUSH CUDSSUBRECARD::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd->GetDlgCtrlID() == IDC_SLID_PERIOD)
	{
		return (HBRUSH) CreateSolidBrush(RGB(72,77,91));   //声明画刷CBrush再CreateSolidBrush看似相同，结果不对
	}
	if (pWnd->GetDlgCtrlID() == IDC_STA_NAME || pWnd->GetDlgCtrlID()==IDC_STA_SEX || pWnd->GetDlgCtrlID()==IDC_STA_NATION ||
		pWnd->GetDlgCtrlID() == IDC_STA_BIRTH || pWnd->GetDlgCtrlID()==IDC_STA_ADDRESS || pWnd->GetDlgCtrlID()==IDC_STA_VALIDITY ||
		pWnd->GetDlgCtrlID() == IDC_STA_CARDNUM || pWnd->GetDlgCtrlID()==IDC_STA_OFFICE || pWnd->GetDlgCtrlID()==IDC_STA_AUTOREAD ||
		pWnd->GetDlgCtrlID() == IDC_STA_INFO || pWnd->GetDlgCtrlID()==IDC_STA_SAVECOPY || pWnd->GetDlgCtrlID()==IDC_STA_MATCHV ||
		pWnd->GetDlgCtrlID() == IDC_STA_FINGERINDEX)
	{
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255, 255, 255));
		CFont   tem_Font;
	}

	if (pWnd->GetDlgCtrlID()==IDC_STA_BT)
	{
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255, 255, 255));
		CFont   tem_Font;
		tem_Font.CreatePointFont(200, _T("隶书"));
		pDC->SelectObject(&tem_Font);
	}
	else if (pWnd->GetDlgCtrlID()==IDC_STA_PERIOD || pWnd->GetDlgCtrlID()==IDC_STA_PERIODV)
	{
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
		pDC->SetBkMode(TRANSPARENT);
		if (m_BAutoRead)
		{
			pDC->SetTextColor(RGB(191, 191, 191));
		}
		else
		{
			pDC->SetTextColor(RGB(255, 255, 255));
		}
		CFont   tem_Font;
		tem_Font.CreatePointFont(100, _T("宋体"));
		pDC->SelectObject(&tem_Font);
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


BOOL CUDSSUBRECARD::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_MOUSEMOVE)
	{
		m_btnSubTips.RelayEvent(pMsg);
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


/**********************************
*         函数目录
*说明：读卡器模块
*
* 1、获取“我的文档”目录
* 2、获取系统时间
* 3、身份证读卡
* 4、显示身份证头像
* 5、写入xls
* 6、读卡
* 7、自动读卡开关
* 10、线程函数
* 11、浏览按钮
* 12、保存复印件
* 13、显示复印件
*
***********************************/
CString CUDSSUBRECARD::Self_GetMyDocument(void)
{
	CString        tem_strMyDocument = _T("");
	TCHAR          tem_cMyDocument[MAX_PATH] = {0};
	LPITEMIDLIST   tem_Pid = NULL;
	SHGetSpecialFolderLocation(NULL, CSIDL_PERSONAL, &tem_Pid);
	if (tem_Pid && SHGetPathFromIDList(tem_Pid, tem_cMyDocument))
	{
		tem_strMyDocument = tem_cMyDocument;
		return tem_strMyDocument;
	}
	return tem_strMyDocument;
}


CString CUDSSUBRECARD::Self_GetSystemTime(CString  dir)
{
	CString       tem_strPicName = dir;   //身份证头像名
	SYSTEMTIME    tem_st;
	CString       tem_strDate, tem_strTime;

	GetLocalTime(&tem_st);
	tem_strDate.Format(_T("%d%02d%02d_"), tem_st.wYear, tem_st.wMonth, tem_st.wDay);
	tem_strTime.Format(_T("%02d%02d%02d"), tem_st.wHour, tem_st.wMinute, tem_st.wSecond);

	tem_strPicName += _T("\\");
	tem_strPicName += tem_strDate;
	tem_strPicName += tem_strTime;
	tem_strPicName += _T(".bmp");
	return tem_strPicName;
}


CString CUDSSUBRECARD::Self_ReadCard(CString picpath)
{
	
	m_editName     = _T("");
	m_editSex      = _T("");
	m_editNation   = _T("");
	m_editBirth    = _T("");
	m_editAddress  = _T("");
	m_editCardnum  = _T("");
	m_editOffice   = _T("");
	m_editValidity = _T("");
	m_StaInfo      = _T("");
	UpdateData(FALSE);

	m_stcIdInfo.imgpath  = _T("");
	m_stcIdInfo.name     = _T("");
	m_stcIdInfo.sex      = _T("");
	m_stcIdInfo.nation   = _T("");
    m_stcIdInfo.birth    = _T("");
	m_stcIdInfo.address  = _T("");
	m_stcIdInfo.IDnumber = _T("");
	m_stcIdInfo.office   = _T("");
	m_stcIdInfo.validate = _T("");

	m_nFingerNum         = 0;

	CRect   tem_rcStaInfo;
	GetDlgItem(IDC_STA_INFO)->SetWindowText(_T(""));
	GetDlgItem(IDC_STA_INFO)->GetWindowRect(&tem_rcStaInfo); 
	ScreenToClient(&tem_rcStaInfo);        
	InvalidateRect(&tem_rcStaInfo);
	UpdateWindow();	

	
	CString    tem_strPicName    = picpath;
	CString    tem_strNewPicName = m_strPicBuffer;
	CString    tem_strIDCopyPath = m_strPicBuffer;
	CString    tem_strRC         = _T("");
	CString    tem_strRecardDate = _T("");

	CString    tem_strOriDat     = m_strFingerPath;
	CString    tem_strOriWlt     = m_strFingerPath;
	CString    tem_strDatPath    = m_strFingerPath;     //指纹dat文件路径
	CString    tem_strWltPath    = m_strFingerPath;     //指纹wlt文件路径

	//获取读卡时间
	SYSTEMTIME  tem_stDateTime;
	GetLocalTime(&tem_stDateTime);
	tem_strRecardDate.Format(_T("%d%02d%02d %02d:%02d:%02d"), tem_stDateTime.wYear, tem_stDateTime.wMonth, tem_stDateTime.wDay,
		                     tem_stDateTime.wHour, tem_stDateTime.wMinute, tem_stDateTime.wSecond);
	

	tem_strRC = m_iCard->OpenCardPort();
	if (tem_strRC != _T("打开端口失败"))
	{
		m_iCard->SetHeadPicPath(tem_strPicName);
		tem_strRC = m_iCard->ReadIDCard(m_strFingerPath);
		if (tem_strRC == _T(""))
		{
			//读卡成功
			
			m_editName     = m_iCard->GetCardName();
			m_editSex      = m_iCard->GetCardSex();
			m_editNation   = m_iCard->GetCardNation();
			m_editBirth    = m_iCard->GetCardBirth();
			m_editAddress  = m_iCard->GetCardAddress();
			m_editCardnum  = m_iCard->GetCardID();
			m_editOffice   = m_iCard->GetIssuingOrgan();
			m_editValidity = m_iCard->GetAllottedTime();
			m_iCard->CloseCardPort();			
			
			tem_strNewPicName += _T("\\");
			tem_strNewPicName += m_editName;
			tem_strNewPicName += m_editCardnum;
			tem_strNewPicName += _T(".bmp");

			tem_strIDCopyPath += _T("\\");
			tem_strIDCopyPath += m_editName;
			tem_strIDCopyPath += m_editCardnum;
			tem_strIDCopyPath += _T(".jpg");

			//旧的目标文件名称
			tem_strOriDat     += _T("fp.dat");
			tem_strOriWlt     += _T("xp.wlt");

			//新的目标文件名称
			tem_strDatPath    += m_editName;
			tem_strDatPath    += m_editCardnum;
			tem_strDatPath    += _T(".dat");

			tem_strWltPath    += m_editName;
			tem_strWltPath    += m_editCardnum;
			tem_strWltPath    += _T(".wlt");

			m_strCopyImg = tem_strIDCopyPath;

			m_stcIdInfo.imgpath  = tem_strNewPicName;
			m_stcIdInfo.name     = m_editName;
			m_stcIdInfo.sex      = m_editSex;
			m_stcIdInfo.nation   = m_editNation;
			m_stcIdInfo.birth    = m_editBirth;
			m_stcIdInfo.address  = m_editAddress;
			m_stcIdInfo.IDnumber = m_editCardnum;
			m_stcIdInfo.office   = m_editOffice;
			m_stcIdInfo.validate = m_editValidity;

			//判断m_strNewPicName是否存在，若存在删除本次保存图像，若不存在重命名为该图像
			BOOL   tem_BIsFinded = FALSE;
			CFileFind  tem_find;
			tem_BIsFinded = tem_find.FindFile(tem_strNewPicName);
			if (tem_BIsFinded)
			{
				//文件已存在
				DeleteFile(tem_strPicName);
			}
			else
			{
				//文件不存在
				tem_BIsFinded = tem_find.FindFile(tem_strPicName);
				if (tem_BIsFinded)
				{
					CFile::Rename(tem_strPicName, tem_strNewPicName);
				}

			}

			if (m_iCard->FindFinger())
			{
				//身份证包含指纹信息***************************************
				m_btnReadFinger.SetFourState();
				GetDlgItem(IDC_BTN_FINGER)->EnableWindow(TRUE);
				//每个人只保存一个dat文件--------------------------
				tem_BIsFinded = tem_find.FindFile(tem_strDatPath);
				if (tem_BIsFinded)
				{
					//该指纹已存在
					DeleteFile(tem_strDatPath);
					CFile::Rename(tem_strOriDat, tem_strDatPath);
				} 
				else
				{
					//该指纹不存在
					CFile::Rename(tem_strOriDat, tem_strDatPath);
				}


				//每个人只保存一个wlt文件--------------------------
				tem_BIsFinded = tem_find.FindFile(tem_strWltPath);
				if (tem_BIsFinded)
				{
					//该指纹已存在
					DeleteFile(tem_strWltPath);
					CFile::Rename(tem_strOriWlt, tem_strWltPath);
				} 
				else
				{
					//该指纹不存在
					CFile::Rename(tem_strOriWlt, tem_strWltPath);
				}


				//判断指纹个数------------------------------------
				int  tem_nFileSize = 0;
				USES_CONVERSION;
				char* tem_cDatPath = T2A(tem_strDatPath);
				tem_strDatPath.ReleaseBuffer();

				FILE* tem_fileDat;
				if ((tem_fileDat = fopen(tem_cDatPath, "rb")))
				{
					fseek(tem_fileDat, 0, SEEK_END);
					tem_nFileSize = ftell(tem_fileDat);				
					fclose(tem_fileDat);
					m_nFingerNum = tem_nFileSize/512;
				}	
			}
			else
			{
				//身份证不包含指纹信息***************************************
				//指纹个数为0-------------------------------------
//				m_btnReadFinger.SetFourState();
				GetDlgItem(IDC_BTN_FINGER)->EnableWindow(FALSE);
				m_nFingerNum = 0;
			}

			UpdateData(FALSE);
			//写入ini文件
			::WritePrivateProfileString(_T("PersonInfo"), _T("cardname"), m_editName, m_strCardIniPath); 
			::WritePrivateProfileString(_T("PersonInfo"), _T("cardsex"), m_editSex, m_strCardIniPath); 
			::WritePrivateProfileString(_T("PersonInfo"), _T("cardnation"), m_editNation, m_strCardIniPath); 
			::WritePrivateProfileString(_T("PersonInfo"), _T("cardbirthdate"), m_editBirth, m_strCardIniPath); 
			::WritePrivateProfileString(_T("PersonInfo"), _T("cardaddress"), m_editAddress, m_strCardIniPath); 
			::WritePrivateProfileString(_T("PersonInfo"), _T("cardid"), m_editCardnum, m_strCardIniPath); 
			::WritePrivateProfileString(_T("PersonInfo"), _T("cardissueo"), m_editOffice, m_strCardIniPath); 
			::WritePrivateProfileString(_T("PersonInfo"), _T("cardvaliddate"), m_editValidity, m_strCardIniPath); 
			::WritePrivateProfileString(_T("PersonInfo"), _T("cardimage"), tem_strNewPicName, m_strCardIniPath);
			::WritePrivateProfileString(_T("PersonInfo"), _T("carddate"), tem_strRecardDate, m_strCardIniPath);

			//写入excel
			Write2Excel(m_strCardIniPath, m_strInfoBuffer);
			//保存身份证复印件
			tem_BIsFinded = tem_find.FindFile(tem_strIDCopyPath);
			if (!tem_BIsFinded)
			{
				Self_SaveCardCopy(m_stcIdInfo, m_strProntImg, m_strBackImg, tem_strIDCopyPath);				
			}
			if (m_BSaveCopy)
			{
				Self_ShowCopyImg(tem_strIDCopyPath);
			}
			
			
			//读卡成功提示	
			CString     tem_strShowInfo = _T("");
			tem_strShowInfo.Format(_T("读卡成功，该身份证包含 %d 个指纹！"), m_nFingerNum);
		    GetDlgItem(IDC_STA_INFO)->SetWindowText(tem_strShowInfo);
			CRect   tem_rcStaInfo;
			GetDlgItem(IDC_STA_INFO)->GetWindowRect(&tem_rcStaInfo); 
			ScreenToClient(&tem_rcStaInfo);        
			InvalidateRect(&tem_rcStaInfo);
			UpdateWindow();	

			return tem_strNewPicName;	
		}
		else
		{
// 			m_btnReadFinger.SetFourState();
 			GetDlgItem(IDC_BTN_FINGER)->EnableWindow(FALSE);

    		m_iCard->CloseCardPort();
			GetDlgItem(IDC_STA_INFO)->SetWindowText(_T("读卡失败，重新读卡！"));
			CRect   tem_rcStaInfo;
			GetDlgItem(IDC_STA_INFO)->GetWindowRect(&tem_rcStaInfo); 
			ScreenToClient(&tem_rcStaInfo);        
			InvalidateRect(&tem_rcStaInfo);
			UpdateWindow();	

			//清空picture控件
			m_strCopyImg = _T("");
			Self_ClearImg();

			return _T("");
		}

	}
	else
	{
		GetDlgItem(IDC_BTN_FINGER)->EnableWindow(FALSE);
		m_iCard->CloseCardPort();
		m_strCopyImg = _T("");
		Self_ClearImg();
		return _T("");
	}
	return tem_strNewPicName;
}


void CUDSSUBRECARD::Self_ShowPic(CString picpath)
{
	CWnd*  pWnd = NULL;
	pWnd = GetDlgItem(IDC_STA_CARDIMG);
	pDc = pWnd->GetDC();

	int     cx, cy;    //图像宽高
	CxImage* pImage;
	pImage = new CxImage; 
	CRect    tem_rectChildPic;

	pImage->Load(picpath);
	cx = pImage->GetWidth();
	cy = pImage->GetHeight();

	GetDlgItem(IDC_STA_CARDIMG)->GetWindowRect(&tem_rectChildPic);
	ScreenToClient(&tem_rectChildPic);

	int   m_neww = 0;
	int   m_newh = 0;
	float m_picw = (float)(tem_rectChildPic.right-tem_rectChildPic.left);
	float m_pich = (float)(tem_rectChildPic.bottom-tem_rectChildPic.top);
	float m_pic_ratio = m_picw/m_pich;
	float m_imgw = (float)cx;
	float m_imgh = (float)cy;
	float m_img_ratio = m_imgw/m_imgh;
	if (m_img_ratio>m_pic_ratio)
	{
		m_neww = (int)m_picw;
		m_newh = (int)((m_picw/m_imgw)*m_imgh);
		pImage->Resample(m_neww, m_newh);
	}
	else
	{
		m_newh = (int)m_pich;
		m_neww = (int)((m_pich/m_imgh)*m_imgw);
		pImage->Resample(m_neww, m_newh);
	}
	imageDC.CreateCompatibleDC(pDc);
	HBITMAP   m_hBitmap;
	m_hBitmap = pImage->MakeBitmap(imageDC);
	CBitmap m_bmp;
	m_bmp.Attach(m_hBitmap);
	imageDC.SelectObject(&m_bmp);
	pDc->BitBlt(0,0,tem_rectChildPic.right,tem_rectChildPic.bottom,&imageDC,0,0,SRCCOPY);

	::delete pImage;	
}


int CUDSSUBRECARD::Write2Excel(CString inipath, CString xlsdir)
{
	CString      tem_strExlPath = xlsdir;   //输出Excel路径
	CString      tem_strPath;               //ini+excel两个路径参数

	CString      tem_strExePath;
	TCHAR        tem_tcFilepath[MAX_PATH+1] = {0};
	GetModuleFileName(NULL, tem_tcFilepath, MAX_PATH);
	(_tcsrchr(tem_tcFilepath, _T('\\')))[1] = 0;

	tem_strExePath    = tem_tcFilepath;
	tem_strExePath   += _T("TxtToExcel.exe");   //配置文件路径
//	tem_strExePath.Replace(_T("\\"), _T("\\\\"));


	tem_strExlPath += _T("\\");
	tem_strExlPath += _T("身份证信息集.xls");
//	tem_strExlPath.Replace(_T("\\"), _T("\\\\"));


	tem_strPath.Format(_T("\"%s\" \"%s\""),inipath, tem_strExlPath);   //\"转义字符，\"=".用于在双引号之间，存在引号的情况，避免编译器出错

	SHELLEXECUTEINFO  tem_shellIt;
	memset(&tem_shellIt, 0, sizeof(SHELLEXECUTEINFO));
	tem_shellIt.cbSize       = sizeof(SHELLEXECUTEINFO);
	tem_shellIt.fMask        = SEE_MASK_NOCLOSEPROCESS;
	tem_shellIt.lpVerb       = _T("open");
	tem_shellIt.lpFile       = tem_strExePath;
	tem_shellIt.lpParameters = tem_strPath;
	tem_shellIt.nShow        = SW_SHOW;
	tem_shellIt.hwnd         = m_hWnd;
	ShellExecuteEx(&tem_shellIt);
	return 0;
}


void CUDSSUBRECARD::OnBnClickedBtnReadcard()
{
	CString  tem_strPicPath = Self_GetSystemTime(m_strPicBuffer);
	tem_strPicPath = Self_ReadCard(tem_strPicPath);
	if (!tem_strPicPath.IsEmpty())
	{
		Self_ShowPic(tem_strPicPath);
	}
}


void CUDSSUBRECARD::OnClickedChkAuto()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHK_AUTO))
	{
		m_BAutoRead           = TRUE;
		GetDlgItem(IDC_SLID_PERIOD)->EnableWindow(FALSE);
		//启动计时线程
		stcThreadInfo.hWnd    = GetSafeHwnd();
		stcThreadInfo.nTime   = m_nReadTime;
		stcThreadInfo.strInfo = _T("");

		hThreadHandle = CreateThread(NULL, 0, ThreadProc, &stcThreadInfo, 0, &dwThreadID);
	}
	else
	{
		m_BAutoRead = FALSE;
		GetDlgItem(IDC_SLID_PERIOD)->EnableWindow(TRUE);
		//关闭计时线程
		KillTimer(1);
		TerminateThread(hThreadHandle,0);
	}
	CRect    tem_rcRect;; 
	GetDlgItem(IDC_STA_PERIOD)->GetWindowRect(&tem_rcRect); 
	ScreenToClient(&tem_rcRect); 
	InvalidateRect(&tem_rcRect);
}


void CUDSSUBRECARD::OnBnClickedChkSave()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED ==IsDlgButtonChecked(IDC_CHK_SAVE))
	{
		m_BSaveCopy = TRUE;
	} 
	else
	{
		m_BSaveCopy = FALSE;
	}
}


DWORD WINAPI ThreadProc(LPVOID lpParam)  
{
	ThreadInfo*   tem_stcThreadInfo = (ThreadInfo*)lpParam;
	SetTimer(tem_stcThreadInfo->hWnd, 1, tem_stcThreadInfo->nTime, NULL);
	return TRUE;
}


void CUDSSUBRECARD::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString  tem_strPicPath = Self_GetSystemTime(m_strPicBuffer);
	tem_strPicPath = Self_ReadCard(tem_strPicPath);
	if (!tem_strPicPath.IsEmpty())
	{
		Self_ShowPic(tem_strPicPath);
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CUDSSUBRECARD::OnBnClickedBtnBegin()
{
	// TODO: 在此添加控件通知处理程序代码
	ShellExecute(NULL, _T("open"), m_strInfoBuffer, NULL, NULL, SW_SHOWNORMAL);
}


void CUDSSUBRECARD::Self_SaveCardCopy(IDCardInfo info, CString prontimg, CString backimg, CString outimg)
{
	if (m_BSaveCopy)
	{
		int        tem_nBackWidth  = 1580;    //背景纸的宽
		int        tem_nBackHeight = 2280;    //背景纸的高
		CString    tem_strImg         = info.imgpath;
		CString    tem_strName        = info.name;
		CString    tem_strSex         = info.sex;
		CString    tem_strNation      = info.nation;
		CString    tem_strBirth       = info.birth;
		CString    tem_strAddress     = info.address;
		CString    tem_strID          = info.IDnumber;
		CString    tem_strOffice      = info.office;
		CString    tem_strValidate    = info.validate;
		
		CString    tem_strBirth_year  = tem_strBirth.Mid(0,4);
		CString    tem_strBirth_month = tem_strBirth.Mid(5,2);
		CString    tem_strBirth_day   = tem_strBirth.Mid(8,2);

		//1)、图像加载------------------------------------------------------
		CxImage    pImageFront, pImageBack;
		CxImage    pImageCard;

		pImageFront.Load(prontimg, CMAX_IMAGE_FORMATS);
		pImageBack.Load(backimg, CMAX_IMAGE_FORMATS);
		pImageCard.Load(tem_strImg, CMAX_IMAGE_FORMATS);

 		pImageFront.Light(0, 5);    pImageFront.Erode(3);    pImageFront.UnsharpMask(); 
 		pImageBack.Light(0, 5);    pImageBack.Erode(3);    pImageBack.UnsharpMask();

		//2)图像透明--------------------------------------------------------
		pImageCard.AlphaCreate();
		pImageCard.AlphaSet(255);

		RGBQUAD   rgb = pImageCard.GetPixelColor(1, 120);     //	  rgb = {254, 254, 254, 0};

		int bpp = pImageCard.GetBpp();
		if (bpp!=24)
		{
			pImageCard.IncreaseBpp(24);
		}
		pImageCard.SetTransIndex(0);
		pImageCard.SetTransColor(rgb);
		pImageCard.Resample(310, 375);

		//3)图像合并---------------------------------------------------------
		pImageFront.Mix(pImageCard, CxImage::OpScreen, -656, -160, FALSE);       //原图复制，透明保留——可用

		//4)添加文字部分-----------------------------------------------------
		CxImage::CXTEXTINFO WMTxtInfo;  // 授权文字水印
		pImageFront.InitTextInfo( &WMTxtInfo );

		RGBQUAD   rgbTEXT = {0, 0, 0, 0};
		pImageFront.DrawString(0, 200, 124, tem_strName, rgbTEXT, _T("黑体"), 44, 400, 0, 0, false);
		pImageFront.DrawString(0, 200, 200, tem_strSex, rgbTEXT, _T("黑体"), 37, 400, 0, 0, false);
		pImageFront.DrawString(0, 420, 200, tem_strNation, rgbTEXT, _T("黑体"), 37, 400, 0, 0, false);
		pImageFront.DrawString(0, 200, 270, tem_strBirth_year, rgbTEXT, _T("黑体"), 40, 400, 0, 0, false);
		pImageFront.DrawString(0, 330, 270, tem_strBirth_month, rgbTEXT, _T("黑体"), 40, 400, 0, 0, false);
		pImageFront.DrawString(0, 430, 270, tem_strBirth_day, rgbTEXT, _T("黑体"), 40, 400, 0, 0, false);

		//每行最多显示11个中文字符
		int tem_nSize = tem_strAddress.GetLength();
		if (tem_nSize<=11)
		{
			pImageFront.DrawString(0, 200, 360, tem_strAddress, rgbTEXT, _T("黑体"), 37, 400, 0, 0, false);
		}
		else if (tem_nSize>11 && tem_nSize<=22)
		{
			CString tem_strFirst  = tem_strAddress.Mid(0, 11);
			pImageFront.DrawString(0, 200, 355, tem_strFirst, rgbTEXT, _T("黑体"), 37, 400, 0, 0, false);
			CString tem_strSecond = tem_strAddress.Mid(11);
			pImageFront.DrawString(0, 200, 410, tem_strSecond, rgbTEXT, _T("黑体"), 37, 400, 0, 0, false);
		}
		else if (tem_nSize>22 && tem_nSize<=33)
		{
			CString tem_strFirst  = tem_strAddress.Mid(0, 11);
			pImageFront.DrawString(0, 200, 355, tem_strFirst, rgbTEXT, _T("黑体"), 37, 400, 0, 0, false);
			CString tem_strSecond = tem_strAddress.Mid(11, 11);
			pImageFront.DrawString(0, 200, 410, tem_strSecond, rgbTEXT, _T("黑体"), 37, 400, 0, 0, false);
			CString tem_strThird  = tem_strAddress.Mid(22);
			pImageFront.DrawString(0, 200, 465, tem_strThird, rgbTEXT, _T("黑体"), 37, 400, 0, 0, false);
		}
		else
		{
			CString tem_strFirst  = tem_strAddress.Mid(0, 11);
			pImageFront.DrawString(0, 200, 355, tem_strFirst, rgbTEXT, _T("黑体"), 37, 400, 0, 0, false);
			CString tem_strSecond = tem_strAddress.Mid(11, 11);
			pImageFront.DrawString(0, 200, 410, tem_strSecond, rgbTEXT, _T("黑体"), 37, 400, 0, 0, false);
			CString tem_strThird  = tem_strAddress.Mid(22,11);
			pImageFront.DrawString(0, 200, 465, tem_strThird, rgbTEXT, _T("黑体"), 37, 400, 0, 0, false);
			CString tem_strFouth  = tem_strAddress.Mid(33);
			pImageFront.DrawString(0, 200, 520, tem_strFouth, rgbTEXT, _T("黑体"), 37, 400, 0, 0, false);

		}
		
		pImageFront.DrawString(0, 350, 560, tem_strID, rgbTEXT, _T("OCR-B 10 BT"), 50, 400, 0, 0, false);

		pImageBack.DrawString(0, 430, 485, tem_strOffice, rgbTEXT, _T("黑体"), 37, 200, 0, 0, false);
		pImageBack.DrawString(0, 430, 570, tem_strValidate, rgbTEXT, _T("黑体"), 40, 200, 0, 0, false);


		//双图合并
		CxImage    pImageMerge;
		COLORREF   rgbBack = RGB(255, 255, 255);
		pImageMerge.Create(tem_nBackWidth, tem_nBackHeight, 24);
		
		for (unsigned int i=0; i<pImageMerge.GetWidth(); i++)
		{
			for (unsigned int j=0; j<pImageMerge.GetHeight(); j++)
			{
				pImageMerge.SetPixelColor(i, j,rgbBack);
			}
		}

		pImageMerge.SetXDPI(200);
		pImageMerge.SetYDPI(200);

		pImageFront.SetXDPI(200);
		pImageFront.SetYDPI(200);
		pImageFront.Resample(674, 426);
		int   tem_nOffsetX = (tem_nBackWidth-pImageFront.GetWidth())/2;
		int   tem_nOffsetY = (tem_nBackHeight-3*pImageFront.GetHeight()/2);
		pImageMerge.MixFrom(pImageFront, tem_nOffsetX, tem_nOffsetY);

		pImageBack.SetXDPI(200);
		pImageBack.SetYDPI(200);
		pImageBack.Resample(674, 426);
		tem_nOffsetY = tem_nBackHeight-3*pImageFront.GetHeight();
		pImageMerge.MixFrom(pImageBack, tem_nOffsetX, tem_nOffsetY);


		pImageMerge.Save(outimg, CXIMAGE_FORMAT_JPG);

//		ShellExecute(NULL, _T("print"), outimg, NULL, NULL, SW_SHOWNORMAL);
	}
}


void CUDSSUBRECARD::Self_ShowCopyImg(CString img)
{
	CxImage* pImage;
	pImage = new CxImage;
	pImage->Load(img, CXIMAGE_FORMAT_JPG);
	CWnd*  pWnd = NULL;
	pWnd = GetDlgItem(IDC_STA_COPYIMG);
	pDc = pWnd->GetDC();

	CRect    tem_rectChildPic;
	GetDlgItem(IDC_STA_COPYIMG)->GetWindowRect(&tem_rectChildPic);
	ScreenToClient(&tem_rectChildPic);

	pImage->Resample(tem_rectChildPic.Width(), tem_rectChildPic.Height());
	imageDC.CreateCompatibleDC(pDc);
	HBITMAP   m_hBitmap;
	m_hBitmap = pImage->MakeBitmap(imageDC);
	CBitmap m_bmp;
	m_bmp.Attach(m_hBitmap);
	imageDC.SelectObject(&m_bmp);
	pDc->BitBlt(0,0,tem_rectChildPic.right,tem_rectChildPic.bottom,&imageDC,0,0,SRCCOPY);	

}


void CUDSSUBRECARD::OnDblclkStaCopyimg()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_strCopyImg != _T("")&&m_BSaveCopy)
	{
		ShellExecute(NULL, _T("open"), m_strCopyImg, NULL, NULL, SW_SHOWNORMAL);
	}
}


void CUDSSUBRECARD::Self_ClearImg(void)
{
	CWnd*  tem_pWnd = NULL;
	tem_pWnd = GetDlgItem(IDC_STA_CARDIMG);
	CDC*   tem_pDc;
	tem_pDc = tem_pWnd->GetDC();
	CRect   tem_rcPicRect;
	GetDlgItem(IDC_STA_CARDIMG)->GetWindowRect(&tem_rcPicRect);
	ScreenToClient(&tem_rcPicRect);

	tem_pDc->FillSolidRect(0, 0, tem_rcPicRect.Width(), tem_rcPicRect.Height(),RGB(176,198,191));
	m_conCardImg.SetBitmap(NULL);

	tem_pWnd = GetDlgItem(IDC_STA_COPYIMG);
	tem_pDc = tem_pWnd->GetDC();
	GetDlgItem(IDC_STA_COPYIMG)->GetWindowRect(&tem_rcPicRect);
	ScreenToClient(&tem_rcPicRect);

	tem_pDc->FillSolidRect(0, 0, tem_rcPicRect.Width(), tem_rcPicRect.Height(),RGB(176,198,191));
	m_conCardImg.SetBitmap(NULL);
}


void CUDSSUBRECARD::OnCustomdrawSlidPeriod(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_nReadTime            = m_conPeriod.GetPos();
	float    tem_fPeriod   = (float)m_nReadTime*1.0/1000;
	CString  tem_strPeriod = _T("");
	tem_strPeriod.Format(_T("%.1f"), tem_fPeriod);
	GetDlgItem(IDC_STA_PERIODV)->SetWindowText(tem_strPeriod);
	CRect    tem_rcRect;; 
	GetDlgItem(IDC_STA_PERIODV)->GetWindowRect(&tem_rcRect); 
	ScreenToClient(&tem_rcRect); 
	InvalidateRect(&tem_rcRect);

	//写入ini文件
	CString tem_strSens = _T("");
	tem_strSens.Format(_T("%d"), m_nReadTime);
	::WritePrivateProfileString(_T("Resource"), _T("ReadSens"), tem_strSens, m_strIniPath); 

	*pResult = 0;
}


void CUDSSUBRECARD::Self_ShowFingerImg(CString img)
{
	CxImage* pImage;
	pImage = new CxImage;
	pImage->Load(img, CXIMAGE_FORMAT_JPG);
	CWnd*  pWnd = NULL;
	pWnd = GetDlgItem(IDC_PIC_FINGERORINT);
	pDc = pWnd->GetDC();

	CRect    tem_rectChildPic;
	GetDlgItem(IDC_PIC_FINGERORINT)->GetWindowRect(&tem_rectChildPic);
	ScreenToClient(&tem_rectChildPic);

	pImage->Resample(tem_rectChildPic.Width(), tem_rectChildPic.Height());
	imageDC.CreateCompatibleDC(pDc);
	HBITMAP   m_hBitmap;
	m_hBitmap = pImage->MakeBitmap(imageDC);
	CBitmap m_bmp;
	m_bmp.Attach(m_hBitmap);
	imageDC.SelectObject(&m_bmp);
	pDc->BitBlt(0,0,tem_rectChildPic.right,tem_rectChildPic.bottom,&imageDC,0,0,SRCCOPY);
}

//指纹采集与指纹识别--------------------------------------------------------------
BYTE penple_code[10 * 2];
void CUDSSUBRECARD::OnBnClickedBtnFinger()
{
	// TODO: 在此添加控件通知处理程序代码
	float      tem_fProximity  = 0;
	CString    tem_strImgPath  = m_strFingerPath;
	CString    tem_strDatPath  = m_strFingerPath;
	tem_strImgPath    += m_editName;
	tem_strImgPath    += m_editCardnum;
	tem_strImgPath    += _T(".bmp");

	tem_strDatPath    += m_editName;
	tem_strDatPath    += m_editCardnum;
	tem_strDatPath    += _T(".dat");

//	tem_strImgPath = _T("C:\\Users\\Administrator\\Desktop\\FingerInfo\\big.bmp");
//	tem_strImgPath = _T("C:\\Users\\Administrator\\Desktop\\FingerInfo\\small.bmp");
	//1、获取并显示指纹信息
	Self_ReadFigerPrint(tem_strImgPath);

	//2、比对并显示结果
	tem_fProximity = Self_ExMatchFeatures(tem_strImgPath, tem_strDatPath);

	//3、指纹匹配则确定为哪个手指
	CString tem_strMatchResult = _T("");
	if (tem_fProximity>0.6)
	{
		//匹配成功
		switch(penple_code[2*m_nFigerIndex])
		{
		case 11: 
			m_strFingerIndex = _T("右手拇指"); 
			break;
		case 12: 
			m_strFingerIndex = _T("右手食指"); 
			break;
		case 13: 
			m_strFingerIndex = _T("右手中指"); 
			break;
		case 14: 
			m_strFingerIndex = _T("右手环指"); 
			break;
		case 15: 
			m_strFingerIndex =_T( "右手小指"); 
			break;
		case 16: 
			m_strFingerIndex = _T("左手拇指"); 
			break;
		case 17: 
			m_strFingerIndex = _T("左手食指"); 
			break;
		case 18: 
			m_strFingerIndex = _T("左手中指"); 
			break;
		case 19: 
			m_strFingerIndex = _T("左手环指"); 
			break;
		case 20: 
			m_strFingerIndex = _T("左手小指"); 
			break;
		case 97: 
			m_strFingerIndex = _T("右手不确定指"); 
			break;
		case 98: 
			m_strFingerIndex = _T("左手不确定指"); 
			break;
		case 99: 
			m_strFingerIndex = _T("不确定指"); 
			break;
		default: 
			break;
		}

		tem_strMatchResult.Format(_T("匹配\n相似度： %.1f%%"), tem_fProximity*100);
		GetDlgItem(IDC_STA_MATCHV)->SetWindowText(tem_strMatchResult);
		GetDlgItem(IDC_STA_FINGERINDEX)->SetWindowText(m_strFingerIndex);
	}
	else
	{
		//匹配失败
		m_strFingerIndex = _T("");
		tem_strMatchResult.Format(_T("不匹配\n相似度： %.1f%%"), tem_fProximity*100);
		GetDlgItem(IDC_STA_MATCHV)->SetWindowText(tem_strMatchResult);
		GetDlgItem(IDC_STA_FINGERINDEX)->SetWindowText(m_strFingerIndex);
	}

	CRect   tem_rcStaInfo;
	GetDlgItem(IDC_STA_MATCHV)->GetWindowRect(&tem_rcStaInfo); 
	ScreenToClient(&tem_rcStaInfo);        
	InvalidateRect(&tem_rcStaInfo);

	GetDlgItem(IDC_STA_FINGERINDEX)->GetWindowRect(&tem_rcStaInfo); 
	ScreenToClient(&tem_rcStaInfo);        
	InvalidateRect(&tem_rcStaInfo);
	UpdateWindow();	
}



BYTE FPRawData[FPRIMG_MAX_SIZE];                            //采集到的指纹图像的元数据
BYTE g_FPBmpData[BMP_HEAD_SIZE + FPRIMG_MAX_SIZE];          //bmp图像数据指针，用于保存为bmp图像
BYTE g_FPBmpDataTemp[360*256];                              //bmp图像数据指针，用于特征提取
CString CUDSSUBRECARD::Self_ReadFigerPrint(CString imgpath)
{
	
	//1、获取并转换为bmp图像----------------------------------------------------------------
	if (LIVESCAN_Init() == LIVESCAN_SUCCESS)
	{
		int  tem_nRC = LIVESCAN_GetCaptWindow(0, &m_nOriX, &m_nOriY, &m_nWidth, &m_nHeight);
		char szDesc[1024] = {0};
		memset(szDesc, 0, 1024);
		tem_nRC = LIVESCAN_GetDesc(szDesc);
		if (tem_nRC != LIVESCAN_SUCCESS)
		{
			return _T("");
		}
		tem_nRC = LIVESCAN_BeginCapture(0);
		if (tem_nRC != LIVESCAN_SUCCESS)
		{
			return _T("");
		}
		//1、获取图像Raw数据----------------------------
		tem_nRC = LIVESCAN_GetFPRawData(0, FPRawData);
		//2、将Raw数据转为Bmp---------------------------
		USES_CONVERSION;
		char* tem_cImgPath = T2A(imgpath);
		imgpath.ReleaseBuffer();
		if (tem_nRC == LIVESCAN_SUCCESS)
		{
			//1)、写入文件头----------------------------
			WriteHead(g_FPBmpData, FPRawData, m_nWidth, m_nHeight);      //元数据为RAW，然后将其装换位bmp
			//2)、写入文件数据--------------------------
			FPBmpDataToBmp(g_FPBmpData, BMP_HEAD_SIZE + m_nWidth * m_nHeight, tem_cImgPath);
			tem_nRC = LIVESCAN_EndCapture(0);
			if (tem_nRC != LIVESCAN_SUCCESS)
			{
				return _T("");
			}
			tem_nRC = LIVESCAN_Close();
			if(tem_nRC != LIVESCAN_SUCCESS)
			{
				return _T("");
			}	
		}

		
		//2、显示bmp图像-------------------------------------------------------------------
		Self_ShowFingerImg(imgpath);
		LoadDll();
		tem_nRC = FP_Begin();
		unsigned char tem_cScore = 0;
		tem_nRC = FP_GetQualityScore(FPRawData, &tem_cScore);      //获取指纹图像的质量值,图像质量值 00H - 64H;用于评价图像质量，暂时未用
		CString tem_strScore;
		tem_strScore.Format(_T("%d"), tem_cScore);
		tem_nRC = FP_End();
		FreeDll();
	}
	
//	Self_ShowFingerImg(imgpath);
	return imgpath;
}


int CUDSSUBRECARD::WriteHead(unsigned char* Output, unsigned char* Input, int nWidth, int nHeight)
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


void CUDSSUBRECARD::FPBmpDataToBmp(BYTE* FPBmpData, DWORD dataLen, char* bmpFileName)
{
	FILE *nf = fopen(bmpFileName, "wb");
	if (!nf)
		return;

	fwrite(FPBmpData, sizeof(unsigned char), dataLen, nf);
	fclose(nf);
}


BYTE FPcharDatatemp[512];     //特征数据指针
BYTE pTemplate[512*10];
float CUDSSUBRECARD::Self_ExMatchFeatures(CString imgpath, CString datpath)
{
	m_strFingerIndex = _T("");
	float  tem_fMaxValue  = -1;

	USES_CONVERSION;
	char* tem_cImgPath = T2A(imgpath);
	char* tem_cDatPath = T2A(datpath);

	LoadDll();
	//1、读取图像文件数据，保存数据指针-----------------------------------
	FILE*    tem_fileImg;
	tem_fileImg = fopen(tem_cImgPath, "rb");
	fseek(tem_fileImg, 1078, SEEK_SET);
	long     tem_nFileLength = fread(g_FPBmpDataTemp, sizeof(char), 360*256, tem_fileImg);
	fclose(tem_fileImg);

	//2、提取数据特征，保存为特征数据指针---------------------------------
	int       tem_nRC = FP_Begin();
	tem_nRC = FP_FeatureExtract(0, 0, g_FPBmpDataTemp, FPcharDatatemp);   //提取图像特征
	if (tem_nRC != LIVESCAN_SUCCESS)
	{
		tem_nRC = FP_End();
		FreeDll();
		return 0;    //提取指纹特征失败，请重新采集指纹
	}

	//3、读取指纹文件数据-------------------------------------------------
	int    tem_nReadItems   = 0;     //读取到的项数
	int    tem_nFileLenghth = 0;
	int    tem_nFileSize    = 0;     //得到文件位置相对于文件首的偏移字节数
	float  tem_fProximity   = 0;

	FILE* tem_fileDat;
	if ((tem_fileDat = fopen(tem_cDatPath, "rb")) == NULL)
	{
		tem_nRC = FP_End();
		FreeDll();
		return 0;    //没有找到指纹dat文件
	}
	fseek(tem_fileDat, 0, SEEK_END);
	tem_nFileSize = ftell(tem_fileDat);
	rewind(tem_fileDat);
	tem_nReadItems = fread(pTemplate, sizeof(char), tem_nFileSize, tem_fileDat);
	fclose(tem_fileDat);

	//获取Dat文件中指纹个数
	int  tem_nFingerNum = tem_nFileSize/512;

	BYTE temp[512];
	for (int i=0; i<tem_nFingerNum; i++)
	{
		//逐个取值dat中指纹数据-------------
		for (int j=0; j<512; j++)
		{
			temp[j] = pTemplate[512*i + j];
		}
		//取出对应dat中的指纹定位数据--------
		penple_code[2 * i] = pTemplate[512*i + 5];

		tem_nRC = FP_FeatureMatch(FPcharDatatemp, temp, &tem_fProximity);
		if (tem_nRC == 1)
		{
			//匹配成功，记录相似度
			if (tem_fProximity>tem_fMaxValue)
			{
				tem_fMaxValue = tem_fProximity;
				m_nFigerIndex = i;
			}
		}
	}
	tem_nRC = FP_End();
	FreeDll();

	//返回一个相似度最大的
	return tem_fMaxValue;
}

//测试不同指纹采集器是否能够通用
void CUDSSUBRECARD::OnBnClickedBtnTestdiffer()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CUDSSUBRECARD::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//检查线程是否关闭，若线程未关闭，关闭线程
	DWORD dwExitCode = 0;  
	GetExitCodeThread(hThreadHandle, &dwExitCode);
	if (dwExitCode == STILL_ACTIVE)  
	{  
		KillTimer(1);
		TerminateThread(hThreadHandle,0);
	}  
	delete m_iCard;

	//清空Finger文件夹内除bmp外所有图像
	BOOL        bGetRc      = NULL;
	CFileFind   temFinder   = NULL;
	CString     strFileName = NULL;
	CString     strSubDoc   = NULL;
	CString     strFilePath = NULL;

	bGetRc = temFinder.FindFile(m_strFingerPath + "*.*");
	while (bGetRc)
	{
		bGetRc = temFinder.FindNextFile();
		if (temFinder.IsDots())
		{
			//为缺省目录
			continue;
		}
		else if (temFinder.IsDirectory())
		{
			//检测为子文件夹，保存备用
			continue;
		} 
		else if (temFinder.IsCompressed())
		{
			//压缩文件
			continue;
		}
		else if (temFinder.IsSystem())
		{
			//系统文件
			continue;
		}
		else
		{
			//检测为文件，进行过滤
			strFileName = temFinder.GetFileName();				
			CString  strMiddle = strFileName;
			int      nIndex = strMiddle.ReverseFind('.');
			if (nIndex != -1)
			{
				strMiddle = strMiddle.Mid(nIndex+1);
				strMiddle.MakeLower();
				if (strMiddle == _T("dat") || strMiddle == _T("wlt"))		
				{
					//区别图像文件和非图像文件，将FieName保存
					strFilePath = "";
					strFilePath = m_strFingerPath + strFileName;	
					DeleteFile(strFilePath);
				}
			}

		}
	}	

	CDialogEx::OnClose();
}

