/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		FLTPORTSPAGE.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CFltPortsPage class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "FltPortsPage.hpp"

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

CFltPortsPage::CFltPortsPage(CFilter& oFilter)
	: CPropertyPage(IDD_FILTER_NETWORK)
	, m_oFilter(oFilter)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_FILTER_ERRORS, &m_ckFltErrors)
		CTRL(IDC_FILTER_PINGS, &m_ckFltPings)
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

void CFltPortsPage::OnInitDialog()
{
	m_ckFltErrors.Check(true);
	m_ckFltPings.Check(false);
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

bool CFltPortsPage::OnValidate()
{
	m_ckFltErrors.IsChecked();
	m_ckFltPings.IsChecked();

	return true;
}
