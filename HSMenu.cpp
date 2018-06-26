// HSMenu.cpp: implementation of the CHSMenu class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "UDS Scanner Capture v1.0.0.h"
#include "HSMenu.h"
#include "HSGraphics.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*
void HSITEM::Assign(const HSITEM &MenuItem)
{
	m_uID			= MenuItem.m_uID; 
	m_pData			= MenuItem.m_pData;
	m_uFlags		= MenuItem.m_uFlags;
	m_nTop			= MenuItem.m_nTop;
	m_strName		= MenuItem.m_strName;
	m_pPopupHSMenu	= MenuItem.m_pPopupHSMenu;
}

HSITEM& HSITEM::operator =(const HSITEM &MenuItem)
{
	Assign( MenuItem );
	return *this;
}

HSITEM::HSITEM( const HSITEM& MenuItem )
{
	Assign( MenuItem );
}
*/
inline HSITEM::HSITEM()
{
	m_uID			= 0; 
	m_uFlags		= ITEM_NORMAL;
	m_pData			= NULL;
	m_pPopupHSMenu	= NULL;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


/*
CLR_BKGNDSEL		0
CLR_BKGNDMENU		1
CLR_BORDERMENU		2
CLR_BORDERSEL		3
CLR_BORDERSPR		4
CLR_TEXTSEL			5
CLR_TEXTNORMAL		6
*/

COLORREF CHSMenu::m_clrMenu[CLRCOUNT_BASE] =
											{ GetSysColor( COLOR_HIGHLIGHT ),
											  GetSysColor( COLOR_MENU ),
											  RGB( 170, 170, 170 ),
											  RGB( 47 , 60 , 174 ),
											  RGB( 160, 160, 160 ),
											  GetSysColor( COLOR_HIGHLIGHTTEXT ),
											  GetSysColor( COLOR_MENUTEXT ),
											  GetSysColor( COLOR_HIGHLIGHTTEXT )
											};

int		CHSMenu::m_nMenuEdge		= 3;
int		CHSMenu::m_nSprHeight		= 5;
int		CHSMenu::m_nMenuItemHeight	= 26;
int		CHSMenu::m_nPopupDelay		= 300;
int		CHSMenu::m_nFontHeight		= 18;
HFONT	CHSMenu::m_hFont			= (HFONT)GetStockObject( DEFAULT_GUI_FONT );
bool	CHSMenu::m_bRegister		= false;

//////////////////////////////////////////////////////////////////////

CHSMenu::CHSMenu()
{
}

CHSMenu::~CHSMenu()
{
}

CHSMenu::CHSMenu(HWND hWndParent )
{
	CreateMenu( hWndParent );
}

CHSMenu::CHSMenu(const CWnd *pParent)
{
	CreateMenu( pParent->GetSafeHwnd() );
}

bool CHSMenu::CreateMenu(const CWnd *pParent)
{
	return CreateMenu( pParent->GetSafeHwnd() );
}

// Must be Called by 
void CHSMenu::SetHSMenu(CHSMenu *pHSMenu)
{
	m_pHSMenu = pHSMenu ? pHSMenu : this;
}

// Create a HSMenu
bool CHSMenu::CreateMenu(HWND hWndParent )
{
	Initialize();

	m_hWndParent = hWndParent;

	// Register HSMenu
	if ( !m_bRegister )
	{
		WNDCLASS	wc;

		wc.style			= 0;
		wc.cbClsExtra		= 0;
		wc.cbWndExtra		= 0;
		wc.lpszMenuName		= NULL;
		wc.hbrBackground	= NULL;
		wc.hIcon			= NULL;

		wc.hCursor			= LoadCursor( NULL, IDC_ARROW );
		wc.hInstance		= AfxGetInstanceHandle();//(HINSTANCE)GetWindowLong( m_hWndParent, GWL_HINSTANCE );
		wc.lpfnWndProc		= lpfnWndProc;
		wc.lpszClassName	= _T("fpmHSMenuClass");

		if ( !::RegisterClass( &wc ) )
			return false;

		m_bRegister = true;
	}

	return true;
}

// Initialize Parameter
inline void CHSMenu::Initialize( void )
{
	m_pHSMenu		= this;
	m_pParent		= NULL;
	m_uMenuFlag		= MENU_NORMAL;
	m_nMenuHeight	= m_nMenuEdge*2 + MENUCAPTION;
	m_nMenuWidth	= m_nMenuEdge*2 + 30;
	m_nCurSel		= SELNONE;
	m_nCurPop		= SELNONE;

	m_qMenuItem.clear();
}

void CHSMenu::SetState(UINT uFlag, int bSet)
{
	if ( bSet )
		 m_uMenuFlag |=  ( uFlag );
	else m_uMenuFlag &= ~( uFlag );
}

bool CHSMenu::GetState(UINT uFlag) const
{
	return ( ( m_uMenuFlag & ( uFlag ) ) != 0ul );
}
/*
void CHSMenu::AppendMenu(UINT uFlag, UINT uID, LPVOID pData, LPCSTR pszName)
{
	AppendMenu( pszName, uID, pData, uFlag );
}
*/
// Append a New menu item to end
void CHSMenu::AppendMenu(LPCTSTR pszName, UINT uID, LPVOID pData, UINT uFlags)
{
	HSITEM	MenuItem;

	// Set New Item top point at Menu Window
	MenuItem.m_nTop = m_nMenuHeight - m_nMenuEdge;
	// Save Flag
	MenuItem.m_uFlags = ( uFlags & ITEM_XXX );
	MenuItem.m_pData  = pData;

	if ( uFlags & ITEM_SEPARATOR )
	{
		MenuItem.m_nHeight = m_nSprHeight;
		// Default Separator height = 5
		m_nMenuHeight += m_nSprHeight;
	}
	else
	{
		// Save New Item ID
		MenuItem.m_uID = uID;
		// Save Item Name 
		if ( pszName )
		{
			USES_CONVERSION;
			MenuItem.m_strName = W2A(pszName); //7-4修改 // 如果该处采用(LPCSTR)(pszName)菜单名将出现乱码， 由此可见强制转换不能随便用
			MeasureItem( pszName );
		}

		MenuItem.m_nHeight = m_nMenuItemHeight;

		// Update Menu Height
		m_nMenuHeight += m_nMenuItemHeight;
	}

	// Save New Item informations
	m_qMenuItem.push_back( MenuItem );

	m_pHSMenu->UpdateMenuCXY( m_nMenuWidth, m_nMenuHeight );
}

// Append a popup menu item
bool CHSMenu::AppendPopup(LPCTSTR pszName, CHSMenu *pPopupMenu, UINT uID, LPVOID pData, UINT uFlags)
{
	if ( ( pPopupMenu && pPopupMenu->m_pParent ) || uFlags & ITEM_SEPARATOR )
		return false;

	AppendMenu( pszName, uID, pData, uFlags );

	return SetSubMenu( m_qMenuItem.size()-1, pPopupMenu, MF_BYPOSITION );
}

// Insert a menu item 
bool CHSMenu::InsertMenu(int nItem, LPCTSTR pszName, UINT uID, LPVOID pData, UINT uFlags, UINT uFlag)
{
	if ( ( nItem = GetItemIndex( nItem, uFlag ) ) == FINDNONE )
		return false;

	if ( m_qMenuItem.size() < 2 )
		AppendMenu( pszName, uID, pData, uFlags );
	else
	{
		HSITEM	MenuItem;
		HSITEM& tMenuItem = m_qMenuItem[nItem];

		// Set New Item top point at Menu Window
		MenuItem.m_nTop		= tMenuItem.m_nTop;
		MenuItem.m_nHeight	= ( uFlags & ITEM_SEPARATOR ? m_nSprHeight : m_nMenuItemHeight );
		// Save Flag
		MenuItem.m_uFlags = ( uFlags & ITEM_XXX );
		//Other Data
		MenuItem.m_pData  = pData;
		// Save New Item ID
		MenuItem.m_uID = uID;
		// Save Item Name 
		if ( pszName )
		{
			MenuItem.m_strName = (LPSTR)pszName;   //7-4修改
			MeasureItem( pszName );
		}
		// Reset Next Item's < m_nTop >
		ResetItemPos( tMenuItem.m_nTop + MenuItem.m_nHeight, nItem );
		// Save New Item informations
		m_qMenuItem.insert( m_qMenuItem.begin() + nItem, MenuItem );
	}

	m_pHSMenu->UpdateMenuCXY( m_nMenuWidth, m_nMenuHeight );

	return true;
}

bool CHSMenu::InsertPopup(int nItem, LPCTSTR pszName, CHSMenu *pPopupMenu, UINT uID, LPVOID pData, UINT uFlags, UINT uFlag)
{
	if ( m_qMenuItem.size() < 2 )
		return AppendPopup( pszName, pPopupMenu, uID, pData, uFlags );
	else
	{
		if ( ( pPopupMenu && pPopupMenu->m_pParent ) ||
		     ( uFlags & ITEM_SEPARATOR )			 ||
			 !InsertMenu( nItem, pszName, uID, pData, uFlags, uFlag )
		   ) return false;

		SetSubMenu( nItem, pPopupMenu, uFlag );
	}
	
	return true;
}

void CHSMenu::ResetItemPos(int nNewTop, int nItem)
{
	DEQUEITEM::size_type  iend = m_qMenuItem.size();

	if ( iend == 0 )
	{
		m_nMenuHeight += nNewTop;
		return;
	}

	m_nMenuHeight += ( nNewTop - m_qMenuItem[nItem].m_nTop );

	for ( ; nItem<iend; nItem++ )
	{
		HSITEM& MenuItem = m_qMenuItem[nItem];

		MenuItem.m_nTop  = nNewTop;

		nNewTop += MenuItem.m_nHeight;
	}
}

// Delete menu item
bool CHSMenu::DeleteMenu(int nItem, UINT uFlag)
{
	if ( ( nItem = GetItemIndex( nItem, uFlag ) ) == FINDNONE )
		return false;

	SetSubMenu( nItem, NULL, uFlag );

	if ( nItem == ( m_qMenuItem.size() - 1 ) )	// must do this first
	{	// Remove The Last Item
		m_nMenuHeight -= m_qMenuItem[nItem].m_nHeight;
		m_qMenuItem.pop_back();
	}
	else
	if ( nItem == 0 )
	{	// Remove The First Item
		ResetItemPos( m_qMenuItem[0].m_nTop, 1 );
		m_qMenuItem.pop_front();
	}
	else
	{
		ResetItemPos( m_qMenuItem[nItem].m_nTop, nItem+1 );
		m_qMenuItem.erase( m_qMenuItem.begin() + nItem );
	}

	m_nMenuWidth = 30;

	int iend = m_qMenuItem.size();
	USES_CONVERSION;
	for ( int i=0; i<iend; i++ )
	{
	
    LPCSTR  tem_str = m_qMenuItem[i].m_strName.c_str();
   	MeasureItem( A2W(tem_str) );
	}

	m_pHSMenu->UpdateMenuCXY( m_nMenuWidth, m_nMenuHeight );

	return true;
}

// Enable or Disable a Menu item
bool CHSMenu::EnableMenuItem(int nItem, UINT uItemFlags, UINT uFlag)
{
	if ( ( nItem = GetItemIndex( nItem, uFlag ) ) == FINDNONE )
		return false;
	
	if ( uItemFlags & ITEM_ENABLE )
	{
		// Reset Menu Item Flag to Enable
		m_qMenuItem[nItem].m_uFlags &= ~( ITEM_DISABLE | ITEM_GRAY );
	}
	else
	if ( uItemFlags & ITEM_DISABLE )
	{
		// Reset Menu Item Flag to Disable 
		m_qMenuItem[nItem].m_uFlags |= ITEM_DISABLE;
	}
	else
	if ( uItemFlags & ITEM_GRAY )
		m_qMenuItem[nItem].m_uFlags |= ITEM_GRAY;

	return true;
}

HFONT CHSMenu::GetFontInfo(int &nFontHeight) const
{
	nFontHeight = m_nFontHeight;
	return m_hFont;
}

// Return Current Selected Item, none to return ( SELNONE )
int CHSMenu::GetCurSelItem( void ) const
{
	return m_nCurSel;
}

// Get the Total item 
int CHSMenu::GetMenuItemCount() const
{
	return (int)m_qMenuItem.size();
}

// Private FN
int CHSMenu::GetItemIndex(int nItem, UINT uFlag) const
{
	if ( m_qMenuItem.size() == 0 )
		return FINDNONE;

	int nIndex = FINDNONE;

	if ( uFlag == MF_BYPOSITION )
	{
		if ( nItem >= 0 && nItem < m_qMenuItem.size() ) 
			nIndex = nItem;
	}
	else
	{
		DEQUEITEM::size_type  index, iend;

		iend = m_qMenuItem.size();

		for ( index = 0; index < iend; index++ )
			if ( m_qMenuItem[index].m_uID == (UINT)nItem ) 	
			{
				nIndex = (int)index;
				break;
			}
	}

	return nIndex;
}

// Get the submenu of this item, return NULL if none
CHSMenu* CHSMenu::GetSubMenu(int nItem, UINT uFlag) const
{
	if ( ( nItem = GetItemIndex( nItem, uFlag ) ) == FINDNONE )
		return NULL;

	return (CHSMenu*)m_qMenuItem[nItem].m_pPopupHSMenu;
}

// Set a submenu for this item
bool CHSMenu::SetSubMenu(int nItem, CHSMenu *pPopupMenu, UINT uFlag)
{
	if ( ( nItem = GetItemIndex( nItem, uFlag ) ) == FINDNONE )
		return false;

	if ( pPopupMenu && pPopupMenu->m_pParent )
		return false;
	
	HSITEM& MenuItem = m_qMenuItem[nItem];

	if ( MenuItem.m_uFlags & ITEM_SEPARATOR )
		return false;

	if ( MenuItem.m_pPopupHSMenu )
	{
		MenuItem.m_uFlags &= ~ITEM_POPUP;
		MenuItem.m_pPopupHSMenu->m_pParent = NULL;
		MenuItem.m_pPopupHSMenu->SetState( MENU_CHILD, false );
	}

	if ( pPopupMenu )
	{
		MenuItem.m_uFlags |= ITEM_POPUP;
		pPopupMenu->m_pParent = this;
		pPopupMenu->SetState( MENU_CHILD, true );
	}

	MenuItem.m_pPopupHSMenu = pPopupMenu;
	return true;
}

// Set addition data for a item
bool CHSMenu::SetMenuData(int nItem, LPVOID pData, UINT uFlag)
{
	if ( ( nItem = GetItemIndex( nItem, uFlag ) ) == FINDNONE )
		return false;

	m_qMenuItem[nItem].m_pData = pData;
	return true;
}

// Get menu item text
bool CHSMenu::GetMenuString(int nItem, LPTSTR lpsName, int nCount, UINT uFlag) const
{
	USES_CONVERSION;
	if ( ( nItem = GetItemIndex( nItem, uFlag ) ) == FINDNONE )
		return false;

	LPCSTR   tem_str = m_qMenuItem[nItem].m_strName.c_str();
	return ( lstrcpyn( lpsName, A2W(tem_str), nCount ) != NULL );

//	return ( lstrcpyn( lpsName, m_qMenuItem[nItem].m_strName.c_str(), nCount ) != NULL );
}

// Set menu item text
bool CHSMenu::GetMenuString(int nItem, CString &strName, UINT uFlag) const
{
	if ( ( nItem = GetItemIndex( nItem, uFlag ) ) == FINDNONE )
		return false;

	strName = m_qMenuItem[nItem].m_strName.c_str();
	return true;
}

// Set Menu Item Text
bool CHSMenu::SetMenuString(int nItem, LPCTSTR lpszNewName, UINT uFlag)
{
	if ( ( nItem = GetItemIndex( nItem, uFlag ) ) == FINDNONE )
		return false;

	m_qMenuItem[nItem].m_strName = (LPSTR)lpszNewName;
	MeasureItem( lpszNewName );

	m_pHSMenu->UpdateMenuCXY( m_nMenuWidth, m_nMenuHeight );
	return true;
}

// Get addition item date
bool CHSMenu::GetMenuData(int nItem, LPVOID pData, UINT uFlag) const
{
	if ( ( nItem = GetItemIndex( nItem, uFlag ) ) == FINDNONE )
		return false;

	pData = m_qMenuItem[nItem].m_pData;
	return true;
}

// Get Menu item Flags
UINT CHSMenu::GetMenuState(int nItem, UINT uFlag) const
{
	if ( ( nItem = GetItemIndex( nItem, uFlag ) ) == FINDNONE )
		return 0;

	return ( m_qMenuItem[nItem].m_uFlags );
}

// Set menu item ID
bool CHSMenu::SetMenuID(int nItem, UINT uID, UINT uFlag)
{
	if ( ( nItem = GetItemIndex( nItem, uFlag ) ) == FINDNONE )
		return false;

	m_qMenuItem[nItem].m_uID = uID;
	return true;
}

int CHSMenu::GetMenuPosition(UINT uID) const
{
	return GetItemIndex( uID, MF_BYCOMMAND );
}

// Get menu item id by position
UINT CHSMenu::GetMenuID(int nPosition) const
{
	if ( nPosition < 0 || nPosition >= m_qMenuItem.size() ) 
		return 0;

	return ( m_qMenuItem[nPosition].m_uID );
}

void CHSMenu::GetMenuCXY(int &nMenuWidth, int &nMenuHeight)
{
	nMenuWidth  = m_nMenuWidth;
	nMenuHeight = m_nMenuHeight;
}

// Static FN Set Menu Font
void CHSMenu::SetMenuFont(HFONT hFont)
{
	if ( !m_bRegister )
		m_hFont = ( hFont ? hFont : (HFONT)GetStockObject( DEFAULT_GUI_FONT ) );
}

//Static FN Set menu edge width
void CHSMenu::SetMenuEdge(int nEdge)
{
	if ( !m_bRegister )
		m_nMenuEdge = ( nEdge <= 0 ? 1 : nEdge );
}

//Static FN Get menu edge width
int CHSMenu::GetMenuEdge() const
{
	return m_nMenuEdge;
}

//Static FN Set submenu popup delay time
void CHSMenu::SetMenuPopDelay(int nDelay)
{
	m_nPopupDelay = abs( nDelay );
}

//Static FN Set menu item height
void CHSMenu::SetMenuItemHeight(int nHeight)
{
	m_nMenuItemHeight = ( ( nHeight - 4 ) > m_nFontHeight ? nHeight : m_nFontHeight + 4 ) ;
}

//Static FN Set the menu color which you want to change
void CHSMenu::SetColor(int nIndex, COLORREF clrSet )
{
	if ( nIndex >= 0 && nIndex < CLRCOUNT_BASE )
		m_clrMenu[nIndex] = clrSet;
}

//Static FN Set menu Separator height 
void CHSMenu::SetSeparatorHeight(int nHeight)
{
	m_nSprHeight = abs( nHeight );
}

//Static FN  Get menu separator height
int CHSMenu::GetSeparatorHeight()
{
	return m_nSprHeight;
}

//Static FN Get submenu popup delay time
int CHSMenu::GetMenuPopDelay() 
{
	return m_nPopupDelay;
}

//Static FN Get menu item height
int CHSMenu::GetMenuItemHeight() 
{
	return m_nMenuItemHeight;
}

//Static FN Get menu color which you want to know
COLORREF CHSMenu::GetColor(int nIndex)
{
	if ( nIndex >= 0 && nIndex < CLRCOUNT_BASE )
		 return m_clrMenu[nIndex];
	
	return -1;
}

void CHSMenu::MeasureItem(LPCTSTR pszName)
{
	USES_CONVERSION;

	HGDIOBJ hfntOld;
	SIZE	sizeText;

	// To Get New Item Name width
	HDC	hDC = CreateIC( _T("DISPLAY"), NULL, NULL, NULL );
	hfntOld = SelectObject( hDC, m_hFont );
	GetTextExtentPoint32( hDC, pszName, strlen( W2A(pszName) ), &sizeText );
	SelectObject( hDC, hfntOld );
	DeleteDC( hDC );

	if ( sizeText.cx + 80 > m_nMenuWidth - m_nMenuEdge*2 )
		m_nMenuWidth = sizeText.cx + 80 + m_nMenuEdge*2;

	if ( m_nMenuItemHeight < m_nFontHeight + 4 )
		 m_nMenuItemHeight = m_nFontHeight + 4;

	static bool bSet = false;
	if ( !bSet )
	{
		m_nFontHeight = sizeText.cy;
		bSet = true;
	}
}

// ( Point x and y is Screen point )
void CHSMenu::ResetDispPoint( int& x, int& y)
{
	static RECT	rectScreen;
	static bool bNoGet = true;
	
	// Get System Screen Rectangle
	if ( bNoGet )
	{
		SystemParametersInfo( SPI_GETWORKAREA, 0, &rectScreen, 0 );
		bNoGet = false;
	}

	if ( x + m_nMenuWidth > rectScreen.right )
	{
		if ( m_pParent )
			 x = x - m_nMenuWidth - m_pParent->CHSMenu::m_nMenuWidth + m_nMenuEdge*2;
		else x = x - m_nMenuWidth;
	}

	if ( y + m_nMenuHeight > rectScreen.bottom )
	{
		if ( m_pParent )
		{
			HSITEM& MenuItem = m_pParent->m_qMenuItem[m_pParent->m_nCurSel];

			 y = y - m_nMenuHeight + MenuItem.m_nHeight + m_nMenuEdge*2;
		}
		else y = y - m_nMenuHeight;
	}

	if ( x < 0 )	x = 0;
	if ( y < 0 )	y = 0;
}

// Display and track menu
bool CHSMenu::TrackPopMenu(short x, short y, UINT uFlag)
{
	ResetDispPoint( (int&)x, (int&)y );

	// Create Menu Window
	m_hMenu = ::CreateWindowEx( WS_EX_TOOLWINDOW,
								_T("fpmHSMenuClass"),
								_T(""),
								WS_POPUP | WS_VISIBLE,
								x,
								y,
								m_nMenuWidth,
								m_nMenuHeight,
								m_hWndParent,
								NULL,
								AfxGetInstanceHandle(),
								(LPVOID)m_pHSMenu );

	if ( !m_hMenu )	return false;

	HRGN hRgnMenu = CreateRectRgn( 0, 0, m_nMenuWidth, m_nMenuHeight );
	SetWindowRgn( m_hMenu, hRgnMenu, true );
	m_pHSMenu->CustomMenuRgn( m_hMenu );
	DeleteObject( hRgnMenu );

	ReleaseCapture();
	SetCapture( m_hMenu );

	if ( m_pParent )
	{
		m_pParent->CHSMenu::SetState( MENU_POPUPED, true );
		m_pParent->CHSMenu::m_nCurPop = m_pParent->CHSMenu::m_nCurSel;
	}

	m_nCurSel = SELNONE;
	m_nCurPop = SELNONE;

	MSG	Msg;

	while ( GetMessage( &Msg, NULL, 0, 0 ) && IsWindow( m_hMenu ) )
	{
		TranslateMessage( &Msg );
		DispatchMessage( &Msg );
	}

	return true;
}

// Update the Menu Item be Selected now
int CHSMenu::GetNewSel(short x, short y)
{
	// OutSide Menu Active Area ?
	if ( !(	x > m_nMenuEdge &&
			y > m_nMenuEdge &&
			x < m_nMenuWidth  - m_nMenuEdge &&
			y < m_nMenuHeight - m_nMenuEdge 
		  )
		) return SELNONE;

	int index;
	int iend  = m_qMenuItem.size();
	// if Current Selected Item Isn't none, Use it 
	// else Find start at middle
	index = ( m_nCurSel != SELNONE ) ? m_nCurSel : iend / 2;

	while ( true )
	{
		HSITEM& MenuItem = m_qMenuItem[index];

		if ( y > MenuItem.m_nTop )
		{
			if ( y <= ( MenuItem.m_nTop + MenuItem.m_nHeight ) )
				return index;

			if ( ++index >= iend )	return SELNONE;
		}
		else
		{
			if ( --index < 0 )  return SELNONE;
		}
	}
}

// Get HSMenu When Mouse Move to SubMenu or Form Sub to Parent
CHSMenu* CHSMenu::GetNewMenu(HWND hWnd, short x, short y)
{
	HWND	hWndPoint;
	TCHAR	sClassName[50];
	POINT	pt;

	pt.x = x;
	pt.y = y;

	ClientToScreen( hWnd, &pt );

	if ( ( hWndPoint = WindowFromPoint( pt ) ) == NULL )	return m_pHSMenu;
	GetClassName( hWndPoint, sClassName, sizeof(sClassName)/sizeof(TCHAR) );
	// If it isn't CHSMenu Class return this menu, Mean no change Capture
	if ( lstrcmp( sClassName, _T("fpmHSMenuClass") ) )	return m_pHSMenu;

	// Return the CHSMenu Pointer, Have mouse Focus
	return ( ( CHSMenu* )( GetWindowLong( hWndPoint, GWL_USERDATA ) ) ); 
}

// Graphics menu background
void CHSMenu::OnEraseBkgnd(HWND hWnd, HDC hDC)
{
	DrawRect( hDC,
			  m_clrMenu[CLR_BORDERMENU],
			  m_clrMenu[CLR_BKGNDMENU],
			  0,
			  0,
			  m_nMenuWidth,
			  m_nMenuHeight );
}

//void CHSMenu::OnChar( HWND hWnd, char c )
//{
//}

void CHSMenu::OnKeyDown( HWND hWnd, UINT nChar, UINT nRepCnt, UINT nFlags)
{
	int nNewSel = m_nCurSel;
	int nOldSel = m_nCurSel;
	int nCount  = m_qMenuItem.size();
	
	nCount <= 0 ? nChar = VK_ESCAPE : nOldSel %= nCount;

	switch( nChar )
	{
		case VK_DOWN:
			if ( m_nCurSel == SELNONE ) 
			{
				nNewSel = -1;
				nOldSel = SELNONE;
			}

			do
			{
				nNewSel = ( nNewSel + 1 ) % nCount;
			}while ( m_qMenuItem[nNewSel].m_uFlags & ITEM_SEPARATOR );

			RedrawItem( hWnd, nNewSel, nOldSel );
			break;

		case VK_UP:
			if ( m_nCurSel == SELNONE ) 
			{
				nNewSel = nCount;
				nOldSel = SELNONE;
			}

			do
			{
				nNewSel = nNewSel ? ( nNewSel - 1 ) % nCount : nCount - 1;
			}while ( m_qMenuItem[nNewSel].m_uFlags & ITEM_SEPARATOR );

			RedrawItem( hWnd, nNewSel, nOldSel );
			break;

		case VK_RIGHT:
			if ( m_nCurSel != SELNONE )
			{
				const HSITEM& MenuItem = m_qMenuItem[m_nCurSel];

				if ( MenuItem.m_pPopupHSMenu && !( MenuItem.m_uFlags & ( ITEM_GRAY | ITEM_DISABLE ) ) )
				{
					POINT	ptDisp;	
					ptDisp.x = m_nMenuWidth - m_nMenuEdge;
					ptDisp.y = MenuItem.m_nTop - m_nMenuEdge;
					ClientToScreen( hWnd, &ptDisp );
					// track the new menu
					MenuItem.m_pPopupHSMenu->TrackPopMenu( ptDisp.x, ptDisp.y );
				}
			}
			break;

		case VK_LEFT:
			if ( m_pParent )
			{
				ReleaseCapture();
				SetActiveWindow( m_pParent->CHSMenu::m_hMenu );
				SetCapture( m_pParent->CHSMenu::m_hMenu );
				DestroyMenu();
			}
			break;

		case VK_ESCAPE:
			if ( m_pParent )
			{
				ReleaseCapture();
				SetActiveWindow( m_pParent->CHSMenu::m_hMenu );
				SetCapture( m_pParent->CHSMenu::m_hMenu );
			}
			DestroyMenu();
			break;		
			
		case VK_LWIN:
		case VK_RWIN:
			DestroyAllMenu();
			break;

		case VK_SPACE:
		case VK_RETURN:
			if ( IsEffectItem() )
			{
				DestroyAllMenu();
				SendMessage( m_hWndParent, WM_COMMAND, MAKEWORD( m_qMenuItem[m_nCurSel].m_uID, 0 ),0 );
			}
	}
}
/*
void CHSMenu::OnKeyUp( HWND hWnd, UINT nChar, UINT nRepCnt, UINT nFlags)
{
}
*/

// Mouse Move Message
void CHSMenu::OnMouseMove(HWND hWnd, short x, short y)
{
	// No need to Call function "GetNewMenu" when this menu have no submenu
	if ( GetState( MENU_POPUPED ) || GetState( MENU_CHILD) )
	{
		CHSMenu *pNewMenu;
		// If Mouse move on a other HSMenu Change Capture
		if ( ( pNewMenu = GetNewMenu( hWnd, x, y ) ) != m_pHSMenu )
		{
			if ( m_nCurSel != m_nCurPop )
			{
				RedrawItem( hWnd, m_nCurPop, m_nCurSel );
				TimerDelay( hWnd );
			}

			ReleaseCapture();
			// Reset Capture To New (Under Point)Menu
			SetCapture( pNewMenu->m_hMenu );
			return;
		}
	}

	// Get the new item be selected now
	int	nNewSel = GetNewSel( x, y );
	// if Select no change return
	if ( nNewSel != m_nCurSel )
	{
		RedrawItem( hWnd, nNewSel, m_nCurSel );
		TimerDelay( hWnd );
	}
}

void CHSMenu::RedrawItem(HWND hWnd, int nCurSel, int nOldSel)
{
	DRAWITEMSTRUCTPRO	disp;

	disp.hDC = GetDC( hWnd );
	disp.rect.left  = m_nMenuEdge;
	disp.rect.right	= m_nMenuWidth - m_nMenuEdge;
	
	// Clear Old selected item background
	if ( nOldSel != SELNONE )
	{	
		SetState( MENU_SELECTED, false );		// Flag for Clear

		HSITEM& MenuItem = m_qMenuItem[nOldSel];

		disp.pData		 = MenuItem.m_pData;
		disp.uFlags		 = MenuItem.m_uFlags | m_uMenuFlag;
		disp.lpcStr		 = MenuItem.m_strName.c_str();	
		disp.rect.top	 = MenuItem.m_nTop;
		disp.rect.bottom = MenuItem.m_nTop + MenuItem.m_nHeight;

		m_pHSMenu->DrawItem( &disp );
	}

	// Draw the new select item background
	if ( nCurSel != SELNONE )
	{
		SetState( MENU_SELECTED, true );	// Draw Select 

		HSITEM& MenuItem = m_qMenuItem[nCurSel];

		disp.pData		 = MenuItem.m_pData;
		disp.uFlags		 = MenuItem.m_uFlags | m_uMenuFlag;
		disp.lpcStr		 = MenuItem.m_strName.c_str();	
		disp.rect.top	 = MenuItem.m_nTop;
		disp.rect.bottom = MenuItem.m_nTop + MenuItem.m_nHeight;

		m_pHSMenu->DrawItem( &disp );
	}

	ReleaseDC( hWnd, disp.hDC );
	m_nCurSel = nCurSel;
}

void CHSMenu::OnTimer(HWND hWnd, unsigned short uID)
{
	KillTimer( hWnd, uID );

	switch ( uID )
	{ 
		case TIMERTRACK:
			if ( !GetState( MENU_POPUPED ) && m_nCurSel != SELNONE )
			{
				POINT	ptDisp;	
				const	HSITEM& MenuItem = m_qMenuItem[m_nCurSel];
				// the init disp point
				ptDisp.x = m_nMenuWidth - m_nMenuEdge ;
				ptDisp.y = MenuItem.m_nTop - m_nMenuEdge - MENUCAPTION;
				ClientToScreen( hWnd, &ptDisp );
				// track the new menu
				MenuItem.m_pPopupHSMenu->TrackPopMenu( ptDisp.x, ptDisp.y );
			}
			else
			SetTimer( hWnd, TIMERTRACK, 10, NULL );
			break;

		case TIMERDESTROY:
			SetState( MENU_TIMERDESTROY, false );
			DestroyAllPop();
			break;
	}
}

void CHSMenu::TimerDelay(HWND hWnd)
{
	KillTimer( hWnd, TIMERTRACK );

	if ( !GetState( MENU_TIMERDESTROY ) && GetState( MENU_POPUPED ) )
	{
		SetState( MENU_TIMERDESTROY, true );
		SetTimer( hWnd, TIMERDESTROY, m_nPopupDelay, NULL );
	}

	// Draw the new select item background
	if ( m_nCurSel != SELNONE )
	{
		if ( m_nCurSel == m_nCurPop )
		{
			KillTimer( hWnd, TIMERTRACK );
			KillTimer( hWnd, TIMERDESTROY );
			SetState( MENU_TIMERDESTROY, false );
		}
		else	// Popup a New Menu ?
		{
			const HSITEM& MenuItem = m_qMenuItem[m_nCurSel];

			if (  MenuItem.m_pPopupHSMenu && !(MenuItem.m_uFlags & ITEM_GRAY) )
			{
				SetTimer( hWnd, TIMERTRACK, m_nPopupDelay, NULL );
			}
		}
	}
}

// Draw the menu item which you want
void CHSMenu::DrawItem(LPDRAWITEMSTRUCTPRO lpdisp)
{	
	// Draw a separator
	if ( lpdisp->uFlags & ITEM_SEPARATOR )
	{
		DrawItemSpr( lpdisp );
		return;
	}

	// Draw or Clear Select Background
	DrawItemBkgnd( lpdisp );

	// Draw the Menu item Text
	DrawItemText( lpdisp );

	// if this Item has son, then Graphics arrowhead
	if ( lpdisp->uFlags & ITEM_POPUP )
		DrawItemArrow( lpdisp );
}

// Draw(Selected) or Clear(Leave) the item Background 
void CHSMenu::DrawItemBkgnd(LPDRAWITEMSTRUCTPRO lpdisp)
{
	int		nClrBorder, nClrBk;
	RECT&	rect = lpdisp->rect;

	if ( GetState( MENU_SELECTED ) )
	{
		nClrBk	   = CLR_BKGNDSEL;
		nClrBorder = CLR_BORDERSEL;
	}
	else
	{
		nClrBk	   = CLR_BKGNDMENU;
		nClrBorder = CLR_BKGNDMENU;
	}

	DrawRect(	lpdisp->hDC, 
				m_clrMenu[nClrBorder], 
				m_clrMenu[nClrBk], 
				rect.left, 
				rect.top, 
				rect.right, 
				rect.bottom	 );
}

// Draw Menu Item's Name
void CHSMenu::DrawItemText(LPDRAWITEMSTRUCTPRO lpdisp)
{
	HGDIOBJ		hfntOld;
	COLORREF	clrText;
	RECT		rect;
	RECT&		rectT = lpdisp->rect;

	rect.left   = rectT.left + 20;
	rect.top    = ( rectT.top + rectT.bottom - m_nFontHeight ) / 2;
	rect.right	= 1000;
	rect.bottom	= 100;
	
	if ( lpdisp->uFlags & ITEM_GRAY )	 
		clrText = GetSysColor( COLOR_GRAYTEXT );
	else
		clrText = m_clrMenu[GetState( MENU_SELECTED ) ? CLR_TEXTSEL : CLR_TEXTNORMAL];
	
	SetTextColor( lpdisp->hDC, clrText );
	SetBkMode( lpdisp->hDC, TRANSPARENT );
	hfntOld = SelectObject( lpdisp->hDC, m_hFont );

	USES_CONVERSION;
	//多字节转Unicode

	DrawText( lpdisp->hDC, A2W(lpdisp->lpcStr), -1, &rect, DT_NOCLIP|DT_LEFT ); 
	SelectObject( lpdisp->hDC, hfntOld );
}

// Draw Separator Item
void CHSMenu::DrawItemSpr(LPDRAWITEMSTRUCTPRO lpdisp)
{
	POINT		pt[2];
	RECT&		rect = lpdisp->rect;

	pt[0].x = rect.left;
	pt[1].x = rect.right;
	pt[0].y = pt[1].y = ( rect.top + rect.bottom ) / 2;

	DrawPolyline( lpdisp->hDC, m_clrMenu[CLR_BORDERSPR], pt, 2 );
}

// Draw Item Arrow ( Only Popup Item has )
void CHSMenu::DrawItemArrow(LPDRAWITEMSTRUCTPRO lpdisp)
{
	RECT&		rect = lpdisp->rect;

	int clrArrow;

	if ( lpdisp->uFlags & ITEM_GRAY )
		clrArrow = GetSysColor( COLOR_GRAYTEXT );
	else
		clrArrow = m_clrMenu[GetState( MENU_SELECTED ) ? CLR_ARROWSEL : CLR_TEXTNORMAL];

	DrawArrow( lpdisp->hDC, clrArrow, rect.right-8, (rect.top+rect.bottom)/2, 4 );
}

bool CHSMenu::IsEffectItem()
{	
	if ( m_nCurSel == SELNONE ) return false;

	UINT uFlag = m_qMenuItem[m_nCurSel].m_uFlags; 

	return ( ( uFlag & ITEM_POPUP     ) == 0 && 
			 ( uFlag & ITEM_DISABLE   ) == 0 &&
			 ( uFlag & ITEM_GRAY      ) == 0 &&
			 ( uFlag & ITEM_SEPARATOR ) == 0
			);
}

bool CHSMenu::IsOutMenu(short x, short y)
{
	return ( x<0 || x>m_nMenuWidth || y<0 || y>m_nMenuHeight );
}

void CHSMenu::OnPaint(HWND hWnd, HDC hDC)
{
	DRAWITEMSTRUCTPRO	disp;
	DEQUEITEM::const_iterator	it, itend;

	itend = m_qMenuItem.end();

	disp.hDC = hDC;
	disp.rect.left  = m_nMenuEdge;
	disp.rect.right	= m_nMenuWidth - m_nMenuEdge;
	
	SetState( MENU_SELECTED, false );

	for ( it = m_qMenuItem.begin(); it != itend; it++ )
	{
		disp.pData		 = it->m_pData;
		disp.uFlags		 = it->m_uFlags | m_uMenuFlag;
		disp.lpcStr		 = it->m_strName.c_str();	
		disp.rect.top	 = it->m_nTop;
		disp.rect.bottom = it->m_nTop + it->m_nHeight;

		m_pHSMenu->DrawItem( &disp );
	}

	if ( m_nCurPop != SELNONE )
	{
		SetState( MENU_SELECTED, true );

		HSITEM& MenuItem = m_qMenuItem[m_nCurPop];

		disp.pData		 = MenuItem.m_pData;
		disp.uFlags		 = MenuItem.m_uFlags | m_uMenuFlag;
		disp.lpcStr		 = MenuItem.m_strName.c_str();	
		disp.rect.top	 = MenuItem.m_nTop;
		disp.rect.bottom = MenuItem.m_nTop + MenuItem.m_nHeight;

		m_pHSMenu->DrawItem( &disp );
	}
}

void CHSMenu::OnLButtonDown(HWND hWnd, short x, short y)
{
	if ( IsOutMenu( x, y ) ) 
		DestroyAllMenu();
}

void CHSMenu::OnLButtonUp(HWND hWnd, short x, short y)
{
	if ( IsOutMenu( x, y ) || !IsEffectItem() )  return;

	DestroyAllMenu();
//	SendMessage( m_hWndParent, WM_COMMAND, MAKEWORD( m_qMenuItem[m_nCurSel].m_uID, 0 ),0 );
	SendMessage( m_hWndParent, WM_COMMAND, m_qMenuItem[m_nCurSel].m_uID, 0 );
}

void CHSMenu::OnRButtonDown(HWND hWnd, short x, short y)
{
	if ( IsOutMenu( x, y ) ) 
		DestroyAllMenu();
}

void CHSMenu::OnRButtonUp(HWND hWnd, short x, short y)
{
}

//void CHSMenu::OnDestroy(HWND hWnd)
//{
//	SetState( MENU_POPUPED, false );
//	DestroyWindow( hWnd );
//}

// this function cycle destroy all the popup item's son
void CHSMenu::DestroyAllPop( void )
{
	if ( m_nCurPop == SELNONE )	return;

	int		 nCurPop;
	CHSMenu	*pDel, *pNext;

	pNext = m_qMenuItem[m_nCurPop].m_pPopupHSMenu;

	SetActiveWindow( m_hMenu );

	do
	{
		pDel = pNext;
		nCurPop = pDel->m_nCurPop;

		if ( nCurPop != SELNONE )
			 pNext = pDel->m_qMenuItem[nCurPop].m_pPopupHSMenu;
		else pNext = NULL;

		pDel->DestroyMenu();

	} while ( pNext );	

	m_nCurPop = SELNONE;
	SetState( MENU_POPUPED, false );
}

// Deatroy All Menu Window
void CHSMenu::DestroyAllMenu()
{
	CHSMenu	 *pDel = m_pHSMenu;
	DestroyAllPop();

	do
	{
		pDel->DestroyMenu();
		pDel = pDel->m_pParent;

	} while ( pDel );
}

inline void CHSMenu::DestroyMenu()
{
	SetState( MENU_POPUPED, false );
	DestroyWindow( m_hMenu );
}
/*
void CHSMenu::CustomMenuRgn( HWND hMenu )
{
}

void CHSMenu::UpdateMenuCXY( int nMenuWidth, int nMenuHeight )
{
}
*/
static LRESULT CALLBACK lpfnWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	CHSMenu	*pHSMenu = (CHSMenu *)( GetWindowLong( hWnd, GWL_USERDATA ) );

	switch ( uMsg )
	{
		case WM_MOUSEMOVE:
			pHSMenu->OnMouseMove( hWnd, LOWORD(lParam), HIWORD(lParam) );
			break;

		case WM_TIMER:
			pHSMenu->OnTimer( hWnd, wParam );
			break;

		case WM_LBUTTONDOWN:
			pHSMenu->OnLButtonDown( hWnd, LOWORD(lParam), HIWORD(lParam) );
			break;

		case WM_LBUTTONUP:
			pHSMenu->OnLButtonUp( hWnd, LOWORD(lParam), HIWORD(lParam) );
			break;

		case WM_RBUTTONDOWN:
			pHSMenu->OnRButtonDown( hWnd, LOWORD(lParam), HIWORD(lParam) );
			break;

		case WM_RBUTTONUP:
			pHSMenu->OnRButtonUp( hWnd, LOWORD(lParam), HIWORD(lParam) );
			break;

		case WM_ERASEBKGND:
			pHSMenu->OnEraseBkgnd( hWnd, (HDC)wParam );
			break;

		case WM_PAINT:
			{
				PAINTSTRUCT	ps;
				BeginPaint( hWnd, &ps );
				pHSMenu->OnPaint( hWnd, ps.hdc );
				EndPaint( hWnd, &ps );
			}
			break;

//		case WM_MOUSEWHEEL:
//			break;

		case WM_MBUTTONDOWN:
			pHSMenu->OnRButtonDown( hWnd, LOWORD(lParam), HIWORD(lParam) );
			break;

		case WM_KEYDOWN:
			pHSMenu->OnKeyDown( hWnd, (UINT)wParam, HIWORD(lParam), LOWORD(lParam));
			break;

//		case WM_KEYUP:
//			pHSMenu->OnKeyUp( hWnd, (UINT)wParam, HIWORD(lParam), LOWORD(lParam));
//			break;

//		case WM_CHAR:
//			pHSMenu->OnChar( hWnd, (char)wParam );
//			break;

		case WM_SYSKEYDOWN:
			pHSMenu->DestroyAllMenu();
			break;

//		case WM_CAPTURECHANGED:
//			ReleaseCapture();
//			SetCapture( hWnd );
//			break;

		case WM_CREATE:
			SetWindowLong( hWnd, GWL_USERDATA, (long)( (LPCREATESTRUCT)lParam )->lpCreateParams );
			break;

//		case WM_DESTROY:
//			pHSMenu->OnDestroy( hWnd );
//			break;

		default:
			return DefWindowProc( hWnd, uMsg, wParam, lParam );
	}

	return 0;
}


// 设置菜单宽度
void CHSMenu::SetMenuWidth(int width)
{
	m_nMenuWidth = width;
}
