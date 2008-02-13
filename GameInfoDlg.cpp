/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		GAMEINFODLG.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CGameInfoDlg class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "GameInfoDlg.hpp"
#include <WCL/BusyCursor.hpp>
#include "GameServer.hpp"
#include "Servers.hpp"
#include "QueryResponse.hpp"
#include <WCL/StrCvt.hpp>

/******************************************************************************
**
** Constants.
**
*******************************************************************************
*/

const uint CGameInfoDlg::TIMER_ID   = 1;
const uint CGameInfoDlg::TIMER_FREQ = 5000;

/******************************************************************************
** Method:		Default constructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CGameInfoDlg::CGameInfoDlg(CRow& oRow)
	: CDialog(IDD_GAME_DETAILS)
	, m_oRow(oRow)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_TEAM_1, &m_lvTeam1)
		CTRL(IDC_TEAM_2, &m_lvTeam2)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
	END_CTRLMSG_TABLE
}

/******************************************************************************
** Method:		OnInitDialog()
**
** Description:	Initialise the dialog.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CGameInfoDlg::OnInitDialog()
{
	// Set team grids style.
	m_lvTeam1.FullRowSelect(true);
	m_lvTeam2.FullRowSelect(true);

	// Create team grids columns.
	m_lvTeam1.InsertColumn(PLAYER, TXT("Player"), 125, LVCFMT_LEFT);
	m_lvTeam1.InsertColumn(FRAGS,  TXT("Frags"),   40, LVCFMT_LEFT);
	m_lvTeam2.InsertColumn(PLAYER, TXT("Player"), 125, LVCFMT_LEFT);
	m_lvTeam2.InsertColumn(FRAGS,  TXT("Frags"),   40, LVCFMT_LEFT);

	// Get the current game status.
	RefreshInfo();

	// Start the refresh timer.
	StartTimer(TIMER_ID, TIMER_FREQ);
}

/******************************************************************************
** Method:		OnCancel()
**
** Description:	The close button was pressed.
**
** Parameters:	None.
**
** Returns:		true.
**
*******************************************************************************
*/

bool CGameInfoDlg::OnCancel()
{
	// Stop the refresh timer.
	StopTimer(TIMER_ID);

	return true;
}

/******************************************************************************
** Method:		OnTimer()
**
** Description:	The refresh timer has fired.
**
** Parameters:	nTimerID	The ID of the timer.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CGameInfoDlg::OnTimer(uint /*nTimerID*/)
{
	RefreshInfo();
}

/******************************************************************************
** Method:		RefreshInfo()
**
** Description:	Query the server for the current game status.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CGameInfoDlg::RefreshInfo()
{
	CBusyCursor    oBusy;
	CGameServer    oServer(m_oRow[CServers::IP_ADDRESS], m_oRow[CServers::IP_PORT]);
	CQueryResponse oResponse;

	// Query server for players and game settings.
	if (oServer.QueryStatus(oResponse))
	{
		// Clear team grids.
		m_lvTeam1.DeleteAllItems();
		m_lvTeam2.DeleteAllItems();

		// Get number of players.
		int nPlayers = CStrCvt::ParseInt(oResponse.FieldValue(TXT("numplayers")));

		// Add players to team grids...
		for (int i = 0; i < nPlayers; ++i)
		{
			CString strIndex;

			// Format player field identifier.
			strIndex.Format(TXT("_%d"), i);

			// Get player details.
			CString strPlayer = oResponse.FieldValue(TXT("player") + strIndex);
			CString strFrags  = oResponse.FieldValue(TXT("frags")  + strIndex);
			CString strTeam   = oResponse.FieldValue(TXT("team")   + strIndex);

			// Ignore if we're missing any details.
			if (strPlayer.Empty() || strFrags.Empty() || strTeam.Empty())
				continue;

			// Add them to the team grid.
			if (CStrCvt::ParseUInt(strTeam) == 0)
				AddPlayer(m_lvTeam1, strPlayer, strFrags);
			else
				AddPlayer(m_lvTeam2, strPlayer, strFrags);
		}

		// Sort the team grids by frag count.
		m_lvTeam1.Sort(Compare, NULL);
		m_lvTeam2.Sort(Compare, NULL);
	}
}

/******************************************************************************
** Method:		AddPlayer()
**
** Description:	Add a player to a team grid.
**
** Parameters:	lvTeam		The team grid to add them to.
**				pszPlayer	The players handle.
**				pszFrags	The players frag count.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CGameInfoDlg::AddPlayer(CListView& lvTeam, const tchar* pszPlayer, const tchar* pszFrags)
{
	ASSERT(pszPlayer != NULL);
	ASSERT(pszFrags  != NULL);

	int n = lvTeam.ItemCount();

	lvTeam.InsertItem(n,        pszPlayer);
	lvTeam.ItemText  (n, FRAGS, pszFrags );
	lvTeam.ItemData  (n,        CStrCvt::ParseInt(pszFrags));
}

/******************************************************************************
** Method:		Compare()
**
** Description:	Compare function for sorting the team grids by frag count.
**
** Parameters:	lParam1		LPARAM for item 1.
**				lParam2		LPARAM for item 2.
**
** Returns:		See strcmp.
**
*******************************************************************************
*/

int CALLBACK CGameInfoDlg::Compare(LPARAM lParam1, LPARAM lParam2, LPARAM)
{
	return (lParam2 - lParam1);
}
