/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		FLTNAMEPAGE.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CFltNamePage class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef FLTNAMEPAGE_HPP
#define FLTNAMEPAGE_HPP

/******************************************************************************
** 
** The filter property page used to configure the filter name.
**
*******************************************************************************
*/

class CFltNamePage : public CPropertyPage
{
public:
	//
	// Constructors/Destructor.
	//
	CFltNamePage(CFilter& oFilter);
	
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

#endif // FLTNAMEPAGE_HPP
