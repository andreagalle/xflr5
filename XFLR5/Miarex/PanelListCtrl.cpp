/****************************************************************************

    CPanelListCtrl Class
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


//
// Built with the help of C++ Codegurus @ www.codeguru.com
// Special thanks to Zafir Anjum's input
//
// SubClass specially (and only) designed for wing definition
//

#include "stdafx.h"
#include "../X-FLR5.h"
#include "../misc/InPlaceNumEdit.h"
#include "../misc/InPlaceFloatEdit.h"
#include "../misc/InPlaceList.h"
#include ".\Panellistctrl.h"
#include "Wing.h"

CPanelListCtrl::CPanelListCtrl(void)
{
	m_nColumns   = 0;
	m_pWing      = NULL;
	m_iItem      = 0;
	m_iSubItem   = 0;
	m_bIsEditing = false;
}

CPanelListCtrl::~CPanelListCtrl(void)
{
}

BEGIN_MESSAGE_MAP(CPanelListCtrl, CListCtrl)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_PANELLIST, OnEndLabelEdit)
	ON_WM_HSCROLL()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


//Zafir Anjum
// HitTestEx	- Determine the row index and column index for a point
// Returns	- the row index or -1 if point is not over a row
// point	- point to be tested.
// col		- to hold the column index
int CPanelListCtrl::HitTestEx(CPoint &point, int *col) const
{
	int colnum = 0;
	int row = HitTest( point, NULL );

	if(col) *col = 0;

	// Make sure that the ListView is in LVS_REPORT
	if( (GetWindowLong(m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT )
		return row;

	// Get the top and bottom row visible
	row = GetTopIndex();
	int bottom = row + GetCountPerPage();
	if( bottom > GetItemCount() )
		bottom = GetItemCount();

	// Get the number of columns
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int nColumnCount = pHeader->GetItemCount();

	// Loop through the visible rows
	for( ;row <=bottom;row++)
	{
		// Get bounding rect of item and check whether point falls in it.
		CRect rect;
		GetItemRect(row, &rect, LVIR_BOUNDS);
		if( rect.PtInRect(point) )
		{
			// Now find the column
			for( colnum = 0; colnum < nColumnCount; colnum++ )
			{
				int colwidth = GetColumnWidth(colnum);
				if( point.x >= rect.left
					&& point.x <= (rect.left + colwidth ) )
				{
					if( col ) *col = colnum;
					return row;
				}
				rect.left += colwidth;
			}
		}
	}
	return -1;
}


// EditSubLabel		- Start edit of a sub item label
// Returns		- Temporary pointer to the new edit control
// nItem		- The row index of the item to edit
// nCol			- The column of the sub item.
CEdit* CPanelListCtrl::EditSubLabel( int nItem, int nCol )
{
	int i;
	// The returned pointer should not be saved

	m_iItem = nItem;
	m_iSubItem = nCol;

	if(m_iSubItem==0) return NULL;

	if(nItem==-1) nItem =GetItemCount()-1;
	if(nItem==GetItemCount()) nItem =0;
	if(nCol==6 || nCol==8 || nCol==10){
		ShowInPlaceList(nItem, nCol);
		return NULL;
	}

	// Make sure that the item is visible
	if(!EnsureVisible(nItem, TRUE)) return NULL;

	// Make sure that nCol is valid
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int nColumnCount = pHeader->GetItemCount();
	if(nCol>=nColumnCount || GetColumnWidth(nCol)<5)
		return NULL;

	// Get the column offset
	int offset = 0;
	for(i=0; i < nCol; i++)
		offset += GetColumnWidth(i);

	CRect rect;
	GetItemRect( nItem, &rect, LVIR_BOUNDS );

	// Now scroll if we need to expose the column
	CRect rcClient;
	GetClientRect( &rcClient );
	if( offset + rect.left < 0 || offset + rect.left > rcClient.right )
	{
		CSize size;
		size.cx = offset + rect.left;
		size.cy = 0;
		Scroll( size );
		rect.left -= size.cx;
	}

	// Get Column alignment
	LV_COLUMN lvcol;
	lvcol.mask = LVCF_FMT;
	GetColumn( nCol, &lvcol );
	DWORD dwStyle ;
	if((lvcol.fmt&LVCFMT_JUSTIFYMASK) == LVCFMT_LEFT)
		dwStyle = ES_LEFT;
	else if((lvcol.fmt&LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT)
		dwStyle = ES_RIGHT;
	else dwStyle = ES_CENTER;

	rect.left += offset+4;
	rect.right = rect.left + GetColumnWidth( nCol ) - 3 ;
	if( rect.right > rcClient.right) rect.right = rcClient.right;

	dwStyle |= WS_BORDER|WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL;

	if(nCol!=7 && nCol !=9) {
		if(nItem<GetItemCount()-1 || (nItem==GetItemCount()-1 && nCol!=4)) {
			CInPlaceFloatEdit *pEdit = new CInPlaceFloatEdit(this, nItem, nCol, GetItemText(nItem, nCol));
			pEdit->Create( dwStyle, rect, this, IDC_PEDIT);
			pEdit->m_ParentList = 0;
			pEdit->m_nColumns = m_nColumns;
			pEdit->m_nRows    = GetItemCount();

			SetItemState(nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED); 
			m_bIsEditing = true;
			return pEdit;
		}
	}
	else if(nCol==7 || nCol==9){
		if(nItem<GetItemCount()-1) {
			CInPlaceNumEdit *pEdit = new CInPlaceNumEdit(this, nItem, nCol, GetItemText(nItem, nCol));
			pEdit->Create( dwStyle, rect, this, IDC_PEDIT);
			pEdit->m_nColumns = m_nColumns;
			pEdit->m_nRows    = GetItemCount();

			SetItemState(nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED); 
			m_bIsEditing = true;
			return pEdit;
		}
	}
	return NULL;
}


void CPanelListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (GetFocus() != this) SetFocus();
	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

/*void CPanelListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (GetFocus() != this) SetFocus();
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}
*/
void CPanelListCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	int index;
	CString strong;
	CListCtrl::OnLButtonDown(nFlags, point);

	int column;
	if((index = HitTestEx(point, &column)) != -1)
	{
		if(column==0) column=1;
		SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED,	LVIS_SELECTED | LVIS_FOCUSED); 
		((CWing*)m_pWing)->m_iSection = index;
	}
//	CListCtrl::OnRButtonDown(nFlags, point);
}



void CPanelListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
//	int CurSection = ((CWing*)m_pWing)->m_iSection;
	int index;
	CString strong;
	CListCtrl::OnLButtonDown(nFlags, point);

	int column;
	if((index = HitTestEx(point, &column)) != -1)
	{
		SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED,	LVIS_SELECTED | LVIS_FOCUSED); 
//		if(index ==CurSection){
			
			((CWing*)m_pWing)->m_iSection = index;

			UINT flag = LVIS_FOCUSED;
			if( GetWindowLong(m_hWnd, GWL_STYLE) & LVS_EDITLABELS )
				if(column==6 || column==8 ||column==10)	{
					ShowInPlaceList(index, column);
				}
				else   EditSubLabel(index, column);
//		}
//		else ((CWing*)m_pWing)->m_iSection = index;
	}
}


void CPanelListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	int index;
	CString strong;

	CListCtrl::OnLButtonDblClk(nFlags, point);
	int column;
	if((index = HitTestEx(point, &column)) != -1)
	{
		SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED,	LVIS_SELECTED | LVIS_FOCUSED); 
					
		((CWing*)m_pWing)->m_iSection = index;

		UINT flag = LVIS_FOCUSED;
		if( GetWindowLong(m_hWnd, GWL_STYLE) & LVS_EDITLABELS )
			if(column==6 || column==8 ||column==10)	{
				ShowInPlaceList(index, column);
			}
			else   EditSubLabel(index, column);
		
	}
}


// ShowInPlaceList	- Creates an in-place drop down list for any 
//					- cell in the list view control
// Returns			- A temporary pointer to the combo-box control
// nItem			- The row index of the cell
// nCol				- The column index of the cell
// lstItems			- A list of strings to populate the control with
// nSel				- Index of the initial selection in the drop down list
CComboBox* CPanelListCtrl::ShowInPlaceList(int nItem, int nCol)
{
	// The returned pointer should not be saved
	int i;

	// Make sure that the item is visible
	if( !EnsureVisible( nItem, TRUE ) ) return NULL;

	// Make sure that nCol is valid 
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int nColumnCount = pHeader->GetItemCount();
	if( nCol >= nColumnCount || GetColumnWidth(nCol) < 10 )
		return NULL;

	// Get the column offset
	int offset = 0;
	for(i = 0; i < nCol; i++ )
		offset += GetColumnWidth( i );

	CRect rect;
	GetItemRect( nItem, &rect, LVIR_BOUNDS );

	// Now scroll if we need to expose the column
	CRect rcClient;
	GetClientRect( &rcClient );
	if( offset + rect.left < 0 || offset + rect.left > rcClient.right )
	{
		CSize size;
		size.cx = offset + rect.left;
		size.cy = 0;
		Scroll( size );
		rect.left -= size.cx;
	}

	rect.left += offset+4;
	rect.right = rect.left + GetColumnWidth( nCol ) - 3 ;
	int height = rect.bottom-rect.top;
	rect.bottom += 15*height;
	if( rect.right > rcClient.right) rect.right = rcClient.right;

	DWORD dwStyle = WS_BORDER|WS_CHILD|WS_VISIBLE|WS_VSCROLL
					|CBS_DROPDOWNLIST|CBS_DISABLENOSCROLL;

	if(nCol==6){
		CComboBox *pList = new CInPlaceList(this, nItem, nCol);
		pList->Create( dwStyle, rect, this, IDC_IPEDIT );
		pList->SetItemHeight( -1, height);
		pList->SetHorizontalExtent( GetColumnWidth( nCol));
		for (i=0; i<m_strList.GetSize();i++) pList->AddString(m_strList[i]);
		int sel = 0;
		CString text = GetItemText(nItem, nCol);
		for (i=0; i<m_strList.GetSize(); i++){
			if(text==m_strList.GetAt(i)){
				sel = i;
				break;
			}
		}
		pList->SetCurSel(sel);
		return pList;
	}
	else if(nCol==8 ){
		if(nItem<GetItemCount()-1) {
			CComboBox *pList = new CInPlaceList(this, nItem, nCol);
			pList->Create( dwStyle, rect, this, IDC_IPEDIT );
			pList->SetItemHeight( -1, height);
			pList->SetHorizontalExtent( GetColumnWidth( nCol));

			pList->AddString("Uniform");
			pList->AddString("Cosine");
			int sel;
			CString text = GetItemText(nItem, nCol);
			if (text=="Uniform")	sel = 0;
			else if(text=="Cosine")	sel = 1;
			pList->SetCurSel(sel);
			return pList;
		}
	}
	else if(nCol ==10){
		if(nItem<GetItemCount()-1) {
			CComboBox *pList = new CInPlaceList(this, nItem, nCol);
			pList->Create( dwStyle, rect, this, IDC_IPEDIT );
			pList->SetItemHeight( -1, height);
			pList->SetHorizontalExtent( GetColumnWidth( nCol));
			pList->AddString("Uniform");
			pList->AddString("Cosine");
			pList->AddString("Sine");
			pList->AddString("-Sine");
			int sel;
			CString text = GetItemText(nItem, nCol);
			if (text=="Uniform")	sel = 0;
			else if(text=="Cosine")	sel = 1;
			else if(text=="Sine")	sel = 2;
			else if(text=="-Sine")	sel = 3;
			pList->SetCurSel(sel);
			return pList;
		}
	}
	return NULL;
}


void CPanelListCtrl::OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO *plvDispInfo = (LV_DISPINFO *)pNMHDR;
	LV_ITEM	*plvItem = &plvDispInfo->item;
	m_bIsEditing = false;
	if (plvItem->pszText != NULL)
	{
		if(plvItem->iItem == GetItemCount()-1 && (plvItem->iSubItem==4 || plvItem->iSubItem>=7))
			SetItemText(plvItem->iItem, plvItem->iSubItem, " ");
		else
			SetItemText(plvItem->iItem, plvItem->iSubItem, plvItem->pszText);
	}
	*pResult = FALSE;
	GetParent()->SendMessage(WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM)plvDispInfo );
}


void CPanelListCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	SHORT sh1 = GetKeyState(VK_LCONTROL);
	SHORT sh2 = GetKeyState(VK_RCONTROL);
	POSITION pos = GetFirstSelectedItemPosition();
	int iItem = GetNextSelectedItem(pos);
//	int iSubItem = GetSelectedColumn();
	int iSubItem = m_iSubItem;

	switch (nChar)
	{
	case VK_UP :
		{
			if (iItem > 0)
				EditSubLabel (iItem - 1, iSubItem);
			return;
		}
	case VK_DOWN :
		{
			EditSubLabel (iItem + 1, iSubItem);
			return;
		}
	case VK_LEFT :
		{
			if (iSubItem > 0)
				EditSubLabel (iItem, iSubItem-1);
			return;
		}
	case VK_RIGHT :
		{
			if (iSubItem < m_nColumns)
				EditSubLabel (iItem , iSubItem+ 1);
			return;
		}
	case VK_HOME :
		{
			if ((sh1 & 0x8000)||(sh2 & 0x8000)){
				EditSubLabel (0, iSubItem);
				return;
			}
		}
	case VK_END :
		{
			if ((sh1 & 0x8000)||(sh2 & 0x8000)){
				int Count = GetItemCount() - 1;
				EditSubLabel (Count, iSubItem);
				return;
			}
		}
	}
	CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CPanelListCtrl::PreTranslateMessage(MSG* pMsg)
{
/*		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;				// DO NOT process further
		}*/
	
	if( pMsg->message == WM_KEYDOWN )
	{
		if(pMsg->wParam == VK_TAB)
		{
			POSITION pos = GetFirstSelectedItemPosition();
			int iItem = GetNextSelectedItem(pos);
//			int iSubItem = GetSelectedColumn();
			int iSubItem = m_iSubItem;
			SHORT sh   = GetKeyState(VK_SHIFT);
			if ((sh & 0x8000))	{
				if (iSubItem > 1) EditSubLabel(iItem, iSubItem - 1);
				else			  EditSubLabel(iItem, m_nColumns - 1);
				
			}
			else{
				if (iSubItem < m_nColumns-1)	EditSubLabel(iItem, iSubItem + 1);
				else							EditSubLabel(iItem , 1);
			}
			return TRUE;
		}
	}
	else if(pMsg->message == WM_VSCROLL){
		return TRUE;
	}
/*	else if(pMsg->message == 9999){
		LV_DISPINFO *plvDispInfo = (LV_DISPINFO *)pMsg->lParam;
		
		EditSubLabel(plvDispInfo->item.iItem, plvDispInfo->item.iSubItem);
		return TRUE;
	}*/
	
	return CListCtrl::PreTranslateMessage(pMsg);
}


void CPanelListCtrl::HighlightCell(int nItem, int nCol )
{
	CDC *pDC = GetDC();
	// Get item image and state info
	LV_ITEM lvi;
	lvi.mask = LVIF_IMAGE | LVIF_STATE;
	lvi.iItem = nItem;
	lvi.iSubItem = 0;
	lvi.stateMask = 0xFFFF;		// get all state flags
	GetItem(&lvi);

	// Should the item be highlighted
	BOOL bHighlight =  ((lvi.state & LVIS_DROPHILITED)
					|| ((lvi.state & LVIS_SELECTED) && ((GetFocus() == this)
					|| (GetStyle() & LVS_SHOWSELALWAYS))));


	// Get rectangles for drawing
	CRect rcBounds, rcLabel, rcIcon;
	GetItemRect(nItem, rcBounds, LVIR_BOUNDS);
	GetItemRect(nItem, rcLabel, LVIR_LABEL);
	GetItemRect(nItem, rcIcon, LVIR_ICON);
	CRect rcCol( rcBounds );


	CString sLabel = GetItemText( nItem, 0 );

	// Labels are offset by a certain amount  
	// This offset is related to the width of a space character
	int offset = pDC->GetTextExtent(_T(" "), 1 ).cx*2;

	CRect rcHighlight;
	CRect rcWnd;
	int nExt;
	switch( 1 )
	{
	case 0:
		nExt = pDC->GetOutputTextExtent(sLabel).cx + offset;
		rcHighlight = rcLabel;
		if( rcLabel.left + nExt < rcLabel.right )
			rcHighlight.right = rcLabel.left + nExt;
		break;
	case 1:
		rcHighlight = rcBounds;
		rcHighlight.left = rcLabel.left;
		break;
	case 2:
		GetClientRect(&rcWnd);
		rcHighlight = rcBounds;
		rcHighlight.left = rcLabel.left;
		rcHighlight.right = rcWnd.right;
		break;
	default:
		rcHighlight = rcLabel;
	}

	// Draw the highlighted background color
	pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
	pDC->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));

	pDC->FillRect(rcHighlight, &CBrush(::GetSysColor(COLOR_HIGHLIGHT)));

	// Draw hilghlighted item label - Column 0
	rcLabel.left += offset/2;
	rcLabel.right -= offset;

	pDC->DrawText(sLabel,-1,rcLabel,DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP
				| DT_VCENTER | DT_END_ELLIPSIS);

	ReleaseDC(pDC);
}


BOOL CPanelListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if(m_bIsEditing) return TRUE;
	return CListCtrl::OnMouseWheel(nFlags, zDelta, pt);
}