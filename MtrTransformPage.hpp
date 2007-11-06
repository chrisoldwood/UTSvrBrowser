/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		MTRTRANSFORMPAGE.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CMtrTransformPage class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef MTRTRANSFORMPAGE_HPP
#define MTRTRANSFORMPAGE_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>

// Forward declarations.
class CMasterQueryOpts;

/******************************************************************************
** 
** The property page for the master server query translation settings.
**
*******************************************************************************
*/

class CMtrTransformPage : public CPropertyPage
{
public:
	//
	// Constructors/Destructor.
	//
	CMtrTransformPage(CMasterQueryOpts& oConfig);
	
protected:
	//
	// Members.
	//
	CMasterQueryOpts&	m_oConfig;
	
	//
	// Controls.
	//
	CCheckBox	m_ckTrimSpace;
	CCheckBox	m_ckConvSyms;

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

#endif // MTRTRANSFORMPAGE_HPP
