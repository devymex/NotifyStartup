#include "stdafx.h"
#include "MainWnd.h"
#include "NetChecker.h"
#include "PnpChecker.h"


BEGIN_MESSAGE_MAP(CMainWnd, CWnd)
	ON_WM_CREATE()
	ON_MESSAGE(WM_STARTUP, OnStartup)
END_MESSAGE_MAP()

CMainWnd::~CMainWnd()
{
	for (auto i : m_Checkers)
	{
		delete i;
	}
	m_Checkers.clear();
}

int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CChecker *pChecker;
	SHELLCMD cmd = {0};

	//----------------
	pChecker = new CNetChecker(this);
	cmd.lpCmd = _T("C:\\Program Files (x86)\\Tencent\\TM\\Bin\\TM.exe");
	pChecker->AddCmd(cmd);
	cmd.lpCmd = _T("C:\\Program Files (x86)\\Bitvise SSH Client\\BvSsh.exe");
	cmd.lpParams = _T("-loginOnStartup");
	cmd.nShow = SW_HIDE;
	pChecker->AddCmd(cmd);
	m_Checkers.push_back(pChecker);
	//----------------
	TCHAR szDevInstId[] = _T("USB\\VID_054C&PID_09A6&MI_00\\6&A11EFDB&0&0000");
	pChecker = new CPnpChecker(this, szDevInstId);
	cmd.lpCmd = _T("c:\\program files (x86)\\lenovo\\thinkpad compact keyboard with trackpoint driver\\setspeed.exe");
	pChecker->AddCmd(cmd);
	cmd.lpCmd = _T("c:\\program files (x86)\\lenovo\\thinkpad compact keyboard with trackpoint driver\\osd.exe");
	pChecker->AddCmd(cmd);
	m_Checkers.push_back(pChecker);

	for (auto i : m_Checkers)
	{
		i->Start();
	}

	return 0;
}

LRESULT CMainWnd::OnStartup(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	for (auto i : m_Checkers)
	{
		if (!i->Empty())
		{
			return 0;
		}
	}
	PostQuitMessage(0);
	return 0;
}