
// CrackSyscptDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CCrackSyscptDlg �Ի���
class CCrackSyscptDlg : public CDialogEx
{
// ����
public:
	CCrackSyscptDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CRACKSYSCPT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
