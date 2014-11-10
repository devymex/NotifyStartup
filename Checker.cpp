#include "stdafx.h"
#include "Chekcer.h"

CChecker::CChecker(CWnd *pMainWnd)
	: m_pMainWnd(pMainWnd)
{
}

void CChecker::AddCmd(const SHELLCMD &cmd)
{
	m_CmdList.push_back(cmd);
}

void CChecker::Startup()
{
	m_mutex.Lock();
	for (const auto &i : m_CmdList)
	{
		HINSTANCE hInst = ShellExecute(NULL, _T("open"), i.lpCmd, i.lpParams, i.lpDir, i.nShow);
		hInst = 0;
	}
	m_CmdList.clear();
	m_pMainWnd->PostMessage(WM_STARTUP);

	m_mutex.Unlock();
}

BOOL CChecker::Empty()
{
	bool br;
	m_mutex.Lock();
	br = m_CmdList.empty();
	m_mutex.Unlock();
	return br;
}
