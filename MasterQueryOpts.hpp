/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		MASTERQUERYOPTS.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CMasterQueryOpts class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef MASTERQUERYOPTS_HPP
#define MASTERQUERYOPTS_HPP

#if _MSC_VER > 1000
#pragma once
#endif

/******************************************************************************
** 
** The data class used to hold the master query settings.
**
*******************************************************************************
*/

class CMasterQueryOpts
{
public:
	//
	// Constructors.
	//
	CMasterQueryOpts();

	//
	// Members.
	//
	bool	m_bModified;		// Edited?
	CString	m_strAddress;		// Master server address.
	int		m_nPort;			// Master server port.
	bool	m_bFirewall;		// Is there a firewall?
	int		m_nFirstPort;		// First port allowed.
	int		m_nLastPort;		// Last port allowed.
	bool	m_bTrimSpace;		// Trim whitespace from server name?
	bool	m_bConvertSyms;		// Convert server name symbols?
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CMasterQueryOpts::CMasterQueryOpts()
	: m_bModified(false)
{
}

#endif // MASTERQUERYOPTS_HPP
