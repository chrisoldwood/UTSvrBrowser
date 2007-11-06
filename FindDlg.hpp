/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		FINDDLG.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CFindDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef FINDDLG_HPP
#define FINDDLG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>

/******************************************************************************
** 
** The dialog used to enter the string to find.
**
*******************************************************************************
*/

class CFindDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CFindDlg();
	
	//
	// Members.
	//
	CString m_strText;
	
protected:
	//
	// Controls.
	//
	CEditBox	m_ebText;

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

#endif // FINDDLG_HPP
