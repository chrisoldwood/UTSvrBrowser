/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CAppDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"

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

CAppDlg::CAppDlg()
	: CMainDlg(IDD_MAIN)
	, m_nSortColumn(CServers::IP_ADDRESS)
	, m_eSortOrder(CSortColumns::ASC)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_GRID,	&m_lvGrid)
	END_CTRL_TABLE

	DEFINE_GRAVITY_TABLE
		CTRLGRAV(IDC_GRID, LEFT_EDGE, TOP_EDGE, RIGHT_EDGE, BOTTOM_EDGE)
	END_GRAVITY_TABLE

	DEFINE_CTRLMSG_TABLE
		NFY_CTRLMSG(IDC_GRID, LVN_ITEMCHANGED, OnGridSelchange)
		NFY_CTRLMSG(IDC_GRID, NM_RCLICK,       OnGridRightClick)
		NFY_CTRLMSG(IDC_GRID, LVN_COLUMNCLICK, OnGridClickColumn)
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

void CAppDlg::OnInitDialog()
{
	// Set grid style.
//	m_lvGrid.Font(CFont(ANSI_FIXED_FONT));
	m_lvGrid.FullRowSelect(true);
//	m_lvGrid.GridLines(true);

	// Create grid columns.
	m_lvGrid.InsertColumn(IP_ADDRESS, "IP Address",  100, LVCFMT_LEFT);
	m_lvGrid.InsertColumn(IP_PORT,    "IP Port",      50, LVCFMT_LEFT);
	m_lvGrid.InsertColumn(HOST_NAME,  "Server",      250, LVCFMT_LEFT);
	m_lvGrid.InsertColumn(MAP_TITLE,  "Map",         100, LVCFMT_LEFT);
	m_lvGrid.InsertColumn(GAME_TYPE,  "Type",        100, LVCFMT_LEFT);
	m_lvGrid.InsertColumn(PLAYERS,    "Players",     100, LVCFMT_LEFT);
}

/******************************************************************************
** Method:		RefreshView()
**
** Description:	Refresh the master server list view.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppDlg::RefreshView()
{
	// Reset grid.
	m_lvGrid.DeleteAllItems();

	// Get the cache details and sort.
	CResultSet oRS = App.m_oServers.SelectAll();

	oRS.OrderBy(CSortColumns(m_nSortColumn, m_eSortOrder));

	// For all rows.
	for (int i = 0; i < oRS.Count(); ++i)
	{
		CRow& oRow = oRS[i];
		int   nRow = m_lvGrid.ItemCount();

		// Add to the grid.
		m_lvGrid.InsertItem(nRow,            oRow[CServers::IP_ADDRESS]);
		m_lvGrid.ItemText  (nRow, IP_PORT,   oRow[CServers::IP_PORT].Format());
		m_lvGrid.ItemText  (nRow, HOST_NAME, oRow[CServers::HOST_NAME]);
		m_lvGrid.ItemText  (nRow, MAP_TITLE, oRow[CServers::MAP_TITLE]);
		m_lvGrid.ItemText  (nRow, GAME_TYPE, oRow[CServers::GAME_TYPE]);
		m_lvGrid.ItemText  (nRow, PLAYERS,   FmtPlayers(oRow));
		m_lvGrid.ItemPtr   (nRow, &oRow);
	}
}

/******************************************************************************
** Method:		Sort()
**
** Description:	Sort the view by the view column specified.
**
** Parameters:	nColumn		The view column to sort on.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppDlg::Sort(int nColumn)
{
	// Reverse sort order by default.
	CSortColumns::Dir eDir = (CSortColumns::Dir) -m_eSortOrder;

	// Sort by different column?
	if (nColumn != m_nSortColumn)
	{
		// Decode column.
		switch (nColumn)
		{
			case IP_ADDRESS:	eDir = CSortColumns::ASC;	break;
			case IP_PORT:		eDir = CSortColumns::ASC;	break;
			case HOST_NAME:		eDir = CSortColumns::ASC;	break;
			case MAP_TITLE:		eDir = CSortColumns::ASC;	break;
			case GAME_TYPE:		eDir = CSortColumns::ASC;	break;
			case PLAYERS:		eDir = CSortColumns::DESC;	break;
			default:			ASSERT(false);				break;
		}
	}

	m_nSortColumn = nColumn;
	m_eSortOrder  = eDir;

	RefreshView();
}

/******************************************************************************
** Method:		Fmt*()
**
** Description:	Column formatting methods.
**
** Parameters:	oRow	The row to format.
**
** Returns:		The formatted string.
**
*******************************************************************************
*/

CString CAppDlg::FmtPlayers(const CRow& oRow)
{
	CString str;

	int nCurPlayers = oRow[CServers::NUM_PLAYERS];
	int nMaxPlayers = oRow[CServers::MAX_PLAYERS];

	str.Format("%d/%d", nCurPlayers, nMaxPlayers);

	return str;
}

/******************************************************************************
** Method:		OnGridSelchange()
**
** Description:	Grid item (de)selected, update the main UI.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

LRESULT CAppDlg::OnGridSelchange(NMHDR&)
{
	return 0;
}

/******************************************************************************
** Method:		OnGridRightClick()
**
** Description:	Right click on the grid, show context menu.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

LRESULT CAppDlg::OnGridRightClick(NMHDR&)
{
	return 0;
}

/******************************************************************************
** Method:		OnGridClickColumn()
**
** Description:	Grid column clicked, resort the grid.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

LRESULT CAppDlg::OnGridClickColumn(NMHDR& oHdr)
{
	NMLISTVIEW& oParam = (NMLISTVIEW&) oHdr;

	// Sort view by column selected.
	Sort(oParam.iSubItem);

	App.m_AppCmds.UpdateUI();

	return 0;
}
