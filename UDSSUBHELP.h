#pragma once


// UDSSUBHELP 对话框

class UDSSUBHELP : public CDialogEx
{
	DECLARE_DYNAMIC(UDSSUBHELP)

public:
	UDSSUBHELP(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~UDSSUBHELP();

// 对话框数据
	enum { IDD = IDD_DLG_HELP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CStatic m_conStaInfo;
	CStatic m_conStaInfo2;
};
