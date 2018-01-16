#include "stdafx.h"
#include "IPC.h"

cIPC::~cIPC ( )
{
	if ( m_lpMappedViewOfFile ) UnmapViewOfFile ( m_lpMappedViewOfFile );
	if ( m_hFileMap           ) CloseHandle     ( m_hFileMap           );
	if ( m_hDataMutex         ) CloseHandle     ( m_hDataMutex         );
	if ( m_hDataEvent         ) CloseHandle     ( m_hDataEvent         );
}

cIPC::cIPC ( LPCTSTR SharedName, DWORD Size, BOOL bHandlesInheritable ) : m_nSize ( Size ), m_bHandlesInheritable ( bHandlesInheritable )
{
	m_hDataMutex			= NULL;
	m_hDataEvent			= NULL;
	m_lpMem					= NULL;
	m_hFileMap				= NULL;
	m_lpMappedViewOfFile	= NULL;

	wsprintf ( m_szFileMapName, _T ( "%s" ), SharedName );
	wsprintf ( m_szMutexName,   _T ( "MUTEX_%s"   ), SharedName );
	wsprintf ( m_szEventName,   _T ( "EVENT_%s"   ), SharedName );

	// open or create file map
	m_hFileMap = OpenFileMapping ( FILE_MAP_ALL_ACCESS, m_bHandlesInheritable, m_szFileMapName );
	if ( m_hFileMap == NULL )
	{
		DWORD dwAccessFlags = PAGE_READWRITE | SEC_COMMIT | SEC_NOCACHE;
		m_hFileMap = CreateFileMapping ( ( HANDLE ) - 1, NULL, dwAccessFlags, NULL, Size + sizeof ( tInterData ), m_szFileMapName );
		LastError = ::GetLastError ( );
		if ( m_hFileMap == NULL )
			return;
	}

	// map view of file (size should be zero for ME and 98)
	m_lpMappedViewOfFile = MapViewOfFile ( m_hFileMap, FILE_MAP_ALL_ACCESS, 0, 0, 0 );
	if ( m_lpMappedViewOfFile == NULL )
	{
		CloseHandle ( m_hFileMap );
		m_hFileMap = NULL;
		return;
	}

	// initialize internal data structure
	m_ipcd = ( tInterData* ) m_lpMappedViewOfFile;

	// initialize buffer exchange memory
	m_lpMem = ( LPVOID ) ( ( DWORD ) m_lpMappedViewOfFile + sizeof ( tInterData ) );

	// try to open mutex
	m_hDataMutex = OpenMutex ( MUTEX_ALL_ACCESS, m_bHandlesInheritable, m_szMutexName );
	if ( m_hDataMutex == NULL )
	{
		m_hDataMutex = CreateMutex (NULL, FALSE, m_szMutexName );
		if ( m_hDataMutex == NULL )
		{
			UnmapViewOfFile   ( m_lpMappedViewOfFile );
			CloseHandle       ( m_hFileMap );
			return;
		}
	}

	// try to open event
	m_hDataEvent = OpenEvent ( EVENT_ALL_ACCESS, FALSE, m_szEventName );
	if ( m_hDataEvent == NULL )
	{
		m_hDataEvent = CreateEvent ( NULL, TRUE, FALSE, m_szEventName );
		if ( m_hDataEvent == NULL )
		{
			CloseHandle       ( m_hDataMutex );
			UnmapViewOfFile   ( m_lpMappedViewOfFile );
			CloseHandle       ( m_hFileMap );
			return;
		}
	}

	// success
	LastError = ERROR_SUCCESS;
}

void cIPC::ReceiveBuffer ( LPVOID Buffer, DWORD dwOffset, DWORD Size )
{
	// Now can read the filemap
	bool bTryAgain = true;
	int iManyAttempts = 5;
	while ( iManyAttempts>0 && bTryAgain==true )
	{
		// X10 - 051207 - remove infinite that MIGHT be causing BUILD GAME (and other) freezes
		//WaitForSingleObject ( m_hDataMutex, INFINITE );
		WaitForSingleObject ( m_hDataMutex, 1000 );
		try 
		{ 
			bTryAgain=false;
			BYTE* lpData = ( BYTE* ) m_lpMem;
			memcpy ( Buffer, &lpData [ dwOffset ], Size );
		} 
		catch (...)
		{
			iManyAttempts--;
			bTryAgain=true;
		}
		ReleaseMutex ( m_hDataMutex );
	}
}

void cIPC::SendBuffer ( LPVOID Buffer, DWORD dwOffset, DWORD Size )
{
	// When have it, write to filemap
	bool bTryAgain = true;
	int iManyAttempts = 5;
	while ( iManyAttempts>0 && bTryAgain==true )
	{
		// Wait for ownership of filemap mutex, then write
		try 
		{ 
			// X10 - 051207 - remove infinite that MIGHT be causing BUILD GAME (and other) freezes
			//WaitForSingleObject ( m_hDataMutex, INFINITE );
			WaitForSingleObject ( m_hDataMutex, 1000 );
			bTryAgain=false;
			BYTE* lpData = ( BYTE* ) m_lpMem;
			memcpy ( &lpData [ dwOffset ], Buffer, Size );
		} 
		catch (...)
		{
			iManyAttempts--;
			bTryAgain=true;
		}

		// Release ownership of filemap mutex
		ReleaseMutex ( m_hDataMutex );
	}
}

bool cIPC::SkipIfEventNotWaiting ( void )
{
	// if no event ptr
	if ( !m_hDataEvent )
		return true;
	
	// if event between interface and mapeditor filemap
	if ( m_hDataEvent )
	{
		// check if event is signalled ('wait for file map event' NOT been called yet)
		if ( WaitForSingleObject ( m_hDataEvent, 0 )==WAIT_OBJECT_0)
		{
			// return without action - must skip until child app ready and 'waiting'
			return false;
		}
		else
		{
			// event has been reset (mapeditor waiting) - so update the interface NOW
		}
	}

	// can do interface code
	return true;
}

void cIPC::AllowSyncEventNow ( void )
{
	// When interface completed its 1millisecond activity, then set the event for waiting child processes
	// this is called at the end of all the update functions for the IPC 
	SetEvent ( m_hDataEvent );
}
