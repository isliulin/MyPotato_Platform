// UDSSUBOUTPUT.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDSPlatformV1.7.h"
#include "UDSSUBOUTPUT.h"
#include "afxdialogex.h"


extern std::vector<CString>   g_vcCtrlName;
extern std::vector<CString>   g_vcCtrlPath;
// CUDSSUBOUTPUT �Ի���

IMPLEMENT_DYNAMIC(CUDSSUBOUTPUT, CDialogEx)

CUDSSUBOUTPUT::CUDSSUBOUTPUT(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUDSSUBOUTPUT::IDD, pParent)
{

}

CUDSSUBOUTPUT::~CUDSSUBOUTPUT()
{
}

void CUDSSUBOUTPUT::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FILES, m_conListFiles);
	DDX_Control(pDX, IDC_EDIT_OUTPATH, m_conEditPath);
	DDX_Control(pDX, IDC_BTN_ADDFILE, m_btnAddItem);
	DDX_Control(pDX, IDC_BTN_DLTFILE, m_btnDltItem);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BTN_SLTDOC, m_btnSlcDoc);
	DDX_Control(pDX, IDC_BTN_DOWN, m_btnDown);
	DDX_Control(pDX, IDC_BTN_UP, m_btnUp);
}


BEGIN_MESSAGE_MAP(CUDSSUBOUTPUT, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_SLTDOC, &CUDSSUBOUTPUT::OnBnClickedBtnSltdoc)
	ON_BN_CLICKED(IDC_BTN_ADDFILE, &CUDSSUBOUTPUT::OnBnClickedBtnAddfile)
	ON_BN_CLICKED(IDC_BTN_DLTFILE, &CUDSSUBOUTPUT::OnBnClickedBtnDltfile)
	ON_BN_CLICKED(IDOK, &CUDSSUBOUTPUT::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CUDSSUBOUTPUT::OnBnClickedCancel)
	ON_NOTIFY(NM_CLICK, IDC_LIST_FILES, &CUDSSUBOUTPUT::OnClickListFiles)
	ON_EN_CHANGE(IDC_EDIT_OUTPATH, &CUDSSUBOUTPUT::OnChangeEditOutpath)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST_FILES, &CUDSSUBOUTPUT::OnBegindragListFiles)
	ON_BN_CLICKED(IDC_BTN_UP, &CUDSSUBOUTPUT::OnBnClickedBtnUp)
	ON_BN_CLICKED(IDC_BTN_DOWN, &CUDSSUBOUTPUT::OnBnClickedBtnDown)
END_MESSAGE_MAP()


// CUDSSUBOUTPUT ��Ϣ�������


BOOL CUDSSUBOUTPUT::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	/************************************************
	*
	* 1��������ʼ��
	* 2��ListCtrl��ʼ��
	* 3��EditCtrl��ʼ��
	* 4����Ӵ��ڱ���
	* 5��ListCtrl����
	* 6��Button����
	*
	************************************************/
	//1��������ʼ��-----------------------------------------------
	m_nRows      = 0;
	m_nSlctFile  = -1;
	m_nDragIndex = -1;
	m_BOutput    = FALSE;

	//2��ListCtrl��ʼ��--------------------------------------------
	DWORD    dwStyle;
	dwStyle   = LVS_EX_FULLROWSELECT;
	dwStyle  |= LVS_EX_GRIDLINES;       //����������

	m_conListFiles.SetExtendedStyle(dwStyle);
	m_conListFiles.SetBkColor(RGB(238, 233, 233));  //���ñ���ɫ
	m_conListFiles.SetTextColor(RGB(0, 0, 0));   //����������ɫ
	m_conListFiles.InsertColumn(0, _T("�ļ�·��"), LVCFMT_CENTER, 420, -1);

	CString    tem_strFilePath;
	std::vector<CString>::iterator item_file;
	for (item_file=g_vcCtrlPath.begin(); item_file!=g_vcCtrlPath.end(); item_file++)
	{
		tem_strFilePath = *item_file;
		m_conListFiles.InsertItem(m_nRows, tem_strFilePath);
		m_nRows++;
	}

	//3��EditControl��ʼ��------------------------------------------
	TCHAR tem_MyDir[_MAX_PATH];  
	SHGetSpecialFolderPath(this->GetSafeHwnd(),tem_MyDir,CSIDL_DESKTOP,0);
	CString   tem_strPDFPath;
	tem_strPDFPath = tem_MyDir;

	if (!PathIsDirectory(tem_strPDFPath))
	{
		CreateDirectory(tem_strPDFPath, NULL);  //��������Ŀ¼
	}
	tem_strPDFPath += "\\";  
	if (m_BOFD)
	{
		tem_strPDFPath += "Desktop.ofd";
	}
	else if (m_BPdfTif)
	{
		tem_strPDFPath += "Desktop.pdf";
	} 
	else
	{
		tem_strPDFPath += "Desktop.tif";
	}
	m_conEditPath.SetWindowText(tem_strPDFPath);

	//4����Ӵ��ڱ���-------------------------------------------------
	CDialogEx::SetBackgroundColor(RGB(72, 77, 91));
//	LoadPicture(m_imgSubBK, IDB_PNG_SUBBK, _T("PNG"));

	//5��ListCtrl����------------------------------------------------
	m_conListFiles.SetBkColor(RGB(94, 147, 163));
	m_conListFiles.SetTextColor(RGB(0, 0, 0));

	//6��Button����--------------------------------------------------
	m_btnAddItem.Init(IDB_PNG_TJ, BTN_IMG_1, BTN_TYPE_NORMAL);
	m_btnDltItem.Init(IDB_PNG_SC, BTN_IMG_1, BTN_TYPE_NORMAL);
	m_btnOK.Init(IDB_PNG_DC, BTN_IMG_1, BTN_TYPE_NORMAL);
	m_btnCancel.Init(IDB_PNG_TC, BTN_IMG_1, BTN_TYPE_NORMAL);
	m_btnSlcDoc.Init(IDB_PNG_LL, BTN_IMG_1,BTN_TYPE_NORMAL);
	m_btnUp.Init(IDB_PNG_UP, BTN_IMG_1, BTN_TYPE_NORMAL);
	m_btnDown.Init(IDB_PNG_DOWN, BTN_IMG_1, BTN_TYPE_NORMAL);


	m_btnSubTips.Create(this);
	m_btnSubTips.AddTool(GetDlgItem(IDC_BTN_SLTDOC), _T("ѡ�񱣴�·��"));
	m_btnSubTips.AddTool(GetDlgItem(IDC_BTN_ADDFILE), _T("����ļ�"));
	m_btnSubTips.AddTool(GetDlgItem(IDC_BTN_DLTFILE), _T("ɾ���ļ�"));
	m_btnSubTips.AddTool(GetDlgItem(IDOK), _T("����"));
	m_btnSubTips.AddTool(GetDlgItem(IDCANCEL), _T("ȡ��"));
	m_btnSubTips.AddTool(GetDlgItem(IDC_BTN_SLTDOC), _T("ѡ�񱣴�Ŀ¼"));
	m_btnSubTips.AddTool(GetDlgItem(IDC_BTN_UP), _T("����"));
	m_btnSubTips.AddTool(GetDlgItem(IDC_BTN_DOWN), _T("����"));

	m_btnSubTips.SetDelayTime(TTDT_INITIAL, 100);
	m_btnSubTips.SetDelayTime(TTDT_AUTOPOP, 5000);
	m_btnSubTips.SetDelayTime(TTDT_RESHOW, 100);

	m_btnSubTips.SetTipTextColor(RGB(0, 0, 0));
	m_btnSubTips.SetTipBkColor(RGB(255, 255, 255));
	m_btnSubTips.Activate(TRUE);




	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CUDSSUBOUTPUT::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	/*
	CDC*      pDC = &dc;
	CDC       dcMem;
	dcMem.CreateCompatibleDC(pDC);

	CRect      tem_rcClient;
	GetClientRect(tem_rcClient);
	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pDC, tem_rcClient.Width(), tem_rcClient.Height());
	dcMem.SelectObject(memBitmap);
	dcMem.FillSolidRect(tem_rcClient, RGB(255, 255, 255));	//���û�����ɫ
	if (!m_imgSubBK.IsNull())
	{
		CRect rcImg = CRect(0, 0, m_imgSubBK.GetWidth(), m_imgSubBK.GetHeight());
		m_imgSubBK.Draw(dcMem.m_hDC, rcImg, rcImg);
	}
	pDC->BitBlt(0, 0, tem_rcClient.Width(), tem_rcClient.Height(), &dcMem, 0, 0, SRCCOPY);
	memBitmap.DeleteObject();
	*/
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}


BOOL CUDSSUBOUTPUT::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_MOUSEMOVE)
	{
		m_btnSubTips.RelayEvent(pMsg);
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


HBRUSH CUDSSUBOUTPUT::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (pWnd->GetDlgCtrlID()==IDC_EDIT_OUTPATH)
	{
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255, 255, 255));
	}
	//�ٽ����͸��
	pWnd = pWnd->GetParent();
	if (pWnd)
	{
		if (pWnd->GetDlgCtrlID()==IDC_EDIT_OUTPATH)
		{
			hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
			pDC->SetBkMode(TRANSPARENT);
		}
	}

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


/***************************
*       ����Ŀ¼
* 1��ѡ��·����ť
* 2������ļ���ť
* 3��ɾ���ļ���ť
* 4��������ť
* 5��ȡ����ť
* 6�����ListCtrl����
* 7��EidtControl��⺯��
* 8����ʼ��ק
* 9������
* 10������
*
****************************/
void CUDSSUBOUTPUT::OnBnClickedBtnSltdoc()
{
	TCHAR     m_selectpath[MAX_PATH];
	CString   m_goalpath;
	//	CString *p_goalpath;
	ZeroMemory(m_selectpath, sizeof(m_selectpath));
	BROWSEINFO   m_broseinfo;
	m_broseinfo.hwndOwner = m_hWnd;
	m_broseinfo.pidlRoot  = NULL;
	m_broseinfo.pszDisplayName = m_selectpath;
	m_broseinfo.lpszTitle = _T("��ѡ��Ŀ¼"); 
	m_broseinfo.ulFlags   = 0;
	m_broseinfo.lpfn      = NULL;
	m_broseinfo.lParam    = 0;
	m_broseinfo.iImage    = 0;
	LPITEMIDLIST   lp = SHBrowseForFolder(&m_broseinfo);
	if (lp&&SHGetPathFromIDList(lp, m_selectpath))
	{
		m_goalpath.Format(_T("%s"), m_selectpath);
//		m_goalpath.Replace(_T("\\"), _T("\\\\"));
		//C�̻����4��\�������������������һλΪ\�����
		if (m_goalpath.Right(1) != _T("\\"))
		{
			m_strFileName = m_goalpath;
			m_strFileName = PathFindFileName(m_strFileName);
			m_goalpath =m_goalpath+_T("\\");
		}
		else
		{
			m_strFileName = m_goalpath;
			m_strFileName = m_strFileName.Left(m_strFileName.GetLength()-3);
		}
		if (m_BPdfTif)
		{
			m_strFileName += _T(".pdf");
		}
		else
		{
			m_strFileName += _T(".tif");
		}
		m_goalpath += m_strFileName;

		m_conEditPath.SetWindowText(m_goalpath);

		CRect   tem_rctEdit;
		GetDlgItem(IDC_EDIT_OUTPATH)->GetWindowRect(&tem_rctEdit);
		ScreenToClient(&tem_rctEdit);
		InvalidateRect(tem_rctEdit);	
		int  tem_nlength = m_conEditPath.GetWindowTextLength();
		m_conEditPath.SetSel(1, 1, FALSE);
		m_conEditPath.SetFocus();

		CRect    tem_rcEdit;

		GetDlgItem(IDC_EDIT_SEARCH)->GetWindowRect(&tem_rcEdit); 
		ScreenToClient(&tem_rcEdit);        
		InvalidateRect(&tem_rcEdit);

		UpdateWindow();

	} 
	else
	{
		AfxMessageBox(_T("ѡ��Ŀ¼��Ч"));
	}	
}


void CUDSSUBOUTPUT::OnBnClickedBtnAddfile()
{
	//1�� ���ù�������ֻ�����ͼ���ļ�
	TCHAR  szFilter[] = _T("ͼ���ļ�(*.jpg *.bmp *.png *.tif)|*.jpg;*.bmp;*.png;*.tif||");
	//2�� ���챣���ļ��Ի���
	CFileDialog  fileDlg(TRUE, _T(""), NULL, 0, szFilter, this);
	CString      tem_strFilePath = _T("");
	if (IDOK == fileDlg.DoModal())
	{
		tem_strFilePath = fileDlg.GetPathName();
		m_conListFiles.InsertItem(m_nRows, tem_strFilePath);
		m_nRows++;

		g_vcCtrlPath.push_back(tem_strFilePath);
		tem_strFilePath = PathFindFileName(tem_strFilePath);
		g_vcCtrlName.push_back(tem_strFilePath);
	}
}


void CUDSSUBOUTPUT::OnBnClickedBtnDltfile()
{
	if (m_nSlctFile>=0)
	{
		g_vcCtrlName.erase(g_vcCtrlName.begin() + m_nSlctFile);
		g_vcCtrlPath.erase(g_vcCtrlPath.begin() + m_nSlctFile);
		m_conListFiles.DeleteItem(m_nSlctFile);
	}
}


void CUDSSUBOUTPUT::OnBnClickedOk()
{
	m_BOutput = TRUE;

	CString   tem_strEdit       = _T("");
	CString   tem_strFileFormat = _T("");  
	
	//��ȡEdit�ؼ����ļ���,�ж��ַ������Ƿ�Ϸ�
	m_conEditPath.GetWindowText(tem_strEdit);
	m_strPDFPath      = tem_strEdit;
	tem_strFileFormat = tem_strEdit;

	if (m_BOFD)
	{
		//��ʱΪOFD�����OFD·���Ƿ�Ϸ�
		tem_strFileFormat = PathFindExtension(tem_strFileFormat);
		tem_strFileFormat.MakeLower();
		if (tem_strFileFormat != _T(".ofd"))
		{
			//�ļ������Ϸ�
			MessageBox(_T("�����ļ�����׺��"), _T("ͼ�񵼳�"), MB_OK);
			return;
		}
		else
		{
			//����OFD�ļ�
			CString tem_strOfdPath = m_strPDFPath;
			Self_GenerateOFD(g_vcCtrlPath, tem_strOfdPath);	
			MessageBox(_T("������ϣ�"), _T("ͼ�񵼳�"), MB_OK);			
		}
		g_vcCtrlName.clear();
		g_vcCtrlPath.clear();
	}
	else if (m_BPdfTif)
	{
		//��ʱΪPDF�����PDF·���Ƿ�Ϸ�
		tem_strFileFormat = PathFindExtension(tem_strFileFormat);
		tem_strFileFormat.MakeLower();
		if (tem_strFileFormat != _T(".pdf"))
		{
			//�ļ������Ϸ�
			MessageBox(_T("�����ļ�����׺��"), _T("ͼ�񵼳�"), MB_OK);
			return;
		}
		else
		{
			//����PDF�ļ�
			int        image, code, pagenum=2;
			wstring    outfile;
			outfile = m_strPDFPath.GetBuffer(0);
			m_strPDFPath.ReleaseBuffer();
			PDFlib*    pPdf;
			pPdf = new PDFlib;
			try
			{
				pPdf->set_parameter(_T("nodemostamp"), _T("false"));
				pPdf->set_parameter(_T("errorpolicy"), _T("return"));
				code = pPdf->begin_document(outfile, _T(""));
				if (code == -1)
				{
					MessageBox(_T("����PDFʧ�ܣ�"));
				}
				//����PDF������Ϣ******************************************************
				pPdf->set_info(_T("Creator"), _T("Smartscan"));   
				pPdf->set_info(_T("Author"), _T("UDS"));   
				pPdf->set_info(_T("Title"), _T("UDSPDF")); 
				pPdf->set_info(_T("Subject"), _T("ɨ���ĵ�"));

				//ͼƬ����***********************************************************
				std::vector<CString>::iterator  item_Name;
				std::vector<CString>::iterator  item_Path;
				item_Name = g_vcCtrlName.begin();
				for (item_Path=g_vcCtrlPath.begin(); item_Path!=g_vcCtrlPath.end(); item_Path++, item_Name++)
				{
					CString   tem_strFilePath = *item_Path;
					CString   tem_strFileName = *item_Name;
					wstring sFilePath(tem_strFilePath.GetBuffer());
					wstring sFileName(tem_strFileName.GetBuffer());
					image = pPdf->load_image(_T("auto"), sFilePath, _T(""));
					pPdf->begin_page_ext(10, 10, _T(""));
					pPdf->fit_image(image, 0, 0, _T("adjustpage dpi=0"));  //����ͼ��
					pPdf->close_image(image);
					pPdf->create_bookmark(sFileName, _T("index=-1"));
					pPdf->end_page_ext(_T(""));
				}		
				pPdf->end_document(_T(""));	
				MessageBox(_T("������ϣ�"), _T("ͼ�񵼳�"), MB_OK);
			}
			catch (PDFlib::Exception e)
			{
				int     num=-1;
				wstring  str;
				CString str2, str3;
				str = e.get_errmsg();
				num = e.get_errnum();
				str2 = str.c_str();
				str3.Format(_T("%d"), num);
//				MessageBox(str2);
//				MessageBox(str3);
			}
		}
		g_vcCtrlName.clear();
		g_vcCtrlPath.clear();
	} 
	else
	{
		//��ʱΪ��ҳTIF�����TIF·���Ƿ�Ϸ�
		tem_strFileFormat = PathFindExtension(tem_strFileFormat);
		tem_strFileFormat.MakeLower();
		if (tem_strFileFormat != _T(".tif"))
		{
			//�ļ������Ϸ�
			MessageBox(_T("�����ļ�����׺��"), _T("ͼ�񵼳�"), MB_OK);
			return;
		}
		else
		{
			//������ҳTIF
			CString str1 =_T("123");  
			int len =WideCharToMultiByte(CP_ACP, 0, m_strPDFPath, -1, NULL, 0, NULL, NULL);  
			char*  tem_cPDFPath =new char[len +1];  
			WideCharToMultiByte(CP_ACP,0,m_strPDFPath,-1,tem_cPDFPath,len,NULL,NULL ); 


// 			const size_t  strsize      = (m_strPDFPath.GetLength()+1)*2;   //���ַ�����
// 			char*        tem_cPDFPath = new char[strsize];               //����ռ�
// 			size_t        sz           = 0;
// 			wcstombs_s(&sz, tem_cPDFPath, strsize, m_strPDFPath, _TRUNCATE);  //CString -> const char*

			CString      tem_strMode = _T("w+b");
			len =WideCharToMultiByte(CP_ACP, 0, tem_strMode, -1, NULL, 0, NULL, NULL);  
			char*  tem_cMode =new char[len +1];  
			WideCharToMultiByte(CP_ACP,0,tem_strMode,-1,tem_cMode,len,NULL,NULL ); 
			
			CString     tem_strTifPath   = _T("");
			CString     tem_strTifFormat = _T("");
			FILE*       tem_hFile;
			CxImageTIF  mutitif;
			tem_hFile = fopen(tem_cPDFPath, tem_cMode);

			std::vector<CString>::iterator tiff_iter;
			for (tiff_iter=g_vcCtrlPath.begin(); tiff_iter!=g_vcCtrlPath.end(); tiff_iter++)
			{
				tem_strTifPath   = *tiff_iter;
				tem_strTifFormat = *tiff_iter;
				tem_strTifFormat = PathFindExtension(tem_strTifFormat);
				tem_strTifFormat = tem_strTifFormat.Mid(1);
				tem_strTifFormat.MakeLower();
				if (tem_strTifFormat == _T("bmp"))
				{
					mutitif.Load(tem_strTifPath, CXIMAGE_FORMAT_BMP);
					mutitif.SetCodecOption(5, CXIMAGE_FORMAT_TIF);
					mutitif.Encode(tem_hFile, true);
				} 
				else if (tem_strTifFormat == _T("png"))
				{
					mutitif.Load(tem_strTifPath, CXIMAGE_FORMAT_PNG);
					mutitif.SetCodecOption(5, CXIMAGE_FORMAT_TIF);
					mutitif.Encode(tem_hFile, true);
				}
				else if (tem_strTifFormat == _T("jpg") || tem_strTifFormat == _T("jpeg"))
				{
					mutitif.Load(tem_strTifPath, CXIMAGE_FORMAT_JPG);
					mutitif.SetCodecOption(5, CXIMAGE_FORMAT_TIF);
					mutitif.Encode(tem_hFile, true);
				} 
				else if (tem_strTifFormat == _T("tif") || tem_strTifFormat == _T("tiff"))
				{
					mutitif.Load(tem_strTifPath, CXIMAGE_FORMAT_TIF);
					mutitif.SetCodecOption(5, CXIMAGE_FORMAT_TIF);
					mutitif.Encode(tem_hFile, true);
				} 
				else
				{
					//�����ļ�
				}	
				
			}

			fclose(tem_hFile);
			delete[] tem_cPDFPath;
			delete[] tem_cMode;
			MessageBox(_T("������ϣ�"), _T("ͼ�񵼳�"), MB_OK);	
			
		}
	}
	CDialogEx::OnOK();
}


void CUDSSUBOUTPUT::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_BOutput = FALSE;
	CDialogEx::OnCancel();
}


void CUDSSUBOUTPUT::OnClickListFiles(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int   tem_nItemIndex = pNMItemActivate->iItem;
	if (tem_nItemIndex>=0 && tem_nItemIndex<m_conListFiles.GetItemCount())
	{
		m_nSlctFile = tem_nItemIndex;
	}
	else
	{
		m_nSlctFile = -1;
	}
	*pResult = 0;
}


void CUDSSUBOUTPUT::OnChangeEditOutpath()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CRect    tem_rcEdit;

	GetDlgItem(IDC_EDIT_OUTPATH)->GetWindowRect(&tem_rcEdit); 
	ScreenToClient(&tem_rcEdit);        
	InvalidateRect(&tem_rcEdit);

	UpdateWindow();
}


void CUDSSUBOUTPUT::OnBegindragListFiles(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_nDragIndex = pNMLV->iItem;
	
	*pResult = 0;
}


void CUDSSUBOUTPUT::OnBnClickedBtnUp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_nSlctFile>0)
	{
		//ȡ��ѡ��������
		CString   tem_strCurItem  = g_vcCtrlPath[m_nSlctFile];
		CString   tem_strCurName  = g_vcCtrlName[m_nSlctFile];
		//ȡ����һ������
		CString   tem_strLastItem = g_vcCtrlPath[m_nSlctFile-1];
		CString   tem_strLastName = g_vcCtrlName[m_nSlctFile-1];

		//����vector�ڶ�Ӧitem
		g_vcCtrlPath[m_nSlctFile-1] = tem_strCurItem;
		g_vcCtrlName[m_nSlctFile-1] = tem_strCurName;
		g_vcCtrlPath[m_nSlctFile]   = tem_strLastItem;
		g_vcCtrlName[m_nSlctFile]   = tem_strLastName;
		
		//������������
		m_conListFiles.SetItemText(m_nSlctFile-1, 0, tem_strCurItem);
		m_conListFiles.SetItemText(m_nSlctFile, 0, tem_strLastItem);
		 
		//ȡ����ǰ������ѡ��
		m_conListFiles.SetItemState(m_nSlctFile, 0, -1);

		//���µ�ǰ�������ֵ
		m_nSlctFile--;

		//�����µ�ѡ��
		m_conListFiles.SetFocus();
		m_conListFiles.SetItemState(m_nSlctFile, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	}
	else if (m_nSlctFile==0)
	{
		//ѡ�е�Ϊ��һ����ָ���
		m_conListFiles.SetFocus();
		m_conListFiles.SetItemState(m_nSlctFile, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	}
}


void CUDSSUBOUTPUT::OnBnClickedBtnDown()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_nSlctFile>=0 && m_nSlctFile<g_vcCtrlPath.size()-1)
	{
		//ȡ��ѡ��������
		CString   tem_strCurItem  = g_vcCtrlPath[m_nSlctFile];
		CString   tem_strCurName  = g_vcCtrlName[m_nSlctFile];
		//ȡ����һ������
		CString   tem_strNextItem = g_vcCtrlPath[m_nSlctFile+1];
		CString   tem_strNextName = g_vcCtrlName[m_nSlctFile+1];

		//����vector�ڶ�Ӧitem
		g_vcCtrlPath[m_nSlctFile+1] = tem_strCurItem;
		g_vcCtrlName[m_nSlctFile+1] = tem_strCurName;
		g_vcCtrlPath[m_nSlctFile]   = tem_strNextItem;
		g_vcCtrlName[m_nSlctFile]   = tem_strNextName;

		//������������
		m_conListFiles.SetItemText(m_nSlctFile+1, 0, tem_strCurItem);
		m_conListFiles.SetItemText(m_nSlctFile, 0, tem_strNextItem);

		//ȡ����ǰ������ѡ��
		m_conListFiles.SetItemState(m_nSlctFile, 0, -1);

		//���µ�ǰ�������ֵ
		m_nSlctFile++;

		//�����µ�ѡ��
		m_conListFiles.SetFocus();
		m_conListFiles.SetItemState(m_nSlctFile, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	}
	else if (m_nSlctFile>=0 && m_nSlctFile==g_vcCtrlPath.size()-1)
	{
		//ѡ�����һ����ָ���
		m_conListFiles.SetFocus();
		m_conListFiles.SetItemState(m_nSlctFile, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	}

}


CString CUDSSUBOUTPUT::Self_GenerateOFD(vector<CString> imgspath, CString dstpath)
{
	if (imgspath.size()>0)
	{
		CString tem_strFilesPath = _T("\"");
		tem_strFilesPath += _T("0");
		tem_strFilesPath += _T("\"");
		tem_strFilesPath += _T(" ");
		//����dstpath�ո�
		tem_strFilesPath += _T("\"");
		tem_strFilesPath += dstpath;
		tem_strFilesPath += _T("\"");
		std::vector<CString>::iterator  item_Path;
		for (item_Path=g_vcCtrlPath.begin(); item_Path!=g_vcCtrlPath.end(); item_Path++)
		{
			CString   tem_strFilePath = *item_Path;
			//����tem_strFilePath�ո�
// 			tem_strFilePath.Replace(_T(" "), _T(""));
			tem_strFilesPath += _T(" ");
			tem_strFilesPath += _T("\"");
			tem_strFilesPath += tem_strFilePath;	
			tem_strFilesPath += _T("\"");
		}	

		//����exe����ofd
		SHELLEXECUTEINFO tem_Info = {0};
		tem_Info.cbSize = sizeof(SHELLEXECUTEINFO);
		tem_Info.fMask = SEE_MASK_NOCLOSEPROCESS;
		tem_Info.hwnd = NULL;
		tem_Info.lpVerb = _T("Open");
		tem_Info.lpFile = _T("editor.exe");

		//ȥ��·���еĿո�
//		CString tem_strFilesPath2 = _T("\"");
//		tem_strFilesPath2 += tem_strFilesPath;
//		tem_strFilesPath2 += _T("\"");
		//���롢����ļ�·��-----------------------------------------------------
		tem_Info.lpParameters = tem_strFilesPath;

		//editor.exeȫ·��------------------------------------------------------
		TCHAR        tem_tcExePath[MAX_PATH+1] = {0};
		GetModuleFileName(NULL, tem_tcExePath, MAX_PATH);
		(_tcsrchr(tem_tcExePath, _T('\\')))[1] = 0;
		CString tem_strExePath = tem_tcExePath;
		tem_strExePath += _T("ofd\\");

		tem_Info.lpDirectory = tem_strExePath;
		tem_Info.nShow = SW_HIDE;
		tem_Info.hInstApp = NULL;
		ShellExecuteEx(&tem_Info);
	}
	
	return dstpath;
}
