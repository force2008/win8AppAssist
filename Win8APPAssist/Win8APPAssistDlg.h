
// Win8APPAssistDlg.h : header file
//

#pragma once
#define MAX_PATH 200

// CWin8APPAssistDlg dialog
class CWin8APPAssistDlg : public CDialogEx
{
// Construction
public:
	CWin8APPAssistDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_WIN8APPASSIST_DIALOG };
	CEdit *ppath;
	CEdit *pname;
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	BOOL createProject(CString path,CString name);
	void copyFolderAllFiles(CString csSourceFolder, CString csNewFolder);
	TCHAR *path;
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFolderBtnClickedButton1();
	afx_msg void OnBnClickedOk();
};
