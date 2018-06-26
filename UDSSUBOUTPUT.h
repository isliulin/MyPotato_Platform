#pragma once
#include <vector>
#include "pdflib.h"
#include "pdflib.hpp"
#include "tiff.h"
#include "tiffio.h"
#include "ximatif.h"
#include <stdlib.h>    //CString->const char* wcstombs_s头文件
#include "PngButton.h"

using namespace std;
using namespace pdflib;

// CUDSSUBOUTPUT 对话框

class CUDSSUBOUTPUT : public CDialogEx
{
	DECLARE_DYNAMIC(CUDSSUBOUTPUT)

public:
	CUDSSUBOUTPUT(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUDSSUBOUTPUT();

// 对话框数据
	enum { IDD = IDD_DLG_OUTPUTIMG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	CImage     m_imgSubBK;           //背景图像

	CListCtrl  m_conListFiles;
	int        m_nRows;
	int        m_nSlctFile;           //选中文件索引
	BOOL       m_BOutput;            //是否导出
	BOOL       m_BPdfTif;            //用于判断当前导出的是PDF还是多页TIF
	BOOL       m_BOFD;               //用于判断当前导出是否为OFD
	CString    m_strFileName;        //根据文件路径，以最下层文件夹名命名文件
	CString    m_strPDFPath;         //默认PDF输出路径
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



	int  m_nDragIndex;    //拖拽项的索引值
	afx_msg void OnBnClickedBtnUp();
	afx_msg void OnBnClickedBtnDown();
	CPngButton    m_btnDown;
	CPngButton    m_btnUp;
	CString Self_GenerateOFD(vector<CString> imgspath, CString dstpath);
};
