/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		FLTMODSPAGE.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CFltModsPage class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "FltModsPage.hpp"

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

CFltModsPage::CFltModsPage(CFilter& oFilter)
	: CPropertyPage(IDD_FILTER_MODS)
	, m_oFilter(oFilter)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_FILTER_MODS, &m_ckFltMods)
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

void CFltModsPage::OnInitDialog()
{
	m_ckFltMods.Check(false);
}

/******************************************************************************
** Method:		OnValidate()
**
** Description:	Validate the settings.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CFltModsPage::OnValidate()
{
	m_ckFltMods.IsChecked();

	return true;
}
