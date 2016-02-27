/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		FAVOURITESDLG.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CFavouritesDlg class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "FavouritesDlg.hpp"
#include "FavFileDlg.hpp"
#include "UTSvrBrowser.hpp"
#include <WCL/RegKey.hpp>

/******************************************************************************
**
** Local variables.
**
*******************************************************************************
*/

static const tchar* SEL_FOLDER_MSG = TXT("Select The UT System Folder\ne.g. C:\\UnrealTournament\\System");

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

CFavouritesDlg::CFavouritesDlg()
	: CDialog(IDD_FAVOURITES)
	, m_oFavFiles()
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_GRID, &m_lvGrid)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_DETECT, BN_CLICKED, &CFavouritesDlg::OnDetect)
		CMD_CTRLMSG(IDC_ADD,    BN_CLICKED, &CFavouritesDlg::OnAdd   )
		CMD_CTRLMSG(IDC_EDIT,   BN_CLICKED, &CFavouritesDlg::OnEdit  )
		CMD_CTRLMSG(IDC_REMOVE, BN_CLICKED, &CFavouritesDlg::OnRemove)
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

void CFavouritesDlg::OnInitDialog()
{
	// Set grid style.
//	m_lvGrid.Font(CFont(ANSI_FIXED_FONT));
	m_lvGrid.FullRowSelect(true);
//	m_lvGrid.GridLines(true);

	// Create grid columns.
	m_lvGrid.InsertColumn(MOD_NAME, TXT("Mod"),  100, LVCFMT_LEFT);
	m_lvGrid.InsertColumn(MOD_FILE, TXT("File"), 325, LVCFMT_LEFT);

	// Load grid...
	for (size_t i = 0; i < m_oFavFiles.RowCount(); ++i)
	{
		CRow& oRow = m_oFavFiles[i];
		int   nRow = m_lvGrid.ItemCount();

		// Add to the grid.
		m_lvGrid.InsertItem(nRow,           oRow[CFavFiles::MOD_NAME]);
		m_lvGrid.ItemText  (nRow, MOD_FILE, oRow[CFavFiles::MOD_FILE]);
		m_lvGrid.ItemPtr   (nRow, &oRow);
	}

	// Select 1st row by default.
	if (m_lvGrid.ItemCount() > 0)
		m_lvGrid.Select(0);
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

bool CFavouritesDlg::OnOk()
{
	return true;
}

/******************************************************************************
** Method:		OnDetect()
**
** Description:	Attempt to auto-detect the mod config files.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CFavouritesDlg::OnDetect()
{
	WCL::RegKey oKey;
	CPath       strFolder;

	// Try and find the regkey that contains the UT base path.
	if (WCL::RegKey::Exists(HKEY_LOCAL_MACHINE, TXT("SOFTWARE\\Unreal Technology\\Installed Apps\\UnrealTournament")))
	{
		oKey.Open(HKEY_LOCAL_MACHINE, TXT("SOFTWARE\\Unreal Technology\\Installed Apps\\UnrealTournament"), KEY_READ);

		strFolder = oKey.ReadStringValue(TXT("Folder"), TXT("")) / TXT("System");
	}

	// Detect failed?
	if (strFolder.Empty())
	{
		// Notify user.
		if (QueryMsg(TXT("Failed to detect the UT installation.\n\nDo you want to locate it manually?")) != IDYES)
			return;

		// Query user for folder.
		if (!strFolder.SelectDir(*this, SEL_FOLDER_MSG))
			return;
	}

	// For all mods.
	for (size_t i = 0; i < App.m_oMods.RowCount(); ++i)
	{
		CRow& oMod = App.m_oMods[i];

		// Mod already configured?
		if (m_oFavFiles.SelectRow(CFavFiles::MOD_NAME, oMod[CMods::MOD_NAME].ToValue()) != NULL)
			continue;

		// Favs file not available?
		if (tstrlen(oMod[CMods::FAVS_FILE]) == 0)
			continue;

		CPath strIniFile(strFolder, oMod[CMods::FAVS_FILE]);

		// Config file doesn't exist?
		if (!strIniFile.Exists())
			continue;

		// Add to favourite config files table.
		CRow& oRow = m_oFavFiles.CreateRow();

		oRow[CFavFiles::MOD_NAME] = oMod[CMods::MOD_NAME];
		oRow[CFavFiles::MOD_FILE] = strIniFile;

		m_oFavFiles.InsertRow(oRow);

		int nRow = m_lvGrid.ItemCount();

		// Add to the grid.
		m_lvGrid.InsertItem(nRow,           oRow[CFavFiles::MOD_NAME]);
		m_lvGrid.ItemText  (nRow, MOD_FILE, oRow[CFavFiles::MOD_FILE]);
		m_lvGrid.ItemPtr   (nRow, &oRow);
	}
}

/******************************************************************************
** Method:		OnAdd()
**
** Description:	Add a favourites file for a mod.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CFavouritesDlg::OnAdd()
{
	CFavFileDlg Dlg;

	Dlg.m_pFavFiles = &m_oFavFiles;

	if (Dlg.RunModal(*this) == IDOK)
	{
		// Update table.
		CRow& oRow = m_oFavFiles.CreateRow();

		oRow[CFavFiles::MOD_NAME] = Dlg.m_strMod;
		oRow[CFavFiles::MOD_FILE] = Dlg.m_strFile;

		m_oFavFiles.InsertRow(oRow, false);

		// Update view.
		int nRow = m_lvGrid.ItemCount();

		m_lvGrid.InsertItem(nRow,           oRow[CFavFiles::MOD_NAME]);
		m_lvGrid.ItemText  (nRow, MOD_FILE, oRow[CFavFiles::MOD_FILE]);
		m_lvGrid.ItemPtr   (nRow, &oRow);

		// Make selection.
		m_lvGrid.Select(nRow);
	}
}

/******************************************************************************
** Method:		OnEdit()
**
** Description:	Edit the selected mods favourites file.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CFavouritesDlg::OnEdit()
{
	int nSel = m_lvGrid.Selection();

	// No selection?
	if (nSel == LB_ERR)
		return;

	// Extract the selected row.
	CRow& oRow = *static_cast<CRow*>(m_lvGrid.ItemPtr(nSel));

	CFavFileDlg Dlg;

	Dlg.m_pFavFiles = &m_oFavFiles;
	Dlg.m_strMod    = oRow[CFavFiles::MOD_NAME];
	Dlg.m_strFile   = oRow[CFavFiles::MOD_FILE];

	// Show the editing dialog.
	if (Dlg.RunModal(*this) == IDOK)
	{
		// Update table.
		oRow[CFavFiles::MOD_FILE] = Dlg.m_strFile;

		// Update view.
		m_lvGrid.ItemText(nSel, MOD_NAME, oRow[CFavFiles::MOD_NAME]);
		m_lvGrid.ItemText(nSel, MOD_FILE, oRow[CFavFiles::MOD_FILE]);
	}
}

/******************************************************************************
** Method:		OnRemove()
**
** Description:	Remove the selected mods favourites file.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CFavouritesDlg::OnRemove()
{
	size_t nSel = m_lvGrid.Selection();

	// No selection?
	if (nSel == LB_ERR)
		return;

	// Extract the selected row.
	CRow& oRow = *static_cast<CRow*>(m_lvGrid.ItemPtr(nSel));

	// Remove from table and view.
	m_oFavFiles.DeleteRow(oRow);
	m_lvGrid.DeleteItem(nSel);

	// Update selection.
	if (nSel == m_lvGrid.ItemCount())
		--nSel;

	m_lvGrid.Select(nSel);
}
