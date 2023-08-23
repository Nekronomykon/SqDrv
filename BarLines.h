#ifndef Bar_Lines_h__
#define Bar_Lines_h__

#include <QSize>
#include <QPaintEvent>
#include <QWidget>

class EditMarkLines;

class BarLines : public QWidget
{
    Q_OBJECT
public:
    explicit BarLines(EditMarkLines *parent);
public:
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    EditMarkLines* ptrHost_;
};

#endif // Bar_Lines_h__
