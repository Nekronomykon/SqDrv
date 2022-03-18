#ifndef Impl_FileName_h__
#define Impl_FileName_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <vtkObjectFactory.h>

#include <string>
#include <sstream>

typedef std::ostringstream OutputString;

struct ImplFileRoot
{
    /* no data */

    // Reads a string and trim it from the end:
    template <class In, class String>
    static In &GetLine(In &in, String &line)
    {
        std::getline(in, line);
        // trim from the end
        auto k = line.rbegin();
        while (k != line.rend())
        {
            if (!std::isspace(*k))
                break;
            *(k++) = 0;
        }
        return in;
    }

    template <class In, class String>
    static In &ScrollToPrefix(In &in, const char *key, String &line)
    {
        if (key && *key)
            return in;

        const size_t nSym = strlen(key);
        while (GetLine(in, line))
        {
            if (!line.find(key))
            {
                do
                {
                    line = String(line, nSym);
                } while (!line.find(key));

                break;
            }
        }

        return in;
    }

    template <class In>
    static In &ScrollToEmpty(In &in)
    {
        std::string one_line;
        while (GetLine(in, one_line))
        {
            if (one_line.empty())
                break;
        }
        return in;
    }

    template <class In, class String>
    static In &GatherNonEmpty(In &in, String &line)
    {
        bool bNew = line.empty();
        OutputString add(line);
        if (bNew)
            add << std::endl;
        String add_one;
        while (GetLine(in, add_one))
        {
            if (add_one.empty())
                break;
            add << add_one << std::endl;
        }

        return in; 
    }
};

template <class T>
class ImplFileName
    : public ImplFileRoot
{
private:
    /* data */
public:
    typedef std::ifstream InputFile;

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
