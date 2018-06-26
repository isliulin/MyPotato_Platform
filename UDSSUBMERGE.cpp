// UDSSUBMERGE.cpp : 实现文件
//

#include "stdafx.h"
#include "UDSPlatformV1.7.h"
#include "UDSSUBMERGE.h"
#include "afxdialogex.h"



extern std::vector<CString>   m_vcFileName;    //文件名
extern std::vector<CString>   m_vcFilePath;    //图像文件路径
extern std::vector<CString>   m_vcBussPath;    //pdf+txt+图像路径 
extern BOOL                   g_BMerged;       //窗口不返回MB_OK，因此使用全局变量标示

// CUDSSUBMERGE 对话框

IMPLEMENT_DYNAMIC(CUDSSUBMERGE, CDialogEx)

CUDSSUBMERGE::CUDSSUBMERGE(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUDSSUBMERGE::IDD, pParent)
{

	m_StaSlcNumer = _T("");
	m_StaMergeDid = _T("");
}

CUDSSUBMERGE::~CUDSSUBMERGE()
{
}

void CUDSSUBMERGE::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MERGE, m_conListMerge);
	DDX_Text(pDX, IDC_STA_SLCNUMBER, m_StaSlcNumer);
	DDX_Control(pDX, IDC_PROGRESS_MERGE, m_conProMerge);
	DDX_Text(pDX, IDC_STA_MERGEDID, m_StaMergeDid);
}


BEGIN_MESSAGE_MAP(CUDSSUBMERGE, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHK_LEFT, &CUDSSUBMERGE::OnClickedChkLeft)
	ON_BN_CLICKED(IDC_CHK_UP, &CUDSSUBMERGE::OnClickedChkUp)
	ON_BN_CLICKED(IDOK, &CUDSSUBMERGE::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHK_LINYE, &CUDSSUBMERGE::OnClickedChkLinye)
	ON_BN_CLICKED(IDC_CHK_GEYE, &CUDSSUBMERGE::OnClickedChkGeye)
	ON_BN_CLICKED(IDC_CHK_QIANHOU, &CUDSSUBMERGE::OnClickedChkQianhou)
	ON_BN_CLICKED(IDC_CHK_JPG, &CUDSSUBMERGE::OnClickedChkJpg)
	ON_BN_CLICKED(IDC_CHK_BMP, &CUDSSUBMERGE::OnClickedChkBmp)
	ON_BN_CLICKED(IDC_CHK_PNG, &CUDSSUBMERGE::OnClickedChkPng)
	ON_BN_CLICKED(IDC_CHK_TIF, &CUDSSUBMERGE::OnClickedChkTif)
	ON_BN_CLICKED(IDC_CHK_SAVE, &CUDSSUBMERGE::OnClickedChkSave)
	ON_BN_CLICKED(IDCANCEL, &CUDSSUBMERGE::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHK_ALLSLC, &CUDSSUBMERGE::OnClickedChkAllslc)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_MERGE, &CUDSSUBMERGE::OnItemchangedListMerge)
	ON_BN_CLICKED(IDC_BTN_HELP, &CUDSSUBMERGE::OnBnClickedBtnHelp)
END_MESSAGE_MAP()


// CUDSSUBMERGE 消息处理程序


BOOL CUDSSUBMERGE::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// TODO:  在此添加额外的初始化
	/*****************************
	*
	* 1、变量初始化
	* 1.5、筛选图像文件
	* 2、ListCtrl初始化
	* 3、CheckBox初始化
	* 4、Static初始化
	* 5、ProgressBar初始化
	*
	*****************************/
	/*1、变量初始化*/
	m_nMergeSite = 0;
	m_nMergeMode = 0;
	m_nSaveType  = 0;
	m_BSaved     = TRUE;
	m_vcMergeIndex.clear();
	m_vcImageName.clear();
	m_vcImagePath.clear();

	/*1.5、筛选图像文件*/
	for (unsigned int i=0; i<m_vcFileName.size(); i++)
	{
		CString  tem_strExtension = _T("");
		CString  tem_strFileName  = m_vcFileName[i];

		tem_strExtension = PathFindExtension(tem_strFileName);
		tem_strExtension.MakeLower();
		if (tem_strExtension == _T(".jpg") || tem_strExtension == _T(".jpeg")||
			tem_strExtension == _T(".tif") || tem_strExtension == _T(".tiff")||
			tem_strExtension == _T(".bmp") || tem_strExtension == _T(".png"))
		{
			//是图像文件
			m_vcImageName.push_back(tem_strFileName);
			m_vcImagePath.push_back(m_vcBussPath[i]);
		}
	}

	/*2、ListCtrl初始化*/
	//0)获取ListCtrl尺寸
	CRect    tem_rcListCtrl;
	GetDlgItem(IDC_LIST_MERGE)->GetClientRect(&tem_rcListCtrl);
	int      tem_nListWidth = tem_rcListCtrl.Width();
	//1)设置ListCtrl风格
	DWORD     tem_dwStyle;
	tem_dwStyle  = LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT;
	m_conListMerge.SetExtendedStyle(tem_dwStyle);
	//2)添加ListCtrl列
	m_conListMerge.InsertColumn(0, _T("选择"), LVCFMT_CENTER, 40);
	m_conListMerge.InsertColumn(1, _T("文件名"), LVCFMT_CENTER, tem_nListWidth-40);
	//3)添加ListCtrl行，即添加ListCtrl信息
	for (unsigned int i=0; i<m_vcImageName.size(); i++)
	{
		m_conListMerge.InsertItem(i, _T(""));
		m_conListMerge.SetCheck(i, TRUE);
		m_conListMerge.SetItemText(i, 1, m_vcImageName[i]);
	}

	/*3、Checkbox初始化*/
	((CButton*)GetDlgItem(IDC_CHK_LEFT))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CHK_UP))->SetCheck(FALSE);

	((CButton*)GetDlgItem(IDC_CHK_LINYE))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CHK_GEYE))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHK_QIANHOU))->SetCheck(FALSE);

	((CButton*)GetDlgItem(IDC_CHK_JPG))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CHK_BMP))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHK_PNG))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHK_TIF))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHK_SAVE))->SetCheck(TRUE);

	((CButton*)GetDlgItem(IDC_CHK_ALLSLC))->SetCheck(TRUE);

	/*4、Static初始化*/
	m_StaSlcNumer = _T("已选： ");
	int tem_nSlcNumber = m_vcImageName.size();
	CString  tem_strSlcNumber;
	tem_strSlcNumber.Format(_T("%d"), tem_nSlcNumber);
	m_StaSlcNumer += tem_strSlcNumber;
	UpdateData(FALSE);

	/*5、ProgressBar初始化*/
	m_conProMerge.SetRange(0, m_vcImageName.size());
	m_conProMerge.SetPos(0);

	/*6、进度条Static初始化*/
	m_StaMergeDid = _T("0%");
	UpdateData(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CUDSSUBMERGE::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
}


/************************************
* 说明：合并算法
*
* 1、图像合并算法
* 2、获取图像类型
*
************************************/
CString CUDSSUBMERGE::Self_MergeImages(CString img1, CString img2, int site, int mode, BOOL saved, int name, CString dir)
{
	CxImage pImageA, pImageB, pImage;

	int  tem_nImgType = Self_GetTypeFromFileName(img1);
	pImageA.Load(img1, tem_nImgType);
	pImageB.Load(img2, tem_nImgType);

	int    tem_nImgAWidth  = pImageA.GetWidth();
	int    tem_nImgAHeight = pImageA.GetHeight();
	int    tem_nImgABpp    = pImageA.GetBpp();
	int    tem_nImgBWidth  = pImageB.GetWidth();
	int    tem_nImgBHeight = pImageB.GetHeight();
	int    tem_nImgBBpp    = pImageB.GetBpp();

	int    tem_nImgBpp, tem_nImgWidth, tem_nImgHeight, tem_nImageType;
	if (tem_nImgABpp>tem_nImgBBpp)
	{
		tem_nImgBpp = tem_nImgABpp;
	}
	else
	{
		tem_nImgBpp = tem_nImgBpp;
	}
	if (tem_nImgAWidth>tem_nImgBWidth)
	{
		tem_nImgWidth = tem_nImgAWidth;
	}
	else
	{
		tem_nImgWidth = tem_nImgBWidth;
	}
	if (tem_nImgAHeight>tem_nImgBHeight)
	{
		tem_nImgHeight = tem_nImgAHeight;
	}
	else
	{
		tem_nImgHeight = tem_nImgBHeight;
	}

	if (site == 0)
	{
		//左右合并
		pImage.Create(tem_nImgAWidth+tem_nImgBWidth, tem_nImgHeight, 24);
		pImage.MixFrom(pImageA, 0, 0);
		pImage.MixFrom(pImageB, tem_nImgAWidth, 0);
	}
	else if (site == 1)
	{
		//上下合并
		pImage.Create(tem_nImgWidth, tem_nImgAHeight+tem_nImgBHeight, 24);
		pImage.MixFrom(pImageA, 0, tem_nImgBHeight);
		pImage.MixFrom(pImageB, 0, 0);
	}

	
	CString tem_strFileName;
	tem_strFileName.Format(_T("%04d"), name);

	CString tem_strFilePath = dir;
	tem_strFilePath += _T("\\合并图");
	tem_strFilePath += tem_strFileName;

	switch(mode)
	{
	case 0:
		tem_nImageType = CXIMAGE_FORMAT_JPG;
		tem_strFilePath += _T(".jpg");
		break;
	case 1:
		tem_nImageType = CXIMAGE_FORMAT_BMP;
		tem_strFilePath += _T(".bmp");
		break;
	case 2:
		tem_nImageType = CXIMAGE_FORMAT_PNG;
		tem_strFilePath += _T(".png");
		break;
	case 3:
		pImage.SetCodecOption(5, CXIMAGE_FORMAT_TIF);
		tem_nImageType = CXIMAGE_FORMAT_TIF;
		tem_strFilePath += _T(".tif");
		break;
	default:
		tem_nImageType = CXIMAGE_FORMAT_JPG;
		tem_strFilePath += _T(".jpg");
		break;
	}
	pImage.Save(tem_strFilePath, tem_nImageType);

	if(!m_BSaved)
	{
		//删除原图
		DeleteFile(img1);
		DeleteFile(img2);
	}

	return tem_strFilePath;
}


int CUDSSUBMERGE::Self_GetTypeFromFileName(LPCTSTR pstr)
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


/************************************
* 说明：CheckBox
*
* 1、左右合并
* 2、上下合并
* 3、邻页
* 4、隔页
* 5、前后
* 6、JPG
* 7、BMP
* 8、PNG
* 9、TIF
* 10、Save SrcImage
* 11、全选
* 12、选择数量更新
*
************************************/

void CUDSSUBMERGE::OnClickedChkLeft()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHK_LEFT))
	{
		m_nMergeSite = 0;
		((CButton*)GetDlgItem(IDC_CHK_UP))->SetCheck(FALSE);
	}
	else
	{
		m_nMergeSite = 1;
		((CButton*)GetDlgItem(IDC_CHK_UP))->SetCheck(TRUE);
	}
}


void CUDSSUBMERGE::OnClickedChkUp()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHK_UP))
	{
		m_nMergeSite = 1;
		((CButton*)GetDlgItem(IDC_CHK_LEFT))->SetCheck(FALSE);
	} 
	else
	{
		m_nMergeSite = 0;
		((CButton*)GetDlgItem(IDC_CHK_LEFT))->SetCheck(TRUE);
	}
}


void CUDSSUBMERGE::OnClickedChkLinye()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHK_LINYE))
	{
		m_nMergeMode = 0;
		((CButton*)GetDlgItem(IDC_CHK_GEYE))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_QIANHOU))->SetCheck(FALSE);
	} 
	else
	{
		m_nMergeMode = 1;
		((CButton*)GetDlgItem(IDC_CHK_GEYE))->SetCheck(TRUE);
	}
}


void CUDSSUBMERGE::OnClickedChkGeye()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHK_GEYE))
	{
		m_nMergeMode = 1;
		((CButton*)GetDlgItem(IDC_CHK_LINYE))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_QIANHOU))->SetCheck(FALSE);

	} 
	else
	{
		m_nMergeMode = 2;
		((CButton*)GetDlgItem(IDC_CHK_QIANHOU))->SetCheck(TRUE);
	}
}


void CUDSSUBMERGE::OnClickedChkQianhou()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHK_QIANHOU))
	{
		m_nMergeMode = 2;
		((CButton*)GetDlgItem(IDC_CHK_LINYE))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_GEYE))->SetCheck(FALSE);

	} 
	else
	{
		m_nMergeMode = 0;
		((CButton*)GetDlgItem(IDC_CHK_LINYE))->SetCheck(TRUE);
	}
}


void CUDSSUBMERGE::OnClickedChkJpg()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHK_JPG))
	{
		m_nSaveType = 0;
		((CButton*)GetDlgItem(IDC_CHK_BMP))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_PNG))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_TIF))->SetCheck(FALSE);

	} 
	else
	{
		m_nMergeMode = 1;
		((CButton*)GetDlgItem(IDC_CHK_BMP))->SetCheck(TRUE);
	}
}


void CUDSSUBMERGE::OnClickedChkBmp()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHK_BMP))
	{
		m_nSaveType = 1;
		((CButton*)GetDlgItem(IDC_CHK_JPG))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_PNG))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_TIF))->SetCheck(FALSE);

	} 
	else
	{
		m_nMergeMode = 2;
		((CButton*)GetDlgItem(IDC_CHK_PNG))->SetCheck(TRUE);
	}
}


void CUDSSUBMERGE::OnClickedChkPng()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHK_PNG))
	{
		m_nSaveType = 2;
		((CButton*)GetDlgItem(IDC_CHK_JPG))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_BMP))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_TIF))->SetCheck(FALSE);

	} 
	else
	{
		m_nMergeMode = 3;
		((CButton*)GetDlgItem(IDC_CHK_TIF))->SetCheck(TRUE);
	}
}


void CUDSSUBMERGE::OnClickedChkTif()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHK_TIF))
	{
		m_nSaveType = 3;
		((CButton*)GetDlgItem(IDC_CHK_JPG))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_BMP))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHK_PNG))->SetCheck(FALSE);

	} 
	else
	{
		m_nMergeMode = 0;
		((CButton*)GetDlgItem(IDC_CHK_JPG))->SetCheck(TRUE);
	}
}


void CUDSSUBMERGE::OnClickedChkSave()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHK_SAVE))
	{
		m_BSaved = TRUE;

	} 
	else
	{
		m_BSaved = FALSE;
	}
}


void CUDSSUBMERGE::OnClickedChkAllslc()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHK_ALLSLC))
	{
		//全选
		for (unsigned int i=0; i<m_vcImageName.size(); i++)
		{
			m_conListMerge.SetCheck(i, TRUE);
		}
	} 
	else
	{
		//全不选
		for (unsigned int i=0; i<m_vcImageName.size(); i++)
		{
			m_conListMerge.SetCheck(i, FALSE);
		}
	}
}


void CUDSSUBMERGE::OnItemchangedListMerge(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_StaSlcNumer = _T("已选： ");
	int tem_nSlcNumber = 0;
	for (unsigned int i=0; i<m_vcImageName.size(); i++)
	{
		if (m_conListMerge.GetItemState(i, LVIS_SELECTED)==LVIS_SELECTED||m_conListMerge.GetCheck(i))
		{
			tem_nSlcNumber++;
		}
	}
	if (tem_nSlcNumber == m_vcImageName.size())
	{
		//自动勾选全选框
		((CButton*)GetDlgItem(IDC_CHK_ALLSLC))->SetCheck(TRUE);
	}
	else
	{
		//取消勾选全选框
		((CButton*)GetDlgItem(IDC_CHK_ALLSLC))->SetCheck(FALSE);
	}
	CString  tem_strSlcNumber;
	tem_strSlcNumber.Format(_T("%d"), tem_nSlcNumber);
	m_StaSlcNumer += tem_strSlcNumber;
	UpdateData(FALSE);

	m_conProMerge.SetRange(0, tem_nSlcNumber);
	m_conProMerge.SetPos(0);
	m_StaMergeDid = _T("0%");
	UpdateData(FALSE);
	*pResult = 0;
}


/************************************
* 说明：功能按钮
*
* 1、导出
* 2、取消
* 3、帮助
*
************************************/
void CUDSSUBMERGE::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//1)创建保存目录
	//a、获取当前目录
	CString  tem_strCurDir = m_vcBussPath[0];
	TCHAR*   tem_cCurDir;
	tem_cCurDir = tem_strCurDir.GetBuffer();
	tem_strCurDir.ReleaseBuffer();
	PathRemoveFileSpec(tem_cCurDir);
	tem_strCurDir  = tem_cCurDir;
	tem_strCurDir += _T("\\");
	tem_strCurDir += _T("合并图");
	
	//b、检测新目录是否存在
	CFileFind   tem_fFileFind;
	if (!tem_fFileFind.FindFile(tem_strCurDir))
	{
		CreateDirectory(tem_strCurDir, NULL);
	}



	//2)获取当前已经勾选的项，并将勾选项放入vector
	for (unsigned int i=0; i<m_vcImageName.size(); i++)
	{
		if (m_conListMerge.GetItemState(i, LVIS_SELECTED)==LVIS_SELECTED||m_conListMerge.GetCheck(i))
		{
			m_vcMergeIndex.push_back(i);
		}
	}
	if (m_vcMergeIndex.size()<=0)
	{
//		CDialogEx::OnCancel();
		return;
	}
	//3)根据奇偶数计算合并次数
	int  tem_nMergeCount = 0;
	if (m_vcMergeIndex.size()%2==0)
	{
		tem_nMergeCount = m_vcMergeIndex.size();
	}
	else
	{
		tem_nMergeCount = m_vcMergeIndex.size()-1;  //最后一页不合并
	}
	m_conProMerge.SetRange(0, tem_nMergeCount/2);
	m_conProMerge.SetPos(0);

	//4)根据不同的合并方式进行合并
	int      j=1;
	//邻页合并
	if (m_nMergeMode == 0)
	{
		for (int i=0; i<tem_nMergeCount; i+=2)
		{
			int      tem_nImage1   = m_vcMergeIndex[i];
			int      tem_nImage2   = m_vcMergeIndex[i+1];

			CString  tem_strImage1 = m_vcImagePath[tem_nImage1];
			CString  tem_strImage2 = m_vcImagePath[tem_nImage2];

			Self_MergeImages(tem_strImage1, tem_strImage2, m_nMergeSite, m_nSaveType, m_BSaved, j, tem_strCurDir);

			m_conProMerge.OffsetPos(1);
			float    tem_fDidRatio = (j*1.0)/(tem_nMergeCount/2);
			int      tem_nDidRatio = (int)(tem_fDidRatio*100);
			CString  tem_strDidRatio = _T("");
			tem_strDidRatio.Format(_T("%d"), tem_nDidRatio);
			tem_strDidRatio += _T("%");
			m_StaMergeDid = tem_strDidRatio;
			UpdateData(FALSE);

			j++;

		}
	}
	//隔页合并
	else if (m_nMergeMode == 1)
	{
		for (int i=0; i<tem_nMergeCount; i+=4)
		{
			//终止条件说明：隔页合并，最后应剩4页
			if ((i+2)>=tem_nMergeCount)
			{
				break;
			}
			int      tem_nImage1   = m_vcMergeIndex[i] ;
			int      tem_nImage2   = m_vcMergeIndex[i+2];

			CString  tem_strImage1 = m_vcImagePath[tem_nImage1];
			CString  tem_strImage2 = m_vcBussPath[tem_nImage2];

			Self_MergeImages(tem_strImage1, tem_strImage2, m_nMergeSite, m_nSaveType, m_BSaved, j, tem_strCurDir);
			m_conProMerge.OffsetPos(1);
			float    tem_fDidRatio = (j*1.0)/(tem_nMergeCount/2);
			int      tem_nDidRatio = (int)(tem_fDidRatio*100);
			CString  tem_strDidRatio = _T("");
			tem_strDidRatio.Format(_T("%d"), tem_nDidRatio);
			tem_strDidRatio += _T("%");
			m_StaMergeDid = tem_strDidRatio;
			UpdateData(FALSE);
			j++;

			if ((i+3)>=tem_nMergeCount)
			{
				break;
			}
			tem_nImage1   = m_vcMergeIndex[i+1];
			tem_nImage2   = m_vcMergeIndex[i+3];

			tem_strImage1 = m_vcImagePath[tem_nImage1];
			tem_strImage2 = m_vcBussPath[tem_nImage2];

			Self_MergeImages(tem_strImage1, tem_strImage2, m_nMergeSite, m_nSaveType, m_BSaved, j, tem_strCurDir);
			m_conProMerge.OffsetPos(j);

			tem_fDidRatio = (j*1.0)/(tem_nMergeCount/2);
			tem_nDidRatio = (int)(tem_fDidRatio*100);
			tem_strDidRatio = _T("");
			tem_strDidRatio.Format(_T("%d"), tem_nDidRatio);
			tem_strDidRatio += _T("%");
			m_StaMergeDid = tem_strDidRatio;
			UpdateData(FALSE);
			j++;

		}
		m_StaMergeDid = _T("100%");
		UpdateData(FALSE);
	}
	//前后合并
	else if (m_nMergeMode == 2)
	{
		for (int i=0, k=tem_nMergeCount/2; i<tem_nMergeCount/2; i++, k++)
		{
			int      tem_nImage1   = m_vcMergeIndex[i];
			int      tem_nImage2   = m_vcMergeIndex[k];

			CString  tem_strImage1 = m_vcImagePath[tem_nImage1];
			CString  tem_strImage2 = m_vcImagePath[tem_nImage2];

			Self_MergeImages(tem_strImage1, tem_strImage2, m_nMergeSite, m_nSaveType, m_BSaved, j, tem_strCurDir);
			m_conProMerge.OffsetPos(1);
			float    tem_fDidRatio = (j*1.0)/(tem_nMergeCount/2);
			int      tem_nDidRatio = (int)(tem_fDidRatio*100);
			CString  tem_strDidRatio = _T("");
			tem_strDidRatio.Format(_T("%d"), tem_nDidRatio);
			tem_strDidRatio += _T("%");
			m_StaMergeDid = tem_strDidRatio;
			UpdateData(FALSE);
			j++;
		}

	}

	m_vcMergeIndex.clear();
	g_BMerged = TRUE;
	//m_vcImageName.clear();
	//m_vcImagePath.clear();
	


//	CDialogEx::OnOK();
}


void CUDSSUBMERGE::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	m_vcMergeIndex.clear();
	m_vcImageName.clear();
	m_vcImagePath.clear();
	CDialogEx::OnCancel();
}


void CUDSSUBMERGE::OnBnClickedBtnHelp()
{
	// TODO: 在此添加控件通知处理程序代码
	m_dlgHelp.DoModal();
}


