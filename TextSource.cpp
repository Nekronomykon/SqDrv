#include "TextSource.h"

#include <QTextStream>

#include <vtkStdString.h>

#include "Elements.h"
using namespace vtk;

#include <sstream>
using namespace std;

TextSource::TextSource(QWidget *parent)
    : QWidget(parent)
{
    this->setupUi(this);
    this->setFont(editAtoms_->font());

    // connect(lineEditTitle_, &QLineEdit::textEdited, this, &TextSource::titleChanged);
    // connect(lineEditTitle_, &QLineEdit::returnPressed, this, &TextSource::updateSourceTitle);
}

EditSource *TextSource::editAtoms() const
{
    return editAtoms_;
}

QTextDocument *TextSource::sourceAtomsDocument() const
{
    return editAtoms_->document();
}

void TextSource::setModified(bool b)
{
    this->sourceAtomsDocument()->setModified(b);
    lineEditTitle_->setModified(b);
}

bool TextSource::isModified() const
{
    return this->sourceAtomsDocument()->isModified() ? true
                                                     : lineEditTitle_->isModified();
}

// void TextSource::titleChanged(const QString &titlenew)
void TextSource::on_lineEditTitle__textEdited(const QString &titlenew)
{
    editAtoms_->setDocumentTitle(QString(titlenew).trimmed());
}
// void TextSource::updateSourceTitle(void)
void TextSource::on_lineEditTitle__returnPressed(void)
{
    lineEditTitle_->setText(editAtoms_->documentTitle().trimmed());
}
bool TextSource::buildSource(QString &content)
{
    QTextStream tsx;
    QString res = lineAtoms_->text();
    res += "\n";
    // res += lineEditTitle_->text().trimmed();
    res += editAtoms_->documentTitle();
    res += "\n";
    res += editAtoms_->document()->toPlainText();
    content = res.trimmed();
    bool bRes = !content.isEmpty();
    content += "\n";
    return bRes;
}
//
///////////////////////////////////////////////////////////////////////
/// \brief TextSource::getAtomString
///
//
///////////////////////////////////////////////////////////////////////
/// \brief TextSource::getPositionString
///
//
///////////////////////////////////////////////////////////////////////
/// \brief TextSource::showMolecule
///
void TextSource::showMolecule(vtkMolecule *pMol, const QString &title)
{
    lineEditTitle_->setText(title);
    int nAtoms = pMol->GetNumberOfAtoms();

    QString number;
    number.setNum(nAtoms);
    lineAtoms_->setText(number);

    QString atoms;

    QString bonds;
    bonds.setNum(pMol->GetNumberOfBonds());
    lineBonds_->setText(bonds);
    // ...for instance

    QMap<long, long> mapFormula;
    QVector<long> idTypeNum(nAtoms, 0);

    double vENN;

    QTextStream out(&atoms, QIODevice::WriteOnly);

    QPointer<QTextDocument> doc(new QTextDocument);

    for (int i = 0; i < nAtoms; ++i)
    {
        ostringstream out_atom;
        const vtkAtom ai = pMol->GetAtom(i);

        long idElem = ai.GetAtomicNumber();
        idTypeNum[i] = ++mapFormula[idElem];

        vtkStdString sElem = Elements::GetElementSymbol(idElem);

        out
            << qSetFieldWidth(4)
            // << '[' << i+1 << ']'
            << sElem.c_str()
            // << idTypeNum[i]
            << ' ';

        out
            << qSetFieldWidth(12) << qSetRealNumberPrecision(8)
            << ai.GetPosition().GetX() // << ' '
            << ai.GetPosition().GetY() // << ' '
            << ai.GetPosition().GetZ()
            << Qt::endl /// ???
            ;
    }
    out.flush();
    editAtoms_->setPlainText(atoms);

    // molecular properties 'as a whole chunk' (no bonds yet)
    QString formula;
    double vMass(0.0);
    QString mass;

    QTextStream out_frm(&formula, QIODevice::WriteOnly);
    for (auto map_key = mapFormula.begin();
         map_key != mapFormula.end();
         ++map_key)
    {
        if (!map_key.value())
            continue;

        long idElem = map_key.key();

        out_frm << Elements::GetElementSymbol(idElem).c_str();

        if (map_key.value() > 1)
            out_frm << '_'
                    << map_key.value();
        out_frm << ' ';

        vMass += Elements::GetMeanMass(idElem) * map_key.key();
    }
    out_frm.flush();
    lineFormula_->setText(formula.trimmed());

    mass.setNum(vMass);
    lineMass_->setText(mass);
}
//
///////////////////////////////////////////////////////////////////////
