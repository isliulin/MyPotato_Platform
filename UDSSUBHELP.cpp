// UDSSUBHELP.cpp : 实现文件
//

#include "stdafx.h"
#include "UDSPlatformV1.7.h"
#include "UDSSUBHELP.h"
#include "afxdialogex.h"


// UDSSUBHELP 对话框

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


// UDSSUBHELP 消息处理程序


BOOL UDSSUBHELP::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CFont   tem_font;
	
//	tem_font.CreateFont(-15/*高度*/, 
//		                -7.5/*宽度*/, 
//						0/*不用管*/, 
//						0/*不用管*/, 
//						400 /*一般这个值设为400*/,
//		                FALSE/*不带斜体*/, 
//						FALSE/*不带下划线*/, 
//						FALSE/*不带删除线*/,
//		                DEFAULT_CHARSET,  //这里我们使用默认字符集，还有其他以 _CHARSET 结尾的常量可用
//		                OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,  //这行参数不用管
//		                DEFAULT_QUALITY,  //默认输出质量
//		                FF_DONTCARE,  //不指定字体族*/
//		                L"微软雅黑"  //字体名
//		                );
//	m_conStaInfo.SetWindowText(_T("帮助信息"));
//	m_conStaInfo.SetFont(&tem_font);
	


	tem_font.CreateFont(-15/*高度*/, 
		                -7.5/*宽度*/, 
		                0/*不用管*/, 
		                0/*不用管*/, 
		                400 /*一般这个值设为400*/,
		                FALSE/*不带斜体*/, 
		                FALSE/*不带下划线*/, 
		                FALSE/*不带删除线*/,
		                DEFAULT_CHARSET,  //这里我们使用默认字符集，还有其他以 _CHARSET 结尾的常量可用
		                OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS,  //这行参数不用管
		                DEFAULT_QUALITY,  //默认输出质量
		                FF_DONTCARE,  //不指定字体族*/
		                L"微软雅黑"  //字体名
		                );
	CString   tem_strHelpInfo = _T("帮助信息：\n    一、邻页合并: 即相邻页合并，如图像列表为1，2,3,4,5,6...，则合并后为(1,2)、(3,4)、(5,6)...\n    二、隔页合并：如图像列表为1,2,3,4,5,6,7,8，则合并后为(1,3)、(2,4)、(5,7)、(6,8)\n    三、前后合并：如图像序列为1,2,3,4,5,6,7,8,则合并后为(1,5)、(2,6)、(3,7)(4,8)\n    四、保存位置：当前目录下的<合并图>文件夹。");
	m_conStaInfo2.SetWindowText(tem_strHelpInfo);
	m_conStaInfo2.SetFont(&tem_font);

	DeleteObject(tem_font);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
