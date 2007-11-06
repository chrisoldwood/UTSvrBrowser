/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		FLTNAMEPAGE.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CFltNamePage class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "FltNamePage.hpp"

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

CFltNamePage::CFltNamePage(CFilter& oFilter)
	: CPropertyPage(IDD_FILTER_NAME)
	, m_oFilter(oFilter)
{
	DEFINE_CTRL_TABLE
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

void CFltNamePage::OnInitDialog()
{
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

bool CFltNamePage::OnValidate()
{
	return true;
}
