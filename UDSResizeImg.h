#pragma once


// CUDSResizeImg �Ի���

class CUDSResizeImg : public CDialogEx
{
	DECLARE_DYNAMIC(CUDSResizeImg)

public:
	CUDSResizeImg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUDSResizeImg();

// �Ի�������
	enum { IDD = IDD_DLG_RESIZEIMG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
