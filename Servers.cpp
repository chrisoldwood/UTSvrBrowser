/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SERVERS.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CServers class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"

#ifdef _DEBUG
// For memory leak detection.
#define new DBGCRT_NEW
#endif

/******************************************************************************
** Method:		Constructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CServers::CServers(CMDB& oDB)
	: CTable(oDB, "Servers")
{
	// Create the table schema.
	AddColumn("ID",          MDCT_IDENTITY, 0,              CColumn::IDENTITY);
	AddColumn("IPKey",       MDCT_FXDSTR,   IP_KEY_LEN,     CColumn::UNIQUE  );
	AddColumn("IPAddress",   MDCT_FXDSTR,   IP_ADDRESS_LEN, CColumn::DEFAULTS);
	AddColumn("IPPort",      MDCT_INT,      0,              CColumn::DEFAULTS);
	AddColumn("HostName",    MDCT_FXDSTR,   HOST_NAME_LEN,  CColumn::DEFAULTS);
	AddColumn("MapTitle",    MDCT_FXDSTR,   MAP_TITLE_LEN,  CColumn::DEFAULTS);
	AddColumn("MapName",     MDCT_FXDSTR,   MAP_NAME_LEN,   CColumn::DEFAULTS);
	AddColumn("GameType",    MDCT_FXDSTR,   GAME_TYPE_LEN,  CColumn::DEFAULTS);
	AddColumn("NumPlayers",  MDCT_INT,      0,              CColumn::DEFAULTS);
	AddColumn("MaxPlayers",  MDCT_INT,      0,              CColumn::DEFAULTS);
}

/******************************************************************************
** Method:		Destructor.
**
** Description:	.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CServers::~CServers()
{
}

/******************************************************************************
** Method:		CreateRow()
**
** Description:	Creates a new row and initialises it.
**
** Parameters:	None.
**
** Returns:		The new row.
**
*******************************************************************************
*/

CRow& CServers::CreateRow()
{
	CRow& oRow = CTable::CreateRow();

//	oRow[ID]          = 
	oRow[IP_KEY]      = "";
	oRow[IP_ADDRESS]  = "";
	oRow[IP_PORT]     = 0;
	oRow[HOST_NAME]   = "";
	oRow[MAP_TITLE]   = "";
	oRow[MAP_NAME]    = "";
	oRow[GAME_TYPE]   = "";
	oRow[NUM_PLAYERS] = 0;
	oRow[MAX_PLAYERS] = 0;
	
	return oRow;
}
