/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		MASTERSERVER.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CMasterServer class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef MASTERSERVER_HPP
#define MASTERSERVER_HPP

#if _MSC_VER > 1000
#pragma once
#endif

/******************************************************************************
** 
** A master server which holds the list of running games.
**
*******************************************************************************
*/

class CMasterServer
{
public:
	//
	// Constructors/Destructor.
	//
	CMasterServer(const tchar* pszAddress, int nPort);
	~CMasterServer();
	
	//
	// Methods.
	//
	int QueryServers(const tchar* pszGameCode, CStrArray& astrAddresses);

protected:
	//
	// Members.
	//
	CString	m_strAddress;
	int		m_nPort;

	//
	// Constants.
	//
	static const char*  QUERY_STRING;
	static const tchar* END_OF_RESPONSE;
	static const tchar* FIELD_SEPS;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // MASTERSERVER_HPP
