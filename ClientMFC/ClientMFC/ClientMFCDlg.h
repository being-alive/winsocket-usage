
// ClientMFCDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CClientMFCDlg 对话框
class CClientMFCDlg : public CDialog
{
// 构造
public:
	CClientMFCDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CLIENTMFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
