#pragma once


// CUDSNewNode �Ի���

class CUDSNewNode : public CDialogEx
{
	DECLARE_DYNAMIC(CUDSNewNode)

public:
	CUDSNewNode(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUDSNewNode();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CString m_editValue;
	virtual BOOL OnInitDialog();
};
