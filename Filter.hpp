/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		FILTER.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CFilter class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef FILTER_HPP
#define FILTER_HPP

/******************************************************************************
** 
** The data class used to store the filter settings.
**
*******************************************************************************
*/

class CFilter
{
public:
	//
	// Constructors/Destructor.
	//
	CFilter();
	~CFilter();
	
	//
	// Members.
	//
	CString		m_strName;		// Filter name.
	CString		m_strDesc;		// Filter description.
	bool		m_bFltMods;		// Filter by mod name?
	CStrArray	m_astrMods;		// Mods to include.
	bool		m_bFltPing;		// Filter by ping?
	int			m_nPingTime;	// Ping threshold.
	bool		m_bFltErrors;	// Filter by errors?
	bool		m_bFltNames;	// Filter by server name?
	CStrArray	m_astrNames;	// Names to include.
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CFilter::CFilter()
	: m_bFltMods(false)
	, m_bFltPing(false)
	, m_nPingTime(250)
	, m_bFltErrors(true)
{
}

inline CFilter::~CFilter()
{
}

#endif // FILTER_HPP
