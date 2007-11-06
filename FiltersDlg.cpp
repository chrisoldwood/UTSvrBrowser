/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		FILTERSDLG.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CFiltersDlg class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "FiltersDlg.hpp"
#include "FilterDlg.hpp"
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

CFiltersDlg::CFiltersDlg()
	: CDialog(IDD_FILTERS)
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

void CFiltersDlg::OnInitDialog()
{
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

bool CFiltersDlg::OnOk()
{
	CFilter    oFilter;
	CFilterDlg Dlg(oFilter);

	if (Dlg.RunModal(*this) == IDOK)
	{
		App.m_bFltEdited = true;
	}

	return true;
}
