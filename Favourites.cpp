/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		FAVOURITES.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CFavourites class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "Favourites.hpp"

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

CFavourites::CFavourites(CMDB& oDB)
	: CTable(oDB, TXT("Favourites"), TRANSIENT)
{
	// Create the table schema.
	AddColumn(TXT("IPKey"),   MDCT_FXDSTR, IP_KEY_LEN,   CColumn::UNIQUE  );
	AddColumn(TXT("ModName"), MDCT_VARSTR, MOD_NAME_LEN, CColumn::DEFAULTS);
	AddColumn(TXT("FavID"),   MDCT_INT,    0,            CColumn::DEFAULTS);
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

CFavourites::~CFavourites()
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

CRow& CFavourites::CreateRow()
{
	CRow& oRow = CTable::CreateRow();

	oRow[IP_KEY]   = TXT("");
	oRow[MOD_NAME] = TXT("");
	oRow[FAV_ID]   = 0;
	
	return oRow;
}
