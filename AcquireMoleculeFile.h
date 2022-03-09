#ifndef Acquire_Molecule_File_h__
#define Acquire_Molecule_File_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "MoleculeBuild.h"
using namespace vtk;

#include "ImplFileName.h"

#include <vtkObjectFactory.h>
#include <vtkNew.h>
#include <vtkSmartPointer.h>

#include <vtkStringArray.h>

#include <vtksys/FStream.hxx>

class vtkInformation;
class vtkInformationVector;

namespace vtk
{
    typedef vtksys::ifstream InputFile;
    typedef std::istringstream InputString;

    template <class In, class String>
    In &GetLine(In &in, String &line)
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

    class /* VTKIOCHEMISTRY_EXPORT*/ AcquireMoleculeFile
        : public MoleculeBuild,
          public ImplFileName<AcquireMoleculeFile>
    {
    public:
        static AcquireMoleculeFile *New();
        vtkTypeMacro(AcquireMoleculeFile, MoleculeBuild);
        void PrintSelf(ostream &os, vtkIndent indent) override;

        // ----------------------------------------------------------------------
        //
        static int ScrollStrings(std::istream & /* in */, int /* ns */ = 1);
        //
        // ----------------------------------------------------------------------

        ///@{
        /**
         *    * Get/Set NumberOfAtoms read from the file:
         **/
        // vtkGetMacro(NumberOfAtoms, vtkIdType);
        vtkIdType GetNumberOfAtoms() const { return NumberOfAtoms_; }
        // vtkSetMacro(NumberOfAtoms, vtkIdType);
        void SetNumberOfAtoms(vtkIdType arg)
        {
            if (NumberOfAtoms_ != arg)
            {
                NumberOfAtoms_ = arg;
                this->Modified();
            }
        }

        ///@}

        Molecule *GetOutput();
        void SetOutput(Molecule *) override;

        ///@{
        /**
         *    * Get/Set the structure name / title of the Molecule File:
         **/
        void SetStructureName(const char *arg)
        {
            vtkDebugMacro(<< this->GetClassName() << ": setting nameStructure to " << ((arg && *arg) ? arg : "(null)"));
            if (arg && *arg)
                this->nameStructure_.assign(arg);
            else
                this->nameStructure_.clear();
            this->Modified();
        }
        const char *GetStructureName(void) const
        {
            return this->nameStructure_.c_str();
        }
        bool HasStructureName(void) const { return !nameStructure_.empty(); }
        ///@}
    protected:
        explicit AcquireMoleculeFile(int nInPorts = 1, int nOutPorts = 1);
        ~AcquireMoleculeFile() override = default;

        vtkIdType &NumberOfAtoms(void) { return NumberOfAtoms_; }
        std::string &NameOfStructure(void) { return nameStructure_; }

        int RequestInformation(vtkInformation *, vtkInformationVector **, vtkInformationVector *) override;
        int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *) override;

        // To be overriden to read information stored in the (file) stream
        virtual int CheckSizesOf(InputFile& /*inp*/);
        virtual int ReadDataFrom(InputFile& /*inp*/);

    private:
        vtkIdType NumberOfAtoms_ = 0;
        std::string nameStructure_ = "";
        vtkNew<vtkStringArray> nameAtoms_;

    private:
        AcquireMoleculeFile(const AcquireMoleculeFile &) = delete;
        void operator=(const AcquireMoleculeFile &) = delete;
    };
}; // namespace vtk

#endif // !Acquire_Molecule_File_h__
