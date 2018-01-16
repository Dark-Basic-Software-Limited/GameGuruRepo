// EditorDoc.cpp : implementation of the CEditorDoc class
//

#include "stdafx.h"
#include "Editor.h"

#include "EditorDoc.h"
#include "CntrItem.h"

#include <tlhelp32.h>

#include "enumprocess.h"
#include "DownloadStoreItems\cleanupFolders.h"

#include "Aclapi.h"

#include "mmsystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc

IMPLEMENT_DYNCREATE(CEditorDoc, COleDocument)

BEGIN_MESSAGE_MAP(CEditorDoc, COleDocument)
	//{{AFX_MSG_MAP(CEditorDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Enable default OLE container implementation
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, COleDocument::OnUpdatePasteMenu)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE_LINK, COleDocument::OnUpdatePasteLinkMenu)
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_CONVERT, COleDocument::OnUpdateObjectVerbMenu)
	ON_COMMAND(ID_OLE_EDIT_CONVERT, COleDocument::OnEditConvert)
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_LINKS, COleDocument::OnUpdateEditLinksMenu)
	ON_COMMAND(ID_OLE_EDIT_LINKS, COleDocument::OnEditLinks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OLE_VERB_FIRST, ID_OLE_VERB_LAST, COleDocument::OnUpdateObjectVerbMenu)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc construction/destruction

CEditorDoc::CEditorDoc()
{
	// Use OLE compound files
	EnableCompoundFile();

	// TODO: add one-time construction code here

	m_bQuit = true;
}

CEditorDoc::~CEditorDoc()
{
}

void CEditorDoc::SetLevelName ( LPTSTR szText )
{
	int c = 0;

	CString title = GetTitle( );
	LPCTSTR test = _T("abc");

	//SetTitle( test );

	//m_strTitle = test;

	//pFrame->OnUpdateFrameTitle(TRUE);

//	OnUpdateFrameTitle(TRUE);

	//CFrameWnd* pFrame = pView->GetParentFrame();

	/*
	POSITION pos = GetFirstViewPosition();
	CView* pView = GetNextView(pos);
	CFrameWnd* pFrame = pView->GetParentFrame();
	
	
	pFrame->OnUpdateFrameTitle(TRUE);
	*/

	//HWND hwnd = GetSafeWindowHandle ( );
	//AfxSetWindowText(NULL, test);
}

BOOL CEditorDoc::OnNewDocument ( )
{
	// create a new document
	theApp.m_Debug.Write ( "CEditorDoc::OnNewDocument" );

	// call the base class function
	if ( !COleDocument::OnNewDocument ( ) )
	{
		theApp.m_Debug.Write ( "CEditorDoc::Failed" );
		return FALSE;
	}

	// string to store executable link
	TCHAR szExecutable [ MAX_PATH ];
	theApp.m_Debug.Write ( "CEditorDoc::SetCurrentDirectory" );
	theApp.m_Debug.Write ( theApp.m_szDirectory );
	SetCurrentDirectory ( theApp.m_szDirectory );

	// dave - clean up folder before start IDE
	// 014  - being ultra careful despite this only removing folders
	// 015 - put back in, clearly is fine as it always wass
	cleanupFolders();

	// get the exe to run
	SetCurrentDirectory ( theApp.m_szDirectory );
	//GetPrivateProfileString ( _T ( "General" ), _T ( "Executable" ), _T ( "" ), szExecutable, MAX_PATH, theApp.m_szLanguageVariant );
	strcpy ( szExecutable, "Guru-MapEditor.exe" );

	// get pointer to frame
	CMainFrame* pFrame = ( CMainFrame* ) theApp.m_pMainWnd;

	// show library window
	pFrame->SetEntityVisible ( TRUE );
	SetTitle ( _T ( "Editor" ) );
 	theApp.m_pDocumentA = this;

	// restarting app
	if ( theApp.m_bRestart )
	{
		theApp.m_Debug.Write ( "CEditorDoc::m_bRestart" );
		theApp.m_bAppRunning = true;
		theApp.m_bRestart  = false;
		theApp.m_DBProApplicationLaunched = false;

		SetCurrentDirectory ( theApp.m_szDirectory );
		char szExeANSI [ 255 ] = "";
		char szParamANSI [ 255 ] = "";
		CString szParamUNI;
		CString szExeUNI;
		CString szDirUNI;
		cIPC* pIPC	= theApp.m_Message.GetIPC ( );
		char  dataA [ 255 ] = "";
		char  dataB [ 255 ] = "";
			
		pIPC->ReceiveBuffer ( &dataA [ 0 ], 1000, sizeof ( dataA ) );
		pIPC->ReceiveBuffer ( &dataB [ 0 ], 1256, sizeof ( dataB ) );

		ConvertANSIToWide ( ( BYTE* ) &dataA [ 0 ], szExeANSI,   &szExeUNI );
		ConvertANSIToWide ( ( BYTE* ) &dataB [ 0 ], szParamANSI, &szParamUNI );

		// 050315 - 015 - changed to pass in the working directory as when steam has done an update for some people the working directory is wrong
		ConvertANSIToWide ( theApp.m_szDirectory, &szDirUNI );

		theApp.SetFileMapData ( 912, 0 );
		theApp.SetFileMapData ( 916, 0 );

		SHELLEXECUTEINFO seinfo;
		ZeroMemory ( &seinfo, sizeof ( seinfo ) );
		seinfo.cbSize       = sizeof ( seinfo );
		seinfo.fMask        = SEE_MASK_NOCLOSEPROCESS;
		seinfo.hwnd         = NULL;
		seinfo.lpVerb       = _T ( "open" );
		seinfo.lpFile       = szExeUNI;
		seinfo.lpParameters = szParamUNI;
		seinfo.lpDirectory  = szDirUNI;
		seinfo.nShow        = SW_HIDE;
			
		char szDebug [ 255 ] = "";
		theApp.m_Debug.Write ( "RESTART EDITOR" );
		sprintf ( szDebug, "Shell execute - executable - %s", szExeANSI   );
		theApp.m_Debug.Write ( szDebug );
		sprintf ( szDebug, "Shell execute - parameters - %s", szParamANSI );
		theApp.m_Debug.Write ( szDebug );
		theApp.m_iRecover = 0;

		pIPC = theApp.m_Message.GetIPC ( );
		BYTE dataClear [ 6144 ] = { 0 };
		pIPC->SendBuffer ( &dataClear [ 0 ], 0, sizeof ( dataClear ) );

		theApp.SetFileMapData ( 44, 1 );
		theApp.SetFileMapData ( 486, 1 );
		theApp.m_bDisplayMouse = TRUE;
		theApp.m_lastPoint.x = 0;
		theApp.m_lastPoint.y = 0;
		theApp.m_bEnd = FALSE;
		theApp.m_bDisable = true;
		theApp.m_bFocus = false;
		theApp.m_bEntityFocus = false;
		theApp.m_iRecover = 0;
		theApp.m_bRestart = false;

		// check running processes
		theApp.m_Debug.Write ( "CEditorDoc::Checking running processes" );
				
		// 010205
		while ( 1 )
		{
			CEnumProcess tmp;
			CString s;
			tmp.SetMethod ( 0x1 );
			CEnumProcess::CProcessEntry entry;
			bool bGame = false;
					
			for (BOOL OK = tmp.GetProcessFirst(&entry); OK; OK = tmp.GetProcessNext(&entry) )
			{
				int image = 0;
				if (entry.hTask16)
				{
					s.Format("%08X (%ld)", entry.dwPID, entry.hTask16);
					image = 1;
				}
				else
					s.Format("%08X", entry.dwPID);

				TRACE(s + " %s\n", entry.lpFilename);

				_strlwr ( entry.lpFilename );
				CString		process = entry.lpFilename;
				char		szTextANSI [ 255 ]	= "";
				ConvertWideToANSI ( NULL, &process, szTextANSI );
				char  seps[ ] = ".\\//";
				char* token;
				token = strtok ( szTextANSI, seps );

				while ( token != NULL )
				{
					if ( _strnicmp ( token, "Guru-game", strlen ( token ) ) == 0 ) bGame = true;
					token = strtok ( NULL, seps );
				}
			}

			if ( bGame ) theApp.m_Debug.Write ( "Guru-game.exe - running" );
			if ( !bGame )
			{
				theApp.m_Debug.Write ( "Guru-game.exe - not running - quit" );
				break;
			}
		}

		// 170105
		theApp.m_bTestGameCancel = true;
	
		// 020205 - original
		if ( ShellExecuteEx ( &seinfo ) == TRUE )
		{
			// set security to allow IDE to terminate the process when it likes (old ones for example)
			SetSecurityInfo ( seinfo.hProcess, SE_KERNEL_OBJECT, PROCESS_TERMINATE, NULL, NULL, NULL, NULL);

			// wait max of 20 seconds for the map editor to start, soon as it has started carry on
			DWORD dwTime = timeGetTime();
			bool bWaitingForExeToStart = true;

			while ( bWaitingForExeToStart )
			{
				if ( timeGetTime() - dwTime > 20000 )
					bWaitingForExeToStart = false;

				//if ( theApp.IsProcessRunning ( "Guru-MapEditor" ) )
				//	bWaitingForExeToStart = false;
				bool bMapEditor = theApp.IsProcessRunning("Game Guru12345");
				if (bMapEditor == false) bMapEditor = theApp.IsProcessRunning("Guru-MapEditor");
				if (bMapEditor == true) bWaitingForExeToStart = false;
			}

			theApp.m_DBProApplicationLaunched = true;
			theApp.m_DBProApplication		  = seinfo;

			theApp.m_hInstance = seinfo.hInstApp;
			theApp.m_bAppRunning = true;

			theApp.m_Debug.Write ( "Shell execute - success" );
		}
		else
		{
			theApp.m_Debug.Write ( "Shell execute - failed" );
			theApp.m_DBProApplicationLaunched = false;
		}
			
		if ( seinfo.hInstApp <= ( HINSTANCE ) 32 )
		{
			theApp.m_Debug.Write ( "Shell execute - invalid hinstance" );
			theApp.m_DBProApplicationLaunched = false;
		}
		
		theApp.SetFileMapData ( 974, 0 );
		theApp.m_bFocus       = false;
		theApp.m_bEntityFocus = false;
	}
	else
	{
		// go to root FPSC folder
		SetCurrentDirectory ( theApp.m_szDirectory );

		// create mapeditor process
		SHELLEXECUTEINFO seinfo;
		ZeroMemory ( &seinfo, sizeof ( seinfo ) );
		seinfo.cbSize       = sizeof ( seinfo );

		// Dave - this code will pick up ANY window called FPS Creator Reloaded, so commented it out
		// check if editor already exists (for debugging)
		/*HWND hEditorWnd = FindWindow ( NULL, "FPS Creator Reloaded" );//"FPSC Map Editor" );
		if ( hEditorWnd )
		{
			// debug
			theApp.m_Debug.Write ( "ACQUIRING EDITOR" );

			// fill data from window
			seinfo.hwnd         = hEditorWnd;
			seinfo.hInstApp     = NULL;

			// set application for linkage
			theApp.m_DBProApplicationLaunched = true;
			theApp.m_DBProApplication		  = seinfo;
			theApp.m_hInstance = seinfo.hInstApp;
			theApp.m_bAppRunning = true;
			theApp.m_Debug.Write ( "ACQUIRE - success\n" );
		}
		else*/
		{
			// debug
			theApp.m_Debug.Write ( "LAUNCH EDITOR" );

			// set working directory
			// 050315 - 015 - changed to pass in the working directory as when steam has done an update for some people the working directory is wrong
			CString szDirUNI;
			ConvertANSIToWide ( theApp.m_szDirectory, &szDirUNI );

			// prepare data for launch
			seinfo.fMask        = SEE_MASK_NOCLOSEPROCESS;
			seinfo.hwnd         = NULL;
			seinfo.lpVerb       = _T ( "open" );
			seinfo.lpFile       = szExecutable;
			seinfo.lpParameters = _T ( theApp.m_CommandLineString );
			seinfo.lpDirectory  = szDirUNI;
			seinfo.nShow        = SW_HIDE;

			// shell execute mapeditor
			if ( ShellExecuteEx ( &seinfo ) == TRUE )
			{
				// set security to allow IDE to terminate the process when it likes (old ones for example)
				SetSecurityInfo ( seinfo.hProcess, SE_KERNEL_OBJECT, PROCESS_TERMINATE, NULL, NULL, NULL, NULL);

				// wait max of 20 seconds for the map editor to start, soon as it has started carry on
				DWORD dwTime = timeGetTime();
				bool bWaitingForExeToStart = true;

				while ( bWaitingForExeToStart )
				{
					if ( timeGetTime() - dwTime > 20000 )
						bWaitingForExeToStart = false;

					//if ( theApp.IsProcessRunning ( "Guru-MapEditor" ) )
					//	bWaitingForExeToStart = false;
					bool bMapEditor = theApp.IsProcessRunning("Game Guru12345");
					if ( bMapEditor == false) bMapEditor = theApp.IsProcessRunning("Guru-MapEditor");
					if ( bMapEditor == true ) bWaitingForExeToStart = false;
				}

				theApp.m_DBProApplicationLaunched = true;
				theApp.m_DBProApplication		  = seinfo;

				theApp.m_hInstance = seinfo.hInstApp;
				theApp.m_bAppRunning = true;

				theApp.m_Debug.Write ( "START - success\n" );
			}
			else
			{
				theApp.m_Debug.Write ( "START - failed\n" );
				theApp.m_DBProApplicationLaunched = false;	
			}
		}

		// debug
		theApp.m_Debug.Write ( "FINISHED\n" );
	}

	// debug
	theApp.m_Debug.Write ( "CEditorDoc::End" );

	// complete
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc serialization
 
void CEditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		
	}
	else
	{
		// TODO: add loading code here
	}

	// Calling the base class COleDocument enables serialization
	//  of the container document's COleClientItem objects.
	COleDocument::Serialize(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc diagnostics

#ifdef _DEBUG
void CEditorDoc::AssertValid() const
{
	COleDocument::AssertValid();
}

void CEditorDoc::Dump(CDumpContext& dc) const
{
	COleDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEditorDoc commands

void CEditorDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class

	//SetFocus ( NULL );

	
	
	COleDocument::OnCloseDocument();
}

BOOL CALLBACK EnumWindowsProcEx(HWND hwnd,LPARAM lParam )
{

	TCHAR szBuffer  [ 256 ];
	TCHAR szDestroy [ 256 ];

	SetCurrentDirectory ( theApp.m_szDirectory );
	//GetPrivateProfileString ( _T ( "General" ), _T ( "Destroy Window" ), _T ( "" ), szDestroy, MAX_PATH, theApp.m_szLanguageVariant );
	strcpy ( szDestroy, "Guru Map Editor" );
	GetWindowText ( hwnd, szBuffer, 256 );

	// 120205 - force to lower case
	_strlwr ( szBuffer );
	_strlwr ( szDestroy );

	//theApp.m_Debug.Write ( "EnumWindowsProcEx" );
	//theApp.m_Debug.Write ( szBuffer );

	// 170105
	//if ( wcscmp ( szBuffer, szDestroy ) == 0 )
	if ( strcmp ( szBuffer, szDestroy ) == 0 )
	{
		if ( theApp.m_bAppRunning == false )
			return TRUE;

		theApp.m_Debug.Write ( "ENUM WINDOWS" );
		theApp.m_Debug.Write ( "Destroying FPS Creator Reloaded" );

		// 211204
		theApp.m_Debug.Write ( "SendMessage ( hwnd, WM_DESTROY, 0, 0 );" );
		SendMessage ( hwnd, WM_DESTROY, 0, 0 );

		theApp.m_Debug.Write ( "DestroyWindow ( hwnd )" );
		DestroyWindow ( hwnd );
	}

	return TRUE;
}

BOOL CEditorDoc::SetQuitMessage ( bool bShow )
{
	m_bQuit = bShow;

	
	theApp.m_bSetupFrame = false;

	/*
		EnumWindows ( EnumWindowsProcEx, 0 );

	// call base class function
	return COleDocument::CanCloseFrame ( NULL );
	*/



	return CanCloseFrame ( NULL );
}

BOOL CEditorDoc::CanCloseFrame ( CFrameWnd* pFrame )
{
	theApp.SetFileMapData ( 908, 1 );
	return FALSE;

	/*
	// message for closing the frame

	// string a and b
	TCHAR szStringA [ MAX_PATH ];
	TCHAR szStringB [ MAX_PATH ];

	SetCurrentDirectory ( theApp.m_szDirectory );

	// get strings for message box
	GetPrivateProfileString ( _T ( "Application" ), _T ( "Name" ), _T ( "" ), szStringA, MAX_PATH, theApp.m_szLanguageVariant );
	GetPrivateProfileString ( _T ( "Messages"    ), _T ( "Quit" ), _T ( "" ), szStringB, MAX_PATH, theApp.m_szLanguage );
	
	// show message box
	//int iReturn = ::MessageBox ( NULL, szStringB, szStringA, MB_APPLMODAL | MB_ICONQUESTION | MB_YESNO );

	int iReturn = 0;

	if ( m_bQuit )
		iReturn = ::MessageBox ( NULL, szStringB, szStringA, MB_APPLMODAL | MB_ICONQUESTION | MB_YESNO );

	if ( !m_bQuit )
		iReturn = IDYES;

	// take action based on response
	if ( iReturn == IDYES )
	{
		theApp.m_bSetupFrame = false;
		return TRUE;
	}
	else
		return FALSE;
	*/

	// call base class function
	return COleDocument::CanCloseFrame ( pFrame );
}

BOOL CEditorDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// TODO: Add your specialized code here and/or call the base class

	theApp.SetFileMapData ( 404, 1 );

	/*
	TCHAR szFileName [ MAX_PATH ];

	wcscpy ( szFileName, lpszPathName );

	theApp.SetFileMapDataString ( 1000, szFileName );
	
	return COleDocument::OnSaveDocument(lpszPathName);
	*/

	return FALSE;
}

void CEditorDoc::OnEnterIdle( UINT, CWnd* )
{
	int lee=42;
}
