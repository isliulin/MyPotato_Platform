#pragma once
#include "shellapi.h"   //shellͷ�ļ�
#include "ximage.h"
#include "PngButton.h"
#include "Shellapi.h"
#include "CTermbReader.h"  //�����֤����



//�����߳�
DWORD WINAPI  ThreadProc(LPVOID  lpParam);
struct ThreadInfo
{
	HWND       hWnd;      //���ھ�����ڷ�����Ϣ
	int        nTime;     //��ʱʱ������λms
	CString    strInfo;   //������Ϣ
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


// CUDSSUBRECARD �Ի���

class CUDSSUBRECARD : public CDialogEx
{
	DECLARE_DYNAMIC(CUDSSUBRECARD)

public:
	CUDSSUBRECARD(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUDSSUBRECARD();

// �Ի�������
	enum { IDD = IDD_DLG_RECARD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	IDCardInfo   m_stcIdInfo;    //���֤��Ϣ�ṹ��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
//	CCardReader*   m_iCard;
	CTermbReader*  m_iCard;
	CString Self_GetMyDocument(void);
	CString Self_GetSystemTime(CString  dir);

	int       m_nReadTime;         //�Զ��������ʱ��

	CString   m_strInfoBuffer;     //���֤��Ϣ����Ŀ¼
	CString   m_strPicBuffer;      //���֤ͷ�񻺴�Ŀ¼
	CString   m_strCardIniPath;    //���֤����ini�ļ�Ŀ¼
	CString   m_strIniPath;        //ini�ļ�����·��

	CString Self_ReadCard(CString picpath);
	CString    m_editAddress;
	CString    m_editBirth;
	CString    m_editCardnum;
	CString    m_editName;
	CString    m_editNation;
	CString    m_editOffice;
	CString    m_editSex;
	CString    m_editValidity;
	CString    m_strProntImg;    //���֤����ģ��
	CString    m_strBackImg;     //���֤����ģ��
	CString    m_strCopyImg;     //���֤��ӡ��
	CDC*       pDc;
	CDC        imageDC;
	BOOL       m_BFirstOpen;
	BOOL       m_BSaveCopy;     //���渴ӡ��
	void Self_ShowPic(CString picpath);
	afx_msg void OnBnClickedBtnReadcard();
	int Write2Excel(CString inipath, CString xlsdir);
	afx_msg void OnClickedChkAuto();
private:
	HANDLE      hThreadHandle;      //�洢�߳̾��
	DWORD       dwThreadID;         //Ѱ���߳�ID
	ThreadInfo  stcThreadInfo;      //�洢�̲߳���

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

	//������ָ��
	BOOL    m_BAutoRead;
	CSliderCtrl m_conPeriod;
	afx_msg void OnCustomdrawSlidPeriod(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_StaInfo;
	int        m_nFingerNum;       //���֤����ָ�Ƹ���
	int        m_nFigerIndex;      //��ָ����
	CString    m_strFingerIndex;   //��λ�ĸ���ָ
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
