// HSMenuPro.cpp: implementation of the CHSMenuPro class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "UDS Scanner Capture v1.0.0.h"
#include "HSMenuPro.h"
#include "HSGraphics.h"
#include "HSMenu.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
_CLR_BKGNDEDGE		0
_CLR_SELSTART		1
_CLR_SELEND			2
_CLR_ICONSHADOW		3
*/

COLORREF	CHSMenuPro::m_clrMenu[CLRCOUNT] =
{	RGB( 253, 253, 253 ),
	RGB( 255, 174, 174 ),
	RGB( 150, 170, 224 ),
	RGB(130,130,130)
};


RECT	CHSMenuPro::m_rcClick;
UINT	CHSMenuPro::m_uMenuFlag = ML_NORMAL;
//////////////////////////////////////////////////////////////////////

CHSMenuPro::CHSMenuPro()
{
	Initialize();
	SetHSMenu( this );
}	

CHSMenuPro::~CHSMenuPro()
{
}

CHSMenuPro::CHSMenuPro(const CWnd *pParent)
{
	Initialize();
	SetHSMenu( this );
	CreateMenu( pParent->GetSafeHwnd() );
}

CHSMenuPro::CHSMenuPro(HWND hWndParent)
{
	Initialize();
	SetHSMenu( this );
	CreateMenu( hWndParent );
}

void CHSMenuPro::AppendMenu(LPCTSTR pszName, UINT uID, int nIconID, UINT uFlags)
{
	CHSMenu::AppendMenu( pszName, uID, (LPVOID)LoadIcon( nIconID ), uFlags );
}

bool CHSMenuPro::AppendPopup(LPCTSTR pszName, CHSMenu *pPopup, UINT uID, int nIconID, UINT uFlags)
{
	return CHSMenu::AppendPopup( pszName, pPopup, uID, (LPVOID)LoadIcon( nIconID ), uFlags );
}

bool CHSMenuPro::InsertMenu(int nItem, LPCTSTR pszName, UINT uID, int nIconID, UINT uFlags, UINT uFlag)
{
	return CHSMenu::InsertMenu( nItem, pszName, uID, (LPVOID)LoadIcon( nIconID ), uFlags, uFlag );
}

bool CHSMenuPro::InsertPopup(int nItem, LPCTSTR pszName, CHSMenu *pPopup, UINT uID, int nIconID, UINT uFlags, UINT uFlag)
{
	return CHSMenu::InsertPopup( nItem, pszName, pPopup, uID, (LPVOID)LoadIcon( nIconID ), uFlags, uFlag );
}

UINT CHSMenuPro::GetMenuStyle(UINT uMenuLook )
{
	return ( m_uMenuFlag & uMenuLook );
}

void CHSMenuPro::SetMenuIconSize(int nSize)
{
	m_nIconSize = nSize < 0 ? -nSize : nSize ;
}

void CHSMenuPro::SetMenuStyle(UINT uMenuLook, bool bSet)
{
	if ( bSet )
		 m_uMenuFlag |=  ( uMenuLook );
	else m_uMenuFlag &= ~( uMenuLook );
}


inline void CHSMenuPro::Initialize()
{
	m_nIconSize = 16;
}

void CHSMenuPro::OnEraseBkgnd(HWND hWnd, HDC hDC)
{
	HRGN hRgn = CreateRectRgn(0,0,0,0);

	GetWindowRgn( hWnd, hRgn );

	if ( GetMenuStyle( ML_BKGNDEDGE ) )
	{
		DrawPartionRgn( hDC,
						CHSMenu::m_clrMenu[CLR_BKGNDMENU],			 	
						m_clrMenu[_CLR_BKGNDEDGE],
						true,
						GetMenuEdge() + m_nIconSize + CX,
						hRgn );
	}
	else
	DrawRgn( hDC, CHSMenu::m_clrMenu[CLR_BKGNDMENU], hRgn );

	if ( GetMenuStyle( ML_MENU3D ) )
		Draw3DRgnEdge( hDC, 1, hRgn );
	else
		DrawFrameRgn( hDC, hRgn, CHSMenu::m_clrMenu[CLR_BORDERMENU], 1, 1 );

	DeleteObject( hRgn );
}

void CHSMenuPro::OnLButtonDown(HWND hWnd, short x, short y)
{
	if ( GetMenuStyle( ML_CLICK3D ) && IsEffectItem() )
	{
		HDC	hDC = GetDC( hWnd );

		HRGN hrgnBkSel = NULL;

		CustomSelRgn( hrgnBkSel, &m_rcClick );

		if ( !hrgnBkSel )
			hrgnBkSel = CreateRectRgn( m_rcClick.left, m_rcClick.top, m_rcClick.right, m_rcClick.bottom );

		Draw3DRgnEdge( hDC, false, hrgnBkSel );
		DeleteObject( hrgnBkSel );

		ReleaseDC( hWnd, hDC );
	}

	CHSMenu::OnLButtonDown( hWnd, x, y );
}

void CHSMenuPro::DrawItem(LPDRAWITEMSTRUCTPRO lpdisp)
{
	// Draw a separator
	if ( lpdisp->uFlags & ITEM_SEPARATOR )
	{
		if ( GetMenuStyle( ML_BKGNDEDGE ) )
		{
			RECT& rect = lpdisp->rect;

			DrawRect(	lpdisp->hDC, 
						m_clrMenu[_CLR_BKGNDEDGE],
						m_clrMenu[_CLR_BKGNDEDGE],
						rect.left + m_nIconSize + CX, 
						rect.top, 
						rect.right, 
						rect.bottom );
		}

		DrawItemSpr( lpdisp );
		return;
	}

	DrawItemBkgnd( lpdisp );

	// Draw the Menu item Text 
	DrawItemText( lpdisp );

	// Draw Icon
	if ( lpdisp->pData )
		DrawItemIcon( lpdisp );

	if ( lpdisp->uFlags & ITEM_POPUP )
		DrawItemArrow( lpdisp );
}

void CHSMenuPro::DrawItemBkgnd(LPDRAWITEMSTRUCTPRO lpdisp)
{
	HRGN hrgnBkSel = NULL;

	CustomSelRgn( hrgnBkSel, &lpdisp->rect );

	if ( !hrgnBkSel )
	{
		const RECT& rect = lpdisp->rect;
		hrgnBkSel = CreateRectRgn( rect.left, rect.top, rect.right, rect.bottom );
	}

	if ( lpdisp->uFlags & MENU_SELECTED )
	{
		if ( GetMenuStyle( ML_SELGRADIENT ) || GetMenuStyle( ML_SELGRADIENT_V ) )
		{
			DrawGradientRgn( lpdisp->hDC,
							  GetMenuStyle( ML_SELGRADIENT_V )!=0,
							  hrgnBkSel, 
							  m_clrMenu[_CLR_SELSTART], 
							  m_clrMenu[_CLR_SELEND] );
		}
		else
		{
			DrawRgn( lpdisp->hDC, CHSMenu::m_clrMenu[CLR_BKGNDSEL], hrgnBkSel );
		}

		if ( GetMenuStyle( ML_SEL3D ) )
		{
			Draw3DRgnEdge( lpdisp->hDC, true, hrgnBkSel );
		}
		else
		if ( GetMenuStyle( ML_SELBORDER ) )
		{
			DrawFrameRgn( lpdisp->hDC, hrgnBkSel, CHSMenu::m_clrMenu[CLR_BORDERSEL], 1, 1 );
		}

		if ( GetMenuStyle( ML_CLICK3D ) )
		{
			const RECT& rect = lpdisp->rect;

			m_rcClick.left	 = rect.left;
			m_rcClick.top	 = rect.top;
			m_rcClick.right	 = rect.right;
			m_rcClick.bottom = rect.bottom;
		}
	}
	else		// Draw Normal Bk
	{
		if ( GetMenuStyle( ML_BKGNDEDGE ) )
		{
			DrawPartionRgn( lpdisp->hDC,
							CHSMenu::m_clrMenu[CLR_BKGNDMENU],			 	
							m_clrMenu[_CLR_BKGNDEDGE],
							true,
							m_nIconSize + CX,
							hrgnBkSel );
		}
		else
		DrawRgn( lpdisp->hDC, CHSMenu::m_clrMenu[CLR_BKGNDMENU], hrgnBkSel );
	}

	DeleteObject( hrgnBkSel );
}

void CHSMenuPro::DrawItemSpr(LPDRAWITEMSTRUCTPRO lpdisp)
{
	POINT	pt[2];
	RECT&	rect = lpdisp->rect;

	pt[1].x = rect.right;
	pt[0].y = pt[1].y = ( rect.top + rect.bottom ) / 2;

	if ( GetMenuStyle( ML_BKGNDEDGE ) )
		pt[0].x = rect.left + m_nIconSize + CX;
	else
		pt[0].x = rect.left;

	// 3D Outer
	if ( GetMenuStyle( ML_SEPARATOR3D_HEAVE ) )
	{
		if ( !GetMenuStyle( ML_BKGNDEDGE ) )
			pt[0].y = ( pt[1].y -= 1 );

		Draw3DLine( lpdisp->hDC, true, pt[0], pt[1] );
	}
	else
	if ( GetMenuStyle( ML_SEPARATOR3D_CONCAVE ) )
	{
		if ( GetMenuStyle( ML_BKGNDEDGE ) )
			pt[0].y = ( pt[1].y -= 1 );

		Draw3DLine( lpdisp->hDC, false, pt[0], pt[1] );
	}
	else	// Normal Line	
	DrawPolyline( lpdisp->hDC, CHSMenu::m_clrMenu[CLR_BORDERSPR], pt, 2 );
}

void CHSMenuPro::DrawItemArrow(LPDRAWITEMSTRUCTPRO lpdisp)
{
	if ( GetMenuStyle( ML_ARROWROUND ) )
	{
		RECT&		rect = lpdisp->rect;
		COLORREF	clrBorder, clrBkgnd;

		if ( lpdisp->uFlags & ITEM_GRAY )
		{
			clrBorder = GetSysColor( COLOR_GRAYTEXT );
			clrBkgnd  = GetSysColor( COLOR_GRAYTEXT );
		}
		else
		{
			if ( lpdisp->uFlags & MENU_SELECTED )
			{
				clrBkgnd  = CHSMenu::m_clrMenu[CLR_ARROWSEL];
				clrBorder = CHSMenu::m_clrMenu[CLR_TEXTNORMAL];
			}
			else
			{
				clrBkgnd  = CHSMenu::m_clrMenu[CLR_TEXTNORMAL];
				clrBorder = CHSMenu::m_clrMenu[CLR_ARROWSEL];
			}

		}

		int	nLeft = rect.right - 16;
		int nTop  = rect.top + (rect.bottom-rect.top)/2 - 3;

		DrawEllipse( lpdisp->hDC, 
					 clrBorder,
					 clrBkgnd,
					 nLeft, 
					 nTop, 
					 nLeft + 6, 
					 nTop  + 6 );
	}
	else
	CHSMenu::DrawItemArrow( lpdisp );
}

void CHSMenuPro::DrawItemText(LPDRAWITEMSTRUCTPRO lpdisp)
{
	HGDIOBJ hfntOld;
	int		nFontHeight;
	HFONT	hFont   = GetFontInfo( nFontHeight );
	UINT	fuFlag	= DST_TEXT;
	RECT&	rect    = lpdisp->rect;
	int		nLeft	= rect.left + m_nIconSize + CX + 8;
	int		nTop	= ( rect.top  + rect.bottom - nFontHeight ) / 2;
	
//	if ( MenuItem.m_uFlags & ITEM_GRAY )	 
//		fuFlag |= DSS_UNION;

	if ( lpdisp->uFlags & ITEM_GRAY )	 
		fuFlag |= DSS_DISABLED;
	else
		SetTextColor( lpdisp->hDC, CHSMenu::GetColor( GetState( MENU_SELECTED ) ? CLR_TEXTSEL : CLR_TEXTNORMAL ) );

	SetBkMode( lpdisp->hDC, TRANSPARENT );
	hfntOld = SelectObject( lpdisp->hDC, hFont );
	DrawState( lpdisp->hDC, NULL, NULL, (LPARAM)lpdisp->lpcStr, 0, nLeft, nTop, 800, 70, fuFlag );
	SelectObject( lpdisp->hDC, hfntOld );	
}

// Draw Item Icon
void CHSMenuPro::DrawItemIcon(LPDRAWITEMSTRUCTPRO lpdisp)
{
	RECT& rect = lpdisp->rect;

	int	x = rect.left + 5;
	int y = ( rect.top + rect.bottom - m_nIconSize ) / 2;

	if ( lpdisp->uFlags & ITEM_GRAY )
	{
		DrawShadowIcon( lpdisp->hDC, 
					    x,
						y,
						(HICON)lpdisp->pData,
						m_nIconSize,
						m_nIconSize, 
						GetSysColor( COLOR_GRAYTEXT ) );
	}
	else
	{
		if ( lpdisp->uFlags & MENU_SELECTED )
		{
			DrawShadowIcon( lpdisp->hDC, 
						    x + 1,
							y + 1,
							(HICON)lpdisp->pData,
							m_nIconSize, 
							m_nIconSize,
							m_clrMenu[_CLR_ICONSHADOW] );

			DrawIconEx( lpdisp->hDC,
						x - 1,
						y - 1,
						(HICON)lpdisp->pData,
						m_nIconSize, 
						m_nIconSize,
						0,
						NULL,
						DI_NORMAL );
		}
		else
		DrawIconEx( lpdisp->hDC,
					x,
					y,
					(HICON)lpdisp->pData,
					m_nIconSize, 
					m_nIconSize,
					0,
					NULL,
					DI_NORMAL );

/*		DrawFadeIcon( lpdisp->hDC,
					  x,
					  y,
					  (HICON)lpdisp->pData,
					  m_nIconSize, 
					  m_nIconSize,
					  100 );
*/
	}
}

COLORREF CHSMenuPro::GetColor(int nIndex)
{
	switch ( nIndex )
	{
		case CLR_BKGNDEDGE:
		case CLR_SELSTART:
		case CLR_SELEND:
		case CLR_ICONSHADOW:
			return m_clrMenu[Index( nIndex )];

		default :
			return CHSMenu::GetColor( nIndex );
	}
}

void CHSMenuPro::SetColor(int nIndex, COLORREF clrSet)
{
	switch ( nIndex )
	{
		case CLR_BKGNDEDGE:
		case CLR_SELSTART:
		case CLR_SELEND:
		case CLR_ICONSHADOW:
			m_clrMenu[Index( nIndex )] = clrSet;
			break;

		default:
			CHSMenu::SetColor( nIndex, clrSet );
	}
}

// Set Menu Item Icon 
bool CHSMenuPro::SetMenuIcon(UINT nItem, int nIconID, UINT uFlag)
{
	return CHSMenu::SetMenuData( nItem, (LPVOID)LoadIcon( nIconID ), uFlag );
}
/*
void CHSMenuPro::CustomMenuRgn(HWND hMenu)
{

}

void CHSMenuPro::CustomSelRgn( HRGN &hrgnBkSel, const RECT* pRectBkSel )
{

}
*/