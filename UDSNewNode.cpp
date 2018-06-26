// UDSNewNode.cpp : 实现文件
//

#include "stdafx.h"
#include "UDSPlatformV1.7.h"
#include "UDSNewNode.h"
#include "afxdialogex.h"


extern CString   g_strNodePath;  

// CUDSNewNode 对话框

IMPLEMENT_DYNAMIC(CUDSNewNode, CDialogEx)

CUDSNewNode::CUDSNewNode(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUDSNewNode::IDD, pParent)
{

	m_editValue = _T("");
}

CUDSNewNode::~CUDSNewNode()
{
}

void CUDSNewNode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_EDIT_NEWNODE, m_editNodeName);
	DDX_Text(pDX, IDC_EDIT_NEWNODE, m_editValue);
}


BEGIN_MESSAGE_MAP(CUDSNewNode, CDialogEx)
	ON_BN_CLICKED(IDOK, &CUDSNewNode::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CUDSNewNode::OnBnClickedCancel)
END_MESSAGE_MAP()


BOOL CUDSNewNode::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_editValue = _T("");
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
// CUDSNewNode 消息处理程序


void CUDSNewNode::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	
	int tem_nLength = m_editValue.GetLength();
	//判断输入文件名是否合法
	if (m_editValue.IsEmpty())
	{
		MessageBox(_T("输入目录名称为空！"), _T("新建目录"), MB_OK);
	}
	else if (m_editValue.Find('/')>=0 || m_editValue.Find('\\')>=0 || m_editValue.Find('|')>=0 ||
		m_editValue.Find(':')>=0 || m_editValue.Find('\?')>=0 || m_editValue.Find('\"')>=0 ||
		m_editValue.Find('*')>=0 || m_editValue.Find('<')>=0 || m_editValue.Find('<')>=0)
	{
		MessageBox(_T("输入目录名称含有非法字符！"), _T("新建目录"), MB_OK);
	}
	
	else
	{
		g_strNodePath = m_editValue;
	
		CDialogEx::OnOK();
	}
	
}


void CUDSNewNode::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


