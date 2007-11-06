/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		MTRTRANSFORMPAGE.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CMtrTransformPage class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "MtrTransformPage.hpp"
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

CMtrTransformPage::CMtrTransformPage(CMasterQueryOpts& oConfig)
	: CPropertyPage(IDD_MASTER_TRANSLATE)
	, m_oConfig(oConfig)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_TRIM_WHITESPACE,	&m_ckTrimSpace)
		CTRL(IDC_CONVERT_SYMBOLS,	&m_ckConvSyms)
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

void CMtrTransformPage::OnInitDialog()
{
	// Initialise controls.
	m_ckTrimSpace.Check(m_oConfig.m_bTrimSpace);
	m_ckConvSyms.Check(m_oConfig.m_bConvertSyms);
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

bool CMtrTransformPage::OnValidate()
{
	// Get new settings.
	m_oConfig.m_bTrimSpace   = m_ckTrimSpace.IsChecked();
	m_oConfig.m_bConvertSyms = m_ckConvSyms.IsChecked();

	return true;
}
