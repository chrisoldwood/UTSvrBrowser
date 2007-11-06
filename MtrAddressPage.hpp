/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		MTRADDRESSPAGE.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CMtrAddressPage class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef MTRADDRESSPAGE_HPP
#define MTRADDRESSPAGE_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>

// Forward declarations.
class CMasterQueryOpts;

/******************************************************************************
** 
** The property page for the master server address and port.
**
*******************************************************************************
*/

class CMtrAddressPage : public CPropertyPage
{
public:
	//
	// Constructors/Destructor.
	//
	CMtrAddressPage(CMasterQueryOpts& oConfig);
	
protected:
	//
	// Members.
	//
	CMasterQueryOpts&	m_oConfig;
	
	//
	// Controls.
	//
	CEditBox	m_ebAddress;
	CDecimalBox	m_ebPort;

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

#endif // MTRADDRESSPAGE_HPP
