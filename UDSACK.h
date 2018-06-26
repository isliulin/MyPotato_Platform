#pragma once


// CUDSACK 对话框

class CUDSACK : public CDialogEx
{
	DECLARE_DYNAMIC(CUDSACK)

public:
	CUDSACK(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUDSACK();

// 对话框数据
	enum { IDD = IDD_DLG_ACK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CFont  tem_CurFont;
};
