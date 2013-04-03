
// Win8APPAssistDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Win8APPAssist.h"
#include "Win8APPAssistDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#ifdef UNICODE
#define Tstring wstring
#else
#define Tstring string
#endif
#define RELEASE(p)    { if(p) { delete p; p = NULL; } }
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
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


// CWin8APPAssistDlg dialog



CWin8APPAssistDlg::CWin8APPAssistDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWin8APPAssistDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWin8APPAssistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWin8APPAssistDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CWin8APPAssistDlg::OnFolderBtnClickedButton1)
	ON_BN_CLICKED(IDOK, &CWin8APPAssistDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CWin8APPAssistDlg message handlers

BOOL CWin8APPAssistDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	path = new TCHAR[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,path);
	SetDlgItemText(IDC_EDIT1, L"test");
	SetDlgItemText(IDC_EDIT2, L"D:\\win8app");
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	ppath = reinterpret_cast<CEdit *> (GetDlgItem (IDC_EDIT2));
	pname = reinterpret_cast<CEdit *> (GetDlgItem (IDC_EDIT1));
	((CButton *)GetDlgItem(IDC_NAVITE))->SetCheck(TRUE);
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CWin8APPAssistDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWin8APPAssistDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWin8APPAssistDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CWin8APPAssistDlg::OnFolderBtnClickedButton1()
{
	// TODO: Add your control notification handler code here
	HRESULT hr;
	IFileOpenDialog *pOpenFolderDialog;

	// CoCreate the dialog object.
	hr = CoCreateInstance(CLSID_FileOpenDialog, 
		NULL, 
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&pOpenFolderDialog));

	if (SUCCEEDED(hr))
	{
		pOpenFolderDialog->SetOptions(FOS_PICKFOLDERS);

		// Show the dialog
		hr = pOpenFolderDialog->Show(::GetActiveWindow());

		if (SUCCEEDED(hr))
		{
			// Obtain the result of the user's interaction with the dialog.
			IShellItem *psiResult;
			hr = pOpenFolderDialog->GetResult(&psiResult);

			if (SUCCEEDED(hr))
			{
				// Do something with the result.
				LPWSTR pwsz = NULL;

				hr = psiResult->GetDisplayName ( SIGDN_FILESYSPATH, &pwsz );

				if ( SUCCEEDED(hr) ) {
					//MessageBox ( CString(pwsz), _T("Caption") );
					ppath->SetWindowTextW(CString(pwsz));
					CoTaskMemFree ( pwsz );
				}
				psiResult->Release();
			}
		}
		pOpenFolderDialog->Release();
	}

}



void CWin8APPAssistDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	LPSTR pName =  new char[100];
	LPSTR pPathname =  new char[100];
	HWND dlg = ::GetActiveWindow();
	GetDlgItemTextA(dlg,IDC_EDIT1, pName,100); 
	GetDlgItemTextA(dlg,IDC_EDIT2, pPathname,100);
	if(*pName=='\0'){
		MessageBox(L"please input project name!", L"Hello, Windows!");
		return;
	}

	if(*pPathname=='\0'){
		MessageBox(L"please input project path!", L"Hello, Windows!");
		return;
	}
	CString cName(pName);
	CString cPathname(pPathname);
	CString srcPath(path);
	if(createProject(cName,cPathname)){
		CString nativePath = cPathname+ L"\\native";
		if (GetFileAttributes(nativePath) == INVALID_FILE_ATTRIBUTES) {
			CreateDirectory(nativePath,NULL);
		}
		if ( BST_CHECKED == IsDlgButtonChecked(IDC_NOTIFICATION ) )
		{
			CopyProject(L"notifications",path+notifications,notificationsProjectFile,nativePath+L"\\notifications");
			addProjectReference(cPathname+L"\\"+cName+"\\"+cName+L".jsproj",L"..\\native\\notifications\\"+notificationsProjectFile);
		}
		if(BST_CHECKED == IsDlgButtonChecked(IDC_SQLITE3 ))
		{
			//injectSqliteFileToMainProjectFile();
			copyFolderAllFiles(srcPath+L"\\native\\src\\third_party",nativePath+L"\\third_party");
			copyFolderAllFiles(srcPath+L"\\native\\src\\sqlitejs",cPathname+"\\"+cName+L"\\js\\data");
			changeMainProjectFile(cPathname+L"\\"+cName+"\\"+cName+L".jsproj");
			CopyProject(L"sqlite3",path+sqlite3,sqlite3ProjectFile,nativePath+L"\\sqlite3");
			addProjectReference(cPathname+L"\\"+cName+"\\"+cName+L".jsproj",L"..\\native\\sqlite3\\"+sqlite3ProjectFile);
		}
		if(BST_CHECKED == IsDlgButtonChecked(IDC_NAVITE ))
		{
			CopyProject(L"native",path+native,nativeProjectFile,nativePath+L"\\native");
			addProjectReference(cPathname+L"\\"+cName+"\\"+cName+L".jsproj",L"..\\native\\native\\"+nativeProjectFile);
		}
		if(BST_CHECKED == IsDlgButtonChecked(IDC_JSONXMLCONVERTER ))
		{
			CopyProject(L"jsonxmlconverter",path+jsonxmlconverter,jsonxmlconverterProjectFile,nativePath+L"\\jsonxmlconverter");
			CopyProject(L"json",path+json,jsonProjectFile, nativePath+L"\\json");
			addProjectReference(cPathname+L"\\"+cName+"\\"+cName+L".jsproj",L"..\\native\\jsonxmlconverter\\"+jsonxmlconverterProjectFile);
		}
		if(BST_CHECKED == IsDlgButtonChecked(IDC_WBXML ))
		{
			CopyProject(L"wbxml",path+wbxml,wbxmlProjectFile, nativePath+L"\\wbxml");
			addProjectReference(cPathname+L"\\"+cName+"\\"+cName+L".jsproj",L"..\\native\\wbxml\\"+wbxmlProjectFile);
		}
	}
	delete pName;
	delete pPathname;
	//CDialogEx::OnOK();
}
void CWin8APPAssistDlg::addProjectReference(CString projectFile,CString referenceProjectFile){
	char *saRet = new char[projectFile.GetLength()+1];
	CString2Char(projectFile,saRet);
	referenceProjectFile = L"<ProjectReference Include='"+referenceProjectFile+L"' />";
	char *referenceFile = new char[referenceProjectFile.GetLength()+1];
	CString2Char(referenceProjectFile,referenceFile);
	TiXmlDocument doc(saRet);
	bool loadOkay = doc.LoadFile();
	if ( !loadOkay )
	{
		TRACE( "Could not load test file %s. Error='%s'. Exiting.\n",saRet, doc.ErrorDesc() );
		//exit( 1 );
	}
	TiXmlHandle docH( &doc );
		TiXmlElement *contentElement = docH.FirstChildElement("Project").ChildElement("ItemGroup",2).Element();
	if(contentElement){
		TiXmlElement elepro( "sql" );
		elepro.Parse(referenceFile, 0, TIXML_ENCODING_UNKNOWN );
		contentElement->InsertEndChild(elepro);
	}
	doc.SaveFile();
}
BOOL CWin8APPAssistDlg::createProject(CString name,CString targetPath){
	CString webappPath(path);
	webappPath +="\\webapp";
	targetPath = targetPath+"\\"+ name;
	if (GetFileAttributes(targetPath) == INVALID_FILE_ATTRIBUTES) {
		CreateDirectory(targetPath,NULL);
	}
	CopyFile(webappPath+L"\\webapp.sln",targetPath+".sln",FALSE);
	copyFolderAllFiles(webappPath+L"\\webapp",targetPath);
	CString targetProjectFile = targetPath+"\\"+name+".jsproj";
	if (GetFileAttributes(targetProjectFile) != INVALID_FILE_ATTRIBUTES) {
		CFile::Remove(targetProjectFile);
	}
	CFile::Rename(targetPath+"\\webapp.jsproj",targetProjectFile);
	targetSlnFile = targetPath+L".sln";
	Sln *slnFile = new Sln(targetSlnFile);
	slnFile->changeProject(L"webapp",name+L"\\"+name+L".jsproj");
	RELEASE(slnFile);
	return TRUE;
}
void CWin8APPAssistDlg::CString2Char(CString cstr,char* &saRet){
	std::Tstring str(cstr.GetBuffer());
	int nLength = str.length()+1;
	TCHAR *c_str = new TCHAR[nLength];
	//char *saRet = new char[nLength];
	wcscpy_s(c_str,nLength,str.c_str());
	WideCharToMultiByte(CP_ACP, 0, cstr, nLength, saRet, 
		nLength*2+1, NULL, NULL);
	cstr.ReleaseBuffer();
	delete []c_str;
	c_str = NULL;
}
CString CWin8APPAssistDlg::getProjectGuid(CString file){
	char *saRet = new char[file.GetLength()+1];
	CString2Char(file,saRet);
	TiXmlDocument doc(saRet);
	bool loadOkay = doc.LoadFile();

	if ( !loadOkay )
	{
		TRACE( "Could not load test file %s. Error='%s'. Exiting.\n",saRet, doc.ErrorDesc() );
		//exit( 1 );
	}
	TiXmlHandle docH( &doc );
	TiXmlElement* element = docH.FirstChildElement( "Project").FirstChildElement("PropertyGroup").FirstChildElement("ProjectGuid").Element();
	CString guid=NULL;
	if ( element ){
		guid = element->GetText();
	}
	TRACE(_T("%s\r\n"),guid);
	return guid;
}
BOOL CWin8APPAssistDlg::changeMainProjectFile(CString file){
	char *saRet = new char[file.GetLength()+1];
	CString2Char(file,saRet);
	TiXmlDocument doc(saRet);
	bool loadOkay = doc.LoadFile();

	if ( !loadOkay )
	{
		TRACE( "Could not load test file %s. Error='%s'. Exiting.\n",saRet, doc.ErrorDesc() );
		//exit( 1 );
	}
	TiXmlHandle docH( &doc );
	TiXmlElement *contentElement = docH.FirstChildElement("Project").ChildElement("ItemGroup",1).Element();
	if(contentElement){
		TiXmlElement elementsql( "sql" ),elementsqlext("sqlext");
		elementsql.Parse( "<Content Include='js\\data\\sqlite3.js' />", 0, TIXML_ENCODING_UNKNOWN );
		elementsqlext.Parse( "<Content Include='js\\data\\sqlite_ext.js' />", 0, TIXML_ENCODING_UNKNOWN );
		contentElement->InsertEndChild(elementsql);
		contentElement->InsertEndChild(elementsqlext);
	}
	doc.SaveFile();
	return true;
}
void CWin8APPAssistDlg::copyFolderAllFiles(CString csSourceFolder, CString csNewFolder)
{
	CFileFind f;
	BOOL bFind=f.FindFile(csSourceFolder+"\\*.*");
	CString tmp = csNewFolder + "\\";
	while(bFind)
	{
		bFind = f.FindNextFile();
		TRACE(_T("%s\r\n"),f.GetFileName());
		if (GetFileAttributes(csNewFolder) == INVALID_FILE_ATTRIBUTES) 
		{
			CreateDirectory(csNewFolder,NULL);
		}
		if(f.IsDots()) continue;
		if(f.IsDirectory())
		{
			copyFolderAllFiles(csSourceFolder+"\\"+f.GetFileName(),csNewFolder+"\\"+f.GetFileName());
		}
		::SetFileAttributes(csSourceFolder+"\\"+f.GetFileName(),FILE_ATTRIBUTE_NORMAL);
		::AfxGetApp()->DoWaitCursor(1);
		CString source = csSourceFolder+"\\"+f.GetFileName();
		if(csNewFolder.Find(L"home")!=-1)
			CString s;
		
		CString dest = csNewFolder+"\\"+f.GetFileName();
		::CopyFile(source,dest,FALSE);
		::AfxGetApp()->DoWaitCursor(-1);

	}
}
char* CWin8APPAssistDlg::NewGuid(char* guidBuf)
{
	memset(guidBuf,0,64);
	GUID guid;
	CoInitialize(NULL);
	if(S_OK == CoCreateGuid(&guid))
	{
		sprintf_s(guidBuf, 64,
			"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
			guid.Data1,
			guid.Data2,
			guid.Data3,
			guid.Data4[0], guid.Data4[1],
			guid.Data4[2], guid.Data4[3],
			guid.Data4[4], guid.Data4[5],
			guid.Data4[6], guid.Data4[7]);
	}
	CoUninitialize();
	return guidBuf;
}

WCHAR* CWin8APPAssistDlg::NewGuid(WCHAR* guidBuf)
{
	char guidBufA[64];
	memset(guidBufA,0,64);
	NewGuid(guidBufA);
	if(guidBufA)
	{
		MultiByteToWideChar(CP_ACP,NULL,guidBufA,64,guidBuf,64);
	}
	return guidBuf;
}

void  CWin8APPAssistDlg::CopyProject(CString projectName,CString srcProjectPath,CString srcProjectFile,CString targetPath){
	CString guid = getProjectGuid(srcProjectPath+L"\\"+srcProjectFile);
	WCHAR *nguid = new WCHAR[64];
	CString slnGuid = NewGuid(nguid);
	Sln *slnFile = new Sln(targetSlnFile);
	slnFile->addProject(projectName,+L"native\\"+projectName+L"\\"+srcProjectFile,guid);
	copyFolderAllFiles(srcProjectPath,targetPath);
	RELEASE(slnFile);

}