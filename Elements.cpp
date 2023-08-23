#include "Elements.h"

using namespace vtk;

#include <cctype>

vtkIdType Elements::idDefaultNewAtom = Elements::id_C;

const char *Elements::Symbol[] = {
    "Xx", "H", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na",
    "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K", "Ca", "Sc", "Ti", "V",
    "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn", "Ga", "Ge", "As", "Se", "Br",
    "Kr", "Rb", "Sr", "Y", "Zr", "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag",
    "Cd", "In", "Sn", "Sb", "Te", "I", "Xe", "Cs", "Ba", "La", "Ce", "Pr",
    "Nd", "Pm", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu",
    "Hf", "Ta", "W", "Re", "Os", "Ir", "Pt", "Au", "Hg", "Tl", "Pb", "Bi",
    "Po", "At", "Rn", "Fr", "Ra", "Ac", "Th", "Pa", "U", "Np", "Pu", "Am",
    "Cm", "Bk", "Cf", "Es", "Fm", "Md", "No", "Lr", "Rf", "Db", "Sg", "Bh",
    "Hs", "Mt", "Ds", "Rg", "Cn", "Nh", "Fl", "Mc", "Lv", "Ts", "Og", "[]"};

const char *Elements::Name[] = {
    "Dummy", "Hydrogen", "Helium", "Lithium",
    "Beryllium", "Boron", "Carbon", "Nitrogen",
    "Oxygen", "Fluorine", "Neon", "Sodium",
    "Magnesium", "Aluminium", "Silicon", "Phosphorus",
    "Sulfur", "Chlorine", "Argon", "Potassium",
    "Calcium", "Scandium", "Titanium", "Vanadium",
    "Chromium", "Manganese", "Iron", "Cobalt",
    "Nickel", "Copper", "Zinc", "Gallium",
    "Germanium", "Arsenic", "Selenium", "Bromine",
    "Krypton", "Rubidium", "Strontium", "Yttrium",
    "Zirconium", "Niobium", "Molybdenum", "Technetium",
    "Ruthenium", "Rhodium", "Palladium", "Silver",
    "Cadmium", "Indium", "Tin", "Antimony",
    "Tellurium", "Iodine", "Xenon", "Caesium",
    "Barium", "Lanthanum", "Cerium", "Praseodymium",
    "Neodymium", "Promethium", "Samarium", "Europium",
    "Gadolinium", "Terbium", "Dysprosium", "Holmium",
    "Erbium", "Thulium", "Ytterbium", "Lutetium",
    "Hafnium", "Tantalum", "Tungsten", "Rhenium",
    "Osmium", "Iridium", "Platinum", "Gold",
    "Mercury", "Thallium", "Lead", "Bismuth",
    "Polonium", "Astatine", "Radon", "Francium",
    "Radium", "Actinium", "Thorium", "Protactinium",
    "Uranium", "Neptunium", "Plutonium", "Americium",
    "Curium", "Berkelium", "Californium", "Einsteinium",
    "Fermium", "Mendelevium", "Nobelium", "Lawrencium",
    "Rutherfordium", "Dubnium", "Seaborgium", "Bohrium",
    "Hassium", "Meitnerium", "Darmstadtium", "Roentgenium",
    "Copernicium", "Nihonium", "Flerovium", "Moscovium",
    "Livermorium", "Tenessine", "Oganesson", "[::]"};

const double Elements::MeanMass[] = {
    0.0,
    1.008, 4.0026,                                                  // He
    6.940, 9.0122,                                                  // Be
    10.81, 12.011, 14.007, 15.999, 18.998, 20.180,                  // Ne
    22.990, 24.305,                                                 // Mg
    26.982, 28.085, 30.974, 32.06, 35.45, 39.95,                    // Ar
    39.098, 40.078,                                                 // Ca
    44.956, 47.867, 50.942, 51.996, 54.938, 55.845, 58.933, 58.693, // Ni
    63.546, 65.38,                                                  // Zn
    69.723, 72.630, 74.922, 78.971, 79.904, 83.798,                 // Kr
    85.468, 87.62,                                                  // Sr
    88.906, 91.224, 92.206, 95.95, 97.000, 101.07, 102.91, 106.42,  // Pd
    107.87, 112.41,                                                 // Cd
    114.82, 118.71, 121.76, 127.60, 126.90, 131.29,                 // Xe
    132.91, 137.33,                                                 // Ba
    138.91, 140.12, 140.91, 144.24, 145.00, 150.36, 151.96,         // Eu
    157.25, 158.93, 162.50, 164.93, 167.26, 168.93, 173.05,         // Yb
    174.97, 178.49, 180.95, 183.84, 186.21, 190.23, 192.22, 195.08, // Pt
    196.97, 200.59,                                                 // Hg
    204.38, 207.2, 208.98, 209.0, 210.0, 222.0,                     // Rn
    223.0, 226.0,                                                   // Ra
    227.0, 232.04, 231.04, 238.03, 237.0, 244.0, 243.0,             // Am
    247.0, 247.0, 251.0, 252.0, 257.0, 258.0, 259.0,                // No
    266.0, 267.0, 268.0, 269.0, 270.0, 269.0, 278.0, 281.0,         // Ds
    282.0, 285.0,                                                   // Cn
    286.0, 289.0, 290.0, 293.0, 294.0, 294.0,                       // Og
    0.0
};

// ----------------------------------------------------------------------------
// Martin Rahm, Roald Hoffman, N. W. Ashkroft (2016):
// "Atomic and Ionic Radii of Elements 1–96" Chem. Eur. J. 22(41), 14625-14632:
// ----------------------------------------------------------------------------
// Neutral atoms:
const double Elements::RadiiRHA0[] = {
    1.00 /* [0] */
    /* H  */,
    1.54, 1.34 /* He*/
    /* Li */,
    2.20, 2.19, 2.05, 1.90, 1.79, 1.71, 1.63, 1.56 /* Ne */
    /* Na */,
    2.25, 2.40, 2.39, 2.32, 2.23, 2.14, 2.06, 1.97 /* Ar */
    /* K  */,
    2.34, 2.70, 2.63, 2.57, 2.52, 2.33, 2.42, 2.26, 2.22, 2.19, 2.17, 2.22 /* Zn */
    /* Ga */,
    2.33, 2.34, 2.31, 2.24, 2.19, 2.12 /* Kr */
    /* Rb */,
    2.40, 2.79, 2.74, 2.68, 2.51, 2.44, 2.41, 2.37, 2.33, 2.15, 2.25, 2.38 /* Cd */
    /* In */,
    2.46, 2.48, 2.46, 2.42, 2.38, 2.32 /* Xe */
    /* >>> Cm */
    ,
    0.00 /* [*] */
};
// Not yet finished --> take to the paper and run there again:
// ----------------------------------------------------------------------------

const float Elements::CovalentRadii[][1] = {
    {7.500000e-01f}, {3.200000e-01f}, {4.600000e-01f}, {1.330000e+00f}, {1.020000e+00f}, {8.500000e-01f}, {7.500000e-01f}, {7.100000e-01f}, 
    {6.300000e-01f}, {6.400000e-01f}, {6.700000e-01f}, {1.550000e+00f}, {1.390000e+00f}, {1.260000e+00f}, {1.160000e+00f}, {1.110000e+00f}, 
    {1.030000e+00f}, {9.900000e-01f}, {9.600000e-01f}, {1.960000e+00f}, {1.710000e+00f}, {1.480000e+00f}, {1.360000e+00f}, {1.340000e+00f}, 
    {1.220000e+00f}, {1.190000e+00f}, {1.160000e+00f}, {1.110000e+00f}, {1.100000e+00f}, {1.120000e+00f}, {1.180000e+00f}, {1.240000e+00f}, 
    {1.210000e+00f}, {1.210000e+00f}, {1.160000e+00f}, {1.140000e+00f}, {1.170000e+00f}, {2.100000e+00f}, {1.850000e+00f}, {1.630000e+00f}, 
    {1.540000e+00f}, {1.470000e+00f}, {1.380000e+00f}, {1.280000e+00f}, {1.250000e+00f}, {1.250000e+00f}, {1.200000e+00f}, {1.280000e+00f}, 
    {1.360000e+00f}, {1.420000e+00f}, {1.400000e+00f}, {1.400000e+00f}, {1.360000e+00f}, {1.330000e+00f}, {1.310000e+00f}, {2.320000e+00f}, 
    {1.960000e+00f}, {1.800000e+00f}, {1.630000e+00f}, {1.760000e+00f}, {1.740000e+00f}, {1.730000e+00f}, {1.720000e+00f}, {1.680000e+00f}, 
    {1.690000e+00f}, {1.680000e+00f}, {1.670000e+00f}, {1.660000e+00f}, {1.650000e+00f}, {1.640000e+00f}, {1.700000e+00f}, {1.620000e+00f}, 
    {1.520000e+00f}, {1.460000e+00f}, {1.370000e+00f}, {1.310000e+00f}, {1.290000e+00f}, {1.220000e+00f}, {1.230000e+00f}, {1.240000e+00f}, 
    {1.330000e+00f}, {1.440000e+00f}, {1.440000e+00f}, {1.510000e+00f}, {1.450000e+00f}, {1.470000e+00f}, {1.420000e+00f}, {2.230000e+00f}, 
    {2.010000e+00f}, {1.860000e+00f}, {1.750000e+00f}, {1.690000e+00f}, {1.700000e+00f}, {1.710000e+00f}, {1.720000e+00f}, {1.660000e+00f}, 
    {1.660000e+00f}, {1.680000e+00f}, {1.680000e+00f}, {1.650000e+00f}, {1.670000e+00f}, {1.730000e+00f}, {1.760000e+00f}, {1.610000e+00f}, 
    {1.570000e+00f}, {1.490000e+00f}, {1.430000e+00f}, {1.410000e+00f}, {1.340000e+00f}, {1.290000e+00f}, {1.280000e+00f}, {1.210000e+00f}, 
    {1.220000e+00f}, {1.360000e+00f}, {1.430000e+00f}, {1.620000e+00f}, {1.750000e+00f}, {1.650000e+00f}, {1.570000e+00f}, {5.000000e-01f}};

const float Elements::VDWRadii[][1] = {
    {7.500000e-01f}, {1.200000e+00f}, {1.400000e+00f}, {2.200000e+00f}, {1.900000e+00f}, {1.800000e+00f}, {1.700000e+00f}, {1.600000e+00f}, 
    {1.550000e+00f}, {1.500000e+00f}, {1.540000e+00f}, {2.400000e+00f}, {2.200000e+00f}, {2.100000e+00f}, {2.100000e+00f}, {1.950000e+00f}, 
    {1.800000e+00f}, {1.800000e+00f}, {1.880000e+00f}, {2.800000e+00f}, {2.400000e+00f}, {2.300000e+00f}, {2.150000e+00f}, {2.050000e+00f}, 
    {2.050000e+00f}, {2.050000e+00f}, {2.050000e+00f}, {2.000000e+00f}, {2.000000e+00f}, {2.000000e+00f}, {2.100000e+00f}, {2.100000e+00f}, 
    {2.100000e+00f}, {2.050000e+00f}, {1.900000e+00f}, {1.900000e+00f}, {2.020000e+00f}, {2.900000e+00f}, {2.550000e+00f}, {2.400000e+00f}, 
    {2.300000e+00f}, {2.150000e+00f}, {2.100000e+00f}, {2.050000e+00f}, {2.050000e+00f}, {2.000000e+00f}, {2.050000e+00f}, {2.100000e+00f}, 
    {2.200000e+00f}, {2.200000e+00f}, {2.250000e+00f}, {2.200000e+00f}, {2.100000e+00f}, {2.100000e+00f}, {2.160000e+00f}, {3.000000e+00f}, 
    {2.700000e+00f}, {2.500000e+00f}, {2.480000e+00f}, {2.470000e+00f}, {2.450000e+00f}, {2.430000e+00f}, {2.420000e+00f}, {2.400000e+00f}, 
    {2.380000e+00f}, {2.370000e+00f}, {2.350000e+00f}, {2.330000e+00f}, {2.320000e+00f}, {2.300000e+00f}, {2.280000e+00f}, {2.270000e+00f}, 
    {2.250000e+00f}, {2.200000e+00f}, {2.100000e+00f}, {2.050000e+00f}, {2.000000e+00f}, {2.000000e+00f}, {2.050000e+00f}, {2.100000e+00f}, 
    {2.050000e+00f}, {2.200000e+00f}, {2.300000e+00f}, {2.300000e+00f}, {2.000000e+00f}, {2.000000e+00f}, {2.000000e+00f}, {2.000000e+00f}, 
    {2.000000e+00f}, {2.000000e+00f}, {2.400000e+00f}, {2.000000e+00f}, {2.300000e+00f}, {2.000000e+00f}, {2.000000e+00f}, {2.000000e+00f}, 
    {2.000000e+00f}, {2.000000e+00f}, {2.000000e+00f}, {2.000000e+00f}, {2.000000e+00f}, {2.000000e+00f}, {2.000000e+00f}, {2.000000e+00f}, 
    {2.000000e+00f}, {2.000000e+00f}, {2.000000e+00f}, {2.000000e+00f}, {2.000000e+00f}, {2.000000e+00f}, {1.000000e+38f}, {1.000000e+38f}, 
    {1.000000e+38f}, {1.000000e+38f}, {1.000000e+38f}, {1.000000e+38f}, {1.000000e+38f}, {1.000000e+38f}, {1.000000e+38f}, {5.000000e-01f}};

IndexElement Elements::SymbolToNumber(const char *symbol, char **save)
{
  if (!symbol || !*symbol || !isalpha(*symbol))
  {
    if (save)
      *save = nullptr;
    return 0L;
  }
  IndexElement number(id_Q);
  int k0 = toupper(*symbol++);
  int k1 = isalpha(*symbol) ? tolower(*symbol++) : 0;
  int k2 = ((k0 == 'U') && k1 && isalpha(*symbol)) ? tolower(*symbol++) : 0;
  // rare cases of the Uu? / Ub? / Ut? heavy elements
  if (save)
    *save = const_cast<char *>(symbol);

  switch (k0)
  {
  case ('A'):
  {
    if (k1 == 'c')
      number = idAc;
    else if (k1 == 'g')
      number = idAg;
    else if (k1 == 'l')
      number = idAl;
    else if (k1 == 'm')
      number = idAm;
    else if (k1 == 'r')
      number = idAr;
    else if (k1 == 's')
      number = idAs;
    else if (k1 == 't')
      number = idAt;
    else if (k1 == 'u')
      number = idAu;
    break;
  }
  case ('B'):
  {
    if (k1 == 'a')
      number = idBa;
    else if (k1 == 'e')
      number = idBe;
    else if (k1 == 'h')
      number = idBh;
    else if (k1 == 'i')
      number = idBi;
    else if (k1 == 'k')
      number = idBk;
    else if (k1 == 'r')
      number = idBr;
    else
      number = id_B;
    break;
  }
  case ('C'):
  {
    if (k1 == 'a')
      number = idCa;
    else if (k1 == 'd')
      number = idCd;
    else if (k1 == 'e')
      number = idCe;
    else if (k1 == 'f')
      number = idCf;
    else if (k1 == 'l')
      number = idCl;
    else if (k1 == 'm')
      number = idCm;
    else if (k1 == 'n')
      number = idCn;
    else if (k1 == 'o')
      number = idCo;
    else if (k1 == 'r')
      number = idCr;
    else if (k1 == 's')
      number = idCs;
    else if (k1 == 'u')
      number = idCu;
    else
      number = id_C;
    break;
  }
  case ('D'):
  {
    if (k1 == 'b')
      number = idDb;
    else if (k1 == 's')
      number = idDs;
    else if (k1 == 'y')
      number = idDy;
    break;
  }
  case ('E'):
  {
    if (k1 == 'r')
      number = idEr;
    else if (k1 == 's')
      number = idEs;
    else if (k1 == 'u')
      number = idEu;
    break;
  }
  case ('F'):
  {
    if (k1 == 'e')
      number = idFe;
    else if (k1 == 'l')
      number = idFl;
    else if (k1 == 'm')
      number = idFm;
    else
      number = id_F;
    break;
  }
  case ('G'):
  {
    if (k1 == 'a')
      number = idGa;
    else if (k1 == 'd')
      number = idGd;
    else if (k1 == 'e')
      number = idGe;
    break;
  }
  case ('H'):
  {
    if (k1 == 'e')
      number = idHe;
    else if (k1 == 'f')
      number = idHf;
    else if (k1 == 'g')
      number = idHg;
    else if (k1 == 'o')
      number = idHo;
    else if (k1 == 's')
      number = idHs;
    else
      number = id_H;
    break;
  }
  case ('I'):
  {
    if (k1 == 'n')
      number = idIn;
    else if (k1 == 'r')
      number = idIr;
    else
      number = id_I;
    break;
  }
  // case('J'):
  case ('K'):
  {
    if (k1 == 'r')
      number = idKr;
    else
      number = id_K;
    break;
  }
  case ('L'):
  {
    if (k1 == 'a')
      number = idLa;
    else if (k1 == 'i')
      number = idLi;
    else if (k1 == 'r')
      number = idLr;
    else if (k1 == 'u')
      number = idLu;
    else if (k1 == 'v')
      number = idLv;
    break;
  }
  case ('M'):
  {
    if (k1 == 'c')
      number = idMc;
    else if (k1 == 'd')
      number = idMd;
    else if (k1 == 'g')
      number = idMg;
    else if (k1 == 'o')
      number = idMo;
    else if (k1 == 'n')
      number = idMn;
    else if (k1 == 't')
      number = idMt;
    break;
  }
  case ('N'):
  {
    if (k1 == 'a')
      number = idNa;
    else if (k1 == 'b')
      number = idNb;
    else if (k1 == 'd')
      number = idNd;
    else if (k1 == 'e')
      number = idNe;
    else if (k1 == 'h')
      number = idNh;
    else if (k1 == 'i')
      number = idNi;
    else if (k1 == 'o')
      number = idNo;
    else if (k1 == 'p')
      number = idNp;
    else
      number = id_N;
    break;
  }
  case ('O'):
  {
    if (k1 == 'g')
      number = idOg;
    else if (k1 == 's')
      number = idOs;
    else
      number = id_O;
    break;
  }

  case ('P'):
  {
    if (k1 == 'a')
      number = idPa;
    else if (k1 == 'b')
      number = idPb;
    else if (k1 == 'd')
      number = idPd;
    else if (k1 == 'm')
      number = idPm;
    else if (k1 == 'o')
      number = idPo;
    else if (k1 == 'r')
      number = idPr;
    else if (k1 == 't')
      number = idPt;
    else if (k1 == 'u')
      number = idPu;
    else
      number = id_P;
    break;
  }
  // case('Q'):
  case ('R'):
  {
    if (k1 == 'a')
      number = idRa;
    else if (k1 == 'b')
      number = idRb;
    else if (k1 == 'e')
      number = idRe;
    else if (k1 == 'h')
      number = idRh;
    else if (k1 == 'f')
      number = idRf;
    else if (k1 == 'g')
      number = idRg;
    else if (k1 == 'n')
      number = idRn;
    else if (k1 == 'u')
      number = idRu;
    break;
  }
  case ('S'):
  {
    if (k1 == 'b')
      number = idSb;
    else if (k1 == 'c')
      number = idSc;
    else if (k1 == 'e')
      number = idSe;
    else if (k1 == 'g')
      number = idSg;
    else if (k1 == 'i')
      number = idSi;
    else if (k1 == 'm')
      number = idSm;
    else if (k1 == 'n')
      number = idSn;
    else if (k1 == 'r')
      number = idSr;
    else
      number = id_S;
    break;
  }
  case ('T'):
  {
    if (k1 == 'a')
      number = idTa;
    else if (k1 == 'b')
      number = idTb;
    else if (k1 == 'c')
      number = idTc;
    else if (k1 == 'e')
      number = idTe;
    else if (k1 == 'h')
      number = idTh;
    else if (k1 == 'i')
      number = idTi;
    else if (k1 == 'l')
      number = idTl;
    else if (k1 == 'm')
      number = idTm;
    else if (k1 == 's')
      number = idTs;
    break;
  }
  case ('U'):
  {
    /*
      if(k1 == 'u' && k2 == 'n') number = 119;  cast heavy element numbers 'Uu
      / else if (k1 == 'b') ; else... this is a stub...
      */
    number = id_U;
    break;
  }
  case ('V'):
  {
    number = id_V;
    break;
  }
  case ('W'):
  {
    number = id_W;
    break;
  }
  case ('X'):
  {
    if (k1 == 'e')
      number = idXe;
    break;
  }
  case ('Y'):
  {
    if (k1 == 'b')
      number = idYb;
    else
      number = id_Y;
    break;
  }
  case ('Z'):
  {
    if (k1 == 'r')
      number = idZr;
    break;
  }
  default:
    break;
  }
  return number;
}

vtkStdString Elements::GetElementSymbol(IndexElement id)
{
  vtkStdString res(*Symbol);
  if (id >= 0 && id < NumberOfKnownElements)
    res = Symbol[id];
  return res;
}
double Elements::GetMeanMass(long id)
{
  return IsValidAtomNumber(id) ? MeanMass[id] : 0.0;
}

vtkIdType Elements::GetDefaultElement()
{
  return idDefaultNewAtom;
}

bool Elements::ResetDefaultElement(vtkIdType id)
{
  if (IsValidAtomNumber(id))
    return false;
  idDefaultNewAtom = id;
  return true;
}