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
