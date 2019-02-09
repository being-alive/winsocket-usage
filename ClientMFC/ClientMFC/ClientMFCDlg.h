
// ClientMFCDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CClientMFCDlg �Ի���
class CClientMFCDlg : public CDialog
{
// ����
public:
	CClientMFCDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CLIENTMFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnSocket(WPARAM	w, LPARAM l);
	DECLARE_MESSAGE_MAP()

public:
	CString m_ip;
	CString m_port;
	CString m_SendMessage;
	//CListBox m_MessageList;

private:
	char* serverName;
	char* serverPort;
	WSADATA wsaData;
    SOCKET ConnectSocket;
	void conn();
	void close();
public:

	afx_msg void OnClickedConnbutton();
	afx_msg void OnClickedSnedbutton();
	afx_msg void OnBnClickedDisconnbutton();
	CString m_MessageEdit;
};
