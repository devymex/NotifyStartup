#include "stdafx.h"
#include "PnpChecker.h"

CPnpChecker::CPnpChecker(CWnd *pMainWnd, LPCTSTR lpDevInstId)
	: CChecker(pMainWnd)
	, m_lpDevInstId(lpDevInstId)
{
}

CPnpChecker::~CPnpChecker()
{
	UnregisterDeviceNotification(m_hNotify);
}

BOOL CPnpChecker::Start()
{
	if (!SetDevInstId(m_lpDevInstId))
	{
		return FALSE;
	}

	BOOL br = CWnd::CreateEx(0, AfxRegisterWndClass(0),
		_T(""), WS_OVERLAPPEDWINDOW, CRect(), NULL, 0);
	if (!br)
	{
		return FALSE;
	}

	ShowWindow(SW_HIDE);

	DEV_BROADCAST_DEVICEINTERFACE devFilter;
	devFilter.dbcc_size = sizeof(devFilter);
	devFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	devFilter.dbcc_classguid = GUID_NULL;
	m_hNotify = RegisterDeviceNotification(GetSafeHwnd(), &devFilter,
		DEVICE_NOTIFY_WINDOW_HANDLE | DEVICE_NOTIFY_ALL_INTERFACE_CLASSES);

	Check();
	return TRUE;
}

BOOL CPnpChecker::SetDevInstId(LPCTSTR lpDevInstId)
{
	HDEVINFO hDevInfo = SetupDiGetClassDevs(NULL, NULL, NULL, DIGCF_ALLCLASSES);

	SP_DEVINFO_DATA devInfoData = {0};
	devInfoData.cbSize = sizeof(devInfoData);
	for (DWORD i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &devInfoData); ++i)
	{
		TCHAR szBuf[MAX_PATH];
		CONFIGRET cr;
		cr = CM_Get_Device_ID(devInfoData.DevInst, szBuf, MAX_PATH, 0);
		if (cr == CR_SUCCESS && CString(szBuf) == lpDevInstId)
		{
			m_dwDevInst = devInfoData.DevInst;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CPnpChecker::OnWndMsg(UINT message, WPARAM wParam,
						   LPARAM lParam, LRESULT* pResult)
{
	if (message == WM_DEVICECHANGE && wParam == DBT_DEVICEARRIVAL)
	{
		Check();
	}

	return CWnd::OnWndMsg(message, wParam, lParam, pResult);
}

void CPnpChecker::Check()
{
	ULONG ulStatus = 0, ulProbNum;
	CM_Get_DevNode_Status(&ulStatus, &ulProbNum, m_dwDevInst, 0);
	if (ulStatus > 0)
	{
		Startup();
	}
}
