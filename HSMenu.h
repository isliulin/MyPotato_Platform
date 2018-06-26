// HSMenu.h: interface for the CHSMenu class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HSMENU_H__4142ADCE_9B31_11D6_879C_D3DBCFA74D6E__INCLUDED_)
#define AFX_HSMENU_H__4142ADCE_9B31_11D6_879C_D3DBCFA74D6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include	<string>
#include	<deque>
//using namespace std;



////////////////////////////////     CLASS   <<< HSITEM >>      ////////////////////////////

#define			ITEM_SEPARATOR				MF_SEPARATOR	//0x00000800		// Menu Item is Separator
#define			ITEM_GRAY					ODS_GRAYED		//0x00000002		// Menu Item Disable Can Not Select
#define			ITEM_DISABLE				ODS_DISABLED	//0x00000004		// Menu Item Disable Do Not Display in Menu
#define			ITEM_ENABLE					0x00000008							// Enable Menu Item

#define			ITEM_POPUP					0x00000010
#define			ITEM_NORMAL					0x00000000
#define			ITEM_XXX					0X00000FFF

//////////////////////////////////////////////////////////////////////////////////////////

class CHSMenu;

class HSITEM  
{
	friend CHSMenu;

	LPVOID			m_pData;								// Item Icon Handle
	UINT			m_uID;									// Item ID
	UINT			m_uFlags;								// Item Flags 
	int				m_nTop;									// Item Top point In Menu Window
	int				m_nHeight;
	CHSMenu*	    m_pPopupHSMenu;							// The SubMenu Of This Item's
	std::string		m_strName;								// Save Item Name, Display in Menu

public:
//			 HSITEM( const HSITEM& MenuItem );
	inline   HSITEM();

//	HSITEM&	operator=( const HSITEM& MenuItem );

//protected:
//	void	Assign( const HSITEM& MenuItem );
};

//////////////////////////////////////////////////////////////////////////////////////////

typedef struct tagDRAWITEMSTRUCTPRO
{
	HDC		hDC;
	UINT	uFlags;
	LPCSTR	lpcStr;
	LPVOID	pData;
	RECT	rect;
} DRAWITEMSTRUCTPRO, *LPDRAWITEMSTRUCTPRO;

//////////////////////////////////////////////////////////////////////////////////////////

#define			MENU_NORMAL			0X00000000				// 
#define			MENU_POPUPED		0X80000000				// Current Menu has Popup a Submenu
#define			MENU_CHILD			0X40000000				// Current Menu has a Parent Menu
#define			MENU_PARENT			0X20000000				// Current Menu has Submenus
#define			MENU_ROOT			0X10000000				// Current Menu No Parent menu
#define			MENU_SELECTED		0X08000000				// Draw a Selected Item Backgroung And Text ...
#define			MENU_TIMERTRACK		0X04000000				// Timer Seted, When End Delay Popup Submenu
#define			MENU_SELBORDER		0X02000000
#define			MENU_TIMERDESTROY	0X01000000
#define			MENU_OWNERDRAW		0X00800000
#define			MENU_DESTROYED		0X00400000
#define			MENU_XXXX			0XFFFF0000

#define			SELNONE				-1
#define			FINDNONE			-5
#define			MENUCAPTION			0
#define			TIMERTRACK			1
#define			TIMERDESTROY		2

//#define			CLR_BORDERSEL		0					// Select Item Border Color
#define			CLR_BKGNDSEL		0						// Background of The Selected Item		
#define			CLR_BKGNDMENU		1						// Normal Menu Background
#define			CLR_BORDERMENU		2
#define			CLR_BORDERSEL		3
#define			CLR_BORDERSPR		4						// Select Item Border Color
#define			CLR_TEXTSEL			5						// Select Item Text Color		
#define			CLR_TEXTNORMAL		6						// Normal Text Color	
#define			CLR_ARROWSEL		7
//#define			CLR_TEXTGRAY		5					// Gray Text Color
#define			CLRCOUNT_BASE		8

typedef std::deque<HSITEM>	DEQUEITEM;

class CHSMenu
{
	friend static LRESULT CALLBACK lpfnWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

public:
	static int		GetSeparatorHeight	( void );
	static int		GetMenuItemHeight	( void );
	static int		GetMenuPopDelay		( void );
	static COLORREF	GetColor			( int nIndex );

	static void		SetColor			( int nIndex, COLORREF clrSet );
	static void		SetMenuFont			( HFONT hFont );
	static void		SetMenuEdge			( int nEdge );
	static void		SetMenuPopDelay		( int nDelay );
	static void		SetSeparatorHeight	( int nHeight );
	static void		SetMenuItemHeight	( int nHeight );

	int				GetMenuEdge			( void ) const;
	int				GetCurSelItem		( void ) const;
	int				GetMenuItemCount	( void ) const;
	int				GetMenuPosition		( UINT uID ) const;
	void			GetMenuCXY			( int& nMenuWidth, int& nMenuHeight );
	UINT			GetMenuID			( int nPosition ) const;
	UINT			GetMenuState		( int nItem, UINT uFlag = MF_BYPOSITION ) const;
	CHSMenu*		GetSubMenu			( int nItem, UINT uFlag = MF_BYPOSITION ) const;
	bool			GetMenuString		( int nItem, CString& strName, UINT uFlag = MF_BYPOSITION ) const;
	bool			GetMenuString		( int nItem, LPTSTR lpsName, int nCount, UINT uFlag = MF_BYPOSITION )const;
	bool			GetMenuData			( int nItem, LPVOID pData, UINT uFlag = MF_BYPOSITION ) const;

	bool			SetSubMenu			( int nItem, CHSMenu *pPopupMenu, UINT uFlag = MF_BYPOSITION );
	bool			SetMenuString		( int nItem, LPCTSTR lpszNewName, UINT uFlag = MF_BYPOSITION );
	bool			SetMenuData			( int nItem, LPVOID pData, UINT uFlag = MF_BYPOSITION );
	bool			SetMenuID			( int nItem, UINT uID, UINT uFlag = MF_BYPOSITION );

	void			AppendMenu			( LPCTSTR pszName, UINT	uID, LPVOID pData = NULL, UINT uFlags = 0 );								// Append a New Menu Item
//	void			AppendMenu			( UINT uFlag, UINT uID = 0, LPVOID pData = NULL, LPCSTR pszName = NULL );
	bool			AppendPopup			( LPCTSTR pszName, CHSMenu *pPopupMenu, UINT uID = 0, LPVOID pData = NULL, UINT uFlags = 0 );	// Append a Popup Item in Menu
	bool			CreateMenu			( const CWnd* pParent );
	bool			CreateMenu			( HWND hWndParent );																		// Use to Create a New HSMenu
	bool			DeleteMenu			( int nItem, UINT uFlag = MF_BYPOSITION );
	bool			EnableMenuItem		( int nItem, UINT uItemFlags = ITEM_ENABLE, UINT uFlag = MF_BYPOSITION );
	bool			InsertMenu			( int nItem, LPCTSTR pszName, UINT uID, LPVOID pData = NULL, UINT uFlags = 0, UINT uFlag = MF_BYPOSITION );
	bool			InsertPopup			( int nItem, LPCTSTR pszName, CHSMenu *pPopupMenu, UINT uID = 0, LPVOID pData = NULL, UINT uFlags = 0, UINT uFlag = MF_BYPOSITION );
	bool			TrackPopMenu		( short x, short y, UINT uFlag = 0 );							// Display and Track Menu		

	CHSMenu		( const CWnd* pParent );
	CHSMenu		( HWND hWndParent );
	CHSMenu		();
	virtual		~CHSMenu();

private:
//	void			OnDestroy		( HWND hWnd );
	void			OnPaint			( HWND hWnd, HDC hDC );
	void			OnChar			( HWND hWnd, char c );
	void			OnKeyDown		( HWND hWnd, UINT nChar, UINT nRepCnt, UINT nFlags );
//	void			OnKeyUp			( HWND hWnd, UINT nChar, UINT nRepCnt, UINT nFlags );
	void			OnMouseMove		( HWND hWnd, short x, short y );

protected:
	virtual void	OnTimer			( HWND hWnd, unsigned short uID );
	virtual void	OnEraseBkgnd	( HWND hWnd, HDC hDC );

	virtual void	OnLButtonDown	( HWND hWnd, short x, short y );
	virtual void	OnLButtonUp		( HWND hWnd, short x, short y );
	virtual void	OnRButtonDown	( HWND hWnd, short x, short y );
	virtual void	OnRButtonUp		( HWND hWnd, short x, short y );

protected:
	virtual void	CustomMenuRgn	( HWND hMenu ){};
	virtual void	UpdateMenuCXY	( int nMenuWidth, int nMenuHeight ){};

	virtual void	DrawItem		( LPDRAWITEMSTRUCTPRO lpdisp );
	void			DrawItemText	( LPDRAWITEMSTRUCTPRO lpdisp );
	void			DrawItemArrow	( LPDRAWITEMSTRUCTPRO lpdisp );
	void			DrawItemSpr		( LPDRAWITEMSTRUCTPRO lpdisp );
	void			DrawItemBkgnd	( LPDRAWITEMSTRUCTPRO lpdisp );

	void			SetHSMenu		( CHSMenu* pHSMenu );				
	bool			IsEffectItem	( void );
	bool			GetState		( UINT uFlag ) const;
	HFONT			GetFontInfo		( int& nFontHeight ) const;

private:
	bool			IsOutMenu		( short x, short y );
	void			RedrawItem		( HWND hWnd, int nCurSel, int nOldSel );

	int				GetNewSel		( short x, short y );
	int				GetItemIndex	( int nItem, UINT uFlag ) const;
	CHSMenu*		GetNewMenu		( HWND hWnd, short x, short y );

	inline void		DestroyMenu		( void );
	void			DestroyAllPop	( void );
	void			DestroyAllMenu	( void );

	inline void		Initialize		( void );

	void			ResetDispPoint	( int& x, int& y );
	void			ResetItemPos	( int nNewTop, int nItem );

	void			MeasureItem		( LPCTSTR pszName );
	void			SetState		( UINT uFlag, int bSet );
	void			TimerDelay		( HWND hWnd );

private:
	HWND			m_hMenu;
	HWND			m_hWndParent;

	int				m_nMenuHeight;
	int				m_nMenuWidth;

	CHSMenu		   *m_pHSMenu;
	CHSMenu		   *m_pParent;								// Pointer to this Menu's Parent Menu

	int				m_nCurPop;
	int				m_nCurSel;								// Current Selected Item Index

	UINT			m_uMenuFlag;							// Flag of this menu
	DEQUEITEM		m_qMenuItem;							// Save Menu Item Informations

//private:
public:
	static COLORREF	m_clrMenu[ CLRCOUNT_BASE ];
	static HFONT	m_hFont;
	static int		m_nFontHeight;
	static int		m_nPopupDelay;							// Popup Menu Delay Time	
	static int		m_nMenuEdge;
	static int		m_nSprHeight;
	static int		m_nMenuItemHeight;						// Menu Item Height
	static bool		m_bRegister;							// Register Flag		
	// …Ë÷√≤Àµ•øÌ∂»
	void SetMenuWidth(int width);
};

#endif // !defined(AFX_HSMENU_H__4142ADCE_9B31_11D6_879C_D3DBCFA74D6E__INCLUDED_)
