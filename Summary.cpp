/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SUMMARY.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CSummary class definition.
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

CSummary::CSummary(CMDB& oDB)
	: CTable(oDB, "Summary", TRANSIENT)
{
	// Create the table schema.
	AddColumn("ModName",     MDCT_VARSTR, MOD_NAME_LEN, CColumn::UNIQUE  );
	AddColumn("NumServers",  MDCT_INT,    0,            CColumn::DEFAULTS);
	AddColumn("NumPlayers",  MDCT_INT,    0,            CColumn::DEFAULTS);
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

CSummary::~CSummary()
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

CRow& CSummary::CreateRow()
{
	CRow& oRow = CTable::CreateRow();

//	oRow[ID]          = 
	oRow[MOD_NAME]    = "";
	oRow[NUM_SERVERS] = 0;
	oRow[NUM_PLAYERS] = 0;
	
	return oRow;
}
