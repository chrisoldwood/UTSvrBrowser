/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		FILTERDLG.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CFilterDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef FILTERDLG_HPP
#define FILTERDLG_HPP

#include "FltNamePage.hpp"
#include "FltModsPage.hpp"
#include "FltPortsPage.hpp"
//#include "FltMiscPage.hpp"

/******************************************************************************
** 
** The property sheet used to configure a filter..
**
*******************************************************************************
*/

class CFilterDlg : public CPropertySheet
{
public:
	//
	// Constructors/Destructor.
	//
	CFilterDlg(CFilter& oFilter);
	
protected:
	//
	// Members.
	//
	
	//
	// Pages.
	//
	CFltNamePage	m_oNamePage;
	CFltModsPage	m_oModsPage;
	CFltPortsPage	m_oPortsPage;
//	CFltMiscPage	m_oMiscPage;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // FILTERDLG_HPP
