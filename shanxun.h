
// shanxun.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CshanxunApp:
// �йش����ʵ�֣������ shanxun.cpp
//

class CshanxunApp : public CWinApp
{
public:
	CshanxunApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CshanxunApp theApp;