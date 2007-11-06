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
	: CTable(oDB, "FavFiles")
{
	// Create the table schema.
	AddColumn("ModName", MDCT_VARSTR, MOD_NAME_LEN, CColumn::UNIQUE  );
	AddColumn("ModFile", MDCT_VARSTR, MOD_FILE_LEN, CColumn::DEFAULTS);
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

	oRow[MOD_NAME] = "";
	oRow[MOD_FILE] = "";
	
	return oRow;
}
