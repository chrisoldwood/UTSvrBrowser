/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		FINDDLG.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CFindDlg class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "FindDlg.hpp"

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

CFindDlg::CFindDlg()
	: CDialog(IDD_FIND)
{
	DEFINE_CTRL_TABLE
		CTRL(IDC_TEXT,	&m_ebText)
	END_CTRL_TABLE

//	DEFINE_CTRLMSG_TABLE
//	END_CTRLMSG_TABLE
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

void CFindDlg::OnInitDialog()
{
	// Initialise controls.
	m_ebText.Text(m_strText);
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

bool CFindDlg::OnOk()
{
	// Validate settings.
	if (m_ebText.TextLength() == 0)
	{
		AlertMsg("Please enter the text to find.");
		m_ebText.Focus();
		return false;
	}

	// Get new settings.
	m_strText = m_ebText.Text();

	return true;
}
