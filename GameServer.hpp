/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		GAMESERVER.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CGameServer class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef GAMESERVER_HPP
#define GAMESERVER_HPP

#if _MSC_VER > 1000
#pragma once
#endif

// Forward declarations.
class CQueryResponse;

/******************************************************************************
** 
** A game server which is running a game.
**
*******************************************************************************
*/

class CGameServer
{
public:
	//
	// Constructors/Destructor.
	//
	CGameServer(const tchar* pszAddress, int nPort);
	~CGameServer();
	
	//
	// Methods.
	//
	bool QueryInfo(CQueryResponse& oResponse);
	bool QueryStatus(CQueryResponse& oResponse);

protected:
	//
	// Members.
	//
	CString	m_strAddress;
	int		m_nPort;

	//
	// Constants.
	//
	static const char*  QUERY_INFO;
	static const char*  QUERY_STATUS;
	static const tchar* END_OF_RESPONSE;
	static const tchar* FIELD_SEPS;

	//
	// Internal methods.
	//
	bool ExecQuery(const char* pszQuery, CQueryResponse& oResponse);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // GAMESERVER_HPP
