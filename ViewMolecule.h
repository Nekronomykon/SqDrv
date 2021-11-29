#ifndef __View_Molecule_h__
#define __View_Molecule_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "ViewStructure.h"
#include "ViewSubstructures.h"

#include "StyleMapMolecule.h"
#include "Molecule.h"

#include <QVTKOpenGLNativeWidget.h>
#include <QPointer>
#include <QWidget>
#include <QSplitter>

class ViewMolecule
    : public QSplitter
{
    Q_OBJECT
public:
    explicit ViewMolecule(QWidget * /* parent */ = nullptr);
    ~ViewMolecule() override = default;

    ViewStructure *viewStructure(void) const;

    void resetMolecule(Molecule *pMol = nullptr);
    void setMoleculeVisualStyle(const StyleMapMolecule & /* style */);

private:
    /* data */
    QPointer<ViewStructure> view3D_;
    QPointer<ViewSubstructures> viewSub_;
    //
    QTreeWidgetItem* itemAtoms_ = nullptr;
    QTreeWidgetItem* itemBonds_ = nullptr;
    QTreeWidgetItem* itemAngles_ = nullptr;
    QTreeWidgetItem* itemTorsions_ = nullptr;
    QTreeWidgetItem* itemFragments_ = nullptr;
    QTreeWidgetItem* itemPaths_ = nullptr;
    QTreeWidgetItem* itemStars_ = nullptr;
    QTreeWidgetItem* itemCycles_ = nullptr;
};

#endif // !__View_Molecule_h__
