#include "ModelElements.h"

#include "Elements.h"
using namespace vtk;

#include <QFont>
#include <QPalette>
#include <QGuiApplication>

const unsigned short ModelElements::NobleGases[] = {2, 10, 18, 36, 54, 86, 118, NumberOfElements + 1};
//                                                  He Ne  Ar  Kr  Xe  Rn  Og    <finish>

const unsigned short ModelElements::InGroup[] =
    {
        1, /*  H                                             He */ 18 // He
        ,
        1, 2 /* Be                       B */, 13, 14, 15, 16, 17, 18 // Ne
        ,
        1, 2 /* Mg                      Al */, 13, 14, 15, 16, 17, 18 // Ar
        ,
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18 // Kr
        ,
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18 // Xe
        ,
        1, 2 // Ba
        ,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 // Ln: La-Lu
        ,
        4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18 // Rn
        ,
        1, 2 // Fr-Ra
        ,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 // Aq: Ac-Lr
        ,
        4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18 // Og
};

const vtkIdType ModelElements::TableForm[] =
    {
        // 1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18
        1, -1, -1, -1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, 2,
        3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 5, 6, 7, 8, 9, 10,
        11, 12, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 13, 14, 15, 16, 17, 18,
        19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36,
        37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54,
        55, 56, -2, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86,
        87, 88, -3, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118,
        // 119,120, -4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -2, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, -1,
        -1, -3, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, -1,
        // -1, -4,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135, -1
};

const vtkIdType ModelElements::NumTable = sizeof(ModelElements::TableForm) / sizeof(ModelElements::TableForm[0]);

ModelElements::ModelElements(QObject *parent)
    : QAbstractTableModel(parent) {}

QVariant ModelElements::data(const QModelIndex &index, int role) const
{
  // cast index.row() and index.column() into the idElement in the Table;
  vtkIdType ofs = index.row() * NumberOfGroups + index.column();
  if (ofs > NumTable)
    return QVariant();
  vtkIdType idElem = TableForm[ofs];
  if (role == Qt::DisplayRole)
  {
    QVariant res;
    {
      if (idElem >= 0)
      {
        res.setValue(tr(Elements::GetElementSymbol(idElem).c_str()));
      }
      else if (idElem == -2)
        res.setValue(tr("[L]")); // lanthanides
      else if (idElem == -3)
        res.setValue(tr("[A]")); // actinides
      else if (idElem == -4)
        res.setValue(tr("[S]")); // superactinides
    }
    return res;
  }
  else if (role == Qt::BackgroundRole)
  {
    return (idElem < 0 || index.row() > NumberOfPeriods)
               ? qApp->palette().base()
               : qApp->palette().alternateBase();
  }
  else if (role == Qt::TextAlignmentRole)
  {
    return QVariant(Qt::AlignCenter);
  }
  else if (role == Qt::SizeHintRole)
  {
    QFont font;
    return QVariant();
  }
  else
    return QVariant();
}

Qt::ItemFlags ModelElements::flags(const QModelIndex &index) const
{
  Qt::ItemFlags res; // (QAbstractTableModel::flags(index));

  vtkIdType idItem = index.row() * NumberOfGroups + index.column();

  if (TableForm[idItem] >= 0)
    res |= (Qt::ItemIsEnabled | Qt::ItemIsSelectable);

  return res;
}