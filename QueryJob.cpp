/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		QUERYJOB.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CQueryJob class definition.
**
*******************************************************************************
*/

#include "AppHeaders.hpp"

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

CQueryJob::CQueryJob(CRow& oRow)
	: m_oRow(oRow)
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

CQueryJob::~CQueryJob()
{
}

/******************************************************************************
** Method:		Run()
**
** Description:	Check the clock on the computer.
**
** Parameters:	None.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CQueryJob::Run()
{
	CString strAddr = m_oRow[CServers::IP_ADDRESS];
	int     nPort   = m_oRow[CServers::IP_PORT];
	DWORD   dwStart = ::GetTickCount();

	try
	{
		CUDPCltSocket oSocket;
		CString       strResponse;

		// Open a connection.
		oSocket.Connect(strAddr, nPort);

		// Send the query.
		oSocket.Send("\\info\\");

		// Until we find the packet terminator.
		while (strResponse.Find("\\final\\") == -1)
		{
			int nAvail, nRead;

			// Anything to read?
			if ((nAvail = oSocket.Available()) > 0)
			{
				CBuffer oBuffer(nAvail);

				// Anything read?
				if ((nRead = oSocket.Recv(oBuffer)) > 0)
				{
					// Append to response buffer.
					strResponse += oBuffer.ToString(nRead);
				}
			}

			// Abort query if it takes longer than 1 sec.
			if ((::GetTickCount() - dwStart) > 1000)
				break;

			::Sleep(1);
		}

		// Close connection.
		oSocket.Close();

		// Got entire response?
		if (strResponse.Find("\\final\\") != -1)
		{
			CStrArray astrFields;

			// Split response in fields.
			strResponse.Split("\\", astrFields);

			// Extract relevant fields
			m_oRow[CServers::HOST_NAME]   = ExtractField(astrFields, "hostname");
			m_oRow[CServers::MAP_TITLE]   = ExtractField(astrFields, "maptitle");
			m_oRow[CServers::MAP_NAME]    = ExtractField(astrFields, "mapname");
			m_oRow[CServers::GAME_TYPE]   = ExtractField(astrFields, "gametype");
			m_oRow[CServers::NUM_PLAYERS] = atoi(ExtractField(astrFields, "numplayers"));
			m_oRow[CServers::MAX_PLAYERS] = atoi(ExtractField(astrFields, "maxplayers"));
		}
	}
	catch (CSocketException& /*e*/)
	{
//		TRACE3("SocketException (%s:%d): %s\n", strAddr, nPort, e.ErrorText());
	}
}

/******************************************************************************
** Method:		ExtractField()
**
** Description:	Extracts a field from the response.
**
** Parameters:	astrFields	The array of fields.
**				pszField	The field name.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

CString CQueryJob::ExtractField(CStrArray& astrFields, const char* pszField)
{
	ASSERT(pszField != NULL);

	int nField = astrFields.Find(pszField);

	return (nField != -1) ? astrFields[nField+1] : "";
}
