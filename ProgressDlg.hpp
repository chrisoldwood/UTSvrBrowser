/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		PROGRESSDLG.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CProgressDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef PROGRESSDLG_HPP
#define PROGRESSDLG_HPP

/******************************************************************************
** 
** This is the dialog to display the scanning progress.
**
*******************************************************************************
*/

class CProgressDlg : public CDialog
{
public:
	//
	// Constructors/Destructor.
	//
	CProgressDlg();
	
	//
	// Methods.
	//
	bool Abort();
	void InitMeter(uint nSteps);
	void UpdateLabel(const char* pszLabel);
	void UpdateMeter(uint nSteps);
	void UpdateLabelAndMeter(const char* pszLabel, uint nSteps);

protected:
	//
	// Controls.
	//
	CLabel			m_txtLabel;
	CProgressBar	m_barMeter;

	//
	// Members.
	//
	bool	m_bAbort;

	//
	// Message processors.
	//
	virtual void OnInitDialog();

	void OnAbort();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline bool CProgressDlg::Abort()
{
	return m_bAbort;
}

#endif //PROGRESSDLG_HPP
