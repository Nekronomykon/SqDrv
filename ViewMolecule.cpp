#include "ViewMolecule.h"

#include <QHeaderView>

ViewMolecule::ViewMolecule(QWidget *parent)
    : QSplitter(Qt::Horizontal, parent), view3D_(new ViewStructure), viewSub_(new ViewSubstructures)
{
    this->setOpaqueResize(false);
    //
    viewSub_->setColumnCount(2);
    viewSub_->headerItem()->setText(0, tr("Geometry"));
    viewSub_->headerItem()->setText(1, tr("Value"));
    viewSub_->setAllColumnsShowFocus(true);
    //*========================================================================*
    // Categories:
    itemAtoms_ = new QTreeWidgetItem(viewSub_, nullptr);
    itemAtoms_->setText(0, tr("Atoms"));
    // ------------------------------------------------------------------------
    itemBonds_ = new QTreeWidgetItem(viewSub_, itemAtoms_);
    itemBonds_->setText(0, tr("Bonds"));
    // ------------------------------------------------------------------------
    itemAngles_ = new QTreeWidgetItem(viewSub_, itemBonds_);
    itemAngles_->setText(0, tr("Valency angles"));
    // ------------------------------------------------------------------------
    itemTorsions_ = new QTreeWidgetItem(viewSub_, itemAngles_);
    itemTorsions_->setText(0, tr("Dihedral angles"));
    // ------------------------------------------------------------------------
    itemPaths_ = new QTreeWidgetItem(viewSub_, itemTorsions_);
    itemPaths_->setText(0, tr("Paths"));
    // ------------------------------------------------------------------------
    itemCycles_ = new QTreeWidgetItem(viewSub_, itemPaths_);
    itemCycles_->setText(0, tr("Cycles"));
    // ------------------------------------------------------------------------
    itemStars_ = new QTreeWidgetItem(viewSub_, itemCycles_);
    itemStars_->setText(0, tr("Stars"));
    // ------------------------------------------------------------------------
    itemFragments_ = new QTreeWidgetItem(viewSub_, itemStars_);
    itemFragments_->setText(0, tr("Fragments"));
    //*========================================================================*
    this->addWidget(view3D_);
    this->addWidget(viewSub_);
}

ViewStructure *ViewMolecule::viewStructure(void) const
{
    return view3D_;
}

void ViewMolecule::resetMolecule(Molecule *pMol)
{
    if (view3D_->initRendering(pMol))
        view3D_->repaint();
    /* whatever */;
}

void ViewMolecule::setMoleculeVisualStyle(const StyleMapMolecule &style)
{
    if (view3D_->resetMoleculeStyle(style))
        view3D_->repaint();
}