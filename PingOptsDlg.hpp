/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PINGOPTSDLG.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CPingOptsDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef PINGOPTSDLG_HPP
#define PINGOPTSDLG_HPP

/******************************************************************************
** 
** The ping options dialog.
**
*******************************************************************************
*/

class CPingOptsDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CPingOptsDlg();
	
	//
	// Members.
	//
	CPingOpts	m_oConfig;

protected:
	//
	// Controls.
	//
	CDecimalBox	m_ebThreads;
	CDecimalBox	m_ebAttempts;
	CDecimalBox	m_ebTimeout;
	CCheckBox	m_ckAutoPing;
	CDecimalBox	m_ebInterval;

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

#endif // PINGOPTSDLG_HPP
