// UDSResizeImg.cpp : 实现文件
//

#include "stdafx.h"
#include "UDSPlatformV1.7.h"
#include "UDSResizeImg.h"
#include "afxdialogex.h"


// CUDSResizeImg 对话框

IMPLEMENT_DYNAMIC(CUDSResizeImg, CDialogEx)

CUDSResizeImg::CUDSResizeImg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUDSResizeImg::IDD, pParent)
{

}

CUDSResizeImg::~CUDSResizeImg()
{
}

void CUDSResizeImg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUDSResizeImg, CDialogEx)
END_MESSAGE_MAP()


// CUDSResizeImg 消息处理程序
