/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		QUERYJOB.CPP
** COMPONENT:	The Application
** DESCRIPTION:	CQueryJob class definition.
**
*******************************************************************************
*/

#include "Common.hpp"
#include "QueryJob.hpp"
#include "UTSvrBrowser.hpp"
#include <limits.h>
#include "GameServer.hpp"
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
	// Clear exisiting data.
	m_oRow[CServers::MAP_TITLE]   = "";
	m_oRow[CServers::MAP_NAME]    = "";
	m_oRow[CServers::NUM_PLAYERS] = 0;
	m_oRow[CServers::MAX_PLAYERS] = 0;
	m_oRow[CServers::PING_TIME]   = INT_MAX;

	CGameServer    oServer(m_oRow[CServers::IP_ADDRESS], m_oRow[CServers::IP_PORT]);
	CQueryResponse oResponse;

	// Ping server and process response.
	if (oServer.QueryInfo(oResponse))
	{
		CString strHostName = oResponse.FieldValue("hostname");

		// Trim excess whitespace from hostname?
		if (App.m_oMtrQryOpts.m_bTrimSpace)
			strHostName.Trim();

		// Translate hostname symbols to letters?
		if (App.m_oMtrQryOpts.m_bConvertSyms)
			ConvertSymbols(strHostName);

		CString strMapTitle = oResponse.FieldValue("maptitle");

		// If no map title, use name instead.
		if (strMapTitle.Empty())
			strMapTitle = oResponse.FieldValue("mapname");

		// Extract relevant fields
		m_oRow[CServers::HOST_NAME]   = strHostName;
		m_oRow[CServers::MAP_TITLE]   = strMapTitle;
		m_oRow[CServers::MAP_NAME]    = oResponse.FieldValue("mapname");
		m_oRow[CServers::GAME_TYPE]   = oResponse.FieldValue("gametype");
		m_oRow[CServers::MOD_NAME]    = FindModName(m_oRow[CServers::GAME_TYPE]);
		m_oRow[CServers::NUM_PLAYERS] = atoi(oResponse.FieldValue("numplayers"));
		m_oRow[CServers::MAX_PLAYERS] = atoi(oResponse.FieldValue("maxplayers"));
		m_oRow[CServers::PING_TIME]   = oResponse.Time();
	}

	// Set result.
	m_oRow[CServers::LAST_ERROR] = oResponse.Error();
}

/******************************************************************************
** Method:		FindModName()
**
** Description:	Find the name of the mod that corresponds to the game type.
**
** Parameters:	pszGameType		The game type.
**
** Returns:		The mod name or "".
**
*******************************************************************************
*/

CString CQueryJob::FindModName(const char* pszGameType)
{
	const char* pszModName = "";

	// Lookup game type in table.
	CRow* pModRow = App.m_oGameTypes.SelectRow(CGameTypes::GAME_TYPE, pszGameType);

	if (pModRow != NULL)
		pszModName = pModRow->Field(CGameTypes::MOD_NAME);

	return pszModName;
}

/******************************************************************************
** Method:		ConvertSymbols()
**
** Description:	Converts any symbols to standard letters.
**
** Parameters:	strString	The string to convert.
**
** Returns:		Nothing.
**
*******************************************************************************
*/

void CQueryJob::ConvertSymbols(CString& strString)
{
	static const char* pszSymbols = "@ß©Ð£Þ®§$¥";
	static const char* pszLetters = "aBcDeprssy";

	int nLen = strString.Length();

	// For all characters...
	for (int i = 0; i < nLen; ++i)
	{
		const char* pcSymbol = strchr(pszSymbols, strString[i]);

		// Remap, if a symbol.
		if (pcSymbol != NULL)
			strString[i] = pszLetters[pcSymbol-pszSymbols];
	}
}
