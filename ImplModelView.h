#ifndef Impl_ModelView_h
#define Impl_ModelView_h

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QWidget>

#include <QTreeView>
#include <QListView>
#include <QTableView>

template <class T, class TBase, class TModel>
class ImplModelView
    : public TBase
{
  // Q_OBJECT
public:
  typedef TBase Base;
  typedef TModel Model;
  explicit ImplModelView(QWidget *parent = nullptr)
      : TBase(parent), ptrModel_(new Model)
  {
    assert(ptrModel_);
    T*pT = static_cast<T*>(this);
    pT->SetupModel(pT->GetViewModel());
    // this->setModel(ptrModel_);
  }
  Model* GetViewModel() const {return ptrModel_;}

  void SetupModel(Model*pM)
  {
    T*pT = static_cast<T*>(this);
    pT->setModel(pM);
  }

private:
  Model *ptrModel_;
};

#endif // !Impl_ModelView_h
