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
	CMasterServer(const char* pszAddress, int nPort);
	~CMasterServer();
	
	//
	// Methods.
	//
	int QueryServers(const char* pszGameCode, CStrArray& astrAddresses);

protected:
	//
	// Members.
	//
	CString			m_strAddress;
	int				m_nPort;
	CTCPCltSocket*	m_pSocket;

	//
	// Constants.
	//
	static const char* QUERY_STRING;
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // MASTERSERVER_HPP
