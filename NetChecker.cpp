#include "stdafx.h"
#include "NetChecker.h"

CComModule _NetModule;

class ATL_NO_VTABLE CEventSink
	: public CComObjectRootEx<CComSingleThreadModel>
	, public INetworkListManagerEvents
{
	DECLARE_NO_REGISTRY()
	BEGIN_COM_MAP(CEventSink)
		COM_INTERFACE_ENTRY(INetworkListManagerEvents)
	END_COM_MAP()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

public:
	void SetParent(CNetChecker *pParent){m_pParent = pParent;}

protected:
	HRESULT FinalConstruct(){return S_OK;}
	void FinalRelease(){}

	STDMETHOD(ConnectivityChanged)(NLM_CONNECTIVITY NewConnectivity)
	{
		UNREFERENCED_PARAMETER(NewConnectivity);
		m_pParent->Check();
		return S_OK;
	}

private:
	CNetChecker *m_pParent;
};

DWORD CALLBACK NetThreadProc(LPVOID pArg)
{
	((CNetChecker*)pArg)->ThreadProc();
	return 0;
}

CNetChecker::CNetChecker(CWnd *pMainWnd)
	: CChecker(pMainWnd)
	, m_dwThreadId(0)
	, m_hThread(NULL)
{
}

CNetChecker::~CNetChecker()
{
	if (m_dwThreadId != 0)
	{
		PostThreadMessage(m_dwThreadId, WM_QUIT, 0, 0);
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
	}
}

BOOL CNetChecker::Start()
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	CoCreateInstance(CLSID_NetworkListManager, NULL, CLSCTX_ALL,
		__uuidof(INetworkListManager), (LPVOID*)&m_pNetMgr);
	m_hThread = CreateThread(NULL, 0, &NetThreadProc, this, 0, &m_dwThreadId);
	Check();
	return TRUE;
}

BOOL CNetChecker::Check()
{
	NLM_CONNECTIVITY conn;
	m_pNetMgr->GetConnectivity(&conn);
	if (((DWORD)conn & NLM_CONNECTIVITY_IPV4_INTERNET) != 0)
	{
		Startup();
		return TRUE;
	}
	return FALSE;
}

void CNetChecker::ThreadProc()
{
	CEventSink *pEventSink = new CComObject<CEventSink>();
	pEventSink->AddRef();
	pEventSink->SetParent(this);

	CComPtr<IConnectionPointContainer> pCpc;
	m_pNetMgr->QueryInterface(__uuidof(IConnectionPointContainer),
		(void**)&pCpc);

	CComPtr<IConnectionPoint> pIcp;
	pCpc->FindConnectionPoint(__uuidof(INetworkListManagerEvents), &pIcp);
	CComPtr<IUnknown> pSink;
	pEventSink->QueryInterface(IID_IUnknown, (void**)&pSink);

	DWORD dwCookie;
	pIcp->Advise(pSink, &dwCookie);
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	BOOL bRet;
	for (MSG msg; (bRet = GetMessage(&msg, NULL, 0, 0)) != 0; )
	{ 
		if (bRet == -1)
		{
			break;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	pIcp->Unadvise(dwCookie);
	pEventSink->Release();
}
