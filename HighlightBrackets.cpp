#include "HighlightBrackets.h"

#include "InfoTextBlockBrackets.h"

HighlightBrackets::HighlightBrackets(QTextDocument *pDoc)
    : QSyntaxHighlighter(pDoc) {}

void HighlightBrackets::highlightBlock(const QString &text)
{
  InfoTextBlockBrackets *pData = new InfoTextBlockBrackets;

  // left brackets
  pData->AddSymbolPosInfo(SymbolPosition::LeftParenthesis,text);
  pData->AddSymbolPosInfo(SymbolPosition::LeftRectBracket,text);
  // pData->AddSymbolPosInfo('{',text);
  // right brackets
  pData->AddSymbolPosInfo(SymbolPosition::RightParenthesis,text);
  pData->AddSymbolPosInfo(SymbolPosition::RightRectBracket,text);
  // pData->AddSymbolPosInfo('}',text);

  // [*]
  this->setCurrentBlockUserData(pData);
}
