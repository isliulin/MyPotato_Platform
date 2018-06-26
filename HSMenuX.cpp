// HSMenuX.cpp: implementation of the CHSMenuX class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "UDS Scanner Capture v1.0.0.h"
#include "HSMenuX.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHSMenuX::CHSMenuX()
{

}

CHSMenuX::~CHSMenuX()
{

}

void CHSMenuX::CustomMenuRgn(HWND hMenu)
{
	HRGN	hRgn;
	int		nMenuWidth, nMenuHeight;

	GetMenuCXY( nMenuWidth, nMenuHeight );

	if ( GetMenuStyle( ML_ROUNDMENU ) )
//		 hRgn = CreateRoundRectRgn( 0, 0, nMenuWidth, nMenuHeight, 20, 20 );
	{
		int		nRate = 9;
		POINT	ptSel[6];

		ptSel[0].x = nRate;
		ptSel[0].y = 0;
		ptSel[1].x = nMenuWidth;
		ptSel[1].y = 0;
		ptSel[2].x = nMenuWidth;
		ptSel[2].y = nMenuHeight - nRate;
		ptSel[3].x = nMenuWidth - nRate;
		ptSel[3].y = nMenuHeight;
		ptSel[4].x = 0;
		ptSel[4].y = nMenuHeight;
		ptSel[5].x = 0;
		ptSel[5].y = nRate;

		hRgn = CreatePolygonRgn( ptSel, 6, WINDING );
	}
	else hRgn = CreateRectRgn( 0, 0, nMenuWidth, nMenuHeight );

	SetWindowRgn( hMenu, hRgn, true );
	DeleteObject( hRgn );
}

void CHSMenuX::CustomSelRgn(HRGN &hrgnBkSel, const RECT *pRectBkSel)
{
	HRGN hRgnRect, hRgnElliptic;

	if ( GetMenuStyle( ML_SELROUND ) )
	{
//		hrgnBkSel = CreateRoundRectRgn( pRectBkSel->left, pRectBkSel->top, pRectBkSel->right, pRectBkSel->bottom, 18, 18 );

	hrgnBkSel = CreateRectRgn( 0, 0, 0, 0 );
	hRgnRect = CreateRectRgn( pRectBkSel->left, pRectBkSel->top, pRectBkSel->right, pRectBkSel->bottom );
	hRgnElliptic = CreateEllipticRgn( pRectBkSel->left-20, pRectBkSel->top, pRectBkSel->left+30, pRectBkSel->bottom );

	CombineRgn( hrgnBkSel, hRgnRect, hRgnElliptic, RGN_DIFF );
	hrgnBkSel = CreateRectRgn( pRectBkSel->left, pRectBkSel->top, pRectBkSel->right, pRectBkSel->bottom );
	hrgnBkSel = CreateRoundRectRgn( pRectBkSel->left, pRectBkSel->top, pRectBkSel->right, pRectBkSel->bottom, 18, 18 );

	int		nRate = 9;
	POINT	ptSel[6];

	ptSel[0].x = pRectBkSel->left + nRate;
	ptSel[0].y = pRectBkSel->top;
	ptSel[1].x = pRectBkSel->right;
	ptSel[1].y = pRectBkSel->top;
	ptSel[2].x = pRectBkSel->right;
	ptSel[2].y = pRectBkSel->bottom - nRate ;
	ptSel[3].x = pRectBkSel->right - nRate;
	ptSel[3].y = pRectBkSel->bottom;
	ptSel[4].x = pRectBkSel->left;
	ptSel[4].y = pRectBkSel->bottom;
	ptSel[5].x = pRectBkSel->left;
	ptSel[5].y = pRectBkSel->top + nRate;

		hrgnBkSel = CreatePolygonRgn( ptSel, 6, WINDING );
	}

}
