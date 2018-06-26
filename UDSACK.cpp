// UDSACK.cpp : 实现文件
//

#include "stdafx.h"
#include "UDSPlatformV1.7.h"
#include "UDSACK.h"
#include "afxdialogex.h"


// CUDSACK 对话框

IMPLEMENT_DYNAMIC(CUDSACK, CDialogEx)

CUDSACK::CUDSACK(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUDSACK::IDD, pParent)
{

}

CUDSACK::~CUDSACK()
{
}

void CUDSACK::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUDSACK, CDialogEx)
END_MESSAGE_MAP()


// CUDSACK 消息处理程序


BOOL CUDSACK::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	
	tem_CurFont.CreatePointFont(160, _T("楷体"));
	GetDlgItem(IDC_STA_ACK)->SetFont(&tem_CurFont);
	GetDlgItem(IDC_STA_ACK)->SetWindowText(_T("    确定删除该目录、子目录及所有文件？"));


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
