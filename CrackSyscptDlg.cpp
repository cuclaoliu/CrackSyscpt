// CrackSyscptDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "CrackSyscpt.h"
#include "CrackSyscptDlg.h"
#include "afxdialogex.h"
#include <string>
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#include <iostream>
#endif

const unsigned char TOBEALTER[] = {0x44, 0x89, 0x44, 0x24, 0x18, 0x48, 0x89, 0x54,
					0x24, 0x10, 0x48, 0x89, 0x4C, 0x24, 0x08, 0x48, 0x81, 0xEC, 0x78, 0x01 };
const unsigned char ALTER[] = {0x33, 0xc0, 0xc3 };
// CCrackSyscptDlg 对话框

CCrackSyscptDlg::CCrackSyscptDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CRACKSYSCPT_DIALOG, pParent)
	, mFilePath(_T(""))
	, mMessage(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCrackSyscptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, mButtonStart);
	DDX_Text(pDX, IDC_EDIT_FILEPATH, mFilePath);
	DDX_Control(pDX, IDC_EDIT_FILEPATH, mEditFilePath);
	DDX_Control(pDX, IDC_BUTTON_BROWSE, mButtonBrowse);
	DDX_Text(pDX, IDC_EDIT_INFO, mMessage);
}

BEGIN_MESSAGE_MAP(CCrackSyscptDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CCrackSyscptDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CCrackSyscptDlg::OnBnClickedButtonBrowse)
END_MESSAGE_MAP()


// CCrackSyscptDlg 消息处理程序

BOOL CCrackSyscptDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	using namespace std;

	char *quartus_rootdir;
	size_t len;
	errno_t err = _dupenv_s(&quartus_rootdir, &len, "QUARTUS_ROOTDIR");
	if (err) 
		mMessage += _T("Read Enviroment Variable Error!\r\n");
	else
		mMessage += _T("Found your Quartus:\r\n\t");

	CString quartusDir(quartus_rootdir);
	mMessage += quartusDir + _T("\r\n");
	mFilePath = quartusDir + _T("\\bin64\\sys_cpt.dll");
	char filePath[1024];
	strcpy_s(filePath, 1024, CStringA(mFilePath));
	if (_access(filePath, 0) == -1)
		mMessage += _T("Can't access THE file:\r\n");
	else
		mMessage += _T("Found the target file:\r\n");
	mMessage += _T("\t") + mFilePath + _T("\r\n");
	UpdateData(false);
	mEditFilePath.ShowWindow(true);
	mEditFilePath.EnableWindow(false);
	mButtonBrowse.ShowWindow(true);
	mButtonStart.EnableWindow(true);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCrackSyscptDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCrackSyscptDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCrackSyscptDlg::OnBnClickedOk()
{
	FILE* inFileStream, *outFileStream;
	char filePath[1024], filePathBak[1024];
	strcpy_s(filePath, 1024, CStringA(mFilePath));
	strcpy_s(filePathBak, 1024, CStringA(mFilePath + _T(".bak")));
	error_status_t err = fopen_s(&inFileStream, filePath, "rb");
	if (err != 0) {
		mMessage += _T("Can not open the target file.\r\n");
		return;
	}else
		mMessage += _T("Target: ") + CString(filePath) + _T("\r\n");
	UpdateData(false);

	if (fseek(inFileStream, 0, SEEK_END) != 0) {
		fclose(inFileStream);
		return;
	}
	size_t size = ftell(inFileStream);
	unsigned char *data = new unsigned char[size];
	if (fseek(inFileStream, 0, SEEK_SET) != 0) {
		fclose(inFileStream);
		return;
	}
	size_t rtSize = fread(data, sizeof(unsigned char), size, inFileStream);
	fclose(inFileStream);
	if (rtSize != size)
		return;
	unsigned long pos = FindBytes(data, size);
	if (pos < 0) {
		mMessage += _T("Failed to find the bytes.\r\n");
		UpdateData(false);
		return;
	}
	mMessage += _T("Find the bytes.\r\n");
	for (int i = 0; i < sizeof(ALTER); i++) {
		data[pos + i] = ALTER[i];
	}
	UpdateData(false);
	if (rename(filePath, filePathBak) != 0)
		return;
	mMessage += _T("Backup to:\r\n\t");
	mMessage += CString(filePathBak) +_T("\r\n");
	UpdateData(false);
	err = fopen_s(&outFileStream, filePath, "wb");
	if (err != 0)
		return;
	rtSize = fwrite(data, sizeof(unsigned char), size, outFileStream);
	fclose(outFileStream);
	if (rtSize != size)
		return;
	mMessage += _T("Wrote to:\r\n\t");
	mMessage += CString(filePath) +_T("\r\n");
	UpdateData(false);
	//CDialogEx::OnOK();
}


unsigned long CCrackSyscptDlg::FindBytes(unsigned char* data, size_t size)
{
	unsigned long pos = -1;
	int bytenum = sizeof(TOBEALTER);
	for (unsigned long i = 0; i < size; i++) {
		bool found = true;
		for (int j = 0; j < bytenum; j++) {
			if (data[i + j] != TOBEALTER[j]) {
				found = false;
				break;
			}
		}
		if (found) {
			pos = i;
			break;
		}
	}
	return pos;
}


void CCrackSyscptDlg::OnBnClickedButtonBrowse()
{
	CString filter;
	filter = "sys_cpt.dll|sys_cpt.dll|All files(*.*)|*.*||";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter);
	if (dlg.DoModal() == IDOK)
	{
		mFilePath = dlg.GetPathName();
		mMessage += _T("You find a new file path:\r\n\t") + mFilePath + _T("\r\n");
		UpdateData(false);
	}
}
