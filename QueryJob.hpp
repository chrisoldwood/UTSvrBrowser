/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		QUERYJOB.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CQueryJob class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef QUERYJOB_HPP
#define QUERYJOB_HPP

/******************************************************************************
** 
** The background job used to check the clock on a computer.
**
*******************************************************************************
*/

class CQueryJob : public CThreadJob
{
public:
	//
	// Constructors/Destructor.
	//
	CQueryJob(CRow& oRow);
	~CQueryJob();
	
	//
	// Methods.
	//
	virtual void Run();

protected:
	//
	// Members.
	//
	CRow& m_oRow;

	//
	// Internal methods.
	//
	CString ExtractField(CStrArray& astrFields, const char* pszField);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // QUERYJOB_HPP
