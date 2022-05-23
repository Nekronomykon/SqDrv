#include "MapMolecule.h"
using namespace vtk;

/*=========================================================================

  Program:   Visualization Toolkit
  Namespace: vtk
  Module:    MapMolecule.cxx

  Copyright (c) ScrewDriver te Blackheadborough
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include <vtkActor.h>
#include <vtkColor.h>
#include <vtkCylinderSource.h>
#include <vtkDataArray.h>
#include <vtkDoubleArray.h>
#include <vtkEventForwarderCommand.h>
#include <vtkExecutive.h>
#include <vtkFloatArray.h>
#include <vtkGlyph3DMapper.h>
#include <vtkIdTypeArray.h>
#include <vtkInformation.h>
#include <vtkLookupTable.h>
#include <vtkMath.h>
#include <vtkMolecule.h>
#include <vtkNew.h>
#include <vtkObjectFactory.h>
#include <vtkPeriodicTable.h>
#include <vtkPointData.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkSelection.h>
#include <vtkSelectionNode.h>
#include <vtkSphereSource.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkTrivialProducer.h>
#include <vtkUnsignedCharArray.h>
#include <vtkUnsignedShortArray.h>
#include <vtkVector.h>
#include <vtkVectorOperators.h>

// Note this class have an accelerated subclass MapMoleculeOpenGL.
// If you change this class please also check that class for impacts.

/*
** Tasks (SDtB):
** 1. Remove its lattice part;
** 2. Adopt to use vtk::Elements instead of vtkPeriodicTable;
** 3. Take care of the OpenGL descendent class mentioned above;
** 4. Prepare for the realization and visualization
**    of 3D control spots for the bond in the structure (--> QTAIM View)
*/
vtkObjectFactoryNewMacro(MapMolecule);

//------------------------------------------------------------------------------
MapMolecule::MapMolecule() : AtomicRadiusType(VDWRadius) //
                             ,
                             AtomicRadiusScaleFactor(0.3) //
                             ,
                             AtomicRadiusArrayName(nullptr) //
                             ,
                             AtomColorMode(DiscreteByAtom) //
                             // ,
                             // RenderBonds(true) //
                             ,
                             BondColorMode(DiscreteByAtom) //
                             // ,
                             // UseMultiCylindersForBonds(true) //
                             ,
                             BondRadius(0.025) //
                             ,
                             style_(new StyleMapMolecule)
{
    style_->Reset(StyleMapMolecule::styleFast);
    // Initialize ivars:
    this->AtomColor[0] = this->AtomColor[1] = this->AtomColor[2] = 100;
    this->BondColor[0] = 200;
    this->BondColor[1] = 200;
    this->BondColor[2] = 0;
    // this->LatticeColor[0] = this->LatticeColor[1] = this->LatticeColor[2] = 255;
    this->SetAtomicRadiusArrayName("radii");

    // Setup glyph sources
    vtkNew<vtkSphereSource> sphere;
    sphere->SetRadius(1.0);
    sphere->SetPhiResolution(50);
    sphere->SetThetaResolution(50);
    sphere->Update();
    this->AtomGlyphMapper->SetSourceConnection(sphere->GetOutputPort());

    vtkNew<vtkCylinderSource> cylinder;
    cylinder->SetRadius(1.0);
    cylinder->SetResolution(20);
    cylinder->SetHeight(1.0);
    cylinder->CappingOff();
    cylinder->Update();
    // Rotate the glyph so that the cylinder is aligned with the x-axis,
    // rather than the y-axis. This makes glyph orientation much easier.
    vtkNew<vtkTransform> cylXform;
    cylXform->RotateWXYZ(90, 0.0, 0.0, 1.0);
    vtkNew<vtkTransformPolyDataFilter> cylXformFilter;
    cylXformFilter->SetInputConnection(cylinder->GetOutputPort());
    cylXformFilter->SetTransform(cylXform);
    cylXformFilter->Update();
    this->BondGlyphMapper->SetSourceConnection(cylXformFilter->GetOutputPort());

    // Configure default LookupTable
    vtkNew<vtkLookupTable> lut;
    this->PeriodicTable->GetDefaultLUT(lut);
    this->SetLookupTable(lut);

    // Setup glyph mappers
    this->AtomGlyphMapper->SetScalarRange(0, this->PeriodicTable->GetNumberOfElements());
    this->AtomGlyphMapper->SetColorModeToMapScalars();
    this->AtomGlyphMapper->SetScalarModeToUsePointFieldData();
    this->AtomGlyphMapper->SetScaleModeToScaleByMagnitude();
    this->BondGlyphMapper->SetScaleModeToScaleByVectorComponents();
    // Bond color mode is setup during updates

    // Forward commands to instance mappers
    vtkNew<vtkEventForwarderCommand> cb;
    cb->SetTarget(this);

    this->AtomGlyphMapper->AddObserver(vtkCommand::StartEvent, cb);
    this->AtomGlyphMapper->AddObserver(vtkCommand::EndEvent, cb);
    this->AtomGlyphMapper->AddObserver(vtkCommand::ProgressEvent, cb);

    this->BondGlyphMapper->AddObserver(vtkCommand::StartEvent, cb);
    this->BondGlyphMapper->AddObserver(vtkCommand::EndEvent, cb);
    this->BondGlyphMapper->AddObserver(vtkCommand::ProgressEvent, cb);

    // Connect the trivial producers to forward the glyph polydata
    this->AtomGlyphPointOutput->SetOutput(this->AtomGlyphPolyData);
    this->AtomGlyphMapper->SetInputConnection(this->AtomGlyphPointOutput->GetOutputPort());

    this->BondGlyphPointOutput->SetOutput(this->BondGlyphPolyData);
    this->BondGlyphMapper->SetInputConnection(this->BondGlyphPointOutput->GetOutputPort());

    // Force the glyph data to be generated on the next render:
    this->GlyphDataInitialized = false;

    this->SetInputArrayToProcess(
        0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_VERTICES, "Atomic Numbers");
}

//------------------------------------------------------------------------------
MapMolecule::~MapMolecule()
{
    this->SetLookupTable(nullptr);
    this->SetAtomicRadiusArrayName(nullptr);
}

//------------------------------------------------------------------------------
void MapMolecule::SetInputData(Molecule *input)
{
    this->SetInputDataInternal(0, input);
}

//------------------------------------------------------------------------------
Molecule *MapMolecule::GetInput()
{
    return Molecule::SafeDownCast(this->GetExecutive()->GetInputData(0, 0));
}

//------------------------------------------------------------------------------
const char *MapMolecule::GetAtomicRadiusTypeAsString()
{
    switch (this->AtomicRadiusType)
    {
    case CovalentRadius:
        return "CovalentRadius";
    case VDWRadius:
        return "VDWRadius";
    case UnitRadius:
        return "UnitRadius";
    case CustomArrayRadius:
        return "CustomArrayRadius";
    default:
        return "Invalid";
    }
}

//------------------------------------------------------------------------------
const char *MapMolecule::GetBondColorModeAsString()
{
    switch (this->BondColorMode)
    {
    case SingleColor:
        return "SingleColor";
    case DiscreteByAtom:
        return "DiscreteByAtom";
    default:
        return "Invalid";
    }
}

//------------------------------------------------------------------------------
void MapMolecule::GetSelectedAtomsAndBonds(
    vtkSelection *selection, vtkIdTypeArray *atomIds, vtkIdTypeArray *bondIds)
{
    // Sanity check
    if (selection == nullptr || (atomIds == nullptr && bondIds == nullptr))
    {
        return;
    }

    // Clear the inputs
    if (atomIds != nullptr)
    {
        atomIds->Reset();
    }
    if (bondIds != nullptr)
    {
        bondIds->Reset();
    }

    const vtkIdType numAtoms = this->GetInput()->GetNumberOfAtoms();
    const vtkIdType numBonds = this->GetInput()->GetNumberOfBonds();
    const vtkIdType numAtomsAndBonds = numAtoms + numBonds;

    // Find selection node that we're interested in:
    const vtkIdType numNodes = selection->GetNumberOfNodes();
    for (vtkIdType nodeId = 0; nodeId < numNodes; ++nodeId)
    {
        vtkSelectionNode *node = selection->GetNode(nodeId);

        // Check if the mapper is this instance of MapMolecule
        vtkActor *selActor =
            vtkActor::SafeDownCast(node->GetProperties()->Get(vtkSelectionNode::PROP()));
        if (selActor && (selActor->GetMapper() == this))
        {
            // Separate the selection ids into atoms and bonds
            vtkIdTypeArray *selIds = vtkArrayDownCast<vtkIdTypeArray>(node->GetSelectionList());
            if (selIds)
            {
                vtkIdType numIds = selIds->GetNumberOfTuples();
                for (vtkIdType i = 0; i < numIds; ++i)
                {
                    vtkIdType curId = selIds->GetValue(i);
                    if (atomIds != nullptr && curId < numAtoms) // atoms
                    {
                        atomIds->InsertNextValue(curId);
                    }
                    else if (bondIds != nullptr && curId < numAtomsAndBonds) // bonds
                    {
                        // Remove offset
                        curId -= numAtoms;
                        bondIds->InsertNextValue(curId);
                    }
                }
            }
        }
    }
}

//------------------------------------------------------------------------------
void MapMolecule::Render(vtkRenderer *ren, vtkActor *act)
{
    // If we add more rendering backend (e.g. point sprites), add a switch here.
    this->GlyphRender(ren, act);
}

//------------------------------------------------------------------------------
void MapMolecule::GlyphRender(vtkRenderer *ren, vtkActor *act)
{
    // Update cached polydata if needed
    this->UpdateGlyphPolyData();

    // Pass rendering call on
    this->AtomGlyphMapper->Render(ren, act);

    if (style_->HasToRenderBonds())
    {
        this->BondGlyphMapper->Render(ren, act);
    }
}

//------------------------------------------------------------------------------
void MapMolecule::UpdateGlyphPolyData()
{
    Molecule *molecule = this->GetInput();

    if (!this->GlyphDataInitialized ||
        ((molecule->GetMTime() > this->AtomGlyphPolyData->GetMTime() ||
          this->GetMTime() > this->AtomGlyphPolyData->GetMTime() ||
          this->LookupTable->GetMTime() > this->AtomGlyphPolyData->GetMTime())))
    {
        this->UpdateAtomGlyphPolyData();
    }

    if (!this->GlyphDataInitialized ||
        ((molecule->GetMTime() > this->BondGlyphPolyData->GetMTime() ||
          this->GetMTime() > this->BondGlyphPolyData->GetMTime() ||
          this->LookupTable->GetMTime() > this->BondGlyphPolyData->GetMTime()) &&
         style_->HasToRenderBonds()))
    {
        this->UpdateBondGlyphPolyData();
    }

    this->GlyphDataInitialized = true;
}
void MapMolecule::SetStyle(StyleMapMolecule newstyle)
{
    style_->Reset(newstyle);
    this->GlyphDataInitialized = false;
}
//------------------------------------------------------------------------------
// Generate scale and position information for each atom sphere
void MapMolecule::UpdateAtomGlyphPolyData()
{
    this->AtomGlyphPolyData->Initialize();

    Molecule *molecule = this->GetInput();

    vtkAbstractArray *inputColorArray = this->GetInputAbstractArrayToProcess(0, molecule);
    vtkAbstractArray *colorArray = nullptr;
    vtkUnsignedCharArray *singleColorArray = nullptr;
    switch (this->AtomColorMode)
    {
    case SingleColor:
    {
        colorArray = vtkUnsignedCharArray::New();
        colorArray->SetNumberOfComponents(3);
        vtkIdType size = 3 * molecule->GetNumberOfAtoms();
        colorArray->Allocate(size);
        colorArray->SetName("Colors");
        singleColorArray = vtkArrayDownCast<vtkUnsignedCharArray>(colorArray);
        this->AtomGlyphPolyData->GetPointData()->SetScalars(singleColorArray);
        break;
    }
    case DiscreteByAtom:
    default:
        if (inputColorArray != nullptr)
        {
            colorArray = inputColorArray->NewInstance();
            colorArray->SetNumberOfComponents(inputColorArray->GetNumberOfComponents());
            colorArray->Allocate(colorArray->GetNumberOfComponents() * molecule->GetNumberOfAtoms());
        }
        this->AtomGlyphMapper->SetLookupTable(this->LookupTable);
        break;
    }

    vtkNew<vtkUnsignedShortArray> atomicNbWithoutGhostArray;
    vtkUnsignedShortArray *atomicNbFullArray = molecule->GetAtomicNumberArray();
    vtkNew<vtkPoints> points;
    vtkPoints *allPoints = molecule->GetAtomicPositionArray();
    vtkUnsignedCharArray *ghosts = molecule->GetAtomGhostArray();
    for (vtkIdType i = 0; i < molecule->GetNumberOfAtoms(); i++)
    {
        /**
         * Skip ghost atoms but not ghost bonds:
         *  - each atom is non-ghost for exactly one MPI node, that will handle it.
         *  - a ghost bond links an atom and a ghost atom. So there is exactly two MPI nodes
         *    that contain this ghost bond and no one that contains this bond as non-ghost.
         *    We let this two MPI nodes handle the ghost bond, as we cannot know if the bond
         *    was already handled.
         */
        if (ghosts && ghosts->GetValue(i) == 1)
        {
            continue;
        }
        atomicNbWithoutGhostArray->InsertNextValue(atomicNbFullArray->GetValue(i));
        points->InsertNextPoint(allPoints->GetPoint(i));
        if (this->AtomColorMode == SingleColor)
        {
            singleColorArray->InsertNextTypedTuple(this->AtomColor);
        }
        else if (colorArray != nullptr && inputColorArray != nullptr)
        {
            colorArray->InsertNextTuple(i, inputColorArray);
        }
    }
    const vtkIdType numAtoms = points->GetNumberOfPoints();

    if (colorArray != nullptr)
    {
        if (colorArray->GetNumberOfTuples() != numAtoms)
        {
            vtkErrorMacro("Color array size does not match number of atoms.");
        }
        else if (this->AtomColorMode == DiscreteByAtom)
        {
            int colorArrayIdx = this->AtomGlyphPolyData->GetPointData()->AddArray(colorArray);
            this->AtomGlyphMapper->SelectColorArray(colorArrayIdx);
        }
        colorArray->Delete();
    }

    this->AtomGlyphPolyData->SetPoints(points.Get());

    vtkNew<vtkFloatArray> scaleFactors;
    scaleFactors->SetNumberOfComponents(1);
    scaleFactors->SetName("Scale Factors");
    scaleFactors->Allocate(numAtoms);

    switch (this->AtomicRadiusType)
    {
    default:
        vtkWarningMacro(<< "Unknown radius type: " << this->AtomicRadiusType
                        << ". Falling back to 'VDWRadius' (" << VDWRadius << ").");
        VTK_FALLTHROUGH;
    case VDWRadius:
        for (vtkIdType i = 0; i < numAtoms; ++i)
        {
            scaleFactors->InsertNextValue(this->AtomicRadiusScaleFactor *
                                          this->PeriodicTable->GetVDWRadius(atomicNbWithoutGhostArray->GetValue(i)));
        }
        break;
    case CovalentRadius:
        for (vtkIdType i = 0; i < numAtoms; ++i)
        {
            scaleFactors->InsertNextValue(this->AtomicRadiusScaleFactor *
                                          this->PeriodicTable->GetCovalentRadius(atomicNbWithoutGhostArray->GetValue(i)));
        }
        break;
    case UnitRadius:
        for (vtkIdType i = 0; i < numAtoms; ++i)
        {
            scaleFactors->InsertNextValue(this->AtomicRadiusScaleFactor);
        }
        break;
    case CustomArrayRadius:
    {
        vtkDataArray *allRadii = molecule->GetVertexData()->GetArray(this->AtomicRadiusArrayName);
        if (!allRadii)
        {
            vtkWarningMacro("AtomicRadiusType set to CustomArrayRadius, but no "
                            "array named "
                            << this->AtomicRadiusArrayName << " found in input VertexData.");
            scaleFactors->SetNumberOfTuples(numAtoms);
            scaleFactors->FillComponent(0, this->AtomicRadiusScaleFactor);
            break;
        }
        vtkNew<vtkDoubleArray> radii;
        for (vtkIdType i = 0; i < molecule->GetNumberOfAtoms(); i++)
        {
            if (ghosts && ghosts->GetValue(i) == 1)
            {
                continue;
            }
            radii->InsertNextValue(allRadii->GetTuple1(i));
        }
        if (radii->GetNumberOfTuples() != numAtoms)
        {
            vtkWarningMacro("'radii' array contains "
                            << radii->GetNumberOfTuples() << " entries, but there are " << numAtoms << " atoms.");
            scaleFactors->SetNumberOfTuples(numAtoms);
            scaleFactors->FillComponent(0, this->AtomicRadiusScaleFactor);
        }
        else
        {
            scaleFactors->DeepCopy(radii);
            scaleFactors->SetName("Scale Factors"); // copy resets name.
        }
        break;
    }
    }

    this->AtomGlyphPolyData->GetPointData()->AddArray(scaleFactors);
    this->AtomGlyphMapper->SetScaleArray("Scale Factors");
}

//------------------------------------------------------------------------------
// Generate position, scale, and orientation vectors for each bond cylinder
void MapMolecule::UpdateBondGlyphPolyData()
{
    this->BondGlyphPolyData->Initialize();

    Molecule *molecule = this->GetInput();

    // For the selection ID offset:
    const vtkIdType numAtoms = molecule->GetNumberOfAtoms();

    // number of bonds itself:
    const vtkIdType numBonds = molecule->GetNumberOfBonds();

    // Create arrays
    vtkNew<vtkPoints> cylCenters;
    vtkNew<vtkFloatArray> cylScales;
    vtkNew<vtkFloatArray> orientationVectors;
    // Since vtkHardwareSelector won't distinguish between the internal instance
    // mappers of MapMolecule, use a custom selection ID range. This also
    // fixes the issue of bonds that are colored-by-atom, as these are rendered
    // as two glyphs.
    vtkNew<vtkIdTypeArray> selectionIds;

    // Setup arrays
    // vtkPoints uses three components by default
    cylScales->SetNumberOfComponents(3);
    orientationVectors->SetNumberOfComponents(3);
    selectionIds->SetNumberOfComponents(1);

    // Name arrays
    // Can't set name for points
    cylScales->SetName("Scale Factors");
    orientationVectors->SetName("Orientation Vectors");
    selectionIds->SetName("Selection Ids");

    // Allocate memory -- find out how many cylinders are needed
    vtkIdType numCylinders = numBonds;
    // Up to three cylinders per bond if multicylinders are enabled:
    if (style_->AreBondsMulticylinder())
    {
        numCylinders *= 3;
    }
    // If DiscreteByAtom coloring is used, each cylinder is represented
    // by two individual cylinders
    if (this->BondColorMode == DiscreteByAtom)
    {
        numCylinders *= 2;
    }

    // Allocate memory. Multiply numCylinders by number of components in array.
    cylCenters->Allocate(3 * numCylinders);
    cylScales->Allocate(3 * numCylinders);
    orientationVectors->Allocate(3 * numCylinders);
    selectionIds->Allocate(numCylinders);

    // Add arrays to BondGlyphPolyData
    this->BondGlyphPolyData->SetPoints(cylCenters);
    this->BondGlyphPolyData->GetPointData()->AddArray(cylScales);
    this->BondGlyphPolyData->GetPointData()->AddArray(orientationVectors);
    this->BondGlyphPolyData->GetPointData()->AddArray(selectionIds);

    // Set up coloring mode
    vtkAbstractArray *atomColorArray = this->GetInputAbstractArrayToProcess(0, molecule);
    vtkUnsignedCharArray *singleColorArray = nullptr;
    vtkAbstractArray *cylColors = nullptr;

    bool useColorArray = this->BondColorMode == DiscreteByAtom && atomColorArray != nullptr;

    // In DiscreteByAtom mode, coloring is done with the data used for atom
    // coloring. So we need an array of the same type, with data corresponding to begin and end atom
    // for each bond : { beginAtom0, endAtom0, beginAtom1, endAtom1, ... }
    if (useColorArray)
    {
        cylColors = atomColorArray->NewInstance();
        cylColors->SetNumberOfComponents(atomColorArray->GetNumberOfComponents());
        cylColors->Allocate(atomColorArray->GetNumberOfComponents() * numCylinders);
        cylColors->SetName("Colors");
        this->BondGlyphMapper->SetScalarRange(0, this->PeriodicTable->GetNumberOfElements());
        this->BondGlyphMapper->SetColorMode(this->AtomGlyphMapper->GetColorMode());
        this->BondGlyphMapper->SetScalarModeToUsePointFieldData();
        this->BondGlyphMapper->SetLookupTable(this->LookupTable);
    }
    // Otherwise an unique color will be used, so we fill a 3-components unsigned char array with
    // BondColor value.
    else
    {
        singleColorArray = vtkUnsignedCharArray::New();
        singleColorArray->SetNumberOfComponents(3);
        singleColorArray->Allocate(3 * numCylinders);
        singleColorArray->SetName("Colors");
        cylColors = singleColorArray;
        this->BondGlyphPolyData->GetPointData()->SetScalars(singleColorArray);
        this->BondGlyphMapper->SetColorModeToDefault();
        this->BondGlyphMapper->SetScalarModeToUsePointData();
    }

    // Declare some variables for later
    unsigned short bondOrder;
    float bondLength;
    // Since the input cylinder is oriented along the z axis, the
    // scale vector should be [radius, radius, bondLength]
    vtkVector3f scale;
    // Current cylinder's selection id
    vtkIdType selectionId;
    // Distance between multicylinder surfaces is approx. 1/3 of the
    // diameter:
    const float deltaLength = this->BondRadius * 2.6;
    // Vector between centers cylinders in a multibond:
    vtkVector3f delta;
    delta.Set(0.0, 0.0, 0.0);
    // The initial displacement when generating a multibond
    vtkVector3f initialDisp;
    initialDisp.Set(0.0, 0.0, 0.0);
    // The geometric center of the bond
    vtkVector3f bondCenter;
    // The center of the current cylinder
    vtkVector3f cylinderCenter;
    // Used in DiscreteByAtom color mode:
    vtkVector3f halfCylinderCenter;
    // The positions of the start and end atoms
    vtkVector3f pos1, pos2;
    // Array containing the atom ids of {begin, end} atoms
    vtkIdType atomIds[2];
    // Normalized vector pointing along bond from begin->end atom
    vtkVector3f bondVec;
    // Unit z vector -- used for multicylinder orientation
    static const vtkVector3f unitZ(0.0, 0.0, 1.0);

    // Generate the scale, orientation, and position of each cylinder
    for (vtkIdType bondInd = 0; bondInd < numBonds; ++bondInd)
    {
        selectionId = numAtoms + bondInd; // mixing 1 and 0 indexed ids on purpose
        // Extract bond info
        Bond bond = molecule->GetBond(bondInd);
        bondOrder = bond.GetOrder();
        pos1 = bond.GetBeginAtom().GetPosition();
        pos2 = bond.GetEndAtom().GetPosition();
        atomIds[0] = bond.GetBeginAtomId();
        atomIds[1] = bond.GetEndAtomId();

        // Compute additional bond info
        // - Normalized vector in direction of bond
        bondVec = pos2 - pos1;
        bondLength = bondVec.Normalize();
        // - Center of bond for translation
        // TODO vtkVector scalar multiplication
        // bondCenter = (pos1 + pos2) * 0.5;
        bondCenter[0] = (pos1[0] + pos2[0]) * 0.5;
        bondCenter[1] = (pos1[1] + pos2[1]) * 0.5;
        bondCenter[2] = (pos1[2] + pos2[2]) * 0.5;
        // end vtkVector TODO

        // Set up delta step vector and bond radius from bond order:
        if (style_->AreBondsMulticylinder())
        {
            switch (bondOrder)
            {
            case 1:
            default:
                delta.Set(0.0, 0.0, 0.0);
                initialDisp.Set(0.0, 0.0, 0.0);
                break;
            case 2:
                // TODO vtkVector scalar multiplication
                // delta = bondVec.Cross(unitZ).Normalized() * deltaLength;
                // initialDisp = delta * (-0.5);
                delta = bondVec.Cross(unitZ).Normalized();
                delta[0] *= deltaLength;
                delta[1] *= deltaLength;
                delta[2] *= deltaLength;
                initialDisp.Set(delta[0] * (-0.5), delta[1] * (-0.5), delta[2] * (-0.5));
                // End vtkVector TODO
                break;
            case 3:
                // TODO vtkVector scalar multiplication, negation
                // delta = bondVec.Cross(unitZ).Normalized() * deltaLength;
                // initialDisp = -delta;
                delta = bondVec.Cross(unitZ).Normalized();
                delta[0] *= deltaLength;
                delta[1] *= deltaLength;
                delta[2] *= deltaLength;
                initialDisp.Set(-delta[0], -delta[1], -delta[2]);
                // End vtkVector TODO
                break;
            }
        }

        // Set up cylinder scale factors
        switch (this->BondColorMode)
        {
        case SingleColor:
            scale.Set(bondLength, this->BondRadius, this->BondRadius);
            break;
        default:
        case DiscreteByAtom:
            scale.Set(0.5 * bondLength, this->BondRadius, this->BondRadius);
            break;
        }

        if (style_->AreBondsMulticylinder())
        {
            cylinderCenter = bondCenter + initialDisp;
        }
        else
        {
            cylinderCenter = bondCenter;
        }

        // For each bond order, add a point to the glyph points, translate
        // by delta, and repeat.
        for (unsigned short iter = 0; iter < bondOrder; ++iter)
        {
            // Single color mode adds a single cylinder, while
            // DiscreteByAtom adds two differently colored and positioned
            // cylinders.
            switch (this->BondColorMode)
            {
            case SingleColor:
                cylCenters->InsertNextPoint(cylinderCenter.GetData());
                cylScales->InsertNextTuple(scale.GetData());
                singleColorArray->InsertNextTypedTuple(this->BondColor);
                orientationVectors->InsertNextTuple(bondVec.GetData());
                selectionIds->InsertNextValue(selectionId);
                break;
            default:
            case DiscreteByAtom:
                // Cache some scaling factors
                const float quarterLength = 0.25 * bondLength;

                // Add cylinder for begin atom:
                // TODO vtkVector subtraction, scalar multiplication
                // halfCylinderCenter = cylinderCenter - (bondVec * quarterLength);
                halfCylinderCenter[0] = cylinderCenter[0] - (bondVec[0] * quarterLength);
                halfCylinderCenter[1] = cylinderCenter[1] - (bondVec[1] * quarterLength);
                halfCylinderCenter[2] = cylinderCenter[2] - (bondVec[2] * quarterLength);
                // end vtkVector TODO

                cylCenters->InsertNextPoint(halfCylinderCenter.GetData());
                cylScales->InsertNextTuple(scale.GetData());
                if (useColorArray)
                {
                    cylColors->InsertNextTuple(atomIds[0], atomColorArray);
                }
                else
                {
                    singleColorArray->InsertNextTypedTuple(this->BondColor);
                }
                orientationVectors->InsertNextTuple(bondVec.GetData());
                selectionIds->InsertNextValue(selectionId);

                // Add cylinder for begin atom:
                // TODO vtkVector addition, scalar multiplication
                // halfCylinderCenter = cylinderCenter + (bondVec * quarterLength);
                halfCylinderCenter[0] = cylinderCenter[0] + (bondVec[0] * quarterLength);
                halfCylinderCenter[1] = cylinderCenter[1] + (bondVec[1] * quarterLength);
                halfCylinderCenter[2] = cylinderCenter[2] + (bondVec[2] * quarterLength);
                // end vtkVector TODO

                cylCenters->InsertNextPoint(halfCylinderCenter.GetData());
                cylScales->InsertNextTuple(scale.GetData());
                if (useColorArray)
                {
                    cylColors->InsertNextTuple(atomIds[1], atomColorArray);
                }
                else
                {
                    singleColorArray->InsertNextTypedTuple(this->BondColor);
                }
                orientationVectors->InsertNextTuple(bondVec.GetData());
                selectionIds->InsertNextValue(selectionId);
            }

            // Prepare for next multicylinder
            if (style_->AreBondsMulticylinder() && bondOrder != 1)
            {
                // TODO vtkVector in-place addition
                // cylinderCenter += delta;
                cylinderCenter[0] += delta[0];
                cylinderCenter[1] += delta[1];
                cylinderCenter[2] += delta[2];
                // end vtkVector TODO
            }
        }
    }

    if (useColorArray)
    {
        int colorArrayIdx = this->BondGlyphPolyData->GetPointData()->AddArray(cylColors);
        this->BondGlyphMapper->SelectColorArray(colorArrayIdx);
    }
    cylColors->Delete();
    // Free up some space
    this->BondGlyphPolyData->Squeeze();

    // Setup glypher
    this->BondGlyphMapper->SetScaleArray("Scale Factors");
    this->BondGlyphMapper->SetOrientationArray("Orientation Vectors");
    this->BondGlyphMapper->SetSelectionIdArray("Selection Ids");
    this->BondGlyphMapper->UseSelectionIdsOn();
}

//------------------------------------------------------------------------------
void MapMolecule::ReleaseGraphicsResources(vtkWindow *w)
{
    this->AtomGlyphMapper->ReleaseGraphicsResources(w);
    this->BondGlyphMapper->ReleaseGraphicsResources(w);
}

double *MapMolecule::GetBounds()
{
    Molecule *input = this->GetInput();
    if (!input)
    {
        vtkMath::UninitializeBounds(this->Bounds);
    }
    else
    {
        if (!this->Static)
        {
            this->Update();
        }
        input->GetBounds(this->Bounds);
        // Pad bounds by 3 Angstrom to contain spheres, etc
        // TODO: Insert this padding in the cycle?
        this->Bounds[0] -= 3.0;
        this->Bounds[1] += 3.0;
        this->Bounds[2] -= 3.0;
        this->Bounds[3] += 3.0;
        this->Bounds[4] -= 3.0;
        this->Bounds[5] += 3.0;
    }
    return this->Bounds;
}

//------------------------------------------------------------------------------
int MapMolecule::FillInputPortInformation(int vtkNotUsed(port), vtkInformation *info)
{
    // info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkMolecule");
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "Molecule");
    return 1;
}

//------------------------------------------------------------------------------
void MapMolecule::PrintSelf(ostream &os, vtkIndent indent)
{
    this->Superclass::PrintSelf(os, indent);

    os << indent << "Molecule Map Style:\n";
    style_->PrintSelf(os, indent.GetNextIndent());

    os << indent << "AtomGlyphMapper:\n";
    this->AtomGlyphMapper->PrintSelf(os, indent.GetNextIndent());

    os << indent << "BondGlyphMapper:\n";
    this->BondGlyphMapper->PrintSelf(os, indent.GetNextIndent());

}

//------------------------------------------------------------------------------
void MapMolecule::SetMapScalars(bool map)
{
    this->AtomGlyphMapper->SetColorMode(
        map ? VTK_COLOR_MODE_MAP_SCALARS : VTK_COLOR_MODE_DIRECT_SCALARS);
    this->BondGlyphMapper->SetColorMode(
        map ? VTK_COLOR_MODE_MAP_SCALARS : VTK_COLOR_MODE_DIRECT_SCALARS);
}
