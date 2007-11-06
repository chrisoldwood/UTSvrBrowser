/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		SUMMARYDLG.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CSummaryDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef SUMMARYDLG_HPP
#define SUMMARYDLG_HPP

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/CommonUI.hpp>

/******************************************************************************
** 
** The dialog used to display the servers summary.
**
*******************************************************************************
*/

class CSummaryDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CSummaryDlg();
	
protected:
	//
	// Controls.
	//
	CListView	m_lvGrid;

	//
	// Column indices.
	//
	enum Column
	{
		MOD_NAME,
		SERVERS,
		PLAYERS,

		NUM_COLUMNS,
	};

	//
	// Message handlers.
	//
	virtual void OnInitDialog();
	virtual bool OnOk();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // SUMMARYDLG_HPP
