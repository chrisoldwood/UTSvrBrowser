/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		MTRFIREWALLPAGE.HPP
** COMPONENT:	The Application
** DESCRIPTION:	The CMtrFirewallPage class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef MTRFIREWALLPAGE_HPP
#define MTRFIREWALLPAGE_HPP

/******************************************************************************
** 
** The property page for the master server query firewall settings.
**
*******************************************************************************
*/

class CMtrFirewallPage : public CPropertyPage
{
public:
	//
	// Constructors/Destructor.
	//
	CMtrFirewallPage(CMasterQueryOpts& oConfig);
	
protected:
	//
	// Members.
	//
	CMasterQueryOpts&	m_oConfig;
	
	//
	// Controls.
	//
	CCheckBox	m_ckFirewall;
	CDecimalBox	m_ebFirstPort;
	CDecimalBox	m_ebLastPort;

	//
	// Message handlers.
	//
	virtual void OnInitDialog();
	virtual bool OnValidate();
	
	void OnClickFirewall();
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

#endif // MTRFIREWALLPAGE_HPP
