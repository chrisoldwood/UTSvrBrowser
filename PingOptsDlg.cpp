/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PINGOPTSDLG.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CPingOptsDlg class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"
#include "PingOptsDlg.hpp"

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

CPingOptsDlg::CPingOptsDlg()
	: CDialog(IDD_PING_OPTIONS)
	, m_ebThreads(false, 2)
	, m_ebAttempts(false, 2)
	, m_ebTimeout(false, 4)
	, m_ebInterval(false, 4)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_THREADS,   &m_ebThreads )
		CTRL(IDC_ATTEMPTS,  &m_ebAttempts)
		CTRL(IDC_TIMEOUT,   &m_ebTimeout )
		CTRL(IDC_AUTO_PING, &m_ckAutoPing)
		CTRL(IDC_INTERVAL,  &m_ebInterval)
	END_CTRL_TABLE
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

void CPingOptsDlg::OnInitDialog()
{
	// Initialise controls
	m_ebThreads.IntValue(m_oConfig.m_nThreads);
	m_ebAttempts.IntValue(m_oConfig.m_nAttempts);
	m_ebTimeout.IntValue(m_oConfig.m_nWaitTime);
	m_ckAutoPing.Check(m_oConfig.m_bAutoPing);
	m_ebInterval.IntValue(m_oConfig.m_nAutoInterval);
}

/******************************************************************************
** Method:		OnOk()
**
** Description:	The OK button was pressed.
**
** Parameters:	None.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CPingOptsDlg::OnOk()
{
	// Validate new settings.
	if ( (m_ebThreads.TextLength() == 0) || (m_ebThreads.IntValue() == 0) )
	{
		AlertMsg("Please enter a valid thread count (1 - 99).");
		m_ebThreads.Focus();
		return false;
	}

	if ( (m_ebAttempts.TextLength() == 0) || (m_ebAttempts.IntValue() == 0) )
	{
		AlertMsg("Please enter a valid number of attempts (1 - 99).");
		m_ebAttempts.Focus();
		return false;
	}

	if ( (m_ebTimeout.TextLength() == 0) || (m_ebTimeout.IntValue() == 0) )
	{
		AlertMsg("Please enter a valid time out (1 - 9999).");
		m_ebTimeout.Focus();
		return false;
	}

	if ( (m_ebInterval.TextLength() == 0) || (m_ebInterval.IntValue() == 0) )
	{
		AlertMsg("Please enter a valid interval (1 - 9999).");
		m_ebInterval.Focus();
		return false;
	}

	// Get new settings.
	m_oConfig.m_nThreads      = m_ebThreads.IntValue();
	m_oConfig.m_nAttempts     = m_ebAttempts.IntValue();
	m_oConfig.m_nWaitTime     = m_ebTimeout.IntValue();
	m_oConfig.m_bAutoPing     = m_ckAutoPing.IsChecked();
	m_oConfig.m_nAutoInterval = m_ebInterval.IntValue();

	return true;
}
