#pragma once
#include "shellapi.h"   //shell头文件
#include "ximage.h"
#include "PngButton.h"
#include "Shellapi.h"
#include "CTermbReader.h"  //新身份证读卡



//创建线程
DWORD WINAPI  ThreadProc(LPVOID  lpParam);
struct ThreadInfo
{
	HWND       hWnd;      //窗口句柄用于发送消息
	int        nTime;     //定时时长，单位ms
	CString    strInfo;   //附加消息
};

typedef struct tagIDCardInfo
{
	CString    imgpath;
	CString    name;
	CString    sex;
	CString    nation;
	CString    birth;
	CString    address;
	CString    IDnumber;
	CString    office;
	CString    validate;
}IDCardInfo;


// CUDSSUBRECARD 对话框

class CUDSSUBRECARD : public CDialogEx
{
	DECLARE_DYNAMIC(CUDSSUBRECARD)

public:
	CUDSSUBRECARD(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUDSSUBRECARD();

// 对话框数据
	enum { IDD = IDD_DLG_RECARD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	IDCardInfo   m_stcIdInfo;    //身份证信息结构体
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
//	CCardReader*   m_iCard;
	CTermbReader*  m_iCard;
	CString Self_GetMyDocument(void);
	CString Self_GetSystemTime(CString  dir);

	int       m_nReadTime;         //自动读卡间隔时间

	CString   m_strInfoBuffer;     //身份证信息缓存目录
	CString   m_strPicBuffer;      //身份证头像缓存目录
	CString   m_strCardIniPath;    //身份证读卡ini文件目录
	CString   m_strIniPath;        //ini文件缓存路径

	CString Self_ReadCard(CString picpath);
	CString    m_editAddress;
	CString    m_editBirth;
	CString    m_editCardnum;
	CString    m_editName;
	CString    m_editNation;
	CString    m_editOffice;
	CString    m_editSex;
	CString    m_editValidity;
	CString    m_strProntImg;    //身份证正面模板
	CString    m_strBackImg;     //身份证背面模板
	CString    m_strCopyImg;     //身份证复印件
	CDC*       pDc;
	CDC        imageDC;
	BOOL       m_BFirstOpen;
	BOOL       m_BSaveCopy;     //保存复印件
	void Self_ShowPic(CString picpath);
	afx_msg void OnBnClickedBtnReadcard();
	int Write2Excel(CString inipath, CString xlsdir);
	afx_msg void OnClickedChkAuto();
private:
	HANDLE      hThreadHandle;      //存储线程句柄
	DWORD       dwThreadID;         //寻出线程ID
	ThreadInfo  stcThreadInfo;      //存储线程参数

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	CToolTipCtrl  m_btnSubTips;     //Button-Tips
	CPngButton    m_btnReadCard;
	CPngButton    m_btnBegin;

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	afx_msg void OnBnClickedBtnBegin();
	
	afx_msg void OnBnClickedChkSave();
	void Self_SaveCardCopy(IDCardInfo info, CString prontimg, CString backimg, CString outimg);
	void Self_ShowCopyImg(CString img);
	afx_msg void OnDblclkStaCopyimg();
	void Self_ClearImg(void);
	CStatic m_conCardImg;
	CStatic m_conCopyImg;

	//读卡与指纹
	BOOL    m_BAutoRead;
	CSliderCtrl m_conPeriod;
	afx_msg void OnCustomdrawSlidPeriod(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_StaInfo;
	int        m_nFingerNum;       //身份证包含指纹个数
	int        m_nFigerIndex;      //手指索引
	CString    m_strFingerIndex;   //定位哪根手指
	CString    m_strFingerDoc;
	CString    m_strFingerPath;
	void Self_ShowFingerImg(CString img);
	afx_msg void OnBnClickedBtnFinger();
	CString Self_ReadFigerPrint(CString imgpath);
	int        m_nOriX;
	int        m_nOriY;
	int        m_nWidth;
	int        m_nHeight;
	int WriteHead(unsigned char* Output, unsigned char* Input, int nWidth, int nHeight);
	void FPBmpDataToBmp(BYTE* FPBmpData, DWORD dataLen, char* bmpFileName);
	float Self_ExMatchFeatures(CString imgpath, CString datpath);
	afx_msg void OnClose();
	CPngButton m_btnReadFinger;
	afx_msg void OnBnClickedBtnTestdiffer();
};
