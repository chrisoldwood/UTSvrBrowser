/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		GAMETYPES.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CGameTypes class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "GameTypes.hpp"

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

CGameTypes::CGameTypes(CMDB& oDB)
	: CTable(oDB, "GameTypes", TRANSIENT)
{
	// Create the table schema.
	AddColumn("ID",       MDCT_IDENTITY, 0,             CColumn::IDENTITY);
	AddColumn("GameType", MDCT_VARSTR,   GAME_TYPE_LEN, CColumn::UNIQUE  );
	AddColumn("ModName",  MDCT_VARSTR,   MOD_NAME_LEN,  CColumn::DEFAULTS);
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

CGameTypes::~CGameTypes()
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

CRow& CGameTypes::CreateRow()
{
	CRow& oRow = CTable::CreateRow();

//	oRow[ID]        = 
	oRow[GAME_TYPE] = "";
	oRow[MOD_NAME]  = "";
	
	return oRow;
}
