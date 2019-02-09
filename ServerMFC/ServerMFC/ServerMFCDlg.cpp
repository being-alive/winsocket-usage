
// ServerMFCDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ServerMFC.h"
#include "ServerMFCDlg.h"
#include "afxdialogex.h"

#include "WinSock2.h"
#include "windows.h"
#include "ws2tcpip.h"
#define CLIEN_MAXNUM 12
#define DEFAULT_BUFLEN 1024

#define WM_SOCKET WM_USER+11

#pragma comment(lib,"Ws2_32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CServerMFCDlg 对话框



CServerMFCDlg::CServerMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerMFCDlg::IDD, pParent)
	, m_ip(_T(""))
	, m_port(_T(""))
	, m_MessageEdit(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_MESSAGELIST, m_MessageList);
	DDX_Text(pDX, IDC_IPEDIT, m_ip);
	DDX_Text(pDX, IDC_PORTEDIT, m_port);

	DDX_Control(pDX, IDC_LIST1, m_OnlineList);
	DDX_Text(pDX, IDC_EDIT1, m_MessageEdit);
}

BEGIN_MESSAGE_MAP(CServerMFCDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_LISTENBUTTON, &CServerMFCDlg::OnClickedListenbutton)
	ON_MESSAGE(WM_SOCKET,OnSocket)
END_MESSAGE_MAP()


// CServerMFCDlg 消息处理程序

BOOL CServerMFCDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	cursor =  0;
	for (int i=0; i<CLIEN_MAXNUM;i++)
		ClientSocketList[i] = NULL;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CServerMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CServerMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CServerMFCDlg::open()
{
	struct addrinfo *result;
    struct addrinfo hints;
	ListenSocket = INVALID_SOCKET;
    ClientSocket = INVALID_SOCKET;
	int iResult = 0;
	// Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (0 != iResult) 
		MessageBoxA(NULL,"WSAStartup failed with error","error",MB_OK);
	ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and por
    iResult = getaddrinfo(serverName, serverPort, &hints, &result);
    if ( 0 != iResult) {
		MessageBoxA(NULL,"getaddrinfo failed with error","error",MB_OK);
        WSACleanup();
	}

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
		MessageBoxA(NULL,"socket failed with error","error",MB_OK);
        freeaddrinfo(result);
        WSACleanup();
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
		MessageBoxA(NULL,"bind failed with error","error",MB_OK);
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, CLIEN_MAXNUM);
    if (iResult == SOCKET_ERROR) {
		MessageBoxA(NULL,"listen failed with error","error",MB_OK);
        closesocket(ListenSocket);
        WSACleanup();
    }
}

void CServerMFCDlg::OnClickedListenbutton()
{
	//first we initialize the port and the ip address
	//transmit the value of the Control to member
	UpdateData(TRUE);
	//convert to char* from CString
	USES_CONVERSION;
	serverName = T2A(m_ip.GetBuffer(0));
	m_ip.ReleaseBuffer();
	serverPort = T2A(m_port.GetBuffer(0));
	m_port.ReleaseBuffer();

	//start listen the port
	open();
	//output the successful information
	CString openSuccessInfo;
	openSuccessInfo.Format(_T("主机%s监听端口%s......\r\n"),m_ip.GetBuffer(0),m_port.GetBuffer(0));
	m_MessageEdit.Append(openSuccessInfo);

	//m_MessageList.AddString(openSuccessInfo);

	//transmit the value of member to the Control
	UpdateData(FALSE);
	WSAAsyncSelect(ListenSocket,m_hWnd,WM_SOCKET, FD_READ|FD_WRITE|FD_ACCEPT);
}
LRESULT CServerMFCDlg::OnSocket(WPARAM	w, LPARAM l)
{
	switch(l)
	{
		case FD_ACCEPT:
		{
			sockaddr_in m_client;
			int sz;
			sz = sizeof(sockaddr_in);
			ClientSocket = accept(ListenSocket, (sockaddr*)&m_client, &sz);
			if(ClientSocket == INVALID_SOCKET)
			{
			   closesocket(ListenSocket);
			   WSACleanup();
			}
			ClientSocketList[cursor] = ClientSocket;
			char* clientIP = inet_ntoa(m_client.sin_addr);
			u_short clientPort = ntohs(m_client.sin_port);
			ClientIPList[cursor] = clientIP;
			ClientPortList[cursor] = clientPort;
			cursor++;
			CString receivedInfo;
			receivedInfo.Format(_T("%s/%d 进入聊天室......\r\n"), CStringW(clientIP),clientPort);
			m_MessageEdit.Append(receivedInfo);
			//m_MessageList.AddString(receivedInfo);
			//add to online client list
			CString clientInfo;
			clientInfo.Format(_T("%s:%d"), CStringW(clientIP),clientPort);
			m_OnlineList.AddString(clientInfo);
			UpdateData(FALSE);
			USES_CONVERSION;
			char *buf = T2A(receivedInfo.GetBuffer(0));
			int len = CStringA(receivedInfo).GetLength();
			for(int i=0; i<cursor; i++)
				send(ClientSocketList[i],buf,len,0);
		}
			break;
		case FD_READ:
		{
			char* buffer = new char[DEFAULT_BUFLEN];
			int res = -1;
			int curClient;
			for (int i=0; i<CLIEN_MAXNUM;i++)
			{
				res = recv(ClientSocketList[i], buffer, DEFAULT_BUFLEN,0);
				if(res > 0)
				{
					curClient = i;
					break;
				}
			}
			buffer[res] = '\0';
			CString receivedStr;
			if(res > 0)
			{
				receivedStr.Format(_T("%s/%d:%s\r\n"), CStringW(ClientIPList[curClient]),ClientPortList[curClient],CStringW(buffer));
				m_MessageEdit.Append(receivedStr);
				//m_MessageList.AddString(receivedStr);
				//transmit the value of member to the Control
				UpdateData(FALSE);
			}
			USES_CONVERSION;
			char *buf = T2A(receivedStr.GetBuffer(0));
			int len = CStringA(receivedStr).GetLength();
			for(int i=0; i<cursor; i++)
				send(ClientSocketList[i],buf,len,0);
		}
			break;
	}
	return 1;
}

