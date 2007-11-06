/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		QUERYRESPONSE.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CQueryResponse class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "QueryResponse.hpp"

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

CQueryResponse::CQueryResponse()
{
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

CQueryResponse::~CQueryResponse()
{
}

/******************************************************************************
** Method:		FindField()
**
** Description:	Finds the index of a field in the response.
**
** Parameters:	pszField	The field name.
**
** Returns:		The index or -1 if not found.
**
*******************************************************************************
*/

int CQueryResponse::FindField(const char* pszField) const
{
	ASSERT(pszField != NULL);

	// For all fields.
	for (int i = 0; i < m_astrFields.Size(); ++i)
	{
		if (m_astrFields[i].Compare(pszField, true) == 0)
			return i;
	}

	return -1;
}

/******************************************************************************
** Method:		FieldValue()
**
** Description:	Gets the value for a field, if it is set.
**
** Parameters:	pszField	The field name.
**
** Returns:		The field value or "" if not found.
**
*******************************************************************************
*/

CString CQueryResponse::FieldValue(const char* pszField) const
{
	ASSERT(pszField != NULL);

	int i = FindField(pszField);

	if (i != -1)
		return m_astrValues[i];

	return "";
}

/******************************************************************************
** Method:		Dump()
**
** Description:	Dumps the fields and values to the debug output.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CQueryResponse::Dump()
{
#ifdef _DEBUG
	for (int i = 0; i < m_astrFields.Size(); ++i)
	{
		TRACE2("%s:%s\n", m_astrFields[i], m_astrValues[i]);
	}
#endif
}
