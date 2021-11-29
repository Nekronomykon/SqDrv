#ifndef Info_TextBlock_Brackets_H__
#define Info_TextBlock_Brackets_H__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QTextBlockUserData>
#include <QString>

struct SymbolPosition
{
  char character;
  int position;

  enum
  {
    LeftParenthesis = '(',
    RightParenthesis = ')',
    LeftCorner = '<',
    RightCorner = '>',
    LeftBracket = '{',
    RightBracket = '}',
    LeftRectBracket = '[',
    RightRectBracket = ']'
  };
};

class InfoTextBlockBrackets
    : public QTextBlockUserData
{
public:
  typedef QVector<SymbolPosition> Symbolics;

  explicit InfoTextBlockBrackets();

  int AddSymbolPosInfo(char c, const QString &str);

  int InsertPosInfo(const SymbolPosition &pos);

  const Symbolics &getSymbolics() const { return symbolics_; }

private:
  Symbolics symbolics_;
};

#endif // !Info_TextBlock_Brackets_H__
