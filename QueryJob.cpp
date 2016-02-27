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
#include <WCL/StrCvt.hpp>

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
	m_oRow[CServers::MAP_TITLE]   = TXT("");
	m_oRow[CServers::MAP_NAME]    = TXT("");
	m_oRow[CServers::NUM_PLAYERS] = 0;
	m_oRow[CServers::MAX_PLAYERS] = 0;
	m_oRow[CServers::PING_TIME]   = INT_MAX;

	CGameServer    oServer(m_oRow[CServers::IP_ADDRESS], m_oRow[CServers::IP_PORT]);
	CQueryResponse oResponse;

	// Ping server and process response.
	if (oServer.QueryInfo(oResponse))
	{
		CString strHostName = oResponse.FieldValue(TXT("hostname"));

		// Trim excess whitespace from hostname?
		if (App.m_oMtrQryOpts.m_bTrimSpace)
			strHostName.Trim();

		// Translate hostname symbols to letters?
		if (App.m_oMtrQryOpts.m_bConvertSyms)
			ConvertSymbols(strHostName);

		CString strMapTitle = oResponse.FieldValue(TXT("maptitle"));

		// If no map title, use name instead.
		if (strMapTitle.Empty())
			strMapTitle = oResponse.FieldValue(TXT("mapname"));

		// Extract relevant fields
		m_oRow[CServers::HOST_NAME]   = strHostName;
		m_oRow[CServers::MAP_TITLE]   = strMapTitle;
		m_oRow[CServers::MAP_NAME]    = oResponse.FieldValue(TXT("mapname"));
		m_oRow[CServers::GAME_TYPE]   = oResponse.FieldValue(TXT("gametype"));
		m_oRow[CServers::MOD_NAME]    = FindModName(m_oRow[CServers::GAME_TYPE]);
		m_oRow[CServers::NUM_PLAYERS] = CStrCvt::ParseInt(oResponse.FieldValue(TXT("numplayers")));
		m_oRow[CServers::MAX_PLAYERS] = CStrCvt::ParseInt(oResponse.FieldValue(TXT("maxplayers")));
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

CString CQueryJob::FindModName(const tchar* pszGameType)
{
	const tchar* pszModName = TXT("");

	// Lookup game type in table.
	CRow* pModRow = App.m_oGameTypes.SelectRow(CGameTypes::GAME_TYPE, pszGameType);

	if (pModRow != nullptr)
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
	static const tchar* pszSymbols = TXT("@ß©Ð£Þ®§$¥");
	static const tchar* pszLetters = TXT("aBcDeprssy");

	size_t nLen = strString.Length();

	// For all characters...
	for (size_t i = 0; i < nLen; ++i)
	{
		const tchar* pcSymbol = tstrchr(pszSymbols, strString[i]);

		// Remap, if a symbol.
		if (pcSymbol != nullptr)
			strString[i] = pszLetters[pcSymbol-pszSymbols];
	}
}
