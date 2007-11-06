/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		MASTERQUERYDLG.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CMasterQueryDlg class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "MasterQueryDlg.hpp"

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

CMasterQueryDlg::CMasterQueryDlg()
	: CPropertySheet()
	, m_oAddressPage(m_oConfig)
	, m_oFirewallPage(m_oConfig)
	, m_oTranslatePage(m_oConfig)
{
	// Set the title.
	m_strTitle = "Master Query Settings";

	DEFINE_PAGE_TABLE
		PAGE(&m_oAddressPage,   "Server"     )
		PAGE(&m_oFirewallPage,  "Firewall"   )
		PAGE(&m_oTranslatePage, "Translation")
	END_PAGE_TABLE
}
