/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		FAVFILES.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CFavFiles class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef FAVFILES_HPP
#define FAVFILES_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <MDBL/Table.hpp>

/******************************************************************************
** 
** The table used to store the favourites .ini files.
**
*******************************************************************************
*/

class CFavFiles : public CTable
{
public:
	//
	// Constructors/Destructor.
	//
	CFavFiles();
	~CFavFiles();
	
	//
	// Column indices.
	//
	enum
	{
		MOD_NAME,
		MOD_FILE,

		NUM_COLUMNS,
	};

	//
	// Column lengths.
	//
	enum
	{
		MOD_NAME_LEN = MAX_PATH,
		MOD_FILE_LEN = MAX_PATH,
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

#endif // FAVFILES_HPP
