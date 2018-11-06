/*@ SendWhatsApp - Send a Whats App message from a C++ desktop applicaiton                       */
/*                                                                          */
/*--------------------------------------------------------------------------*/
/* Written and Designed by Michael Haephrati                                */
/* COPYRIGHT ©2008 by Michael Haephrati    haephrati@gmail.com              */
/* http://michaelhaephrati.com												*/
/* All rights reserved.                                                     */
/* -------------------------------------------------------------------------*/

#include "stdafx.h"
#include "SG_SendWhatsApp.h"
#include "resource.h"
#include "SG_SendWhatsAppDlg.h"
#include "afxdialogex.h"
#include "SGWhatsApp.h"

//#include "PDU.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//v 2018057505

#include <shlwapi.h> 
#include <ole2.h>
#pragma comment (lib, "shlwapi.lib")

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSG_SendWhatsAppDlg dialog



void CSG_SendWhatsAppDlg::UpdateMessageField()
{
	CWnd* fieldMessage = GetDlgItem(IDC_EDIT_MESSAGE);
	UpdateData(TRUE);
	if (m_Attachment != L"")
	{
		fieldMessage->EnableWindow(FALSE);
	}
	else
	{
		fieldMessage->EnableWindow(TRUE);
	}
}



CSG_SendWhatsAppDlg::CSG_SendWhatsAppDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSG_SendWhatsAppDlg::IDD, pParent)
	, m_GroupAdmin(_T(""))
	, m_GroupName(_T(""))
	, m_ClientID(_T(""))
	, m_Message(_T(""))
	, m_Status(_T(""))
	, m_Secret(_T(""))
	, m_Attachment(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSG_SendWhatsAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_GROUPADMIN, m_GroupAdmin);
	DDX_Text(pDX, IDC_EDIT_GROUPNAME, m_GroupName);
	DDX_Text(pDX, IDC_EDIT_CLIENTID, m_ClientID);
	DDX_Text(pDX, IDC_EDIT_MESSAGE, m_Message);
	DDX_Text(pDX, IDC_STATUS, m_Status);
	DDX_Text(pDX, IDC_EDIT_SECRET, m_Secret);
	DDX_Text(pDX, IDC_EDIT_ATTACHMENT, m_Attachment);
}

BEGIN_MESSAGE_MAP(CSG_SendWhatsAppDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CSG_SendWhatsAppDlg::OnBnClickedOk)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK1, &CSG_SendWhatsAppDlg::OnNMClickSyslink1)
	ON_BN_CLICKED(IDC_BUTTON1, &CSG_SendWhatsAppDlg::OnBnClickedButton1)
	ON_WM_CREATE()
	ON_STN_CLICKED(IDC_STATUS, &CSG_SendWhatsAppDlg::OnStnClickedStatus)
	ON_EN_CHANGE(IDC_EDIT_ATTACHMENT, &CSG_SendWhatsAppDlg::OnEnChangeEditAttachment)
END_MESSAGE_MAP()


// CSG_SendWhatsAppDlg message handlers

BOOL CSG_SendWhatsAppDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon



	CWinApp* pApp = AfxGetApp();
	m_GroupAdmin = pApp->GetProfileString(APP_SECTION, APP_GROUPADMIN);
	m_GroupName = pApp->GetProfileString(APP_SECTION, APP_GROUPNAME);
	m_ClientID = pApp->GetProfileString(APP_SECTION, APP_CLIENTID);
	m_Secret = pApp->GetProfileString(APP_SECTION, APP_SECRET);

	UpdateData(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSG_SendWhatsAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSG_SendWhatsAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		 CPaintDC dc(this); // device context for painting
        dc.SetBkColor(RGB(34, 255, 0));
        dc.SetTextColor(RGB(215, 0, 0));
		
        //dc.TextOut (130, 90, L"Hello");

		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSG_SendWhatsAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSG_SendWhatsAppDlg::OnBnClickedOk()
{
    CString filename;
    LPBYTE buffer = NULL;
    int lenght = 0;

    CString Error = L"";
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
      	
	if (m_ClientID == L"") Error += L"\nMissing Client ID";
	if (m_Secret == L"") Error += L"\nMissing API Secret";
	if (m_GroupAdmin == L"") Error += L"\nMissing Group Admin";
	if (m_GroupName == L"") Error += L"\nMissing Group Name";
	//if (m_Message == L"") Error += L"\nMissing message to send";

    filename = PathFindFileName(m_Attachment);
    HANDLE hFile = CreateFile(m_Attachment, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        Error += L"\nCannot open file";
    }
    else
    {
        DWORD fileSize = GetFileSize(hFile, NULL);
        DWORD read = -1;
        buffer = new BYTE[fileSize];
        lenght = (int)fileSize;
        if (!ReadFile(hFile, buffer, fileSize, &read, NULL))
        {
            Error += L"\nRead file error";
        }
        CloseHandle(hFile);
    }

	if (Error != L"")
	{
		Error = L"Missing data to proceed:" + Error;
		MessageBox(Error, APP_SECTION, MB_OK);
        if (buffer)
            delete buffer;
		return;
	}

    //m_Attachment

    SGWhatsApp sgw;
    bool result = false;
    if (!StrCmp(PathFindExtension(m_Attachment), _T("jpg")) || StrCmp(PathFindExtension(m_Attachment), _T("png")))
        result = sgw.SendGroupImage(m_ClientID, m_Secret, m_GroupAdmin, m_GroupName, m_Message, buffer, lenght);
    else
        result = sgw.SendGroupDocument(m_ClientID, m_Secret, m_GroupAdmin, m_GroupName, filename, buffer, lenght);
	
	if (result)
	{
		m_Status = L"Success";
		CWinApp* pApp = AfxGetApp();
		pApp->WriteProfileString(APP_SECTION, APP_GROUPNAME, m_GroupName);
		pApp->WriteProfileString(APP_SECTION, APP_GROUPADMIN, m_GroupAdmin);
		pApp->WriteProfileString(APP_SECTION, APP_SECRET, m_Secret);
		pApp->WriteProfileString(APP_SECTION, APP_CLIENTID, m_ClientID);
	}
	else
		m_Status = L"Failed";

	UpdateData(FALSE);
	//CDialogEx::OnOK();
}

void CSG_SendWhatsAppDlg::OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	ShellExecute(NULL, L"OPEN", L"mailto:vworker@gmail.com", NULL, NULL, SW_SHOWNORMAL);
	*pResult = 0;
}




void CSG_SendWhatsAppDlg::OnBnClickedButton1()
{
	OPENFILENAME ofn;       // common dialog box structure
	wchar_t szFile[1024];       // buffer for file name

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = L"";
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';

	ofn.nMaxFile = sizeof(szFile)*sizeof(wchar_t);
	ofn.lpstrFilter = L"All\0*.*\0pdf\0*.pdf\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	ofn.hwndOwner = GetSafeHwnd();

	int getReturnValue = GetOpenFileName(&ofn);
	if(getReturnValue)
	{
		m_Attachment = ofn.lpstrFile;
		UpdateData(FALSE);
		//UpdateMessageField();
	}
}


int CSG_SendWhatsAppDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}


void CSG_SendWhatsAppDlg::OnStnClickedStatus()
{
	// TODO: Add your control notification handler code here
}


void CSG_SendWhatsAppDlg::OnEnChangeEditAttachment()
{
	UpdateMessageField();
}
