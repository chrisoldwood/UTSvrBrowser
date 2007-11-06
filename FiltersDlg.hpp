/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		FILTERSDLG.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CFiltersDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef FILTERSDLG_HPP
#define FILTERSDLG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>

/******************************************************************************
** 
** .
**
*******************************************************************************
*/

class CFiltersDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CFiltersDlg();
	
protected:
	//
	// Members.
	//
	
	//
	// Controls.
	//

	//
	// Message handlers.
	//
	virtual void OnInitDialog();
	virtual bool OnOk();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // FILTERSDLG_HPP
