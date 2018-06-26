
// UDSPlatformV1.7Dlg.h : 头文件
//

#pragma once
#include <windows.h>
#include <vector>
#include "ximage.h"
#include "Shellapi.h"
#include <algorithm>     //排序函数
#include "UDSSUBRECARD.h"
#include "HSMenu.h"
#include "HSMenuPro.h"
#include "HSMenuX.h"
#include "UDSSUBOUTPUT.h"
#include "ShellContextMenu.h"   //右键菜单
#include "PngButton.h"   
#include "UDSSUBRENAME.h"
#include "UDSSUBMERGE.h"
#include "UDSNewNode.h"
#include "UDSACK.h"
#include "UDSResizeImg.h"

using namespace std;


typedef struct tagNODEINFO 
{
	int      nodeImage;  //节点对应图像
	int      nodeID;     //节点ID
	CString  nodeName;   //节点名称
	CString  nodePath;   //节点对应路径

}NODEINFO;

// CUDSPlatformV17Dlg 对话框
class CUDSPlatformV17Dlg : public CDialogEx
{
// 构造
public:
	CUDSPlatformV17Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_UDSPLATFORMV17_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	HICON hIcon[5];           //TreeCtrl加载Icon
	CImageList  m_IcoList;    //Icon列表
	CImageList  m_ImageList;   //缩略图像列表

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	CUDSSUBRECARD    m_dlgRecard;           //读卡器界面
	CUDSSUBOUTPUT    m_dlgOutpath;          //导出文件路径
	CUDSSUBRENAME    m_dlgRename;           //批量重命名
	CUDSSUBMERGE     m_dlgMerge;            //批量合并
	CUDSNewNode      m_dlgNewNode;          //新建目录
	CUDSACK          m_dlgAck;              //确定删除目录
	CUDSResizeImg    m_dlgResize;           //批量修改图像尺寸

	COLORREF      m_clBackColor;            //窗口背景色
	COLORREF      m_clTextColor;            //字体颜色
	CImage        m_imgBK;                  //背景图像

	CTreeCtrl     m_conTreeCtrl;
	CListCtrl     m_conListCtrl;
	int           m_nRootItems;             //根目录及次级目录数量，这些目录不能删除
	int           m_nNodeID;                //节点ID信息
	NODEINFO      m_NodeInfo;               //保存每个节点信息
	int           m_nOriImg;                //缩略图显示首个图像索引
	int           m_nPageNumer;             //每页显示的图像数量
	int           m_nFileNumber;            //文件夹下图像数量
	int           m_nThumbWidth;            //缩略图宽
	int           m_nThumbHeight;           //缩略图高
	int           m_nPageIndex;             //当前显示第几页
	int           m_nPageCount;             //总页数
	int           m_nSortMode;              //文件排序方式

	CString       m_strPdfImg;              //PDF图标路径
	CString       m_strTxtImg;              //TXT图标路径
	CString       m_strDcmImg;              //DCM图标路径
	CString       m_strIniPath;             //Ini文件的路径
	CString       m_strBT;                  //标题

	BOOL          m_BSlcAll;                //全选标志位
	BOOL          m_BLButtonD;              //鼠标左键点击
	

	std::vector<NODEINFO>  m_vcNodeInfo;    //保存节点所有信息
	std::vector<CString>   m_vcDocName;     //路径下子文件夹名称
	std::vector<CString>   m_vcDocPath;     //路径下子文件夹路径
//	std::vector<CString>   m_vcFileName;    //文件名
//	std::vector<CString>   m_vcFilePath;    //图像文件路径
//	std::vector<CString>   m_vcBussPath;    //pdf+txt+图像路径 
	std::vector<int>       m_vcCtrlIndex;   //选中项索引

	std::vector<FILETIME>  m_vcCreateTime;  //文件创建时间
	std::vector<ULONGLONG> m_vcFileSize;    //文件大小，单位字节

	CHSMenu          m_mnuOutPut;          //图像导出Menu
	CHSMenu          m_mnuImgEdit;         //图像编辑Menu
	CMenu            m_mnuRTree;           //TreeCtrl右键菜单
	CHSMenu          m_mmImgSort;          //文件排序菜单


	int m_editValue;                       //Edit控件绑定变量
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
