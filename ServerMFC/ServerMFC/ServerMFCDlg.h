
// ServerMFCDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#define CLIEN_MAXNUM 12

// CServerMFCDlg 对话框
class CServerMFCDlg : public CDialog
{
// 构造
public:
	CServerMFCDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SERVERMFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnSocket(WPARAM	w, LPARAM l);
	DECLARE_MESSAGE_MAP()
public:
	//CListBox m_MessageList;
	CString m_ip;
	CString m_port;
private:
	char* serverName;
	char* serverPort;
	SOCKET ClientSocketList[CLIEN_MAXNUM];
	char* ClientIPList[CLIEN_MAXNUM];
	u_short ClientPortList[CLIEN_MAXNUM];
	int cursor;
	SOCKET ListenSocket;
	WSADATA wsaData;
    SOCKET ClientSocket;
	void open();
public:
	afx_msg void OnClickedListenbutton();

	CListBox m_OnlineList;
	CString m_MessageEdit;
};
