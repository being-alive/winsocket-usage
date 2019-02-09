
// ClientMFCDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ClientMFC.h"
#include "ClientMFCDlg.h"
#include "afxdialogex.h"

#include "windows.h"
#include "winsock2.h"
#include "ws2tcpip.h"
#define DEFAULT_BUFLEN 1024
#define WM_SOCKET WM_USER+11

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CClientMFCDlg �Ի���



CClientMFCDlg::CClientMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientMFCDlg::IDD, pParent)
	, m_ip(_T(""))
	, m_port(_T(""))
	, m_SendMessage(_T(""))
	, m_MessageEdit(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_ip);
	DDX_Text(pDX, IDC_EDIT2, m_port);
	DDX_Text(pDX, IDC_EDIT4, m_SendMessage);
	//DDX_Control(pDX, IDC_LIST1, m_MessageList);
	DDX_Text(pDX, IDC_EDIT5, m_MessageEdit);
}

BEGIN_MESSAGE_MAP(CClientMFCDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNBUTTON, &CClientMFCDlg::OnClickedConnbutton)
	ON_BN_CLICKED(IDC_SNEDBUTTON, &CClientMFCDlg::OnClickedSnedbutton)
	ON_MESSAGE(WM_SOCKET,OnSocket)
	ON_BN_CLICKED(IDC_DISCONNBUTTON, &CClientMFCDlg::OnBnClickedDisconnbutton)
END_MESSAGE_MAP()


// CClientMFCDlg ��Ϣ�������

BOOL CClientMFCDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CClientMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CClientMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CClientMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CClientMFCDlg::conn()
{
	ConnectSocket = INVALID_SOCKET;
	struct addrinfo *ptr,*result = NULL;
	struct addrinfo hints;
	int iResult = 0;
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) 
		MessageBoxA(NULL,"WSAStartup failed with error","error",MB_OK);
	/*hostName = new char[256];
	gethostname(hostName,sizeof(hostName));*/
	ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(serverName, serverPort, &hints, &result);
    if ( 0 != iResult) {
		MessageBoxA(NULL,"getaddrinfo failed with error","error",MB_OK);
        WSACleanup();
	}
	 // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {

            WSACleanup();
        }

        // Connect to server.
       iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }
    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
		MessageBoxA(NULL,"socket failed with error","error",MB_OK);
        WSACleanup();
    }
}


void CClientMFCDlg::close()
{
	int iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
		MessageBoxA(NULL,"shutdown failed with error","error",MB_OK);
        closesocket(ConnectSocket);
        WSACleanup();
    }
    closesocket(ConnectSocket);
    WSACleanup();
}




void CClientMFCDlg::OnClickedConnbutton()
{
	UpdateData(TRUE);
	//convert to char* from CString
	USES_CONVERSION;
	serverName = T2A(m_ip.GetBuffer(0));
	m_ip.ReleaseBuffer();
	serverPort = T2A(m_port.GetBuffer(0));
	m_port.ReleaseBuffer();
	conn();
	WSAAsyncSelect(ConnectSocket,m_hWnd,WM_SOCKET, FD_READ);
}


void CClientMFCDlg::OnClickedSnedbutton()
{
	CString sendInfo;
	UpdateData(TRUE);
	sendInfo.Format(_T("%s"),CStringW(m_SendMessage.GetBuffer(0)));
	int len = CStringA(sendInfo).GetLength();
	USES_CONVERSION;
	char *buf = T2A(sendInfo.GetBuffer(0));
	int iSendResult = send(ConnectSocket, buf, len, 0);
    if (iSendResult == SOCKET_ERROR) {
			MessageBoxA(NULL,"send failed with error","error",MB_OK);
            closesocket(ConnectSocket);
            WSACleanup();
     }
	
}

LRESULT CClientMFCDlg::OnSocket(WPARAM	w, LPARAM l)
{
	SOCKET s = (SOCKET)w;
	switch(l)
	{
		case FD_READ:
			char* buffer = new char[DEFAULT_BUFLEN];
			int res = recv(s,buffer,DEFAULT_BUFLEN,0);
			if(res > 0)
			{
				CString receivedStr;
				buffer[res] = '\0';
				receivedStr.Format(_T("%s"),CStringW(buffer));
				m_MessageEdit.Append(receivedStr);
				//m_MessageList.AddString(receivedStr);
				//transmit the value of member to the Control
				UpdateData(FALSE);
			}
			break;
	}
	return 1;
}


void CClientMFCDlg::OnBnClickedDisconnbutton()
{
	close();
}
