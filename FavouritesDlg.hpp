/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		FAVOURITESDLG.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CFavouritesDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef FAVOURITESDLG_HPP
#define FAVOURITESDLG_HPP

/******************************************************************************
** 
** The dialog used to configure the favourites files.
**
*******************************************************************************
*/

class CFavouritesDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CFavouritesDlg();
	
	//
	// Members.
	//
	CFavFiles m_oFavFiles;
	
protected:
	//
	// Controls.
	//
	CListView	m_lvGrid;

	//
	// Column indices.
	//
	enum Column
	{
		MOD_NAME,
		MOD_FILE,

		NUM_COLUMNS,
	};

	//
	// Message handlers.
	//
	virtual void OnInitDialog();
	virtual bool OnOk();

	void OnDetect();
	void OnAdd();
	void OnEdit();
	void OnRemove();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // FAVOURITESDLG_HPP
