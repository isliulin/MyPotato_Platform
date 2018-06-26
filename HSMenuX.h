// HSMenuX.h: interface for the CHSMenuX class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HSMENUX_H__9E3D3D71_1ED6_4252_8815_89B55587C7CC__INCLUDED_)
#define AFX_HSMENUX_H__9E3D3D71_1ED6_4252_8815_89B55587C7CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HSMenuPro.h"

class CHSMenuX : public CHSMenuPro  
{
public:
	CHSMenuX();
	virtual ~CHSMenuX();

protected:
	virtual void CustomSelRgn( HRGN &hrgnBkSel, const RECT* pRectBkSel );
	virtual void CustomMenuRgn(HWND hMenu);
};

#endif // !defined(AFX_HSMENUX_H__9E3D3D71_1ED6_4252_8815_89B55587C7CC__INCLUDED_)
