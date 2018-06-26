
// UDSPlatformV1.7Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UDSPlatformV1.7.h"
#include "UDSPlatformV1.7Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

std::vector<CString>   g_vcCtrlName;   //用于图像导出
std::vector<CString>   g_vcCtrlPath;
std::vector<CString>   m_vcFileName;    //文件名
std::vector<CString>   m_vcFilePath;    //图像文件路径
std::vector<CString>   m_vcBussPath;    //pdf+txt+图像路径 
BOOL      g_BMerged;                    //窗口不返回MB_OK，因此使用全局变量标示
BOOL      g_BRenamed;

CString   g_strNodePath;                //新建节点的名称

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CUDSPlatformV17Dlg 对话框




CUDSPlatformV17Dlg::CUDSPlatformV17Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUDSPlatformV17Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_editValue = 0;
}

void CUDSPlatformV17Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_DIR, m_conTreeCtrl);
	DDX_Control(pDX, IDC_LIST_IMG, m_conListCtrl);
	DDX_Text(pDX, IDC_EDIT_NOWPAGE, m_editValue);
	DDX_Control(pDX, IDC_SLID_PAGENUM, m_conSlidPage);
	DDX_Control(pDX, IDC_BTN_CAMERA, m_btnCamera);
	DDX_Control(pDX, IDC_BTN_SCANNER, m_btnScanner);
	DDX_Control(pDX, IDC_BTN_RECARD, m_btnRecard);
	DDX_Control(pDX, IDC_BTN_OUTPUT, m_btnOutput);
	DDX_Control(pDX, IDC_BTN_PROCESS, m_btnProcess);
	DDX_Control(pDX, IDC_BTN_ROTATEL, m_btnRotateL);
	DDX_Control(pDX, IDC_BTN_ROTATER, m_btnRotateR);
	DDX_Control(pDX, IDC_BTN_SEARCH, m_btnSearch);
	DDX_Control(pDX, IDC_BTN_DELETE, m_btnDelete);
	DDX_Control(pDX, IDC_BTN_LAST, m_btnLast);
	DDX_Control(pDX, IDC_BTN_NEXT, m_btnNext);
	DDX_Control(pDX, IDC_BTN_HELP, m_btn_Help);
	DDX_Control(pDX, IDC_BTN_SORT, m_btnSort);
}

BEGIN_MESSAGE_MAP(CUDSPlatformV17Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_DIR, &CUDSPlatformV17Dlg::OnSelchangedTreeDir)
	ON_BN_CLICKED(IDC_BTN_LAST, &CUDSPlatformV17Dlg::OnBnClickedBtnLast)
	ON_BN_CLICKED(IDC_BTN_NEXT, &CUDSPlatformV17Dlg::OnBnClickedBtnNext)
	ON_BN_CLICKED(IDC_BTN_ROTATEL, &CUDSPlatformV17Dlg::OnBnClickedBtnRotatel)
	ON_BN_CLICKED(IDC_BTN_ROTATER, &CUDSPlatformV17Dlg::OnBnClickedBtnRotater)
	ON_BN_CLICKED(IDC_BTN_DELETE, &CUDSPlatformV17Dlg::OnBnClickedBtnDelete)
	ON_BN_CLICKED(IDC_CHK_ALLSLCT, &CUDSPlatformV17Dlg::OnClickedChkAllslct)
	ON_NOTIFY(NM_CLICK, IDC_LIST_IMG, &CUDSPlatformV17Dlg::OnClickListImg)
	ON_NOTIFY(NM_HOVER, IDC_LIST_IMG, &CUDSPlatformV17Dlg::OnHoverListImg)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_CAMERA, &CUDSPlatformV17Dlg::OnBnClickedBtnCamera)
	ON_BN_CLICKED(IDC_BTN_SCANNER, &CUDSPlatformV17Dlg::OnBnClickedBtnScanner)
	ON_BN_CLICKED(IDC_BTN_RECARD, &CUDSPlatformV17Dlg::OnBnClickedBtnRecard)
	ON_BN_CLICKED(IDC_BTN_OUTPUT, &CUDSPlatformV17Dlg::OnBnClickedBtnOutput)
	ON_BN_CLICKED(IDC_BTN_PROCESS, &CUDSPlatformV17Dlg::OnBnClickedBtnProcess)
	ON_COMMAND(ID_32771, &CUDSPlatformV17Dlg::OnPrintImgs)
	ON_COMMAND(ID_32772, &CUDSPlatformV17Dlg::OnCopyImgs)
	ON_COMMAND(ID_32773, &CUDSPlatformV17Dlg::OnPDFImgs)
	ON_COMMAND(ID_32774, &CUDSPlatformV17Dlg::OnTIFImgs)
	ON_COMMAND(ID_32775, &CUDSPlatformV17Dlg::OnBrush)
	ON_COMMAND(ID_32776, &CUDSPlatformV17Dlg::OnProcess)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_IMG, &CUDSPlatformV17Dlg::OnDblclkListImg)
	ON_EN_CHANGE(IDC_EDIT_NOWPAGE, &CUDSPlatformV17Dlg::OnChangeEditNowpage)
	ON_EN_CHANGE(IDC_EDIT_SEARCH, &CUDSPlatformV17Dlg::OnChangeEditSearch)
	ON_MESSAGE(WM_SETTEXT, &CUDSPlatformV17Dlg::OnSettext)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_IMG, &CUDSPlatformV17Dlg::OnRclickListImg)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_IMG, &CUDSPlatformV17Dlg::OnEndlabeleditListImg)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLID_PAGENUM, &CUDSPlatformV17Dlg::OnCustomdrawSlidPagenum)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLID_PAGENUM, &CUDSPlatformV17Dlg::OnReleasedcaptureSlidPagenum)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_IMG, &CUDSPlatformV17Dlg::OnItemchangedListImg)
	ON_COMMAND(ID_32777, &CUDSPlatformV17Dlg::OnBatchRename)
	ON_COMMAND(ID_32778, &CUDSPlatformV17Dlg::OnBatchMerge)
	ON_STN_DBLCLK(IDC_STA_BT, &CUDSPlatformV17Dlg::OnDblclkStaBt)
	ON_BN_CLICKED(IDC_BTN_HELP, &CUDSPlatformV17Dlg::OnBnClickedBtnHelp)
//	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST_IMG, &CUDSPlatformV17Dlg::OnBegindragListImg)
ON_WM_MOUSEMOVE()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
//ON_NOTIFY(NM_RDBLCLK, IDC_TREE_DIR, &CUDSPlatformV17Dlg::OnRdblclkTreeDir)
ON_NOTIFY(NM_RCLICK, IDC_TREE_DIR, &CUDSPlatformV17Dlg::OnRclickTreeDir)
ON_COMMAND(ID_32779, &CUDSPlatformV17Dlg::On32779NewNode)
ON_COMMAND(ID_32780, &CUDSPlatformV17Dlg::On32780DeleteNode)
ON_BN_CLICKED(IDC_BTN_SORT, &CUDSPlatformV17Dlg::OnBnClickedBtnSort)
ON_COMMAND(ID_32781, &CUDSPlatformV17Dlg::On32781)
ON_COMMAND(ID_32782, &CUDSPlatformV17Dlg::On32782)
ON_COMMAND(ID_32783, &CUDSPlatformV17Dlg::On32783)
ON_COMMAND(ID_32784, &CUDSPlatformV17Dlg::On32784)
ON_COMMAND(ID_32785, &CUDSPlatformV17Dlg::On32785)
ON_COMMAND(ID_32786, &CUDSPlatformV17Dlg::On32786)
ON_COMMAND(ID_32787, &CUDSPlatformV17Dlg::On32787)
ON_COMMAND(ID_32788, &CUDSPlatformV17Dlg::On32788)
ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_IMG, &CUDSPlatformV17Dlg::OnCustomdrawListImg)
ON_NOTIFY(NM_KILLFOCUS, IDC_LIST_IMG, &CUDSPlatformV17Dlg::OnKillfocusListImg)
ON_NOTIFY(NM_SETFOCUS, IDC_LIST_IMG, &CUDSPlatformV17Dlg::OnSetfocusListImg)
ON_COMMAND(ID_32791, &CUDSPlatformV17Dlg::On32791)
ON_COMMAND(ID_32790, &CUDSPlatformV17Dlg::On32790)
ON_COMMAND(ID_32793, &CUDSPlatformV17Dlg::On32793)
END_MESSAGE_MAP()


// CUDSPlatformV17Dlg 消息处理程序

BOOL CUDSPlatformV17Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_MAXIMIZE);

	// TODO: 在此添加额外的初始化代码
	/***************************************
	*
	* 1、变量初始化
	* 2、读取配置文件
	* 3、TreeCtrl初始化
	* 4、ListCtrl初始化
	* 5、Menu初始化
	* 6、SliderControl初始化
	* 7、窗口背景色设置
	* 8、TreeCtrl美化
	* 9、ListCtrl美化
	* 10、按钮美化
	* 11、标题设置
	*
	***************************************/
	GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR m_pGdiToken;
	GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);

	/*1、变量初始化*/
	m_nRootItems   = 0;
	m_nNodeID      = 0;
	m_nOriImg      = 0;
	m_nPageNumer   = 9;
	m_nFileNumber  = 0;
	m_nThumbWidth  = 160;
	m_nThumbHeight = 120;
	m_nPageIndex   = 0;
	m_nPageCount   = 0;     //总页数
	m_nSortMode    = 1;
	m_BSlcAll      = FALSE;
	g_BMerged      = FALSE;
	g_BRenamed     = FALSE;
	m_BLButtonD    = FALSE;
	m_strBT        = _T("紫图SmartScan影像平台系统 v1.7");

	m_clBackColor = RGB(72, 77, 91);      //ListCtrl和TreeCtrl背景颜色
	m_clTextColor = RGB(255, 255, 255);         //ListCtrl和TreeCtrl字体颜色

	m_bkColor = RGB(72, 77, 91);

	g_strNodePath = _T("");

	Self_CalculatThumb(m_nPageNumer);

	/*2、配置文件*/
	CString   tem_strResDir = _T("");
	CString   tem_strIniSrc = _T("");
	CString   tem_strIniDst = _T("");
	CString   tem_strFaceSrc= _T("");
	CString   tem_strFaceDst= _T("");
	TCHAR     tem_tcFilepath[MAX_PATH+1] = {0};
	GetModuleFileName(NULL, tem_tcFilepath, MAX_PATH);
	(_tcsrchr(tem_tcFilepath, _T('\\')))[1] = 0;
	tem_strResDir  = tem_tcFilepath;
	tem_strIniSrc  = tem_strResDir;
	tem_strFaceSrc = tem_tcFilepath;

	tem_strResDir  += _T("res\\");
	m_strPdfImg     = tem_strResDir + _T("pdf.jpg");
	m_strTxtImg     = tem_strResDir + _T("txt.jpg");
	m_strDcmImg     = tem_strResDir + _T("dcm.jpg");

	tem_strIniSrc  += _T("\\");
	tem_strIniSrc  += _T("config.ini");      //安装目录下的配置文件
    m_strIniPath    = Self_GetMyDocument();
	m_strIniPath   += _T("\\UDSData");
	CFileFind  tem_fFileFind;
	if (!tem_fFileFind.FindFile(m_strIniPath))
	{
		CreateDirectory(m_strIniPath, NULL);
	}
	m_strIniPath   += _T("\\config.ini");      //我的文档下的配置文件
//	m_strIniPath.Replace(_T("\\"), _T("\\\\"));
	CopyFile(tem_strIniSrc, m_strIniPath, TRUE);


	//复制faceid.INI至我的文档目录下
	tem_strFaceSrc += _T("FaceInfo.INI");
	tem_strFaceDst  = Self_GetMyDocument();
	tem_strFaceDst += _T("\\UDSData\\FaceInfo.INI");
	CopyFile(tem_strFaceSrc, tem_strFaceDst, TRUE);


	//获取软件名称
	CString  tem_strSoftName = _T("");
	::GetPrivateProfileString(_T("Resource"), _T("softname"), _T("没有找到softname信息"), tem_strSoftName.GetBuffer(MAX_PATH), MAX_PATH, m_strIniPath);
	tem_strSoftName.ReleaseBuffer();
	if (tem_strSoftName.Find(_T("紫图SmartScan")) != -1)
	{
		m_strBT = tem_strSoftName;
	}

	::GetPrivateProfileString(_T("Resource"), _T("sortmode"), _T("没有找到sortmode信息"), tem_strSoftName.GetBuffer(MAX_PATH), MAX_PATH, m_strIniPath);
	m_nSortMode = _ttoi(tem_strSoftName);
	tem_strSoftName.ReleaseBuffer();

	/*3、TreeCtrl初始化*/
	//1)加载图标
	hIcon[0] = theApp.LoadIcon(IDI_ICON_DESK);
	hIcon[1] = theApp.LoadIcon(IDI_ICON_SYSDISK);
	hIcon[2] = theApp.LoadIcon(IDI_ICON_NORDISK);
	hIcon[3] = theApp.LoadIcon(IDI_ICON_MYDOC);
	hIcon[4] = theApp.LoadIcon(IDI_ICON_DOC);

	m_IcoList.Create(24, 24, ILC_COLOR32 , 5, 5);
	for (int i=0; i<5; i++)   
	{   
		m_IcoList.Add(hIcon[i]);   
	} 
	m_conTreeCtrl.SetImageList(&m_IcoList, TVSIL_NORMAL);

	//2）添加桌面目录，并设置为唯一根节点
	HTREEITEM   hRoot    = NULL;      //根节点
	HTREEITEM   hSubRoot = NULL;      //次级节点
	hRoot = m_conTreeCtrl.InsertItem(_T("桌面"), 0, 0);	

	int         tem_nRC = 0;
	TCHAR       tem_cDeskPath[MAX_PATH];
	SHGetSpecialFolderPath(this->GetSafeHwnd(), tem_cDeskPath, CSIDL_DESKTOP, 0);
	CString     tem_strDeskPath = tem_cDeskPath;
	m_conTreeCtrl.SetItemData(hRoot, m_nNodeID);
	Self_AddNodeInfo(m_nNodeID, 0, _T("桌面"), tem_strDeskPath);


	//3) 添加我的文档子节点
	CString  tem_strMyDocItemName = Self_GetMyDocument();
	hSubRoot = m_conTreeCtrl.InsertItem(_T("我的文档"), 3, 3, hRoot, TVI_LAST);
	m_conTreeCtrl.SetItemData(hSubRoot, m_nNodeID);
	Self_AddNodeInfo(m_nNodeID, 3, _T("我的文档"), tem_strMyDocItemName);


	//4)增加本地磁盘目录
	CString tem_strDiskDir = _T("");
	tem_strDiskDir = Self_GetDiskInfo();
	Self_AddDiskItem(tem_strDiskDir);
	
	m_nRootItems = m_nNodeID;   //防止误删除

	//5)增加桌面文件夹子节点
	tem_nRC = Self_FindDocs(tem_strDeskPath);
	std::vector<CString>::iterator  item_name;
	std::vector<CString>::iterator  item_path;
	if (tem_nRC>0)
	{
		item_path     = m_vcDocPath.begin();
		for (item_name=m_vcDocName.begin(); item_name!=m_vcDocName.end(); item_name++, item_path++)
		{
			hSubRoot = m_conTreeCtrl.InsertItem(*item_name, 4, 4, hRoot, TVI_LAST);
			m_conTreeCtrl.SetItemData(hSubRoot, m_nNodeID);
			Self_AddNodeInfo(m_nNodeID, 4, *item_name, *item_path);
		}
	}	
	m_conTreeCtrl.Expand(hRoot, TVE_EXPAND);
	m_conTreeCtrl.SelectItem(hRoot);

	/*10、Button美化*/
	m_btnCamera.Init(IDB_PNG_CAMERA, BTN_IMG_3, BTN_TYPE_NORMAL);
	m_btnScanner.Init(IDB_PNG_SCANNER, BTN_IMG_3, BTN_TYPE_NORMAL);
	m_btnRecard.Init(IDB_PNG_CARD, BTN_IMG_3, BTN_TYPE_NORMAL);
	m_btnOutput.Init(IDB_PNG_OUTPUT, BTN_IMG_3, BTN_TYPE_NORMAL);
	m_btnProcess.Init(IDB_PNG_PROCESS, BTN_IMG_3, BTN_TYPE_NORMAL);
	m_btnRotateL.Init(IDB_PNGTOTATEL, BTN_IMG_3, BTN_TYPE_NORMAL);
	m_btnRotateR.Init(IDB_PNG_ROTATER, BTN_IMG_3, BTN_TYPE_NORMAL);
	m_btnSearch.Init(IDB_PNG_SEARCH, BTN_IMG_1, BTN_TYPE_NORMAL);
	m_btnDelete.Init(IDB_PNG_DELETE, BTN_IMG_3, BTN_TYPE_NORMAL);
	m_btnLast.Init(IDB_PNG_LAST, BTN_IMG_3, BTN_TYPE_NORMAL);
	m_btnNext.Init(IDB_PNG_NEXT, BTN_IMG_3, BTN_TYPE_NORMAL);
	m_btn_Help.Init(IDB_PNG_HELP, BTN_IMG_3, BTN_TYPE_NORMAL);
	m_btnSort.Init(IDB_PNG_SORT, BTN_IMG_3, BTN_TYPE_NORMAL);



	BOOL  tem_BRC = FALSE;
	m_btnTips.Create(this);

	tem_BRC = m_btnTips.AddTool(GetDlgItem(IDC_BTN_CAMERA), _T("打开拍摄仪"));
	tem_BRC = m_btnTips.AddTool(GetDlgItem(IDC_BTN_SCANNER), _T("打开扫描仪"));

	tem_BRC = m_btnTips.AddTool(GetDlgItem(IDC_BTN_OUTPUT), _T("图像导出"));
	tem_BRC = m_btnTips.AddTool(GetDlgItem(IDC_BTN_PROCESS), _T("图像编辑"));
	tem_BRC = m_btnTips.AddTool(GetDlgItem(IDC_BTN_HELP), _T("快速上手"));

	m_btnTips.AddTool(GetDlgItem(IDC_BTN_RECARD), _T("身份证读卡器"));
	m_btnTips.AddTool(GetDlgItem(IDC_BTN_ROTATEL), _T("左旋"));
	m_btnTips.AddTool(GetDlgItem(IDC_BTN_ROTATER), _T("右旋"));
	m_btnTips.AddTool(GetDlgItem(IDC_BTN_DELETE), _T("删除"));
	m_btnTips.AddTool(GetDlgItem(IDC_BTN_LAST), _T("上一页"));
	m_btnTips.AddTool(GetDlgItem(IDC_BTN_NEXT), _T("下一页"));
	m_btnTips.AddTool(GetDlgItem(IDC_BTN_SORT), _T("文件排序"));


	m_btnTips.SetDelayTime(TTDT_INITIAL, 100);
	m_btnTips.SetDelayTime(TTDT_AUTOPOP, 5000);
	m_btnTips.SetDelayTime(TTDT_RESHOW, 100);

	m_btnTips.SetTipTextColor(RGB(0, 0, 0));
	m_btnTips.SetTipBkColor(RGB(255, 255, 255));
	m_btnTips.Activate(TRUE);


	/*4、ListCtrl初始化*/
	int tem_nFileNumber = Self_FindAllFiles(tem_strDeskPath);
	m_nFileNumber       = tem_nFileNumber;
	if (tem_nFileNumber == 0)
	{
		//该目录下没有图像文件,清空显示
		Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, m_nOriImg, 0);
	}
	else if (tem_nFileNumber >= m_nPageNumer)
	{
		Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, m_nOriImg, m_nPageNumer);
	}
	else
	{
		Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, m_nOriImg, tem_nFileNumber);
	}
	m_nPageIndex = Self_UpdaPagination(tem_nFileNumber, TRUE);
	::WritePrivateProfileString(_T("Resource"), _T("curdirectory"), tem_strDeskPath, m_strIniPath);

	/*5、Menu初始化*/
	CreateHSMenu();

	/*6、SliderControl*/
	m_conSlidPage.SetRange(1, 5, TRUE);
	m_conSlidPage.SetPos(1);

	/*8、TreeCtrl美化*/
	m_conTreeCtrl.SetBkColor(m_clBackColor);
	m_conTreeCtrl.SetTextColor(m_clTextColor);

	/*9、ListCtrl美化*/
	m_conListCtrl.SetBkColor(m_clBackColor);

	/*7、窗口背景设置*/
	CDialogEx::SetBackgroundColor(m_clBackColor);
	LoadPicture(m_imgBK, IDB_PNG_BKBLACK, _T("PNG"));

	/*11、标题设置*/
	GetDlgItem(IDC_STA_BT)->SetWindowText(m_strBT);

	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CUDSPlatformV17Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUDSPlatformV17Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		
 		CPaintDC  dc(this);
 		CDC*      pDC = &dc;
 		CDC       dcMem;
 		dcMem.CreateCompatibleDC(pDC);
 
 		CRect      tem_rcClient;
 		GetClientRect(tem_rcClient);
 		CBitmap memBitmap;
 		memBitmap.CreateCompatibleBitmap(pDC, tem_rcClient.Width(), tem_rcClient.Height());
 		dcMem.SelectObject(memBitmap);
// 		dcMem.FillSolidRect(tem_rcClient, RGB(72, 77, 91));	//设置画布颜色
		dcMem.FillSolidRect(tem_rcClient, m_bkColor);	//设置画布颜色
 		if (!m_imgBK.IsNull())
 		{
 			CRect rcImg = CRect(0, 0, m_imgBK.GetWidth(), m_imgBK.GetHeight());
 			m_imgBK.Draw(dcMem.m_hDC, rcImg, rcImg);
 		}
 		pDC->BitBlt(0, 0, tem_rcClient.Width(), tem_rcClient.Height(), &dcMem, 0, 0, SRCCOPY);
 		memBitmap.DeleteObject();
		
//		CDialogEx::OnPaint();
		
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CUDSPlatformV17Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*
void CUDSPlatformV17Dlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	// TODO: 在此处添加消息处理程序代码
	int   tem_nCx, tem_nCy,	tem_nWidthCtrl, tem_nHeightCtrl;
	if (::IsWindow(GetDlgItem(IDC_TREE_DIR)->GetSafeHwnd()))
	{
		int    tem_nWidth, tem_nHeight; 
		BOOL   tem_BZoom = IsZoomed();
		if (tem_BZoom)
		{
			//获取屏幕分辩率
			int tem_nScreenWidth  = GetSystemMetrics(SM_CXSCREEN);
			int tem_nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
			tem_nWidth  = tem_nScreenWidth;
			tem_nHeight = tem_nScreenHeight;
		}
		else
		{
			//获取客户区尺寸
			CRect   tem_rcClient;
			GetWindowRect(&tem_rcClient);
			ScreenToClient(&tem_rcClient);

			tem_nWidth  = tem_rcClient.right - tem_rcClient.left;
			tem_nHeight = tem_rcClient.bottom - tem_rcClient.top;
		}
		//TreeCtrl控件----------------------------------------
		tem_nCx    = 5;
		tem_nCy    = 5;
		tem_nWidthCtrl = 200;
		tem_nHeightCtrl= (int)((tem_nHeight*1.0)/5*4 + 40);
		GetDlgItem(IDC_TREE_DIR)->MoveWindow(tem_nCx, tem_nCy, tem_nWidthCtrl, tem_nHeightCtrl, TRUE);	

		//ListCtrl控件----------------------------------------
		tem_nCx    = 200;
		tem_nCy    = 5;
		tem_nWidthCtrl = tem_nWidth-200;
		tem_nHeightCtrl= (int)((tem_nHeight*1.0)/5*4);
		GetDlgItem(IDC_LIST_IMG)->MoveWindow(tem_nCx, tem_nCy, tem_nWidthCtrl, tem_nHeightCtrl, TRUE);	


	}

}
*/

void CUDSPlatformV17Dlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	// TODO: 在此处添加消息处理程序代码
	
	if (::IsWindow(GetDlgItem(IDC_TREE_DIR)->GetSafeHwnd()))
	{
		int   tem_nDivide     =5;       //高度等分
		int   tem_nProportion = 4;      //TreeCtrl控件占比
		float   tem_nWMutiple  = 3.0;    //TreeCtrl物理宽度，单位英寸
		float   tem_nHMutiple  = 1.7;
		float   tem_fHeight;            //屏幕物理高度
		//1、图标对应位置的重绘----------------------------------------------------------
		int    tem_nWidth, tem_nHeight; 
		BOOL   tem_BZoom = IsZoomed();
		if (tem_BZoom)
		{
			//获取屏幕分辩率
			int tem_nScreenWidth  = GetSystemMetrics(SM_CXSCREEN);
			int tem_nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
			tem_nWidth  = tem_nScreenWidth;
			tem_nHeight = tem_nScreenHeight;
			tem_nWMutiple= 3.0;
			tem_nHMutiple  = 1.7;
		}
		else
		{
			//获取客户区尺寸
			CRect   tem_rcClient;
			GetWindowRect(&tem_rcClient);
			ScreenToClient(&tem_rcClient);

			tem_nWidth  = tem_rcClient.right - tem_rcClient.left;
			tem_nHeight = tem_rcClient.bottom - tem_rcClient.top;
			tem_nWMutiple= 2.5;
			tem_nHMutiple  = 1.4;
		}
		CClientDC dc(this);
		int  tem_nCellWidth  = dc.GetDeviceCaps(LOGPIXELSX);
		int  tem_nCellHeight = dc.GetDeviceCaps(LOGPIXELSY);

		int tem_nCx, tem_nCy, tem_nWidthCtrl, tem_nHeightCtrl;

		tem_fHeight = (tem_nHeight*1.0)/tem_nCellHeight;
		//TreeCtrl控件----------------------------------------
		tem_nCx    = 5;
		tem_nCy    = 5;
//		tem_nWidthCtrl = (int)((tem_nWidth*1.0)/5-5);
		tem_nWidthCtrl = (int)((tem_nCellWidth*1.0)*tem_nWMutiple-32);    //10-09宽度修改为物理宽度,单位-英寸
//		tem_nHeightCtrl= (int)((tem_nHeight*1.0)/tem_nDivide*tem_nProportion + 40);
		tem_nHeightCtrl= (int)((tem_fHeight-tem_nHMutiple)*tem_nCellHeight+30);
		GetDlgItem(IDC_TREE_DIR)->MoveWindow(tem_nCx, tem_nCy, tem_nWidthCtrl, tem_nHeightCtrl, TRUE);	

		//ListCtrl控件----------------------------------------
//		tem_nCx    = (int)((tem_nWidth*1.0)/5+5);
		tem_nCx    = (int)((tem_nCellWidth*1.0)*tem_nWMutiple-20);        //10-09宽度修改为物理宽度,单位-英寸
		tem_nCy    = 5;
//		tem_nWidthCtrl = (int)((tem_nWidth*1.0)/5*4-15);
		tem_nWidthCtrl = tem_nWidth-tem_nCx-10;                          //10-09宽度修改为物理宽度,单位-英寸
//		tem_nHeightCtrl= (int)((tem_nHeight*1.0)/tem_nDivide*tem_nProportion);
		tem_nHeightCtrl= (int)((tem_fHeight-tem_nHMutiple)*tem_nCellHeight);
		GetDlgItem(IDC_LIST_IMG)->MoveWindow(tem_nCx, tem_nCy, tem_nWidthCtrl, tem_nHeightCtrl, TRUE);	

		//按钮------------------------------------------------
//		tem_nCx    = (int)((tem_nWidth*1.0)/5+5);
		tem_nWidthCtrl = tem_nWidth-tem_nCx-10;                          //10-09宽度修改为物理宽度,单位-英寸
//		tem_nCy    = (int)((tem_nHeight*1.0)/tem_nDivide*tem_nProportion+15);
		tem_nCy    = (int)((tem_fHeight-tem_nHMutiple)*tem_nCellHeight+10);
		tem_nWidthCtrl = 24;
		tem_nHeightCtrl= 24;
		GetDlgItem(IDC_BTN_LAST)->MoveWindow(tem_nCx, tem_nCy, tem_nWidthCtrl, tem_nHeightCtrl, TRUE);


		tem_nCx   += 42;
//		tem_nCy    = (int)((tem_nHeight*1.0)/tem_nDivide*tem_nProportion+15);
		tem_nCy    = (int)((tem_fHeight-tem_nHMutiple)*tem_nCellHeight+10);
		tem_nWidthCtrl = 32;
		tem_nHeightCtrl= 32;
		GetDlgItem(IDC_BTN_NEXT)->MoveWindow(tem_nCx, tem_nCy, tem_nWidthCtrl, tem_nHeightCtrl, TRUE);

		tem_nCx   += 42;
//		tem_nCy    = (int)((tem_nHeight*1.0)/tem_nDivide*tem_nProportion+15);
		tem_nCy    = (int)((tem_fHeight-tem_nHMutiple)*tem_nCellHeight+10);
		tem_nWidthCtrl = 32;
		tem_nHeightCtrl= 32;
		GetDlgItem(IDC_BTN_ROTATEL)->MoveWindow(tem_nCx, tem_nCy, tem_nWidthCtrl, tem_nHeightCtrl, TRUE);

		tem_nCx   += 42;
//		tem_nCy    = (int)((tem_nHeight*1.0)/tem_nDivide*tem_nProportion+15);
		tem_nCy    = (int)((tem_fHeight-tem_nHMutiple)*tem_nCellHeight+10);
		tem_nWidthCtrl = 32;
		tem_nHeightCtrl= 32;
		GetDlgItem(IDC_BTN_ROTATER)->MoveWindow(tem_nCx, tem_nCy, tem_nWidthCtrl, tem_nHeightCtrl, TRUE);

		tem_nCx   += 42;
//		tem_nCy    = (int)((tem_nHeight*1.0)/tem_nDivide*tem_nProportion+15);
		tem_nCy    = (int)((tem_fHeight-tem_nHMutiple)*tem_nCellHeight+10);
		tem_nWidthCtrl = 32;
		tem_nHeightCtrl= 32;
		GetDlgItem(IDC_BTN_DELETE)->MoveWindow(tem_nCx, tem_nCy, tem_nWidthCtrl, tem_nHeightCtrl, TRUE);

		tem_nCx   += 42;
		tem_nCy    = (int)((tem_fHeight-tem_nHMutiple)*tem_nCellHeight+10);
		tem_nWidthCtrl = 32;
		tem_nHeightCtrl= 32;
		GetDlgItem(IDC_BTN_SORT)->MoveWindow(tem_nCx, tem_nCy, tem_nWidthCtrl, tem_nHeightCtrl, TRUE);

		tem_nCx   += 42;
//		tem_nCy    = (int)((tem_nHeight*1.0)/tem_nDivide*tem_nProportion+15);
		tem_nCy    = (int)((tem_fHeight-tem_nHMutiple)*tem_nCellHeight+10);
		tem_nWidthCtrl = 32;
		tem_nHeightCtrl= 32;
		GetDlgItem(IDC_BTN_HELP)->MoveWindow(tem_nCx, tem_nCy, tem_nWidthCtrl, tem_nHeightCtrl, TRUE);


		tem_nCx   += 52;
//		tem_nCy    = (int)((tem_nHeight*1.0)/tem_nDivide*tem_nProportion+12);
		tem_nCy    = (int)((tem_fHeight-tem_nHMutiple)*tem_nCellHeight+10);
		tem_nWidthCtrl = 26;
		tem_nHeightCtrl= 26;
		GetDlgItem(IDC_EDIT_NOWPAGE)->MoveWindow(tem_nCx, tem_nCy, tem_nWidthCtrl, tem_nHeightCtrl, TRUE);

		tem_nCx   += 36;
//      tem_nCy    = (int)((tem_nHeight*1.0)/tem_nDivide*tem_nProportion+18);
		tem_nCy    = (int)((tem_fHeight-tem_nHMutiple)*tem_nCellHeight+18);
		tem_nWidthCtrl = 60;
		tem_nHeightCtrl= 32;
		GetDlgItem(IDC_STA_ALLPAGES)->MoveWindow(tem_nCx, tem_nCy, tem_nWidthCtrl, tem_nHeightCtrl, TRUE);

		tem_nCx   += 62;
//		tem_nCy    = (int)((tem_nHeight*1.0)/tem_nDivide*tem_nProportion+15);
		tem_nCy    = (int)((tem_fHeight-tem_nHMutiple)*tem_nCellHeight+13);
		tem_nWidthCtrl = 100;
		tem_nHeightCtrl= 32;
		GetDlgItem(IDC_SLID_PAGENUM)->MoveWindow(tem_nCx, tem_nCy, tem_nWidthCtrl, tem_nHeightCtrl, TRUE);


		//静态文本框------------------------------------------
		CRect      tem_rcPic;
		GetDlgItem(IDC_LIST_IMG)->GetWindowRect(&tem_rcPic);
		ScreenToClient(&tem_rcPic);

		tem_nCx    = tem_rcPic.right-100;
//		tem_nCy    = (int)((tem_nHeight*1.0)/tem_nDivide*tem_nProportion + 9);
		tem_nCy    = (int)((tem_fHeight-tem_nHMutiple)*tem_nCellHeight+8);
		tem_nWidthCtrl = 100;
		tem_nHeightCtrl= 26;
		GetDlgItem(IDC_EDIT_SEARCH)->MoveWindow(tem_nCx, tem_nCy, tem_nWidthCtrl, tem_nHeightCtrl, TRUE);

		tem_nCx   -= 34;
//		tem_nCy    = (int)((tem_nHeight*1.0)/tem_nDivide*tem_nProportion + 13);
		tem_nCy    = (int)((tem_fHeight-tem_nHMutiple)*tem_nCellHeight+10);
		tem_nWidthCtrl = 24;
		tem_nHeightCtrl= 24;
		GetDlgItem(IDC_BTN_SEARCH)->MoveWindow(tem_nCx, tem_nCy, tem_nWidthCtrl, tem_nHeightCtrl, TRUE);


		tem_nCx   -= 70;
//		tem_nCy    = (int)((tem_nHeight*1.0)/tem_nDivide*tem_nProportion+18);
		tem_nCy    = (int)((tem_fHeight-tem_nHMutiple)*tem_nCellHeight+18);
		tem_nWidthCtrl = 70;
		tem_nHeightCtrl= 32;
		GetDlgItem(IDC_STA_ALLNBR)->MoveWindow(tem_nCx, tem_nCy, tem_nWidthCtrl, tem_nHeightCtrl, TRUE);

		tem_nCx   -= 80;
//		tem_nCy    = (int)((tem_nHeight*1.0)/tem_nDivide*tem_nProportion+18);
		tem_nCy    = (int)((tem_fHeight-tem_nHMutiple)*tem_nCellHeight+18);
		tem_nWidthCtrl = 80;
		tem_nHeightCtrl= 32;
		GetDlgItem(IDC_STA_CHKNBR)->MoveWindow(tem_nCx, tem_nCy, tem_nWidthCtrl, tem_nHeightCtrl, TRUE);

		tem_nCx   -= 40;
//		tem_nCy    = (int)((tem_nHeight*1.0)/tem_nDivide*tem_nProportion+17);
		tem_nCy    = (int)((tem_fHeight-tem_nHMutiple)*tem_nCellHeight+18);
		tem_nWidthCtrl = 40;
		tem_nHeightCtrl= 32;
		GetDlgItem(IDC_STACHK_ALLSLC)->MoveWindow(tem_nCx, tem_nCy, tem_nWidthCtrl, tem_nHeightCtrl, TRUE);

		tem_nCx   -= 22;
//		tem_nCy    = (int)((tem_nHeight*1.0)/tem_nDivide*tem_nProportion+16);
		tem_nCy    = (int)((tem_fHeight-tem_nHMutiple)*tem_nCellHeight+19);
		tem_nWidthCtrl = 14;
		tem_nHeightCtrl= 14;
		GetDlgItem(IDC_CHK_ALLSLCT)->MoveWindow(tem_nCx, tem_nCy, tem_nWidthCtrl, tem_nHeightCtrl, TRUE);

		//底栏按钮--------------------------------------------
		tem_nCx    = (int)((tem_nWidth*1.0)/2-24);
//		tem_nCy    = (int)((tem_nHeight*1.0)/tem_nDivide*tem_nProportion + 40);
		tem_nCy    = (int)((tem_fHeight-tem_nHMutiple)*tem_nCellHeight+50);
		tem_nWidthCtrl = 48;
		tem_nHeightCtrl= 48;
		GetDlgItem(IDC_BTN_RECARD)->MoveWindow(tem_nCx, tem_nCy, tem_nWidthCtrl, tem_nHeightCtrl, TRUE);

		tem_nCx    = (int)((tem_nWidth*1.0)/2-102);
//		tem_nCy    = (int)((tem_nHeight*1.0)/tem_nDivide*tem_nProportion + 40);
		tem_nCy    = (int)((tem_fHeight-tem_nHMutiple)*tem_nCellHeight+50);
		tem_nWidthCtrl = 48;
		tem_nHeightCtrl= 48;
		GetDlgItem(IDC_BTN_SCANNER)->MoveWindow(tem_nCx, tem_nCy, tem_nWidthCtrl, tem_nHeightCtrl, TRUE);

		tem_nCx    = (int)((tem_nWidth*1.0)/2-180);
//		tem_nCy    = (int)((tem_nHeight*1.0)/tem_nDivide*tem_nProportion + 40);
		tem_nCy    = (int)((tem_fHeight-tem_nHMutiple)*tem_nCellHeight+50);
		tem_nWidthCtrl = 48;
		tem_nHeightCtrl= 48;
		GetDlgItem(IDC_BTN_CAMERA)->MoveWindow(tem_nCx, tem_nCy, tem_nWidthCtrl, tem_nHeightCtrl, TRUE);


		tem_nCx    = 10;
//		tem_nCy    = (int)((tem_nHeight*1.0)/tem_nDivide*tem_nProportion + 80);
		tem_nCy    = (int)((tem_fHeight-tem_nHMutiple)*tem_nCellHeight+50);
		tem_nWidthCtrl = (int)((tem_nWidth*1.0)/2-200);
		tem_nHeightCtrl= (int)((tem_nHeight*1.0)/tem_nDivide*2 - 60);
		GetDlgItem(IDC_STA_BT)->MoveWindow(tem_nCx, tem_nCy, tem_nWidthCtrl, tem_nHeightCtrl, TRUE);


		tem_nCx    = (int)((tem_nWidth*1.0)/2+54);
//		tem_nCy    = (int)((tem_nHeight*1.0)/tem_nDivide*tem_nProportion + 40);
		tem_nCy    = (int)((tem_fHeight-tem_nHMutiple)*tem_nCellHeight+50);
		tem_nWidthCtrl = 48;
		tem_nHeightCtrl= 48;
		GetDlgItem(IDC_BTN_OUTPUT)->MoveWindow(tem_nCx, tem_nCy, tem_nWidthCtrl, tem_nHeightCtrl, TRUE);

		tem_nCx    = (int)((tem_nWidth*1.0)/2+132);
//		tem_nCy    = (int)((tem_nHeight*1.0)/tem_nDivide*tem_nProportion + 40);
		tem_nCy    = (int)((tem_fHeight-tem_nHMutiple)*tem_nCellHeight+50);
		tem_nWidthCtrl = 48;
		tem_nHeightCtrl= 48;
		GetDlgItem(IDC_BTN_PROCESS)->MoveWindow(tem_nCx, tem_nCy, tem_nWidthCtrl, tem_nHeightCtrl, TRUE);



		//2、最大化以及恢复后，ListCtrl显示部分重绘--------------------------------------
		Self_CalculatThumb(m_nPageNumer);

		int   tem_nShowCount = m_conListCtrl.GetItemCount();
		if (tem_nShowCount>0)
		{
			//获取第一个图像对应的索引值，该索引值相对于整个vector
			int   tem_nFirstIndex = (m_nPageIndex-1)*m_nPageNumer;
			Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, tem_nFirstIndex, tem_nShowCount);
		}
	}
	
}


HBRUSH CUDSPlatformV17Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	//1、Static静态文本框美化----------------------------------------------------------
	if (pWnd->GetDlgCtrlID() == IDC_STA_ALLPAGES || pWnd->GetDlgCtrlID()==IDC_STA_CHKNBR || pWnd->GetDlgCtrlID()==IDC_STA_ALLNBR || 
		pWnd->GetDlgCtrlID()==IDC_STACHK_ALLSLC)
	{
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(m_clTextColor);
//		CFont   tem_Font;
//		tem_Font.CreatePointFont(100, _T("宋体"));
//		pDC->SelectObject(&tem_Font);
	}
	else if (pWnd->GetDlgCtrlID() == IDC_STA_BT)
	{
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255, 255, 255));
		CFont   tem_FontSta;
		tem_FontSta.CreatePointFont(200, _T("黑体"));
		pDC->SelectObject(&tem_FontSta);
	}
	//2、SliderBar控件美化---------------------------------------------------------------
	if (pWnd->GetDlgCtrlID() == IDC_SLID_PAGENUM)
	{
		return (HBRUSH) CreateSolidBrush(m_clBackColor);   //声明画刷CBrush再CreateSolidBrush看似相同，结果不对
	}

	if (pWnd->GetDlgCtrlID() == IDC_CHK_ALLSLCT)
	{
		pDC->SetTextColor(m_clBackColor);

	}

	//3、EditCtrol文本框美化----------------------------------------------------------
	//先将外层透明
	if (pWnd->GetDlgCtrlID()==IDC_EDIT_NOWPAGE || pWnd->GetDlgCtrlID()==IDC_EDIT_SEARCH)
	{
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(246, 246, 11));
// 		CFont   tem_Font;
// 		tem_Font.CreatePointFont(100, _T("宋体"));
// 		pDC->SelectObject(&tem_Font);
	}
	//再将里层透名
	pWnd = pWnd->GetParent();
	if (pWnd)
	{
		if (pWnd->GetDlgCtrlID()==IDC_EDIT_NOWPAGE || pWnd->GetDlgCtrlID()==IDC_EDIT_SEARCH)
		{
			hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
			pDC->SetBkMode(TRANSPARENT);
		}
	}
	
	
	


	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	
	return hbr;
}


BOOL CUDSPlatformV17Dlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_MOUSEMOVE)
	{
		m_btnTips.RelayEvent(pMsg);
	}
    if (pMsg->message == WM_KEYDOWN)
    {
		switch(pMsg->wParam)
		{
		case VK_RETURN:
			return TRUE;
			break;
		case VK_UP:
		case VK_LEFT:
			//判断焦点是否在ListCtrl上
			if (GetDlgItem(IDC_LIST_IMG)->GetSafeHwnd()==::GetFocus())
			{
				int tem_nSlcIndex = m_conListCtrl.GetSelectionMark();
				if (tem_nSlcIndex==0)
				{
					OnBnClickedBtnLast();
				}
			}
			break;
		case VK_DOWN:
		case VK_RIGHT:
			//判断焦点是否在ListCtrl上
			if (GetDlgItem(IDC_LIST_IMG)->GetSafeHwnd()==::GetFocus())
			{
				int tem_nSlcIndex = m_conListCtrl.GetSelectionMark();
				if (tem_nSlcIndex==m_conListCtrl.GetItemCount()-1)
				{
					OnBnClickedBtnNext();
				}
			}
			break;
		case VK_PRIOR:
			//PageUp
			if (GetDlgItem(IDC_LIST_IMG)->GetSafeHwnd()==::GetFocus())
			{
				OnBnClickedBtnLast();
			}
			break;
		case VK_NEXT:
			//PageDown
			if (GetDlgItem(IDC_LIST_IMG)->GetSafeHwnd()==::GetFocus())
			{
				OnBnClickedBtnNext();
			}
			break;
		case VK_DELETE:
			if (GetDlgItem(IDC_LIST_IMG)->GetSafeHwnd()==::GetFocus())
			{
				OnBnClickedBtnDelete();
			}
			break;
		case 0x41:
			//A键=0x41
			if (GetDlgItem(IDC_LIST_IMG)->GetSafeHwnd()==::GetFocus()&& (GetKeyState(VK_CONTROL)&0x8000))
			{
				//选取当前页添加到索引
				Self_HightLightItems();
			}
			break;

		}

    }

	return CDialogEx::PreTranslateMessage(pMsg);
}


/**********************************
*         函数目录
*说明：TreeCtrl
*
* 1、添加节点信息
* 2、获取我的文档目录
* 3、获取windows磁盘目录
* 4、增加磁盘节点
* 5、查找子文件夹
* 6、获取节点信息
* 7、选择节点
* 8、右键节点
* 9、添加节点
* 10、删除节点
*
***********************************/

BOOL CUDSPlatformV17Dlg::Self_AddNodeInfo(int id, int image, CString name, CString path)
{
	NODEINFO   tem_NodeInfo;
	tem_NodeInfo.nodeID    = id;
	tem_NodeInfo.nodeImage = image;
	tem_NodeInfo.nodeName  = name;
	tem_NodeInfo.nodePath  = path;
	m_vcNodeInfo.push_back(tem_NodeInfo);

	m_nNodeID++;
	return TRUE;
}


CString CUDSPlatformV17Dlg::Self_GetMyDocument(void)
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


CString CUDSPlatformV17Dlg::Self_GetDiskInfo(void)
{
	UINT     tem_uDriverType;    //盘符类别
	DWORD    tem_dwDirLength;    //存放字符串的长度
	DWORD    tem_dwReturnCode;   //返回码，返回字符串长度
	CString  tem_strEvyDirName;  
	CString  tem_strAllDirName;

	tem_dwDirLength = GetLogicalDriveStrings(0, NULL);
	LPTSTR   tem_lstrDirName = (LPTSTR)HeapAlloc(GetProcessHeap(), 0, tem_dwDirLength * sizeof(TCHAR));    //盘符名，以NULL间隔，以两个NULL结尾,分配内存空间 	  若无后面的sizeof(TCHAR)可能造成内存溢出中断
	tem_dwReturnCode = GetLogicalDriveStrings(tem_dwDirLength, tem_lstrDirName);

	if (tem_dwReturnCode == 0 ||tem_dwReturnCode > tem_dwDirLength)
	{
		MessageBox(_T("获取盘符信息失败"));
	}
	else
	{		
		TCHAR* tem_cDirItem =tem_lstrDirName;
		while (*tem_cDirItem)
		{
			//判断盘符类型
			tem_uDriverType = GetDriveType(tem_cDirItem);
			switch(tem_uDriverType)
			{
			case DRIVE_FIXED:        //固定磁盘
				tem_strEvyDirName  = tem_cDirItem;
				tem_strAllDirName += tem_strEvyDirName + _T("|");
				break;  
			case DRIVE_REMOVABLE:    //可移动磁盘
				tem_strEvyDirName  = tem_cDirItem;
				tem_strAllDirName += tem_strEvyDirName + _T("|");
				break;
			default:                 //其他都按未知处理	
				break;
			}
			tem_cDirItem+=(wcslen(tem_cDirItem)+1);
		}	
	}

	return  tem_strAllDirName;
}


void CUDSPlatformV17Dlg::Self_AddDiskItem(CString diskname)
{
	int          i               = 0;
	HTREEITEM    hRoot           = m_conTreeCtrl.GetRootItem();
	HTREEITEM    hSubRoot        = NULL;
	CString      tem_strDiskName = diskname;


	while (!tem_strDiskName.IsEmpty())
	{
		int      tem_nSite      = 0;
		int      tem_nLength    = 0;
		int      tem_nFindC     = -1;
		int      tem_nFindD     = -1;
		int      tem_nFindE     = -1;
		int      tem_nFindF     = -1;
		int      tem_nFindG     = -1;
		int      tem_nFindH     = -1;

		CString  tem_strOneDisk = _T("");

		tem_nSite      = tem_strDiskName.Find(_T("|"));
		tem_strOneDisk = tem_strDiskName.Left(tem_nSite);
		tem_nFindC     = tem_strOneDisk.Find('C');
		tem_nFindD     = tem_strOneDisk.Find('D');
		tem_nFindE     = tem_strOneDisk.Find('E');
		tem_nFindF     = tem_strOneDisk.Find('F');
		tem_nFindG     = tem_strOneDisk.Find('G');
		tem_nFindH     = tem_strOneDisk.Find('H');
		if (tem_nFindC != -1)
		{
			hSubRoot = m_conTreeCtrl.InsertItem(_T("本地磁盘C"), 1, 1, hRoot, TVI_LAST);
			m_conTreeCtrl.SetItemData(hSubRoot, m_nNodeID);
			Self_AddNodeInfo(m_nNodeID, 1, _T("本地磁盘C"), tem_strOneDisk);
		}
		else if (tem_nFindD != -1)
		{
			hSubRoot = m_conTreeCtrl.InsertItem(_T("本地磁盘D"), 2, 2, hRoot, TVI_LAST);
			m_conTreeCtrl.SetItemData(hSubRoot, m_nNodeID);
			Self_AddNodeInfo(m_nNodeID, 2, _T("本地磁盘D"), tem_strOneDisk);
		}
		else if (tem_nFindE != -1)
		{
			hSubRoot = m_conTreeCtrl.InsertItem(_T("本地磁盘E"), 2, 2, hRoot, TVI_LAST);
			m_conTreeCtrl.SetItemData(hSubRoot, m_nNodeID);
			Self_AddNodeInfo(m_nNodeID, 2, _T("本地磁盘E"), tem_strOneDisk);
		}
		else if (tem_nFindF != -1)
		{
			hSubRoot = m_conTreeCtrl.InsertItem(_T("本地磁盘F"), 2, 2, hRoot, TVI_LAST);
			m_conTreeCtrl.SetItemData(hSubRoot, m_nNodeID);
			Self_AddNodeInfo(m_nNodeID, 2, _T("本地磁盘F"), tem_strOneDisk);
		}
		else if (tem_nFindG != -1)
		{
			hSubRoot = m_conTreeCtrl.InsertItem(_T("本地磁盘G"), 2, 2, hRoot, TVI_LAST);
			m_conTreeCtrl.SetItemData(hSubRoot, m_nNodeID);
			Self_AddNodeInfo(m_nNodeID, 2, _T("本地磁盘G"), tem_strOneDisk);
		}
		else if (tem_nFindH != -1)
		{
			hSubRoot = m_conTreeCtrl.InsertItem(_T("本地磁盘H"), 2, 2, hRoot, TVI_LAST);
			m_conTreeCtrl.SetItemData(hSubRoot, m_nNodeID);
			Self_AddNodeInfo(m_nNodeID, 2, _T("本地磁盘H"), tem_strOneDisk);
		}

		tem_nLength = tem_strDiskName.Delete(0,tem_nSite+1);
		tem_strDiskName = tem_strDiskName.Right(tem_nLength);		
	}
}


int CUDSPlatformV17Dlg::Self_FindDocs(CString dirname)
{
	int   tem_nDocCount = 0;    //子文件夹数量
	m_vcDocName.clear();
	m_vcDocPath.clear();
	BOOL        bGetRc      = NULL;
	CFileFind   temFinder   = NULL;
	CString     strFileName = NULL;
	CString     strSubDoc   = NULL;
	CString     strFilePath = NULL;

	bGetRc = temFinder.FindFile(dirname + "\\*.*");
	while (bGetRc)
	{
		bGetRc = temFinder.FindNextFile();
		if (temFinder.IsDots())
		{
			//为缺省目录
			continue;
		}
		if (temFinder.IsDirectory())
		{
			//检测为子文件夹，保存备用
			strSubDoc = temFinder.GetFilePath();
			m_vcDocPath.push_back(strSubDoc);
			int   tem_nSite = strSubDoc.ReverseFind('\\');
			strSubDoc = strSubDoc.Mid(tem_nSite+1);
			m_vcDocName.push_back(strSubDoc);
			tem_nDocCount++;
		} 

	}
	return tem_nDocCount;
}


CString CUDSPlatformV17Dlg::Self_GetItemInfo(HTREEITEM item, int infoindex)
{
	int         tem_nInfoIndex = infoindex;
	HTREEITEM   tem_hSlcItem   = item;

	NODEINFO    tem_infoSlcNode;
	DWORD_PTR   tem_dwData        = 0;
	CString     tem_strReturnInfo = _T("");
	int         tem_nItemID       = 0;

	tem_dwData   = m_conTreeCtrl.GetItemData(tem_hSlcItem);
	if (tem_dwData>=0)
	{
		tem_infoSlcNode = m_vcNodeInfo[tem_dwData];
		switch(tem_nInfoIndex)
		{
		case 0:
			//返回Name信息
			tem_strReturnInfo  = tem_infoSlcNode.nodeName;

			break;
		case 1:
			//返回path信息
			tem_strReturnInfo = tem_infoSlcNode.nodePath;
			break;
		case 2:
			//返回ID信息
			tem_nItemID = tem_infoSlcNode.nodeID;
			tem_strReturnInfo.Format(_T("%d"), tem_nItemID);
			break;
		default:
			break;
		}

	}

	return tem_strReturnInfo;
}


void CUDSPlatformV17Dlg::OnSelchangedTreeDir(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//查询该节点是否有子节点
	HTREEITEM    tem_hSlcItem   = m_conTreeCtrl.GetSelectedItem();
	HTREEITEM    tem_hChildItem = m_conTreeCtrl.GetChildItem(tem_hSlcItem);
	//1、获取该节点路径
	CString      tem_strSlcItemPath = Self_GetItemInfo(tem_hSlcItem, 1);	
	if (tem_hChildItem == NULL)
	{
		/*该节点无子节点，查询子文件夹，添加节点*/
		
		//2、查找该路径下文件夹
		int      tem_nDocNumber     = Self_FindDocs(tem_strSlcItemPath);
		if (tem_nDocNumber>0)
		{
			//将子文件夹添加为节点
			std::vector<CString>::iterator item_name;
			std::vector<CString>::iterator item_path;
			item_name = m_vcDocName.begin();
			for (item_path=m_vcDocPath.begin(); item_path!=m_vcDocPath.end(); item_path++, item_name++)
			{
				HTREEITEM     tem_hSubRoot = m_conTreeCtrl.InsertItem(*item_name, 4, 4, tem_hSlcItem, TVI_LAST);
				m_conTreeCtrl.SetItemData(tem_hSubRoot, m_nNodeID);
				Self_AddNodeInfo(m_nNodeID, 4, *item_name, *item_path);
			}
		}
	}
	//3、查找该路径下文件
	m_nOriImg      = 0;
	int tem_nFileNumber = Self_FindAllFiles(tem_strSlcItemPath);
	m_nFileNumber       = tem_nFileNumber;
	if (tem_nFileNumber == 0)
	{
		for (int i=0; i<m_ImageList.GetImageCount(); i++)
		{
			m_ImageList.Remove(i);
		}
		m_ImageList.DeleteImageList();
		m_conListCtrl.DeleteAllItems();
		//该目录下没有图像文件,清空显示
//		Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, m_nOriImg, 0);
	}
	else if (tem_nFileNumber >= m_nPageNumer)
	{
		Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, m_nOriImg, m_nPageNumer);
	}
	else
	{
		Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, m_nOriImg, tem_nFileNumber);
	}

	m_BSlcAll = FALSE;
	((CButton*)GetDlgItem(IDC_CHK_ALLSLCT))->SetCheck(BST_UNCHECKED);  

	::WritePrivateProfileString(_T("Resource"), _T("curdirectory"), tem_strSlcItemPath, m_strIniPath);

	//4、将搜索框置为NULL
	GetDlgItem(IDC_EDIT_SEARCH)->SetWindowText(_T(""));
	CRect    tem_rcEdit;

	GetDlgItem(IDC_EDIT_SEARCH)->GetWindowRect(&tem_rcEdit); 
	ScreenToClient(&tem_rcEdit);        
	InvalidateRect(&tem_rcEdit);

	UpdateWindow();
	*pResult = 0;
}


void CUDSPlatformV17Dlg::OnRclickTreeDir(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	//选定节点
	CPoint     tem_ptCurPoint;
	UINT       tem_uFlags;
	HTREEITEM  tem_hRSlcItem;

	GetCursorPos(&tem_ptCurPoint);
	m_conTreeCtrl.ScreenToClient(&tem_ptCurPoint);
	tem_hRSlcItem = m_conTreeCtrl.HitTest(tem_ptCurPoint, &tem_uFlags);
	if (tem_hRSlcItem && (TVHT_ONITEM&tem_uFlags))
	{
		//1)、有节点，选定节点
		m_conTreeCtrl.SelectItem(tem_hRSlcItem);

		//2)、弹出右键菜单
		m_mnuRTree.LoadMenu(IDR_MENU_TOOL);
		CMenu*  tem_pPopup = m_mnuRTree.GetSubMenu(2);
		ClientToScreen(&tem_ptCurPoint);
		tem_pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_TOPALIGN, tem_ptCurPoint.x, tem_ptCurPoint.y, this);
	}

	*pResult = 0;
}


void CUDSPlatformV17Dlg::On32779NewNode()
{
	// TODO: 在此添加命令处理程序代码
	if (IDOK == m_dlgNewNode.DoModal())
	{
		//1)、获取当前节点
		HTREEITEM  tem_hRSlcItem = m_conTreeCtrl.GetSelectedItem();

		//2)、获取节点路径
        CString    tem_strRSlcPath = Self_GetItemInfo(tem_hRSlcItem, 1);

		//3)、拼接新的路径
		CString    tem_strNewNodePath = tem_strRSlcPath;
		if (tem_strNewNodePath == _T("C:\\") || tem_strNewNodePath == _T("D:\\") || tem_strNewNodePath == _T("E:\\") || 
			tem_strNewNodePath == _T("F:\\") || tem_strNewNodePath == _T("G:\\") || tem_strNewNodePath == _T("H:\\"))
		{
			//磁盘根目录后缀自带
			tem_strNewNodePath += g_strNodePath;
		}
		else
		{
			tem_strNewNodePath += _T("\\");
			tem_strNewNodePath += g_strNodePath;
		}
		
		//4)、判断新添加的目录是否存在
		CFileFind  tem_fFileFind;
		if (!tem_fFileFind.FindFile(tem_strNewNodePath))
		{
			CreateDirectory(tem_strNewNodePath, NULL);

			//该节点下无该子节点，添加新节点
			HTREEITEM tem_hSubItem = m_conTreeCtrl.InsertItem(g_strNodePath, 4, 4, tem_hRSlcItem, TVI_LAST);
			m_conTreeCtrl.SetItemData(tem_hSubItem, m_nNodeID);
			Self_AddNodeInfo(m_nNodeID, 4, g_strNodePath, tem_strNewNodePath);	
			//选定新建节点
			m_conTreeCtrl.SelectItem(tem_hSubItem);

		}
		else
		{
			//已存在则展开当前节点
			m_conTreeCtrl.Expand(tem_hRSlcItem, TVE_EXPAND);
			//查询、并选中该节点
			HTREEITEM   tem_hChildItem = m_conTreeCtrl.GetChildItem(tem_hRSlcItem);
			while(tem_hChildItem!=NULL)
			{
				CString      tem_strItemName = Self_GetItemInfo(tem_hChildItem, 0);	
				if (tem_strItemName == g_strNodePath)
				{					
					m_conTreeCtrl.SelectItem(tem_hChildItem);
					break;
				}
				tem_hChildItem = m_conTreeCtrl.GetNextSiblingItem(tem_hChildItem);
			}
		}
	}
}


void CUDSPlatformV17Dlg::On32780DeleteNode()
{
	// TODO: 在此添加命令处理程序代码
	//先判断是不是根目录
	HTREEITEM  tem_hRSlcItem = m_conTreeCtrl.GetSelectedItem();
	CString    tem_strItemID = Self_GetItemInfo(tem_hRSlcItem, 2);
	int        tem_nItemID   = _ttoi(tem_strItemID);
	if (tem_nItemID<m_nRootItems)
	{
		MessageBox(_T("无法删除该目录！"), _T("影像平台"), MB_OK);
		return;
	}
	if (IDOK == m_dlgAck.DoModal())
	{	
		//循环删除子节点
		HTREEITEM   tem_hParentItem= m_conTreeCtrl.GetParentItem(tem_hRSlcItem);
		HTREEITEM   tem_hChildItem = m_conTreeCtrl.GetChildItem(tem_hRSlcItem);
		while(tem_hChildItem!=NULL)
		{
			CString      tem_strItemPath = Self_GetItemInfo(tem_hChildItem, 1);	
			//删除该目录
//			Self_DeleteFile2Recycle(tem_strItemPath);

			//删除该节点
			m_conTreeCtrl.DeleteItem(tem_hChildItem);
			tem_hChildItem = m_conTreeCtrl.GetNextSiblingItem(tem_hChildItem);
		}

		//删除该节点
		CString tem_strItemPath = Self_GetItemInfo(tem_hRSlcItem, 1);
		Self_DeleteFile2Recycle(tem_strItemPath);
		m_conTreeCtrl.DeleteItem(tem_hRSlcItem);

		//跳转至父节点
		m_conTreeCtrl.SelectItem(tem_hParentItem);
	} 	

}

//##################################################################################################################

/**********************************
*         函数目录
*说明：ListCtrl
*
* 1、查找目录下文件名、路径
* 2、获取文件类型
* 3、缩略图显示
* 4、上一页
* 5、下一页
* 6、左旋
* 7、右旋
* 8、删除
* 9、CheckBox
* 10、点击ListCtrl单选/多选
* 11、ListCtrl悬浮选定
* 12、替换指定图像
* 13、更新总页码与当前页码
* 14、删除至回收站
* 15、双击ListCtrl
* 16、页面跳转
* 17、实时搜索
* 18、高亮、选中当前页面items
* 19、取消当前页面高亮以及选中
*
***********************************/

int CUDSPlatformV17Dlg::Self_FindAllFiles(CString dirpath)
{
	m_vcFileName.clear();
	m_vcFilePath.clear();
	m_vcBussPath.clear();
	m_vcFileSize.clear();
	m_vcCreateTime.clear();
	int         tem_nFileCount = 0;    //子文件数量
	BOOL        bGetRc      = NULL;
	CFileFind   temFinder   = NULL;
	CString     strFileName = NULL;
	CString     strSubDoc   = NULL;
	CString     strFilePath = NULL;

	FILETIME    tem_tmFileTime;       //文件创建时间
	ULONGLONG   tem_ulFileSize;       //文件大小，单位字节
	

	bGetRc = temFinder.FindFile(dirpath + "\\*.*");
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
				if (strMiddle == _T("jpg") || strMiddle == _T("jpeg")||
					strMiddle == _T("tif") || strMiddle == _T("tiff")||
					strMiddle == _T("bmp") || strMiddle == _T("png") )		
				{
					//区别图像文件和非图像文件，将FieName保存
					tem_nFileCount++;
					strFilePath = "";
					m_vcFileName.push_back(strFileName);
					strFilePath = dirpath + "\\" + strFileName;
					m_vcFilePath.push_back(strFilePath);
					m_vcBussPath.push_back(strFilePath);

					//保存文件创建时间和文件大小
					temFinder.GetCreationTime(&tem_tmFileTime);
					m_vcCreateTime.push_back(tem_tmFileTime);
					
					tem_ulFileSize = temFinder.GetLength();
					m_vcFileSize.push_back(tem_ulFileSize);					
				}
				else if (strMiddle == _T("pdf"))
				{
					//区别图像文件和非图像文件，将FieName保存
					tem_nFileCount++;
					strFilePath = "";
					m_vcFileName.push_back(strFileName);
					strFilePath = dirpath + "\\" + strFileName;
					m_vcFilePath.push_back(m_strPdfImg);
					m_vcBussPath.push_back(strFilePath);

					//保存文件创建时间和文件大小
					temFinder.GetCreationTime(&tem_tmFileTime);
					m_vcCreateTime.push_back(tem_tmFileTime);

					tem_ulFileSize = temFinder.GetLength();
					m_vcFileSize.push_back(tem_ulFileSize);	
				}
				else if (strMiddle == _T("txt"))
				{
					tem_nFileCount++;
					strFilePath = "";
					m_vcFileName.push_back(strFileName);
					strFilePath = dirpath + "\\" + strFileName;
					m_vcFilePath.push_back(m_strTxtImg);
					m_vcBussPath.push_back(strFilePath);

					//保存文件创建时间和文件大小
					temFinder.GetCreationTime(&tem_tmFileTime);
					m_vcCreateTime.push_back(tem_tmFileTime);

					tem_ulFileSize = temFinder.GetLength();
					m_vcFileSize.push_back(tem_ulFileSize);	
				}
				else if (strMiddle == _T("dcm"))
				{
					tem_nFileCount++;
					strFilePath = "";
					m_vcFileName.push_back(strFileName);
					strFilePath = dirpath + "\\" + strFileName;
					m_vcFilePath.push_back(m_strDcmImg);
					m_vcBussPath.push_back(strFilePath);

					//保存文件创建时间和文件大小
					temFinder.GetCreationTime(&tem_tmFileTime);
					m_vcCreateTime.push_back(tem_tmFileTime);

					tem_ulFileSize = temFinder.GetLength();
					m_vcFileSize.push_back(tem_ulFileSize);	
				}
				
			}

		}
	}	
	//根据设置对文件进行排序
	Self_FileSort(m_nSortMode);

	m_nPageIndex = Self_UpdaPagination(tem_nFileCount, TRUE);
	return tem_nFileCount;
}


int CUDSPlatformV17Dlg::Self_GetTypeFromFileName(LPCTSTR pstr)
{
	CString fileName(pstr);
	CString ext3=fileName.Right(3);
	CString ext4=fileName.Right(4);
#if CXIMAGE_SUPPORT_BMP
	if(ext3.CompareNoCase(_T("bmp"))==0)
		return CXIMAGE_FORMAT_BMP;
#endif

#if CXIMAGE_SUPPORT_GIF
	if(ext3.CompareNoCase(_T("gif"))==0)
		return CXIMAGE_FORMAT_GIF;
#endif

#if CXIMAGE_SUPPORT_JPG
	if(ext3.CompareNoCase(_T("jpg"))==0 || ext4.CompareNoCase(_T("jpeg"))==0)
		return CXIMAGE_FORMAT_JPG;
#endif

#if CXIMAGE_SUPPORT_PNG
	if(ext3.CompareNoCase(_T("png"))==0)
		return CXIMAGE_FORMAT_PNG;
#endif

#if CXIMAGE_SUPPORT_MNG
	if(ext3.CompareNoCase(_T("mng"))==0 || ext3.CompareNoCase(_T("jng"))==0 ||ext3.CompareNoCase(_T("png"))==0)
		return CXIMAGE_FORMAT_MNG;
#endif

#if CXIMAGE_SUPPORT_ICO
	if(ext3.CompareNoCase(_T("ico"))==0)
		return CXIMAGE_FORMAT_ICO;
#endif

#if CXIMAGE_SUPPORT_TIF
	if(ext3.CompareNoCase(_T("tif"))==0 || ext4.CompareNoCase(_T("tiff"))==0)
		return CXIMAGE_FORMAT_TIF;
#endif

#if CXIMAGE_SUPPORT_TGA
	if(ext3.CompareNoCase(_T("tga"))==0)
		return CXIMAGE_FORMAT_TGA;
#endif

#if CXIMAGE_SUPPORT_PCX
	if(ext3.CompareNoCase(_T("pcx"))==0)
		return CXIMAGE_FORMAT_PCX;
#endif

#if CXIMAGE_SUPPORT_WBMP
	if(ext4.CompareNoCase(_T("wbmp"))==0)
		return CXIMAGE_FORMAT_WBMP;
#endif

#if CXIMAGE_SUPPORT_WMF
	if(ext3.CompareNoCase(_T("wmf"))==0 || ext3.CompareNoCase(_T("emf"))==0)
		return CXIMAGE_FORMAT_WMF;
#endif

#if CXIMAGE_SUPPORT_J2K
	if(ext3.CompareNoCase(_T("j2k"))==0 || ext3.CompareNoCase(_T("jp2"))==0)
		return CXIMAGE_FORMAT_J2K;
#endif

#if CXIMAGE_SUPPORT_JBG
	if(ext3.CompareNoCase(_T("jbg"))==0)
		return CXIMAGE_FORMAT_JBG;
#endif

#if CXIMAGE_SUPPORT_JP2
	if(ext3.CompareNoCase(_T("j2k"))==0 || ext3.CompareNoCase(_T("jp2"))==0)
		return CXIMAGE_FORMAT_JP2;
#endif

#if CXIMAGE_SUPPORT_JPC
	if(ext3.CompareNoCase(_T("j2c"))==0 || ext3.CompareNoCase(_T("jpc"))==0)
		return CXIMAGE_FORMAT_JPC;
#endif

#if CXIMAGE_SUPPORT_PGX
	if(ext3.CompareNoCase(_T("pgx"))==0)
		return CXIMAGE_FORMAT_PGX;
#endif

#if CXIMAGE_SUPPORT_PNM
	if(ext3.CompareNoCase(_T("pnm"))==0 || ext3.CompareNoCase(_T("pgm"))==0 || ext3.CompareNoCase(_T("ppm"))==0)
		return CXIMAGE_FORMAT_PNM;
#endif

#if CXIMAGE_SUPPORT_RAS
	if(ext3.CompareNoCase(_T("ras"))==0)
		return CXIMAGE_FORMAT_RAS;
#endif

	return CXIMAGE_FORMAT_UNKNOWN;
}


BOOL CUDSPlatformV17Dlg::Self_ThumbailList(int thumbwidth, int thumbheight, int origin, int number)
{
	int          tem_nOriIndex   = origin;       //显示图像序列的起点
	int          tem_nPageNumber = number;     //每页显示图像的数量
	int          m_nImageIndex   = 0;
	DWORD  dwStyle;
	//CImageList初始化
	m_ImageList.Create(thumbwidth,thumbheight,ILC_COLOR24,0,1);

	//ListCtrl初始化
	dwStyle = m_conListCtrl.GetExtendedStyle();
	dwStyle=dwStyle|LVS_SHOWSELALWAYS|LVS_ALIGNTOP|LVS_ICON|LVS_AUTOARRANGE;
//	dwStyle=dwStyle|LVS_SHOWSELALWAYS|LVS_ALIGNTOP|LVS_ICON|LVS_AUTOARRANGE|LVS_EDITLABELS|LVS_ALIGNMASK;     //原ListCtrl显示样式
//	dwStyle=dwStyle|LVS_SHOWSELALWAYS|LVS_ALIGNTOP|LVS_ICON|LVS_AUTOARRANGE|LVS_EDITLABELS|LVS_EX_BORDERSELECT|LVS_ALIGNMASK; 
	m_conListCtrl.SetExtendedStyle(dwStyle);
	m_conListCtrl.Arrange(LVA_SNAPTOGRID);
	m_conListCtrl.SetImageList(&m_ImageList,LVSIL_NORMAL);
	for(int i=0;i<m_ImageList.GetImageCount();i++)
	{
		m_ImageList.Remove(i);
	}
	m_conListCtrl.DeleteAllItems();
	m_ImageList.SetImageCount(tem_nPageNumber); 

	wchar_t path[MAX_PATH]; //多字节char
	std::vector<CString>::iterator iter1;
	std::vector<CString>::iterator iter2;
	m_conListCtrl.SetRedraw(false);
	m_nImageIndex = 0;
	m_conListCtrl.SetTextBkColor(m_clBackColor);        //Label标签背景色
	m_conListCtrl.SetTextColor(m_clTextColor);          //Label标签字体色
	for(int i=tem_nOriIndex; i<tem_nOriIndex+tem_nPageNumber; i++, m_nImageIndex++)
	{
		CString    tem_strImgName = m_vcFileName[i];
// 		if (tem_strImgName.IsEmpty())
// 		{
// 			tem_nPageNumber++;
// 		}
// 		else
// 		{
// 			m_conListCtrl.InsertItem(m_nImageIndex, tem_strImgName, m_nImageIndex);
// 		}
		if (!tem_strImgName.IsEmpty())
		{
			m_conListCtrl.InsertItem(m_nImageIndex, tem_strImgName, m_nImageIndex);
		}	
	}
	m_conListCtrl.SetRedraw(true);
	m_conListCtrl.Invalidate();

	HBRUSH hBrushBorder    =::CreateSolidBrush(m_clBackColor);       //缩略图边框色
	HBRUSH hBrushBkground  =::CreateSolidBrush(m_clBackColor);       //缩略图背景色

	RECT rcBorder;
	rcBorder.left=rcBorder.top  =0;
	rcBorder.right              =thumbwidth;
	rcBorder.bottom             =thumbheight;

	float fRatio=(float)thumbheight/thumbwidth; 

	int XDest,YDest,nDestWidth,nDestHeight;
	m_nImageIndex=0;

	
	for (int i=tem_nOriIndex; i<tem_nOriIndex+tem_nPageNumber; i++, m_nImageIndex++)
	{
		CString   tem_strImgPath = m_vcFilePath[i];
// 		if (tem_strImgPath.IsEmpty())
// 		{
// 			continue;	
// 		}		
		_stprintf(path, tem_strImgPath);
		int nImageType=Self_GetTypeFromFileName(path);
		if(nImageType==CXIMAGE_FORMAT_UNKNOWN)
			continue;
		CxImage image;
		image.Load(path, nImageType);
		if(image.IsValid()==false)
			continue;
		float fImgRatio=(float)image.GetHeight()/image.GetWidth();  
		if(fImgRatio>fRatio)
		{
			nDestWidth=(int)thumbheight/fImgRatio;   
			XDest=(thumbwidth-nDestWidth)/2;
			YDest=0;
			nDestHeight=thumbheight;
		}
		else
		{
			XDest=0;
			nDestWidth=thumbwidth;
			nDestHeight=(int)thumbwidth*fImgRatio;  
			YDest=(thumbheight-nDestHeight)/2;
		}

		CClientDC cdc(this);
		HDC hDC=::CreateCompatibleDC(cdc.m_hDC);
		HBITMAP bm=CreateCompatibleBitmap(cdc.m_hDC,thumbwidth,thumbheight);
		HBITMAP pOldBitmapImage=(HBITMAP)SelectObject(hDC,bm);

		::FillRect(hDC,&rcBorder,hBrushBkground);
		image.Stretch(hDC,XDest,YDest,nDestWidth,nDestHeight);
		::FrameRect(hDC,&rcBorder,hBrushBorder);
		SelectObject(hDC,pOldBitmapImage);
		CBitmap bitmap;
		bitmap.Attach(bm);
		m_ImageList.Replace(m_nImageIndex,&bitmap,NULL);
		m_conListCtrl.RedrawItems(m_nImageIndex,m_nImageIndex);

		DeleteDC(hDC);
		DeleteObject(bm);
	}
	DeleteObject(hBrushBorder);
	DeleteObject(hBrushBkground);

	return TRUE;
}


void CUDSPlatformV17Dlg::OnBnClickedBtnLast()
{
	m_nOriImg -= m_nPageNumer;
	m_nPageIndex--;
	int    tem_nBegin = m_nOriImg;
	int    tem_nEnd   = tem_nBegin + m_nPageNumer;
	if (tem_nBegin<0)
	{
		m_nOriImg += m_nPageNumer;
		m_nPageIndex++;
		return;
	}
	else if (tem_nBegin>=0 && tem_nEnd<=m_nFileNumber)
	{
		Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, tem_nBegin, m_nPageNumer);
	}
	else if (tem_nBegin>=0 && tem_nEnd>m_nFileNumber)
	{
		int tem_nResudue = m_nFileNumber - tem_nBegin;
		Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, tem_nBegin, tem_nResudue);
	}
	if (m_BSlcAll)
	{
		Self_HightLightItems();    //全选状态上下翻页高亮
	}
	m_editValue = m_nPageIndex;
	UpdateData(FALSE);

	CRect    tem_rcEdit;

	GetDlgItem(IDC_EDIT_NOWPAGE)->GetWindowRect(&tem_rcEdit); 
	ScreenToClient(&tem_rcEdit);        
	InvalidateRect(&tem_rcEdit);

	UpdateWindow();
	
}


void CUDSPlatformV17Dlg::OnBnClickedBtnNext()
{
	m_nOriImg += m_nPageNumer;
	m_nPageIndex++;
	int     tem_nBegin = m_nOriImg;
	int     tem_nEnd   = tem_nBegin + m_nPageNumer;
	
	if (m_nFileNumber >= tem_nEnd)
	{
		Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, tem_nBegin, m_nPageNumer);
	}
	else if (m_nFileNumber<tem_nEnd && m_nFileNumber>tem_nBegin)
	{
		int tem_nResudue = m_nFileNumber - tem_nBegin;
		Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, tem_nBegin, tem_nResudue);
	}
	else if (m_nFileNumber<=tem_nBegin)
	{
		m_nOriImg -= m_nPageNumer;
		m_nPageIndex--;
		return;
	}

	if (m_BSlcAll)
	{
		Self_HightLightItems();    //全选状态上下翻页高亮
	}
	m_editValue = m_nPageIndex;
	UpdateData(FALSE);

	CRect    tem_rcEdit;

	GetDlgItem(IDC_EDIT_NOWPAGE)->GetWindowRect(&tem_rcEdit); 
	ScreenToClient(&tem_rcEdit);        
	InvalidateRect(&tem_rcEdit);

	UpdateWindow();

}


void CUDSPlatformV17Dlg::OnBnClickedBtnRotatel()
{
	m_vcCtrlIndex.clear();
	POSITION   pos;
	pos = m_conListCtrl.GetFirstSelectedItemPosition();
	while (pos)
	{
		int  tem_nIndex = (int)pos-1;
		m_vcCtrlIndex.push_back(tem_nIndex);
		m_conListCtrl.GetNextSelectedItem(pos);		
	}
	

	if (m_vcCtrlIndex.size()>0)
	{
		CxImage  tem_Image;
		int      tem_nSlctIndex;
		std::vector<int>::iterator tem_itIndex;
		for (tem_itIndex=m_vcCtrlIndex.begin(); tem_itIndex!=m_vcCtrlIndex.end(); tem_itIndex++)
		{
			tem_nSlctIndex  = *tem_itIndex + (m_nPageIndex-1)*m_nPageNumer;
			CString  tem_strFilePath = m_vcBussPath[tem_nSlctIndex];
			//判断文件是否为图像文件
			CString  strMiddle = tem_strFilePath;
			int      nIndex    = strMiddle.ReverseFind('.');
			if (nIndex != -1)
			{
				strMiddle = strMiddle.Mid(nIndex+1);
				strMiddle.MakeLower();
				if (strMiddle == _T("jpg") || strMiddle == _T("jpeg")||
					strMiddle == _T("tif") || strMiddle == _T("tiff")||
					strMiddle == _T("bmp") || strMiddle == _T("png") )	
				{
					int      tem_nImageType = Self_GetTypeFromFileName(tem_strFilePath);
					tem_Image.Load(tem_strFilePath, tem_nImageType);
					tem_Image.Rotate(-90);
					tem_Image.Save(tem_strFilePath, tem_nImageType);
					Self_ReplaceImage(m_nThumbWidth, m_nThumbHeight, *tem_itIndex);
				}
			}		
		}
		m_BSlcAll = FALSE;
		((CButton*)GetDlgItem(IDC_CHK_ALLSLCT))->SetCheck(BST_UNCHECKED); 

		Self_HightLightItems2();
	}

}


void CUDSPlatformV17Dlg::OnBnClickedBtnRotater()
{
	m_vcCtrlIndex.clear();
	POSITION   pos;
	pos = m_conListCtrl.GetFirstSelectedItemPosition();
	while (pos)
	{
		int  tem_nIndex = (int)pos-1;
		m_vcCtrlIndex.push_back(tem_nIndex);
		m_conListCtrl.GetNextSelectedItem(pos);		
	}

	if (m_vcCtrlIndex.size()>0)
	{
		CxImage  tem_Image;
		int     tem_nSlctIndex;
		std::vector<int>::iterator tem_itIndex;
		for (tem_itIndex=m_vcCtrlIndex.begin(); tem_itIndex!=m_vcCtrlIndex.end(); tem_itIndex++)
		{
			tem_nSlctIndex  = *tem_itIndex + (m_nPageIndex-1)*m_nPageNumer;
			CString  tem_strFilePath = m_vcBussPath[tem_nSlctIndex];

			//判断文件是否为图像文件
			CString  strMiddle = tem_strFilePath;
			int      nIndex    = strMiddle.ReverseFind('.');
			if (nIndex != -1)
			{
				strMiddle = strMiddle.Mid(nIndex+1);
				strMiddle.MakeLower();
				if (strMiddle == _T("jpg") || strMiddle == _T("jpeg")||
					strMiddle == _T("tif") || strMiddle == _T("tiff")||
					strMiddle == _T("bmp") || strMiddle == _T("png") )	
				{
					int      tem_nImageType = Self_GetTypeFromFileName(tem_strFilePath);
					tem_Image.Load(tem_strFilePath, tem_nImageType);
					tem_Image.Rotate(90);
					tem_Image.Save(tem_strFilePath, tem_nImageType);
					Self_ReplaceImage(m_nThumbWidth, m_nThumbHeight, *tem_itIndex);
				}
			}		
		}
		m_BSlcAll = FALSE;
		((CButton*)GetDlgItem(IDC_CHK_ALLSLCT))->SetCheck(BST_UNCHECKED); 

		Self_HightLightItems2();
	}	
}


void CUDSPlatformV17Dlg::OnBnClickedBtnDelete()
{
	if (!m_BSlcAll)
	{
		m_vcCtrlIndex.clear();
		POSITION   pos;
		pos = m_conListCtrl.GetFirstSelectedItemPosition();
		while (pos)
		{
			int  tem_nIndex = (int)pos-1;
			m_vcCtrlIndex.push_back(tem_nIndex);
			m_conListCtrl.GetNextSelectedItem(pos);		
		}
	}
	
	if (m_vcCtrlIndex.size()>0)
	{
		int   tem_nOK = MessageBox( _T("确定删除所选项？"), _T("删除"), MB_YESNO);
		if (tem_nOK == 7)
		{
			return;
		}
		int     tem_nSlctIndex;
		sort(m_vcCtrlIndex.rbegin(), m_vcCtrlIndex.rend());
		std::vector<int>::iterator tem_itIndex;
		for (tem_itIndex=m_vcCtrlIndex.begin(); tem_itIndex!=m_vcCtrlIndex.end(); tem_itIndex++)
		{
			if (!m_BSlcAll)
			{
				tem_nSlctIndex  = *tem_itIndex + (m_nPageIndex-1)*m_nPageNumer;
			} 
			else
			{
				tem_nSlctIndex  = *tem_itIndex;
			}
			
			//删除对应图像文件
			CString tem_strFilePath = m_vcBussPath[tem_nSlctIndex];
			BOOL   tem_BRC = Self_DeleteFile2Recycle(tem_strFilePath);
			if (tem_BRC)
			{
				m_vcFileName.erase(m_vcFileName.begin()+tem_nSlctIndex);
				m_vcFilePath.erase(m_vcFilePath.begin()+tem_nSlctIndex);
				m_vcBussPath.erase(m_vcBussPath.begin()+tem_nSlctIndex);
			}		
		}
		m_nFileNumber = m_vcFileName.size();
		if (m_nFileNumber>0)
		{
			//依然有图，更新文件数量
			
			int tem_nBegin = m_nOriImg;
			int tem_nEnd   = tem_nBegin + m_nPageNumer;
			if (tem_nBegin<m_nFileNumber && tem_nEnd<=m_nFileNumber)
			{
				Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, tem_nBegin, m_nPageNumer);
			}
			else if (tem_nBegin<=m_nFileNumber && tem_nEnd>m_nFileNumber)
			{
				int tem_nResudue = m_nFileNumber - tem_nBegin;
				Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, tem_nBegin, tem_nResudue);
			}
			Self_InverHighLightItems();
		}
		else
		{
			Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, 0, 0);
		}
		if (!m_BSlcAll)
		{
			Self_UpdaPagination(m_nFileNumber, FALSE);
		}
		else
		{
			m_BSlcAll = FALSE;
			m_nFileNumber = 0;
			m_nOriImg     = 0;
			((CButton*)GetDlgItem(IDC_CHK_ALLSLCT))->SetCheck(BST_UNCHECKED);  
			GetDlgItem(IDC_STA_CHKNBR)->SetWindowText(_T("已选：0幅"));

			CRect    tem_rcSlcItems;
			GetDlgItem(IDC_STA_CHKNBR)->GetWindowRect(&tem_rcSlcItems);
			ScreenToClient(&tem_rcSlcItems);
			InvalidateRect(&tem_rcSlcItems);

			UpdateWindow();

			Self_UpdaPagination(m_nFileNumber, TRUE);

		}
		
		m_BSlcAll = FALSE;
		

		
	}
}


void CUDSPlatformV17Dlg::OnBnClickedBtnHelp()
{
	// TODO: 在此添加控件通知处理程序代码
	ShellExecute(NULL, _T("open"), _T("快速上手.CHM"), NULL, NULL, SW_SHOWNORMAL);
}


void CUDSPlatformV17Dlg::OnClickedChkAllslct()
{
	// TODO: 在此添加控件通知处理程序代码
	//待完善************************************************
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHK_ALLSLCT))
	{
		m_BSlcAll  = TRUE;
		m_vcCtrlIndex.clear();
		int       tem_nItemNumber   = m_vcFileName.size();
		CString   tem_strItemNumber = _T("");
		for (unsigned int i=0; i<m_vcFileName.size(); i++)   //unsigned int否则会有 “warning C4018: “<”: 有符号/无符号不匹配”的警告
		{
			m_vcCtrlIndex.push_back(i);
		}
		Self_HightLightItems();
		tem_strItemNumber.Format(_T("已选：%d"), tem_nItemNumber);
		tem_strItemNumber += _T("项");
		GetDlgItem(IDC_STA_CHKNBR)->SetWindowText(tem_strItemNumber);		
	}
	else
	{
		m_BSlcAll = FALSE;
		m_vcCtrlIndex.clear();
		Self_InverHighLightItems();
		GetDlgItem(IDC_STA_CHKNBR)->SetWindowText(_T("已选：0项"));
	}

	CRect    tem_rcSTA;
	GetDlgItem(IDC_STA_CHKNBR)->GetWindowRect(&tem_rcSTA); 
	ScreenToClient(&tem_rcSTA);        
	InvalidateRect(&tem_rcSTA);
	UpdateWindow();

	
}


void CUDSPlatformV17Dlg::OnClickListImg(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int    tem_nListIndex = pNMItemActivate->iItem;
	if (tem_nListIndex>=0 && tem_nListIndex<m_conListCtrl.GetItemCount())
	{
		m_BSlcAll = FALSE;
		((CButton*)GetDlgItem(IDC_CHK_ALLSLCT))->SetCheck(BST_UNCHECKED);  
	}
	

	*pResult = 0;
}


void CUDSPlatformV17Dlg::OnHoverListImg(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 1;
}


BOOL CUDSPlatformV17Dlg::Self_ReplaceImage(int thumbwidth, int thumbheight, int item)
{
	CString    tem_strItemName = m_vcFileName[item+(m_nPageIndex-1)*m_nPageNumer];      //地址和名称对应vector中存放的文件的真实索引
	CString    tem_strItemPath = m_vcFilePath[item+(m_nPageIndex-1)*m_nPageNumer];
	m_conListCtrl.SetItemText(item, 0, tem_strItemName);

	HBRUSH hBrushBorder    =::CreateSolidBrush(m_clBackColor);
	HBRUSH hBrushBkground  =::CreateSolidBrush(m_clBackColor);   
	float fRatio=(float)thumbheight/thumbwidth; 
	RECT rcBorder;
	rcBorder.left=rcBorder.top  =0;
	rcBorder.right              =thumbwidth;
	rcBorder.bottom             =thumbheight;

	int XDest,YDest,nDestWidth,nDestHeight;
	CxImage    image; 
	int nImageType=Self_GetTypeFromFileName(tem_strItemPath);
	image.Load(tem_strItemPath, nImageType);
	float fImgRatio=(float)image.GetHeight()/image.GetWidth();  
	if(fImgRatio>fRatio)
	{
		nDestWidth=(int)thumbheight/fImgRatio;   
		XDest=(thumbwidth-nDestWidth)/2;
		YDest=0;
		nDestHeight=thumbheight;
	}
	else
	{
		XDest=0;
		nDestWidth=thumbwidth;
		nDestHeight=(int)thumbwidth*fImgRatio;  
		YDest=(thumbheight-nDestHeight)/2;
	}

	CClientDC cdc(this);
	HDC hDC=::CreateCompatibleDC(cdc.m_hDC);
	HBITMAP bm=CreateCompatibleBitmap(cdc.m_hDC,thumbwidth,thumbheight);
	HBITMAP pOldBitmapImage=(HBITMAP)SelectObject(hDC,bm);

	::FillRect(hDC,&rcBorder,hBrushBkground);
	image.Stretch(hDC,XDest,YDest,nDestWidth,nDestHeight);
	::FrameRect(hDC,&rcBorder,hBrushBorder);
	SelectObject(hDC,pOldBitmapImage);
	CBitmap bitmap;
	bitmap.Attach(bm);
	m_ImageList.Replace(item,&bitmap,NULL);
	m_conListCtrl.RedrawItems(item, item);           //ListCtrl重绘对应当前页面中Item的索引
	DeleteDC(hDC);
	DeleteObject(bm);

	DeleteObject(hBrushBorder);
	DeleteObject(hBrushBkground);	

	return TRUE;
}


int CUDSPlatformV17Dlg::Self_UpdaPagination(int filecount, BOOL mode)
{
	//mode=FALSE时，仅更新总页数
	int      tem_nFileCount   = filecount;
	CString  tem_strFileCount = _T("");
	CString  tem_strFileInfo  = _T("");
	BOOL     tem_BMode        = mode;
	if (tem_nFileCount>0)
	{
		int   tem_nInteger   = tem_nFileCount/m_nPageNumer;    
		int   tem_nRemainder = tem_nFileCount%m_nPageNumer;
		if (tem_nRemainder>0)
		{
			tem_nInteger++;
		}
		m_nPageCount = tem_nInteger;
		CString   tem_strPageNum = _T(""); //页码总数
		CString   tem_strPages   = _T("");
		tem_strPages.Format(_T("%d"), tem_nInteger);
		tem_strPageNum  = _T("共");
		tem_strPageNum += tem_strPages;
		tem_strPageNum += _T("页");
		GetDlgItem(IDC_STA_ALLPAGES)->SetWindowText(tem_strPageNum);

		//更新文件数目
		tem_strFileCount.Format(_T("%d"), tem_nFileCount);
		tem_strFileInfo  = _T("共");
		tem_strFileInfo += tem_strFileCount;
		tem_strFileInfo += _T(" 文件");
		GetDlgItem(IDC_STA_ALLNBR)->SetWindowText(tem_strFileInfo);
			
		if (tem_BMode)
		{
			m_editValue  = 1;
			UpdateData(FALSE);
		}
	}
	else
	{
		GetDlgItem(IDC_STA_ALLPAGES)->SetWindowText(_T("共0页"));
		GetDlgItem(IDC_STA_ALLNBR)->SetWindowText(_T("共0 文件"));
		if (tem_BMode)
		{
			m_nPageCount = 0;
			m_editValue  = 0;
			UpdateData(FALSE);
		}
		
	}
	CRect    tem_rcSTA;

	GetDlgItem(IDC_STA_ALLPAGES)->GetWindowRect(&tem_rcSTA); 
	ScreenToClient(&tem_rcSTA);        
	InvalidateRect(&tem_rcSTA);

	GetDlgItem(IDC_STA_ALLNBR)->GetWindowRect(&tem_rcSTA); 
	ScreenToClient(&tem_rcSTA);        
	InvalidateRect(&tem_rcSTA);


	GetDlgItem(IDC_EDIT_NOWPAGE)->GetWindowRect(&tem_rcSTA); 
	ScreenToClient(&tem_rcSTA);        
	InvalidateRect(&tem_rcSTA);

	UpdateWindow();

	return m_editValue;
}


BOOL CUDSPlatformV17Dlg::Self_DeleteFile2Recycle(CString filepath)
{
	//删除指定文件
	int              tem_nRC;    //ReturnCode操作返回码
	SHFILEOPSTRUCT   FileOp;
	CString          tem_strSrcPath = filepath;
	CString          tem_strDstPath;
	CString          tem_strTitle   = _T("正在删除...");

	tem_strSrcPath += '\0';
	tem_strDstPath  = '\0';     //删除操作，直接置为NULL
	tem_strTitle   += '\0';

	FileOp.fFlags = FOF_ALLOWUNDO|FOF_MULTIDESTFILES|FOF_SIMPLEPROGRESS|FOF_NOCONFIRMATION;
	FileOp.lpszProgressTitle = tem_strTitle;
	FileOp.hwnd              = m_hWnd;
	FileOp.hNameMappings     = NULL;
	FileOp.pFrom             = tem_strSrcPath;
	FileOp.pTo               = tem_strDstPath;    //目标目录
	FileOp.wFunc             = FO_DELETE;
	

	tem_nRC = SHFileOperation(&FileOp);
	if (!FileOp.fAnyOperationsAborted)
	{
		if(tem_nRC)   
		{
			//操作出现错误
			return FALSE;
		}		
	}
	else
	{
		//出现意外中止
		return FALSE;
	}
	return TRUE;
}


void CUDSPlatformV17Dlg::OnDblclkListImg(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int    tem_nListIndex = pNMItemActivate->iItem;
	if (tem_nListIndex>=0 && tem_nListIndex<m_conListCtrl.GetItemCount())
	{
		//修改全选状态
		m_BSlcAll = FALSE;
		 ((CButton*)GetDlgItem(IDC_CHK_ALLSLCT))->SetCheck(BST_UNCHECKED);  

		CString   tem_strFilePath = m_vcBussPath[tem_nListIndex + (m_nPageIndex-1)*m_nPageNumer];
		//获取文件后缀，若文件为PDF文件使用内置PDF文件阅读器
		CString   tem_strFileFormat = PathFindExtension(tem_strFilePath);
		tem_strFileFormat.MakeLower();
		if (tem_strFileFormat == _T(".pdf"))
		{
			//去除路径中的空格
//			tem_strFilePath.Replace(_T(" "), NULL);
			tem_strFilePath.Format(_T("\"%s"), tem_strFilePath);
			CString  tem_strExePath = _T("UDSPDFViewer\\PPFViewerCsharp.exe");
			SHELLEXECUTEINFO    tem_shellInfo;
			memset(&tem_shellInfo, 0, sizeof(SHELLEXECUTEINFO));
			tem_shellInfo.cbSize       = sizeof(SHELLEXECUTEINFO);
			tem_shellInfo.fMask        = SEE_MASK_NOCLOSEPROCESS;
			tem_shellInfo.lpVerb       = _T("open"); 
			tem_shellInfo.lpFile       = tem_strExePath; 
			tem_shellInfo.lpParameters =tem_strFilePath;
			tem_shellInfo.nShow        = SW_SHOW; 
			tem_shellInfo.hwnd         = m_hWnd;
			ShellExecuteEx(&tem_shellInfo); 



		}
		else
		{
			ShellExecute(NULL, _T("open"), tem_strFilePath, NULL, NULL, SW_SHOWNORMAL);
		}
		
		m_conListCtrl.SetItemState(tem_nListIndex, 0, -1);
	}
	*pResult = 0;
}


void CUDSPlatformV17Dlg::OnChangeEditNowpage()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	UpdateData(TRUE);
	int   tem_nNowValue = m_editValue;
	
	if (m_nPageCount>0)
	{
		if (tem_nNowValue>0 && tem_nNowValue<=m_nPageCount)
		{
			m_nPageIndex = tem_nNowValue;         //上下翻页索引记录
			m_nOriImg = (tem_nNowValue-1)*m_nPageNumer;
			int     tem_nBegin = m_nOriImg;
			int     tem_nEnd   = tem_nBegin + m_nPageNumer;

			if (m_nFileNumber >= tem_nEnd)
			{
				Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, tem_nBegin, m_nPageNumer);
			}
			else if (m_nFileNumber<tem_nEnd && m_nFileNumber>tem_nBegin)
			{
				int tem_nResudue = m_nFileNumber - tem_nBegin;
				Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, tem_nBegin, tem_nResudue);
			}
			if (m_BSlcAll)
			{
				Self_HightLightItems();    //全选状态上下翻页高亮
			}
			
		}
	}
	//刷新EditControl
	CRect    tem_rcEdit;

	GetDlgItem(IDC_EDIT_NOWPAGE)->GetWindowRect(&tem_rcEdit); 
	ScreenToClient(&tem_rcEdit);        
	InvalidateRect(&tem_rcEdit);

	UpdateWindow();

}


void CUDSPlatformV17Dlg::OnChangeEditSearch()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CString   tem_strSearch = _T("");
	GetDlgItem(IDC_EDIT_SEARCH)->GetWindowText(tem_strSearch);
	HTREEITEM    tem_hSlcItem   = m_conTreeCtrl.GetSelectedItem();
	CString      tem_strSlcItemPath = Self_GetItemInfo(tem_hSlcItem, 1);
	int tem_nFileNumber = Self_FindAllFiles(tem_strSlcItemPath);
	m_nFileNumber       = tem_nFileNumber;
	if (tem_strSearch.IsEmpty())
	{
		//重新显示当前文件夹，通过节点获取当前的目录
		m_nOriImg      = 0;
		
		if (tem_nFileNumber == 0)
		{
			//该目录下没有图像文件,清空显示
			Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, m_nOriImg, 0);
		}
		else if (tem_nFileNumber >= m_nPageNumer)
		{
			Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, m_nOriImg, m_nPageNumer);
		}
		else
		{
			Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, m_nOriImg, tem_nFileNumber);
		}

	}
	else
	{
		//根据搜索结果，刷新显示
		int   tem_nFindIndex = -1;
		std::vector<CString>::iterator  item_str;
		std::vector<CString>::iterator  item_name;
		std::vector<CString>::iterator  item_path;
		std::vector<CString>::iterator  item_buss;
		item_path = m_vcFilePath.begin();
		item_buss = m_vcBussPath.begin();
		for (item_name=m_vcFileName.begin(); item_name!=m_vcFileName.end(); )
		{
			CString   tem_strName = *item_name;
			tem_nFindIndex = tem_strName.Find(tem_strSearch);
			if (tem_nFindIndex == -1)
			{
				//该项不包含关键字，erase处理
				item_str = item_name;
				m_vcFileName.erase(item_str);
				item_str = item_path;
				m_vcFilePath.erase(item_str);
				item_str = item_buss;
				m_vcBussPath.erase(item_str);
			}
			else
			{
				item_name++;
				item_path++;
				item_buss++;
			}
		}
		m_nFileNumber       = m_vcFileName.size();
		if (m_nFileNumber == 0)
		{
			//该目录下没有图像文件,清空显示
			Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, m_nOriImg, 0);
		}
		else if (m_nFileNumber >= m_nPageNumer)
		{
			Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, m_nOriImg, m_nPageNumer);
		}
		else
		{
			Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, m_nOriImg, m_nFileNumber);
		}
	}
	CRect    tem_rcEdit;

	GetDlgItem(IDC_EDIT_SEARCH)->GetWindowRect(&tem_rcEdit); 
	ScreenToClient(&tem_rcEdit);        
	InvalidateRect(&tem_rcEdit);

//	UpdateWindow();

}


void CUDSPlatformV17Dlg::Self_HightLightItems(void)
{
	m_conListCtrl.SetFocus();
	for (int i=0; i<m_conListCtrl.GetItemCount(); i++)
	{
		m_conListCtrl.SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED,LVIS_FOCUSED | LVIS_SELECTED);
	}	
}


void CUDSPlatformV17Dlg::Self_HightLightItems2(void)
{
	m_conListCtrl.SetFocus();
	std::vector<int>::iterator tem_itIndex;
	for (tem_itIndex=m_vcCtrlIndex.begin(); tem_itIndex!=m_vcCtrlIndex.end(); tem_itIndex++)
	{
		int tem_nSlctIndex  = *tem_itIndex + (m_nPageIndex-1)*m_nPageNumer;
		m_conListCtrl.SetItemState(tem_nSlctIndex, LVIS_FOCUSED | LVIS_SELECTED,LVIS_FOCUSED | LVIS_SELECTED);
	}
}


void CUDSPlatformV17Dlg::Self_InverHighLightItems(void)
{
	m_conListCtrl.SetFocus();
	for (int i=0; i<m_conListCtrl.GetItemCount(); i++)
	{
		m_conListCtrl.SetItemState(i, 0, -1);
	}	
}



//##################################################################################################################

/**********************************
*         函数目录
*说明：速拍仪、扫描仪、读卡器模块
*
* 1、打开速拍仪
* 2、打开扫描仪
* 3、身份证读卡
*
***********************************/
void CUDSPlatformV17Dlg::OnBnClickedBtnCamera()
{
	m_BSlcAll = FALSE;
	((CButton*)GetDlgItem(IDC_CHK_ALLSLCT))->SetCheck(BST_UNCHECKED);  

//	ShowWindow(SW_MINIMIZE);
	ShellExecute(NULL, _T("open"), _T("CameraTools\\UDSCameraTool.exe"), NULL, NULL, SW_SHOWNORMAL);
	
}


void CUDSPlatformV17Dlg::OnBnClickedBtnScanner()
{
	m_BSlcAll = FALSE;
	((CButton*)GetDlgItem(IDC_CHK_ALLSLCT))->SetCheck(BST_UNCHECKED); 

//	ShowWindow(SW_MINIMIZE);
	ShellExecute(NULL, _T("open"), _T("ScannerTools\\UDSScannerTool.exe"), NULL, NULL, SW_SHOWNORMAL);	
}


void CUDSPlatformV17Dlg::OnBnClickedBtnRecard()
{
	m_BSlcAll = FALSE;
	((CButton*)GetDlgItem(IDC_CHK_ALLSLCT))->SetCheck(BST_UNCHECKED);  
	m_dlgRecard.DoModal();
}


//##################################################################################################################

/**********************************
*         函数目录
*说明：图像导出
*
* 1、Menu初始化
* 2、图像导出按钮
* 3、打印
* 4、复制
* 5、导出为多页PDF
* 6、导出为多页TIF
* 7、导出多页PDF
* 8、导出多页TIF
*
***********************************/
BOOL CUDSPlatformV17Dlg::CreateHSMenu(void)
{
	COLORREF clrSet0 = RGB(185,205,246);
	COLORREF clrSet1 = RGB(222,160,228);
	COLORREF clrSet2 = RGB(131,88,136);

	HWND hwnd = GetSafeHwnd();
	m_mnuOutPut.CreateMenu(hwnd);
	CHSMenu::SetMenuItemHeight(28);  //设置Menu高度
	CHSMenuPro::SetColor(CLR_BKGNDMENU, clrSet0); //设置底色
	CHSMenuPro::SetColor(CLR_BKGNDSEL, clrSet1);	//设置被选中时底色
	CHSMenuPro::SetColor(CLR_BORDERSPR, clrSet2);	//设置分支颜色
	CHSMenuPro::SetMenuStyle(ML_ARROWROUND, true);
	CHSMenuPro::SetMenuStyle(ML_SELGRADIENT_V, true);


	m_mnuOutPut.CreateMenu(hwnd);
	m_mnuOutPut.AppendMenu(_T("打印"), 32771, 0, 0);
	m_mnuOutPut.AppendMenu(_T("复制到..."), 32772, 0, 0);
	m_mnuOutPut.AppendMenu(_T("导出为PDF"), 32773, 0, 0);
	m_mnuOutPut.AppendMenu(_T("导出为多页TIF"), 32774, 0, 0);
	m_mnuOutPut.AppendMenu(_T("导出为OFD"), 32793, 0, 0);
	m_mnuOutPut.AppendMenu(_T("全部导出为PDF"), 32787, 0, 0);
	m_mnuOutPut.AppendMenu(_T("全部导出为TIF"), 32788, 0, 0);
	m_mnuOutPut.AppendMenu(_T("全部导出为OFD"), 32790, 0, 0);

	m_mnuImgEdit.CreateMenu(hwnd);
	m_mnuImgEdit.AppendMenu(_T("画笔工具"), 32775, 0, 0);
	m_mnuImgEdit.AppendMenu(_T("编辑工具"), 32776, 0, 0);
	m_mnuImgEdit.AppendMenu(_T("批量命名"), 32777, 0, 0);
	m_mnuImgEdit.AppendMenu(_T("批量合并"), 32778, 0, 0);
//	m_mnuImgEdit.AppendMenu(_T("修改图像尺寸"), 32786, 0, 0);

	
	m_mmImgSort.CreateMenu(hwnd);
	m_mmImgSort.SetMenuItemHeight(20);  //设置Menu高度

	m_mmImgSort.AppendMenu(_T("默认"),32781, 0, 0);
	m_mmImgSort.AppendMenu(_T("创建时间升序"),32782, 0, 0);
	m_mmImgSort.AppendMenu(_T("创建时间降序"),32783, 0, 0);
	m_mmImgSort.AppendMenu(_T("文件大小升序"),32784, 0, 0);
	m_mmImgSort.AppendMenu(_T("文件大小降序"),32785, 0, 0);
	

	return TRUE;
}


void CUDSPlatformV17Dlg::OnBnClickedBtnSort()
{
	// TODO: 在此添加控件通知处理程序代码
	CRect   tem_rcBtnSort;
	GetDlgItem(IDC_BTN_SORT)->GetWindowRect(&tem_rcBtnSort);
	m_mmImgSort.SetMenuWidth(150);
	m_mmImgSort.TrackPopMenu(tem_rcBtnSort.left, tem_rcBtnSort.top, TPM_BOTTOMALIGN);
}


void CUDSPlatformV17Dlg::OnBnClickedBtnOutput()
{
	// TODO: 在此添加控件通知处理程序代码
	CRect    tem_rcBtnOutput;
	GetDlgItem(IDC_BTN_OUTPUT)->GetWindowRect(&tem_rcBtnOutput);
	m_mnuOutPut.TrackPopMenu(tem_rcBtnOutput.left, tem_rcBtnOutput.top, TPM_BOTTOMALIGN);

//	Self_HightLightItems2();
}


void CUDSPlatformV17Dlg::OnPrintImgs()
{
	m_vcCtrlIndex.clear();
	POSITION   pos;
	pos = m_conListCtrl.GetFirstSelectedItemPosition();
	while (pos)
	{
		int  tem_nIndex = (int)pos-1;
		m_vcCtrlIndex.push_back(tem_nIndex);
		m_conListCtrl.GetNextSelectedItem(pos);		
	}

	if (m_vcCtrlIndex.size()>0)
	{
		int       tem_nSlctIndex  = m_vcCtrlIndex.back() + (m_nPageIndex-1)*m_nPageNumer; 
		CString   tem_strName     = m_vcFileName[tem_nSlctIndex];
		CString   tem_strPath     = m_vcBussPath[tem_nSlctIndex];
		ShellExecute(NULL, _T("print"), tem_strPath, NULL, NULL, SW_SHOWNORMAL);

//		ShellExecute(NULL, _T("print"), _T("C:\\Users\\Administrator\\Desktop\\timg.jpg C:\\Users\\Administrator\\Desktop\\主界面.jpg"), NULL, NULL, SW_SHOWNORMAL);

	}	
	//清空Item的选择状态
	std::vector<int>::iterator tem_itIndex;
	for (tem_itIndex=m_vcCtrlIndex.begin(); tem_itIndex!=m_vcCtrlIndex.end(); tem_itIndex++)
	{
		m_conListCtrl.SetItemState(*tem_itIndex, 0, -1);
	}

	//点击菜单后取消Checkbox的全选状态
	m_BSlcAll = FALSE;
	((CButton*)GetDlgItem(IDC_CHK_ALLSLCT))->SetCheck(FALSE);
	m_vcCtrlIndex.clear();
	GetDlgItem(IDC_STA_CHKNBR)->SetWindowText(_T("已选：0幅"));
	CRect    tem_rcSTA;
	GetDlgItem(IDC_STA_CHKNBR)->GetWindowRect(&tem_rcSTA); 
	ScreenToClient(&tem_rcSTA);        
	InvalidateRect(&tem_rcSTA);
	UpdateWindow();
	
}


void CUDSPlatformV17Dlg::OnCopyImgs()
{
	// TODO: 在此添加命令处理程序代码
	m_vcCtrlIndex.clear();
	POSITION   pos;
	pos = m_conListCtrl.GetFirstSelectedItemPosition();
	while (pos)
	{
		int  tem_nIndex = (int)pos-1;
		m_vcCtrlIndex.push_back(tem_nIndex);
		m_conListCtrl.GetNextSelectedItem(pos);		
	}

	if (m_vcCtrlIndex.size()>0)
	{

		TCHAR     m_selectpath[MAX_PATH];
		CString  m_goalpath;
		CString  p_goalpath;
		ZeroMemory(m_selectpath, sizeof(m_selectpath));
		BROWSEINFO   m_broseinfo;
		m_broseinfo.hwndOwner = m_hWnd;
		m_broseinfo.pidlRoot  = NULL;
		m_broseinfo.pszDisplayName = m_selectpath;
		m_broseinfo.lpszTitle = _T("请选择要保存目录");
		m_broseinfo.ulFlags   = 0;
		m_broseinfo.lpfn      = NULL;
		m_broseinfo.lParam    = 0;
		m_broseinfo.iImage    = 0;
		LPITEMIDLIST   lp = SHBrowseForFolder(&m_broseinfo);
		std::vector<int>::iterator tem_itIndex;
		if (lp&&SHGetPathFromIDList(lp, m_selectpath))
		{
			m_goalpath.Format(_T("%s"), m_selectpath);
//			m_goalpath.Replace(_T("\\"), _T("\\\\"));
			m_goalpath =m_goalpath+_T("\\\\");
			//将m_vcCtrlpath中文件复制到指定位置
			for (tem_itIndex=m_vcCtrlIndex.begin(); tem_itIndex!=m_vcCtrlIndex.end(); tem_itIndex++)
			{
				int       tem_nSlctIndex  = *tem_itIndex + (m_nPageIndex-1)*m_nPageNumer; 
				CString   tem_strSrc = m_vcBussPath[tem_nSlctIndex];
				CString   tem_strDst = m_goalpath + m_vcFileName[tem_nSlctIndex];
				CopyFile(tem_strSrc, tem_strDst, TRUE);
			}
			MessageBox(_T("复制完毕！"), _T("图像复制"), MB_OK);
	    }
		//清空Item的选择状态
		for (tem_itIndex=m_vcCtrlIndex.begin(); tem_itIndex!=m_vcCtrlIndex.end(); tem_itIndex++)
		{
			m_conListCtrl.SetItemState(*tem_itIndex, 0, -1);
		}
		
	}

	//点击菜单后取消Checkbox的全选状态
	m_BSlcAll = FALSE;
	((CButton*)GetDlgItem(IDC_CHK_ALLSLCT))->SetCheck(FALSE);
	m_vcCtrlIndex.clear();
	GetDlgItem(IDC_STA_CHKNBR)->SetWindowText(_T("已选：0幅"));
	CRect    tem_rcSTA;
	GetDlgItem(IDC_STA_CHKNBR)->GetWindowRect(&tem_rcSTA); 
	ScreenToClient(&tem_rcSTA);        
	InvalidateRect(&tem_rcSTA);
	UpdateWindow();
}


void CUDSPlatformV17Dlg::OnPDFImgs()
{
	// TODO: 在此添加命令处理程序代码
	m_vcCtrlIndex.clear();
	POSITION   pos;
	pos = m_conListCtrl.GetFirstSelectedItemPosition();
	while (pos)
	{
		int  tem_nIndex = (int)pos-1;
		m_vcCtrlIndex.push_back(tem_nIndex);
		m_conListCtrl.GetNextSelectedItem(pos);		
	}

	if (m_vcCtrlIndex.size()>0)
	{
		std::vector<int>::iterator tem_itIndex;
		for (tem_itIndex=m_vcCtrlIndex.begin(); tem_itIndex!=m_vcCtrlIndex.end(); tem_itIndex++)
		{
			int       tem_nSlctIndex  = *tem_itIndex + (m_nPageIndex-1)*m_nPageNumer; 
			CString   tem_strName     = m_vcFileName[tem_nSlctIndex];
			CString   tem_strPath     = m_vcBussPath[tem_nSlctIndex];
			//判断是否为图像文件
			CString  strMiddle = tem_strPath;
			int      nIndex    = strMiddle.ReverseFind('.');
			if (nIndex != -1)
			{
				strMiddle = strMiddle.Mid(nIndex+1);
				strMiddle.MakeLower();
				if (strMiddle == _T("jpg") || strMiddle == _T("jpeg")||
					strMiddle == _T("tif") || strMiddle == _T("tiff")||
					strMiddle == _T("bmp") || strMiddle == _T("png") )	
				{
					g_vcCtrlName.push_back(tem_strName);
					g_vcCtrlPath.push_back(tem_strPath);
				}
			}	
		}
		if (g_vcCtrlName.size()>0)
		{
			m_dlgOutpath.m_BPdfTif = TRUE;
			m_dlgOutpath.m_BOFD = FALSE;
			m_dlgOutpath.DoModal();

			//清空Item的选择状态
			for (tem_itIndex=m_vcCtrlIndex.begin(); tem_itIndex!=m_vcCtrlIndex.end(); tem_itIndex++)
			{
				m_conListCtrl.SetItemState(*tem_itIndex, 0, -1);
			}
			//清空vector容器
			g_vcCtrlName.clear();
			g_vcCtrlPath.clear();
			m_vcCtrlIndex.clear();
		}	
	}

	//点击菜单后取消Checkbox的全选状态
	m_BSlcAll = FALSE;
	((CButton*)GetDlgItem(IDC_CHK_ALLSLCT))->SetCheck(FALSE);
	m_vcCtrlIndex.clear();
	GetDlgItem(IDC_STA_CHKNBR)->SetWindowText(_T("已选：0幅"));
	CRect    tem_rcSTA;
	GetDlgItem(IDC_STA_CHKNBR)->GetWindowRect(&tem_rcSTA); 
	ScreenToClient(&tem_rcSTA);        
	InvalidateRect(&tem_rcSTA);
	UpdateWindow();
}


void CUDSPlatformV17Dlg::OnTIFImgs()
{
	// TODO: 在此添加命令处理程序代码
	m_vcCtrlIndex.clear();
	POSITION   pos;
	pos = m_conListCtrl.GetFirstSelectedItemPosition();
	while (pos)
	{
		int  tem_nIndex = (int)pos-1;
		m_vcCtrlIndex.push_back(tem_nIndex);
		m_conListCtrl.GetNextSelectedItem(pos);		
	}

	if (m_vcCtrlIndex.size()>0)
	{
		std::vector<int>::iterator tem_itIndex;
		for (tem_itIndex=m_vcCtrlIndex.begin(); tem_itIndex!=m_vcCtrlIndex.end(); tem_itIndex++)
		{
			int       tem_nSlctIndex  = *tem_itIndex + (m_nPageIndex-1)*m_nPageNumer; 
			CString   tem_strName     = m_vcFileName[tem_nSlctIndex];
			CString   tem_strPath     = m_vcBussPath[tem_nSlctIndex];
			//判断是否为图像文件
			CString  strMiddle = tem_strPath;
			int      nIndex    = strMiddle.ReverseFind('.');
			if (nIndex != -1)
			{
				strMiddle = strMiddle.Mid(nIndex+1);
				strMiddle.MakeLower();
				if (strMiddle == _T("jpg") || strMiddle == _T("jpeg")||
					strMiddle == _T("tif") || strMiddle == _T("tiff")||
					strMiddle == _T("bmp") || strMiddle == _T("png") )	
				{
					g_vcCtrlName.push_back(tem_strName);
					g_vcCtrlPath.push_back(tem_strPath);
				}
			}	
		}
		if (g_vcCtrlName.size()>0)
		{
			m_dlgOutpath.m_BPdfTif = FALSE;
			m_dlgOutpath.m_BOFD = FALSE;
			m_dlgOutpath.DoModal();

			//清空Item的选择状态
			for (tem_itIndex=m_vcCtrlIndex.begin(); tem_itIndex!=m_vcCtrlIndex.end(); tem_itIndex++)
			{
				m_conListCtrl.SetItemState(*tem_itIndex, 0, -1);
			}
			//清空vector容器
			g_vcCtrlName.clear();
			g_vcCtrlPath.clear();
			m_vcCtrlIndex.clear();
		}
	}

	//点击菜单后取消Checkbox的全选状态
	m_BSlcAll = FALSE;
	((CButton*)GetDlgItem(IDC_CHK_ALLSLCT))->SetCheck(FALSE);
	m_vcCtrlIndex.clear();
	GetDlgItem(IDC_STA_CHKNBR)->SetWindowText(_T("已选：0幅"));
	CRect    tem_rcSTA;
	GetDlgItem(IDC_STA_CHKNBR)->GetWindowRect(&tem_rcSTA); 
	ScreenToClient(&tem_rcSTA);        
	InvalidateRect(&tem_rcSTA);
	UpdateWindow();
}


void CUDSPlatformV17Dlg::On32787()
{
	// TODO: 在此添加命令处理程序代码
	if (m_vcFileName.size()>0)
	{
		std::vector<CString>::iterator tem_itIndex;
		std::vector<CString>::iterator tem_itIndex2;
		for (tem_itIndex=m_vcFileName.begin(), tem_itIndex2=m_vcBussPath.begin(); tem_itIndex!=m_vcFileName.end(); tem_itIndex++,tem_itIndex2++)
		{
			CString   tem_strName     = *tem_itIndex;
			CString   tem_strPath     = *tem_itIndex2;
			//判断是否为图像文件
			CString  strMiddle = tem_strPath;
			int      nIndex    = strMiddle.ReverseFind('.');
			if (nIndex != -1)
			{
				strMiddle = strMiddle.Mid(nIndex+1);
				strMiddle.MakeLower();
				if (strMiddle == _T("jpg") || strMiddle == _T("jpeg")||
					strMiddle == _T("tif") || strMiddle == _T("tiff")||
					strMiddle == _T("bmp") || strMiddle == _T("png") )	
				{
					g_vcCtrlName.push_back(tem_strName);
					g_vcCtrlPath.push_back(tem_strPath);
				}
			}	
		}
		if (g_vcCtrlName.size()>0)
		{
			m_dlgOutpath.m_BPdfTif = TRUE;
			m_dlgOutpath.m_BOFD = FALSE;
			m_dlgOutpath.DoModal();

			//清空vector容器
			g_vcCtrlName.clear();
			g_vcCtrlPath.clear();
		}	
	}
}


void CUDSPlatformV17Dlg::On32788()
{
	// TODO: 在此添加命令处理程序代码
	if (m_vcFileName.size()>0)
	{
		std::vector<CString>::iterator tem_itIndex;
		std::vector<CString>::iterator tem_itIndex2;
		for (tem_itIndex=m_vcFileName.begin(), tem_itIndex2=m_vcBussPath.begin(); tem_itIndex!=m_vcFileName.end(); tem_itIndex++,tem_itIndex2++)
		{
			CString   tem_strName     = *tem_itIndex;
			CString   tem_strPath     = *tem_itIndex2;
			//判断是否为图像文件
			CString  strMiddle = tem_strPath;
			int      nIndex    = strMiddle.ReverseFind('.');
			if (nIndex != -1)
			{
				strMiddle = strMiddle.Mid(nIndex+1);
				strMiddle.MakeLower();
				if (strMiddle == _T("jpg") || strMiddle == _T("jpeg")||
					strMiddle == _T("tif") || strMiddle == _T("tiff")||
					strMiddle == _T("bmp") || strMiddle == _T("png") )	
				{
					g_vcCtrlName.push_back(tem_strName);
					g_vcCtrlPath.push_back(tem_strPath);
				}
			}	
		}
		if (g_vcCtrlName.size()>0)
		{
			m_dlgOutpath.m_BPdfTif = FALSE;
			m_dlgOutpath.m_BOFD = FALSE;
			m_dlgOutpath.DoModal();

			//清空vector容器
			g_vcCtrlName.clear();
			g_vcCtrlPath.clear();
		}
	}
}


void CUDSPlatformV17Dlg::On32791()
{
	// TODO: 在此添加命令处理程序代码
	MessageBox(_T("OFD"));
}


void CUDSPlatformV17Dlg::On32793()
{
	// TODO: 在此添加命令处理程序代码
	m_vcCtrlIndex.clear();
	POSITION   pos;
	pos = m_conListCtrl.GetFirstSelectedItemPosition();
	while (pos)
	{
		int  tem_nIndex = (int)pos-1;
		m_vcCtrlIndex.push_back(tem_nIndex);
		m_conListCtrl.GetNextSelectedItem(pos);		
	}

	if (m_vcCtrlIndex.size()>0)
	{
		std::vector<int>::iterator tem_itIndex;
		for (tem_itIndex=m_vcCtrlIndex.begin(); tem_itIndex!=m_vcCtrlIndex.end(); tem_itIndex++)
		{
			int       tem_nSlctIndex  = *tem_itIndex + (m_nPageIndex-1)*m_nPageNumer; 
			CString   tem_strName     = m_vcFileName[tem_nSlctIndex];
			CString   tem_strPath     = m_vcBussPath[tem_nSlctIndex];
			//判断是否为图像文件
			CString  strMiddle = tem_strPath;
			int      nIndex    = strMiddle.ReverseFind('.');
			if (nIndex != -1)
			{
				strMiddle = strMiddle.Mid(nIndex+1);
				strMiddle.MakeLower();
				if (strMiddle == _T("jpg") || strMiddle == _T("jpeg")||
					strMiddle == _T("tif") || strMiddle == _T("tiff")||
					strMiddle == _T("bmp") || strMiddle == _T("png") )	
				{
					g_vcCtrlName.push_back(tem_strName);
					g_vcCtrlPath.push_back(tem_strPath);
				}
			}	
		}
		if (g_vcCtrlName.size()>0)
		{
			m_dlgOutpath.m_BPdfTif = FALSE;
			m_dlgOutpath.m_BOFD = TRUE;
			m_dlgOutpath.DoModal();

			//清空Item的选择状态
			for (tem_itIndex=m_vcCtrlIndex.begin(); tem_itIndex!=m_vcCtrlIndex.end(); tem_itIndex++)
			{
				m_conListCtrl.SetItemState(*tem_itIndex, 0, -1);
			}
			//清空vector容器
			g_vcCtrlName.clear();
			g_vcCtrlPath.clear();
			m_vcCtrlIndex.clear();
		}	
	}

	//点击菜单后取消Checkbox的全选状态
	m_BSlcAll = FALSE;
	((CButton*)GetDlgItem(IDC_CHK_ALLSLCT))->SetCheck(FALSE);
	m_vcCtrlIndex.clear();
	GetDlgItem(IDC_STA_CHKNBR)->SetWindowText(_T("已选：0幅"));
	CRect    tem_rcSTA;
	GetDlgItem(IDC_STA_CHKNBR)->GetWindowRect(&tem_rcSTA); 
	ScreenToClient(&tem_rcSTA);        
	InvalidateRect(&tem_rcSTA);
	UpdateWindow();
}

void CUDSPlatformV17Dlg::On32790()
{
	// TODO: 在此添加命令处理程序代码
	MessageBox(_T("OFD2"));
}


//##################################################################################################################

/**********************************
*         函数目录
*说明：图像编辑
*
* 1、图像编辑按钮
* 2、画笔
* 3、编辑
* 4、批量重命名
* 5、批量合并
* 6、批量修改图像尺寸
*
***********************************/


void CUDSPlatformV17Dlg::OnBnClickedBtnProcess()
{
	// TODO: 在此添加控件通知处理程序代码
	CRect    tem_rcBtnImgEdit;
	GetDlgItem(IDC_BTN_PROCESS)->GetWindowRect(&tem_rcBtnImgEdit);
	m_mnuImgEdit.TrackPopMenu(tem_rcBtnImgEdit.left, tem_rcBtnImgEdit.top, TPM_BOTTOMALIGN);    //TPM_BOTTOMALIGN和TOPALIGN都是顶端对齐，因此增加了偏移量

//	Self_HightLightItems2();
}


void CUDSPlatformV17Dlg::OnBrush()
{
	// TODO: 在此添加命令处理程序代码
	m_vcCtrlIndex.clear();
	POSITION   pos;
	pos = m_conListCtrl.GetFirstSelectedItemPosition();
	while (pos)
	{
		int  tem_nIndex = (int)pos-1;
		m_vcCtrlIndex.push_back(tem_nIndex);
		m_conListCtrl.GetNextSelectedItem(pos);		
	}

	if (m_vcCtrlIndex.size()>0)
	{
		int       tem_nSlctIndex  = m_vcCtrlIndex.back() + (m_nPageIndex-1)*m_nPageNumer; 
		CString   tem_strName     = m_vcFileName[tem_nSlctIndex];
		CString   tem_strPath     = m_vcFilePath[tem_nSlctIndex];
		tem_strPath.Format(_T("\"%s"),tem_strPath);
		ShellExecute(NULL, _T("open"), _T("mspaint.exe"), tem_strPath, NULL, SW_SHOWNORMAL);
	}	

	//点击菜单后取消Checkbox的全选状态
	m_BSlcAll = FALSE;
	Self_InverHighLightItems();
	((CButton*)GetDlgItem(IDC_CHK_ALLSLCT))->SetCheck(FALSE);
	m_vcCtrlIndex.clear();
	GetDlgItem(IDC_STA_CHKNBR)->SetWindowText(_T("已选：0幅"));
	CRect    tem_rcSTA;
	GetDlgItem(IDC_STA_CHKNBR)->GetWindowRect(&tem_rcSTA); 
	ScreenToClient(&tem_rcSTA);        
	InvalidateRect(&tem_rcSTA);
	UpdateWindow();

}


void CUDSPlatformV17Dlg::OnProcess()
{
	// TODO: 在此添加命令处理程序代码
	m_vcCtrlIndex.clear();
	POSITION   pos;
	pos = m_conListCtrl.GetFirstSelectedItemPosition();
	while (pos)
	{
		int  tem_nIndex = (int)pos-1;
		m_vcCtrlIndex.push_back(tem_nIndex);
		m_conListCtrl.GetNextSelectedItem(pos);		
	}

	if (m_vcCtrlIndex.size()>0)
	{
		int       tem_nSlctIndex  = m_vcCtrlIndex.back() + (m_nPageIndex-1)*m_nPageNumer; 
		CString   tem_strName     = m_vcFileName[tem_nSlctIndex];
		CString   tem_strPath     = m_vcFilePath[tem_nSlctIndex];
		
		//判断该图像是否为tif文件
		CString tem_strFormat = PathFindExtension(tem_strPath);
		tem_strFormat.MakeLower();
		int  tem_nNum = -1;
		if (tem_strFormat==_T(".tif")||tem_strFormat==_T(".tiff"))
		{
			//若为tif文件，判断该图像是否为多层图像
			CxImage  tem_cxMutiImg;
			tem_cxMutiImg.Load(tem_strPath, CXIMAGE_FORMAT_TIF);
			tem_nNum = tem_cxMutiImg.GetNumFrames();
			if (tem_nNum == 1)
			{
				tem_strPath.Format(_T("\"%s"),tem_strPath);
				ShellExecute(NULL, _T("open"), _T("UDSImageProcess\\UDSImageProcess.exe"), tem_strPath, NULL, SW_SHOWNORMAL);
			}
			else
			{
				MessageBox(_T("图像编辑不支持多页TIF文件！"), _T("紫图UDS"), MB_OK);
			}
		}
		else 
		{
			tem_strPath.Format(_T("\"%s"),tem_strPath);
			ShellExecute(NULL, _T("open"), _T("UDSImageProcess\\UDSImageProcess.exe"), tem_strPath, NULL, SW_SHOWNORMAL);
		}
		
	}

	//点击菜单后取消Checkbox的全选状态
	m_BSlcAll = FALSE;
	Self_InverHighLightItems();
	((CButton*)GetDlgItem(IDC_CHK_ALLSLCT))->SetCheck(FALSE);
	m_vcCtrlIndex.clear();
	GetDlgItem(IDC_STA_CHKNBR)->SetWindowText(_T("已选：0幅"));
	CRect    tem_rcSTA;
	GetDlgItem(IDC_STA_CHKNBR)->GetWindowRect(&tem_rcSTA); 
	ScreenToClient(&tem_rcSTA);        
	InvalidateRect(&tem_rcSTA);
	UpdateWindow();
	
}


void CUDSPlatformV17Dlg::OnBatchRename()
{
	// TODO: 在此添加命令处理程序代码
	if(m_vcFileName.size()>0)
	{
		int   tem_nRC = m_dlgRename.DoModal();
		if (g_BRenamed)
		{
			//获取当前节点
			HTREEITEM    tem_hSlcItem   = m_conTreeCtrl.GetSelectedItem();
			//获取该节点路径
			CString      tem_strSlcItemPath = Self_GetItemInfo(tem_hSlcItem, 1);	
			
			//获取当前目录， 获取当前显示页，重新加载
			m_nOriImg      = 0;
			int tem_nFileNumber = Self_FindAllFiles(tem_strSlcItemPath);
			m_nFileNumber       = tem_nFileNumber;
			if (tem_nFileNumber == 0)
			{
				//该目录下没有图像文件,清空显示
				Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, m_nOriImg, 0);
			}
			else if (tem_nFileNumber >= m_nPageNumer)
			{
				Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, m_nOriImg, m_nPageNumer);
			}
			else
			{
				Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, m_nOriImg, tem_nFileNumber);
			}

			((CButton*)GetDlgItem(IDC_CHK_ALLSLCT))->SetCheck(BST_UNCHECKED);  

			g_BRenamed = FALSE;

			//将搜索框置为NULL
			GetDlgItem(IDC_EDIT_SEARCH)->SetWindowText(_T(""));
			CRect    tem_rcEdit;

			GetDlgItem(IDC_EDIT_SEARCH)->GetWindowRect(&tem_rcEdit); 
			ScreenToClient(&tem_rcEdit);        
			InvalidateRect(&tem_rcEdit);
			UpdateWindow();

//			MessageBox(_T("重命名完成！"));

		}
	}

	//点击菜单后取消Checkbox的全选状态
	m_BSlcAll = FALSE;
	Self_InverHighLightItems();
	((CButton*)GetDlgItem(IDC_CHK_ALLSLCT))->SetCheck(FALSE);
	m_vcCtrlIndex.clear();
	GetDlgItem(IDC_STA_CHKNBR)->SetWindowText(_T("已选：0幅"));
	CRect    tem_rcSTA;
	GetDlgItem(IDC_STA_CHKNBR)->GetWindowRect(&tem_rcSTA); 
	ScreenToClient(&tem_rcSTA);        
	InvalidateRect(&tem_rcSTA);
	UpdateWindow();
	
	
}


void CUDSPlatformV17Dlg::OnBatchMerge()
{
	// TODO: 在此添加命令处理程序代码
	if(m_vcFileName.size()>0)
	{
		int tem_nRC = m_dlgMerge.DoModal();
		if (g_BMerged)
		{
			
			HTREEITEM    tem_hSlcItem   = m_conTreeCtrl.GetSelectedItem();            //获取当前节点
			CString      tem_strSlcItemPath = Self_GetItemInfo(tem_hSlcItem, 1);	  //获取该节点路径

			
			//1)查询当前节点下是否有“合并图”节点
			BOOL         tem_BFindIt    = FALSE;
			CString      tem_strNewPath = tem_strSlcItemPath + _T("\\合并图");
			HTREEITEM    tem_hSubItem   = NULL;      
			HTREEITEM    tem_hChildItem = m_conTreeCtrl.GetChildItem(tem_hSlcItem);

			if (tem_hChildItem == NULL)
			{
				//该节点下无子节点，添加“合并图”节点
				tem_hSubItem = m_conTreeCtrl.InsertItem(_T("合并图"), 4, 4, tem_hSlcItem, TVI_LAST);
				m_conTreeCtrl.SetItemData(tem_hSubItem, m_nNodeID);
				Self_AddNodeInfo(m_nNodeID, 4, _T("合并图"), tem_strNewPath);
				tem_strSlcItemPath = tem_strNewPath;
				m_conTreeCtrl.SelectItem(tem_hSubItem);
			}
			else
			{
				//该节点下有子节点，判断是否有"合并图"节点
				while(tem_hChildItem!=NULL)
				{
					CString      tem_strItemName = Self_GetItemInfo(tem_hChildItem, 0);	
					if (tem_strItemName == _T("合并图"))
					{
						//子节点中已有合并图节点
						tem_BFindIt = TRUE;
						tem_strSlcItemPath = Self_GetItemInfo(tem_hChildItem, 1);   //获取该节点路径
						m_conTreeCtrl.SelectItem(tem_hChildItem);
						break;
					}
					tem_hChildItem = m_conTreeCtrl.GetNextSiblingItem(tem_hChildItem);
				}

				if (!tem_BFindIt)
				{
					//该节点下无子节点，添加“合并图”节点
					tem_hSubItem = m_conTreeCtrl.InsertItem(_T("合并图"), 4, 4, tem_hSlcItem, TVI_LAST);
					m_conTreeCtrl.SetItemData(tem_hSubItem, m_nNodeID);
					Self_AddNodeInfo(m_nNodeID, 4, _T("合并图"), tem_strNewPath);	
					tem_strSlcItemPath = tem_strNewPath;
					m_conTreeCtrl.SelectItem(tem_hSubItem);
				}
				
			}

			g_BMerged = FALSE;

			


// 			//获取当前目录， 获取当前显示页，重新加载
// 			m_nOriImg      = 0;
// 			int tem_nFileNumber = Self_FindAllFiles(tem_strSlcItemPath);
// 			m_nFileNumber       = tem_nFileNumber;
// 			if (tem_nFileNumber == 0)
// 			{
// 				//该目录下没有图像文件,清空显示
// 				Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, m_nOriImg, 0);
// 			}
// 			else if (tem_nFileNumber >= m_nPageNumer)
// 			{
// 				Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, m_nOriImg, m_nPageNumer);
// 			}
// 			else
// 			{
// 				Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, m_nOriImg, tem_nFileNumber);
// 			}
			



			((CButton*)GetDlgItem(IDC_CHK_ALLSLCT))->SetCheck(BST_UNCHECKED);  

			//将搜索框置为NULL
			GetDlgItem(IDC_EDIT_SEARCH)->SetWindowText(_T(""));
			CRect    tem_rcEdit;

			GetDlgItem(IDC_EDIT_SEARCH)->GetWindowRect(&tem_rcEdit); 
			ScreenToClient(&tem_rcEdit);        
			InvalidateRect(&tem_rcEdit);
			UpdateWindow();			
		}
	}
	//点击菜单后取消Checkbox的全选状态
	m_BSlcAll = FALSE;
	Self_InverHighLightItems();
	((CButton*)GetDlgItem(IDC_CHK_ALLSLCT))->SetCheck(FALSE);
	m_vcCtrlIndex.clear();
	GetDlgItem(IDC_STA_CHKNBR)->SetWindowText(_T("已选：0幅"));
	CRect    tem_rcSTA;
	GetDlgItem(IDC_STA_CHKNBR)->GetWindowRect(&tem_rcSTA); 
	ScreenToClient(&tem_rcSTA);        
	InvalidateRect(&tem_rcSTA);
	UpdateWindow();
	
}


void CUDSPlatformV17Dlg::On32786()
{
	// TODO: 在此添加命令处理程序代码
	if(m_vcFileName.size()>0)
	{
		int tem_nRC = m_dlgResize.DoModal();
	}
	else
	{
		//没有图像
	}
	

}


//##################################################################################################################

/**********************************
*         函数目录
*说明：其他细节
*
* 1、返回接收消息
* 2、右键菜单
* 3、修改文件名
* 4、SliderControl实时响应
* 5、SliderControl释放响应
* 6、修复显示幅面后修正当前页
* 7、计算thumbwidth和thumbheight
* 8、item被选中触发，更新底栏显示选中数量
* 9、双击标题，权利声明
*
***********************************/

afx_msg LRESULT CUDSPlatformV17Dlg::OnSettext(WPARAM wParam, LPARAM lParam)
{
	ShowWindow(SW_SHOWMAXIMIZED);
	CString   tem_strCurDir;
	::GetPrivateProfileString(_T("Resource"), _T("curdirectory"), _T("没有找到curdirectory信息"), tem_strCurDir.GetBuffer(MAX_PATH), MAX_PATH, m_strIniPath);
	tem_strCurDir.ReleaseBuffer();
	
	m_nOriImg      = 0;
	int tem_nFileNumber = Self_FindAllFiles(tem_strCurDir);
	m_nFileNumber       = tem_nFileNumber;
	if (tem_nFileNumber == 0)
	{
		//该目录下没有图像文件,清空显示
		Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, m_nOriImg, 0);
	}
	else if (tem_nFileNumber >= m_nPageNumer)
	{
		Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, m_nOriImg, m_nPageNumer);
	}
	else
	{
		Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, m_nOriImg, tem_nFileNumber);
	}
	GetDlgItem(IDC_EDIT_SEARCH)->SetWindowText(_T(""));
	

	return 0;
}


void CUDSPlatformV17Dlg::OnRclickListImg(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int       tem_nItemIndex = 0;
	int       tem_nSlctIndex = 0;
	CString   tem_strRC      = _T("");
	tem_nItemIndex = pNMItemActivate->iItem;
	if (tem_nItemIndex>=0 && tem_nItemIndex<m_conListCtrl.GetItemCount())
	{
		//取消全选状态
		m_BSlcAll = FALSE;
		((CButton*)GetDlgItem(IDC_CHK_ALLSLCT))->SetCheck(BST_UNCHECKED);  

		tem_nSlctIndex = tem_nItemIndex + (m_nPageIndex-1)*m_nPageNumer;
		CString  tem_strFilePath = m_vcBussPath[tem_nSlctIndex];
		CShellContextMenu* tem_iContextMenu;
		tem_iContextMenu = new CShellContextMenu();
		tem_iContextMenu->SetObjects(tem_strFilePath);

		CPoint   tem_ptClick(pNMItemActivate->ptAction);
		m_conListCtrl.ClientToScreen(&tem_ptClick);

		tem_strRC = tem_iContextMenu->ShowContextMenu(this, tem_ptClick);
		if (tem_strRC.Find(_T("delete"))!=-1 || tem_strRC.Find(_T("删除"))!=-1)
		{
			m_vcFileName.erase(m_vcFileName.begin()+tem_nSlctIndex);
			m_vcFilePath.erase(m_vcFilePath.begin()+tem_nSlctIndex);
			m_vcBussPath.erase(m_vcBussPath.begin()+tem_nSlctIndex);

			m_nFileNumber = m_vcFileName.size();
			if (m_nFileNumber>0)
			{
				//依然有图，更新文件数量
				int tem_nBegin = m_nOriImg;
				int tem_nEnd   = tem_nBegin + m_nPageNumer;
				if (tem_nBegin<m_nFileNumber && tem_nEnd<=m_nFileNumber)
				{
					Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, tem_nBegin, m_nPageNumer);
				}
				else if (tem_nBegin<=m_nFileNumber && tem_nEnd>m_nFileNumber)
				{
					int tem_nResudue = m_nFileNumber - tem_nBegin;
					Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, tem_nBegin, tem_nResudue);
				}
			}
			else
			{
				Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, 0, 0);
			}
			Self_UpdaPagination(m_nFileNumber, FALSE);
		}
	}
	*pResult = 0;
}


void CUDSPlatformV17Dlg::OnEndlabeleditListImg(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int       tem_nSlcImage =-1;
	tem_nSlcImage = pDispInfo->item.iItem;
	CString   tem_strSlcName      = m_vcFileName[tem_nSlcImage+(m_nPageIndex-1)*m_nPageNumer];        //原文件名
	CString   tem_strSlcBussName     = m_vcBussPath[tem_nSlcImage+(m_nPageIndex-1)*m_nPageNumer];     //原文件路径

	CString   tem_strNewName  = pDispInfo->item.pszText;                //新文件名
	if (tem_strNewName == _T(""))
	{
		tem_strNewName = tem_strSlcName;
	}
	CString   tem_strNewWhole = _T("");
	CString   tem_strNewBuss  = _T("");

	CString   tem_strFormat   = PathFindExtension(tem_strSlcBussName);    //原文件后缀
	tem_strFormat.MakeLower();

	CString   tem_strMiddle    = tem_strSlcBussName;  
	LPWSTR    tem_wstrBussName = tem_strMiddle.GetBuffer();
	tem_strMiddle.ReleaseBuffer();
	PathRemoveFileSpecW(tem_wstrBussName);   
	CString   tem_strDocumnet  = tem_wstrBussName;                        //原文件目录

	if (tem_strFormat == _T(".jpg")||tem_strFormat == _T(".bmp")||tem_strFormat == _T(".png")||tem_strFormat == _T(".tif"))
	{
		tem_strNewWhole = tem_strDocumnet + _T("\\") + tem_strNewName;
		tem_strNewBuss  = tem_strNewWhole;

		int len =WideCharToMultiByte(CP_ACP,0,tem_strSlcBussName,-1,NULL,0,NULL,NULL);  
		char *ptxtSrc =new char[len +1];  
		WideCharToMultiByte(CP_ACP,0,tem_strSlcBussName,-1,ptxtSrc,len,NULL,NULL );  

		len =WideCharToMultiByte(CP_ACP,0,tem_strNewBuss,-1,NULL,0,NULL,NULL);  
		char *ptxtDst =new char[len +1];  
		WideCharToMultiByte(CP_ACP,0,tem_strNewBuss,-1,ptxtDst,len,NULL,NULL );  

		int  tem_rcRename = rename(ptxtSrc, ptxtDst);
		if (tem_rcRename == 0)
		{
			m_vcFileName[tem_nSlcImage+(m_nPageIndex-1)*m_nPageNumer] = tem_strNewName;
			m_vcFilePath[tem_nSlcImage+(m_nPageIndex-1)*m_nPageNumer] = tem_strNewWhole;
			m_vcBussPath[tem_nSlcImage+(m_nPageIndex-1)*m_nPageNumer] = tem_strNewBuss;					

			m_nFileNumber = m_vcFileName.size();
			if (m_nFileNumber>0)
			{
				//依然有图，更新文件数量
				int tem_nBegin = m_nOriImg;
				int tem_nEnd   = tem_nBegin + m_nPageNumer;
				if (tem_nBegin<m_nFileNumber && tem_nEnd<=m_nFileNumber)
				{
					Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, tem_nBegin, m_nPageNumer);
				}
				else if (tem_nBegin<=m_nFileNumber && tem_nEnd>m_nFileNumber)
				{
					int tem_nResudue = m_nFileNumber - tem_nBegin;
					Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, tem_nBegin, tem_nResudue);
				}
			}
		} 
		else
		{
			//			MessageBox(_T("文件名已存在！"));
		}
		delete[] ptxtSrc;  
		delete[] ptxtDst;  
	} 
	else if (tem_strFormat == _T(".pdf")||tem_strFormat == _T(".txt")||tem_strFormat == _T(".dcm"))
	{
		tem_strNewBuss = tem_strDocumnet + _T("\\") + tem_strNewName;

		int len =WideCharToMultiByte(CP_ACP,0,tem_strSlcBussName,-1,NULL,0,NULL,NULL);  
		char *ptxtSrc =new char[len +1];  
		WideCharToMultiByte(CP_ACP,0,tem_strSlcBussName,-1,ptxtSrc,len,NULL,NULL );  

		len =WideCharToMultiByte(CP_ACP,0,tem_strNewBuss,-1,NULL,0,NULL,NULL);  
		char *ptxtDst =new char[len +1];  
		WideCharToMultiByte(CP_ACP,0,tem_strNewBuss,-1,ptxtDst,len,NULL,NULL );  

		int  tem_rcRename = rename(ptxtSrc, ptxtDst);
		if (tem_rcRename == 0)
		{
			m_vcFileName[tem_nSlcImage+(m_nPageIndex-1)*m_nPageNumer] = tem_strNewName;
			m_vcBussPath[tem_nSlcImage+(m_nPageIndex-1)*m_nPageNumer] = tem_strNewBuss;

			m_nFileNumber = m_vcFileName.size();
			if (m_nFileNumber>0)
			{
				//依然有图，更新文件数量
				int tem_nBegin = m_nOriImg;
				int tem_nEnd   = tem_nBegin + m_nPageNumer;
				if (tem_nBegin<m_nFileNumber && tem_nEnd<=m_nFileNumber)
				{
					Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, tem_nBegin, m_nPageNumer);
				}
				else if (tem_nBegin<=m_nFileNumber && tem_nEnd>m_nFileNumber)
				{
					int tem_nResudue = m_nFileNumber - tem_nBegin;
					Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, tem_nBegin, tem_nResudue);
				}
			}

		}
		else
		{
			//			MessageBox(_T("文件名已存在！"));
		}	
		delete[] ptxtSrc;  
		delete[] ptxtDst; 
	} 

	*pResult = 0;
}


void CUDSPlatformV17Dlg::OnCustomdrawSlidPagenum(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//SliderBar实时响应
	//获取首个图像索引
	int   tem_nFrist  = (m_nPageIndex-1)*m_nPageNumer;    //当前首页索引
	int   tem_nCurPos = m_conSlidPage.GetPos();           //当前slider值
	int   tem_nCurPos2= 0;                                //对应幅面值
	switch (tem_nCurPos)
	{
	case 1:
		tem_nCurPos2 = 9;
		break;
	case 2:
		tem_nCurPos2 = 6;
		break;
	case 3:
		tem_nCurPos2 = 4;
		break;
	case 4:
		tem_nCurPos2 = 2;
		break;
	case 5:
		tem_nCurPos2 = 1;
		break;
	default:
		tem_nCurPos2 = 9;
		break;
	}
	//判断每页图像数量是否修改
	if (m_nPageNumer == tem_nCurPos2)
	{
		return;
	}
	else
	{
		m_nPageNumer = tem_nCurPos2;
	}
	//修改thumb尺寸
	Self_CalculatThumb(m_nPageNumer);

	//重绘
	m_nFileNumber = m_vcFileName.size();
	if (m_nFileNumber>0)
	{
		m_nOriImg      = Self_UpdataPageCur(m_nPageNumer, tem_nFrist);
		int tem_nBegin = m_nOriImg;
		int tem_nEnd   = tem_nBegin + m_nPageNumer;
		if (tem_nBegin<m_nFileNumber && tem_nEnd<=m_nFileNumber)
		{
			Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, tem_nBegin, m_nPageNumer);
		}
		else if (tem_nBegin<=m_nFileNumber && tem_nEnd>m_nFileNumber)
		{
			int tem_nResudue = m_nFileNumber - tem_nBegin;
			Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, tem_nBegin, tem_nResudue);
		}
	}
	else
	{
		Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, 0, 0);
	}
	m_BSlcAll = FALSE;
	((CButton*)GetDlgItem(IDC_CHK_ALLSLCT))->SetCheck(BST_UNCHECKED);  
	Self_UpdaPagination(m_nFileNumber, FALSE);

	*pResult = 0;
}


void CUDSPlatformV17Dlg::OnReleasedcaptureSlidPagenum(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	/*
	//SliderBar释放左键响应
	//获取首个图像索引
	int   tem_nFrist  = (m_nPageIndex-1)*m_nPageNumer;    //当前首页索引
	int   tem_nCurPos = m_conSlidPage.GetPos();           //当前slider值
	int   tem_nCurPos2= 0;                                //对应幅面值
	switch (tem_nCurPos)
	{
	case 1:
		tem_nCurPos2 = 9;
		break;
	case 2:
		tem_nCurPos2 = 6;
		break;
	case 3:
		tem_nCurPos2 = 4;
		break;
	case 4:
		tem_nCurPos2 = 2;
		break;
	case 5:
		tem_nCurPos2 = 1;
		break;
	default:
		tem_nCurPos2 = 9;
		break;
	}
	//判断每页图像数量是否修改
	if (m_nPageNumer == tem_nCurPos2)
	{
		return;
	}
	else
	{
		m_nPageNumer = tem_nCurPos2;
	}
	//修改thumb尺寸
	Self_CalculatThumb(m_nPageNumer);

	//重绘
	m_nFileNumber = m_vcFileName.size();
	if (m_nFileNumber>0)
	{
		m_nOriImg      = Self_UpdataPageCur(m_nPageNumer, tem_nFrist);
		int tem_nBegin = m_nOriImg;
		int tem_nEnd   = tem_nBegin + m_nPageNumer;
		if (tem_nBegin<m_nFileNumber && tem_nEnd<=m_nFileNumber)
		{
			Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, tem_nBegin, m_nPageNumer);
		}
		else if (tem_nBegin<=m_nFileNumber && tem_nEnd>m_nFileNumber)
		{
			int tem_nResudue = m_nFileNumber - tem_nBegin;
			Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, tem_nBegin, tem_nResudue);
		}
	}
	else
	{
		Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, 0, 0);
	}
	m_BSlcAll = FALSE;
	((CButton*)GetDlgItem(IDC_CHK_ALLSLCT))->SetCheck(BST_UNCHECKED);  
	Self_UpdaPagination(m_nFileNumber, FALSE);
	*/

	*pResult = 0;
}


int CUDSPlatformV17Dlg::Self_UpdataPageCur(int pagenum, int firstindex)
{
	//pagenum当前每页显示多少幅面， firstindex首个图像对应的总的索引值
	int  tem_nPageNum    = pagenum;
	int  tem_nFirstIndex = firstindex;
	int  tem_nBeginPage  = 0;
	while(tem_nFirstIndex%tem_nPageNum!=0)
	{
		tem_nFirstIndex--;
	}
	if (tem_nFirstIndex == 0)
	{
		tem_nBeginPage =0;
	}
	else
	{
		tem_nBeginPage = tem_nFirstIndex;
	}
	m_nOriImg = tem_nBeginPage;

	//更新下侧页码
	m_nPageIndex = tem_nBeginPage/tem_nPageNum+1;
	m_editValue  = m_nPageIndex;
	UpdateData(FALSE);

	return tem_nBeginPage;
}


void CUDSPlatformV17Dlg::Self_CalculatThumb(int pagenum)
{
	CRect   tem_rcPic;
	GetDlgItem(IDC_LIST_IMG)->GetWindowRect(&tem_rcPic);
	ScreenToClient(&tem_rcPic);
	int     tem_nWidth      = tem_rcPic.right - tem_rcPic.left;
	int     tem_nHeight     = tem_rcPic.bottom- tem_rcPic.top;
	int     tem_nThumbWidth = 0;
	int     tem_nThumbHeigth= 0;
	switch(pagenum)
	{
	case 1:
		tem_nThumbWidth = tem_nWidth-50;
		tem_nThumbHeigth= tem_nHeight-50;
		break;
	case 2:
		tem_nThumbWidth = tem_nWidth/2-50;
		tem_nThumbHeigth=tem_nHeight-50;
		break;
	case 4:
		tem_nThumbWidth = tem_nWidth/2-50;
		tem_nThumbHeigth=tem_nHeight/2-50;
		break;
	case 6:
		tem_nThumbWidth = tem_nWidth/3-50;
		tem_nThumbHeigth=tem_nHeight/2-50;
		break;
	case 9:
		tem_nThumbWidth = tem_nWidth/3-50;
		tem_nThumbHeigth=tem_nHeight/3-50;
		break;
	default:
		break;
	}
	m_nThumbWidth  = tem_nThumbWidth;
	m_nThumbHeight = tem_nThumbHeigth;

}


void CUDSPlatformV17Dlg::OnItemchangedListImg(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int      tem_nSlcCount   = m_conListCtrl.GetSelectedCount();
	CString  tem_strSlcCount = _T("");
	tem_strSlcCount.Format(_T("%d"), tem_nSlcCount);
	CString  tem_strSlcInfo  = _T("已选：");
	tem_strSlcInfo += tem_strSlcCount;
	tem_strSlcInfo += _T("项");

	GetDlgItem(IDC_STA_CHKNBR)->SetWindowText(tem_strSlcInfo);

	CRect    tem_rcSTA;
	GetDlgItem(IDC_STA_CHKNBR)->GetWindowRect(&tem_rcSTA); 
	ScreenToClient(&tem_rcSTA);        
	InvalidateRect(&tem_rcSTA);
	UpdateWindow();

	*pResult = 0;
}


void CUDSPlatformV17Dlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_vcCtrlIndex.clear();
	m_vcFileName.clear();
	m_vcFilePath.clear();
	m_vcBussPath.clear();
	m_vcDocName.clear();
	m_vcDocPath.clear();
	g_vcCtrlName.clear();
	g_vcCtrlPath.clear();

	CDialogEx::OnClose();
}


void CUDSPlatformV17Dlg::OnDblclkStaBt()
{
	// TODO: 在此添加控件通知处理程序代码
	MessageBox(_T("版本v1.7   (C)北京紫光图文系统有限公司保留所有权利"), _T("权利声明"), MB_OK);

}


void CUDSPlatformV17Dlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	/*与OnSize中采用固定的物理尺寸显示存在冲突
	//1)计算中缝区域---------------------------------------------
	CRect   tem_rcTreeCtrl, tem_rcListCtrl, tem_rcDetectionArea;
	m_conTreeCtrl.GetWindowRect(&tem_rcTreeCtrl);
	m_conListCtrl.GetWindowRect(&tem_rcListCtrl);
	ScreenToClient(&tem_rcTreeCtrl);
	ScreenToClient(&tem_rcListCtrl);
	tem_rcDetectionArea.left  = tem_rcTreeCtrl.right;
	tem_rcDetectionArea.top   = tem_rcListCtrl.top;
	tem_rcDetectionArea.right = tem_rcListCtrl.left;
	tem_rcDetectionArea.bottom= tem_rcListCtrl.bottom;

//	ScreenToClient(&point);    //如果将point-ScreenToClient,上下边界会产生一定的误差

	//2)修改鼠标状态----------------------------------------------
	if (tem_rcDetectionArea.PtInRect(point))
	{
		SetCursor(LoadCursor(NULL, IDC_SIZEWE));
		if (m_BLButtonD)
		{

		}
	}
	*/


	CDialogEx::OnMouseMove(nFlags, point);
}


void CUDSPlatformV17Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_BLButtonD = TRUE;


	CDialogEx::OnLButtonDown(nFlags, point);
}


void CUDSPlatformV17Dlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_BLButtonD = FALSE;


	CDialogEx::OnLButtonUp(nFlags, point);
}


void CUDSPlatformV17Dlg::Self_FileSort(int mode)
{
	if (m_vcFilePath.size()==0)
	{
		return;
	}
	if (mode==1)
	{
		//创建时间倒序,新创建的在前面
		for (int i=0; i<m_vcBussPath.size()-1; i++)
		{
			for (int j=0; j<m_vcBussPath.size()-i-1; j++)
			{
				FILETIME  tem_tmSrcTime = m_vcCreateTime[j];
				FILETIME  tem_tmDstTime = m_vcCreateTime[j+1];
				if (-1 == CompareFileTime(&tem_tmSrcTime, &tem_tmDstTime))   //Src<Dst
				{
					//交换CReatTime信息
					FILETIME  tem_tmFileTime = m_vcCreateTime[j];
					m_vcCreateTime[j] = m_vcCreateTime[j+1];
					m_vcCreateTime[j+1] = tem_tmFileTime;
					//交换FileSize信息
					ULONGLONG   tem_ulValue = m_vcFileSize[j];
					m_vcFileSize[j] = m_vcFileSize[j+1];
					m_vcFileSize[j+1] = tem_ulValue;

					//交换FileName信息
					CString tem_strInfo = m_vcFileName[j];
					m_vcFileName[j] = m_vcFileName[j+1];
					m_vcFileName[j+1] = tem_strInfo;

					//交换FilePath信息
					tem_strInfo = m_vcFilePath[j];
					m_vcFilePath[j] = m_vcFilePath[j+1];
					m_vcFilePath[j+1] = tem_strInfo;

					//交换BussPath信息
					tem_strInfo = m_vcBussPath[j];
					m_vcBussPath[j] = m_vcBussPath[j+1];
					m_vcBussPath[j+1] = tem_strInfo;
				}
			}
		}

	}
	else if(mode==2)
	{
		//创建时间正序，新创建的后面
		for (int i=0; i<m_vcBussPath.size()-1; i++)
		{
			for (int j=0; j<m_vcBussPath.size()-i-1; j++)
			{
				FILETIME  tem_tmSrcTime = m_vcCreateTime[j];
				FILETIME  tem_tmDstTime = m_vcCreateTime[j+1];
				if (1 == CompareFileTime(&tem_tmSrcTime, &tem_tmDstTime))   //Src<Dst
				{
					//交换CReatTime信息
					FILETIME  tem_tmFileTime = m_vcCreateTime[j];
					m_vcCreateTime[j] = m_vcCreateTime[j+1];
					m_vcCreateTime[j+1] = tem_tmFileTime;
					//交换FileSize信息
					ULONGLONG   tem_ulValue = m_vcFileSize[j];
					m_vcFileSize[j] = m_vcFileSize[j+1];
					m_vcFileSize[j+1] = tem_ulValue;

					//交换FileName信息
					CString tem_strInfo = m_vcFileName[j];
					m_vcFileName[j] = m_vcFileName[j+1];
					m_vcFileName[j+1] = tem_strInfo;

					//交换FilePath信息
					tem_strInfo = m_vcFilePath[j];
					m_vcFilePath[j] = m_vcFilePath[j+1];
					m_vcFilePath[j+1] = tem_strInfo;

					//交换BussPath信息
					tem_strInfo = m_vcBussPath[j];
					m_vcBussPath[j] = m_vcBussPath[j+1];
					m_vcBussPath[j+1] = tem_strInfo;
				}
			}
		}
	}
	else if (mode==3)
	{
		//文件大小正序，从小到大
		for (int i=0; i<m_vcBussPath.size()-1; i++)
		{
			for (int j=0; j<m_vcBussPath.size()-i-1; j++)
			{
				if (m_vcFileSize[j]>m_vcFileSize[j+1])
				{
					//交换FileSize信息
					ULONGLONG   tem_ulValue = m_vcFileSize[j];
					m_vcFileSize[j] = m_vcFileSize[j+1];
					m_vcFileSize[j+1] = tem_ulValue;

					//交换FileName信息
					CString tem_strInfo = m_vcFileName[j];
					m_vcFileName[j] = m_vcFileName[j+1];
					m_vcFileName[j+1] = tem_strInfo;

					//交换FilePath信息
					tem_strInfo = m_vcFilePath[j];
					m_vcFilePath[j] = m_vcFilePath[j+1];
					m_vcFilePath[j+1] = tem_strInfo;

					//交换BussPath信息
					tem_strInfo = m_vcBussPath[j];
					m_vcBussPath[j] = m_vcBussPath[j+1];
					m_vcBussPath[j+1] = tem_strInfo;

					//交换CReatTime信息
					FILETIME  tem_tmFileTime = m_vcCreateTime[j];
					m_vcCreateTime[j] = m_vcCreateTime[j+1];
					m_vcCreateTime[j+1] = tem_tmFileTime;
				}
			}
		}
	}
	else if(mode==4)
	{
		//文件大小倒序，从大到小
		for (int i=0; i<m_vcBussPath.size()-1; i++)
		{
			for (int j=0; j<m_vcBussPath.size()-i-1; j++)
			{
				if (m_vcFileSize[j]<m_vcFileSize[j+1])
				{
					//交换FileSize信息
					ULONGLONG   tem_ulValue = m_vcFileSize[j];
					m_vcFileSize[j] = m_vcFileSize[j+1];
					m_vcFileSize[j+1] = tem_ulValue;

					//交换FileName信息
					CString tem_strInfo = m_vcFileName[j];
					m_vcFileName[j] = m_vcFileName[j+1];
					m_vcFileName[j+1] = tem_strInfo;

					//交换FilePath信息
					tem_strInfo = m_vcFilePath[j];
					m_vcFilePath[j] = m_vcFilePath[j+1];
					m_vcFilePath[j+1] = tem_strInfo;

					//交换BussPath信息
					tem_strInfo = m_vcBussPath[j];
					m_vcBussPath[j] = m_vcBussPath[j+1];
					m_vcBussPath[j+1] = tem_strInfo;

					//交换CReatTime信息
					FILETIME  tem_tmFileTime = m_vcCreateTime[j];
					m_vcCreateTime[j] = m_vcCreateTime[j+1];
					m_vcCreateTime[j+1] = tem_tmFileTime;
				}
			}
		}
	}
	else
	{
		//默认排序，及Windows名称排序
	}

//	MessageBox(_T(""));
	/*
	FILETIME tem_timeIt;
	temFinder.GetCreationTime(&tem_timeIt);
	SYSTEMTIME tem_it;
	::FileTimeToSystemTime(&tem_timeIt, &tem_it);   //获取的是格林威治时间，比北京时间慢8个小时
	tem_it.wHour += 8;
	if (tem_it.wHour>23)
	{
		tem_it.wHour -= 24;
	}
	*/
}


//文件排序右键菜单
void CUDSPlatformV17Dlg::On32781()
{
	// TODO: 在此添加命令处理程序代码
	m_nSortMode = 0;

	//重新加载当前目录
	//查询该节点是否有子节点
	HTREEITEM    tem_hSlcItem   = m_conTreeCtrl.GetSelectedItem();
	HTREEITEM    tem_hChildItem = m_conTreeCtrl.GetChildItem(tem_hSlcItem);
	//1、获取该节点路径
	CString      tem_strSlcItemPath = Self_GetItemInfo(tem_hSlcItem, 1);	
	if (tem_hChildItem == NULL)
	{
		/*该节点无子节点，查询子文件夹，添加节点*/

		//2、查找该路径下文件夹
		int      tem_nDocNumber     = Self_FindDocs(tem_strSlcItemPath);
		if (tem_nDocNumber>0)
		{
			//将子文件夹添加为节点
			std::vector<CString>::iterator item_name;
			std::vector<CString>::iterator item_path;
			item_name = m_vcDocName.begin();
			for (item_path=m_vcDocPath.begin(); item_path!=m_vcDocPath.end(); item_path++, item_name++)
			{
				HTREEITEM     tem_hSubRoot = m_conTreeCtrl.InsertItem(*item_name, 4, 4, tem_hSlcItem, TVI_LAST);
				m_conTreeCtrl.SetItemData(tem_hSubRoot, m_nNodeID);
				Self_AddNodeInfo(m_nNodeID, 4, *item_name, *item_path);
			}
		}
	}
	//3、查找该路径下文件
	m_nOriImg      = 0;
	int tem_nFileNumber = Self_FindAllFiles(tem_strSlcItemPath);
	m_nFileNumber       = tem_nFileNumber;
	if (tem_nFileNumber == 0)
	{
		for (int i=0; i<m_ImageList.GetImageCount(); i++)
		{
			m_ImageList.Remove(i);
		}
		m_ImageList.DeleteImageList();
		m_conListCtrl.DeleteAllItems();
		//该目录下没有图像文件,清空显示
		//		Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, m_nOriImg, 0);
	}
	else if (tem_nFileNumber >= m_nPageNumer)
	{
		Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, m_nOriImg, m_nPageNumer);
	}
	else
	{
		Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, m_nOriImg, tem_nFileNumber);
	}

	m_BSlcAll = FALSE;
	((CButton*)GetDlgItem(IDC_CHK_ALLSLCT))->SetCheck(BST_UNCHECKED);  


	//4、将搜索框置为NULL
	GetDlgItem(IDC_EDIT_SEARCH)->SetWindowText(_T(""));
	CRect    tem_rcEdit;

	GetDlgItem(IDC_EDIT_SEARCH)->GetWindowRect(&tem_rcEdit); 
	ScreenToClient(&tem_rcEdit);        
	InvalidateRect(&tem_rcEdit);

	UpdateWindow();
	
	//写入配置文件
	CString  tem_strIniInfo = _T("");
	tem_strIniInfo.Format(_T("%d"), m_nSortMode);
	::WritePrivateProfileString(_T("Resource"), _T("sortmode"), tem_strIniInfo, m_strIniPath);

}


void CUDSPlatformV17Dlg::On32782()
{
	// TODO: 在此添加命令处理程序代码
	m_nSortMode = 2;

	//重新加载当前目录
	//重新加载当前目录
	//查询该节点是否有子节点
	HTREEITEM    tem_hSlcItem   = m_conTreeCtrl.GetSelectedItem();
	HTREEITEM    tem_hChildItem = m_conTreeCtrl.GetChildItem(tem_hSlcItem);
	//1、获取该节点路径
	CString      tem_strSlcItemPath = Self_GetItemInfo(tem_hSlcItem, 1);	
	if (tem_hChildItem == NULL)
	{
		/*该节点无子节点，查询子文件夹，添加节点*/

		//2、查找该路径下文件夹
		int      tem_nDocNumber     = Self_FindDocs(tem_strSlcItemPath);
		if (tem_nDocNumber>0)
		{
			//将子文件夹添加为节点
			std::vector<CString>::iterator item_name;
			std::vector<CString>::iterator item_path;
			item_name = m_vcDocName.begin();
			for (item_path=m_vcDocPath.begin(); item_path!=m_vcDocPath.end(); item_path++, item_name++)
			{
				HTREEITEM     tem_hSubRoot = m_conTreeCtrl.InsertItem(*item_name, 4, 4, tem_hSlcItem, TVI_LAST);
				m_conTreeCtrl.SetItemData(tem_hSubRoot, m_nNodeID);
				Self_AddNodeInfo(m_nNodeID, 4, *item_name, *item_path);
			}
		}
	}
	//3、查找该路径下文件
	m_nOriImg      = 0;
	int tem_nFileNumber = Self_FindAllFiles(tem_strSlcItemPath);
	m_nFileNumber       = tem_nFileNumber;
	if (tem_nFileNumber == 0)
	{
		for (int i=0; i<m_ImageList.GetImageCount(); i++)
		{
			m_ImageList.Remove(i);
		}
		m_ImageList.DeleteImageList();
		m_conListCtrl.DeleteAllItems();
		//该目录下没有图像文件,清空显示
		//		Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, m_nOriImg, 0);
	}
	else if (tem_nFileNumber >= m_nPageNumer)
	{
		Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, m_nOriImg, m_nPageNumer);
	}
	else
	{
		Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, m_nOriImg, tem_nFileNumber);
	}

	m_BSlcAll = FALSE;
	((CButton*)GetDlgItem(IDC_CHK_ALLSLCT))->SetCheck(BST_UNCHECKED);  


	//4、将搜索框置为NULL
	GetDlgItem(IDC_EDIT_SEARCH)->SetWindowText(_T(""));
	CRect    tem_rcEdit;

	GetDlgItem(IDC_EDIT_SEARCH)->GetWindowRect(&tem_rcEdit); 
	ScreenToClient(&tem_rcEdit);        
	InvalidateRect(&tem_rcEdit);

	UpdateWindow();


	//写入配置文件
	CString  tem_strIniInfo = _T("");
	tem_strIniInfo.Format(_T("%d"), m_nSortMode);
	::WritePrivateProfileString(_T("Resource"), _T("sortmode"), tem_strIniInfo, m_strIniPath);
}


void CUDSPlatformV17Dlg::On32783()
{
	// TODO: 在此添加命令处理程序代码
	m_nSortMode = 1;

	//重新加载当前目录
	//重新加载当前目录
	//查询该节点是否有子节点
	HTREEITEM    tem_hSlcItem   = m_conTreeCtrl.GetSelectedItem();
	HTREEITEM    tem_hChildItem = m_conTreeCtrl.GetChildItem(tem_hSlcItem);
	//1、获取该节点路径
	CString      tem_strSlcItemPath = Self_GetItemInfo(tem_hSlcItem, 1);	
	if (tem_hChildItem == NULL)
	{
		/*该节点无子节点，查询子文件夹，添加节点*/

		//2、查找该路径下文件夹
		int      tem_nDocNumber     = Self_FindDocs(tem_strSlcItemPath);
		if (tem_nDocNumber>0)
		{
			//将子文件夹添加为节点
			std::vector<CString>::iterator item_name;
			std::vector<CString>::iterator item_path;
			item_name = m_vcDocName.begin();
			for (item_path=m_vcDocPath.begin(); item_path!=m_vcDocPath.end(); item_path++, item_name++)
			{
				HTREEITEM     tem_hSubRoot = m_conTreeCtrl.InsertItem(*item_name, 4, 4, tem_hSlcItem, TVI_LAST);
				m_conTreeCtrl.SetItemData(tem_hSubRoot, m_nNodeID);
				Self_AddNodeInfo(m_nNodeID, 4, *item_name, *item_path);
			}
		}
	}
	//3、查找该路径下文件
	m_nOriImg      = 0;
	int tem_nFileNumber = Self_FindAllFiles(tem_strSlcItemPath);
	m_nFileNumber       = tem_nFileNumber;
	if (tem_nFileNumber == 0)
	{
		for (int i=0; i<m_ImageList.GetImageCount(); i++)
		{
			m_ImageList.Remove(i);
		}
		m_ImageList.DeleteImageList();
		m_conListCtrl.DeleteAllItems();
		//该目录下没有图像文件,清空显示
		//		Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, m_nOriImg, 0);
	}
	else if (tem_nFileNumber >= m_nPageNumer)
	{
		Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, m_nOriImg, m_nPageNumer);
	}
	else
	{
		Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, m_nOriImg, tem_nFileNumber);
	}

	m_BSlcAll = FALSE;
	((CButton*)GetDlgItem(IDC_CHK_ALLSLCT))->SetCheck(BST_UNCHECKED);  


	//4、将搜索框置为NULL
	GetDlgItem(IDC_EDIT_SEARCH)->SetWindowText(_T(""));
	CRect    tem_rcEdit;

	GetDlgItem(IDC_EDIT_SEARCH)->GetWindowRect(&tem_rcEdit); 
	ScreenToClient(&tem_rcEdit);        
	InvalidateRect(&tem_rcEdit);

	UpdateWindow();
	

	//写入配置文件
	CString  tem_strIniInfo = _T("");
	tem_strIniInfo.Format(_T("%d"), m_nSortMode);
	::WritePrivateProfileString(_T("Resource"), _T("sortmode"), tem_strIniInfo, m_strIniPath);
}


void CUDSPlatformV17Dlg::On32784()
{
	// TODO: 在此添加命令处理程序代码
	m_nSortMode = 3;

	//重新加载当前目录
	//重新加载当前目录
	//查询该节点是否有子节点
	HTREEITEM    tem_hSlcItem   = m_conTreeCtrl.GetSelectedItem();
	HTREEITEM    tem_hChildItem = m_conTreeCtrl.GetChildItem(tem_hSlcItem);
	//1、获取该节点路径
	CString      tem_strSlcItemPath = Self_GetItemInfo(tem_hSlcItem, 1);	
	if (tem_hChildItem == NULL)
	{
		/*该节点无子节点，查询子文件夹，添加节点*/

		//2、查找该路径下文件夹
		int      tem_nDocNumber     = Self_FindDocs(tem_strSlcItemPath);
		if (tem_nDocNumber>0)
		{
			//将子文件夹添加为节点
			std::vector<CString>::iterator item_name;
			std::vector<CString>::iterator item_path;
			item_name = m_vcDocName.begin();
			for (item_path=m_vcDocPath.begin(); item_path!=m_vcDocPath.end(); item_path++, item_name++)
			{
				HTREEITEM     tem_hSubRoot = m_conTreeCtrl.InsertItem(*item_name, 4, 4, tem_hSlcItem, TVI_LAST);
				m_conTreeCtrl.SetItemData(tem_hSubRoot, m_nNodeID);
				Self_AddNodeInfo(m_nNodeID, 4, *item_name, *item_path);
			}
		}
	}
	//3、查找该路径下文件
	m_nOriImg      = 0;
	int tem_nFileNumber = Self_FindAllFiles(tem_strSlcItemPath);
	m_nFileNumber       = tem_nFileNumber;
	if (tem_nFileNumber == 0)
	{
		for (int i=0; i<m_ImageList.GetImageCount(); i++)
		{
			m_ImageList.Remove(i);
		}
		m_ImageList.DeleteImageList();
		m_conListCtrl.DeleteAllItems();
		//该目录下没有图像文件,清空显示
		//		Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, m_nOriImg, 0);
	}
	else if (tem_nFileNumber >= m_nPageNumer)
	{
		Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, m_nOriImg, m_nPageNumer);
	}
	else
	{
		Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, m_nOriImg, tem_nFileNumber);
	}

	m_BSlcAll = FALSE;
	((CButton*)GetDlgItem(IDC_CHK_ALLSLCT))->SetCheck(BST_UNCHECKED);  


	//4、将搜索框置为NULL
	GetDlgItem(IDC_EDIT_SEARCH)->SetWindowText(_T(""));
	CRect    tem_rcEdit;

	GetDlgItem(IDC_EDIT_SEARCH)->GetWindowRect(&tem_rcEdit); 
	ScreenToClient(&tem_rcEdit);        
	InvalidateRect(&tem_rcEdit);

	UpdateWindow();
	

	//写入配置文件
	CString  tem_strIniInfo = _T("");
	tem_strIniInfo.Format(_T("%d"), m_nSortMode);
	::WritePrivateProfileString(_T("Resource"), _T("sortmode"), tem_strIniInfo, m_strIniPath);
}


void CUDSPlatformV17Dlg::On32785()
{
	// TODO: 在此添加命令处理程序代码
	m_nSortMode = 4;

	//重新加载当前目录
	//重新加载当前目录
	//查询该节点是否有子节点
	HTREEITEM    tem_hSlcItem   = m_conTreeCtrl.GetSelectedItem();
	HTREEITEM    tem_hChildItem = m_conTreeCtrl.GetChildItem(tem_hSlcItem);
	//1、获取该节点路径
	CString      tem_strSlcItemPath = Self_GetItemInfo(tem_hSlcItem, 1);	
	if (tem_hChildItem == NULL)
	{
		/*该节点无子节点，查询子文件夹，添加节点*/

		//2、查找该路径下文件夹
		int      tem_nDocNumber     = Self_FindDocs(tem_strSlcItemPath);
		if (tem_nDocNumber>0)
		{
			//将子文件夹添加为节点
			std::vector<CString>::iterator item_name;
			std::vector<CString>::iterator item_path;
			item_name = m_vcDocName.begin();
			for (item_path=m_vcDocPath.begin(); item_path!=m_vcDocPath.end(); item_path++, item_name++)
			{
				HTREEITEM     tem_hSubRoot = m_conTreeCtrl.InsertItem(*item_name, 4, 4, tem_hSlcItem, TVI_LAST);
				m_conTreeCtrl.SetItemData(tem_hSubRoot, m_nNodeID);
				Self_AddNodeInfo(m_nNodeID, 4, *item_name, *item_path);
			}
		}
	}
	//3、查找该路径下文件
	m_nOriImg      = 0;
	int tem_nFileNumber = Self_FindAllFiles(tem_strSlcItemPath);
	m_nFileNumber       = tem_nFileNumber;
	if (tem_nFileNumber == 0)
	{
		for (int i=0; i<m_ImageList.GetImageCount(); i++)
		{
			m_ImageList.Remove(i);
		}
		m_ImageList.DeleteImageList();
		m_conListCtrl.DeleteAllItems();
		//该目录下没有图像文件,清空显示
		//		Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, m_nOriImg, 0);
	}
	else if (tem_nFileNumber >= m_nPageNumer)
	{
		Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, m_nOriImg, m_nPageNumer);
	}
	else
	{
		Self_ThumbailList(m_nThumbWidth, m_nThumbHeight, m_nOriImg, tem_nFileNumber);
	}

	m_BSlcAll = FALSE;
	((CButton*)GetDlgItem(IDC_CHK_ALLSLCT))->SetCheck(BST_UNCHECKED);  


	//4、将搜索框置为NULL
	GetDlgItem(IDC_EDIT_SEARCH)->SetWindowText(_T(""));
	CRect    tem_rcEdit;

	GetDlgItem(IDC_EDIT_SEARCH)->GetWindowRect(&tem_rcEdit); 
	ScreenToClient(&tem_rcEdit);        
	InvalidateRect(&tem_rcEdit);

	UpdateWindow();
	

	//写入配置文件
	CString  tem_strIniInfo = _T("");
	tem_strIniInfo.Format(_T("%d"), m_nSortMode);
	::WritePrivateProfileString(_T("Resource"), _T("sortmode"), tem_strIniInfo, m_strIniPath);
}


//方法一：ListCtrl失焦后高亮，不可行
void CUDSPlatformV17Dlg::OnCustomdrawListImg(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	
	/*
	if (CDDS_PREPAINT == pNMCD->dwDrawStage)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (CDDS_ITEMPREPAINT == pNMCD->dwDrawStage)
	{
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if ((CDDS_ITEMPREPAINT|CDDS_SUBITEM) == pNMCD->dwDrawStage)
	{
		COLORREF clrNewTextColor, clrNewBkColor;

		int nItem = static_cast<int>(pNMCD->dwItemSpec);

		BOOL bSelect = FALSE;
		POSITION pos = m_conListCtrl.GetFirstSelectedItemPosition();
		while (pos)
		{
			int index = m_conListCtrl.GetNextSelectedItem(pos);
			if (index == nItem)
			{
				bSelect = TRUE;
				break;
			}
		}
		if (bSelect)
		{
			clrNewTextColor = RGB(255, 0, 0);
			clrNewBkColor   = RGB(0, 0, 255);

		}
		else
		{	
			clrNewTextColor = RGB(0,0,0);	//Leave the text black
			clrNewBkColor = RGB(255,255,255);	//leave the bkgrnd color white

		}
		NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)pNMCD;
		pLVCD->clrText   = clrNewTextColor;
		pLVCD->clrTextBk = clrNewBkColor;
		*pResult = CDRF_DODEFAULT;
	}
	*/
}


//方法二：ListCtrl失焦后高亮，可行
void CUDSPlatformV17Dlg::OnKillfocusListImg(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION   pos;
	pos = m_conListCtrl.GetFirstSelectedItemPosition();
	while (pos)
	{
		int  tem_nIndex = (int)pos-1;
		m_conListCtrl.SetItemState(tem_nIndex, LVIS_DROPHILITED, LVIS_DROPHILITED);
		m_conListCtrl.GetNextSelectedItem(pos);		
	}
//	m_conListCtrl.SetItemState(1, LVIS_DROPHILITED, LVIS_DROPHILITED);

	*pResult = 0;
}


void CUDSPlatformV17Dlg::OnSetfocusListImg(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION   pos;
	pos = m_conListCtrl.GetFirstSelectedItemPosition();
	while (pos)
	{
		int  tem_nIndex = (int)pos-1;
		m_conListCtrl.SetItemState(tem_nIndex, FALSE, LVIF_STATE);
		m_conListCtrl.GetNextSelectedItem(pos);		
	}
//	m_conListCtrl.SetItemState(1, FALSE, LVIF_STATE);

	*pResult = 0;
}