#pragma once
#include "Chekcer.h"

class CPnpChecker
	: public CChecker
	, public CWnd
{
public:
	CPnpChecker(CWnd *pMainWnd, LPCTSTR lpDevInstId);
	~CPnpChecker();
	BOOL Start();

private:
	BOOL SetDevInstId(LPCTSTR lpDevInstId);
	BOOL OnWndMsg(UINT message, WPARAM wParam,
		LPARAM lParam, LRESULT* pResult);
	void Check();


	LPCTSTR m_lpDevInstId;
	DWORD m_dwDevInst;
	HDEVNOTIFY m_hNotify;
};
