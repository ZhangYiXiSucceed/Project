
// CommDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Comm.h"
#include "CommDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int BaudRateArray[] = { 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 38400, 56000, 57600, 115200 };

CSerialPort m_SerialPort;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCommDlg �Ի���



CCommDlg::CCommDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCommDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCommDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PORT_Nr, m_PortNr);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_BaudRate);
	DDX_Control(pDX, IDC_BUTTON_OPEN_CLOSE, m_OpenCloseCtrl);
	DDX_Control(pDX, IDC_SendEdit, m_Send);
	DDX_Control(pDX, IDC_ReceiveEdit, m_ReceiveCtrl);
	DDX_Control(pDX, IDC_STATIC_RECV_COUNT_VALUE, m_recvCountCtrl);
	DDX_Control(pDX, IDC_STATIC_SEND_COUNT_VALUE, m_sendCountCtrl);
}

BEGIN_MESSAGE_MAP(CCommDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN_CLOSE, &CCommDlg::OnBnClickedButtonOpenClose)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CCommDlg::OnBnClickedButtonSend)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CCommDlg::OnBnClickedButtonClear)
END_MESSAGE_MAP()


// CCommDlg ��Ϣ�������

BOOL CCommDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	rx = 0;
	tx = 0;
	m_recvCountCtrl.SetWindowText(CString("0"));
	m_sendCountCtrl.SetWindowText(CString("0"));

	CString temp;
	//��Ӳ����ʵ������б�
	for (int i = 0; i < sizeof(BaudRateArray) / sizeof(int); i++)
	{
		temp.Format(_T("%d"), BaudRateArray[i]);
		m_BaudRate.AddString((LPCTSTR)temp);
	}

	temp.Format(_T("%d"), 9600);
	m_BaudRate.SetCurSel(m_BaudRate.FindString(0, temp));

	//��ȡ���ں�
	list<string> m_portsList = CSerialPortInfo::availablePorts();
	list<string>::iterator itor;
	TCHAR m_regKeyValue[255];
	for (itor = m_portsList.begin(); itor != m_portsList.end(); ++itor)
	{
#ifdef UNICODE
		int iLength;
		const char * _char = (*itor).c_str();
		iLength = MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, NULL, 0);
		MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, m_regKeyValue, iLength);
#else
		strcpy_s(m_regKeyValue, 255, (*itor).c_str());
#endif
		m_PortNr.AddString(m_regKeyValue);
	}
	m_PortNr.SetCurSel(0);
	
	OnBnClickedButtonOpenClose();

	m_Send.SetWindowText(_T("http://blog.csdn.net/itas109"));

	m_SerialPort.readReady.connect(this, &CCommDlg::OnReceive);
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CCommDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CCommDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCommDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//void CCommDlg::OnSendMessage(unsigned char* str, int port, int str_len)
//{
//	CString str1((char*)str);
//
//	m_ReceiveCtrl.SetSel(-1, -1);
//	m_ReceiveCtrl.ReplaceSel(str1);
//}

//LRESULT CCommDlg::OnReceiveStr(WPARAM str, LPARAM commInfo)
//{
//	struct serialPortInfo
//	{
//		UINT portNr;//���ں�
//		DWORD bytesRead;//��ȡ���ֽ���
//	}*pCommInfo;
//	pCommInfo = (serialPortInfo*)commInfo;
//
//	CString str1((char*)str);
//	//char* m_str = (char*)str;
//	//TCHAR* dest = NULL;
//	//dest = new TCHAR[sizeof(TCHAR) * pCommInfo->bytesRead];
//	//wmemcpy(dest, (TCHAR*)str, sizeof(TCHAR)* pCommInfo->bytesRead+1);
//	
//	//��ȡָ�����ڵ�����
//	//if (pCommInfo->portNr == 2)
//	//{
//			//m_ReceiveCtrl.SetSel(-1, -1);
//			//m_ReceiveCtrl.ReplaceSel(str1);
//	//}
//	return TRUE;
//}

void CCommDlg::OnBnClickedButtonOpenClose()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//GetDlgItem(IDC_SendEdit)->SetFocus();
	CString temp;
	m_OpenCloseCtrl.GetWindowText(temp);///��ȡ��ť���ı�
	UpdateData(true);
	if (temp == _T("�رմ���"))///��ʾ�������"�رմ���"��Ҳ�����Ѿ��ر��˴���
	{
		m_SerialPort.close();
		m_OpenCloseCtrl.SetWindowText(_T("�򿪴���"));///���ð�ť����Ϊ"�򿪴���"
	}
	///�򿪴��ڲ���
	else if (m_PortNr.GetCount() > 0)///��ǰ�б�����ݸ���
	{

		int SelBaudRate;

		UpdateData(true);
		m_PortNr.GetWindowText(temp);///CString temp
		string portName;
#ifdef UNICODE
		portName = CW2A(temp.GetString());
#else
		portName = temp.GetString();
#endif // UNICODE

		

		m_BaudRate.GetWindowText(temp);
		SelBaudRate = _tstoi(temp);	

		m_SerialPort.init(portName, SelBaudRate);
		m_SerialPort.open();

		if (m_SerialPort.isOpened())
		{
			m_OpenCloseCtrl.SetWindowText(_T("�رմ���"));
		}
		else
		{
			AfxMessageBox(_T("�����ѱ�ռ�ã�"));
		}
	}
	else
	{
		AfxMessageBox(_T("û�з��ִ��ڣ�"));
	}
}


void CCommDlg::OnBnClickedButtonSend()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_SendEdit)->SetFocus();
	CString temp;
	UpdateData(true);
	m_OpenCloseCtrl.GetWindowText(temp);
	if (temp == "�򿪴���")///û�д򿪴���
	{
		AfxMessageBox(_T("�����ȴ򿪴���"));
		return;
	}

	m_Send.GetWindowText(temp);
	size_t len = _tcsclen(temp) + 1;;
	char* m_str = NULL;
	size_t* converted = 0;
	m_str = new char[len];
#ifdef UNICODE
	wcstombs_s(converted, m_str, len, temp.GetBuffer(0), _TRUNCATE);
#else
	m_str = temp.GetBuffer(0);
#endif
	
	m_SerialPort.writeData(m_str, len);

	tx += len - 1;

	CString str2;
	str2.Format(_T("%d"), tx);
	m_sendCountCtrl.SetWindowText(str2);
}


void CCommDlg::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_SerialPort.close();
	CDialogEx::OnClose();
}

void CCommDlg::OnReceive()
{
	char * str = NULL;
	str = new char[256];
	m_SerialPort.readAllData(str);

	CString str1((char*)str);

	rx += str1.GetLength();

	//m_ReceiveCtrl.SetSel(-1, -1);
	m_ReceiveCtrl.ReplaceSel(str1);
	m_ReceiveCtrl.LineScroll(m_ReceiveCtrl.GetLineCount());
	CString str2;
	str2.Format(_T("%d"), rx);
	m_recvCountCtrl.SetWindowText(str2);
}


void CCommDlg::OnBnClickedButtonClear()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	rx = 0;
	tx = 0;
	m_recvCountCtrl.SetWindowText(CString("0"));
	m_sendCountCtrl.SetWindowText(CString("0"));
}
