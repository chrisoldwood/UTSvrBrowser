/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		FILTERDLG.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CFilterDlg class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "FilterDlg.hpp"

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

CFilterDlg::CFilterDlg(CFilter& oFilter)
	: CPropertySheet()
	, m_oNamePage(oFilter)
	, m_oModsPage(oFilter)
	, m_oPortsPage(oFilter)
//	, m_oMiscPage(oFilter)
{
	// Set the title.
	m_strTitle = TXT("Filter Settings");

	DEFINE_PAGE_TABLE
		PAGE(&m_oNamePage,  TXT("Name")   )
		PAGE(&m_oModsPage,  TXT("Mods")   )
		PAGE(&m_oPortsPage, TXT("Network"))
//		PAGE(&m_oMiscPage,  TXT("Other")  )
	END_PAGE_TABLE
}
