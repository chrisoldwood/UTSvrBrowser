/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		FLTMISCPAGE.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CFltMiscPage class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "FltMiscPage.hpp"

#ifdef _DEBUG
// For memory leak detection.
#define new DBGCRT_NEW
#endif

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

CFltMiscPage::CFltMiscPage(CFilter& oFilter)
	: CPropertyPage(IDD_FILTER_MISC)
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

void CFltMiscPage::OnInitDialog()
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

bool CFltMiscPage::OnValidate()
{
	return true;
}
