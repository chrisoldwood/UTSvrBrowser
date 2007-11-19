/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PROGRESSDLG.CPP
** COMPONENT:	The Application.
** DESCRIPTION:	CProgressDlg class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "ProgressDlg.hpp"
#include "UTSvrBrowser.hpp"

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

CProgressDlg::CProgressDlg()
	: CDialog(IDD_PROGRESS)
	, m_bAbort(false)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_OPERATION,	&m_txtLabel)
		CTRL(IDC_PROGRESS,	&m_barMeter)
	END_CTRL_TABLE

	DEFINE_CTRLMSG_TABLE
		CMD_CTRLMSG(IDC_ABORT, BN_CLICKED, &CProgressDlg::OnAbort)
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

void CProgressDlg::OnInitDialog()
{
	// Display it.
	Centre();
	Show();
}

/******************************************************************************
** Method:		InitMeter()
**
** Description:	Initialise the progress bar.
**
** Parameters:	nSteps		The maximum number of steps.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CProgressDlg::InitMeter(uint nSteps)
{
	m_barMeter.Range(0, nSteps);
}

/******************************************************************************
** Method:		Update*()
**
** Description:	Updates the label and/or progress bar.
**
** Parameters:	pszLabel	The new label.
**				nSteps		The number of steps done.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CProgressDlg::UpdateLabel(const char* pszLabel)
{
	ASSERT(pszLabel != NULL);

	m_txtLabel.Text(pszLabel);

	App.m_MainThread.ProcessMsgQueue();
}

void CProgressDlg::UpdateMeter(uint nSteps)
{
	m_barMeter.Pos(nSteps);

	App.m_MainThread.ProcessMsgQueue();
}

void CProgressDlg::UpdateLabelAndMeter(const char* pszLabel, uint nSteps)
{
	ASSERT(pszLabel != NULL);

	m_txtLabel.Text(pszLabel);
	m_barMeter.Pos(nSteps);

	App.m_MainThread.ProcessMsgQueue();
}

/******************************************************************************
** Method:		OnAbort()
**
** Description:	Abort button pressed.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CProgressDlg::OnAbort()
{
	m_bAbort = true;
}
