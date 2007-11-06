/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PINGOPTS.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CPingOpts class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef PINGOPTS_HPP
#define PINGOPTS_HPP

#if _MSC_VER > 1000
#pragma once
#endif

/******************************************************************************
** 
** The data class used to hold the ping settings.
**
*******************************************************************************
*/

class CPingOpts
{
public:
	//
	// Constructors.
	//
	CPingOpts();

	//
	// Members.
	//
	bool	m_bModified;		// Edited?
	int		m_nThreads;			// Ping thread pool size.
	int		m_nAttempts;		// Ping attempts.
	uint	m_nWaitTime;		// Ping max wait time (ms).
	bool	m_bAutoPing;		// Automatically ping the selection?
	int		m_nAutoInterval;	// Interval betwen auto-pings (s).
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CPingOpts::CPingOpts()
	: m_bModified(false)
{
}

#endif // PINGOPTS_HPP
