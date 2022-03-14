#ifndef Impl_FileName_h__
#define Impl_FileName_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <vtkObjectFactory.h>

#include <string>

template <class T>
class ImplFileName
{
private:
    /* data */
public:
    typedef std::ifstream InputFileStream;

    explicit ImplFileName() {}

    ///////////////////////////////////////////////////////////////////////////////
    //
    void SetFileName(const char *arg)
    {
        // CRTP stuff:
        T *pThis = static_cast<T *>(this);
        vtkDebugMacro(<< pThis->GetClassName() << ": setting nameFile to" << ((arg && *arg) ? arg : "(null)"));
        if (arg && *arg)
            this->nameFile_.assign(arg);
        else
            this->nameFile_.clear();

        pThis->Modified();
    }
    const char *GetFileName() const { return this->nameFile_.c_str(); }
    bool HasFileName() const { return !this->nameFile_.empty(); }
    //
    ///////////////////////////////////////////////////////////////////////////////

protected:
    std::string &FileName() { return this->nameFile_; }

    std::string nameFile_ = "";
};

#endif // !Impl_FileName_h__
