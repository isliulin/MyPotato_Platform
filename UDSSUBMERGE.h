#pragma once
#include <vector>
#include "ximage.h"
#include "UDSSUBHELP.h"

// CUDSSUBMERGE �Ի���

class CUDSSUBMERGE : public CDialogEx
{
	DECLARE_DYNAMIC(CUDSSUBMERGE)

public:
	CUDSSUBMERGE(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUDSSUBMERGE();

// �Ի�������
	enum { IDD = IDD_DLG_MERGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	UDSSUBHELP   m_dlgHelp;    //��������

	CListCtrl m_conListMerge;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	CString Self_MergeImages(CString img1, CString img2, int site, int mode, BOOL saved, int name, CString dir);
	int Self_GetTypeFromFileName(LPCTSTR pstr);
	afx_msg void OnClickedChkLeft();
	afx_msg void OnClickedChkUp();


	int     m_nMergeSite;     //�ϲ�λ��
	int     m_nMergeMode;     //�ϲ���ʽ
	int     m_nSaveType;      //����ͼ������

	BOOL    m_BSaved;         //����ԭͼ

	std::vector<int>      m_vcMergeIndex;   //���ϲ�ͼ������
	std::vector<CString>  m_vcImageName;    //ͼ���ļ���
	std::vector<CString>  m_vcImagePath;    //ͼ���ļ�·��

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
