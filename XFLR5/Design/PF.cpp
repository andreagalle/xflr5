/****************************************************************************

	Splined Curve Foil class
    Copyright (C) 2004 Andr� Deperrois XFLR5@yahoo.com

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

// PF.cpp: implementation of the CPF class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../XDirect/XDirect.h"
#include "PF.h"
#include "math.h"
#include "Spline.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPF::CPF()
{
	m_FoilStyle    = PS_SOLID;
	m_FoilWidth    = 1;
	m_FoilColor    = RGB(50,180,130);
	m_bOutPoints   = false;
	m_OutPoints    = 0;
	m_bVisible     = true;
	m_bOutPoints   = false;
	m_bCenterLine  = false;
	memset(m_rpMid, 0, sizeof(m_rpMid));
}

CPF::~CPF()
{

}

bool CPF::CompMidLine(bool first)
{
	double x, yex, yin;
	m_fThickness = 0.0;
	m_fCamber    = 0.0;
	m_fxCambMax  = 0.0;
	m_fxThickMax = 0.0;

	m_rpMid[0].x   = 0.0;
	m_rpMid[0].y   = 0.0;
	m_rpMid[100].x = 1.0;
	m_rpMid[100].y = 0.0;

	
	double step = 0.001;
	for (int l=0; l<=1000; l++){
		x = l*step;
		if(first){
			yex = m_Extrados.GetY(x);
			yin = m_Intrados.GetY(x);
		}
		else{
			yex = m_Extrados.m_Outy[l];
			yin = m_Intrados.m_Outy[l];
		}
		m_rpMid[l].x = x;
		m_rpMid[l].y = (yex+yin)/2.0;
		if(fabs(yex-yin)>m_fThickness){
			m_fThickness = (double)fabs(yex-yin);
			m_fxThickMax = x;
		}
		if(fabs(m_rpMid[l].y)>fabs(m_fCamber)){
			m_fCamber = m_rpMid[l].y;
			m_fxCambMax = x;
		}
	}
	return true;
}


void CPF::DrawFoil(CDC *pDC, double scalex, double scaley, CPoint Offset, bool IsPrinting)
{
	m_Extrados.DrawSplines(pDC, scalex, scaley, Offset, IsPrinting);
	m_Intrados.DrawSplines(pDC, scalex, scaley, Offset, IsPrinting);
	CompMidLine();
}


void CPF::DrawMidLine(CDC *pDC, double scalex, double scaley, CPoint Offset, bool IsPrinting){
	if (IsPrinting){
		scaley=-scaley;
	}
	pDC->MoveTo((int)(m_rpMid[0].x*scalex)  +Offset.x,
				(int)(m_rpMid[0].y*scaley)  +Offset.y);

	for (int k=1; k<=100; k++){
		pDC->LineTo((int)(m_rpMid[k*10].x*scalex)+Offset.x,
					(int)(m_rpMid[k*10].y*scaley)+Offset.y);
	}
}



bool CPF::InitSplinedFoil()
{
	m_bModified   = false;
	m_strFoilName = "Splined Points Foil";
	m_Extrados.m_iPoints = -1;
	m_Extrados.InsertPoint(1.00,  0.0000);
	m_Extrados.InsertPoint(0.887, 0.0173);
	m_Extrados.InsertPoint(0.657, 0.0438);
	m_Extrados.InsertPoint(0.383, 0.0612);
	m_Extrados.InsertPoint(0.247, 0.0603);
	m_Extrados.InsertPoint(0.129, 0.0484);
	m_Extrados.InsertPoint(0.041, 0.0279);
	m_Extrados.InsertPoint(0.000, 0.0000);
	m_Extrados.m_Slope[0].x  = 0.0;
	m_Extrados.m_Slope[0].y  = 1.0;
	m_Extrados.m_RearPoint.x = 1.06119;
	m_Extrados.m_RearPoint.y = -0.00948;

	m_Intrados.m_iPoints = -1;
	m_Intrados.InsertPoint(1.000,  0.0000);
	m_Intrados.InsertPoint(0.032, -0.0196);
	m_Intrados.InsertPoint(0.119, -0.0300);
	m_Intrados.InsertPoint(0.423, -0.0254);
	m_Intrados.InsertPoint(0.837, -0.0023);
	m_Intrados.InsertPoint(0.941, -0.0009);
	m_Intrados.InsertPoint(0.000,  0.0000);
	m_Intrados.m_RearPoint.x = 1.0612;
	m_Intrados.m_RearPoint.y = 0.00;
	m_Intrados.m_Slope[0].x =  0.0;
	m_Intrados.m_Slope[0].y = -1.0;

	m_Extrados.CompSlopes();
	m_Intrados.CompSlopes();
	CompMidLine();
	m_OutPoints = m_Extrados.m_iPoints + m_Intrados.m_iPoints;
	return true;
}

bool CPF::DrawCtrlPoints(CDC *pDC, double scalex, double scaley, CPoint Offset, bool IsPrinting)
{
	for (int i=0; i<=m_Extrados.m_iPoints;i++){
		m_Extrados.DrawControlPoint(pDC, i, scalex, scaley, Offset, IsPrinting);
	}
	for (i=0; i<=m_Intrados.m_iPoints;i++){
		m_Intrados.DrawControlPoint(pDC, i, scalex, scaley, Offset, IsPrinting);
	}
	m_Extrados.DrawRearPoint(pDC, scalex, scaley, Offset, IsPrinting);
	m_Intrados.DrawRearPoint(pDC, scalex, scaley, Offset, IsPrinting);
	return true;
}


bool CPF::Export(CStdioFile *pFile){
	m_Extrados.Export(pFile, true);
	m_Intrados.Export(pFile, false);
	return true;
}

void CPF::SetViewRect(CRect rc){
	m_Intrados.m_rViewRect = rc;
	m_Extrados.m_rViewRect = rc;
}


// FoilPoints.cpp: implementation of the CSplinedPoints class.
//
//////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSplinedPoints::CSplinedPoints(){
	m_iHighlight = -10;
	m_iSelect    = -10;
	m_iPoints    = -1;
	m_bOutPts    = false;
	m_Freq       = 8;
	m_rViewRect.SetRectEmpty();
	memset(m_Outy, 0, sizeof(m_Outy));
	memset(m_ctrlPoint, 0, sizeof(m_ctrlPoint));
	memset(m_Slope, 0, sizeof(m_Slope));
}

CSplinedPoints::~CSplinedPoints(){

}


bool CSplinedPoints::DrawControlPoint(CDC *pDC, int i, double scalex, double scaley, CPoint Offset, bool IsPrinting)
{
	if(i<0 && i>m_iPoints) return false;

	int width;
	if (IsPrinting){
		scaley=-scaley;
		width = 60;
	}
	else{
		width = 3;
	}

	CPoint pt;
	pt.x = (int)( m_ctrlPoint[i].x*scalex)+Offset.x;
	pt.y = (int)(-m_ctrlPoint[i].y*scaley)+Offset.y;
	if(!m_rViewRect.PtInRect(pt)) return false;

	if (!IsPrinting && m_iSelect==i) {
		CPen SelectPen(PS_SOLID, 2, RGB(0,0,150));
		CPen *pOldPen = pDC->SelectObject(&SelectPen);
		pDC->Ellipse(pt.x-width,
					 pt.y-width,
					 pt.x+width,
					 pt.y+width);
		pDC->SelectObject(pOldPen);
	}
	else if(!IsPrinting && m_iHighlight==i) {
		CPen HighlightPen(PS_SOLID, 2, RGB(255,0,0));
		CPen *pOldPen = pDC->SelectObject(&HighlightPen);
		pDC->Ellipse(pt.x-width,
					 pt.y-width,
					 pt.x+width,
					 pt.y+width);
		pDC->SelectObject(pOldPen);
	}
	else pDC->Ellipse(pt.x-width,
					  pt.y-width,
					  pt.x+width,
					  pt.y+width);
	return true;
}


bool CSplinedPoints::InsertPoint(double x, double y){
	if (x>=0. && x<=1.){//No points yet
		if(m_iPoints<0){
			m_iPoints=0;
			m_ctrlPoint[0].x = x;
			m_ctrlPoint[0].y = y;
		}
		else{
			if(x<m_ctrlPoint[0].x){// if we're the new minimum point
				m_iPoints++;
				for (int j=m_iPoints; j>=0; j--){
					m_ctrlPoint[j] = m_ctrlPoint[j-1];
				}
				m_ctrlPoint[0].x = x;
				m_ctrlPoint[0].y = y;
			}
			else{// if we're the new maximum point
				if(x>=m_ctrlPoint[m_iPoints].x){
					m_iPoints++;
					m_ctrlPoint[m_iPoints].x = x;
					m_ctrlPoint[m_iPoints].y = y;
				}
				else{// else if we're in between
					for (int k=0; k<m_iPoints; k++){
						if (x>m_ctrlPoint[k].x && x<m_ctrlPoint[k+1].x){
							m_iPoints++;
							for (int j=m_iPoints; j>k+1; j--){
								m_ctrlPoint[j] = m_ctrlPoint[j-1];
							}
							m_ctrlPoint[k+1].x = x;
							m_ctrlPoint[k+1].y = y;
						}
					}
				}
			}
		}
	}
	CompSlopes();
	return true;
}

bool CSplinedPoints::RemovePoint(int k){
	if (k>0 && k<m_iPoints){
		for (int j=k; j<=m_iPoints; j++){
			m_ctrlPoint[j] = m_ctrlPoint[j+1];
		}
		m_iPoints--;
	}
	CompSlopes();
	return true;
}



double CSplinedPoints::GetY(double x){
	CSpline LinkSpline;
	LinkSpline.m_iRes = m_Freq;
	LinkSpline.m_iCtrlPoints = -1;
	for (int k=0; k<m_iPoints; k++){
		if(fabs(x-m_ctrlPoint[k].x )<0.001f) return m_ctrlPoint[k].y;
		if (m_ctrlPoint[k].x < x && x<=m_ctrlPoint[k+1].x){
			LinkSpline.InsertPoint(m_ctrlPoint[k].x, m_ctrlPoint[k].y);
			LinkSpline.InsertPoint(m_ctrlPoint[k].x + (m_ctrlPoint[k+1].x-m_ctrlPoint[k].x)/3.f * m_Slope[k].x,
								   m_ctrlPoint[k].y + (m_ctrlPoint[k+1].x-m_ctrlPoint[k].x)/3.f * m_Slope[k].y);
			LinkSpline.InsertPoint(m_ctrlPoint[k+1].x - (m_ctrlPoint[k+1].x-m_ctrlPoint[k].x)/3.f * m_Slope[k+1].x,
								   m_ctrlPoint[k+1].y - (m_ctrlPoint[k+1].x-m_ctrlPoint[k].x)/3.f * m_Slope[k+1].y);
			LinkSpline.InsertPoint(m_ctrlPoint[k+1].x, m_ctrlPoint[k+1].y);
			LinkSpline.SplineKnots();
			LinkSpline.SplineCurve();
			double y = LinkSpline.GetY(x);
			return y;
		}
	}
	return 0.f;
}




bool CSplinedPoints::DrawSplines(CDC* pDC, double scalex, double scaley, CPoint Offset,
								 bool IsPrinting)
{
	int l;
	CSpline LinkSpline;
	LinkSpline.m_iRes = m_Freq;
	LinkSpline.m_iDegree = 3;
	LinkSpline.m_iCtrlPoints = 3;
	LinkSpline.SplineKnots();
	LinkSpline.m_rViewRect.CopyRect(&m_rViewRect);
	LOGBRUSH lb;
	lb.lbStyle = BS_SOLID;
	lb.lbColor = RGB(200,200,200);


	for (int k=0; k<m_iPoints; k++){

		double dist = (double)fabs(m_ctrlPoint[k+1].x-m_ctrlPoint[k].x);
		LinkSpline.m_iCtrlPoints = -1;
		LinkSpline.AddPoint(m_ctrlPoint[k].x, m_ctrlPoint[k].y);
		LinkSpline.AddPoint(m_ctrlPoint[k].x   + (dist)/3.0 * m_Slope[k].x,
						    m_ctrlPoint[k].y   + (dist)/3.0 * m_Slope[k].y);
		LinkSpline.AddPoint(m_ctrlPoint[k+1].x - (dist)/3.0 * m_Slope[k+1].x,
							m_ctrlPoint[k+1].y - (dist)/3.0 * m_Slope[k+1].y);
		LinkSpline.AddPoint(m_ctrlPoint[k+1].x, m_ctrlPoint[k+1].y);
		LinkSpline.SplineCurve();
		LinkSpline.DrawSpline(pDC, scalex, scaley, Offset, IsPrinting);

		if(m_bOutPts){
			for (int k=1; k<LinkSpline.m_iRes-1; k++){
				LinkSpline.DrawOutputPoint(pDC,k,scalex,scaley,Offset, IsPrinting);
			}
		}
		
		double x1 = __min(m_ctrlPoint[k].x, m_ctrlPoint[k+1].x);
		double x2 = __max(m_ctrlPoint[k].x, m_ctrlPoint[k+1].x);

		while(x1>=0 && x1<=x2 && x2<=1.0){
			l=(int)(x1*1000.001);
			m_Outy[l] = LinkSpline.GetY(x1);
			x1+=0.001;
		}
		l=(int)(x2*1000);
		m_Outy[l] = LinkSpline.GetY(x2);
	}

	return true;
}



bool CSplinedPoints::CompSlopes()
{
	double val;
	//infinite slope at origin, set in CPF
	//because depends on whether upper or lower surface
	m_Slope[m_iPoints].x = m_RearPoint.x-1.f;
	m_Slope[m_iPoints].y = m_RearPoint.y-0.f;
	val = m_Slope[m_iPoints].x*m_Slope[m_iPoints].x + m_Slope[m_iPoints].y * m_Slope[m_iPoints].y;
	val = (double) sqrt(val);
	if(fabs(val)>0.0000001f){
		m_Slope[m_iPoints].x = m_Slope[m_iPoints].x /val;
		m_Slope[m_iPoints].y = m_Slope[m_iPoints].y /val;
	}
	else{// anything, points are merged
		m_Slope[m_iPoints].x = (double)sqrt(1.f/2.f);
		m_Slope[m_iPoints].y = (double)sqrt(1.f/2.f);
	}
	for (int k=1; k<m_iPoints; k++){
		m_Slope[k].x = m_ctrlPoint[k+1].x-m_ctrlPoint[k-1].x;
		m_Slope[k].y = m_ctrlPoint[k+1].y-m_ctrlPoint[k-1].y;
		val = m_Slope[k].x*m_Slope[k].x + m_Slope[k].y * m_Slope[k].y;
		val = (double) sqrt(val);
		if(fabs(val)>0.0000001f){
			m_Slope[k].x = m_Slope[k].x /val;
			m_Slope[k].y = m_Slope[k].y /val;
		}
		else{// anything, points are merged
			m_Slope[k].x = (double)sqrt(1.f/2.f);
			m_Slope[k].y = (double)sqrt(1.f/2.f);
		}		
	}
	return true;
}

void CPF::Update(bool bExtrados)
{
	m_Extrados.CompSlopes();
	m_Intrados.CompSlopes();
	m_OutPoints = m_Extrados.m_iPoints + m_Intrados.m_iPoints;

}

int CSplinedPoints::IsControlPoint(CVector Real, double ZoomFactor)
{
	for (int k=0; k<=m_iPoints; k++){
		if (fabs(Real.x-m_ctrlPoint[k].x)<0.003f/ZoomFactor &&
			fabs(Real.y-m_ctrlPoint[k].y)<0.003f/ZoomFactor) return k;
	}
	return -10;
}


int CSplinedPoints::IsControlPoint(double x, double y, double Zoom)
{
	for (int k=0; k<=m_iPoints; k++){
		if (fabs(x-m_ctrlPoint[k].x)/Zoom<0.003f &&
			fabs(y-m_ctrlPoint[k].y)/Zoom<0.003f) return k;
	}
	return -10;
}


int CSplinedPoints::IsRearPoint(CVector Real, double ZoomFactor)
{
	if (fabs(Real.x-m_RearPoint.x)<0.003f/ZoomFactor &&
		fabs(Real.y-m_RearPoint.y)<0.003f/ZoomFactor) return -1;
	return -10;
}


void CSplinedPoints::DrawRearPoint(CDC *pDC, double scalex, double scaley, CPoint Offset, bool IsPrinting)
{
	double scale = scalex;
	if (!IsPrinting){
		if(m_iHighlight ==-1){
			CPen RearPen(PS_SOLID, 2, RGB(255,0,0));
			CPen *pOldPen = pDC->SelectObject(&RearPen);
			pDC->Ellipse((int)( m_RearPoint.x*scale+Offset.x-3),
						 (int)(-m_RearPoint.y*scale+Offset.y-3),
						 (int)( m_RearPoint.x*scale+Offset.x+3),
						 (int)(-m_RearPoint.y*scale+Offset.y+3));
			pDC->SelectObject(pOldPen);
		}
		else if(m_iSelect ==-1){
			CPen RearPen(PS_SOLID, 2, RGB(0,0,150));
			CBrush RearBrush(RGB(186,100,100));
			CPen *pOldPen = pDC->SelectObject(&RearPen);
			pDC->Ellipse((int)( m_RearPoint.x*scale+Offset.x-3),
						 (int)(-m_RearPoint.y*scale+Offset.y-3),
						 (int)( m_RearPoint.x*scale+Offset.x+3),
						 (int)(-m_RearPoint.y*scale+Offset.y+3));
			pDC->SelectObject(pOldPen);
		}
		else{
			CPen RearPen(PS_SOLID, 1, RGB(100,100,100));
			CPen *pOldPen = pDC->SelectObject(&RearPen);
			pDC->Ellipse((int)( m_RearPoint.x*scale+Offset.x-3),
						 (int)(-m_RearPoint.y*scale+Offset.y-3),
						 (int)( m_RearPoint.x*scale+Offset.x+3),
						 (int)(-m_RearPoint.y*scale+Offset.y+3));
			pDC->SelectObject(pOldPen);
		}
	}
}


void CPF::SetOutPoints(bool state)
{
	m_Extrados.m_bOutPts = state;
	m_Intrados.m_bOutPts = state;
}


void CPF::ExportToBuffer(CFoil *pFoil)
{
	m_Extrados.ExportToBuffer(pFoil, true);
	m_Intrados.ExportToBuffer(pFoil, false);
	memcpy(pFoil->xb, pFoil->x, sizeof(pFoil->x));
	memcpy(pFoil->yb, pFoil->y, sizeof(pFoil->y));
	pFoil->nb = pFoil->n;
}


void CSplinedPoints::Export(CStdioFile *pFile, bool bExtrados)
{
	CString strOut;
	CSpline LinkSpline;
	LinkSpline.m_iRes = m_Freq;
	LinkSpline.m_rViewRect.CopyRect(&m_rViewRect);

	if(bExtrados){
		for (int k=m_iPoints-1;k>=0; k--){
			LinkSpline.m_iCtrlPoints = -1;
			LinkSpline.InsertPoint(m_ctrlPoint[k].x, m_ctrlPoint[k].y);
			LinkSpline.InsertPoint(m_ctrlPoint[k].x + (m_ctrlPoint[k+1].x-m_ctrlPoint[k].x)/3.f * m_Slope[k].x,
								   m_ctrlPoint[k].y + (m_ctrlPoint[k+1].x-m_ctrlPoint[k].x)/3.f * m_Slope[k].y);
			LinkSpline.InsertPoint(m_ctrlPoint[k+1].x - (m_ctrlPoint[k+1].x-m_ctrlPoint[k].x)/3.f * m_Slope[k+1].x,
								   m_ctrlPoint[k+1].y - (m_ctrlPoint[k+1].x-m_ctrlPoint[k].x)/3.f * m_Slope[k+1].y);
			LinkSpline.InsertPoint(m_ctrlPoint[k+1].x, m_ctrlPoint[k+1].y);
			LinkSpline.SplineKnots();
			LinkSpline.SplineCurve();
			for (int j=LinkSpline.m_iRes-1;j>=1; j--){
				strOut.Format(" %7.4f  %7.4f\n", LinkSpline.m_Output[j].x,
												 LinkSpline.m_Output[j].y);
				pFile->WriteString(strOut);
			}
		}
	}
	else {
		for (int k=0; k<m_iPoints; k++){
			LinkSpline.m_iCtrlPoints = -1;
			LinkSpline.InsertPoint(m_ctrlPoint[k].x, m_ctrlPoint[k].y);
			LinkSpline.InsertPoint(m_ctrlPoint[k].x + (m_ctrlPoint[k+1].x-m_ctrlPoint[k].x)/3.f * m_Slope[k].x,
								   m_ctrlPoint[k].y + (m_ctrlPoint[k+1].x-m_ctrlPoint[k].x)/3.f * m_Slope[k].y);
			LinkSpline.InsertPoint(m_ctrlPoint[k+1].x - (m_ctrlPoint[k+1].x-m_ctrlPoint[k].x)/3.f * m_Slope[k+1].x,
								   m_ctrlPoint[k+1].y - (m_ctrlPoint[k+1].x-m_ctrlPoint[k].x)/3.f * m_Slope[k+1].y);
			LinkSpline.InsertPoint(m_ctrlPoint[k+1].x, m_ctrlPoint[k+1].y);
			LinkSpline.SplineKnots();
			LinkSpline.SplineCurve();
			for (int j=0; j<LinkSpline.m_iRes-1;j++){
				strOut.Format(" %7.4f  %7.4f\n", LinkSpline.m_Output[j].x,
												 LinkSpline.m_Output[j].y);
				pFile->WriteString(strOut);
			}
		}
		strOut.Format(" %7.4f  %7.4f\n", 1.0,0.);
		pFile->WriteString(strOut);
	}
}

void CSplinedPoints::ExportToBuffer(CFoil *pFoil, bool bExtrados){
	CString strOut;
	CSpline LinkSpline;
	LinkSpline.m_iRes = m_Freq;
	LinkSpline.m_rViewRect.CopyRect(&m_rViewRect);
	if(bExtrados){
		pFoil->n = 0;
		for (int k=m_iPoints-1;k>=0; k--){
			LinkSpline.m_iCtrlPoints = -1;
			LinkSpline.InsertPoint(m_ctrlPoint[k].x, m_ctrlPoint[k].y);
			LinkSpline.InsertPoint(m_ctrlPoint[k].x + (m_ctrlPoint[k+1].x-m_ctrlPoint[k].x)/3.f * m_Slope[k].x,
								   m_ctrlPoint[k].y + (m_ctrlPoint[k+1].x-m_ctrlPoint[k].x)/3.f * m_Slope[k].y);
			LinkSpline.InsertPoint(m_ctrlPoint[k+1].x - (m_ctrlPoint[k+1].x-m_ctrlPoint[k].x)/3.f * m_Slope[k+1].x,
								   m_ctrlPoint[k+1].y - (m_ctrlPoint[k+1].x-m_ctrlPoint[k].x)/3.f * m_Slope[k+1].y);
			LinkSpline.InsertPoint(m_ctrlPoint[k+1].x, m_ctrlPoint[k+1].y);
			LinkSpline.SplineKnots();
			LinkSpline.SplineCurve();
			for (int j=LinkSpline.m_iRes-1;j>=1; j--){
				pFoil->x[pFoil->n] = LinkSpline.m_Output[j].x;
				pFoil->y[pFoil->n] = LinkSpline.m_Output[j].y;
				pFoil->n++;
			}
		}
		
	}
	else {
		for (int k=0; k<m_iPoints; k++){
			LinkSpline.m_iCtrlPoints = -1;
			LinkSpline.InsertPoint(m_ctrlPoint[k].x, m_ctrlPoint[k].y);
			LinkSpline.InsertPoint(m_ctrlPoint[k].x + (m_ctrlPoint[k+1].x-m_ctrlPoint[k].x)/3.f * m_Slope[k].x,
								   m_ctrlPoint[k].y + (m_ctrlPoint[k+1].x-m_ctrlPoint[k].x)/3.f * m_Slope[k].y);
			LinkSpline.InsertPoint(m_ctrlPoint[k+1].x - (m_ctrlPoint[k+1].x-m_ctrlPoint[k].x)/3.f * m_Slope[k+1].x,
								   m_ctrlPoint[k+1].y - (m_ctrlPoint[k+1].x-m_ctrlPoint[k].x)/3.f * m_Slope[k+1].y);
			LinkSpline.InsertPoint(m_ctrlPoint[k+1].x, m_ctrlPoint[k+1].y);
			LinkSpline.SplineKnots();
			LinkSpline.SplineCurve();
			for (int j=0; j<LinkSpline.m_iRes-1;j++){
				pFoil->x[pFoil->n] = LinkSpline.m_Output[j].x;
				pFoil->y[pFoil->n] = LinkSpline.m_Output[j].y;
				pFoil->n++;
			}
		}
		pFoil->x[pFoil->n] = 1.0f;
		pFoil->y[pFoil->n] = 0.0f;	
		pFoil->n++;
	}
}


bool CPF::LoadFile(CStdioFile *pFile)
{
	CString strIn;
	m_bModified = false;
	m_Extrados.m_iPoints = -1;
	m_Intrados.m_iPoints = -1;

	// read FoilColor
	pFile->ReadString(strIn);
	sscanf(strIn, "%d", &m_FoilColor);

	// number of points on extrados
	pFile->ReadString(strIn);
	sscanf(strIn, "%d", &m_Extrados.m_iPoints);
	// then extrados coords
	for (int k=0; k<=m_Extrados.m_iPoints;k++){
		pFile->ReadString(strIn);
		sscanf(strIn,"%lf%lf", &m_Extrados.m_ctrlPoint[k].x, &m_Extrados.m_ctrlPoint[k].y);
	}
	// number of points on intrados
	pFile->ReadString(strIn);
	sscanf(strIn, "%d", &m_Intrados.m_iPoints);
	// then intrados coords
	for (k=0; k<=m_Intrados.m_iPoints;k++){
		pFile->ReadString(strIn);
		sscanf(strIn,"%lf%lf", &m_Intrados.m_ctrlPoint[k].x, &m_Intrados.m_ctrlPoint[k].y);
	}

	// extrados rear point
	pFile->ReadString(strIn);
	sscanf(strIn,"%lf%lf", &m_Extrados.m_RearPoint.x, &m_Extrados.m_RearPoint.y);
	// intrados rear point
	sscanf(strIn,"%lf%lf", &m_Intrados.m_RearPoint.x, &m_Intrados.m_RearPoint.y);
	Update(true);
	Update(false);
	return true;
}

bool CPF::SaveFile(CStdioFile *pFile)
{
	CString strong;
	// FoilName
	strong = m_strFoilName + "\n";
	pFile->WriteString(strong);

	//FoilColor
	strong.Format("%d\n", m_FoilColor);
	pFile->WriteString(strong);

	// number of points on extrados (minus 1, C++ convention ?)
	strong.Format("%d\n", m_Extrados.m_iPoints);
	pFile->WriteString(strong);
	// then extrados coords
	for (int k=0; k<=m_Extrados.m_iPoints;k++){
		strong.Format(" %f  %f\n", 
			m_Extrados.m_ctrlPoint[k].x, m_Extrados.m_ctrlPoint[k].y);
		pFile->WriteString(strong);
	}

	// number of points on intrados (minus 1, C++ convention ?)
	strong.Format("%d\n", m_Intrados.m_iPoints);
	pFile->WriteString(strong);

	// then extrados coords
	for (k=0; k<=m_Intrados.m_iPoints;k++){
		strong.Format(" %f  %f\n", 
			m_Intrados.m_ctrlPoint[k].x, m_Intrados.m_ctrlPoint[k].y);
		pFile->WriteString(strong);
	}

	// extrados rear point
	strong.Format("%f %f\n", m_Extrados.m_RearPoint.x, m_Extrados.m_RearPoint.y);
	pFile->WriteString(strong);
	// intrados rear point
	strong.Format("%f %f\n",  m_Intrados.m_RearPoint.x, m_Intrados.m_RearPoint.y);
	pFile->WriteString(strong);
	
	m_bModified = false;
	return true;
}


bool CPF::Serialize(CArchive &ar)
{
	//starting with v3.05
	if(ar.IsLoading()){
		float f;
		int ArchiveFormat,k ;
		ar >> ArchiveFormat;
		if(ArchiveFormat  != 100305) 
			return false;
		ar >> m_strFoilName; m_strFoilName = "Splined Points Foil";
		ar >> m_FoilColor >> m_FoilStyle >> m_FoilWidth;
		ar >> m_Extrados.m_iPoints;
		for (k=0; k<=m_Extrados.m_iPoints;k++){
			ar >> f; m_Extrados.m_ctrlPoint[k].x =f;
			ar >> f; m_Extrados.m_ctrlPoint[k].y =f;
		}

		// number of points on intrados (minus 1, C++ convention ?)
		ar >> m_Intrados.m_iPoints;
		for (k=0; k<=m_Intrados.m_iPoints;k++){
			ar >> f; m_Intrados.m_ctrlPoint[k].x =f;
			ar >> f; m_Intrados.m_ctrlPoint[k].y =f;
		}

		ar >> f; m_Extrados.m_RearPoint.x =f;
		ar >> f; m_Extrados.m_RearPoint.y =f;
		ar >> f; m_Intrados.m_RearPoint.x =f;
		ar >> f; m_Intrados.m_RearPoint.y =f;

		ar >> k;
		if(k !=0 && k !=1){
			CArchiveException *pfe = new CArchiveException(CArchiveException::badIndex);
			throw pfe;
		}
		if(k) m_bVisible = true; else m_bVisible = false;

		ar >> k;
		if(k !=0 && k !=1){
			CArchiveException *pfe = new CArchiveException(CArchiveException::badIndex);
			throw pfe;
		}
		if(k) m_bOutPoints = true; else m_bOutPoints = false;

		ar >> k;
		if(k !=0 && k !=1){
			CArchiveException *pfe = new CArchiveException(CArchiveException::badIndex);
			throw pfe;
		}
		if(k) m_bCenterLine = true; else m_bCenterLine = false;

		Update(true);
		Update(false);
		m_bModified = false;
		return true;
	}
	else{
		ar << 100305;
		ar << m_strFoilName;
		ar << m_FoilColor << m_FoilStyle << m_FoilWidth;
		ar << m_Extrados.m_iPoints;
		for (int k=0; k<=m_Extrados.m_iPoints;k++){
			ar << (float)m_Extrados.m_ctrlPoint[k].x << (float)m_Extrados.m_ctrlPoint[k].y;
		}

		// number of points on intrados (minus 1, C++ convention ?)
		ar << m_Intrados.m_iPoints;
		for (k=0; k<=m_Intrados.m_iPoints;k++){
			ar << (float)m_Intrados.m_ctrlPoint[k].x << (float)m_Intrados.m_ctrlPoint[k].y;
		}

		ar << (float)m_Extrados.m_RearPoint.x << (float)m_Extrados.m_RearPoint.y;
		ar << (float)m_Intrados.m_RearPoint.x << (float)m_Intrados.m_RearPoint.y;	

		if(m_bVisible)    ar<<1; else ar <<0;
		if(m_bOutPoints)  ar<<1; else ar <<0;
		if(m_bCenterLine) ar<<1; else ar <<0;

		m_bModified = false;
		return true;
	}
}