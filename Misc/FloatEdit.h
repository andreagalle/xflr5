/****************************************************************************

	FloatEdit Class
	Copyright (C) 2008-2008 Andre Deperrois xflr5@yahoo.com

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

#ifndef FLOATEDIT_H
#define FLOATEDIT_H

#include <QLineEdit>
#include <QString>
#include <QKeyEvent>

class FloatEdit : public QLineEdit
{
public:
	static int s_MaxWidth;

	FloatEdit(QWidget *pParent  = NULL);
	FloatEdit(QString str, int precision=2);

	double GetValue();
	bool GetValue(double &f);
	bool CheckBeforeExit();
	void Clear();
	void SetMin(double f);
	void SetMax(double f);
	void SetValue(double f);
	void SetPrecision(int i);
	void focusInEvent ( QFocusEvent * event );
	void focusOutEvent ( QFocusEvent * event );
	void keyPressEvent(QKeyEvent *event);
	void showEvent ( QShowEvent * event );

	double m_Value;
	int m_iPrecision;
	double m_fMin, m_fMax;
	QString m_iniStr;

signals:


};

#endif // FLOATEDIT_H