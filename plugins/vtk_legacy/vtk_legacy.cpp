#include <basecfd/plugin.hpp>
#include <basecfd/format.hpp>
#include <basecfd/grid.hpp>
#include <basecfd/element.hpp>
#include <basecfd/unstructured_grid.hpp>

#include <vector>
#include <stdexcept>
#include <fstream>
#include <string>
#include <memory>

#ifdef BASECFD_WITH_UNIT_TESTS
#define CATCH_CONFIG_MAIN
#include <thirdparty/catch/catch.hpp>
#include <sstream>
#endif // BASECFD_WITH_UNIT_TESTS

using namespace basecfd;

class VtkLegacyFormater : public Formater
{
public:
    VtkLegacyFormater();
    virtual Grid* load(const std::string& file, int flags);
    virtual void save(const Grid& grid, const std::string& file, int flags);

    ElementType getNativeType(int vtkType) const;

    int getVtkType(ElementType nativeType) const;
private:

    void extractMinimalCsr(MinimalCsr& csr, std::istream& is, int flags) const;

    std::vector<ElementType> typeMappingVtkToNative;
    std::map<ElementType, int> typeMappingNativeToVtk;

protected:
    void testExtractMinimalCsr(MinimalCsr& csr, std::istream& is, int flags) const
    {
        return extractMinimalCsr(csr, is, flags);
    }
};


VtkLegacyFormater::VtkLegacyFormater()
{
    ElementType ctypesRaw[] = {WrongElement,    //0
                               Point,             //1
                               WrongElement,      //2
                               Line,              //3
                               WrongElement,      //4
                               WrongElement,      //5 TRIANGLE NOT SUPPORTED NOW
                               WrongElement,      //6
                               WrongElement,      //7
                               WrongElement,      //8
                               Quadr,             //9
                               WrongElement,      //10 TETRAHEDRON NOT SUPPORTED NOW
                               WrongElement,      //11
                               WrongElement,      //12 HEXAHEDRON NOT SUPPORTED NOW
                               WrongElement,      //13
                               WrongElement,      //14
                              };
    typeMappingVtkToNative.assign(ctypesRaw, ctypesRaw + sizeof(ctypesRaw) / sizeof(*ctypesRaw));
    for (size_t i = 0; i < typeMappingVtkToNative.size(); ++i)
    {
        typeMappingNativeToVtk[typeMappingVtkToNative[i]] = static_cast<int>(i);
    }
}

Grid* VtkLegacyFormater::load(const std::string& file, int flags)
{
    std::ifstream is(file.c_str());
    if (!is)
    {
        throw std::runtime_error("Failed to open file " + file);
    }
    MinimalCsr csr;
    extractMinimalCsr(csr, is, flags);

    std::auto_ptr<UnstructuredGrid> unstructuredGrid(new UnstructuredGrid(csr));
    Grid* result = new GridWrapper<UnstructuredGrid>(unstructuredGrid.get());
    unstructuredGrid.release();
    return result;
}

void VtkLegacyFormater::save(const Grid& grid, const std::string& file, int flags)
{

}

ElementType VtkLegacyFormater::getNativeType(int vtkType) const
{
    return typeMappingVtkToNative.at(vtkType);
}

int VtkLegacyFormater::getVtkType(ElementType nativeType) const
{
    std::map<ElementType, int>::const_iterator it = typeMappingNativeToVtk.find(nativeType);
    if (it == typeMappingNativeToVtk.end())
    {
        return -1;
    }
    return it->second;
}

void VtkLegacyFormater::extractMinimalCsr(MinimalCsr& csr, std::istream& is, int flags) const
{
    const int line_maxlen = 1024;
    char line[line_maxlen];
    std::string str;
    is >> str;
    if (str != "#")
    {
        throw std::domain_error("Error: bad vtk legacy header");
    }
    is >> str;
    if (str != "vtk")
    {
        throw std::domain_error("Error: bad vtk legacy header");
    }
    is >> str;
    if (str != "DataFile")
    {
        throw std::domain_error("Error: bad vtk legacy header");
    }
    is >> str;
    if (str != "Version")
    {
        throw std::domain_error("Error: bad vtk legacy header");
    }
    double vtkVersion;
    is >> vtkVersion;
    const double vtkRequiredVersion = 2.0;
    if (vtkVersion < vtkRequiredVersion)
    {
        throw std::domain_error("Error: bad vtk version");
    }
    std::string projectName;
    is.getline(line, line_maxlen);
    is.getline(line, line_maxlen);
    if (!is.eof() && !is.fail())
    {
        projectName.assign(line);
    }
    else
    {
        throw std::domain_error("Error: bad vtk project name");
    }
    is >> str;
    if (str != "ASCII")
    {
        throw std::domain_error("Error: vtk file has no ASCII format");
    }
    is >> str;
    if (str != "DATASET")
    {
        throw std::domain_error("Error: bad vtk legacy header (expected DATASET block)");
    }
    is >> str;
    if (str != "UNSTRUCTURED_GRID")
    {
        throw std::domain_error("Error: improper mesh type (expected UNSTRUCTURED_GRID)");
    }
    is >> str;
    if (str != "POINTS")
    {
        throw std::domain_error("Error: improper data block (expected POINTS)");
    }
    unsigned np;
    is >> np;
    std::map<std::string, size_t> typeSizes;
    typeSizes["bit"] = 0;
    typeSizes["unsigned_char"] = 1;
    typeSizes["char"] = 1;
    typeSizes["unsigned_short"] = 2;
    typeSizes["short"] = 2;
    typeSizes["unsigned_int"] = 4;
    typeSizes["int"] = 4;
    typeSizes["unsigned_long"] = 8;
    typeSizes["long"] = 8;
    typeSizes["float"] = 4;
    typeSizes["double"] = 8;
    is >> str;
    if (typeSizes.find(str) == typeSizes.end())
    {
        throw std::domain_error("Error: improper dataType in POINTS");
    }

    std::vector<Vector> mp(np);
    for (unsigned p = 0; p < np; ++p)
        for (unsigned d = 0; d < Vector::MAXDIM; ++d)
        {
            is >> mp[p][d];
            if (is.eof() || is.fail())
            {
                throw std::domain_error("Error: unexpected end of file (or error) while reading POINTS");
            }
        }
    is >> str;
    if (str != "CELLS")
    {
        throw std::domain_error("Error: improper data block (expected CELLS)");
    }
    unsigned nc;
    unsigned nCellsRawData;
    is >> nc >> nCellsRawData;
    if (2 * nc > nCellsRawData)
    {
        throw std::domain_error("Error: 2X number of cells must be less than (CELLS size)");
    }

    std::vector<idx_t> eptr(nc + 1, BAD_IDX);
    eptr[0] = 0;
    std::vector<idx_t> eind(nCellsRawData - nc, BAD_IDX);
    for (size_t c = 0; c < nc; ++c)
    {
        idx_t ncp;
        is >> ncp;
        nCellsRawData--;
        if (ncp == 0)
        {
            throw std::domain_error("Error: cell must have at least 1 point");
        }
        eptr[c + 1] = eptr[c] + ncp;
        if (eptr[c + 1] > static_cast<idx_t>(eind.size()))
        {
            throw std::domain_error("Error: bad cell size (size is exhausted)");
        }
        for (idx_t i = 0; i < ncp; ++i)
        {
            is >> eind[eptr[c] + i];
            nCellsRawData--;
            if (is.eof() || is.fail())
            {
                throw std::domain_error("Error: unexpected end of file (or error) while reading CELLS");
            }
        }
    }
    if (nCellsRawData != 0)
    {
        throw std::domain_error("Error: bad size or cells structure");
    }
    is >> str;
    if (str != "CELL_TYPES")
    {
        throw std::domain_error("Error: improper data block (expected CELL_TYPES)");
    }
    size_t ctypenc;
    is >> ctypenc;
    if (ctypenc != nc)
    {
        throw std::domain_error("Error: bad cell number in CELL_TYPES");
    }
    std::vector<ElementType> types(nc, WrongElement);
    for (size_t c = 0; c < nc; ++c)
    {
        size_t typeInt;
        is >> typeInt;
        ElementType type = WrongElement;
        if (typeInt < typeMappingVtkToNative.size())
        {
            type = typeMappingVtkToNative[typeInt];
        }
        if (type == WrongElement)
        {
            throw std::domain_error("Error: cell type is wrong or unsupported");
        }
        types[c] = type;
    }
    is >> str;
    std::vector<std::vector<double> > f_in;
    std::vector<std::string> names;
    int r_mark = -1;
    if (!is.eof() && !is.fail())
    {
        if (str != "CELL_DATA")
        {
            throw std::domain_error("Error: improper data block (expected CELL_DATA or nothing, POINT_DATA is unsupported)");
        }
        size_t fnc;
        is >> fnc;
        if (fnc != nc)
        {
            throw ("Error: bad cell number in CELL_DATA");
        }
        do
        {
            is >> str;
            if (is.eof() || is.fail())
            {
                break;
            }
            if (str != "SCALARS")
            {
                throw std::domain_error("Error: improper data block (expected SCALARS ot nothing, other types not supported)");
            }
            std::string compName;
            is >> compName;
            if ((flags & BOUNDARY_COMPONENT) && compName == "Boundary" && r_mark == -1)
            {
                r_mark = static_cast<int>(f_in.size());
            }
            else
            {
                names.push_back(compName);
            }
            std::string compType;
            is >> compType;
            if (typeSizes.find(compType) == typeSizes.end())
            {
                throw std::domain_error("Error: improper dataType in CELL_DATA SCALARS");
            }
            size_t compNum;
            is >> compNum;
            if (compNum != 1)
            {
                throw std::domain_error("Error: component size must be equal 1 (other values are unsupported)");
            }
            f_in.push_back(std::vector<double>());
            f_in.back().resize(nc);
            is >> str;
            if (str != "LOOKUP_TABLE")
            {
                throw std::domain_error("Error: improper data block (expected LOOKUP_TABLE)");
            }
            std::string tableName;
            is >> tableName;
            for (size_t i = 0; i < nc; ++i)
            {
                is >> f_in.back()[i];
                if (is.eof() || is.fail())
                {
                    throw std::domain_error("Error: unexpected end of file (or error) while reading CELL_DATA");
                }
            }
        }
        while (!is.eof() && !is.fail());
    }
    std::vector<int> r(nc, 0);
    if (r_mark != -1)
    {
        std::vector<std::vector<double> >::iterator it = f_in.begin() + r_mark;
        for (size_t c = 0; c < nc; ++c)
        {
            r[c] = static_cast<int>((*it)[c]);
        }
        f_in.erase(it);
    }
    csr.eind = eind;
    csr.eptr = eptr;
    if (squash(f_in, csr.f) == 0 && !f_in.empty())
    {
        throw std::runtime_error("Failed to load F vector");
    }
    csr.mp = mp;
    csr.r = r;
    csr.types = types;
}

#ifdef BASECFD_WITH_UNIT_TESTS
TEST_CASE_METHOD(VtkLegacyFormater, "A one-cell 2D unstructured VTK mesh", "[vtk]")
{
    const std::string data = "# vtk DataFile Version 3.0\n\
vtk output\n\
ASCII\n\
DATASET UNSTRUCTURED_GRID\n\
POINTS 4 float\n\
0 0 0\n\
1 0 0\n\
0 1 0\n\
1.1 1.1 0\n\
CELLS 1 5\n\
4 0 1 3 2\n\
CELL_TYPES 1\n\
9\n\
CELL_DATA 1\n\
SCALARS scalar1 float 1\n\
LOOKUP_TABLE default\n\
5.0\n\
            ";
    MinimalCsr csr;
    std::istringstream is(data);
    REQUIRE_NOTHROW(testExtractMinimalCsr(csr, is, 0));
    REQUIRE(csr.eptr.size() == 2);
    REQUIRE(csr.eptr.front() == 0);
    REQUIRE(csr.eptr.back() == 4);
    REQUIRE(csr.eind.size() == 4);
    REQUIRE(csr.eind[0] == 0);
    REQUIRE(csr.eind[1] == 1);
    REQUIRE(csr.eind[2] == 3);
    REQUIRE(csr.eind[3] == 2);
}

#endif // BASECFD_WITH_UNIT_TESTS
