// UDSACK.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDSPlatformV1.7.h"
#include "UDSACK.h"
#include "afxdialogex.h"


// CUDSACK �Ի���

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


// CUDSACK ��Ϣ�������


BOOL CUDSACK::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	
	tem_CurFont.CreatePointFont(160, _T("����"));
	GetDlgItem(IDC_STA_ACK)->SetFont(&tem_CurFont);
	GetDlgItem(IDC_STA_ACK)->SetWindowText(_T("    ȷ��ɾ����Ŀ¼����Ŀ¼�������ļ���"));


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
