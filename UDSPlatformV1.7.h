
// UDSPlatformV1.7.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CUDSPlatformV17App:
// �йش����ʵ�֣������ UDSPlatformV1.7.cpp
//

class CUDSPlatformV17App : public CWinApp
{
public:
	CUDSPlatformV17App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CUDSPlatformV17App theApp;