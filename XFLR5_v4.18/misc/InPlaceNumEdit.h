/****************************************************************************

    CInPlaceNumEdit Class
	Copyright (C) 2006 Andr� Deperrois xflr5@yahoo.com

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*****************************************************************************/

#pragma once
#include "afxwin.h"

class CInPlaceNumEdit :
	public CEdit
{
	friend class CPanelListCtrl;
	friend class CControlListCtrl;
public:
	CInPlaceNumEdit();
	CInPlaceNumEdit(CListCtrl* pCtrl, int iItem, int iSubItem, CString sInitText);
	~CInPlaceNumEdit(void);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNumEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetMax(int k);
	void SetMin(int k);
	bool CheckBeforeExit();
	CString m_iniStr;
	int m_iMax;
	int m_iMin;
	void SetValue(int i);
	bool IsValid();
	int GetValue();
	void Set(int iItem, int iSubItem, CString sInitText);

	CListCtrl *m_pListCtrl;

	void EditSubLabel(int iItem, int iSubItem);

private:
	int m_iItem;
	int m_iSubItem;
	int m_nColumns, m_nRows;
	BOOL    m_bESC;	 	// To indicate whether ESC key was pressed


	// Generated message map functions
protected:
	//{{AFX_MSG(CNumEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNcDestroy();
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
