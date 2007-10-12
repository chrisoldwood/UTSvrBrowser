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
#include <Legacy/STLUtils.hpp>

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
const char* CUTSvrBrowser::VERSION      = "v1.0 Alpha [Debug]";
#else
const char* CUTSvrBrowser::VERSION      = "v1.0 Alpha";
#endif

const char* CUTSvrBrowser::APP_INI_FILE_VER   = "1.0";
const char* CUTSvrBrowser::MOD_INI_FILE_VER  = "1.0";
const char* CUTSvrBrowser::MOD_INI_FILE      = "UTSBMods.ini";
const char* CUTSvrBrowser::DEF_MASTER_ADDRESS = "unreal.epicgames.com";
int         CUTSvrBrowser::DEF_MASTER_PORT    = 28900;
int         CUTSvrBrowser::DEF_PING_THREADS   = 10;
int         CUTSvrBrowser::DEF_PING_ATTEMPTS  = 3;
uint        CUTSvrBrowser::DEF_PING_WAIT_TIME = 1000;
bool        CUTSvrBrowser::DEF_AUTO_PING      = false;
int         CUTSvrBrowser::DEF_PING_INTERVAL  = 10;
const char* CUTSvrBrowser::CACHE_FILENAME     = "UTSvrBrowser.dat";
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
	, m_oModIniFile(CPath::ApplicationDir(), MOD_INI_FILE)
	, m_nSortCol(CAppDlg::HOST_NAME)
	, m_nSortDir(CSortColumns::ASC)
	, m_bDetectFavs(true)
	, m_oMods(m_oMDB)
	, m_oGameTypes(m_oMDB)
	, m_oServers(m_oMDB)
	, m_oFavFiles(m_oMDB)
	, m_oFavourites(m_oMDB)
	, m_oSummary(m_oMDB)
	, m_bFltEdited(false)
	, m_pFilter(NULL)
	, m_oRS(m_oServers)
	, m_strCacheFile(CPath::ApplicationDir(), CACHE_FILENAME)
	, m_nPingTimerID(0)
	, m_nLastFound(-1)
{
	// Add tables to MDB.
	m_oMDB.AddTable(m_oMods);
	m_oMDB.AddTable(m_oGameTypes);
	m_oMDB.AddTable(m_oServers);
	m_oMDB.AddTable(m_oFavFiles);
	m_oMDB.AddTable(m_oFavourites);
	m_oMDB.AddTable(m_oSummary);
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
	m_strTitle = "UT Server Browser";

	// Load settings.
	LoadAppConfig();
	
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
	if (ShowNormal() && !m_rcLastPos.Empty())
		m_AppWnd.Move(m_rcLastPos);

	m_AppWnd.Show(m_iCmdShow);

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
	catch (CStreamException& e)
	{
		AlertMsg("Warning: Failed to load backup file:\n\n%s\n\n%s", m_strCacheFile, e.what());
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
	catch (CStreamException& e)
	{
		AlertMsg("Warning: Failed to save backup file:\n\n%s\n\n%s", m_strCacheFile, e.what());
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
	CString strAppVer  = m_oAppIniFile.ReadString("Version", "Version", APP_INI_FILE_VER);
	CString strModsVer = m_oModIniFile.ReadString("Version", "Version", MOD_INI_FILE_VER);

	// Read the window pos and size.
	m_rcLastPos.left   = m_oAppIniFile.ReadInt("UI", "Left",   0);
	m_rcLastPos.top    = m_oAppIniFile.ReadInt("UI", "Top",    0);
	m_rcLastPos.right  = m_oAppIniFile.ReadInt("UI", "Right",  0);
	m_rcLastPos.bottom = m_oAppIniFile.ReadInt("UI", "Bottom", 0);

	// Read the master server query settings.
	m_oMtrQryOpts.m_strAddress   = m_oAppIniFile.ReadString("Master", "Address",   DEF_MASTER_ADDRESS);
	m_oMtrQryOpts.m_nPort        = m_oAppIniFile.ReadInt   ("Master", "Port",      DEF_MASTER_PORT   );
	m_oMtrQryOpts.m_bFirewall    = m_oAppIniFile.ReadBool  ("Master", "Firewall",  false             );
	m_oMtrQryOpts.m_nFirstPort   = m_oAppIniFile.ReadInt   ("Master", "FirstPort", 0                 );
	m_oMtrQryOpts.m_nLastPort    = m_oAppIniFile.ReadInt   ("Master", "LastPort",  65535             );
	m_oMtrQryOpts.m_bTrimSpace   = m_oAppIniFile.ReadBool  ("Master", "TrimSpace", false             );
	m_oMtrQryOpts.m_bConvertSyms = m_oAppIniFile.ReadBool  ("Master", "ConvSyms",  false             );

	// Read the sorting options.
	m_nSortCol = m_oAppIniFile.ReadInt("View", "SortCol", m_nSortCol);
	m_nSortDir = m_oAppIniFile.ReadInt("View", "SortDir", m_nSortDir);

	// Read the list of column widths.
	for (int i = 0; i < CAppDlg::NUM_COLUMNS; ++i)
	{
		CString strEntry;

		strEntry.Format("Column[%d]", i);

		m_anColWidths.push_back(m_oAppIniFile.ReadInt("View", strEntry, CAppDlg::DEF_COLUMN_WIDTH));
	}

	// Read pinging settings.
	m_oPingOpts.m_nThreads      = m_oAppIniFile.ReadInt ("Ping", "Threads",  DEF_PING_THREADS  );
	m_oPingOpts.m_nAttempts     = m_oAppIniFile.ReadInt ("Ping", "Attempts", DEF_PING_ATTEMPTS );
	m_oPingOpts.m_nWaitTime     = m_oAppIniFile.ReadInt ("Ping", "WaitTime", DEF_PING_WAIT_TIME);
	m_oPingOpts.m_bAutoPing     = m_oAppIniFile.ReadBool("Ping", "AutoPing", DEF_AUTO_PING     );
	m_oPingOpts.m_nAutoInterval = m_oAppIniFile.ReadInt ("Ping", "Interval", DEF_PING_INTERVAL );

	// Read the filters.
	int nFilters = m_oAppIniFile.ReadInt("Filters", "Count", 0);

	for (int i = 0; i < nFilters; ++i)
	{
		CString strSection, strEntry;

		strEntry.Format("Filter[%d]", i);

		// Get next filter name.
		CString strFilterName = m_oAppIniFile.ReadString("Filters", strEntry, "");

		if (strFilterName.Empty())
			continue;

		strSection.Format("%s Filter", strFilterName);

		// Read the filter definition.
		CString strName = m_oAppIniFile.ReadString(strSection, "Name", "");

		// Ignore, if invalid.
		if (strName.Empty())
			continue;

		// Create filter and read full defintion.
		CFilter* pFilter = new CFilter;

		pFilter->m_strName    = strName;
		pFilter->m_strDesc    = m_oAppIniFile.ReadString (strSection, "Description",  "");
		pFilter->m_bFltMods   = m_oAppIniFile.ReadBool   (strSection, "FilterMods",   false);
		pFilter->m_astrMods   = m_oAppIniFile.ReadStrings(strSection, "Mods", ',',    "");
		pFilter->m_bFltPing   = m_oAppIniFile.ReadBool   (strSection, "FilterPing",   false);
		pFilter->m_nPingTime  = m_oAppIniFile.ReadInt    (strSection, "PingTime",     250);
		pFilter->m_bFltErrors = m_oAppIniFile.ReadBool   (strSection, "FilterErrors", true);
		pFilter->m_bFltNames  = m_oAppIniFile.ReadBool   (strSection, "FilterNames",  false);
		pFilter->m_astrNames  = m_oAppIniFile.ReadStrings(strSection, "Names", ',',   "");

		// Add to collection.
		m_aoFilters.push_back(pFilter);
	}

	// Load the other configs.
	LoadModsConfig();
	LoadFavsConfig();
	LoadFavourites();

	// Set filter to last used.
	m_pFilter = FindFilter(m_oAppIniFile.ReadString("Filters", "Default", ""));
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
	m_oAppIniFile.WriteString("Version", "Version", APP_INI_FILE_VER);

	// Write the window pos and size.
	m_oAppIniFile.WriteInt("UI", "Left",   m_rcLastPos.left  );
	m_oAppIniFile.WriteInt("UI", "Top",    m_rcLastPos.top   );
	m_oAppIniFile.WriteInt("UI", "Right",  m_rcLastPos.right );
	m_oAppIniFile.WriteInt("UI", "Bottom", m_rcLastPos.bottom);

	if (m_oMtrQryOpts.m_bModified)
	{
		// Write the master server query settings.
		m_oAppIniFile.WriteString("Master", "Address",   m_oMtrQryOpts.m_strAddress  );
		m_oAppIniFile.WriteInt   ("Master", "Port",      m_oMtrQryOpts.m_nPort       );
		m_oAppIniFile.WriteBool  ("Master", "Firewall",  m_oMtrQryOpts.m_bFirewall   );
		m_oAppIniFile.WriteInt   ("Master", "FirstPort", m_oMtrQryOpts.m_nFirstPort  );
		m_oAppIniFile.WriteInt   ("Master", "LastPort",  m_oMtrQryOpts.m_nLastPort   );
		m_oAppIniFile.WriteBool  ("Master", "TrimSpace", m_oMtrQryOpts.m_bTrimSpace  );
		m_oAppIniFile.WriteBool  ("Master", "ConvSyms",  m_oMtrQryOpts.m_bConvertSyms);
	}

	// Write the sorting options.
	m_oAppIniFile.WriteInt("View", "SortCol", m_nSortCol);
	m_oAppIniFile.WriteInt("View", "SortDir", m_nSortDir);

	// Write the list of column widths.
	for (int i = 0; i < CAppDlg::NUM_COLUMNS; ++i)
	{
		CString strEntry;

		strEntry.Format("Column[%d]", i);

		m_oAppIniFile.WriteInt("View", strEntry, m_anColWidths[i]);
	}

	if (m_oPingOpts.m_bModified)
	{
		// Write pinging settings.
		m_oAppIniFile.WriteInt ("Ping", "Threads",  m_oPingOpts.m_nThreads     );
		m_oAppIniFile.WriteInt ("Ping", "Attempts", m_oPingOpts.m_nAttempts    );
		m_oAppIniFile.WriteInt ("Ping", "WaitTime", m_oPingOpts.m_nWaitTime    );
		m_oAppIniFile.WriteBool("Ping", "AutoPing", m_oPingOpts.m_bAutoPing    );
		m_oAppIniFile.WriteInt ("Ping", "Interval", m_oPingOpts.m_nAutoInterval);
	}

	if (m_bFltEdited)
	{
		// Write the filters.
		m_oAppIniFile.WriteInt("Filters", "Count", m_aoFilters.size());

		for (uint i = 0; i < m_aoFilters.size(); ++i)
		{
			CFilter* pFilter = m_aoFilters[i];

			CString strSection, strEntry;

			strEntry.Format("Filter[%d]", i);

			// Write the filter name.
			m_oAppIniFile.WriteString("Filters", strEntry, pFilter->m_strName);

			strSection.Format("%s Filter", pFilter->m_strName);

			// Write the filter definition.
			m_oAppIniFile.WriteString (strSection, "Name",         pFilter->m_strName   );
			m_oAppIniFile.WriteString (strSection, "Description",  pFilter->m_strDesc   );
			m_oAppIniFile.WriteBool   (strSection, "FilterMods",   pFilter->m_bFltMods  );
			m_oAppIniFile.WriteStrings(strSection, "Mods", ',',    pFilter->m_astrMods  );
			m_oAppIniFile.WriteBool   (strSection, "FilterPing",   pFilter->m_bFltPing  );
			m_oAppIniFile.WriteInt    (strSection, "PingTime",     pFilter->m_nPingTime );
			m_oAppIniFile.WriteBool   (strSection, "FilterErrors", pFilter->m_bFltErrors);
			m_oAppIniFile.WriteBool   (strSection, "FilterNames",  pFilter->m_bFltNames );
			m_oAppIniFile.WriteStrings(strSection, "Names", ',',   pFilter->m_astrNames );
		}
	}

	// Save filter last used.
	m_oAppIniFile.WriteString("Filters", "Default", (m_pFilter != NULL) ? m_pFilter->m_strName : "");

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
	int nMods = m_oModIniFile.ReadInt("Mods", "Count", 0);

	for (int i = 0; i < nMods; ++i)
	{
		CString strSection, strEntry;

		strEntry.Format("Mod[%d]", i);

		// Get next mod name.
		CString strModName = m_oModIniFile.ReadString("Mods", strEntry, "");

		if (strModName.Empty())
			continue;

		// Create the mod table entry.
		CRow& oRow = m_oMods.CreateRow();

		oRow[CMods::MOD_NAME]  = strModName;
		oRow[CMods::FAVS_FILE] = m_oModIniFile.ReadString(strModName, "FavsFile", "");

		m_oMods.InsertRow(oRow, false);

		// Read the list of game types the mod uses.
		CString strGameTypes = m_oModIniFile.ReadString(strModName, "GameTypes", "");

		if (strGameTypes.Empty())
			continue;

		CStrArray astrGameTypes;

		// Split list of game types.
		CStrTok::Split(strGameTypes, ",", astrGameTypes);

		// Create game type table entries.
		for (int j = 0; j < astrGameTypes.Size(); ++j)
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
	m_bDetectFavs = m_oAppIniFile.ReadBool("Favourites", "Detect", m_bDetectFavs);

	// Read the favourite files configuration.
	for (int i = 0; i < m_oMods.RowCount(); ++i)
	{
		CRow& oMod = m_oMods[i];

		CString strFile = m_oAppIniFile.ReadString("Favourites", oMod[CMods::MOD_NAME], "");

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
	for (int i = 0; i < m_oFavFiles.RowCount(); ++i)
	{
		CRow&    oFavFile = m_oFavFiles[i];
		CIniFile oIniFile(oFavFile[CFavFiles::MOD_FILE]);

		// Ignore if file doesn't exist.
		if (!oIniFile.m_strPath.Exists())
			continue;

		// Read number of favourites in file.
		int nFavs = oIniFile.ReadInt("UBrowser.UBrowserFavoritesFact", "FavoriteCount", 0);

		// Read all favourites.
		for (int j = 0; j < nFavs; ++j)
		{
			CString strEntry;

			strEntry.Format("Favorites[%d]", j);

			// Format: Favorites[?]=Name\Address\Port\Flag
			CStrArray astrFavInfo = oIniFile.ReadStrings("UBrowser.UBrowserFavoritesFact", strEntry, '\\', "");

			// Invalid entry?
			if (astrFavInfo.Size() != 4)
				continue;

			// Generate IP Key.
			CString strKey = astrFavInfo[1] + ":" + astrFavInfo[2];

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
		m_oAppIniFile.DeleteSection("Favourites");

		// Write the favourites config files.
		for (int i = 0; i < App.m_oFavFiles.RowCount(); ++i)
		{
			CRow& oRow = App.m_oFavFiles[i];

			m_oAppIniFile.WriteString("Favourites", oRow[CFavFiles::MOD_NAME], oRow[CFavFiles::MOD_FILE]);		
		}
	}

	// Write detection setting.
	m_oAppIniFile.WriteBool("Favourites", "Detect", m_bDetectFavs);
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
** Returns:		The filter or NULL.
**
*******************************************************************************
*/

CFilter* CUTSvrBrowser::FindFilter(const char* pszName) const
{
	// For all filters.
	for (uint i = 0; i < m_aoFilters.size(); ++i)
	{
		CFilter* pFilter = m_aoFilters[i];

		if (pFilter->m_strName == pszName)
			return pFilter;
	}

	return NULL;
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
	if (m_pFilter == NULL)
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
		for (int i = 0; i < App.m_oRS.Count(); ++i)
		{
			CRow&   oRow    = App.m_oRS[i];
			CString strName = CString(oRow[CServers::HOST_NAME]).ToLower();
			bool    bMatch  = false;

			// Matches any name in the filter?
			for (int j = 0; ((j < m_pFilter->m_astrNames.Size()) && (!bMatch)); ++j)
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
	CPopupMenu oMenu = m_AppWnd.m_Menu.Popup(FILTER_MENU_POS);

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
	for (int i = 0; i < m_oServers.RowCount(); ++i)
	{
		CRow& oSvrRow = m_oServers[i];
		CRow* pFavRow = App.m_oFavourites.SelectRow(CFavourites::IP_KEY, oSvrRow[CServers::IP_KEY]);

		// Update favourites column.
		if (pFavRow != NULL)
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
