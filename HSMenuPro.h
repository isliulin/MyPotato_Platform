// HSMenuPro.h: interface for the CHSMenuPro class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HSMENUPRO_H__8CBC9604_448C_44CB_AF6F_9F6618562436__INCLUDED_)
#define AFX_HSMENUPRO_H__8CBC9604_448C_44CB_AF6F_9F6618562436__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HSMenu.h"

//////////////////////////////////////////////////////////////////////////////////////// 

#define			ML_NORMAL				0//ML_BORDER | ML_SELBORDER
#define			ML_MENU3D				0X00000001				// Menu 3D Look
#define			ML_ROUNDMENU			0X00000002				// Menu has Round Corner
#define			ML_SELROUND				0X00000004				// Select Item has Round Corner
#define			ML_ARROWROUND			0X00000008				// Popup Item has a round Arrow
#define			ML_CLICK3D				0X00000010				// When User Left Buttom Down Draw a sunken Rectamgle
#define			ML_SEL3D				0X00000020				// The Item be Selected Has Heave Style
#define			ML_SELBORDER			0X00000040
#define			ML_SELGRADIENT			0X00000080
#define			ML_BKGNDEDGE			0X00000100
#define			ML_SELGRADIENT_V		0X00000200
#define			ML_SEPARATOR3D_HEAVE	0X00000400				// Menu has 3D(Outer) look separator
#define			ML_SEPARATOR3D_CONCAVE	0X00000800				// Menu has 3D(Outer) look separator
#define			ML_XXXX					0X0000FFFF

#define			CX					12

#define			CLR_BKGNDEDGE		CLRCOUNT_BASE
#define			CLR_SELSTART		CLRCOUNT_BASE + 1
#define			CLR_SELEND			CLRCOUNT_BASE + 2
#define			CLR_ICONSHADOW		CLRCOUNT_BASE + 3

#define			_CLR_BKGNDEDGE		0
#define			_CLR_SELSTART		1
#define			_CLR_SELEND			2
#define			_CLR_ICONSHADOW		3

#define			CLRCOUNT			4

#define			Index( index )		( index - CLRCOUNT_BASE )


class CHSMenuPro : public CHSMenu  
{
public:
	static void		SetMenuStyle	( UINT uMenuLook, bool bSet );
	static void		SetColor		( int nIndex, COLORREF clrSet );
	static COLORREF	GetColor		( int nIndex );
	static UINT		GetMenuStyle	( UINT uMenuLook );

	void			AppendMenu		( LPCTSTR pszName, UINT uID, int nIconID = 0, UINT uFlag = 0 );
	bool			AppendPopup		( LPCTSTR pszName, CHSMenu *pPopup, UINT uID = 0, int nIconID = 0, UINT uFlag = 0 );
	bool			InsertPopup		( int nItem, LPCTSTR pszName, CHSMenu *pPopup, UINT uID = 0, int nIconID = 0, UINT uFlags = 0, UINT uFlag = MF_BYPOSITION);
	bool			InsertMenu		( int nItem, LPCTSTR pszName, UINT uID, int nIconID = 0, UINT uFlags = 0, UINT uFlag = MF_BYPOSITION );

	void			SetMenuIconSize	( int nSize );
	bool			SetMenuIcon		( UINT nItem, int nNewIconID, UINT uFlag = MF_BYPOSITION );

	CHSMenuPro		( const CWnd *pParent );
	CHSMenuPro		( HWND hWndParent );
	CHSMenuPro		();
	virtual			~CHSMenuPro();

protected:
	virtual void	OnLButtonDown	( HWND hWnd, short x, short y );
	virtual void	OnEraseBkgnd	( HWND hWnd, HDC hDC );

	virtual void	DrawItem		( LPDRAWITEMSTRUCTPRO lpdisp );
	virtual void	DrawItemIcon	( LPDRAWITEMSTRUCTPRO lpdisp );
	virtual void	DrawItemText	( LPDRAWITEMSTRUCTPRO lpdisp );
	virtual void	DrawItemArrow	( LPDRAWITEMSTRUCTPRO lpdisp );
	virtual void	DrawItemSpr		( LPDRAWITEMSTRUCTPRO lpdisp );
	virtual void	DrawItemBkgnd	( LPDRAWITEMSTRUCTPRO lpdisp );

	virtual void	CustomMenuRgn	( HWND hMenu ){};
	virtual void	CustomSelRgn	( HRGN& hrgnBkSel, const RECT* pRectBkSel ) {};

private:
	inline void		Initialize		( void );

private:
	int		m_nIconSize;

	HDC		m_hdcBk;

protected:
	static COLORREF m_clrMenu[CLRCOUNT];

private:
	static UINT		m_uMenuFlag;
	static RECT		m_rcClick;
};

#endif // !defined(AFX_HSMENUPRO_H__8CBC9604_448C_44CB_AF6F_9F6618562436__INCLUDED_)
