/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SERVERS.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CServers class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef SERVERS_HPP
#define SERVERS_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <MDBL/Table.hpp>

/******************************************************************************
** 
** The table used to store the server list.
**
*******************************************************************************
*/

class CServers : public CTable
{
public:
	//
	// Constructors/Destructor.
	//
	CServers();
	~CServers();
	
	//
	// Column indices.
	//
	enum
	{
		ID,
		IP_KEY,
		IP_ADDRESS,
		IP_PORT,
		HOST_NAME,
		MAP_TITLE,
		MAP_NAME,
		GAME_TYPE,
		MOD_NAME,
		NUM_PLAYERS,
		MAX_PLAYERS,
		PING_TIME,
		LAST_ERROR,
		FAV_ID,

		NUM_COLUMNS,
	};

	//
	// Column lengths.
	//
	enum
	{
		IP_KEY_LEN     = 30,
		IP_ADDRESS_LEN = 20,
		HOST_NAME_LEN  = MAX_PATH,
		MAP_TITLE_LEN  = MAX_PATH,
		MAP_NAME_LEN   = MAX_PATH,
		GAME_TYPE_LEN  = MAX_PATH,
		MOD_NAME_LEN   = MAX_PATH,
	};

	//
	// Methods (overriden).
	//
	virtual CRow& CreateRow();

	//
	// Status codes.
	//
	enum
	{
		ERROR_NONE,
		ERROR_FAILED,
		ERROR_TIMED_OUT,
	};
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // SERVERS_HPP
