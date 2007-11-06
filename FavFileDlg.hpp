/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		FAVFILEDLG.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CFavFileDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef FAVFILEDLG_HPP
#define FAVFILEDLG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>
#include <WCL/Path.hpp>

// Foward declarations.
class CFavFiles;

/******************************************************************************
** 
** The dialog used to edit the settings for a mods favourites file.
**
*******************************************************************************
*/

class CFavFileDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CFavFileDlg();
	
	//
	// Members.
	//
	CFavFiles*	m_pFavFiles;
	CString		m_strMod;
	CPath		m_strFile;
	
protected:
	//
	// Controls.
	//
	CComboBox	m_cbMod;
	CEditBox	m_ebFile;

	//
	// Message handlers.
	//
	virtual void OnInitDialog();
	virtual bool OnOk();

	void OnBrowse();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // FAVFILEDLG_HPP
