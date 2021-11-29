#include "SideBarDecorator.h"

#include <QFont>
#include <QPainter>
#include <QTextStream>
#include <QTextBlock>

#include <QDir>
#include <QFile>
#include <QFileInfo>

#include <QMessageBox>

#include "ResetCursor.h"

#include "EditCode.h"

//![constructor]

EditCode::EditCode(QWidget *parent)
    : QPlainTextEdit(parent)
    , side_bar_(new SideBarDecorator(this))
    , highlight_(new HighlightBrackets(this->document()))
    , fontSrc_("Monospace", 10)
{
  fontSrc_.setStyleHint(QFont::Monospace);
  this->setFont(fontSrc_);
  // this->setReadOnly(true); // the ViewCodeFirst paradigm ;)

  connect(this, &EditCode::blockCountChanged, this, &EditCode::updateSideBarWidth);
  connect(this, SIGNAL(updateRequest(QRect, int)), this, SLOT(updateSideBar(QRect, int)));
  connect(this, &EditCode::cursorPositionChanged, this, &EditCode::updateCursorEnvironment);

  this->updateSideBarWidth(100);
}

//![constructor]

//![extraAreaWidth]

int EditCode::SideBarWidth()
{
  int num = qMax(1, this->blockCount());
  int digits = 1;
  while (num /= 10)
    ++digits;

  int space = this->fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits + 2 * this->fontMetrics().height() + 4;
  // both sides paddings
  return space;
}

//![extraAreaWidth]

//![slotUpdateExtraAreaWidth]

void EditCode::updateSideBarWidth(int /* newBlockCount */)
{
  this->setViewportMargins(this->SideBarWidth(), 0, 0, 0);
}

//![slotUpdateExtraAreaWidth]

//![slotUpdateRequest]

void EditCode::updateSideBar(const QRect &rect, int dy)
{
  if (dy)
    side_bar_->scroll(0, dy);
  else
    side_bar_->update(0, rect.y(), side_bar_->width(), rect.height());

  if (rect.contains(this->viewport()->rect()))
    this->updateSideBarWidth(0);
}

//![slotUpdateRequest]

//![resizeEvent]
void EditCode::resizeEvent(QResizeEvent *e)
{
  QPlainTextEdit::resizeEvent(e);

  QRect cr = this->contentsRect();
  side_bar_->setGeometry(QRect(cr.left(), cr.top(), this->SideBarWidth(), cr.height()));
}

//![resizeEvent]

//![cursorPositionChanged]
void EditCode::updateCursorEnvironment()
{
  QTextCursor cursor(this->textCursor());
  cursor.clearSelection();

  this->highlightCurrentLine();
  this->highlightMatchBrackets();
  // [*]
}

void EditCode::highlightCurrentLine()
{
  QTextCursor cursor(this->textCursor());
  ListExtraSelection list_sel;
  ExtraSel sel;

  QColor lineColor(this->isReadOnly() ? Qt::darkGray : Qt::yellow);

  sel.format.setBackground(lineColor.lighter(128));
  sel.format.setProperty(QTextFormat::FullWidthSelection, true);
  sel.cursor = cursor;
  list_sel.append(sel);
  this->setExtraSelections(list_sel);
}

//![cursorPositionChanged]

//![extraAreaPaintEvent_0]

void EditCode::SideBarDecoratorPaintEvent(QPaintEvent *event)
{
  QPainter painter(side_bar_);
  painter.fillRect(event->rect(), Qt::lightGray);

  //![extraAreaPaintEvent_0]

  //![extraAreaPaintEvent_1]
  QTextBlock block = this->firstVisibleBlock();
  int blockNumber = block.blockNumber();
  int top = (int)this->blockBoundingGeometry(block).translated(contentOffset()).top();
  int bottom = top + (int)this->blockBoundingRect(block).height();
  //![extraAreaPaintEvent_1]

  int nH = this->fontMetrics().height() + 1;
  //![extraAreaPaintEvent_2]
  while (block.isValid() && top <= event->rect().bottom())
  {

    if (block.isVisible() && bottom >= event->rect().top())
    {
      QString number = QString::number(blockNumber + 1);
      painter.setPen(Qt::black);
      painter.drawText(nH, top, side_bar_->width() - 2 * nH, nH - 1, Qt::AlignRight // | Qt::AlignVCenter
                       ,
                       number);
    }

    block = block.next();
    top = bottom;
    bottom = top + (int)blockBoundingRect(block).height();
    ++blockNumber;
  }
}
//![extraAreaPaintEvent_2]

void EditCode::load(QIODevice *pIO)
{
  QTextStream in(pIO);
  ResetCursor rc(Qt::WaitCursor);
  this->setPlainText(in.readAll());
}

void EditCode::save(QIODevice *pIO)
{
  QTextStream out(pIO);
  ResetCursor rc(Qt::WaitCursor);
  out << this->toPlainText();
}

bool EditCode::loadFromPath(const QString &fileName)
{
  assert(!fileName.isEmpty());

  QFile file(fileName);

  if (!file.open(QFile::ReadOnly | QFile::Text))
    return false;

  this->load(&file);
  // this->setReadOnly(true); // to prevent source from the occasional damage;
  // NEDODELANO! Isn't it the right place here to put it?
  this->show();

  return true;
}

bool EditCode::saveToPath(const QString &fileName)
{
  assert(!fileName.isEmpty());

  QFile file(fileName);

  if (!file.open(QFile::WriteOnly | QFile::Text))
    return false;

  this->save(&file);
  return true;
}

bool EditCode::matchToRight(QTextBlock block, char cWhat, int iFrom, int iDepth, char cFrom)
{
  InfoTextBlockBrackets *pInfo = static_cast<InfoTextBlockBrackets *>(block.userData());
  const InfoTextBlockBrackets::Symbolics &sym = pInfo->getSymbolics();

  int docPos = block.position();
  for (; iFrom < sym.size(); ++iFrom)
  {
    const SymbolPosition &info = sym[iFrom];

    if (info.character == cFrom) // go deeper
    {
      ++iDepth;
      continue;
    }

    if (info.character == cWhat) // pop it up --> here is the cWhat use:
    {
      if (iDepth <= 0)
      {
        this->selectSymbolAt(docPos + info.position);
        return true;
      }
      // else if (iDepth > 0)
        --iDepth;
    }
  }

  block = block.next();
  return (block.isValid()) ? this->matchToRight(block, cWhat, 0, iDepth, cFrom) : false;
}

bool EditCode::matchToLeft(QTextBlock block, char cWhat, int iFrom, int iDepth, char cFrom)
{
  InfoTextBlockBrackets *pInfo = static_cast<InfoTextBlockBrackets *>(block.userData());
  const InfoTextBlockBrackets::Symbolics &sym = pInfo->getSymbolics();
  if (iFrom < 0)
    iFrom = sym.size() - 1;

  int docPos = block.position();
  for (; iFrom >= 0; --iFrom)
  {
    const SymbolPosition &info = sym[iFrom];

    if (info.character == cFrom) // go deeper
    {
      ++iDepth;
      continue;
    }

    if (info.character == cWhat) // pop it up --> here is the cWhat use:
    {
      if (iDepth <= 0)
      {
        this->selectSymbolAt(docPos + info.position);
        return true;
      }
      // else if (iDepth > 0)
        --iDepth;
    }
  }

  block = block.previous();
  return (block.isValid()) ? this->matchToLeft(block, cWhat, -1, iDepth, cFrom) : false;
}

void EditCode::highlightMatchBrackets()
{
  QTextCursor cursor(this->textCursor());
  QTextBlock curBlock = cursor.block();

  InfoTextBlockBrackets *pData = static_cast<InfoTextBlockBrackets *>(cursor.block().userData());
  if (!pData)
    return;
  const InfoTextBlockBrackets::Symbolics &sym = pData->getSymbolics();

  int iCurPos = cursor.position();
  int iRelPos = iCurPos - curBlock.position();
  for (int i = 0; i < sym.size(); ++i)
  {
    if (/*iRelPos > 0 &&*/ sym[i].position == iRelPos - 1) // from the preceding symbol
    {
      if (sym[i].character == SymbolPosition::LeftParenthesis)
      {
        if (this->matchToRight(curBlock, SymbolPosition::RightParenthesis, i + 1, 0, sym[i].character))
          this->selectSymbolAt(iCurPos - 1);
      }
      if (sym[i].character == SymbolPosition::RightParenthesis)
      {
        if (this->matchToLeft(curBlock, SymbolPosition::LeftParenthesis, i - 1, 0, sym[i].character))
          this->selectSymbolAt(iCurPos - 1);
      }
    }
    //
    if (/*iRelPos >= 0 &&*/ sym[i].position == iRelPos) // from the next symbol
    {
      if (sym[i].character == SymbolPosition::LeftParenthesis)
      {
        if (this->matchToRight(curBlock, SymbolPosition::RightParenthesis, i + 1, 0, sym[i].character))
          this->selectSymbolAt(iCurPos);
      }
      if (sym[i].character == SymbolPosition::RightParenthesis)
      {
        if (this->matchToLeft(curBlock, SymbolPosition::LeftParenthesis, i - 1, 0, sym[i].character))
          this->selectSymbolAt(iCurPos);
      }
    }
  }
}

void EditCode::selectSymbolAt(int pos)
{
  QTextCursor cursor(this->textCursor());
  ListExtraSelection list_sel = this->extraSelections();
  ExtraSel sel;

  sel.format.setBackground(QColor(Qt::green).lighter(144));
  cursor.setPosition(pos);
  cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
  sel.cursor = cursor;

  list_sel.append(sel);
  this->setExtraSelections(list_sel);
}
