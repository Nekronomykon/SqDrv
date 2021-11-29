#ifndef HIGHLIGHTBRACKETS_H
#define HIGHLIGHTBRACKETS_H

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QSyntaxHighlighter>
#include <QTextDocument>

class HighlightBrackets
: public QSyntaxHighlighter
{
  Q_OBJECT
public:
  explicit HighlightBrackets(QTextDocument *pDoc = nullptr);

protected:
  void highlightBlock(const QString &text) override;
};

#endif // HIGHLIGHTBRACKETS_H
