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

#if _MSC_VER > 1000
#pragma once
#endif

#include <WCL/MainDlg.hpp>
#include <WCL/ListView.hpp>
#include <MDBL/Row.hpp>
#include <MDBL/SortColumns.hpp>

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
	int   SortColumn() const;
	CRow* SelectedRow() const;

	//
	// Methods.
	//
	void RefreshView();
	void RefreshRow(CRow& oRow);
	void RefreshAllRows();
	void Sort(int nColumn);
	void ToggleColumn(int nColumn);
	void AutoSizeColumns();
	void SelectRow(int nRow);

	//
	// Column indices.
	//
	enum Column
	{
		HOST_NAME,
		MOD_NAME,
		MAP_TITLE,
		GAME_TYPE,
		PLAYERS,
		PING_TIME,
		IP_ADDRESS,
		IP_PORT,
//		LAST_ERROR,

		NUM_COLUMNS,
	};

	//
	// Constants.
	//
	static const int DEF_COLUMN_WIDTH;

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
	virtual void OnDestroy();
	LRESULT OnGridSelchange(NMHDR& oNMHeader);
	LRESULT OnGridRightClick(NMHDR& oNMHeader);
	LRESULT OnGridDoubleClick(NMHDR& oNMHeader);
	LRESULT OnGridClickColumn(NMHDR& oNMHeader);

	//
	// Internal methods.
	//
	CRow*   GridRow(int nRow) const;
	CString FmtPlayers(const CRow& oRow);
	CString FmtPingTime(const CRow& oRow);
	CString FmtLastErr(const CRow& oRow);
	CString FmtModName(const CRow& oRow);
	int     TableColumn(int nColumn);
};

/******************************************************************************
**
** Implementation of inline functions.
**
*******************************************************************************
*/

inline CRow* CAppDlg::GridRow(int nRow) const
{
	ASSERT((nRow >= 0) && (nRow < m_lvGrid.ItemCount()));

	return (CRow*) m_lvGrid.ItemPtr(nRow);
}

inline int CAppDlg::SortColumn() const
{
	return m_nSortColumn;
}

inline CRow* CAppDlg::SelectedRow() const
{
	CRow* pRow = NULL;

	if (m_lvGrid.IsSelection())
		pRow = GridRow(m_lvGrid.Selection());

	return pRow;
}

inline void CAppDlg::SelectRow(int nRow)
{
	ASSERT((nRow >= 0) && (nRow < m_lvGrid.ItemCount()));

	m_lvGrid.Select(nRow);
	m_lvGrid.MakeItemVisible(nRow);
}

#endif //APPDLG_HPP
