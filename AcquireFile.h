#ifndef Acquire_File_h__
#define Acquire_File_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <vtkObjectFactory.h>
#include <vtkNew.h>
#include <vtkSmartPointer.h>

#include <vtkMoleculeAlgorithm.h>
#include <vtkStringArray.h>

#include <vtksys/FStream.hxx>

namespace vtk
{
    typedef vtkMoleculeAlgorithm AcquireFileRoot;

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

    class /* VTKIOCHEMISTRY_EXPORT*/ AcquireFile
        : public AcquireFileRoot
    {
    public:
        static AcquireFile *New();
        vtkTypeMacro(AcquireFile, AcquireFileRoot);
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

        vtkMolecule *GetOutput();
        void SetOutput(vtkMolecule *) override;

        ///@{
        /**
         *    * Get/Set the path to the Molecule file
         **/
        // vtkSetStdStringFromCharMacro(nameFile);
        void SetFileName(const char *arg)
        {
            vtkDebugMacro(<< this->GetClassName() << ": setting nameFile to" << ((arg && *arg) ? arg : "(null)"));
            if (arg && *arg)
                this->nameFile_.assign(arg);
            else
                this->nameFile_.clear();
            this->Modified();
        }
        const char *GetFileName() const
        {
            return this->nameFile_.c_str();
        }
        bool HasFileName() const { return !nameFile_.empty(); }
        ///@}

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
        const char *GetStructureName(void)const
        {
            return this->nameStructure_.c_str();
        }
        bool HasStructureName(void) const { return !nameStructure_.empty(); }
        ///@}
    protected:
        explicit AcquireFile(int /* nInPorts */ = 0);
        ~AcquireFile() override = default;

        vtkIdType& NumberOfAtoms(void) {return NumberOfAtoms_;}
        std::string& NameOfStructure(void) {return nameStructure_;}

    private:
        std::string nameFile_ = "";
        vtkIdType NumberOfAtoms_ = 0;
        std::string nameStructure_ = "";
        vtkNew<vtkStringArray> nameAtoms_;
    private:
        AcquireFile(const AcquireFile &) = delete;
        void operator=(const AcquireFile &) = delete;
    };
}; // namespace vtk

#endif // !Acquire_File_h__
