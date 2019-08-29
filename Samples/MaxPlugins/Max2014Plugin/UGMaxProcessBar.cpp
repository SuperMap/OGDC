#include "UGMaxProcessBar.h"
#include "MaxPluginres.h"
#include"contextids.h" 
#include "Max.h"
#define STEPS 1000
static INT_PTR CALLBACK ProcessProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message) 
	{
case WM_INITDIALOG:
	{   
		SetWindowContextHelpId(hDlg, idh_3dsexp_export);
		CenterWindow(hDlg,GetParent(hDlg));
		return FALSE;
	}

	}
	return FALSE;
}
UGMaxProcessBar::UGMaxProcessBar(HINSTANCE hInstance,HWND hWndParent)
{
	m_hInstance = hInstance;
	m_hWndParent = hWndParent;
	m_fWeight = 1;
	m_nSteps = 0;
	m_fAccum = 0.0;
}
void UGMaxProcessBar::Start()
{
	m_hWnd = CreateDialog(m_hInstance,MAKEINTRESOURCE(IDD_PROCESS),m_hWndParent,ProcessProc);
	ShowWindow(m_hWnd,SW_SHOW);
	m_hpw = GetDlgItem(m_hWnd,IDC_PROGRESS1);
	SendMessage(m_hpw,PBM_SETRANGE32, 0,STEPS);
	m_fAccum = 0.0;
 	m_hThread = NULL /*CreateThread(NULL,1000,UpdateBar,(void*)&m_hWnd,0,NULL)*/;
}

void UGMaxProcessBar::SetWeight(UGC::UGfloat fWeight)
{
	m_fWeight = fWeight;
}

void UGMaxProcessBar::SetNumInWeight(UGC::UGint nSteps)
{
		m_nSteps = nSteps;
}

void UGMaxProcessBar::Update()
{
	m_fAccum = m_fAccum + STEPS*m_fWeight/m_nSteps;
	SendMessage(m_hpw,PBM_SETPOS, m_fAccum,0);
	UpdateWindow(m_hWndParent);
	UpdateWindow(m_hWnd);
}

void UGMaxProcessBar::Update(UGC::UGint nIncrement)
{
	m_fAccum = m_fAccum + nIncrement*STEPS*m_fWeight/m_nSteps;
	SendMessage(m_hpw,PBM_SETPOS,m_fAccum,0);
	UpdateWindow(m_hWndParent);
	UpdateWindow(m_hWnd);
}

void UGMaxProcessBar::End()
{
	m_fAccum = 0.0;
	TerminateThread(m_hThread,0);
	DestroyWindow(m_hWnd);
}

DWORD UGMaxProcessBar::UpdateBar(LPVOID lpParam)
{
	
// 	HWND hWnd =*((HWND*)(lpParam));
// 	while (true)
// 	{
// 		UpdateWindow(m_hWnd);
// 		Sleep(200);
// 	}
	return 1;
}

UGMaxProcessBar::~UGMaxProcessBar()
{
	TerminateThread(m_hThread,0);
	DestroyWindow(m_hWnd);
}