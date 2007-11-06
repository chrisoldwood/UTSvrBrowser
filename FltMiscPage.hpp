/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		FLTMISCPAGE.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CFltMiscPage class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef FLTMISCPAGE_HPP
#define FLTMISCPAGE_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>

// Forward declarations.
class CFilter;

/******************************************************************************
** 
** The filter property page used to configure the misc settings.
**
*******************************************************************************
*/

class CFltMiscPage : public CPropertyPage
{
public:
	//
	// Constructors/Destructor.
	//
	CFltMiscPage(CFilter& oFilter);
	
protected:
	//
	// Members.
	//
	CFilter& m_oFilter;
	
	//
	// Controls.
	//

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

#endif // FLTMISCPAGE_HPP
