// SecureTransDlg.cpp : implementation file
//
#include "stdafx.h"
#include <winsock2.h>
#include <fstream>
#include "SecureTrans.h"
#include "SecureTransDlg.h"
#include "DES.h"

#pragma comment(lib, "ws2_32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSecureTransDlg dialog

CSecureTransDlg::CSecureTransDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSecureTransDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSecureTransDlg)
	m_strIP = _T("");
	m_strKey = _T("");
	m_strFile = _T("");
	m_strLog = _T("");
	m_nMode = -1;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSecureTransDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSecureTransDlg)
	DDX_Text(pDX, IDC_EDIT_IP, m_strIP);
	DDX_Text(pDX, IDC_EDIT_KEY, m_strKey);
	DDX_Text(pDX, IDC_EDIT_FILE, m_strFile);
	DDX_Text(pDX, IDC_EDIT_LOG, m_strLog);
	DDX_Radio(pDX, IDC_RADIO1, m_nMode);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSecureTransDlg, CDialog)
	//{{AFX_MSG_MAP(CSecureTransDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RADIO1, Client)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSecureTransDlg message handlers

BOOL CSecureTransDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	// 初始化 Winsock 库
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		MessageBox("Winsock 初始化失败！", "严重错误", MB_ICONERROR);
	}
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSecureTransDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSecureTransDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSecureTransDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSecureTransDlg::Client() 
{
	// TODO: Add your control notification handler code here
	
}

void CSecureTransDlg::OnButton1() 
{
	// 打开 Windows 标准文件选择对话框
    CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
                    "所有文件 (*.*)|*.*||");
    
    if (dlg.DoModal() == IDOK)
    {
        // 获取选中的文件路径
        m_strFile = dlg.GetPathName();
        // 将变量内容刷新到界面的编辑框中显示
        UpdateData(FALSE); 
    }
}

void CSecureTransDlg::AppendLog(CString msg)
{
	// 获取当前时间
    CTime time = CTime::GetCurrentTime();
    CString strTime = time.Format("[%H:%M:%S] ");
    
    // 追加信息并带上换行符 \r\n
    m_strLog += strTime + msg + "\r\n";
    
    // 刷新界面显示
    UpdateData(FALSE);
    
    // 让滚动条自动滚到底部（防止日志太多看不到最新的）
    CEdit* pLog = (CEdit*)GetDlgItem(IDC_EDIT_LOG);
    if(pLog != NULL)
    {
        pLog->LineScroll(pLog->GetLineCount());
    }
}

// ==========================================
// DES 加密文件
// ==========================================
void Real_DES_EncryptFile(const char* inFile, const char* outFile, const char* keyStr) {
    std::ifstream in(inFile, std::ios::binary);
    std::ofstream out(outFile, std::ios::binary);

    bit bitKey[64];
    char key[8] = {0};
    strncpy(key, keyStr, 8);
    bytetobit(bitKey, key, 64);

    char buffer[8];
    bit bitIn[64], bitOut[64];

    while (in.read(buffer, 8) || in.gcount() > 0) {
        int bytesRead = in.gcount();
        for (int i = bytesRead; i < 8; ++i) {
            buffer[i] = 0;
        }

        bytetobit(bitIn, buffer, 64);      
        DES_encrypt(bitOut, bitIn, bitKey); 
        
        char outBuffer[8];
        bittobyte(outBuffer, bitOut, 64);  
        out.write(outBuffer, 8); 
        memset(buffer, 0, 8); 
    }

    in.close();
    out.close();
}

// ==========================================
// DES 解密文件
// ==========================================
void Real_DES_DecryptFile(const char* inFile, const char* outFile, const char* keyStr) {
    std::ifstream in(inFile, std::ios::binary);
    std::ofstream out(outFile, std::ios::binary);

    bit bitKey[64];
    char key[8] = {0};
    strncpy(key, keyStr, 8);
    bytetobit(bitKey, key, 64);

    char buffer[8];
    bit bitIn[64], bitOut[64];

    // 密文必然是 8 的倍数，每次严格读取 8 字节
    while (in.read(buffer, 8) && in.gcount() == 8) {
        bytetobit(bitIn, buffer, 64);      
        DES_decrypt(bitOut, bitIn, bitKey); 
        
        char outBuffer[8];
        bittobyte(outBuffer, bitOut, 64);  
        out.write(outBuffer, 8);           
    }

    in.close();
    out.close();
}

void CSecureTransDlg::OnButton3() 
{
    // 1. 获取界面上的所有输入（包括单选框的选择状态）
    UpdateData(TRUE);
    
    // 2. 基础安全校验
    if (m_strKey.GetLength() != 8) {
        MessageBox("请输入刚好 8 个字符的 DES 密钥！", "错误提示", MB_ICONERROR);
        return;
    }

    const char* szIP = (LPCTSTR)m_strIP;
    const char* szFile = (LPCTSTR)m_strFile; // 对于接收端，这可以作为保存路径
    const char* szKey = (LPCTSTR)m_strKey;
    int port = 8888; 

    // ==========================================
    // 模式 0：客户端 (发送端)
    // ==========================================
    if (m_nMode == 0) 
    {
        if (m_strFile.IsEmpty() || m_strIP.IsEmpty()) {
            MessageBox("发送模式下，请确保 IP 地址和文件路径不为空！", "错误提示", MB_ICONERROR);
            return;
        }

        AppendLog("=== 发送端(Client)任务启动 ===");
        CString strEncryptedFile = "D:\\temp_send_encrypted.dat";
        
        AppendLog("正在使用 DES 加密文件...");
        Real_DES_EncryptFile(szFile, (LPCTSTR)strEncryptedFile, szKey);
        AppendLog("加密完成，准备发送！");

        SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        serverAddr.sin_addr.s_addr = inet_addr(szIP);

        AppendLog("正在连接接收方...");
        if (connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            AppendLog("错误: 连接失败，请检查对方是否已启动！");
            closesocket(clientSocket);
            return;
        }

        AppendLog("连接成功！开始发送...");
        std::ifstream file((LPCTSTR)strEncryptedFile, std::ios::binary);
        char buffer[4096];
        while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0) {
            send(clientSocket, buffer, file.gcount(), 0);
        }

        file.close();
        closesocket(clientSocket);
        AppendLog("=== 发送任务圆满执行完毕 ===");
    }
    // ==========================================
    // 模式 1：服务端 (接收端)
    // ==========================================
    else if (m_nMode == 1)
    {
        AppendLog("=== 接收端(Server)任务启动 ===");
        
        SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        serverAddr.sin_addr.s_addr = INADDR_ANY;

        bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
        listen(listenSocket, 5);

        // 注意：这里界面会短暂“卡住”等待对方连接，这是正常的！
        AppendLog("正在端口 8888 监听，等待发送方连接...(界面将锁定直至连接成功)");
        UpdateData(FALSE); // 强制刷新一次日志，以免卡住前看不到这行字

        sockaddr_in clientAddr;
        int clientAddrLen = sizeof(clientAddr);
        SOCKET clientSocket = accept(listenSocket, (SOCKADDR*)&clientAddr, &clientAddrLen);
        
        if (clientSocket != INVALID_SOCKET) {
            AppendLog("收到连接请求！开始接收网络密文...");
        }

        CString strEncryptedFile = "D:\\temp_recv_encrypted.dat";
        std::ofstream file((LPCTSTR)strEncryptedFile, std::ios::binary);
        
        char buffer[4096];
        int bytesReceived;
        while ((bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
            file.write(buffer, bytesReceived);
        }
        
        file.close();
        closesocket(clientSocket);
        closesocket(listenSocket);
        AppendLog("密文接收完毕！已保存至 D 盘。");

        // 如果用户没填保存路径，默认放在 D 盘
        CString strDecryptedFile = m_strFile.IsEmpty() ? "D:\\received_decrypted.txt" : m_strFile;
        
        AppendLog("正在使用密钥解密文件...");
        Real_DES_DecryptFile((LPCTSTR)strEncryptedFile, (LPCTSTR)strDecryptedFile, szKey);
        
        AppendLog("解密成功！明文已保存至: " + strDecryptedFile);
        AppendLog("=== 接收任务圆满执行完毕 ===");
    }
}