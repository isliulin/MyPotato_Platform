// HSGraphics.cpp: implementation of the CHSGraphics class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HSGraphics.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



void WINAPI Draw3DLine(HDC hDC, bool bHeave, const POINT& ptStart, const POINT& ptEnd)
{
	HPEN	hpen;
	HGDIOBJ hpenOld;

	hpen = ::CreatePen( PS_SOLID, 1, GetSysColor( bHeave ? COLOR_3DHILIGHT : COLOR_3DSHADOW  ) );
	hpenOld = ::SelectObject( hDC, hpen );
	::MoveToEx( hDC, ptStart.x, ptStart.y, NULL );
	::LineTo( hDC, ptEnd.x, ptEnd.y );
	::DeleteObject( hpen );
	
	hpen = ::CreatePen( PS_SOLID, 1, GetSysColor( bHeave ? COLOR_3DSHADOW : COLOR_3DHILIGHT ) );
	::SelectObject( hDC, hpen );
	::MoveToEx( hDC, ptStart.x, ptStart.y+1, NULL );
	::LineTo( hDC, ptEnd.x, ptEnd.y+1 );
	::DeleteObject( hpen );

	::SelectObject( hDC, hpenOld );
}

void WINAPI DrawRectEdge(HDC hDC, const RECT& rect, int nWidth, COLORREF clrEdge, int nStyle)
{
	HPEN	hpen	= ::CreatePen( nStyle, nWidth, clrEdge );
	HGDIOBJ	hobjOld = ::SelectObject( hDC, hpen );

	POINT	pt[3];

	pt[0].x = pt[1].x = rect.left;							//	 p1	+-----+ p2
	pt[0].y = rect.bottom;									//		|	
	pt[1].y = pt[2].y = rect.top;							//		|
	pt[2].x = rect.right;									//	 p0	+

	::Polyline( hDC, pt, 3 );

	pt[1].x = pt[2].x = rect.right;							//		      + p2
	pt[1].y = rect.bottom;									//			  |	
	pt[2].y = rect.top;										//			  |			
															//	 p0 +-----+	p1	
	::Polyline( hDC, pt, 3 );

	::SelectObject( hDC, hobjOld );
	::DeleteObject( hpen );
}

void WINAPI Draw3DRectEdge(HDC hDC, bool bHeave, int nLeft, int nTop, int nRight, int nBottom)
{
	HPEN	hpen;
	HGDIOBJ hpenOld;
	POINT	pt[3];

	pt[0].x = pt[1].x = nLeft;							//	 p1	+-----+ p2
	pt[0].y = nBottom;									//		|	
	pt[1].y = pt[2].y = nTop;							//		|
	pt[2].x = nRight + 1;								//	 p0	+
		
	hpen = ::CreatePen( PS_SOLID, 1, GetSysColor( bHeave ? COLOR_3DHILIGHT : COLOR_3DSHADOW  ) );
	hpenOld = ::SelectObject( hDC, hpen );
	::Polyline( hDC, pt, 3 );
	::DeleteObject( hpen );

	pt[1].x = pt[2].x = nRight;							//		      + p2
	pt[1].y = nBottom;									//			  |	
	pt[2].y = nTop;										//			  |			
														//	 p0 +-----+	p1	

	hpen = ::CreatePen( PS_SOLID, 1, GetSysColor( bHeave ? COLOR_3DSHADOW : COLOR_3DHILIGHT ) );
	::SelectObject( hDC, hpen );
	::Polyline( hDC, pt, 3 );
	::DeleteObject( hpen );

	::SelectObject( hDC, hpenOld );
}

void WINAPI Draw3DRgnEdge(HDC hDC, bool bHeave, HRGN rgnWnd)
{
	HBRUSH		hbr;
	HRGN		rgnTemp = ::CreateRectRgn( 0, 0, 0, 0 );

	::CombineRgn( rgnTemp, rgnWnd, 0, RGN_COPY );
	::OffsetRgn ( rgnTemp, 1, 1 );
	::CombineRgn( rgnTemp, rgnWnd, rgnTemp, RGN_DIFF );
	hbr = ::CreateSolidBrush( GetSysColor( bHeave ? COLOR_3DHILIGHT : COLOR_3DSHADOW ) );
	::FillRgn( hDC, rgnTemp, hbr );
	::DeleteObject( hbr );
	
	::CombineRgn( rgnTemp, rgnWnd, 0, RGN_COPY );
	::OffsetRgn( rgnTemp, -1, -1 );
	::CombineRgn( rgnTemp, rgnWnd, rgnTemp, RGN_DIFF );
	hbr = ::CreateSolidBrush( GetSysColor( bHeave ? COLOR_3DSHADOW : COLOR_3DHILIGHT ) );
	::FillRgn( hDC, rgnTemp, hbr );
	::DeleteObject( hbr );

	::DeleteObject( rgnTemp );
}

void WINAPI DrawGradientRgn(HDC hDC, bool bVertical, HRGN hRgn, COLORREF clrStart, COLORREF clrEnd)
{
	float	fStep;
	RECT	rect;
	HBRUSH	hbrFill;
	HRGN	hRgnFill = ::CreateRectRgn( 0, 0, 0, 0 );
	HRGN	hRgnTemp = ::CreateRectRgn( 0, 0, 0, 0 );

	::GetRgnBox ( hRgn, &rect );
	::CombineRgn( hRgnTemp, hRgn, 0, RGN_COPY );
	::OffsetRgn ( hRgnTemp, -rect.left, -rect.top );

	int	nWidth  = rect.right  - rect.left;
	int	nHeight = rect.bottom - rect.top; 

	HRGN hRgnRect = ::CreateRectRgn( 0, 0, nWidth, nHeight );

	int nR = GetRValue( clrStart );
	int nG = GetGValue( clrStart );
	int nB = GetBValue( clrStart );

	float fRStep = ( GetRValue( clrEnd ) - GetRValue( clrStart ) ) / (float)STEP;
	float fGStep = ( GetGValue( clrEnd ) - GetGValue( clrStart ) ) / (float)STEP;
	float fBStep = ( GetBValue( clrEnd ) - GetBValue( clrStart ) ) / (float)STEP;

	HDC		memDC   = ::CreateCompatibleDC( hDC );
	HBITMAP	bmp     = ::CreateCompatibleBitmap( hDC, nWidth, nHeight );
	HGDIOBJ hobjOld = ::SelectObject( memDC, bmp );

	hbrFill = ::CreateSolidBrush( 0x00FFFFFF );	// White
	::FillRgn( memDC, hRgnRect, hbrFill );
	::DeleteObject( hbrFill );

	if ( bVertical )
	{
		int nBottom, nTop = 0; 

		fStep = (float)nHeight / (float)STEP;

		for ( int nCount=0; nCount<=STEP; nCount++ )
		{
			nBottom = (int)( (nCount+1) * fStep );

			::SetRectRgn( hRgnRect, 0, nTop, nWidth, nBottom );
			::CombineRgn( hRgnFill, hRgnRect, hRgnTemp, RGN_AND );

			hbrFill = ::CreateSolidBrush( RGB( nR+fRStep*nCount, nG+fGStep*nCount, nB+fBStep*nCount ) );
			::FillRgn( memDC, hRgnFill, hbrFill );
			::DeleteObject( hbrFill );

			nTop = nBottom;
		}
	}
	else
	{
		int nRight, nLeft = 0;

		fStep = (float)nWidth / (float)STEP;

		for ( int nCount=0; nCount<=STEP; nCount++ )
		{
			nRight = (int)( (nCount+1) * fStep );

			::SetRectRgn( hRgnRect, nLeft, 0, nRight, nHeight );
			::CombineRgn( hRgnFill, hRgnRect, hRgnTemp, RGN_AND );

			hbrFill = ::CreateSolidBrush( RGB( nR+fRStep*nCount, nG+fGStep*nCount, nB+fBStep*nCount ) );
			::FillRgn( memDC, hRgnFill, hbrFill );
			::DeleteObject( hbrFill );

			nLeft = nRight;
		}
	}

////////////////////////////////////////////////////////////////////////////////////////////////

	HDC		hdcTmp  = ::CreateCompatibleDC( hDC );
	HBITMAP	bmpTmp  = ::CreateCompatibleBitmap( hDC, nWidth, nHeight );
	HGDIOBJ hbmpOld = ::SelectObject( hdcTmp, bmpTmp );

	HDC		hdcMsk  = ::CreateCompatibleDC( hDC );
	HBITMAP hbmpMsk = ::CreateBitmap( nWidth, nHeight, 1, 1, NULL );
	HGDIOBJ hMskOld = ::SelectObject( hdcMsk, hbmpMsk );

	::SetBkColor( memDC, 0x00FFFFFF );
	::BitBlt( hdcMsk, 0, 0, nWidth, nHeight, memDC, 0, 0, SRCCOPY );

	::BitBlt( hdcTmp, 0, 0, nWidth, nHeight, hDC, rect.left, rect.top, SRCCOPY );

	::BitBlt( hdcTmp, 0, 0, nWidth, nHeight, memDC,  0, 0, SRCINVERT );
	::BitBlt( hdcTmp, 0, 0, nWidth, nHeight, hdcMsk, 0, 0, SRCAND );
	::BitBlt( hdcTmp, 0, 0, nWidth, nHeight, memDC,  0, 0, SRCINVERT );

	::BitBlt( hDC, rect.left, rect.top, nWidth, nHeight, hdcTmp,  0, 0, SRCCOPY );

	::DeleteObject( hbmpMsk );
	::SelectObject( hdcMsk, hMskOld );
	::DeleteDC( hdcMsk );

	::SelectObject( hdcTmp, hbmpOld );
	::DeleteObject( bmpTmp );
	::DeleteDC( hdcTmp );	

////////////////////////////////////////////////////////////////////////////////////////////////

	::DeleteObject( hRgnRect );
	::DeleteObject( hRgnTemp );
	::DeleteObject( hRgnFill );

	::SelectObject( memDC, hobjOld );
	::DeleteObject( bmp );
	::DeleteDC( memDC );	
}

void WINAPI DrawGradientRect(HDC hDC, bool bVertical, const RECT &rectFill, COLORREF clrStart, COLORREF clrEnd)
{
	float	fStep;
	RECT	rect;
	HBRUSH	hbrFill;

	int		nWidth  = rectFill.right  - rectFill.left;
	int		nHeight = rectFill.bottom - rectFill.top; 

	int nR = GetRValue( clrStart );
	int nG = GetGValue( clrStart );
	int nB = GetBValue( clrStart );

	float fRStep = ( GetRValue( clrEnd ) - GetRValue( clrStart ) ) / (float)STEP;
	float fGStep = ( GetGValue( clrEnd ) - GetGValue( clrStart ) ) / (float)STEP;
	float fBStep = ( GetBValue( clrEnd ) - GetBValue( clrStart ) ) / (float)STEP;

	HDC		memDC   = ::CreateCompatibleDC( hDC );
	HBITMAP	bmp     = ::CreateCompatibleBitmap( hDC, nWidth, nHeight );
	HGDIOBJ hobjOld = ::SelectObject( memDC, bmp );

	rect.top = rect.left = 0;

	if ( bVertical )
	{
		rect.right	= nWidth;
		fStep		= (float)nHeight / (float)STEP;

		for ( int nCount=0; nCount<=STEP; nCount++ )
		{
			rect.bottom = (int)( (nCount+1) * fStep );
	
			hbrFill = ::CreateSolidBrush( RGB( nR+fRStep*nCount, nG+fGStep*nCount, nB+fBStep*nCount ) );
			::FillRect( memDC, &rect, hbrFill );
			::DeleteObject( hbrFill );

			rect.top = rect.bottom;
		}
	}
	else
	{
		rect.bottom = nHeight;
		fStep		= (float)nWidth / (float)STEP;

		for ( int nCount=0; nCount<=STEP; nCount++ )
		{
			rect.right	= (int)( (nCount+1) * fStep );

			hbrFill = ::CreateSolidBrush( RGB( nR+fRStep*nCount, nG+fGStep*nCount, nB+fBStep*nCount ) );
			::FillRect( memDC, &rect, hbrFill );
			::DeleteObject( hbrFill );

			rect.left = rect.right;
		}
	}
	
	::BitBlt( hDC, rectFill.left, rectFill.top, nWidth, nHeight, memDC, 0, 0, SRCCOPY );

	::SelectObject( memDC, hobjOld );
	::DeleteObject( bmp );
	::DeleteDC( memDC );
}

void WINAPI DrawLightPointRect(HDC hDC, const RECT &rectFill, const POINT &pointLight, COLORREF clrBkgnd, COLORREF clrPoint, float fFouce)
{
	HBRUSH	hbrFill;

	int		nStep;

	int nX = pointLight.x - rectFill.left;
	int nY = pointLight.y - rectFill.top;

	int	nWidth  = rectFill.right  - rectFill.left;
	int	nHeight = rectFill.bottom - rectFill.top; 

	int nR = GetRValue( clrPoint );
	int nG = GetGValue( clrPoint );
	int nB = GetBValue( clrPoint );

	float fRStep = ( GetRValue( clrBkgnd ) - GetRValue( clrPoint ) ) / (float)STEP;
	float fGStep = ( GetGValue( clrBkgnd ) - GetGValue( clrPoint ) ) / (float)STEP;
	float fBStep = ( GetBValue( clrBkgnd ) - GetBValue( clrPoint ) ) / (float)STEP;

	HDC		memDC   = ::CreateCompatibleDC( hDC );
	HBITMAP	bmp     = ::CreateCompatibleBitmap( hDC, nWidth, nHeight );
	HGDIOBJ hobjOld = ::SelectObject( memDC, bmp );

	HRGN	rgnTemp;
	HRGN	rgnRect	 = ::CreateRectRgn( 0, 0, nWidth, nHeight );
	HRGN	rgnLight = ::CreateEllipticRgn( nX-1, nY-1, nX+1, nY+1 );

	hbrFill = ::CreateSolidBrush( clrBkgnd );
	::FillRgn( memDC, rgnRect, hbrFill );
	::DeleteObject( hbrFill );

	hbrFill = ::CreateSolidBrush( clrPoint );
	::FillRgn( memDC, rgnLight, hbrFill );
	::DeleteObject( hbrFill );

	if ( fFouce == 0.0f )
	{
		float fMax = (float)hypot( max(abs(nX-rectFill.right),abs(nX-rectFill.left)), max(abs(nY-rectFill.bottom),abs(nY-rectFill.top)) );
		fFouce = ( fMax / (float)STEP );
	}

	for ( int nCount=0; nCount<STEP; nCount++ )
	{
		nStep = (int)( fFouce * ( nCount + 1 ) );

		rgnTemp  = ::CreateEllipticRgn( nX-nStep, nY-nStep, nX+nStep, nY+nStep );
		::CombineRgn( rgnLight, rgnTemp, rgnLight, RGN_DIFF );
//		::CombineRgn( rgnLight, rgnRect, rgnLight, RGN_AND );
		::DeleteObject( rgnTemp );

		hbrFill = ::CreateSolidBrush( RGB( nR+fRStep*nCount, nG+fGStep*nCount, nB+fBStep*nCount ) );
		::FillRgn( memDC, rgnLight, hbrFill );
		::DeleteObject( hbrFill );

		::DeleteObject( rgnLight );
		rgnLight = ::CreateEllipticRgn( nX-nStep, nY-nStep, nX+nStep, nY+nStep );
	}

	::BitBlt( hDC, rectFill.left, rectFill.top, nWidth, nHeight, memDC, 0, 0, SRCCOPY );

	::DeleteObject( bmp );
	::DeleteObject( rgnRect );
	::DeleteObject( rgnLight );
	::SelectObject( memDC, hobjOld );
	::DeleteDC( memDC );
}

void WINAPI DrawRect(HDC hDC, COLORREF clrBorder, COLORREF clrBkgnd, int nLeft, int nTop, int nRight, int nBottom)
{
	HPEN	 hpen;
	HBRUSH	 hbr;
	HGDIOBJ	 hpenOld, hbrOld;

	hpen = ::CreatePen( PS_SOLID, 1, clrBorder );
	hbr  = ::CreateSolidBrush( clrBkgnd );

	hpenOld = ::SelectObject( hDC, hpen );
	hbrOld  = ::SelectObject( hDC, hbr );
	// Draw rectangle item	
	::Rectangle( hDC, nLeft, nTop, nRight, nBottom );

	::SelectObject( hDC, hpenOld );
	::SelectObject( hDC, hbrOld );

	::DeleteObject( hpen );
	::DeleteObject( hbr );
}

void WINAPI DrawArrow(HDC hDC, COLORREF clrArrow, int nX, int nY, int nSize, int nDirection)
{
	POINT	pt[2];
	HPEN	hpen;

	pt[0].x = pt[1].x = nX;
	pt[0].y = nY;
	pt[1].y = pt[0].y + 1;

	hpen = ::CreatePen( PS_SOLID, 1, clrArrow );
	HGDIOBJ hpenOld = ::SelectObject( hDC, hpen );

	switch( nDirection )
	{
		case DIR_RIGHT:
			for ( int i=0; i<nSize; i++ )
			{
				Polyline( hDC, pt, 2 );

				pt[0].x--;
				pt[0].y--;
				pt[1].x--;
				pt[1].y++;
			}
			break;
	}

	::SelectObject( hDC, hpenOld );

	::DeleteObject( hpen );
}

void WINAPI DrawRoundRect(HDC hDC, COLORREF clrBorder, COLORREF clrBkgnd, const RECT &rect, int nWidth, int nHeight)
{
	DrawRoundRect( hDC, clrBorder, clrBkgnd, rect.left, rect.top, rect.right, rect.bottom, nWidth, nHeight );
}

void WINAPI DrawRoundRect(HDC hDC, COLORREF clrBorder, COLORREF clrBkgnd, int nLeft, int nTop, int nRight, int nBottom, int nWidth, int nHeight)
{
	HPEN	 hpen;
	HBRUSH	 hbr;
	HGDIOBJ	 hpenOld, hbrOld;

	hpen = ::CreatePen( PS_SOLID, 1, clrBorder );
	hbr  = ::CreateSolidBrush( clrBkgnd );

	hpenOld = ::SelectObject( hDC, hpen );
	hbrOld  = ::SelectObject( hDC, hbr );
	// Draw rectangle item	
	::RoundRect( hDC, nLeft, nTop, nRight, nBottom, nWidth, nHeight );

	::SelectObject( hDC, hpenOld );
	::SelectObject( hDC, hbrOld );

	::DeleteObject( hpen );
	::DeleteObject( hbr );
}

void WINAPI DrawPartionRgn(HDC hDC, COLORREF clrLeftTop, COLORREF clrRightBottom, bool bVerticalLine, int nOffset, HRGN hRgn)
{
	RECT rect;

	::GetRgnBox( hRgn, &rect );

	HRGN hRgnFill = ::CreateRectRgn( 0, 0, 0, 0 );
	HRGN hRgnTemp = ::CreateRectRgn( rect.left, rect.top, rect.right, rect.bottom );

	if ( bVerticalLine )
		 ::OffsetRgn( hRgnTemp, nOffset, 0 );
	else ::OffsetRgn( hRgnTemp, 0, nOffset );

	::CombineRgn( hRgnFill, hRgn, hRgnTemp, RGN_DIFF );
	::DrawRgn( hDC, clrLeftTop, hRgnFill );

	::CombineRgn( hRgnFill, hRgn, hRgnTemp, RGN_AND );
	::DrawRgn( hDC, clrRightBottom, hRgnFill );

	::DeleteObject( hRgnFill );
	::DeleteObject( hRgnTemp );
}

void WINAPI DrawRgn(HDC hDC, COLORREF clrBkgnd, HRGN hRgn)
{
	HBRUSH hbr = ::CreateSolidBrush( clrBkgnd );
	::FillRgn( hDC, hRgn, hbr );
	::DeleteObject( hbr );	
}

void WINAPI DrawFrameRgn( HDC hDC, HRGN hRgn, COLORREF clrEdge, int nWidth, int nHeight )
{
	HBRUSH hbr = ::CreateSolidBrush( clrEdge );
	::FrameRgn( hDC, hRgn, hbr, nWidth, nHeight );
	::DeleteObject( hbr );	
}

void WINAPI DrawEllipse(HDC hDC, COLORREF clrBorder, COLORREF clrBkgnd, int nLeft, int nTop, int nRight, int nBottom)
{
	HPEN	 hpen;
	HBRUSH	 hbr;
	HGDIOBJ	 hpenOld, hbrOld;

	hpen = ::CreatePen( PS_SOLID, 1, clrBorder );
	hbr  = ::CreateSolidBrush( clrBkgnd );

	hbrOld  = ::SelectObject( hDC, hbr );
	hpenOld = ::SelectObject( hDC, hpen );

	::Ellipse( hDC, nLeft, nTop, nLeft + 6, nTop + 6 );

	::SelectObject( hDC, hbrOld );
	::SelectObject( hDC, hpenOld );

	::DeleteObject( hpen );
	::DeleteObject( hbr );
}

void WINAPI DrawPolyline(HDC hDC, COLORREF clrLine, POINT *pt, int nSize)
{
	HPEN hpen = ::CreatePen( PS_SOLID, 1, clrLine );
	HGDIOBJ	hpenOld = ::SelectObject( hDC, hpen );

	::Polyline( hDC, pt, nSize );

	::SelectObject( hDC, hpenOld );
	::DeleteObject( hpen );
}

void WINAPI DrawTranBmp(HDC hDC, int nX, int nY, HBITMAP hBmp, int nWidth, int nHeight, COLORREF clrTran)
{
	HDC			hdcMem, hdcMsk;
	HBITMAP		hbmpMsk;
	HGDIOBJ		hbmpOld, hbmpMskOld;

	hdcMem = ::CreateCompatibleDC( hDC );
	hdcMsk = ::CreateCompatibleDC( hDC );

	hbmpOld = ::SelectObject( hdcMem, hBmp );

	hbmpMsk = ::CreateBitmap( nWidth, nHeight, 1, 1, NULL );
	hbmpMskOld = ::SelectObject( hdcMsk, hbmpMsk );

	::SetBkColor( hdcMem, clrTran );
	::BitBlt( hdcMsk, 0, 0, nWidth, nHeight, hdcMem, 0, 0, SRCCOPY );

	::BitBlt( hDC, nX, nY, nWidth, nHeight, hdcMem, 0, 0, SRCINVERT );
	::BitBlt( hDC, nX, nY, nWidth, nHeight, hdcMsk, 0, 0, SRCAND );
	::BitBlt( hDC, nX, nY, nWidth, nHeight, hdcMem, 0, 0, SRCINVERT );

	::DeleteObject( hbmpMsk );
	::SelectObject( hdcMsk, hbmpMskOld );
	::SelectObject( hdcMem, hbmpOld );
	::DeleteDC( hdcMem );
	::DeleteDC( hdcMsk );
}

void WINAPI DrawTranBmp(HDC hDC, int nX, int nY, UINT uID, COLORREF clrTran)
{
	HDC			hdcMem, hdcTemp;
	HBITMAP		hbmp, hbmpMask;
	HGDIOBJ		hbmpOld, hbmpMaskOld;
	BITMAP		bmpInfo;

	hdcMem  = ::CreateCompatibleDC( hDC );
	hdcTemp = ::CreateCompatibleDC( hDC );

	hbmp = ::LoadBitmap( AfxFindResourceHandle( MAKEINTRESOURCE( uID ), RT_BITMAP ), MAKEINTRESOURCE( uID ) );
	::GetObject( hbmp, sizeof(BITMAP), &bmpInfo );

	int nWidth  = bmpInfo.bmWidth;
	int nHeight = bmpInfo.bmHeight;

	hbmpOld = ::SelectObject( hdcMem, hbmp );

	hbmpMask = ::CreateBitmap( nWidth, nHeight, 1, 1, NULL );
	hbmpMaskOld = ::SelectObject( hdcTemp, hbmpMask );

	::SetBkColor( hdcMem, clrTran );
	::BitBlt( hdcTemp, 0, 0, nWidth, nHeight, hdcMem, 0, 0, SRCCOPY );

	::BitBlt( hDC, nX, nY, nWidth, nHeight, hdcMem,  0, 0, SRCINVERT );
	::BitBlt( hDC, nX, nY, nWidth, nHeight, hdcTemp, 0, 0, SRCAND );
	::BitBlt( hDC, nX, nY, nWidth, nHeight, hdcMem,  0, 0, SRCINVERT );

	::DeleteObject( hbmpMask );
	::SelectObject( hdcMem, hbmpOld );
	::SelectObject( hdcTemp, hbmpMaskOld );
	::DeleteDC( hdcMem );
	::DeleteDC( hdcTemp );
}

// Note : Initialize parameter "hrgn" by CreateRectRgn(0,0,0,0) first !
//		  Remember to DeleteObject( hrgn ) !
void WINAPI RgnFromBmp(HRGN hrgn, HDC hDC, UINT uID, COLORREF clrTran)
{
	HBITMAP	hbmp;
	HGDIOBJ	hbmpOld;
	HDC		hdcMem;
	BITMAP	bmpInfo;
	HRGN	hrgnBmp, hrgnTemp;
	
	hbmp = ::LoadBitmap( AfxFindResourceHandle( MAKEINTRESOURCE( uID ), RT_BITMAP ), 
						 MAKEINTRESOURCE( uID ) ); 

	hdcMem  = ::CreateCompatibleDC( hDC );
	hbmpOld = ::SelectObject( hdcMem, hbmp );

	::GetObject( hbmp, sizeof(BITMAP), &bmpInfo );
	int nWidth  = bmpInfo.bmWidth;
	int nHeight = bmpInfo.bmHeight;

	hrgnBmp = ::CreateRectRgn( 0, 0, nWidth, nHeight );

	for ( int nX=0; nX<nWidth; nX++ )
		for ( int nY=0; nY<nHeight; nY++ )
			if ( GetPixel( hdcMem, nX, nY ) == clrTran )
			{
				hrgnTemp = ::CreateRectRgn( nX, nY, nX+1, nY+1 );
				::CombineRgn( hrgnBmp, hrgnBmp, hrgnTemp, RGN_DIFF );
				::DeleteObject( hrgnTemp );
			}

	::CombineRgn( hrgn, hrgnBmp, 0, RGN_COPY );	

	::DeleteObject( hrgnBmp );
	::DeleteObject( hrgnTemp );
	::DeleteObject( hbmp );
	::SelectObject( hdcMem, hbmpOld );
	::DeleteDC( hdcMem );
}

void WINAPI DrawRect(HDC hDC, COLORREF clrFill, int nLeft, int nTop, int nRight, int nBottom)
{
	RECT	rect;

	rect.left	= nLeft;
	rect.top	= nTop;
	rect.right  = nRight;
	rect.bottom = nBottom;

	HBRUSH	br	= ::CreateSolidBrush( clrFill );
	::FillRect( hDC, &rect, br );
	::DeleteObject( br );
}

void WINAPI CreateRoundRectRgn( HRGN hRgn, const RECT &rect, int nWidth, int nHeight, bool bLeftTopR, bool bLeftBottomR, bool bRightTopR, bool bRightBottomR)
{
	int nXCenter = (rect.left+rect.right)/2;
	int nYCenter = (rect.top+rect.bottom)/2;

	HRGN hRect  = ::CreateRectRgn( rect.left, rect.top, nXCenter, nYCenter );
	HRGN hRound = ::CreateRoundRectRgn( rect.left, rect.top, rect.right+1, rect.bottom+1, nWidth, nHeight );

	if ( !bLeftTopR )
	{
		::CombineRgn( hRound, hRound, hRect, RGN_OR );
	}

	if ( !bLeftBottomR )
	{
		::SetRectRgn( hRect, rect.left, nYCenter, nXCenter, rect.bottom );
		::CombineRgn( hRound, hRound, hRect, RGN_OR );
	}

	if ( !bRightTopR )
	{
		::SetRectRgn( hRect, nXCenter, rect.top, rect.right, nYCenter );
		::CombineRgn( hRound, hRound, hRect, RGN_OR );
	}

	if ( !bRightBottomR )
	{
		::SetRectRgn( hRect, nXCenter, nYCenter, rect.right, rect.bottom );
		::CombineRgn( hRound, hRound, hRect, RGN_OR );
	}

	::CombineRgn( hRgn, hRound, 0, RGN_COPY );

	::DeleteObject( hRect );
	::DeleteObject( hRound );
}



void WINAPI GetBitmapRgn(HRGN hrgn, HDC hDC, UINT uID, COLORREF clrTran)
{
	HBITMAP	hbmp;
	HGDIOBJ	hbmpOld;
	HDC		hdcMem;
	BITMAP	bmpInfo;
	HRGN	hrgnBmp, hrgnTemp;
	
	hbmp = ::LoadBitmap( AfxFindResourceHandle( MAKEINTRESOURCE( uID ), RT_BITMAP ), 
					     MAKEINTRESOURCE( uID ) ); 

	hdcMem  = ::CreateCompatibleDC( hDC );
	hbmpOld = ::SelectObject( hdcMem, hbmp );

	::GetObject( hbmp, sizeof(BITMAP), &bmpInfo );
	int nWidth  = bmpInfo.bmWidth;
	int nHeight = bmpInfo.bmHeight;

	hrgnBmp = ::CreateRectRgn( 0, 0, nWidth, nHeight );

	for ( int nX=0; nX<nWidth; nX++ )
		for ( int nY=0; nY<nHeight; nY++ )
			if ( GetPixel( hdcMem, nX, nY ) == clrTran )
			{
				hrgnTemp = ::CreateRectRgn( nX, nY, nX+1, nY+1 );
				::CombineRgn( hrgnBmp, hrgnBmp, hrgnTemp, RGN_DIFF );
				::DeleteObject( hrgnTemp );
			}

	::CombineRgn( hrgn, hrgnBmp, 0, RGN_COPY );	

	::DeleteObject( hrgnBmp );
	::DeleteObject( hrgnTemp );
	::DeleteObject( hbmp );
	::SelectObject( hdcMem, hbmpOld );
	::DeleteDC( hdcMem );
}

void WINAPI DrawFadeBmp( HDC hDC, int nX, int nY, HBITMAP hBmp, COLORREF clrTarget, UINT nRate )
{
	BITMAPINFO bmpinfo;
	float fRate = nRate / (float)255;

	::ZeroMemory( &bmpinfo, sizeof( BITMAPINFO ) );
	bmpinfo.bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
	::GetDIBits( hDC, hBmp, 0, 0, NULL, &bmpinfo, DIB_RGB_COLORS );
	
	int nWidth   = bmpinfo.bmiHeader.biWidth;
	int nHeight  = bmpinfo.bmiHeader.biHeight;
	int nBmpSize = ( nWidth * bmpinfo.bmiHeader.biBitCount + 31 ) / 32 * 8 *nHeight;

	BYTE *pData, *pBuffer;
	pData = pBuffer = new BYTE[ nBmpSize ];
	::ZeroMemory( pBuffer, nBmpSize );

	bmpinfo.bmiHeader.biBitCount = 24;
	bmpinfo.bmiHeader.biCompression = BI_RGB;
	bmpinfo.bmiHeader.biSizeImage = 0;

	::GetDIBits( hDC, hBmp, 0, nHeight, pBuffer, &bmpinfo, DIB_RGB_COLORS );

	int nR = GetRValue( clrTarget );
	int nG = GetGValue( clrTarget );
	int nB = GetBValue( clrTarget );

	for ( int nBmpY=0; nBmpY<nHeight; nBmpY++ )
		for ( int nBmpX=0; nBmpX<nWidth; nBmpX++ )
		{
//			pData = pBuffer + nBmpY*nWidth*3 + nBmpX*3;
			
			*pData = (BYTE)(  nB + (((*pData) - nB)/(float)STEP) * nRate ); 	pData++;
			*pData = (BYTE)(  nG + (((*pData) - nG)/(float)STEP) * nRate ); 	pData++;
			*pData = (BYTE)(  nR + (((*pData) - nR)/(float)STEP) * nRate );		pData++;
		}

	HBITMAP hBmpTmp = ::CreateDIBitmap( hDC, &bmpinfo.bmiHeader, CBM_INIT, pBuffer, &bmpinfo, DIB_RGB_COLORS ); 
	HDC		hdcMem  = ::CreateCompatibleDC( hDC );
	HBITMAP hbmpOld = (HBITMAP)::SelectObject( hdcMem, hBmpTmp );
	::BitBlt( hDC, nX, nY, nWidth, nHeight, hdcMem, 0, 0, SRCCOPY );
	::SelectObject( hdcMem, hbmpOld );
	::DeleteObject( hBmpTmp );
	::DeleteDC( hdcMem );

	delete[] pBuffer;
}

void WINAPI DrawFadeIcon(HDC hDC, int nX, int nY, HICON hIcon, int nWidth, int nHeight, UINT nRate)
{
	HDC		hdcMem  = ::CreateCompatibleDC( hDC );
	HBITMAP	hbmpMem = ::CreateCompatibleBitmap( hDC, nWidth, nHeight );
	HGDIOBJ	hBmpOld = ::SelectObject( hdcMem, hbmpMem );

	::DrawRect( hdcMem, 0, 0, 0, nWidth, nHeight );
	::DrawIconEx( hdcMem, 0, 0, hIcon, nWidth, nHeight, 0, NULL, DI_NORMAL );
	::DrawFadeBmp( hdcMem, 0, 0, hbmpMem, 0, nRate );
//	::DrawFadeBmp( hDC, nX, nY, hbmpMem, 0, nRate );
//	::BitBlt( hDC, nX, nY, nWidth, nHeight, hdcMem, 0, 0, SRCCOPY );

	::DrawTranBmp( hDC, nX, nY, hbmpMem, nWidth, nHeight, 0 );

	::SelectObject( hdcMem, hBmpOld );
	::DeleteObject( hbmpMem );
	::DeleteDC( hdcMem );
}

void WINAPI DrawShadowIcon(HDC hDC, int nX, int nY, HICON hIcon, int nWidth, int nHeight, COLORREF clrShadow)
{
	HDC		hdcMem  = ::CreateCompatibleDC( hDC );
	HDC		hdcMsk  = ::CreateCompatibleDC( hDC );
	HBITMAP	hbmpMsk = ::CreateCompatibleBitmap( hDC, nWidth, nHeight );
	HBITMAP	hbmpMem = ::CreateCompatibleBitmap( hDC, nWidth, nHeight );
	HGDIOBJ	hBmpOld = ::SelectObject( hdcMem, hbmpMem );
	HGDIOBJ	hMskOld = ::SelectObject( hdcMsk, hbmpMsk );

	::DrawIconEx( hdcMsk, 0, 0, hIcon, nWidth, nHeight, 0, NULL, DI_MASK );
	::DrawRect( hdcMem, clrShadow, 0, 0, nWidth, nHeight );
	::BitBlt( hdcMem, 0, 0, nWidth, nHeight, hdcMsk, 0, 0, SRCPAINT );

	::BitBlt( hDC, nX, nY, nWidth, nHeight, hdcMem, 0, 0, SRCINVERT );
	::BitBlt( hDC, nX, nY, nWidth, nHeight, hdcMsk, 0, 0, SRCAND );
	::BitBlt( hDC, nX, nY, nWidth, nHeight, hdcMem, 0, 0, SRCINVERT );

	::SelectObject( hdcMsk, hMskOld );
	::SelectObject( hdcMem, hBmpOld );
	::DeleteObject( hbmpMem );
	::DeleteObject( hbmpMsk );
	::DeleteDC( hdcMem );
	::DeleteDC( hdcMsk );
}

HICON WINAPI LoadIcon(int nIconID)
{
//	ASSERT( nIconID != 0 );
	if ( nIconID )

	return (HICON)::LoadImage(	AfxFindResourceHandle( MAKEINTRESOURCE(nIconID), RT_ICON ),
								MAKEINTRESOURCE(nIconID), 
								IMAGE_ICON, 
								0, 
								0, 
								0 );

	return NULL;
}
/*
HDC WINAPI SaveBmp( HDC hDC, const RECT* pRect )
{
	int		nWidth  = pRect->right  - pRect->left;
	int		nHeight = pRect->bottom - pRect->top;
	HDC		hdcMem	= ::CreateCompatibleDC( hDC );
	HBITMAP	hBmp	= ::CreateCompatibleBitmap( hDC, nWidth, nHeight );
	::SelectObject( hdcMem, hBmp );
	::BitBlt( hdcMem, pRect->left, pRect->top, nWidth, nHeight, hDC, 
}
*/