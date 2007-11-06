/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		MODS.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CMods class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef MODS_HPP
#define MODS_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <MDBL/Table.hpp>

/******************************************************************************
** 
** The table used to store the list of known mods.
**
*******************************************************************************
*/

class CMods : public CTable
{
public:
	//
	// Constructors/Destructor.
	//
	CMods(CMDB& oDB);
	~CMods();
	
	//
	// Column indices.
	//
	enum
	{
		ID,
		MOD_NAME,
		FAVS_FILE,

		NUM_COLUMNS,
	};

	//
	// Column lengths.
	//
	enum
	{
		MOD_NAME_LEN  = MAX_PATH,
		FAVS_FILE_LEN = MAX_PATH,
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

#endif // MODS_HPP
