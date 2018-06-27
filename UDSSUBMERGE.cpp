// UDSSUBMERGE.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDSPlatformV1.7.h"
#include "UDSSUBMERGE.h"
#include "afxdialogex.h"



extern std::vector<CString>   m_vcFileName;    //�ļ���
extern std::vector<CString>   m_vcFilePath;    //ͼ���ļ�·��
extern std::vector<CString>   m_vcBussPath;    //pdf+txt+ͼ��·�� 
extern BOOL                   g_BMerged;       //���ڲ�����MB_OK�����ʹ��ȫ�ֱ�����ʾ

// CUDSSUBMERGE �Ի���

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


// CUDSSUBMERGE ��Ϣ�������


BOOL CUDSSUBMERGE::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	/*****************************
	*
	* 1��������ʼ��
	* 1.5��ɸѡͼ���ļ�
	* 2��ListCtrl��ʼ��
	* 3��CheckBox��ʼ��
	* 4��Static��ʼ��
	* 5��ProgressBar��ʼ��
	*
	*****************************/
	/*1��������ʼ��*/
	m_nMergeSite = 0;
	m_nMergeMode = 0;
	m_nSaveType  = 0;
	m_BSaved     = TRUE;
	m_vcMergeIndex.clear();
	m_vcImageName.clear();
	m_vcImagePath.clear();

	/*1.5��ɸѡͼ���ļ�*/
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
			//��ͼ���ļ�
			m_vcImageName.push_back(tem_strFileName);
			m_vcImagePath.push_back(m_vcBussPath[i]);
		}
	}

	/*2��ListCtrl��ʼ��*/
	//0)��ȡListCtrl�ߴ�
	CRect    tem_rcListCtrl;
	GetDlgItem(IDC_LIST_MERGE)->GetClientRect(&tem_rcListCtrl);
	int      tem_nListWidth = tem_rcListCtrl.Width();
	//1)����ListCtrl���
	DWORD     tem_dwStyle;
	tem_dwStyle  = LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES|LVS_EX_FULLROWSELECT;
	m_conListMerge.SetExtendedStyle(tem_dwStyle);
	//2)���ListCtrl��
	m_conListMerge.InsertColumn(0, _T("ѡ��"), LVCFMT_CENTER, 40);
	m_conListMerge.InsertColumn(1, _T("�ļ���"), LVCFMT_CENTER, tem_nListWidth-40);
	//3)���ListCtrl�У������ListCtrl��Ϣ
	for (unsigned int i=0; i<m_vcImageName.size(); i++)
	{
		m_conListMerge.InsertItem(i, _T(""));
		m_conListMerge.SetCheck(i, TRUE);
		m_conListMerge.SetItemText(i, 1, m_vcImageName[i]);
	}

	/*3��Checkbox��ʼ��*/
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

	/*4��Static��ʼ��*/
	m_StaSlcNumer = _T("��ѡ�� ");
	int tem_nSlcNumber = m_vcImageName.size();
	CString  tem_strSlcNumber;
	tem_strSlcNumber.Format(_T("%d"), tem_nSlcNumber);
	m_StaSlcNumer += tem_strSlcNumber;
	UpdateData(FALSE);

	/*5��ProgressBar��ʼ��*/
	m_conProMerge.SetRange(0, m_vcImageName.size());
	m_conProMerge.SetPos(0);

	/*6��������Static��ʼ��*/
	m_StaMergeDid = _T("0%");
	UpdateData(FALSE);


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CUDSSUBMERGE::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}


/************************************
* ˵�����ϲ��㷨
*
* 1��ͼ��ϲ��㷨
* 2����ȡͼ������
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
		//���Һϲ�
		pImage.Create(tem_nImgAWidth+tem_nImgBWidth, tem_nImgHeight, 24);
		pImage.MixFrom(pImageA, 0, 0);
		pImage.MixFrom(pImageB, tem_nImgAWidth, 0);
	}
	else if (site == 1)
	{
		//���ºϲ�
		pImage.Create(tem_nImgWidth, tem_nImgAHeight+tem_nImgBHeight, 24);
		pImage.MixFrom(pImageA, 0, tem_nImgBHeight);
		pImage.MixFrom(pImageB, 0, 0);
	}

	
	CString tem_strFileName;
	tem_strFileName.Format(_T("%04d"), name);

	CString tem_strFilePath = dir;
	tem_strFilePath += _T("\\�ϲ�ͼ");
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
		//ɾ��ԭͼ
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
* ˵����CheckBox
*
* 1�����Һϲ�
* 2�����ºϲ�
* 3����ҳ
* 4����ҳ
* 5��ǰ��
* 6��JPG
* 7��BMP
* 8��PNG
* 9��TIF
* 10��Save SrcImage
* 11��ȫѡ
* 12��ѡ����������
*
************************************/

void CUDSSUBMERGE::OnClickedChkLeft()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHK_ALLSLC))
	{
		//ȫѡ
		for (unsigned int i=0; i<m_vcImageName.size(); i++)
		{
			m_conListMerge.SetCheck(i, TRUE);
		}
	} 
	else
	{
		//ȫ��ѡ
		for (unsigned int i=0; i<m_vcImageName.size(); i++)
		{
			m_conListMerge.SetCheck(i, FALSE);
		}
	}
}


void CUDSSUBMERGE::OnItemchangedListMerge(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_StaSlcNumer = _T("��ѡ�� ");
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
		//�Զ���ѡȫѡ��
		((CButton*)GetDlgItem(IDC_CHK_ALLSLC))->SetCheck(TRUE);
	}
	else
	{
		//ȡ����ѡȫѡ��
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
* ˵�������ܰ�ť
*
* 1������
* 2��ȡ��
* 3������
*
************************************/
void CUDSSUBMERGE::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//1)��������Ŀ¼
	//a����ȡ��ǰĿ¼
	CString  tem_strCurDir = m_vcBussPath[0];
	TCHAR*   tem_cCurDir;
	tem_cCurDir = tem_strCurDir.GetBuffer();
	tem_strCurDir.ReleaseBuffer();
	PathRemoveFileSpec(tem_cCurDir);
	tem_strCurDir  = tem_cCurDir;
	tem_strCurDir += _T("\\");
	tem_strCurDir += _T("�ϲ�ͼ");
	
	//b�������Ŀ¼�Ƿ����
	CFileFind   tem_fFileFind;
	if (!tem_fFileFind.FindFile(tem_strCurDir))
	{
		CreateDirectory(tem_strCurDir, NULL);
	}



	//2)��ȡ��ǰ�Ѿ���ѡ���������ѡ�����vector
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
	//3)������ż������ϲ�����
	int  tem_nMergeCount = 0;
	if (m_vcMergeIndex.size()%2==0)
	{
		tem_nMergeCount = m_vcMergeIndex.size();
	}
	else
	{
		tem_nMergeCount = m_vcMergeIndex.size()-1;  //���һҳ���ϲ�
	}
	m_conProMerge.SetRange(0, tem_nMergeCount/2);
	m_conProMerge.SetPos(0);

	//4)���ݲ�ͬ�ĺϲ���ʽ���кϲ�
	int      j=1;
	//��ҳ�ϲ�
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
	//��ҳ�ϲ�
	else if (m_nMergeMode == 1)
	{
		for (int i=0; i<tem_nMergeCount; i+=4)
		{
			//��ֹ����˵������ҳ�ϲ������Ӧʣ4ҳ
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
	//ǰ��ϲ�
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_vcMergeIndex.clear();
	m_vcImageName.clear();
	m_vcImagePath.clear();
	CDialogEx::OnCancel();
}


void CUDSSUBMERGE::OnBnClickedBtnHelp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_dlgHelp.DoModal();
}


