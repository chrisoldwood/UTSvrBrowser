/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		GAMEINFODLG.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CGameInfoDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef GAMEINFODLG_HPP
#define GAMEINFODLG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>
#include <MDBL/Row.hpp>

/******************************************************************************
** 
** The dialog used to display the current game status.
**
*******************************************************************************
*/

class CGameInfoDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CGameInfoDlg(CRow& oRow);
	
protected:
	//
	// Members.
	//
	CRow& m_oRow;
	
	//
	// Controls.
	//
	CListView	m_lvTeam1;
	CListView	m_lvTeam2;

	// Team grid columns.
	enum Column
	{
		PLAYER,
		FRAGS,
	};

	//
	// Message handlers.
	//
	virtual void OnInitDialog();
	virtual bool OnCancel();
	virtual void OnTimer(uint nTimerID);

	//
	// Constants.
	//
	static const uint TIMER_ID;
	static const uint TIMER_FREQ;

	//
	// Internal methods.
	//
	void RefreshInfo();
	void AddPlayer(CListView& lvTeam, const tchar* pszPlayer, const tchar* pszFrags);

	static int CALLBACK Compare(LPARAM lParam1, LPARAM lParam2, LPARAM);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // GAMEINFODLG_HPP
