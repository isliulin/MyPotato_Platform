#pragma once


// UDSSUBHELP �Ի���

class UDSSUBHELP : public CDialogEx
{
	DECLARE_DYNAMIC(UDSSUBHELP)

public:
	UDSSUBHELP(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~UDSSUBHELP();

// �Ի�������
	enum { IDD = IDD_DLG_HELP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CStatic m_conStaInfo;
	CStatic m_conStaInfo2;
};
