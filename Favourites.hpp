/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		FAVOURITES.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CFavourites class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef FAVOURITES_HPP
#define FAVOURITES_HPP

/******************************************************************************
** 
** The table used to store the favourites.
**
*******************************************************************************
*/

class CFavourites : public CTable
{
public:
	//
	// Constructors/Destructor.
	//
	CFavourites(CMDB& oDB);
	~CFavourites();
	
	//
	// Column indices.
	//
	enum
	{
		IP_KEY,
		MOD_NAME,
		FAV_ID,

		NUM_COLUMNS,
	};

	//
	// Column lengths.
	//
	enum
	{
		IP_KEY_LEN   = 30,
		MOD_NAME_LEN = MAX_PATH,
	};

	//
	// Methods (overriden).
	//
	virtual CRow& CreateRow();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // FAVOURITES_HPP
