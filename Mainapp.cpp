#include "stdafx.h"
#include "MainApp.h"
#include "MainWnd.h"
CMainApp theApp;

CMainApp::CMainApp()
	: m_pTmpWnd(NULL)
{
}

CMainApp::~CMainApp()
{
	delete m_pTmpWnd;
}

BOOL CMainApp::InitInstance()
{
	m_pTmpWnd = new CMainWnd;
	m_pMainWnd = m_pTmpWnd;
	m_pTmpWnd->CreateEx(0, AfxRegisterWndClass(0),
		_T(""), WS_OVERLAPPEDWINDOW, CRect(), NULL, 0);

	return CWinApp::InitInstance();
}
