/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		MTRFIREWALLPAGE.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CMtrFirewallPage class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "MtrFirewallPage.hpp"

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

CMtrFirewallPage::CMtrFirewallPage(CMasterQueryOpts& oConfig)
	: CPropertyPage(IDD_MASTER_FIREWALL)
	, m_oConfig(oConfig)
	, m_ebFirstPort(false, 5)
	, m_ebLastPort(false, 5)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_MASTER_PORTS,	&m_ckFirewall)
		CTRL(IDC_FIRST_PORT,	&m_ebFirstPort)
		CTRL(IDC_LAST_PORT,		&m_ebLastPort)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_MASTER_PORTS, BN_CLICKED, OnClickFirewall)
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

void CMtrFirewallPage::OnInitDialog()
{
	// Initialise controls.
	m_ckFirewall.Check(m_oConfig.m_bFirewall);
	m_ebFirstPort.IntValue(m_oConfig.m_nFirstPort);
	m_ebLastPort.IntValue(m_oConfig.m_nLastPort);

	OnClickFirewall();
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

bool CMtrFirewallPage::OnValidate()
{
	if ( (m_ebFirstPort.TextLength() == 0) || (m_ebFirstPort.IntValue() > 65535) )
	{
		AlertMsg("Please enter a valid 'from' IP port number (0 - 65535).");
		m_ebFirstPort.Focus();
		return false;
	}

	if ( (m_ebLastPort.TextLength() == 0) || (m_ebLastPort.IntValue() > 65535) )
	{
		AlertMsg("Please enter a valid 'to' IP port number (0 - 65535).");
		m_ebLastPort.Focus();
		return false;
	}

	if (m_ebLastPort.IntValue() < m_ebFirstPort.IntValue())
	{
		AlertMsg("The second port number should be greater than the first.");
		m_ebLastPort.Focus();
		return false;
	}

	// Get new settings.
	m_oConfig.m_bFirewall  = m_ckFirewall.IsChecked();
	m_oConfig.m_nFirstPort = m_ebFirstPort.IntValue();
	m_oConfig.m_nLastPort  = m_ebLastPort.IntValue();

	return true;
}

/******************************************************************************
** Method:		OnClickFirewall()
**
** Description:	Enable/Disable the firewall port range edit boxes.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CMtrFirewallPage::OnClickFirewall()
{
	bool bFirewall = m_ckFirewall.IsChecked();

	m_ebFirstPort.Enable(bFirewall);
	m_ebLastPort.Enable(bFirewall);
}
