// SecureTransDlg.h : header file
//

#if !defined(AFX_SECURETRANSDLG_H__DBBF2691_3049_447C_BD12_BD9B35DF6C21__INCLUDED_)
#define AFX_SECURETRANSDLG_H__DBBF2691_3049_447C_BD12_BD9B35DF6C21__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSecureTransDlg dialog

class CSecureTransDlg : public CDialog
{
// Construction
public:
	void AppendLog(CString msg);
	CSecureTransDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSecureTransDlg)
	enum { IDD = IDD_SECURETRANS_DIALOG };
	CString	m_strIP;
	CString	m_strKey;
	CString	m_strFile;
	CString	m_strLog;
	int		m_nMode;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSecureTransDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSecureTransDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void Client();
	afx_msg void OnButton1();
	afx_msg void OnButton3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SECURETRANSDLG_H__DBBF2691_3049_447C_BD12_BD9B35DF6C21__INCLUDED_)
