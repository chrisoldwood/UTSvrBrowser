/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		UTSVRBROWSER.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CUTSvrBrowser class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef UTSVRBROWSER_HPP
#define UTSVRBROWSER_HPP

/******************************************************************************
** 
** The application class.
**
*******************************************************************************
*/

class CUTSvrBrowser : public CApp
{
public:
	//
	// Constructors/Destructor.
	//
	CUTSvrBrowser();
	~CUTSvrBrowser();

	//
	// Members
	//
	CAppWnd		m_AppWnd;			// Main window.
	CAppCmds	m_AppCmds;			// Command handler.

	CIniFile	m_oIniFile;			// .INI FIle
	CRect		m_rcLastPos;		// Main window position.
	CString		m_strMasterAddr;	// Master server address.
	int			m_nMasterPort;		// Master server port.

	CMDB		m_oMDB;				// Memory database.
	CServers	m_oServers;			// Servers table.
	
	//
	// Constants.
	//
	static const char* VERSION;

protected:
	//
	// Startup and Shutdown template methods.
	//
	virtual	bool OnOpen();
	virtual	bool OnClose();

	//
	// Internal methods.
	//
	void LoadConfig();
	void SaveConfig();

	//
	// Constants.
	//
	static const char* INI_FILE_VER;
	static const char* DEF_MASTER_ADDRESS;
	static int         DEF_MASTER_PORT;
};

/******************************************************************************
**
** Global variables.
**
*******************************************************************************
*/

// The application object.
extern CUTSvrBrowser App;

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/


#endif //UTSVRBROWSER_HPP
