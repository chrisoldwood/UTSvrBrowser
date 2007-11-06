/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		MODS.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CMods class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "Mods.hpp"

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

CMods::CMods(CMDB& oDB)
	: CTable(oDB, "Mods", TRANSIENT)
{
	// Create the table schema.
	AddColumn("ID",       MDCT_IDENTITY, 0,             CColumn::IDENTITY);
	AddColumn("ModName",  MDCT_VARSTR,   MOD_NAME_LEN,  CColumn::UNIQUE  );
	AddColumn("FavsFile", MDCT_VARSTR,   FAVS_FILE_LEN, CColumn::DEFAULTS);
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

CMods::~CMods()
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

CRow& CMods::CreateRow()
{
	CRow& oRow = CTable::CreateRow();

//	oRow[ID]        = 
	oRow[MOD_NAME]  = "";
	oRow[FAVS_FILE] = "";
	
	return oRow;
}
