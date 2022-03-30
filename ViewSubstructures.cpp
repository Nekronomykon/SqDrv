#include "ViewSubstructures.h"

ViewSubstructures::ViewSubstructures(QWidget* parent)
: QTreeWidget(parent)
{
    // ???
    this->setColumnCount(6);
    this->headerItem()->setText(0, tr("Id"));
    this->headerItem()->setText(1, tr("Content"));
    this->headerItem()->setText(2, tr("X"));
    this->headerItem()->setText(3, tr("Y"));
    this->headerItem()->setText(4, tr("Z"));
    this->headerItem()->setText(5, tr("Value"));
    this->setAllColumnsShowFocus(true);
    //*========================================================================*
    // Categories:
    itemAtoms_ = new QTreeWidgetItem(this, nullptr);
    itemAtoms_->setText(0, tr("Atoms"));
    // ------------------------------------------------------------------------
    itemBonds_ = new QTreeWidgetItem(this, itemAtoms_);
    itemBonds_->setText(0, tr("Bonds"));
    // ------------------------------------------------------------------------
    itemAngles_ = new QTreeWidgetItem(this, itemBonds_);
    itemAngles_->setText(0, tr("Valency angles"));
    // ------------------------------------------------------------------------
    itemTorsions_ = new QTreeWidgetItem(this, itemAngles_);
    itemTorsions_->setText(0, tr("Dihedral angles"));
    // ------------------------------------------------------------------------
    itemPaths_ = new QTreeWidgetItem(this, itemTorsions_);
    itemPaths_->setText(0, tr("Paths"));
    // ------------------------------------------------------------------------
    itemCycles_ = new QTreeWidgetItem(this, itemPaths_);
    itemCycles_->setText(0, tr("Cycles"));
    // ------------------------------------------------------------------------
    itemStars_ = new QTreeWidgetItem(this, itemCycles_);
    itemStars_->setText(0, tr("Stars"));
    // ------------------------------------------------------------------------
    itemFragments_ = new QTreeWidgetItem(this, itemStars_);
    itemFragments_->setText(0, tr("Fragments"));
    //*========================================================================*
}