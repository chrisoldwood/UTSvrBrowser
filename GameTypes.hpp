/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		GAMETYPES.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CGameTypes class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef GAMETYPES_HPP
#define GAMETYPES_HPP

/******************************************************************************
** 
** The table used to store the list of known game types.
**
*******************************************************************************
*/

class CGameTypes : public CTable
{
public:
	//
	// Constructors/Destructor.
	//
	CGameTypes(CMDB& oDB);
	~CGameTypes();
	
	//
	// Column indices.
	//
	enum
	{
		ID,
		GAME_TYPE,
		MOD_NAME,

		NUM_COLUMNS,
	};

	//
	// Column lengths.
	//
	enum
	{
		GAME_TYPE_LEN = MAX_PATH,
		MOD_NAME_LEN  = MAX_PATH,
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

#endif // GAMETYPES_HPP
