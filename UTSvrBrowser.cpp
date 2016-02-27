/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		UTSVRBROWSER.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CUTSvrBrowser class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "UTSvrBrowser.hpp"
#include <Legacy/STLUtils.hpp>
#include <NCL/WinSock.hpp>
#include <WCL/File.hpp>
#include <WCL/StreamException.hpp>
#include <WCL/StrTok.hpp>
#include <MDBL/WhereCmp.hpp>
#include <MDBL/WhereIn.hpp>

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
const tchar* CUTSvrBrowser::VERSION = TXT("v1.0 Alpha [Debug]");
#else
const tchar* CUTSvrBrowser::VERSION = TXT("v1.0 Alpha");
#endif

const tchar* CUTSvrBrowser::APP_INI_FILE_VER  = TXT("1.0");
const tchar* CUTSvrBrowser::MOD_INI_FILE_VER  = TXT("1.0");
const tchar* CUTSvrBrowser::MOD_INI_FILE      = TXT("UTSBMods.ini");
const tchar* CUTSvrBrowser::DEF_MASTER_ADDRESS = TXT("unreal.epicgames.com");
int         CUTSvrBrowser::DEF_MASTER_PORT    = 28900;
int         CUTSvrBrowser::DEF_PING_THREADS   = 10;
int         CUTSvrBrowser::DEF_PING_ATTEMPTS  = 3;
uint        CUTSvrBrowser::DEF_PING_WAIT_TIME = 1000;
bool        CUTSvrBrowser::DEF_AUTO_PING      = false;
int         CUTSvrBrowser::DEF_PING_INTERVAL  = 10;
const tchar* CUTSvrBrowser::CACHE_FILENAME    = TXT("UTSvrBrowser.dat");
const char* CUTSvrBrowser::CACHE_FILE_MAGIC   = "UTSB";
uint16      CUTSvrBrowser::CACHE_FILE_VER     = 0x000A;

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
	, m_AppCmds(m_AppWnd)
	, m_oModIniFile(CPath::ApplicationDir(), MOD_INI_FILE)
	, m_nSortCol(CAppDlg::HOST_NAME)
	, m_nSortDir(CSortColumns::ASC)
	, m_bDetectFavs(true)
	, m_oMods()
	, m_oGameTypes()
	, m_oServers()
	, m_oFavFiles()
	, m_oFavourites()
	, m_oSummary()
	, m_bFltEdited(false)
	, m_pFilter(nullptr)
	, m_oRS(m_oServers)
	, m_strCacheFile(CPath::ApplicationDir(), CACHE_FILENAME)
	, m_nPingTimerID(0)
	, m_nLastFound(-1)
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
	// Cleanup filters.
	DeleteAll(m_aoFilters);
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
	m_strTitle = TXT("UT Server Browser");

	// Load settings.
	LoadAppConfig();
	
	// Initialise WinSock.
	int nResult = CWinSock::Startup(1, 1);

	if (nResult != 0)
	{
		FatalMsg(TXT("Failed to initialise WinSock layer: %d."), nResult);
		return false;
	}

	// Create the main window.
	if (!m_AppWnd.Create())
		return false;

	// Show it.
	if (!m_rcLastPos.Empty())
		m_AppWnd.Move(m_rcLastPos);

	m_AppWnd.Show(m_iCmdShow);

	m_AppCmds.InitialiseUI();

	// Create the inital filter menu.
	BuildFilterMenu();

	// Update UI.
	m_AppCmds.UpdateUI();
	App.m_AppWnd.UpdateTitle();

	try
	{
		// Server cache exists?
		if (m_strCacheFile.Exists())
		{
			CFile  oFile;
			uint16 nVersion;
			char   szMagic[4];

			oFile.Open(m_strCacheFile, GENERIC_READ);

			// Read file magic.
			oFile.Read(szMagic, 4);

			// Read file version.
			oFile >> nVersion;

			// Magic AND version numbers match?
			if ( (strncmp(CACHE_FILE_MAGIC, szMagic, 4) == 0)
			  && (nVersion == CACHE_FILE_VER) )
			{
				// Read servers table.
				m_oServers.Read(oFile);
			}

			oFile.Close();

			// Fix up any changes to the favourites.
			CheckFavourites();

			// Apply the current filter.
			ApplyFilter();

			// Display cached data.
			m_AppWnd.m_AppDlg.RefreshView();
		}
	}
	catch (const CStreamException& e)
	{
		AlertMsg(TXT("Warning: Failed to load backup file:\n\n%s\n\n%s"), m_strCacheFile, e.twhat());
	}

	// Start the ping timer.
	StartPingTimer();

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
	// Stop the ping timer.
	StopPingTimer();

	try
	{
		CFile  oFile;
		uint16 nVersion = CACHE_FILE_VER;

		oFile.Create(m_strCacheFile);

		// Write file magic.
		oFile.Write(CACHE_FILE_MAGIC, 4);

		// Write file version.
		oFile << nVersion;

		// Write servers table.
		m_oServers.Write(oFile);

		oFile.Close();
	}
	catch (const CStreamException& e)
	{
		AlertMsg(TXT("Warning: Failed to save backup file:\n\n%s\n\n%s"), m_strCacheFile, e.twhat());
	}

	// Terminate WinSock.
	CWinSock::Cleanup();

	// Save settings.
	SaveAppConfig();

	return true;
}

/******************************************************************************
** Method:		LoadAppConfig()
**
** Description:	Load the app configuration.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CUTSvrBrowser::LoadAppConfig()
{
	// Read the file versions.
	CString strAppVer  = m_oAppIniFile.ReadString(TXT("Version"), TXT("Version"), APP_INI_FILE_VER);
	CString strModsVer = m_oModIniFile.ReadString(TXT("Version"), TXT("Version"), MOD_INI_FILE_VER);

	// Read the window pos and size.
	m_rcLastPos.left   = m_oAppIniFile.ReadInt(TXT("UI"), TXT("Left"),   0);
	m_rcLastPos.top    = m_oAppIniFile.ReadInt(TXT("UI"), TXT("Top"),    0);
	m_rcLastPos.right  = m_oAppIniFile.ReadInt(TXT("UI"), TXT("Right"),  0);
	m_rcLastPos.bottom = m_oAppIniFile.ReadInt(TXT("UI"), TXT("Bottom"), 0);

	// Read the master server query settings.
	m_oMtrQryOpts.m_strAddress   = m_oAppIniFile.ReadString(TXT("Master"), TXT("Address"),   DEF_MASTER_ADDRESS);
	m_oMtrQryOpts.m_nPort        = m_oAppIniFile.ReadInt   (TXT("Master"), TXT("Port"),      DEF_MASTER_PORT   );
	m_oMtrQryOpts.m_bFirewall    = m_oAppIniFile.ReadBool  (TXT("Master"), TXT("Firewall"),  false             );
	m_oMtrQryOpts.m_nFirstPort   = m_oAppIniFile.ReadInt   (TXT("Master"), TXT("FirstPort"), 0                 );
	m_oMtrQryOpts.m_nLastPort    = m_oAppIniFile.ReadInt   (TXT("Master"), TXT("LastPort"),  65535             );
	m_oMtrQryOpts.m_bTrimSpace   = m_oAppIniFile.ReadBool  (TXT("Master"), TXT("TrimSpace"), false             );
	m_oMtrQryOpts.m_bConvertSyms = m_oAppIniFile.ReadBool  (TXT("Master"), TXT("ConvSyms"),  false             );

	// Read the sorting options.
	m_nSortCol = m_oAppIniFile.ReadInt(TXT("View"), TXT("SortCol"), m_nSortCol);
	m_nSortDir = m_oAppIniFile.ReadInt(TXT("View"), TXT("SortDir"), m_nSortDir);

	// Read the list of column widths.
	for (int i = 0; i < CAppDlg::NUM_COLUMNS; ++i)
	{
		CString strEntry;

		strEntry.Format(TXT("Column[%d]"), i);

		m_anColWidths.push_back(m_oAppIniFile.ReadInt(TXT("View"), strEntry, CAppDlg::DEF_COLUMN_WIDTH));
	}

	// Read pinging settings.
	m_oPingOpts.m_nThreads      = m_oAppIniFile.ReadInt (TXT("Ping"), TXT("Threads"),  DEF_PING_THREADS  );
	m_oPingOpts.m_nAttempts     = m_oAppIniFile.ReadInt (TXT("Ping"), TXT("Attempts"), DEF_PING_ATTEMPTS );
	m_oPingOpts.m_nWaitTime     = m_oAppIniFile.ReadInt (TXT("Ping"), TXT("WaitTime"), DEF_PING_WAIT_TIME);
	m_oPingOpts.m_bAutoPing     = m_oAppIniFile.ReadBool(TXT("Ping"), TXT("AutoPing"), DEF_AUTO_PING     );
	m_oPingOpts.m_nAutoInterval = m_oAppIniFile.ReadInt (TXT("Ping"), TXT("Interval"), DEF_PING_INTERVAL );

	// Read the filters.
	int nFilters = m_oAppIniFile.ReadInt(TXT("Filters"), TXT("Count"), 0);

	for (int i = 0; i < nFilters; ++i)
	{
		CString strSection, strEntry;

		strEntry.Format(TXT("Filter[%d]"), i);

		// Get next filter name.
		CString strFilterName = m_oAppIniFile.ReadString(TXT("Filters"), strEntry, TXT(""));

		if (strFilterName.Empty())
			continue;

		strSection.Format(TXT("%s Filter"), strFilterName);

		// Read the filter definition.
		CString strName = m_oAppIniFile.ReadString(strSection, TXT("Name"), TXT(""));

		// Ignore, if invalid.
		if (strName.Empty())
			continue;

		// Create filter and read full defintion.
		CFilter* pFilter = new CFilter;

		pFilter->m_strName    = strName;
		pFilter->m_strDesc    = m_oAppIniFile.ReadString (strSection, TXT("Description"),  TXT(""));
		pFilter->m_bFltMods   = m_oAppIniFile.ReadBool   (strSection, TXT("FilterMods"),   false);
		pFilter->m_astrMods   = m_oAppIniFile.ReadStrings(strSection, TXT("Mods"), ',',    TXT(""));
		pFilter->m_bFltPing   = m_oAppIniFile.ReadBool   (strSection, TXT("FilterPing"),   false);
		pFilter->m_nPingTime  = m_oAppIniFile.ReadInt    (strSection, TXT("PingTime"),     250);
		pFilter->m_bFltErrors = m_oAppIniFile.ReadBool   (strSection, TXT("FilterErrors"), true);
		pFilter->m_bFltNames  = m_oAppIniFile.ReadBool   (strSection, TXT("FilterNames"),  false);
		pFilter->m_astrNames  = m_oAppIniFile.ReadStrings(strSection, TXT("Names"), ',',   TXT(""));

		// Add to collection.
		m_aoFilters.push_back(pFilter);
	}

	// Load the other configs.
	LoadModsConfig();
	LoadFavsConfig();
	LoadFavourites();

	// Set filter to last used.
	m_pFilter = FindFilter(m_oAppIniFile.ReadString(TXT("Filters"), TXT("Default"), TXT("")));
}

/******************************************************************************
** Method:		SaveAppConfig()
**
** Description:	Save the app configuration.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CUTSvrBrowser::SaveAppConfig()
{
	// Write the file version.
	m_oAppIniFile.WriteString(TXT("Version"), TXT("Version"), APP_INI_FILE_VER);

	// Write the window pos and size.
	m_oAppIniFile.WriteInt(TXT("UI"), TXT("Left"),   m_rcLastPos.left  );
	m_oAppIniFile.WriteInt(TXT("UI"), TXT("Top"),    m_rcLastPos.top   );
	m_oAppIniFile.WriteInt(TXT("UI"), TXT("Right"),  m_rcLastPos.right );
	m_oAppIniFile.WriteInt(TXT("UI"), TXT("Bottom"), m_rcLastPos.bottom);

	if (m_oMtrQryOpts.m_bModified)
	{
		// Write the master server query settings.
		m_oAppIniFile.WriteString(TXT("Master"), TXT("Address"),   m_oMtrQryOpts.m_strAddress  );
		m_oAppIniFile.WriteInt   (TXT("Master"), TXT("Port"),      m_oMtrQryOpts.m_nPort       );
		m_oAppIniFile.WriteBool  (TXT("Master"), TXT("Firewall"),  m_oMtrQryOpts.m_bFirewall   );
		m_oAppIniFile.WriteInt   (TXT("Master"), TXT("FirstPort"), m_oMtrQryOpts.m_nFirstPort  );
		m_oAppIniFile.WriteInt   (TXT("Master"), TXT("LastPort"),  m_oMtrQryOpts.m_nLastPort   );
		m_oAppIniFile.WriteBool  (TXT("Master"), TXT("TrimSpace"), m_oMtrQryOpts.m_bTrimSpace  );
		m_oAppIniFile.WriteBool  (TXT("Master"), TXT("ConvSyms"),  m_oMtrQryOpts.m_bConvertSyms);
	}

	// Write the sorting options.
	m_oAppIniFile.WriteInt(TXT("View"), TXT("SortCol"), m_nSortCol);
	m_oAppIniFile.WriteInt(TXT("View"), TXT("SortDir"), m_nSortDir);

	// Write the list of column widths.
	for (int i = 0; i < CAppDlg::NUM_COLUMNS; ++i)
	{
		CString strEntry;

		strEntry.Format(TXT("Column[%d]"), i);

		m_oAppIniFile.WriteInt(TXT("View"), strEntry, m_anColWidths[i]);
	}

	if (m_oPingOpts.m_bModified)
	{
		// Write pinging settings.
		m_oAppIniFile.WriteInt (TXT("Ping"), TXT("Threads"),  m_oPingOpts.m_nThreads     );
		m_oAppIniFile.WriteInt (TXT("Ping"), TXT("Attempts"), m_oPingOpts.m_nAttempts    );
		m_oAppIniFile.WriteInt (TXT("Ping"), TXT("WaitTime"), m_oPingOpts.m_nWaitTime    );
		m_oAppIniFile.WriteBool(TXT("Ping"), TXT("AutoPing"), m_oPingOpts.m_bAutoPing    );
		m_oAppIniFile.WriteInt (TXT("Ping"), TXT("Interval"), m_oPingOpts.m_nAutoInterval);
	}

	if (m_bFltEdited)
	{
		// Write the filters.
		m_oAppIniFile.WriteInt(TXT("Filters"), TXT("Count"), m_aoFilters.size());

		for (uint i = 0; i < m_aoFilters.size(); ++i)
		{
			CFilter* pFilter = m_aoFilters[i];

			CString strSection, strEntry;

			strEntry.Format(TXT("Filter[%d]"), i);

			// Write the filter name.
			m_oAppIniFile.WriteString(TXT("Filters"), strEntry, pFilter->m_strName);

			strSection.Format(TXT("%s Filter"), pFilter->m_strName);

			// Write the filter definition.
			m_oAppIniFile.WriteString (strSection, TXT("Name"),         pFilter->m_strName   );
			m_oAppIniFile.WriteString (strSection, TXT("Description"),  pFilter->m_strDesc   );
			m_oAppIniFile.WriteBool   (strSection, TXT("FilterMods"),   pFilter->m_bFltMods  );
			m_oAppIniFile.WriteStrings(strSection, TXT("Mods"), ',',    pFilter->m_astrMods  );
			m_oAppIniFile.WriteBool   (strSection, TXT("FilterPing"),   pFilter->m_bFltPing  );
			m_oAppIniFile.WriteInt    (strSection, TXT("PingTime"),     pFilter->m_nPingTime );
			m_oAppIniFile.WriteBool   (strSection, TXT("FilterErrors"), pFilter->m_bFltErrors);
			m_oAppIniFile.WriteBool   (strSection, TXT("FilterNames"),  pFilter->m_bFltNames );
			m_oAppIniFile.WriteStrings(strSection, TXT("Names"), ',',   pFilter->m_astrNames );
		}
	}

	// Save filter last used.
	m_oAppIniFile.WriteString(TXT("Filters"), TXT("Default"), (m_pFilter != nullptr) ? m_pFilter->m_strName : TXT(""));

	// Ave the other configs.
//	SaveModsConfig();
	SaveFavsConfig();
}

/******************************************************************************
** Method:		LoadModsConfig()
**
** Description:	Load the mods configuration.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CUTSvrBrowser::LoadModsConfig()
{
	// Read the mods configuration.
	int nMods = m_oModIniFile.ReadInt(TXT("Mods"), TXT("Count"), 0);

	for (int i = 0; i < nMods; ++i)
	{
		CString strSection, strEntry;

		strEntry.Format(TXT("Mod[%d]"), i);

		// Get next mod name.
		CString strModName = m_oModIniFile.ReadString(TXT("Mods"), strEntry, TXT(""));

		if (strModName.Empty())
			continue;

		// Create the mod table entry.
		CRow& oRow = m_oMods.CreateRow();

		oRow[CMods::MOD_NAME]  = strModName;
		oRow[CMods::FAVS_FILE] = m_oModIniFile.ReadString(strModName, TXT("FavsFile"), TXT(""));

		m_oMods.InsertRow(oRow, false);

		// Read the list of game types the mod uses.
		CString strGameTypes = m_oModIniFile.ReadString(strModName, TXT("GameTypes"), TXT(""));

		if (strGameTypes.Empty())
			continue;

		CStrArray astrGameTypes;

		// Split list of game types.
		CStrTok::Split(strGameTypes, TXT(","), astrGameTypes);

		// Create game type table entries.
		for (size_t j = 0; j < astrGameTypes.Size(); ++j)
		{
			CRow& oRow = m_oGameTypes.CreateRow();

			oRow[CGameTypes::GAME_TYPE] = astrGameTypes[j];
			oRow[CGameTypes::MOD_NAME]  = strModName;

			m_oGameTypes.InsertRow(oRow, false);
		}
	}
}

/******************************************************************************
** Method:		SaveModsConfig()
**
** Description:	Save the mods configuration.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CUTSvrBrowser::SaveModsConfig()
{
}

/******************************************************************************
** Method:		LoadFavsConfig()
**
** Description:	Load the favourites configuration.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CUTSvrBrowser::LoadFavsConfig()
{
	// Read detection setting.
	m_bDetectFavs = m_oAppIniFile.ReadBool(TXT("Favourites"), TXT("Detect"), m_bDetectFavs);

	// Read the favourite files configuration.
	for (size_t i = 0; i < m_oMods.RowCount(); ++i)
	{
		CRow& oMod = m_oMods[i];

		CString strFile = m_oAppIniFile.ReadString(TXT("Favourites"), oMod[CMods::MOD_NAME], TXT(""));

		// Ignore if not set.
		if (strFile.Empty())
			continue;

		// Add to favourite config files table.
		CRow& oRow = m_oFavFiles.CreateRow();

		oRow[CFavFiles::MOD_NAME] = oMod[CMods::MOD_NAME];
		oRow[CFavFiles::MOD_FILE] = strFile;

		m_oFavFiles.InsertRow(oRow, false);
	}
}

/******************************************************************************
** Method:		LoadFavsConfig()
**
** Description:	Load the favourites configuration.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CUTSvrBrowser::LoadFavourites()
{
	// Delete old entries.
	m_oFavourites.Truncate();

	// Read the favourites from the config files.
	for (size_t i = 0; i < m_oFavFiles.RowCount(); ++i)
	{
		CRow&    oFavFile = m_oFavFiles[i];
		CIniFile oIniFile(oFavFile[CFavFiles::MOD_FILE]);

		// Ignore if file doesn't exist.
		if (!oIniFile.m_strPath.Exists())
			continue;

		// Read number of favourites in file.
		int nFavs = oIniFile.ReadInt(TXT("UBrowser.UBrowserFavoritesFact"), TXT("FavoriteCount"), 0);

		// Read all favourites.
		for (int j = 0; j < nFavs; ++j)
		{
			CString strEntry;

			strEntry.Format(TXT("Favorites[%d]"), j);

			// Format: Favorites[?]=Name\Address\Port\Flag
			CStrArray astrFavInfo = oIniFile.ReadStrings(TXT("UBrowser.UBrowserFavoritesFact"), strEntry, '\\', TXT(""));

			// Invalid entry?
			if (astrFavInfo.Size() != 4)
				continue;

			// Generate IP Key.
			CString strKey = astrFavInfo[1] + TXT(":") + astrFavInfo[2];

			// Add to favourites table.
			CRow& oRow = m_oFavourites.CreateRow();

			oRow[CFavourites::IP_KEY]   = strKey;
			oRow[CFavourites::MOD_NAME] = oFavFile[CFavFiles::MOD_NAME];
			oRow[CFavourites::FAV_ID]   = j;

			m_oFavourites.InsertRow(oRow, false);
		}
	}
}

/******************************************************************************
** Method:		SaveFavsConfig()
**
** Description:	Save the favourites configuration.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CUTSvrBrowser::SaveFavsConfig()
{
	if (App.m_oFavFiles.Modified())
	{
		// Delete entire secetion.
		m_oAppIniFile.DeleteSection(TXT("Favourites"));

		// Write the favourites config files.
		for (size_t i = 0; i < App.m_oFavFiles.RowCount(); ++i)
		{
			CRow& oRow = App.m_oFavFiles[i];

			m_oAppIniFile.WriteString(TXT("Favourites"), oRow[CFavFiles::MOD_NAME], oRow[CFavFiles::MOD_FILE]);		
		}
	}

	// Write detection setting.
	m_oAppIniFile.WriteBool(TXT("Favourites"), TXT("Detect"), m_bDetectFavs);
}

/******************************************************************************
** Method:		StartPingTimer()
**
** Description:	Start the background ping timer.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CUTSvrBrowser::StartPingTimer()
{
	if (m_nPingTimerID == 0)
		m_nPingTimerID = StartTimer(m_oPingOpts.m_nAutoInterval * 1000);
}

/******************************************************************************
** Method:		StopPingTimer()
**
** Description:	Stop the background ping timer.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CUTSvrBrowser::StopPingTimer()
{
	if (m_nPingTimerID != 0)
		StopTimer(m_nPingTimerID);

	m_nPingTimerID = 0;
}

/******************************************************************************
** Method:		FindFilter()
**
** Description:	Finds a filter by its name.
**
** Parameters:	pszName		The filters name.
**
** Returns:		The filter or nullptr.
**
*******************************************************************************
*/

CFilter* CUTSvrBrowser::FindFilter(const tchar* pszName) const
{
	// For all filters.
	for (uint i = 0; i < m_aoFilters.size(); ++i)
	{
		CFilter* pFilter = m_aoFilters[i];

		if (pFilter->m_strName == pszName)
			return pFilter;
	}

	return nullptr;
}

/******************************************************************************
** Method:		FilterIndex()
**
** Description:	Gets the index of the given filter.
**
** Parameters:	pFilter		The filter.
**
** Returns:		The index or -1.
**
*******************************************************************************
*/

int CUTSvrBrowser::FilterIndex(CFilter* pFilter) const
{
	// For all filters.
	for (uint i = 0; i < m_aoFilters.size(); ++i)
	{
		if (m_aoFilters[i] == pFilter)
			return i;
	}

	return -1;
}

/******************************************************************************
** Method:		ApplyFilter()
**
** Description:	Applies the current filter to the server list.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CUTSvrBrowser::ApplyFilter()
{
	// Start with all servers.
	App.m_oRS = App.m_oServers.SelectAll();

	// No filter in use?
	if (m_pFilter == nullptr)
		return;

	// Filter by ping times?
	if (m_pFilter->m_bFltPing)
		App.m_oRS = App.m_oRS.Select(CWhereCmp(CServers::PING_TIME, CWhereCmp::LESS, m_pFilter->m_nPingTime));

	// Filter errors?
	if (m_pFilter->m_bFltErrors)
		App.m_oRS = App.m_oRS.Select(CWhereCmp(CServers::LAST_ERROR, CWhereCmp::EQUALS, CServers::ERROR_NONE));

	// Filter mods?
	if (m_pFilter->m_bFltMods)
		App.m_oRS = App.m_oRS.Select(CWhereIn(CServers::MOD_NAME, m_pFilter->m_astrMods));

	// Filter server names?
	if (m_pFilter->m_bFltNames)
	{
		CResultSet oRS(App.m_oServers);

		// For all rows...
		for (size_t i = 0; i < App.m_oRS.Count(); ++i)
		{
			CRow&   oRow    = App.m_oRS[i];
			CString strName = CString(oRow[CServers::HOST_NAME]).ToLower();
			bool    bMatch  = false;

			// Matches any name in the filter?
			for (size_t j = 0; ((j < m_pFilter->m_astrNames.Size()) && (!bMatch)); ++j)
			{
				if (strName.Find(m_pFilter->m_astrNames[j]) != -1)
					bMatch = true;
			}

			// Matches name list?
			if (bMatch)
				oRS.Add(oRow);
		}

		App.m_oRS = oRS;
	}
}

/******************************************************************************
** Method:		BuildFilterMenu()
**
** Description:	Build the filters main menu.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CUTSvrBrowser::BuildFilterMenu()
{
	// Get the filters popup menu.
	CPopupMenu oMenu = m_AppWnd.m_Menu.GetItemPopup(FILTER_MENU_POS);

	// Delete the old filter entries.
	while (oMenu.ItemCount() > FILTER_MENU_FIXED_ITEMS)
		oMenu.RemoveItem(FIRST_FILTER_CMD_POS);

	// Add all new filters.
	for (uint i = 0; i < m_aoFilters.size(); ++i)
		oMenu.AppendCmd(ID_FIRST_FILTER_CMD+i, m_aoFilters[i]->m_strName);
}

/******************************************************************************
** Method:		CheckFavourites()
**
** Description:	Processes the main server list to ensure the favourites are
**				marked correctly.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CUTSvrBrowser::CheckFavourites()
{
	// For all servers.
	for (size_t i = 0; i < m_oServers.RowCount(); ++i)
	{
		CRow& oSvrRow = m_oServers[i];
		CRow* pFavRow = App.m_oFavourites.SelectRow(CFavourites::IP_KEY, oSvrRow[CServers::IP_KEY].ToValue());

		// Update favourites column.
		if (pFavRow != nullptr)
			oSvrRow[CServers::FAV_ID] = pFavRow->Field(CFavourites::FAV_ID);
		else
			oSvrRow[CServers::FAV_ID] = null;
	}
}

/******************************************************************************
** Method:		OnTimer()
**
** Description:	The background ping timer has fired.
**
** Parameters:	nTimerID	The timer ID.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CUTSvrBrowser::OnTimer(uint /*nTimerID*/)
{
	// If enabled, ping the selected server.
	if (m_oPingOpts.m_bAutoPing)
		m_AppCmds.OnServersPing();
}
