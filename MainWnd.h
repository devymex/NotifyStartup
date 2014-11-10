#pragma once

class CMainWnd : public CWnd
{
	DECLARE_MESSAGE_MAP()

public:
	~CMainWnd();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnStartup(WPARAM wParam, LPARAM lParam);


	std::vector<class CChecker*> m_Checkers;
};