#pragma once
#include <vector>
#include "ximage.h"
#include "UDSSUBHELP.h"

// CUDSSUBMERGE 对话框

class CUDSSUBMERGE : public CDialogEx
{
	DECLARE_DYNAMIC(CUDSSUBMERGE)

public:
	CUDSSUBMERGE(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUDSSUBMERGE();

// 对话框数据
	enum { IDD = IDD_DLG_MERGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	UDSSUBHELP   m_dlgHelp;    //帮助窗口

	CListCtrl m_conListMerge;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	CString Self_MergeImages(CString img1, CString img2, int site, int mode, BOOL saved, int name, CString dir);
	int Self_GetTypeFromFileName(LPCTSTR pstr);
	afx_msg void OnClickedChkLeft();
	afx_msg void OnClickedChkUp();


	int     m_nMergeSite;     //合并位置
	int     m_nMergeMode;     //合并方式
	int     m_nSaveType;      //保存图像类型

	BOOL    m_BSaved;         //保存原图

	std::vector<int>      m_vcMergeIndex;   //待合并图像索引
	std::vector<CString>  m_vcImageName;    //图像文件名
	std::vector<CString>  m_vcImagePath;    //图像文件路径

	afx_msg void OnBnClickedOk();
	afx_msg void OnClickedChkLinye();
	afx_msg void OnClickedChkGeye();
	afx_msg void OnClickedChkQianhou();
	afx_msg void OnClickedChkJpg();
	afx_msg void OnClickedChkBmp();
	afx_msg void OnClickedChkPng();
	afx_msg void OnClickedChkTif();
	afx_msg void OnClickedChkSave();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClickedChkAllslc();
	CString m_StaSlcNumer;
	afx_msg void OnItemchangedListMerge(NMHDR *pNMHDR, LRESULT *pResult);
	CProgressCtrl m_conProMerge;
	afx_msg void OnBnClickedBtnHelp();
	CString m_StaMergeDid;
};
