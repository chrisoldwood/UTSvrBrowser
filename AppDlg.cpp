/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CAppDlg class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "AppDlg.hpp"
#include "UTSvrBrowser.hpp"
#include <limits.h>
#include <WCL/BusyCursor.hpp>

/******************************************************************************
**
** Constants.
**
*******************************************************************************
*/

const int CAppDlg::DEF_COLUMN_WIDTH = 50;

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
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_GRID,	&m_lvGrid)
	END_CTRL_TABLE

	DEFINE_GRAVITY_TABLE
		CTRLGRAV(IDC_GRID, LEFT_EDGE, TOP_EDGE, RIGHT_EDGE, BOTTOM_EDGE)
	END_GRAVITY_TABLE

	DEFINE_CTRLMSG_TABLE
		NFY_CTRLMSG(IDC_GRID, LVN_ITEMCHANGED, &CAppDlg::OnGridSelchange)
		NFY_CTRLMSG(IDC_GRID, NM_RCLICK,       &CAppDlg::OnGridRightClick)
		NFY_CTRLMSG(IDC_GRID, NM_DBLCLK,       &CAppDlg::OnGridDoubleClick)
		NFY_CTRLMSG(IDC_GRID, LVN_COLUMNCLICK, &CAppDlg::OnGridClickColumn)
	END_CTRLMSG_TABLE
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
/*
CAppDlg::~CAppDlg()
{
	delete m_pRS;
}
*/
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
	m_lvGrid.InsertColumn(HOST_NAME,  "Server",     App.m_anColWidths[HOST_NAME],  LVCFMT_LEFT);
	m_lvGrid.InsertColumn(MOD_NAME,   "Mod",        App.m_anColWidths[MOD_NAME],   LVCFMT_LEFT);
	m_lvGrid.InsertColumn(MAP_TITLE,  "Map",        App.m_anColWidths[MAP_TITLE],  LVCFMT_LEFT);
	m_lvGrid.InsertColumn(GAME_TYPE,  "Type",       App.m_anColWidths[GAME_TYPE],  LVCFMT_LEFT);
	m_lvGrid.InsertColumn(PLAYERS,    "Players",    App.m_anColWidths[PLAYERS],    LVCFMT_LEFT);
	m_lvGrid.InsertColumn(PING_TIME,  "Ping",       App.m_anColWidths[PING_TIME],  LVCFMT_LEFT);
	m_lvGrid.InsertColumn(IP_ADDRESS, "IP Address", App.m_anColWidths[IP_ADDRESS], LVCFMT_LEFT);
	m_lvGrid.InsertColumn(IP_PORT,    "IP Port",    App.m_anColWidths[IP_PORT],    LVCFMT_LEFT);
//	m_lvGrid.InsertColumn(LAST_ERROR, "Error",      App.m_anColWidths[LAST_ERROR], LVCFMT_LEFT);

	// Set the listview icons.
	m_lvGrid.ImageList(LVSIL_SMALL, IDB_LIST_ICONS, 16, RGB(255, 0, 255));

	// Fetch inital sort options.
	m_nSortColumn = App.m_nSortCol;
	m_eSortOrder  = (CSortColumns::Dir) App.m_nSortDir;
}

/******************************************************************************
** Method:		OnDestroy()
**
** Description:	The dialog is closing, save the column widths.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppDlg::OnDestroy()
{
	// Save final sort order.
	App.m_nSortCol = m_nSortColumn;
	App.m_nSortDir = m_eSortOrder;

	// Save final column widths.
	for (int i = 0; i < NUM_COLUMNS; ++i)
		App.m_anColWidths[i] = m_lvGrid.ColumnWidth(i);
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
	CBusyCursor oCursor;

	// Remove previous data set.
	m_lvGrid.DeleteAllItems();

	App.m_oRS.OrderBy(CSortColumns(TableColumn(m_nSortColumn), m_eSortOrder));

	// Initialise listview with new data set.
	m_lvGrid.Reserve(App.m_oServers.RowCount());

	// For all rows.
	for (int i = 0; i < App.m_oRS.Count(); ++i)
	{
		CRow& oRow  = App.m_oRS[i];
		int   nIcon = (oRow[CServers::FAV_ID] == null) ? 1 : 0;

		// Add to the grid.
		m_lvGrid.InsertItem(i,             oRow[CServers::HOST_NAME], nIcon);
		m_lvGrid.ItemText  (i, MOD_NAME,   oRow[CServers::MOD_NAME]);
		m_lvGrid.ItemText  (i, MAP_TITLE,  oRow[CServers::MAP_TITLE]);
		m_lvGrid.ItemText  (i, GAME_TYPE,  oRow[CServers::GAME_TYPE]);
		m_lvGrid.ItemText  (i, PLAYERS,    FmtPlayers(oRow));
		m_lvGrid.ItemText  (i, PING_TIME,  FmtPingTime(oRow));
		m_lvGrid.ItemText  (i, IP_ADDRESS, oRow[CServers::IP_ADDRESS]);
		m_lvGrid.ItemText  (i, IP_PORT,    oRow[CServers::IP_PORT].Format());
//		m_lvGrid.ItemText  (i, LAST_ERROR, FmtLastErr(oRow));
		m_lvGrid.ItemPtr   (i, &oRow);
	}
}

/******************************************************************************
** Method:		RefreshRow()
**
** Description:	Refresh a single servers details.
**
** Parameters:	oRow	The servers details.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppDlg::RefreshRow(CRow& oRow)
{
	// Find grid row number.
	int nRow = m_lvGrid.FindItem(&oRow);

	if (nRow == LB_ERR)
		return;

	// Display server details.
	m_lvGrid.ItemImage(nRow,             (oRow[CServers::FAV_ID] == null) ? 1 : 0);
	m_lvGrid.ItemText (nRow, HOST_NAME,  oRow[CServers::HOST_NAME]);
	m_lvGrid.ItemText (nRow, MOD_NAME,   oRow[CServers::MOD_NAME]);
	m_lvGrid.ItemText (nRow, MAP_TITLE,  oRow[CServers::MAP_TITLE]);
	m_lvGrid.ItemText (nRow, GAME_TYPE,  oRow[CServers::GAME_TYPE]);
	m_lvGrid.ItemText (nRow, PLAYERS,    FmtPlayers(oRow));
	m_lvGrid.ItemText (nRow, PING_TIME,  FmtPingTime(oRow));
	m_lvGrid.ItemText (nRow, IP_ADDRESS, oRow[CServers::IP_ADDRESS]);
	m_lvGrid.ItemText (nRow, IP_PORT,    oRow[CServers::IP_PORT].Format());
//	m_lvGrid.ItemText (nRow, LAST_ERROR, FmtLastErr(oRow));
}

/******************************************************************************
** Method:		RefreshAllRows()
**
** Description:	Refreshes all servers details without changing the listview
**				structure.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppDlg::RefreshAllRows()
{
	CBusyCursor oCursor;

	// For all rows...
	for (int i = 0; i < App.m_oRS.Count(); ++i)
	{
		CRow& oRow = App.m_oRS[i];

		// Display server details.
		m_lvGrid.ItemImage(i,             (oRow[CServers::FAV_ID] == null) ? 1 : 0);
		m_lvGrid.ItemText (i, HOST_NAME,  oRow[CServers::HOST_NAME]);
		m_lvGrid.ItemText (i, MOD_NAME,   oRow[CServers::MOD_NAME]);
		m_lvGrid.ItemText (i, MAP_TITLE,  oRow[CServers::MAP_TITLE]);
		m_lvGrid.ItemText (i, GAME_TYPE,  oRow[CServers::GAME_TYPE]);
		m_lvGrid.ItemText (i, PLAYERS,    FmtPlayers(oRow));
		m_lvGrid.ItemText (i, PING_TIME,  FmtPingTime(oRow));
		m_lvGrid.ItemText (i, IP_ADDRESS, oRow[CServers::IP_ADDRESS]);
		m_lvGrid.ItemText (i, IP_PORT,    oRow[CServers::IP_PORT].Format());
//		m_lvGrid.ItemText (i, LAST_ERROR, FmtLastErr(oRow));
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
			case HOST_NAME:		eDir = CSortColumns::ASC;	break;
			case MOD_NAME:		eDir = CSortColumns::ASC;	break;
			case MAP_TITLE:		eDir = CSortColumns::ASC;	break;
			case GAME_TYPE:		eDir = CSortColumns::ASC;	break;
			case PLAYERS:		eDir = CSortColumns::DESC;	break;
			case PING_TIME:		eDir = CSortColumns::ASC;	break;
			case IP_ADDRESS:	eDir = CSortColumns::ASC;	break;
			case IP_PORT:		eDir = CSortColumns::ASC;	break;
//			case LAST_ERROR:	eDir = CSortColumns::ASC;	break;
			default:			ASSERT_FALSE();				break;
		}
	}

	m_nSortColumn = nColumn;
	m_eSortOrder  = eDir;

	RefreshView();
}

/******************************************************************************
** Method:		ToggleColumn()
**
** Description:	Show/Hide the given column.
**
** Parameters:	nColumn		The view column to show/hide.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppDlg::ToggleColumn(int nColumn)
{
	int nWidth = m_lvGrid.ColumnWidth(nColumn);

	// Show?
	if (nWidth == 0)
	{
		nWidth = App.m_anColWidths[nColumn];

		// Column saved as hidden?
		if (nWidth == 0)
			m_lvGrid.ColumnWidthAuto(nColumn);  
		else
			m_lvGrid.ColumnWidth(nColumn, nWidth);  
	}
	// Hide
	else
	{
		App.m_anColWidths[nColumn] = nWidth;
		m_lvGrid.ColumnWidth(nColumn, 0);  
	}
}

/******************************************************************************
** Method:		AutoSizeColumns()
**
** Description:	Auto-size all the columns.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CAppDlg::AutoSizeColumns()
{
	int nColumns = m_lvGrid.NumColumns();

	for (int i = 0; i < nColumns; ++i)
		m_lvGrid.ColumnWidthAuto(i);
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

	if (nMaxPlayers != 0)
		str.Format("%d/%d", nCurPlayers, nMaxPlayers);
	else
		str = "N/A";

	return str;
}

CString CAppDlg::FmtPingTime(const CRow& oRow)
{
	CString str;

	int nTime = oRow[CServers::PING_TIME];

	if (nTime != INT_MAX)
		str.Format("%d ms", nTime);
	else
		str = "N/A";

	return str;
}

CString CAppDlg::FmtLastErr(const CRow& oRow)
{
	int nError = oRow[CServers::LAST_ERROR];

	switch (nError)
	{
		case CServers::ERROR_NONE:		return "";
		case CServers::ERROR_FAILED:	return "Socket Error";
		case CServers::ERROR_TIMED_OUT:	return "Timed Out";
		default:						break;
	}

	return "";
}

/******************************************************************************
** Method:		TableColumn()
**
** Description:	Gets the table column that corresponds to the view column.
**
** Parameters:	nColumn		The view column.
**
** Returns:		The table column.
**
*******************************************************************************
*/

int CAppDlg::TableColumn(int nColumn)
{
	int nTabCol = CServers::HOST_NAME;

	// Decode column.
	switch (nColumn)
	{
		case HOST_NAME:		nTabCol = CServers::HOST_NAME;		break;
		case MOD_NAME:		nTabCol = CServers::MOD_NAME;		break;
		case MAP_TITLE:		nTabCol = CServers::MAP_TITLE;		break;
		case GAME_TYPE:		nTabCol = CServers::GAME_TYPE;		break;
		case PLAYERS:		nTabCol = CServers::NUM_PLAYERS;	break;
		case PING_TIME:		nTabCol = CServers::PING_TIME;		break;
		case IP_ADDRESS:	nTabCol = CServers::IP_ADDRESS;		break;
		case IP_PORT:		nTabCol = CServers::IP_PORT;		break;
//		case LAST_ERROR:	nTabCol = CServers::LAST_ERROR;		break;
		default:			ASSERT_FALSE();						break;
	}

	return nTabCol;
}

/******************************************************************************
** Method:		OnGridSelchange()
**
** Description:	Grid item (de)selected, update the main UI.
**
** Parameters:	See LVN_ITEMCHANGED.
**
** Returns:		See LVN_ITEMCHANGED.
**
*******************************************************************************
*/

LRESULT CAppDlg::OnGridSelchange(NMHDR& oNMHdr)
{
	NMLISTVIEW& oMsgHdr = reinterpret_cast<NMLISTVIEW&>(oNMHdr);

	if (oMsgHdr.uChanged & LVIF_STATE)
		App.m_AppCmds.UpdateUI();

	return 0;
}

/******************************************************************************
** Method:		OnGridRightClick()
**
** Description:	Right click on the grid, show context menu.
**
** Parameters:	See NM_RCLICK.
**
** Returns:		See NM_RCLICK.
**
*******************************************************************************
*/

LRESULT CAppDlg::OnGridRightClick(NMHDR& oNMHdr)
{
	NMITEMACTIVATE& oMsgHdr = reinterpret_cast<NMITEMACTIVATE&>(oNMHdr);

	CPopupMenu oMenu(IDR_GRIDMENU);

	CRow* pSelRow   = SelectedRow();
	bool bAnyRows   = (m_lvGrid.ItemCount() > 0);
	bool bSelection = (pSelRow != NULL);
	bool bFiltered  = (App.m_pFilter != NULL);
	bool bModValid  = ((pSelRow != NULL) && (strlen(pSelRow->Field(CServers::MOD_NAME)) > 0));
	bool bCanAddFav = ((bModValid) && (pSelRow->Field(CServers::FAV_ID) == null));
	bool bCanDelFav = ((bModValid) && (pSelRow->Field(CServers::FAV_ID) != null));

	// Enable/Disable relevant menu commands.
	oMenu.EnableCmd(ID_SERVERS_PING,          bSelection);
	oMenu.EnableCmd(ID_SERVERS_PING_FILTERED, bFiltered);
	oMenu.EnableCmd(ID_SERVERS_PING_ALL,      bAnyRows);
	oMenu.EnableCmd(ID_SERVERS_REFRESH,       true);
	oMenu.EnableCmd(ID_SERVERS_ADD_FAV,       bCanAddFav);
	oMenu.EnableCmd(ID_SERVERS_DEL_FAV,       bCanDelFav);
	oMenu.EnableCmd(ID_VIEW_DETAILS,          bSelection);

	// Calculate popup menu position.
	CPoint ptMenu = oMsgHdr.ptAction;

	::ClientToScreen(m_lvGrid.Handle(), &ptMenu);

	// Show menu.
	uint nCmdID = oMenu.TrackMenu(m_lvGrid, ptMenu);

	// Dispatch command.
	if (nCmdID != NULL)
		App.m_AppWnd.PostCommand(nCmdID);

	return 0;
}

/******************************************************************************
** Method:		OnGridDoubleClick()
**
** Description:	Grid row double-clicked, show the details.
**
** Parameters:	See LVN_COLUMNCLICK.
**
** Returns:		See LVN_COLUMNCLICK.
**
*******************************************************************************
*/

LRESULT CAppDlg::OnGridDoubleClick(NMHDR& /*oNMHeader*/)
{
	// Repost as a "View | Details" command.
	if (m_lvGrid.IsSelection())
		App.m_AppWnd.PostCommand(ID_VIEW_DETAILS);

	return 0;
}

/******************************************************************************
** Method:		OnGridClickColumn()
**
** Description:	Grid column clicked, resort the grid.
**
** Parameters:	See LVN_COLUMNCLICK.
**
** Returns:		See LVN_COLUMNCLICK.
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
