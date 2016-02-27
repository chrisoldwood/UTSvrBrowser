/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPCMDS.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CAppCmds class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef APPCMDS_HPP
#define APPCMDS_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CmdCtrl.hpp>

// Forward declarations.
class CAppWnd;

/******************************************************************************
** 
** Application command controller.
**
*******************************************************************************
*/

class CAppCmds : public CCmdControl
{
public:
	//
	// Constructors/Destructor.
	//
	CAppCmds(CAppWnd& appWnd);
	~CAppCmds();

	//
	// Commands.
	//

	// Servers menu.
	void OnServersPing();
	void OnServersPingFiltered();
	void OnServersPingAll();
	void OnServersRefresh();
	void OnServersFind();
	void OnServersFindNext();
	void OnServersAddFav();
	void OnServersDelFav();
	void OnServersSummary();
	void OnServersExit();

	// Filter menu.
	void OnFilterNone();
	void OnFilter(uint nCmdID);

	// View menu.
	void OnViewColumnName();
	void OnViewColumnMod();
	void OnViewColumnMap();
	void OnViewColumnPlayers();
	void OnViewColumnPing();
	void OnViewColumnIPAddress();
	void OnViewColumnIPPort();
	void OnViewColumnAutoSize();
	void OnViewSortByName();
	void OnViewSortByMod();
	void OnViewSortByMap();
	void OnViewSortByPlayers();
	void OnViewSortByPing();
	void OnViewSortByIPAddress();
	void OnViewSortByIPPort();
	void OnViewSortBy(int eColumn);
	void OnViewDetails();

	// Options menu.
	void OnOptionsMaster();
	void OnOptionsPing();
	void OnOptionsFilters();
	void OnOptionsFavourites();

	// Help menu.
	void OnHelpAbout();

	//
	// UI handlers.
	//
	void OnUIServersPingFiltered();
	void OnUIServersFindNext();
	void OnUIServersAddFav();
	void OnUIServersDelFav();
	void OnUIFilter();
	void OnUIViewSortByName();
	void OnUIViewSortByMod();
	void OnUIViewSortByMap();
	void OnUIViewSortByPlayers();
	void OnUIViewSortByPing();
	void OnUIViewSortByIPAddress();
	void OnUIViewSortByIPPort();
	void OnUIViewSortBy(uint nCmdID, size_t nColumn);
	void OnUIViewDetails();

	//
	// Command property methods.
	//
	virtual CString CmdHintStr(uint iCmdID) const;

protected:
	//
	// Internal methods.
	//
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif //APPCMDS_HPP
