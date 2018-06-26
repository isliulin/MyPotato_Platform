// UDSSUBRENAME.cpp : 实现文件
//

#include "stdafx.h"
#include "UDSPlatformV1.7.h"
#include "UDSSUBRENAME.h"
#include "afxdialogex.h"


extern std::vector<CString>   m_vcFileName;    //文件名
extern std::vector<CString>   m_vcFilePath;    //图像文件路径
extern std::vector<CString>   m_vcBussPath;    //pdf+txt+图像路径 
extern BOOL                   g_BRenamed;
// CUDSSUBRENAME 对话框

IMPLEMENT_DYNAMIC(CUDSSUBRENAME, CDialogEx)

CUDSSUBRENAME::CUDSSUBRENAME(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUDSSUBRENAME::IDD, pParent)
{

	m_EditFirstKey = _T("");
	m_EditSecondKey = _T("");
	m_EditThirdKey = _T("");
	//  m_EditLastKey = _T("");
	m_EditSeparator = _T("");
	m_EditLastName = 0;
	m_StaSlctdNumber = _T("");
	m_StaDidRatio = _T("");
}

CUDSSUBRENAME::~CUDSSUBRENAME()
{
}

void CUDSSUBRENAME::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RENAME, m_conListRename);
	DDX_Control(pDX, IDC_COM_FIRST, m_conPrename);
	//  DDX_Control(pDX, IDC_EDIT_SECOND, m_conFirstKey);
	//  DDX_Control(pDX, IDC_EDIT_THIRD, m_conSecondKey);
	//  DDX_Control(pDX, IDC_EDIT_FOUTH, m_conThirdKey);
	//  DDX_Control(pDX, IDC_EDIT_LAST, m_conLastName);
	//  DDX_Control(pDX, IDC_EDIT_SECOND, m_EditFirstKey);
	DDX_Text(pDX, IDC_EDIT_SECOND, m_EditFirstKey);
	DDX_Text(pDX, IDC_EDIT_THIRD, m_EditSecondKey);
	DDX_Text(pDX, IDC_EDIT_FOUTH, m_EditThirdKey);
	//  DDX_Text(pDX, IDC_EDIT_LAST, m_EditLastKey);
	DDX_Text(pDX, IDC_EDIT_SEPARATOR, m_EditSeparator);
	DDX_Text(pDX, IDC_EDIT_LAST, m_EditLastName);
	DDV_MinMaxInt(pDX, m_EditLastName, 0, 9999);
	DDX_Text(pDX, IDC_STA_SLCTEDNUMBER, m_StaSlctdNumber);
	DDX_Control(pDX, IDC_PROGRESS_RENAME, m_conProRename);
	DDX_Text(pDX, IDC_STA_DIDIT, m_StaDidRatio);
}


BEGIN_MESSAGE_MAP(CUDSSUBRENAME, CDialogEx)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COM_FIRST, &CUDSSUBRENAME::OnSelchangeComFirst)
	ON_EN_CHANGE(IDC_EDIT_SECOND, &CUDSSUBRENAME::OnChangeEditSecond)
	ON_EN_CHANGE(IDC_EDIT_THIRD, &CUDSSUBRENAME::OnChangeEditThird)
	ON_EN_CHANGE(IDC_EDIT_FOUTH, &CUDSSUBRENAME::OnChangeEditFouth)
	ON_EN_CHANGE(IDC_EDIT_LAST, &CUDSSUBRENAME::OnChangeEditLast)
	ON_BN_CLICKED(IDOK, &CUDSSUBRENAME::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CUDSSUBRENAME::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_EDIT_SEPARATOR, &CUDSSUBRENAME::OnChangeEditSeparator)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHK_ALLST, &CUDSSUBRENAME::OnClickedChkAllst)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_RENAME, &CUDSSUBRENAME::OnItemchangedListRename)
END_MESSAGE_MAP()


// CUDSSUBRENAME 消息处理程序


BOOL CUDSSUBRENAME::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	/*****************************
	*
	* 1、变量初始化
	* 2、Combox初始化
	* 3、Edit初始化
	* 4、ListCtrl初始化
	* 5、Static初始化
	* 6、Checkbox初始化
	* 7、ProgressBar初始化
	*
	*****************************/
	/*2、Combox初始化*/
	m_conPrename.InsertString(0, _T("空"));
	m_conPrename.InsertString(1, _T("日期(年月日)"));
	m_conPrename.InsertString(2, _T("时间(时分)"));
	m_conPrename.SetCurSel(0);

	/*3、Edit初始化*/
	m_EditFirstKey  = _T("");
	m_EditSecondKey = _T("");
	m_EditThirdKey  = _T("");
	m_EditLastName  = 1;
	m_EditSeparator = _T("_");
	UpdateData(FALSE);

	m_strPreName    = _T("");
	m_strFirstKey   = _T("");
	m_strSecondKey  = _T("");
	m_strThirdKey   = _T("");
	m_strLastName.Format(_T("%d"), m_EditLastName);
	m_strSeparator  = _T("_");

	/*4、ListCtrl初始化*/
	//0)获取ListCtrl尺寸
	CRect    tem_rcListCtrl;
	GetDlgItem(IDC_LIST_RENAME)->GetClientRect(&tem_rcListCtrl);
	int      tem_nListWidth = tem_rcListCtrl.Width();
	//1)设置ListCtrl风格
	DWORD     tem_dwStyle;
	tem_dwStyle  = LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT;
	m_conListRename.SetExtendedStyle(tem_dwStyle);
	//2)添加ListCtrl列
	m_conListRename.InsertColumn(0, _T("选择"), LVCFMT_CENTER, 40);
	m_conListRename.InsertColumn(1, _T("原文件名"), LVCFMT_CENTER, (tem_nListWidth-40)/2);
	m_conListRename.InsertColumn(2, _T("新文件名"),LVCFMT_CENTER, (tem_nListWidth-40)/2);
	//3)添加ListCtrl行，即添加ListCtrl信息
	for (unsigned int i=0; i<m_vcFileName.size(); i++)
	{
		//(1)新文件名FileName
		int       tem_nLastName   = i+1;
		CString   tem_strLastName, tem_strExtension;	
		tem_strExtension = PathFindExtension(m_vcFileName[i]);
		tem_strLastName.Format(_T("%04d"), tem_nLastName);
		tem_strLastName += tem_strExtension;

		//(2)插入ListCtrl
		m_conListRename.InsertItem(i, _T(""));
		m_conListRename.SetCheck(i, TRUE);
		m_conListRename.SetItemText(i, 1, m_vcFileName[i]);
		m_conListRename.SetItemText(i, 2, tem_strLastName);

		//(3)获取新文件名
		m_vcNewFileName.push_back(tem_strLastName);

		//(4)获取新显示路径
		CString   tem_strFilePath = _T("");
		tem_strExtension.MakeLower();
		if (tem_strExtension == _T(".jpg") || tem_strExtension == _T(".jpeg")||
			tem_strExtension == _T(".tif") || tem_strExtension == _T(".tiff")||
			tem_strExtension == _T(".bmp") || tem_strExtension == _T(".png") )	
		{
			tem_strFilePath = m_vcFilePath[i];
			//a、获取文件目录
			TCHAR*   tem_cFilePath;
			tem_cFilePath = tem_strFilePath.GetBuffer();
			tem_strFilePath.ReleaseBuffer();
			PathRemoveFileSpec(tem_cFilePath);
			tem_strFilePath  = tem_cFilePath;
			tem_strFilePath += _T("\\");
			tem_strFilePath += tem_strLastName;
		}
		else
		{
			tem_strFilePath = m_vcFilePath[i];
		}
		m_vcNewFilePath.push_back(tem_strFilePath);


		//(5)获取新文件路径
		CString   tem_strBussPath = _T("");
		tem_strBussPath = m_vcBussPath[i];
		TCHAR*   tem_cBussPath;
		tem_cBussPath = tem_strBussPath.GetBuffer();
		tem_strBussPath.ReleaseBuffer();
		PathRemoveFileSpec(tem_cBussPath);
		tem_strBussPath  = tem_cBussPath;
		tem_strBussPath += _T("\\");
		tem_strBussPath += tem_strLastName;	
		m_vcNewBussPath.push_back(tem_strBussPath);

		/*5、Static初始化*/
		CString   tem_strInsfo = _T("说明：\n    1、命名由前缀、字段一、字段二、字段三、计数起始位、分隔符六部分组成，不输入则为空；\n    2、计数起始位默认为1，分隔符默认为下划线；\n");
		GetDlgItem(IDC_STA_EXPLAIN2)->SetWindowText(tem_strInsfo);

		m_StaSlctdNumber = _T("已选： ");
		int tem_nSlcNumber = m_vcFileName.size();
		CString  tem_strSlcNumber;
		tem_strSlcNumber.Format(_T("%d"), tem_nSlcNumber);
		m_StaSlctdNumber += tem_strSlcNumber;
		UpdateData(FALSE);

		/*6、Checkbox初始化*/
		((CButton*)GetDlgItem(IDC_CHK_ALLST))->SetCheck(TRUE);

		/*7、ProgressBar初始化*/
		m_conProRename.SetRange(0, m_vcFileName.size());
		m_conProRename.SetPos(0);

		/*7、进度显示Static初始化*/
		m_StaDidRatio = _T("0%");
		UpdateData(FALSE);




	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CUDSSUBRENAME::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
}


HBRUSH CUDSSUBRENAME::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd->GetDlgCtrlID() == IDC_STA_EXPLAIN)
	{
		 CFont    tem_ftFont;
		 tem_ftFont.CreatePointFont(150, _T("宋体"));
		 pDC->SelectObject(&tem_ftFont);
	}
	else if (pWnd->GetDlgCtrlID() == IDC_STA_EXPLAIN2)
	{
		CFont    tem_ftFont;
		tem_ftFont.CreatePointFont(120, _T("楷体"));
		pDC->SelectObject(&tem_ftFont);
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CUDSSUBRENAME::OnSelchangeComFirst()
{
	// TODO: 在此添加控件通知处理程序代码
	int         tem_nSlcIndex = m_conPrename.GetCurSel();
	SYSTEMTIME  tem_stDateTime;
	CString     tem_strDate, tem_strTime;
	switch(tem_nSlcIndex)
	{
	case 0:
		//空
		m_strPreName = _T("");
		break;
	case 1:
		//日期
		GetLocalTime(&tem_stDateTime);
		tem_strDate.Format(_T("%d%02d%02d"), tem_stDateTime.wYear, tem_stDateTime.wMonth, tem_stDateTime.wDay);
		m_strPreName = tem_strDate;
		break;
	case 2:
		//时间
		GetLocalTime(&tem_stDateTime);
		tem_strTime.Format(_T("%02d%02d"), tem_stDateTime.wHour, tem_stDateTime.wMinute);
		m_strPreName = tem_strTime;
		break;
	default:
		m_strPreName = _T("");
		break;
	}

	//获得完整文件名
	CString   tem_strNewWholName = Self_GetNewName();
	UpdateData(TRUE);
	m_nLastName = m_EditLastName;
	int j=0;
	for (unsigned int i=0; i<m_vcFileName.size(); i++)
	{
		if (!(m_conListRename.GetItemState(i, LVIS_SELECTED)==LVIS_SELECTED||m_conListRename.GetCheck(i)))
		{
			m_conListRename.SetItemText(i, 2, m_vcFileName[i]);
			m_vcNewFileName[i] = m_vcFileName[i];
			m_vcNewFilePath[i] = m_vcFilePath[i];
			m_vcNewBussPath[i] = m_vcBussPath[i];
			continue;
		}
		//1)文件名FileName
		int       tem_nLastName = j+m_nLastName;
		CString   tem_strLastName;
		tem_strLastName.Format(_T("%04d"), tem_nLastName);
		CString   tem_strExtension;	
		tem_strExtension = PathFindExtension(m_vcFileName[i]);

		CString   tem_strPrename = tem_strNewWholName;
		tem_strPrename += tem_strLastName;
		tem_strPrename += tem_strExtension;
		
		m_vcNewFileName[i] = tem_strPrename;
		m_conListRename.SetItemText(i, 2, tem_strPrename);

		//2)显示文件路径FilePath
		CString   tem_strFilePath = _T("");
		tem_strExtension.MakeLower();
		if (tem_strExtension == _T(".jpg") || tem_strExtension == _T(".jpeg")||
			tem_strExtension == _T(".tif") || tem_strExtension == _T(".tiff")||
			tem_strExtension == _T(".bmp") || tem_strExtension == _T(".png") )	
		{
			tem_strFilePath = m_vcFilePath[i];
			//a、获取文件目录
			TCHAR*   tem_cFilePath;
			tem_cFilePath = tem_strFilePath.GetBuffer();
			tem_strFilePath.ReleaseBuffer();
			PathRemoveFileSpec(tem_cFilePath);
			tem_strFilePath  = tem_cFilePath;
			tem_strFilePath += _T("\\");
			tem_strFilePath += tem_strPrename;
		}
		else
		{
			tem_strFilePath = m_vcFilePath[i];
		}
		m_vcNewFilePath[i] = tem_strFilePath;
		//3)操作文件路径BussPath
		CString   tem_strBussPath = _T("");
		tem_strBussPath = m_vcBussPath[i];
		TCHAR*   tem_cBussPath;
		tem_cBussPath = tem_strBussPath.GetBuffer();
		tem_strBussPath.ReleaseBuffer();
		PathRemoveFileSpec(tem_cBussPath);
		tem_strBussPath  = tem_cBussPath;
		tem_strBussPath += _T("\\");
		tem_strBussPath += tem_strPrename;	
		
		m_vcNewBussPath[i] = tem_strBussPath;	

		j++;
	}
}


void CUDSSUBRENAME::OnChangeEditSecond()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	//获得完整文件名
	CString   tem_strNewWholName = Self_GetNewName();
	UpdateData(TRUE);
	m_nLastName = m_EditLastName;
	int j=0;
	for (unsigned int i=0; i<m_vcFileName.size(); i++)
	{
		if (!(m_conListRename.GetItemState(i, LVIS_SELECTED)==LVIS_SELECTED||m_conListRename.GetCheck(i)))
		{
			m_conListRename.SetItemText(i, 2, m_vcFileName[i]);
			m_vcNewFileName[i] = m_vcFileName[i];
			m_vcNewFilePath[i] = m_vcFilePath[i];
			m_vcNewBussPath[i] = m_vcBussPath[i];
			continue;
		}
		//1)文件名FileName
		int       tem_nLastName = j+m_nLastName;
		CString   tem_strLastName;
		tem_strLastName.Format(_T("%04d"), tem_nLastName);
		CString   tem_strExtension;	
		tem_strExtension = PathFindExtension(m_vcFileName[i]);

		CString   tem_strPrename = tem_strNewWholName;
		tem_strPrename += tem_strLastName;
		tem_strPrename += tem_strExtension;

		m_vcNewFileName[i] = tem_strPrename;
		m_conListRename.SetItemText(i, 2, tem_strPrename);

		//2)显示文件路径FilePath
		CString   tem_strFilePath = _T("");
		tem_strExtension.MakeLower();
		if (tem_strExtension == _T(".jpg") || tem_strExtension == _T(".jpeg")||
			tem_strExtension == _T(".tif") || tem_strExtension == _T(".tiff")||
			tem_strExtension == _T(".bmp") || tem_strExtension == _T(".png") )	
		{
			tem_strFilePath = m_vcFilePath[i];
			//a、获取文件目录
			TCHAR*   tem_cFilePath;
			tem_cFilePath = tem_strFilePath.GetBuffer();
			tem_strFilePath.ReleaseBuffer();
			PathRemoveFileSpec(tem_cFilePath);
			tem_strFilePath  = tem_cFilePath;
			tem_strFilePath += _T("\\");
			tem_strFilePath += tem_strPrename;
		}
		else
		{
			tem_strFilePath = m_vcFilePath[i];
		}
		m_vcNewFilePath[i] = tem_strFilePath;
		//3)操作文件路径BussPath
		CString   tem_strBussPath = _T("");
		tem_strBussPath = m_vcBussPath[i];
		TCHAR*   tem_cBussPath;
		tem_cBussPath = tem_strBussPath.GetBuffer();
		tem_strBussPath.ReleaseBuffer();
		PathRemoveFileSpec(tem_cBussPath);
		tem_strBussPath  = tem_cBussPath;
		tem_strBussPath += _T("\\");
		tem_strBussPath += tem_strPrename;	

		m_vcNewBussPath[i] = tem_strBussPath;

		j++;
	}
}


void CUDSSUBRENAME::OnChangeEditThird()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	//获得完整文件名
	CString   tem_strNewWholName = Self_GetNewName();
	UpdateData(TRUE);
	m_nLastName = m_EditLastName;
	int j=0;
	for (unsigned int i=0; i<m_vcFileName.size(); i++)
	{
		if (!(m_conListRename.GetItemState(i, LVIS_SELECTED)==LVIS_SELECTED||m_conListRename.GetCheck(i)))
		{
			m_conListRename.SetItemText(i, 2, m_vcFileName[i]);
			m_vcNewFileName[i] = m_vcFileName[i];
			m_vcNewFilePath[i] = m_vcFilePath[i];
			m_vcNewBussPath[i] = m_vcBussPath[i];
			continue;
		}
		//1)文件名FileName
		int       tem_nLastName = j+m_nLastName;
		CString   tem_strLastName;
		tem_strLastName.Format(_T("%04d"), tem_nLastName);
		CString   tem_strExtension;	
		tem_strExtension = PathFindExtension(m_vcFileName[i]);

		CString   tem_strPrename = tem_strNewWholName;
		tem_strPrename += tem_strLastName;
		tem_strPrename += tem_strExtension;

		m_vcNewFileName[i] = tem_strPrename;
		m_conListRename.SetItemText(i, 2, tem_strPrename);

		//2)显示文件路径FilePath
		CString   tem_strFilePath = _T("");
		tem_strExtension.MakeLower();
		if (tem_strExtension == _T(".jpg") || tem_strExtension == _T(".jpeg")||
			tem_strExtension == _T(".tif") || tem_strExtension == _T(".tiff")||
			tem_strExtension == _T(".bmp") || tem_strExtension == _T(".png") )	
		{
			tem_strFilePath = m_vcFilePath[i];
			//a、获取文件目录
			TCHAR*   tem_cFilePath;
			tem_cFilePath = tem_strFilePath.GetBuffer();
			tem_strFilePath.ReleaseBuffer();
			PathRemoveFileSpec(tem_cFilePath);
			tem_strFilePath  = tem_cFilePath;
			tem_strFilePath += _T("\\");
			tem_strFilePath += tem_strPrename;
		}
		else
		{
			tem_strFilePath = m_vcFilePath[i];
		}
		m_vcNewFilePath[i] = tem_strFilePath;
		//3)操作文件路径BussPath
		CString   tem_strBussPath = _T("");
		tem_strBussPath = m_vcBussPath[i];
		TCHAR*   tem_cBussPath;
		tem_cBussPath = tem_strBussPath.GetBuffer();
		tem_strBussPath.ReleaseBuffer();
		PathRemoveFileSpec(tem_cBussPath);
		tem_strBussPath  = tem_cBussPath;
		tem_strBussPath += _T("\\");
		tem_strBussPath += tem_strPrename;	

		m_vcNewBussPath[i] = tem_strBussPath;	

		j++;
	}
}


void CUDSSUBRENAME::OnChangeEditFouth()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	//获得完整文件名
	CString   tem_strNewWholName = Self_GetNewName();
	UpdateData(TRUE);
	m_nLastName = m_EditLastName;
	int j=0;
	for (unsigned int i=0; i<m_vcFileName.size(); i++)
	{
		if (!(m_conListRename.GetItemState(i, LVIS_SELECTED)==LVIS_SELECTED||m_conListRename.GetCheck(i)))
		{
			m_conListRename.SetItemText(i, 2, m_vcFileName[i]);
			m_vcNewFileName[i] = m_vcFileName[i];
			m_vcNewFilePath[i] = m_vcFilePath[i];
			m_vcNewBussPath[i] = m_vcBussPath[i];
			continue;
		}
		//1)文件名FileName
		int       tem_nLastName = j+m_nLastName;
		CString   tem_strLastName;
		tem_strLastName.Format(_T("%04d"), tem_nLastName);
		CString   tem_strExtension;	
		tem_strExtension = PathFindExtension(m_vcFileName[i]);

		CString   tem_strPrename = tem_strNewWholName;
		tem_strPrename += tem_strLastName;
		tem_strPrename += tem_strExtension;

		m_vcNewFileName[i] = tem_strPrename;
		m_conListRename.SetItemText(i, 2, tem_strPrename);

		//2)显示文件路径FilePath
		CString   tem_strFilePath = _T("");
		tem_strExtension.MakeLower();
		if (tem_strExtension == _T(".jpg") || tem_strExtension == _T(".jpeg")||
			tem_strExtension == _T(".tif") || tem_strExtension == _T(".tiff")||
			tem_strExtension == _T(".bmp") || tem_strExtension == _T(".png") )	
		{
			tem_strFilePath = m_vcFilePath[i];
			//a、获取文件目录
			TCHAR*   tem_cFilePath;
			tem_cFilePath = tem_strFilePath.GetBuffer();
			tem_strFilePath.ReleaseBuffer();
			PathRemoveFileSpec(tem_cFilePath);
			tem_strFilePath  = tem_cFilePath;
			tem_strFilePath += _T("\\");
			tem_strFilePath += tem_strPrename;
		}
		else
		{
			tem_strFilePath = m_vcFilePath[i];
		}
		m_vcNewFilePath[i] = tem_strFilePath;
		//3)操作文件路径BussPath
		CString   tem_strBussPath = _T("");
		tem_strBussPath = m_vcBussPath[i];
		TCHAR*   tem_cBussPath;
		tem_cBussPath = tem_strBussPath.GetBuffer();
		tem_strBussPath.ReleaseBuffer();
		PathRemoveFileSpec(tem_cBussPath);
		tem_strBussPath  = tem_cBussPath;
		tem_strBussPath += _T("\\");
		tem_strBussPath += tem_strPrename;	

		m_vcNewBussPath[i] = tem_strBussPath;	

		j++;
	}
}


void CUDSSUBRENAME::OnChangeEditLast()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	// TODO:  在此添加控件通知处理程序代码
	//获得完整文件名
	CString   tem_strNewWholName = Self_GetNewName();
	UpdateData(TRUE);
	m_nLastName = m_EditLastName;
	int j=0;
	for (unsigned int i=0; i<m_vcFileName.size(); i++)
	{
		if (!(m_conListRename.GetItemState(i, LVIS_SELECTED)==LVIS_SELECTED||m_conListRename.GetCheck(i)))
		{
			m_conListRename.SetItemText(i, 2, m_vcFileName[i]);
			m_vcNewFileName[i] = m_vcFileName[i];
			m_vcNewFilePath[i] = m_vcFilePath[i];
			m_vcNewBussPath[i] = m_vcBussPath[i];
			continue;
		}
		//1)文件名FileName
		int       tem_nLastName = j+m_nLastName;
		CString   tem_strLastName;
		tem_strLastName.Format(_T("%04d"), tem_nLastName);
		CString   tem_strExtension;	
		tem_strExtension = PathFindExtension(m_vcFileName[i]);

		CString   tem_strPrename = tem_strNewWholName;
		tem_strPrename += tem_strLastName;
		tem_strPrename += tem_strExtension;

		m_vcNewFileName[i] = tem_strPrename;
		m_conListRename.SetItemText(i, 2, tem_strPrename);

		//2)显示文件路径FilePath
		CString   tem_strFilePath = _T("");
		tem_strExtension.MakeLower();
		if (tem_strExtension == _T(".jpg") || tem_strExtension == _T(".jpeg")||
			tem_strExtension == _T(".tif") || tem_strExtension == _T(".tiff")||
			tem_strExtension == _T(".bmp") || tem_strExtension == _T(".png") )	
		{
			tem_strFilePath = m_vcFilePath[i];
			//a、获取文件目录
			TCHAR*   tem_cFilePath;
			tem_cFilePath = tem_strFilePath.GetBuffer();
			tem_strFilePath.ReleaseBuffer();
			PathRemoveFileSpec(tem_cFilePath);
			tem_strFilePath  = tem_cFilePath;
			tem_strFilePath += _T("\\");
			tem_strFilePath += tem_strPrename;
		}
		else
		{
			tem_strFilePath = m_vcFilePath[i];
		}
		m_vcNewFilePath[i] = tem_strFilePath;
		//3)操作文件路径BussPath
		CString   tem_strBussPath = _T("");
		tem_strBussPath = m_vcBussPath[i];
		TCHAR*   tem_cBussPath;
		tem_cBussPath = tem_strBussPath.GetBuffer();
		tem_strBussPath.ReleaseBuffer();
		PathRemoveFileSpec(tem_cBussPath);
		tem_strBussPath  = tem_cBussPath;
		tem_strBussPath += _T("\\");
		tem_strBussPath += tem_strPrename;	

		m_vcNewBussPath[i] = tem_strBussPath;	

		j++;
	}
}


void CUDSSUBRENAME::OnChangeEditSeparator()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	//获得完整文件名
	CString   tem_strNewWholName = Self_GetNewName();
	UpdateData(TRUE);
	m_nLastName = m_EditLastName;
	int j=0;
	for (unsigned int i=0; i<m_vcFileName.size(); i++)
	{
		if (!(m_conListRename.GetItemState(i, LVIS_SELECTED)==LVIS_SELECTED||m_conListRename.GetCheck(i)))
		{
			m_conListRename.SetItemText(i, 2, m_vcFileName[i]);
			m_vcNewFileName[i] = m_vcFileName[i];
			m_vcNewFilePath[i] = m_vcFilePath[i];
			m_vcNewBussPath[i] = m_vcBussPath[i];
			continue;
		}
		//1)文件名FileName
		int       tem_nLastName = j+m_nLastName;
		CString   tem_strLastName;
		tem_strLastName.Format(_T("%04d"), tem_nLastName);
		CString   tem_strExtension;	
		tem_strExtension = PathFindExtension(m_vcFileName[i]);

		CString   tem_strPrename = tem_strNewWholName;
		tem_strPrename += tem_strLastName;
		tem_strPrename += tem_strExtension;

		m_vcNewFileName[i] = tem_strPrename;
		m_conListRename.SetItemText(i, 2, tem_strPrename);

		//2)显示文件路径FilePath
		CString   tem_strFilePath = _T("");
		tem_strExtension.MakeLower();
		if (tem_strExtension == _T(".jpg") || tem_strExtension == _T(".jpeg")||
			tem_strExtension == _T(".tif") || tem_strExtension == _T(".tiff")||
			tem_strExtension == _T(".bmp") || tem_strExtension == _T(".png") )	
		{
			tem_strFilePath = m_vcFilePath[i];
			//a、获取文件目录
			TCHAR*   tem_cFilePath;
			tem_cFilePath = tem_strFilePath.GetBuffer();
			tem_strFilePath.ReleaseBuffer();
			PathRemoveFileSpec(tem_cFilePath);
			tem_strFilePath  = tem_cFilePath;
			tem_strFilePath += _T("\\");
			tem_strFilePath += tem_strPrename;
		}
		else
		{
			tem_strFilePath = m_vcFilePath[i];
		}
		m_vcNewFilePath[i] = tem_strFilePath;
		//3)操作文件路径BussPath
		CString   tem_strBussPath = _T("");
		tem_strBussPath = m_vcBussPath[i];
		TCHAR*   tem_cBussPath;
		tem_cBussPath = tem_strBussPath.GetBuffer();
		tem_strBussPath.ReleaseBuffer();
		PathRemoveFileSpec(tem_cBussPath);
		tem_strBussPath  = tem_cBussPath;
		tem_strBussPath += _T("\\");
		tem_strBussPath += tem_strPrename;	

		m_vcNewBussPath[i] = tem_strBussPath;	

		j++;
	}
}


CString CUDSSUBRENAME::Self_SaveNewName(int index, CString filename)
{

	return CString();
}


void CUDSSUBRENAME::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	int   tem_nSlcNumber = 0;
	for (unsigned int i=0; i<m_vcFileName.size(); i++)
	{
		if (m_conListRename.GetItemState(i, LVIS_SELECTED)==LVIS_SELECTED||m_conListRename.GetCheck(i))
		{
			tem_nSlcNumber++;
		}
	}
	if (tem_nSlcNumber == 0)
	{
		//未选择图像
// 		m_vcNewFileName.clear();
// 		m_vcNewFilePath.clear();
// 		m_vcNewBussPath.clear();
// 		CDialogEx::OnCancel();
		return;
	}

	for(unsigned int i=0; i<m_vcFileName.size(); i++)
	{
		CString  tem_strOldName = m_vcBussPath[i];
		CString  tem_strNewName = m_vcNewBussPath[i];
		if (tem_strOldName != tem_strNewName)
		{
			int      tem_nLen =WideCharToMultiByte(CP_ACP, 0, tem_strOldName, -1, NULL, 0, NULL, NULL);  
			char*    ptxtSrc =new char[tem_nLen +1];  
			WideCharToMultiByte(CP_ACP, 0, tem_strOldName, -1, ptxtSrc, tem_nLen, NULL, NULL); 

			tem_nLen =WideCharToMultiByte(CP_ACP, 0, tem_strNewName, -1, NULL, 0, NULL, NULL);  
			char*    ptxtDst =new char[tem_nLen +1];  
			WideCharToMultiByte(CP_ACP, 0, tem_strNewName, -1, ptxtDst, tem_nLen, NULL, NULL);  
			int  tem_rcRename = rename(ptxtSrc, ptxtDst);

			m_conProRename.OffsetPos(i+1);
			float    tem_fDidRatio = (i+1)*1.0/m_vcFileName.size();
			int      tem_nDidRatio = (int)(tem_fDidRatio*100);
			CString  tem_strDidRatio = _T("");
			tem_strDidRatio.Format(_T("%d"), tem_nDidRatio);
			tem_strDidRatio += _T("%");
			m_StaDidRatio = tem_strDidRatio;
			UpdateData(FALSE);

		}
	}
	//替换原来的vector
	for (unsigned int i=0; i<m_vcNewFileName.size(); i++)
	{
		m_vcFileName[i] = m_vcNewFileName[i];
		m_vcFilePath[i] = m_vcNewFilePath[i];
		m_vcBussPath[i] = m_vcNewBussPath[i];
	}
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	g_BRenamed = TRUE;
	m_vcNewFileName.clear();
	m_vcNewFilePath.clear();
	m_vcNewBussPath.clear();

//	MessageBox(_T("已完成！"), _T("重命名"), MB_OK);
//	CDialogEx::OnOK();
}


void CUDSSUBRENAME::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	m_vcNewFileName.clear();
	m_vcNewFilePath.clear();
	m_vcNewBussPath.clear();
	CDialogEx::OnCancel();
}


CString CUDSSUBRENAME::Self_GetNewName(void)
{
	UpdateData(TRUE);
	m_strFirstKey  = m_EditFirstKey;
	m_strSecondKey = m_EditSecondKey;
	m_strThirdKey  = m_EditThirdKey;
	m_nLastName    = m_EditLastName;
	m_strSeparator = m_EditSeparator;

	CString    tem_strWholeName = _T("");
	if (!m_strPreName.IsEmpty())
	{
		tem_strWholeName += m_strPreName;
		tem_strWholeName += m_strSeparator;
	}
	if (!m_strFirstKey.IsEmpty())
	{
		tem_strWholeName += m_strFirstKey;
		tem_strWholeName += m_strSeparator;
	}
	if (!m_strSecondKey.IsEmpty())
	{
		tem_strWholeName += m_strSecondKey;
		tem_strWholeName += m_strSeparator;
	}
	if (!m_strThirdKey.IsEmpty())
	{
		tem_strWholeName += m_strThirdKey;
		tem_strWholeName += m_strSeparator;
	}


	return tem_strWholeName;
}


void CUDSSUBRENAME::OnClickedChkAllst()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHK_ALLST))
	{
		//全选
		for (unsigned int i=0; i<m_vcFileName.size(); i++)
		{
			m_conListRename.SetCheck(i, TRUE);
		}
	} 
	else
	{
		//全不选
		for (unsigned int i=0; i<m_vcFileName.size(); i++)
		{
			m_conListRename.SetCheck(i, FALSE);
		}
	}
}


void CUDSSUBRENAME::OnItemchangedListRename(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_StaSlctdNumber = _T("已选： ");
	int tem_nSlcNumber = 0;
	for (unsigned int i=0; i<m_vcFileName.size(); i++)
	{
		if (m_conListRename.GetItemState(i, LVIS_SELECTED)==LVIS_SELECTED||m_conListRename.GetCheck(i))
		{
			tem_nSlcNumber++;
		}
	}
	if (tem_nSlcNumber == m_vcFileName.size())
	{
		//自动勾选全选框
		((CButton*)GetDlgItem(IDC_CHK_ALLST))->SetCheck(TRUE);
	}
	else
	{
		//自动取消全选框
		((CButton*)GetDlgItem(IDC_CHK_ALLST))->SetCheck(FALSE);
	}
	CString  tem_strSlcNumber;
	tem_strSlcNumber.Format(_T("%d"), tem_nSlcNumber);
	m_StaSlctdNumber += tem_strSlcNumber;
	UpdateData(FALSE);

	m_conProRename.SetRange(0, tem_nSlcNumber);
	m_conProRename.SetPos(0);

	*pResult = 0;
}
