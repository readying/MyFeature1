/****************************************************************************
** Meta object code from reading C++ file 'g2.h'
**
** Created: Thu Jun 25 10:11:08 2015
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../g2.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'g2.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_G2[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       8,    3,    4,    3, 0x0a,
      23,    3,    3,    3, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_G2[] = {
    "G2\0\0int\0SmileExtract()\0OpenWav()\0"
};

const QMetaObject G2::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_G2,
      qt_meta_data_G2, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &G2::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *G2::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *G2::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_G2))
        return static_cast<void*>(const_cast< G2*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int G2::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: { int _r = SmileExtract();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 1: OpenWav(); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
