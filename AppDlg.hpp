/******************************************************************************
** (C) Chris Oldwood
**
** MODULE:		APPDLG.HPP
** COMPONENT:	The Application.
** DESCRIPTION:	The CAppDlg class declaration.
**
*******************************************************************************
*/

// Check for previous inclusion
#ifndef APPDLG_HPP
#define APPDLG_HPP

/******************************************************************************
** 
** This is the main application dialog.
**
*******************************************************************************
*/

class CAppDlg : public CMainDlg
{
public:
	//
	// Constructors/Destructor.
	//
	CAppDlg();
	
	//
	// Properties.
	//
	int SortColumn() const;

	//
	// Methods.
	//
	void RefreshView();
	void Sort(int nColumn);

	//
	// Column indices.
	//
	enum Column
	{
		IP_ADDRESS,
		IP_PORT,
		HOST_NAME,
		MAP_TITLE,
		GAME_TYPE,
		PLAYERS,
	};

protected:
	//
	// Controls.
	//
	CListView	m_lvGrid;

	//
	// Members.
	//
	int					m_nSortColumn;
	CSortColumns::Dir	m_eSortOrder;

	//
	// Message processors.
	//
	virtual void OnInitDialog();
	LRESULT OnGridSelchange(NMHDR& oNMHeader);
	LRESULT OnGridRightClick(NMHDR& oNMHeader);
	LRESULT OnGridClickColumn(NMHDR& oNMHeader);

	//
	// Internal methods.
	//
	CString FmtPlayers(const CRow& oRow);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline int CAppDlg::SortColumn() const
{
	return m_nSortColumn;
}

#endif //APPDLG_HPP
