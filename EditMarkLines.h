#ifndef Edit_Mark_Lines_h__
#define Edit_Mark_Lines_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QPlainTextEdit>
#include <QWidget>

#include "BarLines.h"

#include <QPointer>

class EditMarkLines : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit EditMarkLines(QWidget *parent = nullptr);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

    bool isModified() { return this->document()->isModified(); }

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);

private:
    QPointer<BarLines> barLines_;
};

#endif // Edit_Mark_Lines_h__
