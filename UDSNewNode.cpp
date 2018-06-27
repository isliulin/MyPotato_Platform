// UDSNewNode.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDSPlatformV1.7.h"
#include "UDSNewNode.h"
#include "afxdialogex.h"


extern CString   g_strNodePath;  

// CUDSNewNode �Ի���

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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_editValue = _T("");
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
// CUDSNewNode ��Ϣ�������


void CUDSNewNode::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	
	int tem_nLength = m_editValue.GetLength();
	//�ж������ļ����Ƿ�Ϸ�
	if (m_editValue.IsEmpty())
	{
		MessageBox(_T("����Ŀ¼����Ϊ�գ�"), _T("�½�Ŀ¼"), MB_OK);
	}
	else if (m_editValue.Find('/')>=0 || m_editValue.Find('\\')>=0 || m_editValue.Find('|')>=0 ||
		m_editValue.Find(':')>=0 || m_editValue.Find('\?')>=0 || m_editValue.Find('\"')>=0 ||
		m_editValue.Find('*')>=0 || m_editValue.Find('<')>=0 || m_editValue.Find('<')>=0)
	{
		MessageBox(_T("����Ŀ¼���ƺ��зǷ��ַ���"), _T("�½�Ŀ¼"), MB_OK);
	}
	
	else
	{
		g_strNodePath = m_editValue;
	
		CDialogEx::OnOK();
	}
	
}


void CUDSNewNode::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


