// UDSSUBHELP.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UDSPlatformV1.7.h"
#include "UDSSUBHELP.h"
#include "afxdialogex.h"


// UDSSUBHELP �Ի���

IMPLEMENT_DYNAMIC(UDSSUBHELP, CDialogEx)

UDSSUBHELP::UDSSUBHELP(CWnd* pParent /*=NULL*/)
	: CDialogEx(UDSSUBHELP::IDD, pParent)
{

}

UDSSUBHELP::~UDSSUBHELP()
{
}

void UDSSUBHELP::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STA_HELPINFO, m_conStaInfo);
	DDX_Control(pDX, IDC_STA_HELPINFO2, m_conStaInfo2);
}


BEGIN_MESSAGE_MAP(UDSSUBHELP, CDialogEx)
END_MESSAGE_MAP()


// UDSSUBHELP ��Ϣ�������


BOOL UDSSUBHELP::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CFont   tem_font;
	
//	tem_font.CreateFont(-15/*�߶�*/, 
//		                -7.5/*���*/, 
//						0/*���ù�*/, 
//						0/*���ù�*/, 
//						400 /*һ�����ֵ��Ϊ400*/,
//		                FALSE/*����б��*/, 
//						FALSE/*�����»���*/, 
//						FALSE/*����ɾ����*/,
//		                DEFAULT_CHARSET,  //��������ʹ��Ĭ���ַ��������������� _CHARSET ��β�ĳ�������
//		                OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,  //���в������ù�
//		                DEFAULT_QUALITY,  //Ĭ���������
//		                FF_DONTCARE,  //��ָ��������*/
//		                L"΢���ź�"  //������
//		                );
//	m_conStaInfo.SetWindowText(_T("������Ϣ"));
//	m_conStaInfo.SetFont(&tem_font);
	


	tem_font.CreateFont(-15/*�߶�*/, 
		                -7.5/*���*/, 
		                0/*���ù�*/, 
		                0/*���ù�*/, 
		                400 /*һ�����ֵ��Ϊ400*/,
		                FALSE/*����б��*/, 
		                FALSE/*�����»���*/, 
		                FALSE/*����ɾ����*/,
		                DEFAULT_CHARSET,  //��������ʹ��Ĭ���ַ��������������� _CHARSET ��β�ĳ�������
		                OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,  //���в������ù�
		                DEFAULT_QUALITY,  //Ĭ���������
		                FF_DONTCARE,  //��ָ��������*/
		                L"΢���ź�"  //������
		                );
	CString   tem_strHelpInfo = _T("������Ϣ��\n    һ����ҳ�ϲ�: ������ҳ�ϲ�����ͼ���б�Ϊ1��2,3,4,5,6...����ϲ���Ϊ(1,2)��(3,4)��(5,6)...\n    ������ҳ�ϲ�����ͼ���б�Ϊ1,2,3,4,5,6,7,8����ϲ���Ϊ(1,3)��(2,4)��(5,7)��(6,8)\n    ����ǰ��ϲ�����ͼ������Ϊ1,2,3,4,5,6,7,8,��ϲ���Ϊ(1,5)��(2,6)��(3,7)(4,8)\n    �ġ�����λ�ã���ǰĿ¼�µ�<�ϲ�ͼ>�ļ��С�");
	m_conStaInfo2.SetWindowText(tem_strHelpInfo);
	m_conStaInfo2.SetFont(&tem_font);

	DeleteObject(tem_font);


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
