#ifndef __View_Substructures_h__
#define __View_Substructures_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QTreeView>
#include <QTreeWidget>
#include <QTreeWidgetItem>

class ViewSubstructures
    : public QTreeWidget
{
    Q_OBJECT
public:
    ViewSubstructures(QWidget * /*parent*/);
    ~ViewSubstructures() override = default;

private:
    QTreeWidgetItem *itemAtoms_ = nullptr;
    QTreeWidgetItem *itemBonds_ = nullptr;
    QTreeWidgetItem *itemAngles_ = nullptr;
    QTreeWidgetItem *itemTorsions_ = nullptr;
    QTreeWidgetItem *itemFragments_ = nullptr;
    QTreeWidgetItem *itemPaths_ = nullptr;
    QTreeWidgetItem *itemStars_ = nullptr;
    QTreeWidgetItem *itemCycles_ = nullptr;
};

#endif // !__View_Substructures_h__