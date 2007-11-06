/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		MASTERQUERYDLG.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CMasterQueryDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef MASTERQUERYDLG_HPP
#define MASTERQUERYDLG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/PropertySheet.hpp>
#include "MtrAddressPage.hpp"
#include "MtrFirewallPage.hpp"
#include "MtrTransformPage.hpp"
#include "MasterQueryOpts.hpp"

/******************************************************************************
** 
** The property sheet for the master server query settings.
**
*******************************************************************************
*/

class CMasterQueryDlg : public CPropertySheet
{
public:
	//
	// Constructors/Destructor.
	//
	CMasterQueryDlg();

	//
	// Members.
	//
	CMasterQueryOpts	m_oConfig;

protected:
	//
	// Pages.
	//
	CMtrAddressPage		m_oAddressPage;
	CMtrFirewallPage	m_oFirewallPage;
	CMtrTransformPage	m_oTranslatePage;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // MASTERQUERYDLG_HPP
