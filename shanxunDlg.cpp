
// shanxunDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "shanxun.h"
#include "shanxunDlg.h"
#include "afxdialogex.h"
#include <stdio.h>
#include "md5.h"
#include<ras.h>
#include <winhttp.h>

#include<winsock2.h>
//���� ws2_32.lib
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"winhttp.lib")
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"RASAPI32.Lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
    byte username[35]="@GDPF.XY";
	char *mm="";
    byte res[35];
CString user="",password="";//�˺���Ϣ

static void getPIN(byte *userName, byte *PIN)
{
    int i,j;//ѭ������
    long timedivbyfive;//ʱ�������
    time_t timenow;//��ǰʱ�䣬��time()���
    byte RADIUS[16]="singlenet01";//��λ�ַ�
    byte timeByte[4];//ʱ�� div 5
    byte beforeMD5[32];//ʱ�� div 5+�û���+��λ
    CMD5 md5;//MD5�ṹ��
    byte afterMD5[16];//MD5���
    char MD501H[2]; //MD5ǰ��λ
    byte MD501[3];
    byte timeHash[4]; //ʱ��div5������һ��ת����ֵ
    byte temp[32]; //��һ��ת��ʱ���õ���ʱ����
    byte PIN27[6]; //PIN��2��7λ����ϵͳʱ��ת��

    //code
    //info("sxplugin : using zjxinlisx01");
    //strcpy(RADIUS, "zjxinlisx01");
    timenow = time(NULL);
    timedivbyfive = timenow / 5;

    for(i = 0; i < 4; i++) {
        timeByte[i] = (byte)(timedivbyfive >> (8 * (3 - i)) & 0xFF);
    }
    for(i = 0; i < 4; i++) {
        beforeMD5[i]= timeByte[i];
    }
    for(i = 4; i < 16 && userName[i-4]!='@' ; i++) {
        beforeMD5[i] = userName[i-4];
    }
    j=0;
    while(RADIUS[j]!='\0')
        beforeMD5[i++] = RADIUS[j++];

    md5.GenerateMD5(beforeMD5,i);

   // MD5_Update (&md5, beforeMD5, i);
   // printf("%d,%s\n",i,beforeMD5);
    //MD5_Final (afterMD5, &md5);
    string t=md5.ToString();
    for(int i=0;i<16;i++)
        afterMD5[i]=t[i];
    //afterMD5=t;
    //cout<<t<<","<<t.size()<<endl;;
//������һ�飬�����JAVAʱ����ʧ��
  /*  MD501H[0] = afterMD5[0] >> 4 & 0xF;
    MD501H[1] = afterMD5[0] & 0xF;

    sprintf((char*)MD501,"%x%x",(char)MD501H[0],(char)MD501H[1]);//������
*/
    MD501[0]=t[0];
    MD501[1]=t[1];
//�ҷ����java����
/* M
        D501H[0] = (char) (afterMD5[0] >> 4 & 0xF);
        MD501H[1] = (char) (afterMD5[0] & 0xF);
        byte MD1=(byte)(MD501H[0]);byte MD2=(byte)(MD501H[1]);

       String MD=String.format("%x%x",MD1,MD2);
       MD501=MD.toCharArray();*/


    for(i = 0; i < 32; i++) {
        temp[i] = timeByte[(31 - i) / 8] & 1;
        timeByte[(31 - i) / 8] = timeByte[(31 - i) / 8] >> 1;
    }

    for (i = 0; i < 4; i++) {
        timeHash[i] = temp[i] * 128 + temp[4 + i] * 64 + temp[8 + i]
            * 32 + temp[12 + i] * 16 + temp[16 + i] * 8 + temp[20 + i]
            * 4 + temp[24 + i] * 2 + temp[28 + i];
    }

    temp[1] = (timeHash[0] & 3) << 4;
    temp[0] = (timeHash[0] >> 2) & 0x3F;
    temp[2] = (timeHash[1] & 0xF) << 2;
    temp[1] = (timeHash[1] >> 4 & 0xF) + temp[1];
    temp[3] = timeHash[2] & 0x3F;
    temp[2] = ((timeHash[2] >> 6) & 0x3) + temp[2];
    temp[5] = (timeHash[3] & 3) << 4;
    temp[4] = (timeHash[3] >> 2) & 0x3F;

    for (i = 0; i < 6; i++) {
        PIN27[i] = temp[i] + 0x020;
        if(PIN27[i]>=0x40) {
            PIN27[i]++;
        }
    }

    PIN[0] = '\r';
    PIN[1] = '\n';

    memcpy(PIN+2, PIN27, 6);

    PIN[8] = MD501[0];
    PIN[9] = MD501[1];

    strcpy((char*)PIN+10, (char*)userName);
}
HRASCONN handle=NULL;
void ras(){
    RASDIALPARAMS  ms;
    RASDIALPARAMS params;
	handle=NULL;
    memset(&ms, '\0', sizeof(params));
    params.dwSize=sizeof(RASDIALPARAMS);
    strcpy(params.szEntryName,"shanxun");
    strcpy(params.szPhoneNumber,"");
    strcpy(params.szCallbackNumber,"");


	strcpy(params.szPassword, password.GetString());  //����
    strcpy(params.szDomain,  "");
	getPIN((byte *)user.GetString(),res);
    strcpy(params.szUserName,(char*)res);
    int a =RasDial(NULL, NULL, &params, NULL, NULL, &handle);
    if (a!=ERROR_SUCCESS)
    {
		
        printf("���ڲ���ļ����û��Ӧ���Ժ�������");
        DWORD off=RasHangUp(handle);
         if (off==0)
        {
         printf("�����ѶϿ�...\n");
        }
         else{
          printf("�Ͽ����ӳ���...\n");
        }
    }
    else
    {

       // cout<<"���ӳɹ�\n"<<endl;
    }

}
bool httpsend(){
  DWORD dwSize = 0;
  DWORD dwDownloaded = 0;
  LPSTR pszOutBuffer;
  BOOL  bResults = FALSE;
  HINTERNET  hSession = NULL,
             hConnect = NULL,
             hRequest = NULL;

  // Use WinHttpOpen to obtain a session handle.
  hSession = WinHttpOpen( L"WinHTTP Example/1.0",
                          WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                          WINHTTP_NO_PROXY_NAME,
                          WINHTTP_NO_PROXY_BYPASS, 0 );

  // Specify an HTTP server.
  if( hSession )
    hConnect = WinHttpConnect( hSession, L"127.0.0.1",
                               INTERNET_DEFAULT_HTTP_PORT, 0 );

  // Create an HTTP request handle.
  if( hConnect )
    hRequest = WinHttpOpenRequest( hConnect, L"GET",
									NULL,//pwszObjectName
									NULL, WINHTTP_NO_REFERER,
                                   WINHTTP_DEFAULT_ACCEPT_TYPES,
                                   WINHTTP_FLAG_SECURE );
    else
        return false;
  if (hRequest&&0)
	  bResults = WinHttpAddRequestHeaders(hRequest,
	  L"If-Modified-Since: Mon, 20 Nov 2000 20:00:00 GMT",
	  (ULONG)-1L,
	  WINHTTP_ADDREQ_FLAG_ADD);

  // Send a request.
  if( hRequest )
    bResults = WinHttpSendRequest( hRequest,
                                   WINHTTP_NO_ADDITIONAL_HEADERS, 0,
                                   WINHTTP_NO_REQUEST_DATA, 0,
                                   0, 0 );


  // End the request.
  if( bResults )
    bResults = WinHttpReceiveResponse( hRequest, NULL );
  else
	  return false;
  // Keep checking for data until there is nothing left.
  if( bResults )
  {
    do
    {
      // Check for available data.
      dwSize = 0;
      if( !WinHttpQueryDataAvailable( hRequest, &dwSize ) )
        printf( "Error %u in WinHttpQueryDataAvailable.\n",
                GetLastError( ) );

      // Allocate space for the buffer.
      pszOutBuffer = new char[dwSize+1];
      if( !pszOutBuffer )
      {
        printf( "Out of memory\n" );
        dwSize=0;
      }
      else
      {
        // Read the data.
        ZeroMemory( pszOutBuffer, dwSize+1 );

        if( !WinHttpReadData( hRequest, (LPVOID)pszOutBuffer,
                              dwSize, &dwDownloaded ) )
          printf( "Error %u in WinHttpReadData.\n", GetLastError( ) );
        else
          printf( "%s", pszOutBuffer );

        // Free the memory allocated to the buffer.
        delete [] pszOutBuffer;
      }
    } while( dwSize > 0 );
  }


  // Report any errors.
  if( !bResults )
    printf( "Error %d has occurred.\n", GetLastError( ) );

  // Close any open handles.
  if( hRequest ) WinHttpCloseHandle( hRequest );
  if( hConnect ) WinHttpCloseHandle( hConnect );
  if( hSession ) WinHttpCloseHandle( hSession );
  return true;
}

#define IsHexNum(c) ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))

CString Utf8ToStringT(LPSTR str)
{
	_ASSERT(str);
	USES_CONVERSION;
	WCHAR *buf;
	int length = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	buf = new WCHAR[length + 1];
	ZeroMemory(buf, (length + 1) * sizeof(WCHAR));
	MultiByteToWideChar(CP_UTF8, 0, str, -1, buf, length);

	return (CString(W2T(buf)));
}
CString UrlDecode(LPCTSTR url)
{
	_ASSERT(url);
	USES_CONVERSION;
	LPSTR _url = T2A(const_cast<LPTSTR>(url));
	int i = 0;
	int length = (int)strlen(_url);
	CHAR *buf = new CHAR[length];
	ZeroMemory(buf, length);
	LPSTR p = buf;
	char tmp[4];
	while (i < length)
	{
		if (i <= length - 3 && _url[i] == '%' && IsHexNum(_url[i + 1]) && IsHexNum(_url[i + 2]))
		{
			memset(tmp, 0, sizeof(tmp));
			memcpy(tmp, _url + i + 1, 2);
			sscanf(tmp, "%x", p++);
			i += 3;
		}
		else
		{
			*(p++) = _url[i++];
		}
	}
	return Utf8ToStringT(buf);
}
std::string UrlEncode(const std::string& szToEncode)
{
	std::string src = szToEncode;
	char hex[] = "0123456789ABCDEF";
	string dst;

	for (size_t i = 0; i < src.size(); ++i)
	{
		unsigned char cc = src[i];
		if (isascii(cc))
		{
			if (cc == ' ')
			{
				dst += "%20";
			}
			else
				dst += cc;
		}
		else
		{
			unsigned char c = static_cast<unsigned char>(src[i]);
			dst += '%';
			dst += hex[c / 16];
			dst += hex[c % 16];
		}
	}
	return dst;
}

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
public:
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_WM_TIMER()
END_MESSAGE_MAP()


// CshanxunDlg �Ի���

BEGIN_DHTML_EVENT_MAP(CshanxunDlg)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



CshanxunDlg::CshanxunDlg(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CshanxunDlg::IDD, CshanxunDlg::IDH, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CshanxunDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CshanxunDlg, CDHtmlDialog)
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(IDC_BUTTON1, &CshanxunDlg::OnBnClickedButton1)
	ON_WM_TIMER()
	ON_WM_DESTROY()
//	ON_EN_CHANGE(IDC_EDIT3, &CshanxunDlg::OnEnChangeEdit3)
ON_BN_CLICKED(IDC_BUTTON1, &CshanxunDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CshanxunDlg ��Ϣ�������

BOOL CshanxunDlg::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();

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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	GetDlgItem(IDC_EDIT1)->SetWindowTextA(user);
	GetDlgItem(IDC_EDIT2)->SetWindowTextA(password);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CshanxunDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDHtmlDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CshanxunDlg::OnPaint()
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
		CDHtmlDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CshanxunDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HRESULT CshanxunDlg::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;
}

HRESULT CshanxunDlg::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;
}


//void CAboutDlg::OnTimer(UINT_PTR nIDEvent)
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//	if(!httpsend())
//        ras();
//    else
//		this->MessageBox("���ӳɹ�");
//	CDialogEx::OnTimer(nIDEvent);
//}


void CshanxunDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	 WORD  wVersionRequested; 
 WSADATA  wsaData; 
 char  name[255]; 
 CString  ip;
 int err;

 PHOSTENT  hostinfo; 
 wVersionRequested  =  MAKEWORD( 1, 1 );//�汾��1.1

 //1.�����׽��ֿ� 
 err = WSAStartup( wVersionRequested, &wsaData );
 if ( err != 0 ) {
  return;
 }

 //�ж��Ƿ����������winsocket�汾���������
 //�����WSACleanup��ֹwinsocket��ʹ�ò�����            

 if ( LOBYTE( wsaData.wVersion ) != 1 ||
  HIBYTE( wsaData.wVersion ) != 1 ) {

   WSACleanup( );
   return;
  }///...if

 if  ( err  ==  0  ) 
 { 
  
  if(  gethostname  (  name,  sizeof(name))  ==  0) 
  { 
   if((hostinfo  =  gethostbyname(name))  !=  NULL) 
   { 
    ip  =  inet_ntoa  (*(struct  in_addr  *)*hostinfo->h_addr_list); 
   } 
  } 
  
  WSACleanup(  ); 
 }   

 this->GetDlgItem(IDC_EDIT1)->GetWindowTextA(user);
	GetDlgItem(IDC_EDIT2)->GetWindowTextA(password);
//CString m_IPAddress = ip;
GetDlgItem(IDC_STATIC)->SetWindowTextA(ip);
 if(ip[0]=='1'&&ip[1]=='1'&&ip[2]=='5')
	 ;
 else
     ras();
	
	CDHtmlDialog::OnTimer(nIDEvent);
}


void CshanxunDlg::OnDestroy()
{
	CDHtmlDialog::OnDestroy();
	RasHangUp(handle);
	// TODO: �ڴ˴������Ϣ����������
}


//void CshanxunDlg::OnEnChangeEdit3()
//{
//	// TODO:  If this is a RICHEDIT control, the control will not
//	// send this notification unless you override the CDHtmlDialog::OnInitDialog()
//	// function and call CRichEditCtrl().SetEventMask()
//	// with the ENM_CHANGE flag ORed into the mask.
//
//	// TODO:  Add your control notification handler code here
//}


void CshanxunDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	SetTimer(1, 2000, NULL);
}
