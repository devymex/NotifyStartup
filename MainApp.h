#pragma once

class CMainApp : public CWinApp
{
public:
	CMainApp();
	~CMainApp();
	virtual BOOL InitInstance();
private:
	class CMainWnd *m_pTmpWnd;
};

extern CMainApp theApp;