/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SUMMARY.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CSummary class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef SUMMARY_HPP
#define SUMMARY_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <MDBL/Table.hpp>
#include "Mods.hpp"

/******************************************************************************
** 
** The table used to hold the server summary.
**
*******************************************************************************
*/

class CSummary : public CTable
{
public:
	//
	// Constructors/Destructor.
	//
	CSummary(CMDB& oDB);
	~CSummary();
	
	//
	// Column indices.
	//
	enum
	{
		MOD_NAME,
		NUM_SERVERS,
		NUM_PLAYERS,

		NUM_COLUMNS,
	};

	//
	// Column lengths.
	//
	enum
	{
		MOD_NAME_LEN = CMods::MOD_NAME_LEN,
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

#endif // SUMMARY_HPP
