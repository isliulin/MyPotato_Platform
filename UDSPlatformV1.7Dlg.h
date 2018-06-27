
// UDSPlatformV1.7Dlg.h : ͷ�ļ�
//

#pragma once
#include <windows.h>
#include <vector>
#include "ximage.h"
#include "Shellapi.h"
#include <algorithm>     //������
#include "UDSSUBRECARD.h"
#include "HSMenu.h"
#include "HSMenuPro.h"
#include "HSMenuX.h"
#include "UDSSUBOUTPUT.h"
#include "ShellContextMenu.h"   //�Ҽ��˵�
#include "PngButton.h"   
#include "UDSSUBRENAME.h"
#include "UDSSUBMERGE.h"
#include "UDSNewNode.h"
#include "UDSACK.h"
#include "UDSResizeImg.h"

using namespace std;


typedef struct tagNODEINFO 
{
	int      nodeImage;  //�ڵ��Ӧͼ��
	int      nodeID;     //�ڵ�ID
	CString  nodeName;   //�ڵ�����
	CString  nodePath;   //�ڵ��Ӧ·��

}NODEINFO;

// CUDSPlatformV17Dlg �Ի���
class CUDSPlatformV17Dlg : public CDialogEx
{
// ����
public:
	CUDSPlatformV17Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_UDSPLATFORMV17_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	HICON hIcon[5];           //TreeCtrl����Icon
	CImageList  m_IcoList;    //Icon�б�
	CImageList  m_ImageList;   //����ͼ���б�

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	CUDSSUBRECARD    m_dlgRecard;           //����������
	CUDSSUBOUTPUT    m_dlgOutpath;          //�����ļ�·��
	CUDSSUBRENAME    m_dlgRename;           //����������
	CUDSSUBMERGE     m_dlgMerge;            //�����ϲ�
	CUDSNewNode      m_dlgNewNode;          //�½�Ŀ¼
	CUDSACK          m_dlgAck;              //ȷ��ɾ��Ŀ¼
	CUDSResizeImg    m_dlgResize;           //�����޸�ͼ��ߴ�

	COLORREF      m_clBackColor;            //���ڱ���ɫ
	COLORREF      m_clTextColor;            //������ɫ
	CImage        m_imgBK;                  //����ͼ��

	CTreeCtrl     m_conTreeCtrl;
	CListCtrl     m_conListCtrl;
	int           m_nRootItems;             //��Ŀ¼���μ�Ŀ¼��������ЩĿ¼����ɾ��
	int           m_nNodeID;                //�ڵ�ID��Ϣ
	NODEINFO      m_NodeInfo;               //����ÿ���ڵ���Ϣ
	int           m_nOriImg;                //����ͼ��ʾ�׸�ͼ������
	int           m_nPageNumer;             //ÿҳ��ʾ��ͼ������
	int           m_nFileNumber;            //�ļ�����ͼ������
	int           m_nThumbWidth;            //����ͼ��
	int           m_nThumbHeight;           //����ͼ��
	int           m_nPageIndex;             //��ǰ��ʾ�ڼ�ҳ
	int           m_nPageCount;             //��ҳ��
	int           m_nSortMode;              //�ļ�����ʽ

	CString       m_strPdfImg;              //PDFͼ��·��
	CString       m_strTxtImg;              //TXTͼ��·��
	CString       m_strDcmImg;              //DCMͼ��·��
	CString       m_strIniPath;             //Ini�ļ���·��
	CString       m_strBT;                  //����

	BOOL          m_BSlcAll;                //ȫѡ��־λ
	BOOL          m_BLButtonD;              //���������
	

	std::vector<NODEINFO>  m_vcNodeInfo;    //����ڵ�������Ϣ
	std::vector<CString>   m_vcDocName;     //·�������ļ�������
	std::vector<CString>   m_vcDocPath;     //·�������ļ���·��
//	std::vector<CString>   m_vcFileName;    //�ļ���
//	std::vector<CString>   m_vcFilePath;    //ͼ���ļ�·��
//	std::vector<CString>   m_vcBussPath;    //pdf+txt+ͼ��·�� 
	std::vector<int>       m_vcCtrlIndex;   //ѡ��������

	std::vector<FILETIME>  m_vcCreateTime;  //�ļ�����ʱ��
	std::vector<ULONGLONG> m_vcFileSize;    //�ļ���С����λ�ֽ�

	CHSMenu          m_mnuOutPut;          //ͼ�񵼳�Menu
	CHSMenu          m_mnuImgEdit;         //ͼ��༭Menu
	CMenu            m_mnuRTree;           //TreeCtrl�Ҽ��˵�
	CHSMenu          m_mmImgSort;          //�ļ�����˵�


	int m_editValue;                       //Edit�ؼ��󶨱���
	BOOL Self_AddNodeInfo(int id, int image, CString name, CString path);

	CString Self_GetMyDocument(void);
	CString Self_GetDiskInfo(void);
	void Self_AddDiskItem(CString diskname);
	int Self_FindDocs(CString dirname);
	afx_msg void OnSelchangedTreeDir(NMHDR *pNMHDR, LRESULT *pResult);
	CString Self_GetItemInfo(HTREEITEM item, int infoindex);
	int Self_FindAllFiles(CString dirpath);
	int Self_GetTypeFromFileName(LPCTSTR pstr);
	BOOL Self_ThumbailList(int thumbwidth, int thumbheight, int origin, int number);
	afx_msg void OnBnClickedBtnLast();
	afx_msg void OnBnClickedBtnNext();
	afx_msg void OnBnClickedBtnRotatel();
	afx_msg void OnBnClickedBtnRotater();
	afx_msg void OnBnClickedBtnDelete();
	afx_msg void OnClickedChkAllslct();
	afx_msg void OnClickListImg(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHoverListImg(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL Self_ReplaceImage(int thumbwidth, int thumbheight, int item);
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnCamera();
	afx_msg void OnBnClickedBtnScanner();
	int Self_UpdaPagination(int filecount, BOOL mode);
	BOOL Self_DeleteFile2Recycle(CString filepath);
	afx_msg void OnBnClickedBtnRecard();
	BOOL CreateHSMenu(void);
	afx_msg void OnBnClickedBtnOutput();
	afx_msg void OnBnClickedBtnProcess();
	afx_msg void OnPrintImgs();
	afx_msg void OnCopyImgs();
	afx_msg void OnPDFImgs();
	afx_msg void OnTIFImgs();
	afx_msg void OnBrush();
	afx_msg void OnProcess();
	afx_msg void OnDblclkListImg(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnChangeEditNowpage();
	afx_msg void OnChangeEditSearch();
protected:
	afx_msg LRESULT OnSettext(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnRclickListImg(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEndlabeleditListImg(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_conSlidPage;
	afx_msg void OnCustomdrawSlidPagenum(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnReleasedcaptureSlidPagenum(NMHDR *pNMHDR, LRESULT *pResult);
	int Self_UpdataPageCur(int pagenum, int firstindex);
	void Self_HightLightItems(void);
	void Self_InverHighLightItems(void);
	void Self_CalculatThumb(int pagenum);
	afx_msg void OnItemchangedListImg(NMHDR *pNMHDR, LRESULT *pResult);

	CToolTipCtrl     m_btnTips;     //Button-Tips
	CToolTipCtrl     m_btnTips2;
	CPngButton       m_btnCamera;
	CPngButton       m_btnScanner;
	CPngButton       m_btnRecard;
	CPngButton       m_btnOutput;
	CPngButton       m_btnProcess;
	CPngButton       m_btnRotateL;
	CPngButton       m_btnRotateR;
	CPngButton       m_btnSearch;
	CPngButton       m_btnDelete;
	CPngButton       m_btnLast;
	CPngButton       m_btnNext;
	CPngButton       m_btn_Help;

	afx_msg void OnBatchRename();
	afx_msg void OnBatchMerge();
	afx_msg void OnDblclkStaBt();
	afx_msg void OnBnClickedBtnHelp();
	
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRclickTreeDir(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void On32779NewNode();
	afx_msg void On32780DeleteNode();
	void Self_FileSort(int mode);
	afx_msg void OnBnClickedBtnSort();
	afx_msg void On32781();
	afx_msg void On32782();
	afx_msg void On32783();
	afx_msg void On32784();
	afx_msg void On32785();
	CPngButton m_btnSort;
	afx_msg void On32786();
	COLORREF   m_bkColor;
	afx_msg void On32787();
	afx_msg void On32788();
	void Self_HightLightItems2(void);
	afx_msg void OnCustomdrawListImg(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnKillfocusListImg(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSetfocusListImg(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void On32791();
	afx_msg void On32790();
	afx_msg void On32793();
};
