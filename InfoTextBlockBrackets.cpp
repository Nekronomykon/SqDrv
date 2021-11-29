#include "InfoTextBlockBrackets.h"

InfoTextBlockBrackets::InfoTextBlockBrackets()
    : QTextBlockUserData() {}

int InfoTextBlockBrackets::InsertPosInfo(const SymbolPosition &info)
{
  int i = 0;

  while (i < symbolics_.size() &&
         info.position > symbolics_[i].position)
    ++i;

  symbolics_.insert(i, info);
  return i;
}

int InfoTextBlockBrackets::AddSymbolPosInfo(char c, const QString &text)
{
  int nRes = 0;
  // left bracket
  SymbolPosition info = {c, -1};
  int nPos = text.indexOf(c);
  while (nPos != -1)
  {
    info.position = nPos;
    this->InsertPosInfo(info);
    nPos = text.indexOf(c, ++nPos);
  }
  return nRes;
}
