/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		UTSVRBROWSER.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CUTSvrBrowser class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"

/******************************************************************************
**
** Global variables.
**
*******************************************************************************
*/

// "The" application object.
CUTSvrBrowser App;

/******************************************************************************
**
** Class constants.
**
*******************************************************************************
*/

#ifdef _DEBUG
const char* CUTSvrBrowser::VERSION      = "v1.0 [Debug]";
#else
const char* CUTSvrBrowser::VERSION      = "v1.0";
#endif

const char* CUTSvrBrowser::INI_FILE_VER       = "1.0";
const char* CUTSvrBrowser::DEF_MASTER_ADDRESS = "unreal.epicgames.com";
int         CUTSvrBrowser::DEF_MASTER_PORT    = 28900;

/******************************************************************************
** Method:		Constructor
**
** Description:	Default constructor.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CUTSvrBrowser::CUTSvrBrowser()
	: CApp(m_AppWnd, m_AppCmds)
	, m_oServers(m_oMDB)
	, m_strMasterAddr(DEF_MASTER_ADDRESS)
	, m_nMasterPort(DEF_MASTER_PORT)
{

}

/******************************************************************************
** Method:		Destructor
**
** Description:	Cleanup.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CUTSvrBrowser::~CUTSvrBrowser()
{

}

/******************************************************************************
** Method:		OnOpen()
**
** Description:	Initialises the application.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CUTSvrBrowser::OnOpen()
{
	// Set the app title.
	m_strTitle = "UT Server Browser";

	// Load settings.
	LoadConfig();
	
	// Initialise WinSock.
	int nResult = CWinSock::Startup(1, 1);

	if (nResult != 0)
	{
		FatalMsg("Failed to initialise WinSock layer: %d.", nResult);
		return false;
	}

	// Load the toolbar bitmap.
	m_rCmdControl.CmdBitmap().LoadRsc(IDR_APPTOOLBAR);

	// Create the main window.
	if (!m_AppWnd.Create())
		return false;

	// Show it.
	if ( (m_iCmdShow == SW_SHOWNORMAL) && (m_rcLastPos.Empty() == false) )
		m_AppWnd.Move(m_rcLastPos);

	m_AppWnd.Show(m_iCmdShow);

	// Update UI.
	m_AppCmds.UpdateUI();

	return true;
}

/******************************************************************************
** Method:		OnClose()
**
** Description:	Cleans up the application resources.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CUTSvrBrowser::OnClose()
{
	// Terminate WinSock.
	CWinSock::Cleanup();

	// Save settings.
	SaveConfig();

	return true;
}

/******************************************************************************
** Method:		LoadConfig()
**
** Description:	Load the app configuration.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CUTSvrBrowser::LoadConfig()
{
	// Read the file version.
	CString strVer = m_oIniFile.ReadString("Version", "Version", INI_FILE_VER);

	// Read the window pos and size.
	m_rcLastPos.left   = m_oIniFile.ReadInt("UI", "Left",   0);
	m_rcLastPos.top    = m_oIniFile.ReadInt("UI", "Top",    0);
	m_rcLastPos.right  = m_oIniFile.ReadInt("UI", "Right",  0);
	m_rcLastPos.bottom = m_oIniFile.ReadInt("UI", "Bottom", 0);

	// Read the master server settings.
	m_strMasterAddr = m_oIniFile.ReadString("Master", "Address", m_strMasterAddr);
	m_nMasterPort   = m_oIniFile.ReadInt   ("Master", "Port",    m_nMasterPort);
}

/******************************************************************************
** Method:		SaveConfig()
**
** Description:	Save the app configuration.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CUTSvrBrowser::SaveConfig()
{
	// Write the file version.
	m_oIniFile.WriteString("Version", "Version", INI_FILE_VER);

	// Write the window pos and size.
	m_oIniFile.WriteInt("UI", "Left",   m_rcLastPos.left  );
	m_oIniFile.WriteInt("UI", "Top",    m_rcLastPos.top   );
	m_oIniFile.WriteInt("UI", "Right",  m_rcLastPos.right );
	m_oIniFile.WriteInt("UI", "Bottom", m_rcLastPos.bottom);

	// Write the master server settings.
	m_oIniFile.WriteString("Master", "Address", m_strMasterAddr);
	m_oIniFile.WriteInt   ("Master", "Port",    m_nMasterPort);
}
