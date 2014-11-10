#pragma once

#define WM_STARTUP (WM_USER + 2)

struct SHELLCMD
{
	LPCTSTR lpCmd;
	LPCTSTR lpParams;
	LPCTSTR lpDir;
	UINT nShow;
};

class CChecker
{
public:
	CChecker(CWnd *pMainWnd);
	void AddCmd(const SHELLCMD &cmd);
	virtual BOOL Start() = 0;
	BOOL Empty();

protected:
	void Startup();

	DWORD m_dwDevInst;
	std::vector<SHELLCMD> m_CmdList;

	CMutex m_mutex;

	CWnd *m_pMainWnd;
};