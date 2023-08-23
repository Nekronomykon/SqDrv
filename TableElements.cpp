#include "TableElements.h"

#include <QFont>
#include <QFontMetrics>

TableElements::TableElements(QWidget *parent)
    : _Base(parent)
{
  this->setSelectionMode(QAbstractItemView::SingleSelection);
  this->setShowGrid(false);

  QFont font(tr("Monospace"), 12, QFont::Bold);
  this->setFont(font);
  QFontMetrics fm(font);

  int w1 = fm.horizontalAdvance(QLatin1Char('9')) * 4;
  int h1 = fm.height(); // * 3 / 2;

  Model *pM = this->GetViewModel();

  //  How to calculate the sizes? Using sizeHint?
  //
  for (int j = 0; j <= pM->columnCount();
       this->setColumnWidth(j++, w1))
    ;

  for (int j = 0; j <= pM->rowCount();
       this->setRowHeight(j++, h1))
    ;

  int kwidth = (pM->columnCount() + 1) * w1;
  int kheight = (pM->rowCount() + 3) * h1;

  this->setGeometry(parent->x(), parent->y(), kwidth, kheight);
}
