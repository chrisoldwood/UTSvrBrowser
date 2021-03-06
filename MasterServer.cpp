/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		MASTERSERVER.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CMasterServer class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "MasterServer.hpp"
#include <NCL/TCPCltSocket.hpp>
#include <WCL/StrTok.hpp>
#include <WCL/StrArray.hpp>
#include <Core/AnsiWide.hpp>

/******************************************************************************
**
** Constants.
**
*******************************************************************************
*/

const char*  CMasterServer::QUERY_STRING    = "\\gamename\\ut\\location\\0\\validate\\2/TYFMRc\\final\\\\list\\\\gamename\\ut\\final\\";
const tchar* CMasterServer::END_OF_RESPONSE = TXT("\\final\\");
const tchar* CMasterServer::FIELD_SEPS      = TXT("\\");

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

CMasterServer::CMasterServer(const tchar* pszAddress, int nPort)
	: m_strAddress(pszAddress)
	, m_nPort(nPort)
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
** Exceptions:	CSocketException.
**
*******************************************************************************
*/

int CMasterServer::QueryServers(const tchar* /*pszGameCode*/, CStrArray& astrAddresses)
{
//	ASSERT(pszGameCode != nullptr);

	CTCPCltSocket oSocket;
	CString       strResponse;

	// Open a socket to the server.
	oSocket.Connect(m_strAddress, m_nPort);

	// Send query.
	oSocket.Send(QUERY_STRING, strlen(QUERY_STRING));

	// Until we find the packet terminator.
	while (strResponse.Find(END_OF_RESPONSE) == -1)
	{
		int nAvail, nRead;

		// Anything to read?
		if ((nAvail = oSocket.Available()) > 0)
		{
			CBuffer oBuffer(nAvail);

			// Anything read?
			if ((nRead = oSocket.Recv(oBuffer)) > 0)
			{
				const char* psz = static_cast<const char*>(oBuffer.Buffer());

				// Append to response buffer.
				strResponse += A2T(std::string(psz, psz+nRead));
			}
		}

		::Sleep(1);
	}

	// Close session.
	oSocket.Close();

	// Split response buffer into separate fields.
	if (CStrTok::Split(strResponse, FIELD_SEPS, astrAddresses) > 0)
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
