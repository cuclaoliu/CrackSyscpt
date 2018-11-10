
// CrackSyscptDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CCrackSyscptDlg 对话框
class CCrackSyscptDlg : public CDialogEx
{
// 构造
public:
	CCrackSyscptDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CRACKSYSCPT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CButton mButtonStart;
	CString mFilePath;
	CEdit mEditFilePath;
	CButton mButtonBrowse;
	afx_msg void OnBnClickedOk();
	unsigned long FindBytes(unsigned char*, size_t);
	afx_msg void OnBnClickedButtonBrowse();
	CString mMessage;
};
