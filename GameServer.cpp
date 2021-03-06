/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		GAMESERVER.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CGameServer class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "GameServer.hpp"
#include "UTSvrBrowser.hpp"
#include <limits.h>
#include <NCL/UDPCltSocket.hpp>
#include <WCL/StrTok.hpp>
#include "QueryResponse.hpp"
#include <NCL/SocketException.hpp>
#include <Core/AnsiWide.hpp>

/******************************************************************************
**
** Constants.
**
*******************************************************************************
*/

const char*  CGameServer::QUERY_INFO      = "\\info\\";
const char*  CGameServer::QUERY_STATUS    = "\\status\\";
const tchar* CGameServer::END_OF_RESPONSE = TXT("\\final\\");
const tchar* CGameServer::FIELD_SEPS      = TXT("\\");

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

CGameServer::CGameServer(const tchar* pszAddress, int nPort)
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

CGameServer::~CGameServer()
{
}

/******************************************************************************
** Method:		QueryInfo()
**
** Description:	Queries the server for the basic server info. It is used as the
**				ping request.
**
** Parameters:	oResponse	The servers response.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CGameServer::QueryInfo(CQueryResponse& oResponse)
{
	return ExecQuery(QUERY_INFO, oResponse);
}

/******************************************************************************
** Method:		QueryStatus()
**
** Description:	Queries the server for the full status of the current game.
**
** Parameters:	oResponse	The servers response.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CGameServer::QueryStatus(CQueryResponse& oResponse)
{
	return ExecQuery(QUERY_STATUS, oResponse);
}

/******************************************************************************
** Method:		ExecQuery()
**
** Description:	Send a query to the server and extract the response.
**
** Parameters:	pszQuery	The query to execute.
**				oResponse	The servers response.
**
** Returns:		true or false.
**
*******************************************************************************
*/

bool CGameServer::ExecQuery(const char* pszQuery, CQueryResponse& oResponse)
{
	int nError = CServers::ERROR_FAILED;
	int nTime  = INT_MAX;

	try
	{
		int  nMaxAttempts = App.m_oPingOpts.m_nAttempts;
		uint nPingTimeout = App.m_oPingOpts.m_nWaitTime;

		// For n attempts...
		for (int nAttempt = 0; ((nAttempt < nMaxAttempts) && (nError != CServers::ERROR_NONE)); ++nAttempt)
		{
			CUDPCltSocket oSocket;
			CString       strResponse;

			// Open a connection.
			oSocket.Connect(m_strAddress, m_nPort);

			// Send the query.
			oSocket.Send(pszQuery, strlen(pszQuery));

			DWORD dwStart = ::GetTickCount();

			// Until we find the response terminator.
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

				// Abort query if it takes too long.
				if ((::GetTickCount() - dwStart) > nPingTimeout)
				{
					nError = CServers::ERROR_TIMED_OUT;
					break;
				}

				::Sleep(1);
			}

			// Calculate ping time.
			nTime = ::GetTickCount() - dwStart;

			// Close connection.
			oSocket.Close();
			
			// Got entire response?
			if (strResponse.Find(END_OF_RESPONSE) != -1)
			{
				CStrArray astrFields;

				// Discard leading '\\'.
				strResponse.Delete(0);

				// Split response into fields.
				CStrTok::Split(strResponse, FIELD_SEPS, astrFields);

				// Add field/value pairs to response...
				for (size_t i = 0; i < (astrFields.Size() / 2); ++i)
				{
					oResponse.m_astrFields.Add(astrFields[(i*2)+0]);
					oResponse.m_astrValues.Add(astrFields[(i*2)+1]);
				}

				// Success.
				nError = CServers::ERROR_NONE;
			}
		}
	}
	catch (const CSocketException& /*e*/)
	{
//		TRACE3("SocketException (%s:%d): %s\n", m_strAddress, m_nPort, e.ErrorText());
	}

	// Complete response details.
	oResponse.m_nError = nError;
	oResponse.m_nTime  = nTime;

	return (nError == CServers::ERROR_NONE);
}
