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
#include "AboutDlg.hpp"
#include "ProgressDlg.hpp"

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
		CMD_ENTRY(ID_SERVERS_REFRESH,	OnServersRefresh,	NULL,				-1)
		CMD_ENTRY(ID_SERVERS_EXIT,		OnServersExit,		NULL,				-1)
		// Sort menu.
		CMD_ENTRY(ID_SORT_NAME,			OnSortByName,		OnUISortByName,		-1)
		CMD_ENTRY(ID_SORT_PLAYERS,		OnSortByPlayers,	OnUISortByPlayers,	-1)
		// Help menu.
		CMD_ENTRY(ID_HELP_ABOUT,		OnHelpAbout,		NULL,				10)
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

	// Trash old servers table.
	App.m_oServers.Truncate();

	// Show the progress dialog.
	CProgressDlg Dlg;

	Dlg.RunModeless(App.m_AppWnd);
	Dlg.Title("Finding Servers");
	App.m_AppWnd.Enable(false);

	Dlg.UpdateLabel("Querying " + App.m_strMasterAddr + "...");

	CMasterServer oMaster(App.m_strMasterAddr, App.m_nMasterPort);
	CStrArray     astrAddresses;

	// Fetch master list.
	if (oMaster.QueryServers("ut", astrAddresses) > 0)
	{
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

			// Add server to table.
			CRow& oRow = App.m_oServers.CreateRow();

			oRow[CServers::IP_KEY]      = strKey;
			oRow[CServers::IP_ADDRESS]  = strAddr;
			oRow[CServers::IP_PORT]     = atoi(strPort);

			App.m_oServers.InsertRow(oRow);
		}
	}

	// Create a thread pool.
	CThreadPool	oThreadPool(10);

	oThreadPool.Start();

	Dlg.InitMeter(App.m_oServers.RowCount());
	Dlg.UpdateLabel("Pinging servers...");

	// Start jobs to check all computers.
	for (int i = 0; (i < App.m_oServers.RowCount()) && (!Dlg.Abort()); ++i)
		oThreadPool.AddJob(new CQueryJob(App.m_oServers[i]));

	// Wait for jobs to complete OR user to cancel.
	while (oThreadPool.CompletedJobCount() != App.m_oServers.RowCount())
	{
		Dlg.UpdateMeter(oThreadPool.CompletedJobCount());

		// Check for "Cancel" button.
		App.m_MainThread.ProcessMsgQueue();

		if (Dlg.Abort())
		{
			Dlg.UpdateLabel("Cancelling...");
			oThreadPool.CancelAllJobs();
		}

		Sleep(250);
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
** Method:		OnSortBy*()
**
** Description:	Sort the servers view by the specified column.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppCmds::OnSortByName()
{
	App.m_AppWnd.m_AppDlg.Sort(CAppDlg::HOST_NAME);

	UpdateUI();
}

void CAppCmds::OnSortByPlayers()
{
	App.m_AppWnd.m_AppDlg.Sort(CAppDlg::PLAYERS);

	UpdateUI();
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

void CAppCmds::OnUISortByName()
{
	CMenu&   oMenu   = App.m_AppWnd.m_Menu;
	CAppDlg& oAppDlg = App.m_AppWnd.m_AppDlg;

	oMenu.CheckCmd(ID_SORT_NAME, (oAppDlg.SortColumn() == CAppDlg::HOST_NAME));
}

void CAppCmds::OnUISortByPlayers()
{
	CMenu&   oMenu   = App.m_AppWnd.m_Menu;
	CAppDlg& oAppDlg = App.m_AppWnd.m_AppDlg;

	oMenu.CheckCmd(ID_SORT_PLAYERS, (oAppDlg.SortColumn() == CAppDlg::PLAYERS));
}
