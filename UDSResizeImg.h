#pragma once


// CUDSResizeImg 对话框

class CUDSResizeImg : public CDialogEx
{
	DECLARE_DYNAMIC(CUDSResizeImg)

public:
	CUDSResizeImg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUDSResizeImg();

// 对话框数据
	enum { IDD = IDD_DLG_RESIZEIMG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
