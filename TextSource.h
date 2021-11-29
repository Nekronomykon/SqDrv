#ifndef __Text_Source_h__
#define __Text_Source_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>

#include <QTextDocument>

#include "EditSource.h"

#include <QPointer>

#include <vtkMolecule.h>
#include <vtkIdTypeArray.h>

#include <vtkNew.h>
#include <vtkSmartPointer.h>

typedef vtkNew<vtkIdTypeArray> NewTypeArray;
typedef vtkSmartPointer<vtkIdTypeArray> ATypeArray;

#include "ui_TextSource.h"

class TextSource
    : public QWidget,
      private Ui_TextSource

{
    Q_OBJECT
public:
    explicit TextSource(QWidget * /*parent*/ = nullptr);
    ~TextSource() override = default;

    QTextDocument *sourceAtomsDocument() const;
    EditSource* editAtoms() const;

    void setModified(bool b);
    bool isModified() const;

    void showMolecule(vtkMolecule * /* pMol */, const QString & /* title */);
    bool buildSource(QString & /*content*/);

protected:
private:
private slots:
    // void titleChanged(const QString & /* titlenew */);
    void on_lineEditTitle__textEdited(const QString &/* titlenew */);
    // void updateSourceTitle(void);
    void on_lineEditTitle__returnPressed(void);
};

#endif // !__Text_Source_h__
