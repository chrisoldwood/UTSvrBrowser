/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		MASTERSERVER.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CMasterServer class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"

#ifdef _DEBUG
// For memory leak detection.
#define new DBGCRT_NEW
#endif

/******************************************************************************
**
** Constants.
**
*******************************************************************************
*/
const char* CMasterServer::QUERY_STRING = "\\gamename\\ut\\location\\0\\validate\\2/TYFMRc\\final\\\\list\\\\gamename\\ut\\final\\";

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

CMasterServer::CMasterServer(const char* pszAddress, int nPort)
	: m_strAddress(pszAddress)
	, m_nPort(nPort)
	, m_pSocket(new CTCPCltSocket)
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

CMasterServer::~CMasterServer()
{
	delete m_pSocket;
}

/******************************************************************************
** Method:		QueryServers()
**
** Description:	Query the master server for the list of running game servers.
**
** Parameters:	pszGameCode		The games query code.
**				astrAddresses	The list of addresses as "Address:Port"
**
** Returns:		The number of servers found.
**
*******************************************************************************
*/

int CMasterServer::QueryServers(const char* pszGameCode, CStrArray& astrAddresses)
{
	ASSERT(pszGameCode != NULL);

	CString strResponse;

	try
	{
		// Open a socket to the server.
		m_pSocket->Connect(m_strAddress, m_nPort);

		// Send query.
		m_pSocket->Send(QUERY_STRING);

		// Until we find the packet terminator.
		while (strResponse.Find("\\final\\") == -1)
		{
			int nAvail, nRead;

			// Anything to read?
			if ((nAvail = m_pSocket->Available()) > 0)
			{
				CBuffer oBuffer(nAvail);

				// Anything read?
				if ((nRead = m_pSocket->Recv(oBuffer)) > 0)
				{
					// Append to response buffer.
					strResponse += oBuffer.ToString(nRead);
				}
			}

			::Sleep(1);
		}
	}
	catch (CSocketException& e)
	{
		TRACE1("SocketException: %s\n", e.ErrorText());
	}

	// Close session.
	m_pSocket->Close();

	// Split response buffer into separate fields.
	if (strResponse.Split("\\", astrAddresses) > 0)
	{
		// Strip fields which don't contain an IP address.
		for (int i = astrAddresses.Size()-1; i >= 0; --i)
		{
			if (astrAddresses[i].Find(':') == -1)
				astrAddresses.Delete(i);
		} 
	}

	return astrAddresses.Size();
}
