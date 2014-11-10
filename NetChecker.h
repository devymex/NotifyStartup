#pragma once
#include "Chekcer.h"

class CNetChecker : public CChecker
{
public:
	CNetChecker(CWnd *pMainWnd);
	~CNetChecker();

	BOOL Start();
	BOOL Check();

	void ThreadProc();
private:
	HANDLE m_hThread;
	DWORD m_dwThreadId;

	CComPtr<INetworkListManager> m_pNetMgr;
	class CEventSink *pEventResponse;
};