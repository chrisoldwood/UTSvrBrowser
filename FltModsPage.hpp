/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		FLTMODSPAGE.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CFltModsPage class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef FLTMODSPAGE_HPP
#define FLTMODSPAGE_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>

// Forward declarations.
class CFilter;

/******************************************************************************
** 
** The filter property page used to configure the mods.
**
*******************************************************************************
*/

class CFltModsPage : public CPropertyPage
{
public:
	//
	// Constructors/Destructor.
	//
	CFltModsPage(CFilter& oFilter);
	
protected:
	//
	// Members.
	//
	CFilter& m_oFilter;
	
	//
	// Controls.
	//
	CCheckBox	m_ckFltMods;

	//
	// Message handlers.
	//
	virtual void OnInitDialog();
	virtual bool OnValidate();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // FLTMODSPAGE_HPP
