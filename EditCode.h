#ifndef Edit_Code__H_
#define Edit_Code__H_

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QPointer>

#include <QResizeEvent>
#include <QPaintEvent>

#include <QIODevice>

#include <QTextEdit>
#include <QPlainTextEdit>

class SideBarDecorator;

#include "HighlightBrackets.h"
#include "InfoTextBlockBrackets.h"

//![codeeditordefinition]

class EditCode : public QPlainTextEdit
{
  Q_OBJECT

public:
  explicit EditCode(QWidget * /*parent*/ = nullptr);
  ~EditCode() override = default;

  void SideBarDecoratorPaintEvent(QPaintEvent *);
  int SideBarWidth();

  void load(QIODevice * /*pIO*/);
  void save(QIODevice * /*pIO*/);

  bool loadFromPath(const QString & /*pathname*/);
  bool saveToPath(const QString & /*pathname*/);

protected:
  typedef QTextEdit::ExtraSelection ExtraSel;
  typedef QList<ExtraSel> ListExtraSelection;

  void resizeEvent(QResizeEvent *event) override;

  void  highlightMatchBrackets();
  void  highlightCurrentLine();

  void  selectSymbolAt( int /* pos */ );

  bool matchToRight(QTextBlock, char cWhat, int iFrom, int iDepth, char cFrom);
  bool matchToLeft(QTextBlock, char cWhat, int iFrom, int iDepth, char cFrom);

private slots:
  void updateSideBarWidth(int /*newBlockCount*/);
  void updateCursorEnvironment();
  void updateSideBar(const QRect &, int);

private:
  QFont fontSrc_;
  QPointer<SideBarDecorator> side_bar_;
  QPointer<HighlightBrackets> highlight_;
};

#endif // !Edit_Code__H_
