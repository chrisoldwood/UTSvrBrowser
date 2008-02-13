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
	m_strTitle = TXT("Master Query Settings");

	DEFINE_PAGE_TABLE
		PAGE(&m_oAddressPage,   TXT("Server")     )
		PAGE(&m_oFirewallPage,  TXT("Firewall")   )
		PAGE(&m_oTranslatePage, TXT("Translation"))
	END_PAGE_TABLE
}
