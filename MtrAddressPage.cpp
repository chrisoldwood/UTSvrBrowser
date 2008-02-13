/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		MTRADDRESSPAGE.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CMtrAddressPage class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "MtrAddressPage.hpp"
#include "MasterQueryOpts.hpp"

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

CMtrAddressPage::CMtrAddressPage(CMasterQueryOpts& oConfig)
	: CPropertyPage(IDD_MASTER_ADDRESS)
	, m_oConfig(oConfig)
	, m_ebPort(false, 5)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_ADDRESS,	&m_ebAddress)
		CTRL(IDC_PORT,		&m_ebPort   )
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

void CMtrAddressPage::OnInitDialog()
{
	// Initialise controls.
	m_ebAddress.Text(m_oConfig.m_strAddress);
	m_ebPort.IntValue(m_oConfig.m_nPort);
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

bool CMtrAddressPage::OnValidate()
{
	// Validate new settings.
	if (m_ebAddress.TextLength() == 0)
	{
		AlertMsg(TXT("Please enter the server IP address."));
		m_ebAddress.Focus();
		return false;
	}

	if ( (m_ebPort.TextLength() == 0) || (m_ebPort.IntValue() > 65535) )
	{
		AlertMsg(TXT("Please enter a valid IP port number (0 - 65535)."));
		m_ebPort.Focus();
		return false;
	}

	// Get new settings.
	m_oConfig.m_strAddress = m_ebAddress.Text();
	m_oConfig.m_nPort      = m_ebPort.IntValue();

	return true;
}
