/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		FAVFILES.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CFavFiles class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "FavFiles.hpp"

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

CFavFiles::CFavFiles(CMDB& oDB)
	: CTable(oDB, TXT("FavFiles"))
{
	// Create the table schema.
	AddColumn(TXT("ModName"), MDCT_VARSTR, MOD_NAME_LEN, CColumn::UNIQUE  );
	AddColumn(TXT("ModFile"), MDCT_VARSTR, MOD_FILE_LEN, CColumn::DEFAULTS);
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

CFavFiles::~CFavFiles()
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

CRow& CFavFiles::CreateRow()
{
	CRow& oRow = CTable::CreateRow();

	oRow[MOD_NAME] = TXT("");
	oRow[MOD_FILE] = TXT("");
	
	return oRow;
}
