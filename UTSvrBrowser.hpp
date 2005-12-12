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

// Template shorthands.
typedef std::vector<int> CIntArray;
typedef std::vector<CFilter*> CFilters;

/******************************************************************************
** 
** Global constants.
**
*******************************************************************************
*/

enum
{
	FILTER_MENU_POS			= 1,
	FILTER_MENU_FIXED_ITEMS	= 2,
	FIRST_FILTER_CMD_POS	= FILTER_MENU_FIXED_ITEMS,
	ID_FIRST_FILTER_CMD		= ID_FILTER_NONE + 1,
	ID_LAST_FILTER_CMD		= 299,
};

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
	CAppWnd				m_AppWnd;			// Main window.
	CAppCmds			m_AppCmds;			// Command handler.

	CIniFile			m_oAppIniFile;		// App .INI File
	CIniFile			m_oModIniFile;		// Mods .INI FIle

	CRect				m_rcLastPos;		// Main window position.
	CIntArray			m_anColWidths;		// The last view column widths.
	int					m_nSortCol;			// The last sort column.
	int					m_nSortDir;			// The last sort direction.

	CMasterQueryOpts	m_oMtrQryOpts;		// The master query settings.
	CPingOpts			m_oPingOpts;		// The ping settings.
	bool				m_bDetectFavs;		// Detect favourites files?

	CMDB				m_oMDB;				// Memory database.
	CMods				m_oMods;			// Mods table.
	CGameTypes			m_oGameTypes;		// Game types table.
	CServers			m_oServers;			// Servers table.
	CFavFiles			m_oFavFiles;		// Favourites .ini files table.
	CFavourites			m_oFavourites;		// Favourites table.
	CSummary			m_oSummary;			// Servers summary table.

	bool				m_bFltEdited;		// Filters modified?
	CFilters			m_aoFilters;		// The filters.
	CFilter*			m_pFilter;			// The current filter.
	CResultSet			m_oRS;				// The filtered servers.

	CPath				m_strCacheFile;		// Cached servers filename.
	
	uint				m_nPingTimerID;		// Ping timer ID.

	CString				m_strFindText;		// Find Server text.
	int					m_nLastFound;		// Last server found.

	//
	// Ping methods.
	//
	void StartPingTimer();
	void StopPingTimer();

	//
	// Filter methods.
	//
	CFilter* FindFilter(const char* pszName) const;
	int      FilterIndex(CFilter* pFilter) const;
	void     ApplyFilter();
	void     BuildFilterMenu();

	//
	// Favourites methods.
	//
	void CheckFavourites();

	//
	// Config methods.
	//
	void LoadModsConfig();
	void SaveModsConfig();
	void LoadFavsConfig();
	void LoadFavourites();
	void SaveFavsConfig();

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
	void LoadAppConfig();
	void SaveAppConfig();

	void OnTimer(uint nTimerID);

	//
	// Constants.
	//
	static const char* APP_INI_FILE_VER;
	static const char* MOD_INI_FILE_VER;
	static const char* MOD_INI_FILE;
	static const char* DEF_MASTER_ADDRESS;
	static int         DEF_MASTER_PORT;
	static int         DEF_PING_THREADS;
	static int         DEF_PING_ATTEMPTS;
	static uint        DEF_PING_WAIT_TIME;
	static bool        DEF_AUTO_PING;
	static int         DEF_PING_INTERVAL;
	static const char* CACHE_FILENAME;
	static const char* CACHE_FILE_MAGIC;
	static uint16      CACHE_FILE_VER;
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
