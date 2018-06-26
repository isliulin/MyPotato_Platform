#pragma once
#include <vector>



// CUDSSUBRENAME 对话框

class CUDSSUBRENAME : public CDialogEx
{
	DECLARE_DYNAMIC(CUDSSUBRENAME)

public:
	CUDSSUBRENAME(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUDSSUBRENAME();

// 对话框数据
	enum { IDD = IDD_DLG_RENAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

	CListCtrl m_conListRename;
	CComboBox m_conPrename;
	CString   m_EditFirstKey;
	CString   m_EditSecondKey;
	CString   m_EditThirdKey;
    int       m_EditLastName;
	CString   m_EditSeparator;

	CString   m_strPreName;       //前缀
	CString   m_strFirstKey;      //字段1
	CString   m_strSecondKey;     //字段2
	CString   m_strThirdKey;      //字段3
	int       m_nLastName;
	CString   m_strLastName;      //末尾字段
	CString   m_strSeparator;     //分隔符

	std::vector<CString>  m_vcNewFileName;
	std::vector<CString>  m_vcNewFilePath;
	std::vector<CString>  m_vcNewBussPath;
	
	afx_msg void OnSelchangeComFirst();
	afx_msg void OnChangeEditSecond();
	afx_msg void OnChangeEditThird();
	afx_msg void OnChangeEditFouth();
	afx_msg void OnChangeEditLast();
	CString Self_SaveNewName(int index, CString filename);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CString Self_GetNewName(void);
	afx_msg void OnChangeEditSeparator();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClickedChkAllst();
	CString m_StaSlctdNumber;
	afx_msg void OnItemchangedListRename(NMHDR *pNMHDR, LRESULT *pResult);
	CProgressCtrl m_conProRename;
	CString m_StaDidRatio;
};
