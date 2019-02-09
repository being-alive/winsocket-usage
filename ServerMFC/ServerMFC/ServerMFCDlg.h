
// ServerMFCDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#define CLIEN_MAXNUM 12

// CServerMFCDlg �Ի���
class CServerMFCDlg : public CDialog
{
// ����
public:
	CServerMFCDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SERVERMFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
