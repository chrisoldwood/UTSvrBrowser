/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SERVERS.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CServers class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "Servers.hpp"

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

CServers::CServers()
	: CTable(TXT("Servers"))
{
	// Create the table schema.
	AddColumn(TXT("ID"),          MDCT_IDENTITY, 0,              CColumn::IDENTITY);
	AddColumn(TXT("IPKey"),       MDCT_FXDSTR,   IP_KEY_LEN,     CColumn::UNIQUE  );
	AddColumn(TXT("IPAddress"),   MDCT_FXDSTR,   IP_ADDRESS_LEN, CColumn::DEFAULTS);
	AddColumn(TXT("IPPort"),      MDCT_INT,      0,              CColumn::DEFAULTS);
	AddColumn(TXT("HostName"),    MDCT_VARSTR,   HOST_NAME_LEN,  CColumn::DEFAULTS);
	AddColumn(TXT("MapTitle"),    MDCT_VARSTR,   MAP_TITLE_LEN,  CColumn::DEFAULTS);
	AddColumn(TXT("MapName"),     MDCT_VARSTR,   MAP_NAME_LEN,   CColumn::DEFAULTS);
	AddColumn(TXT("GameType"),    MDCT_VARSTR,   GAME_TYPE_LEN,  CColumn::DEFAULTS);
	AddColumn(TXT("ModName"),     MDCT_VARSTR,   MOD_NAME_LEN,   CColumn::DEFAULTS);
	AddColumn(TXT("NumPlayers"),  MDCT_INT,      0,              CColumn::DEFAULTS);
	AddColumn(TXT("MaxPlayers"),  MDCT_INT,      0,              CColumn::DEFAULTS);
	AddColumn(TXT("PingTime"),    MDCT_INT,      0,              CColumn::DEFAULTS);
	AddColumn(TXT("LastError"),   MDCT_INT,      0,              CColumn::DEFAULTS);
	AddColumn(TXT("FavID"),       MDCT_INT,      0,              CColumn::NULLABLE);
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
	oRow[IP_KEY]      = TXT("");
	oRow[IP_ADDRESS]  = TXT("");
	oRow[IP_PORT]     = 0;
	oRow[HOST_NAME]   = TXT("");
	oRow[MAP_TITLE]   = TXT("");
	oRow[MAP_NAME]    = TXT("");
	oRow[GAME_TYPE]   = TXT("");
	oRow[MOD_NAME]    = TXT("");
	oRow[NUM_PLAYERS] = 0;
	oRow[MAX_PLAYERS] = 0;
	oRow[PING_TIME]   = 999;
	oRow[LAST_ERROR]  = ERROR_NONE;
	oRow[FAV_ID]      = null;
	
	return oRow;
}
