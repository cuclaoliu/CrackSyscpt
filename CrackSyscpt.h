
// CrackSyscpt.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCrackSyscptApp: 
// �йش����ʵ�֣������ CrackSyscpt.cpp
//

class CCrackSyscptApp : public CWinApp
{
public:
	CCrackSyscptApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CCrackSyscptApp theApp;