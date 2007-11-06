/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SUMMARYDLG.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CSummaryDlg class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "SummaryDlg.hpp"
#include "UTSvrBrowser.hpp"

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

CSummaryDlg::CSummaryDlg()
	: CDialog(IDD_SUMMARY)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_GRID,	&m_lvGrid)
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

void CSummaryDlg::OnInitDialog()
{
	// Set grid style.
//	m_lvGrid.Font(CFont(ANSI_FIXED_FONT));
	m_lvGrid.FullRowSelect(true);
//	m_lvGrid.GridLines(true);

	// Create grid columns.
	m_lvGrid.InsertColumn(MOD_NAME, "Mod",     125, LVCFMT_LEFT);
	m_lvGrid.InsertColumn(SERVERS,  "Servers",  60, LVCFMT_LEFT);
	m_lvGrid.InsertColumn(PLAYERS,  "Players",  60, LVCFMT_LEFT);

	// Extract active servers only.
	CResultSet oRS = App.m_oSummary.SelectAll();

	// Sort result set.
	oRS.OrderBy(CSummary::NUM_SERVERS, CSortColumns::DESC);

	// Load grid with data.
	for (int i = 0; i < oRS.Count(); ++i)
	{
		CRow& oRow = oRS[i];
		int   nRow = m_lvGrid.ItemCount();

		// Add to the grid.
		m_lvGrid.InsertItem(nRow,          oRow[CSummary::MOD_NAME]);
		m_lvGrid.ItemText  (nRow, SERVERS, oRow[CSummary::NUM_SERVERS].Format());
		m_lvGrid.ItemText  (nRow, PLAYERS, oRow[CSummary::NUM_PLAYERS].Format());
		m_lvGrid.ItemPtr   (nRow, &oRow);

		// Set unknown mod to "(other)".
		if (oRow[CSummary::MOD_NAME] == "")
			m_lvGrid.ItemText(nRow, MOD_NAME, "(other)");
	}
}

/******************************************************************************
** Method:		OnOk()
**
** Description:	The OK button was pressed.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CSummaryDlg::OnOk()
{
	return true;
}
