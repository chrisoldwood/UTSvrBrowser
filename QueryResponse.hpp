/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		QUERYRESPONSE.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CQueryResponse class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef QUERYRESPONSE_HPP
#define QUERYRESPONSE_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/StrArray.hpp>

/******************************************************************************
** 
** The response from a query sent to a master/game server.
**
*******************************************************************************
*/

class CQueryResponse
{
public:
	//
	// Constructors/Destructor.
	//
	CQueryResponse();
	~CQueryResponse();
	
	//
	// Accessors.
	//
	int Error()  const;
	int Time()   const; 
	int Fields() const;

	const CString& FieldName(int nIndex) const;
	const CString& FieldValue(int nIndex) const;

	int     FindField(const tchar* pszField) const;
	CString FieldValue(const tchar* pszField) const;

	//
	// Debug methods.
	//
	void Dump();

private:
	//
	// Members.
	//
	int			m_nError;		// The error code.
	int			m_nTime;		// The time taken to respond.
	CStrArray	m_astrFields;	// The response field names.
	CStrArray	m_astrValues;	// The response field values.

	// Friends.
	friend class CMasterServer;
	friend class CGameServer;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline int CQueryResponse::Error() const
{
	return m_nError;
}

inline int CQueryResponse::Time() const
{
	return m_nTime;
}

inline int CQueryResponse::Fields() const
{
	return m_astrFields.Size();
}

inline const CString& CQueryResponse::FieldName(int nIndex) const
{
	return m_astrFields[nIndex];
}

inline const CString& CQueryResponse::FieldValue(int nIndex) const
{
	return m_astrValues[nIndex];
}

#endif // QUERYRESPONSE_HPP
