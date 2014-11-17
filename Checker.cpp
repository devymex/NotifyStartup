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
		std::basic_string<TCHAR> strDir;
		if (i.lpDir != NULL)
		{
			strDir = i.lpDir;
		}
		if (strDir.empty())
		{
			strDir = i.lpCmd;
			int nPos = strDir.find_last_of(_T('\\'));
			int nCnt = strDir.length() - nPos;
			strDir.erase(nPos, nCnt);
		}
		HINSTANCE hInst = ShellExecute(NULL, _T("open"), i.lpCmd,
			i.lpParams, strDir.c_str(), i.nShow);
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
