/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		FLTPORTSPAGE.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CFltPortsPage class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef FLTPORTSPAGE_HPP
#define FLTPORTSPAGE_HPP

/******************************************************************************
** 
** The filter property page used to configure the port range.
**
*******************************************************************************
*/

class CFltPortsPage : public CPropertyPage
{
public:
	//
	// Constructors/Destructor.
	//
	CFltPortsPage(CFilter& oFilter);
	
protected:
	//
	// Members.
	//
	CFilter& m_oFilter;

	//
	// Controls.
	//
	CCheckBox	m_ckFltErrors;
	CCheckBox	m_ckFltPings;

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

#endif // FLTPORTSPAGE_HPP
