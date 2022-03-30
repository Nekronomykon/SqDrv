#ifndef NamedOperation_h__
#define NamedOperation_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QString>

template <class Host, class Arg>
struct NamedOperationT
{
    typedef bool (Host::*Operation)(const Arg &);
    QString nameContext_;
    QString nameOperation_;
    Operation operation_;
    // Applications:
    bool ApplyForHost(Host &host, const Arg &arg) const
    {
        return (!operation_) ? false : (host.*operation_)(arg);
    }
};

template <class Host, class Arg>
struct NamedFileFormatT
{
    typedef QString NameFormat;
    typedef bool (Host::*Operation)(const Arg &);
    NameFormat name_;
    Operation reader_;
    Operation writer_;
    // whatever operations??
    // Attributes:
    const NameFormat &getName() const { return name_; }
    bool isUnnamed() const { return name_.isEmpty(); }
    bool HasRead() const { return bool(reader_); }
    bool HasSave() const { return bool(writer_); }
    // Applications:
    static bool ApplyOperationOn(const Operation op, Host &host, Arg arg = Arg())
    {
        return (!op) ? false
                     : (host.*op)(arg);
    }
    bool ReadHostContent(Host &host, const Arg &arg) const
    {
        return ApplyOperationOn(reader_, host, arg);
    }
    bool SaveHostContent(Host &host, const Arg &arg) const
    {
        return ApplyOperationOn(writer_, host, arg);
    }
};

#endif // !NamedOperation_h__
