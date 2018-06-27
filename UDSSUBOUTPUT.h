#pragma once
#include <vector>
#include "pdflib.h"
#include "pdflib.hpp"
#include "tiff.h"
#include "tiffio.h"
#include "ximatif.h"
#include <stdlib.h>    //CString->const char* wcstombs_sͷ�ļ�
#include "PngButton.h"

using namespace std;
using namespace pdflib;

// CUDSSUBOUTPUT �Ի���

class CUDSSUBOUTPUT : public CDialogEx
{
	DECLARE_DYNAMIC(CUDSSUBOUTPUT)

public:
	CUDSSUBOUTPUT(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUDSSUBOUTPUT();

// �Ի�������
	enum { IDD = IDD_DLG_OUTPUTIMG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	CImage     m_imgSubBK;           //����ͼ��

	CListCtrl  m_conListFiles;
	int        m_nRows;
	int        m_nSlctFile;           //ѡ���ļ�����
	BOOL       m_BOutput;            //�Ƿ񵼳�
	BOOL       m_BPdfTif;            //�����жϵ�ǰ��������PDF���Ƕ�ҳTIF
	BOOL       m_BOFD;               //�����жϵ�ǰ�����Ƿ�ΪOFD
	CString    m_strFileName;        //�����ļ�·���������²��ļ����������ļ�
	CString    m_strPDFPath;         //Ĭ��PDF���·��
	CEdit      m_conEditPath;
	afx_msg void OnBnClickedBtnSltdoc();
	afx_msg void OnBnClickedBtnAddfile();
	afx_msg void OnBnClickedBtnDltfile();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClickListFiles(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnChangeEditOutpath();

	CToolTipCtrl    m_btnSubTips;    //Button-Tips

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CPngButton      m_btnAddItem;
	CPngButton      m_btnDltItem;
	CPngButton      m_btnOK;
	CPngButton      m_btnCancel;
	CPngButton      m_btnSlcDoc;
	afx_msg void OnBegindragListFiles(NMHDR *pNMHDR, LRESULT *pResult);



	int  m_nDragIndex;    //��ק�������ֵ
	afx_msg void OnBnClickedBtnUp();
	afx_msg void OnBnClickedBtnDown();
	CPngButton    m_btnDown;
	CPngButton    m_btnUp;
	CString Self_GenerateOFD(vector<CString> imgspath, CString dstpath);
};
