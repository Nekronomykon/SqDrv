#ifndef Impl_FileName_h__
#define Impl_FileName_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <vtkObjectFactory.h>

#include <string>
using String = std::string;

#include <utility>
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;
using Path = fs::path;
// using Path = String;

#include <sstream>
#include <cassert>

typedef std::ifstream InputFile;
typedef std::ofstream OutputFile;
//
typedef std::istringstream InputString;
typedef std::ostringstream OutputString;

struct ImplPathRoot
{
    /* no data */

    //------------------------------------------------------------------------------
    template <class _In>
    static int ScrollStrings(_In &in, int ns)
    {
        if (ns <= 0)
            return 0;
        std::string one;
        do
        {
            std::getline(in, one);
        } while (--ns);
        return ns;
    }
    //------------------------------------------------------------------------------
    static std::string GetMarkupContent(std::istream &inp, const char *tag)
    {
        std::string keyOpen("<");
        std::string keyClose("</");
        std::string res;
        std::string one;
        bool bAdd(false);

        assert(tag && *tag);
        keyOpen.append(tag) += '>';
        keyClose.append(tag) += '>';

        if (!std::getline(inp, one))
            return res;
        do
        {
            // one = std::trim(one)
            if (one.empty())
                continue;
            if (bAdd)
            {
                if (one.find(keyClose) != std::string::npos)
                {
                    bAdd = false;
                    break;
                }
                else
                {
                    res += ' ';
                    res.append(one);
                }
            }
            else
            {
                if (one.find(keyOpen) != std::string::npos)
                    bAdd = true;
            }
            /* code */
        } while (std::getline(inp, one));

        return res;
    }
    //------------------------------------------------------------------------------
    // Reads a string and trim it from the end:
    template <class _In, class _String>
    static _In &GetLine(_In &in, _String &line)
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

    template <class _In, class _String>
    static _In &ScrollToPrefix(_In &in, const char *key, _String &line)
    {
        if (!key || !*key)
            return in; //

        const size_t nSym = strlen(key);
        while (GetLine(in, line))
        {
            if (!line.find(key))
            {
                do
                {
                    line = _String(line, nSym);
                } while (!line.find(key));

                break;
            }
        }

        return in;
    }

    template <class In, class Value>
    static In &ReadAfterPrefix(In &in, const char *key, Value &val)
    {
        if (key && *key)
            return in;
        String src;
        if (ScrollToPrefix(in, key, src))
        {
            InputString input(src);
            input >> val;
        }

        return in;
    }

    template <class _In>
    static _In &ScrollToEmpty(_In &in)
    {
        String one_line;
        while (GetLine(in, one_line))
        {
            if (one_line.empty())
                break;
        }
        return in;
    }

    template <class _In, class _String>
    static _In &GatherNonEmpty(_In &in, _String &line)
    {
        bool bNew = line.empty();
        OutputString add(line);
        if (bNew)
            add << std::endl;
        _String add_one;
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
class ImplPathName
    : public ImplPathRoot
{
private:
    /* data */
public:
    explicit ImplPathName() {}

    ///////////////////////////////////////////////////////////////////////////////
    //
    Path resetPath(Path a_path = Path())
    {
        std::swap(this->thePath_, a_path);
        return a_path;
    }
    Path getPath() const { return this->thePath_; }
    bool hasPath() const { return !this->thePath_.empty(); }
    //
    ///////////////////////////////////////////////////////////////////////////////

protected:
    Path &PathName() { return this->thePath_; }

    Path thePath_ = Path();
};

#endif // !Impl_FileName_h__
