#pragma once


// CUDSACK �Ի���

class CUDSACK : public CDialogEx
{
	DECLARE_DYNAMIC(CUDSACK)

public:
	CUDSACK(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUDSACK();

// �Ի�������
	enum { IDD = IDD_DLG_ACK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CFont  tem_CurFont;
};
