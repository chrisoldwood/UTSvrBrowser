/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPCMDS.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CAppCmds class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "ProgressDlg.hpp"
#include "FindDlg.hpp"
#include "SummaryDlg.hpp"
#include "GameInfoDlg.hpp"
#include "MasterQueryDlg.hpp"
#include "PingOptsDlg.hpp"
#include "FiltersDlg.hpp"
#include "FavouritesDlg.hpp"
#include "AboutDlg.hpp"

/******************************************************************************
** Method:		Constructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CAppCmds::CAppCmds()
{
	// Define the command table.
	DEFINE_CMD_TABLE
		// Servers menu.
		CMD_ENTRY(ID_SERVERS_PING,				OnServersPing,			NULL,						-1)
		CMD_ENTRY(ID_SERVERS_PING_FILTERED,		OnServersPingFiltered,	OnUIServersPingFiltered,	-1)
		CMD_ENTRY(ID_SERVERS_PING_ALL,			OnServersPingAll,		NULL,						-1)
		CMD_ENTRY(ID_SERVERS_REFRESH,			OnServersRefresh,		NULL,						-1)
		CMD_ENTRY(ID_SERVERS_FIND,				OnServersFind,			NULL,						-1)
		CMD_ENTRY(ID_SERVERS_FIND_NEXT,			OnServersFindNext,		OnUIServersFindNext,		-1)
		CMD_ENTRY(ID_SERVERS_ADD_FAV,			OnServersAddFav,		OnUIServersAddFav,			-1)
		CMD_ENTRY(ID_SERVERS_DEL_FAV,			OnServersDelFav,		OnUIServersDelFav,			-1)
		CMD_ENTRY(ID_SERVERS_SUMMARY,			OnServersSummary,		NULL,						-1)
		CMD_ENTRY(ID_SERVERS_EXIT,				OnServersExit,			NULL,						-1)
		// Filter menu.
		CMD_ENTRY(ID_FILTER_NONE,				OnFilterNone,			OnUIFilter,					-1)
		CMD_RANGE(ID_FIRST_FILTER_CMD,
				  ID_LAST_FILTER_CMD,			OnFilter,				NULL,						-1)
		// View menu.
		CMD_ENTRY(ID_VIEW_COLUMN_NAME,			OnViewColumnName,		NULL,						-1)
		CMD_ENTRY(ID_VIEW_COLUMN_MOD,			OnViewColumnMod,		NULL,						-1)
		CMD_ENTRY(ID_VIEW_COLUMN_MAP,			OnViewColumnMap,		NULL,						-1)
		CMD_ENTRY(ID_VIEW_COLUMN_PLAYERS,		OnViewColumnPlayers,	NULL,						-1)
		CMD_ENTRY(ID_VIEW_COLUMN_PING,			OnViewColumnPing,		NULL,						-1)
		CMD_ENTRY(ID_VIEW_COLUMN_IP_ADDRESS,	OnViewColumnIPAddress,	NULL,						-1)
		CMD_ENTRY(ID_VIEW_COLUMN_IP_PORT,		OnViewColumnIPPort,		NULL,						-1)
		CMD_ENTRY(ID_VIEW_COLUMN_AUTO_SIZE,		OnViewColumnAutoSize,	NULL,						-1)
		CMD_ENTRY(ID_VIEW_SORT_NAME,			OnViewSortByName,		OnUIViewSortByName,			-1)
		CMD_ENTRY(ID_VIEW_SORT_MOD,				OnViewSortByMod,		OnUIViewSortByMod,			-1)
		CMD_ENTRY(ID_VIEW_SORT_MAP,				OnViewSortByMap,		OnUIViewSortByMap,			-1)
		CMD_ENTRY(ID_VIEW_SORT_PLAYERS,			OnViewSortByPlayers,	OnUIViewSortByPlayers,		-1)
		CMD_ENTRY(ID_VIEW_SORT_PING,			OnViewSortByPing,		OnUIViewSortByPing,			-1)
		CMD_ENTRY(ID_VIEW_SORT_IP_ADDRESS,		OnViewSortByIPAddress,	OnUIViewSortByIPAddress,	-1)
		CMD_ENTRY(ID_VIEW_SORT_IP_PORT,			OnViewSortByIPPort,		OnUIViewSortByIPPort,		-1)
		CMD_ENTRY(ID_VIEW_DETAILS,				OnViewDetails,			OnUIViewDetails,			-1)
		// Options menu.
		CMD_ENTRY(ID_OPTIONS_MASTER,			OnOptionsMaster,		NULL,						-1)
		CMD_ENTRY(ID_OPTIONS_PING,				OnOptionsPing,			NULL,						-1)
		CMD_ENTRY(ID_OPTIONS_FILTERS,			OnOptionsFilters,		NULL,						-1)
		CMD_ENTRY(ID_OPTIONS_FAVOURITES,		OnOptionsFavourites,	NULL,						-1)
		// Help menu.
		CMD_ENTRY(ID_HELP_ABOUT,				OnHelpAbout,			NULL,						10)
	END_CMD_TABLE
}

/******************************************************************************
** Method:		Destructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CAppCmds::~CAppCmds()
{
}

/******************************************************************************
** Method:		OnServersPing()
**
** Description:	Ping the selected server.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnServersPing()
{
	CBusyCursor oCursor;

	// Get selected server, if one.
	CRow* pRow = App.m_AppWnd.m_AppDlg.SelectedRow();

	if (pRow == NULL)
		return;

	// Ping server.
	CQueryJob oJob(*pRow);

	oJob.Run();

	// Update UI.
	App.m_AppWnd.m_AppDlg.RefreshRow(*pRow);
}

/******************************************************************************
** Method:		OnServersPingFiltered()
**
** Description:	Ping all the servers in the filter.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnServersPingFiltered()
{
	// Show the progress dialog.
	CProgressDlg Dlg;

	Dlg.RunModeless(App.m_AppWnd);
	Dlg.Title("Pinging Filtered Servers");
	Dlg.UpdateLabel("Pinging servers...");
	App.m_AppWnd.Enable(false);

		// Create a thread pool.
	CThreadPool	oThreadPool(App.m_oPingOpts.m_nThreads);

	oThreadPool.Start();

	Dlg.InitMeter(App.m_oRS.Count());

	// Start jobs to check all computers.
	for (int i = 0; i < App.m_oRS.Count(); ++i)
		oThreadPool.AddJob(new CQueryJob(App.m_oRS[i]));

	// Wait for jobs to complete OR user to cancel.
	while (oThreadPool.CompletedJobCount() < App.m_oRS.Count())
	{
		Dlg.UpdateMeter(oThreadPool.CompletedJobCount());

		// Check for "Cancel" button.
		App.m_MainThread.ProcessMsgQueue();

		if (Dlg.Abort())
		{
			Dlg.UpdateLabel("Cancelling...");
			oThreadPool.CancelAllJobs();
		}

		Sleep(1000);
	}

	// Stop the thread pool.
	oThreadPool.DeleteCompletedJobs();
	oThreadPool.Stop();

	// Remove progress dialog.
	App.m_AppWnd.Enable(true);
	Dlg.Destroy();

	// Update UI.
	App.m_AppWnd.m_AppDlg.RefreshView();
}

/******************************************************************************
** Method:		OnServersPingAll()
**
** Description:	Ping all the servers.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnServersPingAll()
{
	// Show the progress dialog.
	CProgressDlg Dlg;

	Dlg.RunModeless(App.m_AppWnd);
	Dlg.Title("Pinging All Servers");
	Dlg.UpdateLabel("Pinging servers...");
	App.m_AppWnd.Enable(false);

		// Create a thread pool.
	CThreadPool	oThreadPool(App.m_oPingOpts.m_nThreads);

	oThreadPool.Start();

	Dlg.InitMeter(App.m_oServers.RowCount());

	// Start jobs to check all computers.
	for (int i = 0; i < App.m_oServers.RowCount(); ++i)
		oThreadPool.AddJob(new CQueryJob(App.m_oServers[i]));

	// Wait for jobs to complete OR user to cancel.
	while (oThreadPool.CompletedJobCount() < App.m_oServers.RowCount())
	{
		Dlg.UpdateMeter(oThreadPool.CompletedJobCount());

		// Check for "Cancel" button.
		App.m_MainThread.ProcessMsgQueue();

		if (Dlg.Abort())
		{
			Dlg.UpdateLabel("Cancelling...");
			oThreadPool.CancelAllJobs();
		}

		Sleep(1000);
	}

	// Stop the thread pool.
	oThreadPool.DeleteCompletedJobs();
	oThreadPool.Stop();

	// Remove progress dialog.
	App.m_AppWnd.Enable(true);
	Dlg.Destroy();

	// Update UI.
	App.m_AppWnd.m_AppDlg.RefreshView();
}

/******************************************************************************
** Method:		OnServersRefresh()
**
** Description:	Refresh the master list of servers.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnServersRefresh()
{
	CBusyCursor oCursor;

	// Show the progress dialog.
	CProgressDlg Dlg;

	Dlg.RunModeless(App.m_AppWnd);
	Dlg.Title("Finding Servers");
	App.m_AppWnd.Enable(false);

	Dlg.UpdateLabel("Querying " + App.m_oMtrQryOpts.m_strAddress + "...");

	CMasterServer oMaster(App.m_oMtrQryOpts.m_strAddress, App.m_oMtrQryOpts.m_nPort);
	CStrArray     astrAddresses;

	try
	{
		// Fetch master list.
		if (oMaster.QueryServers("ut", astrAddresses) > 0)
		{
			// Trash old servers table.
			App.m_oServers.Truncate();

			// Add servers to the table.
			for (int i = 0; i < astrAddresses.Size(); ++i)
			{
				CString strKey = astrAddresses[i];

				// Ignore duplicates.
				if (App.m_oServers.SelectRow(CServers::IP_KEY, strKey) != NULL)
					continue;

				// Split key into IP Address & Port.
				int     nLength = strKey.Length();
				int     nColon  = strKey.Find(':');
				CString strAddr = strKey.Left(nColon);
				CString strPort = strKey.Right(nLength-nColon-1);
				int     nPort   = atoi(strPort);
				CRow*   pFavRow = App.m_oFavourites.SelectRow(CFavourites::IP_KEY, strKey);

				// Ignore if blocked by firewall.
				if ( (App.m_oMtrQryOpts.m_bFirewall)
				  && ((nPort < App.m_oMtrQryOpts.m_nFirstPort)
				   || (nPort > App.m_oMtrQryOpts.m_nLastPort)) )
					continue;

				// Add server to table.
				CRow& oRow = App.m_oServers.CreateRow();

				oRow[CServers::IP_KEY]      = strKey;
				oRow[CServers::IP_ADDRESS]  = strAddr;
				oRow[CServers::IP_PORT]     = nPort;
				oRow[CServers::LAST_ERROR]  = CServers::ERROR_FAILED;

				if (pFavRow != NULL)
					oRow[CServers::FAV_ID] = pFavRow->Field(CFavourites::FAV_ID);

				App.m_oServers.InsertRow(oRow);
			}
		}

		// Create a thread pool.
		CThreadPool	oThreadPool(App.m_oPingOpts.m_nThreads);

		oThreadPool.Start();

		Dlg.InitMeter(App.m_oServers.RowCount());
		Dlg.UpdateLabel("Pinging servers...");

		// Start jobs to check all computers.
		for (int i = 0; i < App.m_oServers.RowCount(); ++i)
			oThreadPool.AddJob(new CQueryJob(App.m_oServers[i]));

		// Wait for jobs to complete OR user to cancel.
		while (oThreadPool.CompletedJobCount() < App.m_oServers.RowCount())
		{
			Dlg.UpdateMeter(oThreadPool.CompletedJobCount());

			// Check for "Cancel" button.
			App.m_MainThread.ProcessMsgQueue();

			if (Dlg.Abort())
			{
				Dlg.UpdateLabel("Cancelling...");
				oThreadPool.CancelAllJobs();
			}

			Sleep(1000);
		}

		// Stop the thread pool.
		oThreadPool.DeleteCompletedJobs();
		oThreadPool.Stop();
	}
	catch (CSocketException& e)
	{
		App.AlertMsg("Failed to query master server:\n\n%s", e.ErrorText());
	}

	// Remove progress dialog.
	App.m_AppWnd.Enable(true);
	Dlg.Destroy();

	// Apply the current filter.
	App.ApplyFilter();
	
	// Update UI.
	App.m_AppWnd.m_AppDlg.RefreshView();
}

/******************************************************************************
** Method:		OnServersFind()
**
** Description:	Find the first server name that matches the string.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnServersFind()
{
	CFindDlg Dlg;

	Dlg.m_strText = App.m_strFindText;

	// Query user for server name string.
	if (Dlg.RunModal(App.m_AppWnd) == IDOK)
	{
		App.m_strFindText = Dlg.m_strText.ToLower();
		App.m_nLastFound  = -1;

		// Find first server.
		OnServersFindNext();
	}
}

/******************************************************************************
** Method:		OnServersFindNext()
**
** Description:	Find the next server name that matches the string.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnServersFindNext()
{
	// Search from last row found...
	for (int i = App.m_nLastFound+1; i < App.m_oRS.Count(); ++i)
	{
		CString strHostName = App.m_oRS[i][CServers::HOST_NAME];

		strHostName.ToLower();

		// Match?
		if (strHostName.Find(App.m_strFindText) != -1)
		{
			// Make it the selection.
			App.m_nLastFound = i;
			App.m_AppWnd.m_AppDlg.SelectRow(i);
			return;
		}
	}

	// Found anything last time?
	if (App.m_nLastFound != -1)
	{
		// Wrap search.
		App.m_nLastFound = -1;
		OnServersFindNext();
	}
}

/******************************************************************************
** Method:		OnServersAddFav()
**
** Description:	Add the selected server to the mods favourites.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnServersAddFav()
{
	CRow* pRow = App.m_AppWnd.m_AppDlg.SelectedRow();

	ASSERT(pRow != NULL);
	ASSERT(pRow->Field(CServers::FAV_ID) == null);

	// Find the .ini file for the mod.
	CString strMod   = pRow->Field(CServers::MOD_NAME);
	CRow*   pFavFile = App.m_oFavFiles.SelectRow(CFavFiles::MOD_NAME, strMod);

	// Not configured?
	if (pFavFile == NULL)
	{
		App.m_AppWnd.AlertMsg("There is no favourites file configured for the %s mod.", strMod);
		return;
	}

	// Extract the config file path.
	CPath    strIniFile = pFavFile->Field(CFavFiles::MOD_FILE);
	CIniFile oIniFile(strIniFile);

	// Find the next free favourites slot.
	int nEntry = oIniFile.ReadInt("UBrowser.UBrowserFavoritesFact", "FavoriteCount", -1);

	if (nEntry == -1)
	{
		App.m_AppWnd.AlertMsg("The %s favourites file is missing or corrupt.\n\n%s", strMod, strIniFile);
		return;
	}

	// Extract the servers details.
	CString strName    = pRow->Field(CServers::HOST_NAME);
	CString strAddress = pRow->Field(CServers::IP_ADDRESS);
	CString strPort    = pRow->Field(CServers::IP_PORT).Format();

	CString strEntry, strValue;

	// Format the favourite entry.
	strEntry.Format("Favorites[%d]", nEntry);
	strValue.Format("%s\\%s\\%s\\False", strName, strAddress, strPort);

	// Write it and update the count.
	oIniFile.WriteString("UBrowser.UBrowserFavoritesFact", strEntry,        strValue);
	oIniFile.WriteInt   ("UBrowser.UBrowserFavoritesFact", "FavoriteCount", nEntry+1);

	// Update the server entry.
	pRow->Field(CServers::FAV_ID) = nEntry;

	// Add to favourites table.
	CRow& oRow = App.m_oFavourites.CreateRow();

	oRow[CFavourites::IP_KEY]   = pRow->Field(CServers::IP_KEY);
	oRow[CFavourites::MOD_NAME] = pRow->Field(CServers::MOD_NAME);
	oRow[CFavourites::FAV_ID]   = nEntry;

	App.m_oFavourites.InsertRow(oRow);

	// Update UI.
	App.m_AppWnd.m_AppDlg.RefreshRow(*pRow);
	UpdateUI();
}

/******************************************************************************
** Method:		OnServersDelFav()
**
** Description:	Remove the selected server from the mods favourites.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnServersDelFav()
{
	CRow* pRow = App.m_AppWnd.m_AppDlg.SelectedRow();

	ASSERT(pRow != NULL);
	ASSERT(pRow->Field(CServers::FAV_ID) != null);

	// Find the .ini file for the mod.
	CString strMod   = pRow->Field(CServers::MOD_NAME);
	CRow*   pFavFile = App.m_oFavFiles.SelectRow(CFavFiles::MOD_NAME, strMod);

	// Not configured?
	if (pFavFile == NULL)
	{
		App.m_AppWnd.AlertMsg("There is no favourites file configured for the %s mod.", strMod);
		return;
	}

	// Extract the config file path.
	CPath    strIniFile = pFavFile->Field(CFavFiles::MOD_FILE);
	CIniFile oIniFile(strIniFile);

	int     nEntry = pRow->Field(CServers::FAV_ID);
	CString strEntry, strValue;

	strEntry.Format("Favorites[%d]", nEntry);

	// Write an empty entry.
	oIniFile.WriteString("UBrowser.UBrowserFavoritesFact", strEntry, strValue);

	// Update the server entry.
	pRow->Field(CServers::FAV_ID) = null;

	// Remove from favourites table.
	CRow* pFavRow = App.m_oFavourites.SelectRow(CFavourites::IP_KEY, pRow->Field(CServers::IP_KEY));

	App.m_oFavourites.DeleteRow(*pFavRow);

	// Update UI.
	App.m_AppWnd.m_AppDlg.RefreshRow(*pRow);
	UpdateUI();
}

/******************************************************************************
** Method:		OnServersSummary()
**
** Description:	Show the servers summary dialog.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnServersSummary()
{
	// Trash old summary.
	App.m_oSummary.Truncate();

	// Get all servers that responded.
	CResultSet oServersRS = App.m_oServers.Select(CWhereCmp(CServers::LAST_ERROR, CWhereCmp::EQUALS, CServers::ERROR_NONE));

	// Group servers by MOD.
	CGroupSet oGS = oServersRS.GroupBy(CServers::MOD_NAME);

	// Generate summary table.
	for (int i = 0; i < oGS.Count(); ++i)
	{
		CResultSet& oRS = oGS[i];

		// Create MOD summary.
		CRow& oRow = App.m_oSummary.CreateRow();

		oRow[CSummary::MOD_NAME]    = oRS[0][CServers::MOD_NAME];
		oRow[CSummary::NUM_SERVERS] = oRS.Count();
		oRow[CSummary::NUM_PLAYERS] = oRS.Sum(CServers::NUM_PLAYERS).m_iValue;

		App.m_oSummary.InsertRow(oRow);
	}

	CSummaryDlg Dlg;

	Dlg.RunModal(App.m_rMainWnd);
}

/******************************************************************************
** Method:		OnServersExit()
**
** Description:	Terminate the app.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnServersExit()
{
	App.m_AppWnd.Close();
}

/******************************************************************************
** Method:		OnFilterNone()
**
** Description:	Remove any filter.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnFilterNone()
{
	App.m_pFilter = NULL;

	// Apply the new filter.
	App.ApplyFilter();
	
	// Update UI.
	App.m_AppWnd.UpdateTitle();
	App.m_AppWnd.m_AppDlg.RefreshView();
	UpdateUI();
}

/******************************************************************************
** Method:		OnFilter()
**
** Description:	Apply the new filter.
**
** Parameters:	nCmdID		The command ID of the filter.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnFilter(int nCmdID)
{
	// Get the filter to use.
	App.m_pFilter = App.m_aoFilters[nCmdID-ID_FIRST_FILTER_CMD];

	// Apply it.
	App.ApplyFilter();
	
	// Update UI.
	App.m_AppWnd.UpdateTitle();
	App.m_AppWnd.m_AppDlg.RefreshView();
	UpdateUI();
}

/******************************************************************************
** Method:		OnViewColumn*()
**
** Description:	Show/Hide the specfied column.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnViewColumnName()
{
	App.m_AppWnd.m_AppDlg.ToggleColumn(CAppDlg::HOST_NAME);
}

void CAppCmds::OnViewColumnMod()
{
	App.m_AppWnd.m_AppDlg.ToggleColumn(CAppDlg::MOD_NAME);
}

void CAppCmds::OnViewColumnMap()
{
	App.m_AppWnd.m_AppDlg.ToggleColumn(CAppDlg::MAP_TITLE);
}

void CAppCmds::OnViewColumnPlayers()
{
	App.m_AppWnd.m_AppDlg.ToggleColumn(CAppDlg::PLAYERS);
}

void CAppCmds::OnViewColumnPing()
{
	App.m_AppWnd.m_AppDlg.ToggleColumn(CAppDlg::PING_TIME);
}

void CAppCmds::OnViewColumnIPAddress()
{
	App.m_AppWnd.m_AppDlg.ToggleColumn(CAppDlg::IP_ADDRESS);
}

void CAppCmds::OnViewColumnIPPort()
{
	App.m_AppWnd.m_AppDlg.ToggleColumn(CAppDlg::IP_PORT);
}

/******************************************************************************
** Method:		OnViewColumnAutoSize()
**
** Description:	Auto size all the columns.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnViewColumnAutoSize()
{
	App.m_AppWnd.m_AppDlg.AutoSizeColumns();
}

/******************************************************************************
** Method:		OnViewSortBy*()
**
** Description:	Sort the servers view by the specified column.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnViewSortByName()
{
	OnViewSortBy(CAppDlg::HOST_NAME);
}

void CAppCmds::OnViewSortByMod()
{
	OnViewSortBy(CAppDlg::MOD_NAME);
}

void CAppCmds::OnViewSortByMap()
{
	OnViewSortBy(CAppDlg::MAP_TITLE);
}

void CAppCmds::OnViewSortByPlayers()
{
	OnViewSortBy(CAppDlg::PLAYERS);
}

void CAppCmds::OnViewSortByPing()
{
	OnViewSortBy(CAppDlg::PING_TIME);
}

void CAppCmds::OnViewSortByIPAddress()
{
	OnViewSortBy(CAppDlg::IP_ADDRESS);
}

void CAppCmds::OnViewSortByIPPort()
{
	OnViewSortBy(CAppDlg::IP_PORT);
}

void CAppCmds::OnViewSortBy(int eColumn)
{
	App.m_AppWnd.m_AppDlg.Sort(eColumn);

	UpdateUI();
}

/******************************************************************************
** Method:		OnViewDetails()
**
** Description:	View more details about the game on the selected server.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnViewDetails()
{
	// Get selected server, if one.
	CRow* pRow = App.m_AppWnd.m_AppDlg.SelectedRow();

	if (pRow == NULL)
		return;

	CGameInfoDlg Dlg(*pRow);

	Dlg.RunModal(App.m_rMainWnd);
}

/******************************************************************************
** Method:		OnOptionsMaster()
**
** Description:	Configure the master server settings.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnOptionsMaster()
{
	CMasterQueryDlg Dlg;

	Dlg.m_oConfig = App.m_oMtrQryOpts;

	if (Dlg.RunModal(App.m_rMainWnd) == IDOK)
	{
		App.m_oMtrQryOpts = Dlg.m_oConfig;

		App.m_oPingOpts.m_bModified = true;
	}
}

/******************************************************************************
** Method:		OnOptionsPing()
**
** Description:	Configure the ping settings.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnOptionsPing()
{
	CPingOptsDlg Dlg;

	Dlg.m_oConfig = App.m_oPingOpts;

	if (Dlg.RunModal(App.m_rMainWnd) == IDOK)
	{
		App.m_oPingOpts = Dlg.m_oConfig;

		// Update the background ping timer.
		App.StopPingTimer();
		App.StartPingTimer();

		App.m_oPingOpts.m_bModified = true;
	}
}

/******************************************************************************
** Method:		OnOptionsFilters()
**
** Description:	Configure the filters.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnOptionsFilters()
{
	CFiltersDlg Dlg;

	Dlg.RunModal(App.m_rMainWnd);
}

/******************************************************************************
** Method:		OnOptionsFavourites()
**
** Description:	Configure the files where the favourites are stored.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnOptionsFavourites()
{
	CFavouritesDlg Dlg;

	Dlg.m_oFavFiles.CopyTable(App.m_oFavFiles);

	if (Dlg.RunModal(App.m_rMainWnd) == IDOK)
	{
		// Save new config.
		App.m_oFavFiles.Truncate();
		App.m_oFavFiles.CopyTable(Dlg.m_oFavFiles);

		// Rebuild the favourites cache.
		App.LoadFavourites();
		App.CheckFavourites();

		// Update UI.
		App.m_AppWnd.m_AppDlg.RefreshAllRows();
		UpdateUI();
	}
}

/******************************************************************************
** Method:		OnHelpAbout()
**
** Description:	Show the about dialog.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnHelpAbout()
{
	CAboutDlg Dlg;

	Dlg.RunModal(App.m_rMainWnd);
}

/******************************************************************************
** Method:		OnUI...()
**
** Description:	UI update handlers.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnUIServersPingFiltered()
{
	CMenu& oMenu = App.m_AppWnd.m_Menu;

	oMenu.EnableCmd(ID_SERVERS_PING_FILTERED, (App.m_pFilter != NULL));
}

void CAppCmds::OnUIServersFindNext()
{
	CMenu& oMenu = App.m_AppWnd.m_Menu;

	oMenu.EnableCmd(ID_SERVERS_FIND_NEXT, !App.m_strFindText.Empty());
}

void CAppCmds::OnUIServersAddFav()
{
	CMenu&   oMenu     = App.m_AppWnd.m_Menu;
	CAppDlg& oAppDlg   = App.m_AppWnd.m_AppDlg;
	CRow*    pSelRow   = oAppDlg.SelectedRow();
	bool     bModValid = ((pSelRow != NULL) && (strlen(pSelRow->Field(CServers::MOD_NAME)) > 0));
	bool     bCanAdd   = ((bModValid) && (pSelRow->Field(CServers::FAV_ID) == null));

	oMenu.EnableCmd(ID_SERVERS_ADD_FAV, bCanAdd);
}

void CAppCmds::OnUIServersDelFav()
{
	CMenu&   oMenu     = App.m_AppWnd.m_Menu;
	CAppDlg& oAppDlg   = App.m_AppWnd.m_AppDlg;
	CRow*    pSelRow   = oAppDlg.SelectedRow();
	bool     bModValid = ((pSelRow != NULL) && (strlen(pSelRow->Field(CServers::MOD_NAME)) > 0));
	bool     bCanDel   = ((bModValid) && (pSelRow->Field(CServers::FAV_ID) != null));

	oMenu.EnableCmd(ID_SERVERS_DEL_FAV, bCanDel);
}

void CAppCmds::OnUIFilter()
{
	CMenu& oMenu = App.m_AppWnd.m_Menu;

	oMenu.CheckCmd(ID_FILTER_NONE, (App.m_pFilter == NULL));

	for (int i = 0; i < App.m_aoFilters.Size(); ++i)
		oMenu.CheckCmd(ID_FIRST_FILTER_CMD+i, (App.m_pFilter == App.m_aoFilters[i]));
}

void CAppCmds::OnUIViewSortByName()
{
	OnUIViewSortBy(ID_VIEW_SORT_NAME, CAppDlg::HOST_NAME);
}

void CAppCmds::OnUIViewSortByMod()
{
	OnUIViewSortBy(ID_VIEW_SORT_MOD, CAppDlg::MOD_NAME);
}

void CAppCmds::OnUIViewSortByMap()
{
	OnUIViewSortBy(ID_VIEW_SORT_MAP, CAppDlg::MAP_TITLE);
}

void CAppCmds::OnUIViewSortByPlayers()
{
	OnUIViewSortBy(ID_VIEW_SORT_PLAYERS, CAppDlg::PLAYERS);
}

void CAppCmds::OnUIViewSortByPing()
{
	OnUIViewSortBy(ID_VIEW_SORT_PING, CAppDlg::PING_TIME);
}

void CAppCmds::OnUIViewSortByIPAddress()
{
	OnUIViewSortBy(ID_VIEW_SORT_IP_ADDRESS, CAppDlg::IP_ADDRESS);
}

void CAppCmds::OnUIViewSortByIPPort()
{
	OnUIViewSortBy(ID_VIEW_SORT_IP_PORT, CAppDlg::IP_PORT);
}

void CAppCmds::OnUIViewSortBy(uint nCmdID, int nColumn)
{
	CMenu&   oMenu   = App.m_AppWnd.m_Menu;
	CAppDlg& oAppDlg = App.m_AppWnd.m_AppDlg;

	oMenu.CheckCmd(nCmdID, (oAppDlg.SortColumn() == nColumn));
}

void CAppCmds::OnUIViewDetails()
{
	CMenu&   oMenu   = App.m_AppWnd.m_Menu;
	CAppDlg& oAppDlg = App.m_AppWnd.m_AppDlg;

	oMenu.EnableCmd(ID_VIEW_DETAILS, (oAppDlg.SelectedRow() != NULL));
}

/******************************************************************************
** Method:		CmdHintStr()
**
** Description:	Get the commands' hint text. If the command is one of the
**				filters return a custom hint, otherwise foward to base class.
**
** Parameters:	nCmdID		The command.
**
** Returns:		The hint text.
**
*******************************************************************************
*/

CString CAppCmds::CmdHintStr(uint nCmdID) const
{
	// Is a filter?
	if ( (nCmdID >= ID_FIRST_FILTER_CMD) && (nCmdID <= ID_LAST_FILTER_CMD) )
		return App.m_aoFilters[nCmdID-ID_FIRST_FILTER_CMD]->m_strDesc;

	return CCmdControl::CmdHintStr(nCmdID);
}
