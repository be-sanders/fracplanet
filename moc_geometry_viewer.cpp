// Source file for fracplanet
// Copyright (C) 2002 Tim Day
/*
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
/****************************************************************************
** GeometryViewerViewPane meta object code from reading C++ file 'geometry_viewer.h'
**
** Created: Tue Dec 3 15:23:15 2002
**      by: The Qt MOC ($Id: moc_geometry_viewer.cpp,v 1.1.1.1 2002-12-31 14:31:22 timday Exp $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "geometry_viewer.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 19)
#error "This file was generated using the moc from 3.0.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *GeometryViewerViewPane::className() const
{
    return "GeometryViewerViewPane";
}

QMetaObject *GeometryViewerViewPane::metaObj = 0;
static QMetaObjectCleanUp cleanUp_GeometryViewerViewPane;

#ifndef QT_NO_TRANSLATION
QString GeometryViewerViewPane::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GeometryViewerViewPane", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString GeometryViewerViewPane::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GeometryViewerViewPane", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* GeometryViewerViewPane::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QGLWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"setElevation", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"setSpinRate", 1, param_slot_1 };
    static const QUMethod slot_2 = {"tick", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "setElevation(int)", &slot_0, QMetaData::Public },
	{ "setSpinRate(int)", &slot_1, QMetaData::Public },
	{ "tick()", &slot_2, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"GeometryViewerViewPane", parentObject,
	slot_tbl, 3,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_GeometryViewerViewPane.setMetaObject( metaObj );
    return metaObj;
}

void* GeometryViewerViewPane::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "GeometryViewerViewPane" ) ) return (GeometryViewerViewPane*)this;
    return QGLWidget::qt_cast( clname );
}

bool GeometryViewerViewPane::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: setElevation((int)static_QUType_int.get(_o+1)); break;
    case 1: setSpinRate((int)static_QUType_int.get(_o+1)); break;
    case 2: tick(); break;
    default:
	return QGLWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool GeometryViewerViewPane::qt_emit( int _id, QUObject* _o )
{
    return QGLWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool GeometryViewerViewPane::qt_property( int _id, int _f, QVariant* _v)
{
    return QGLWidget::qt_property( _id, _f, _v);
}
#endif // QT_NO_PROPERTIES
