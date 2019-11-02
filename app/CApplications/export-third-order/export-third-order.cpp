#include <iostream>

#include "boost/filesystem.hpp"

#include <DGtal/io/writers/GenericWriter.h>


#include <LPModel/terms/model/Term.h>
#include <LPModel/terms/data/CData.h>
#include <LPModel/utils/dispUtils.h>
#include <LPModel/terms/API.h>

#include "LPModel/initialization/API.h"

#include "LPModel/terms/sqc/CSqc.h"
#include "LPModel/constraints/ClosedAndConnected.h"

#include "InputData.h"
#include "InputReader.h"

#include "LPModel/python-exporter/PythonExporter.h"

using namespace LPModel;
using namespace LPModel::Terms;

using namespace PythonExporter;

typedef DIPaCUS::Representation::Image2D Image2D;
typedef DGtal::Z2i::DigitalSet DigitalSet;

DigitalSet loadImageAsDigitalSet(const std::string& imageFilePath)
{
    std::cerr << "Load Image as DigitalSet\n";

    Image2D imgInput = DGtal::GenericReader<Image2D>::import(imageFilePath);
    DigitalSet ds(imgInput.domain());
    DIPaCUS::Representation::imageAsDigitalSet(ds,imgInput);

    return ds;
}

template<int TDim>
void consistentPixel(const Constraints::ClosedAndConnected::PixelIncidence& pi,
                     unsigned long int cNum,
                     SparseMatrix<TDim>& C,
                     std::vector<double>& c)
{
    typedef Constraints::ClosedAndConnected::Pixel Pixel;

    if(pi.pixel.ct==Pixel::CellType::AuxiliarFrg)
    {
        if(pi.posIncidence) c[cNum] += -1;
        else c[cNum] += 1;
    }
    else if(pi.pixel.ct==Pixel::CellType::Variable)
    {
        if(pi.posIncidence) C.insert( {cNum,pi.pixel.varIndex}, 1 );
        else C.insert( {cNum,pi.pixel.varIndex},-1 );
    }
}

template<int TDim>
void consistentEdge(const Constraints::ClosedAndConnected::EdgeIncidence& ei,
                    unsigned long int numC,
                    SparseMatrix<TDim>& C,
                    std::vector<double>& c)
{
    if(ei.posIncidence) C.insert( {numC,ei.edge.varIndex},1 );
    else C.insert( {numC,ei.edge.varIndex},-1 );
}


void exportThirdOrder(const std::string& outputPath,
                      const DigitalSet& ds,
                      const Initialization::Grid& grid,
                      const Terms::Term& mergedTerm)
{
    unsigned long int numVars = grid.pixelMap.size() -3 + grid.edgeMap.size();

    std::vector<double> U(numVars);
    {
        for(auto it=U.begin();it!=U.end();++it) *it=0;
        const Term::UnaryMap& um = mergedTerm.unaryMap;
        for(auto it=um.begin();it!=um.end();++it)
        {
            Term::UIntMultiIndex mIndex = it->first;
            auto mit = mIndex.begin();
            unsigned long int pixel = *mit;

            if(pixel>=numVars) throw std::runtime_error("Index out of range!");

            U[pixel] = mergedTerm.unaryMap.at(mIndex);
        }
    }


    SparseMatrix<2> S;
    {
        const Term::BinaryMap& bm = mergedTerm.binaryMap;
        for(auto it=bm.begin();it!=bm.end();++it)
        {
            const Term::UIntMultiIndex& mIndex = it->first;
            auto mit = mIndex.begin();
            if(it->second !=0) S.insert( { *(mit++), *(mit++) }, it->second );
        }
    }


    SparseMatrix<3> T;
    {
        const Term::TernaryMap& tm = mergedTerm.ternaryMap;
        for(auto it=tm.begin();it!=tm.end();++it)
        {
            const Term::UIntMultiIndex& mIndex = it->first;
            auto mit = mIndex.begin();

            if(it->second !=0) T.insert( { *(mit++), *(mit++),*(mit++) },it->second );
        }
    }


    SparseMatrix<2> C;
    std::vector<double> c;
    {
        c.resize(grid.edgeMap.size()/2);

        Constraints::ClosedAndConnected::LinelConstraints lc;
        Constraints::ClosedAndConnected::closedConnectedContraints(lc,grid);

        typedef Constraints::ClosedAndConnected::Linel Linel;
        typedef Constraints::ClosedAndConnected::LinelIncidence LinelIncidence;
        typedef Constraints::ClosedAndConnected::PixelIncidence PixelIncidence;
        typedef Constraints::ClosedAndConnected::EdgeIncidence EdgeIncidence;

        int nc=0;
        for(auto it=lc.begin();it!=lc.end();++it,++nc)
        {
            const Linel& linel = it->first;
            const LinelIncidence li = it->second;

            consistentPixel(li.pixel1,nc,C,c);
            consistentPixel(li.pixel2,nc,C,c);
            consistentEdge(li.edge1,nc,C,c);
            consistentEdge(li.edge2,nc,C,c);

        }
    }

    std::ofstream ofs(outputPath);

    ofs << "numPixels = " << grid.pixelMap.size() - 3 << "\n";
    ofs << "numEdges = " << grid.edgeMap.size() << "\n";
    ofs << "numSecondOrder = " << mergedTerm.binaryMap.size() << "\n";
    ofs << "numThirdOrder = " << mergedTerm.ternaryMap.size() << "\n";
    ofs << "numConstraints = " << grid.edgeMap.size()/2 << "\n";
    ofs << "numVars = " << numVars  << "\n";

    exportPython(ofs,U,S,T,C,c);

    exportDigitalSetAsMatrix(ofs,ds);
    exportPixelMap(ofs,grid.pixelMap);

    ofs.close();
}




int main(int argc, char* argv[])
{
    InputData in;
    in = readInput(argc,argv);

    boost::filesystem::path p (in.outputPath);
    boost::filesystem::create_directories(p.remove_filename());

    std::cerr << "Preparing python model for general optimization for image: " << in.pgmInputImage << "\n"
              << "with sq-weight=" << in.sqWeight << "; data-weight=" << in.dataWeight << "\n"
              << "and optimization region width of " << in.optRegionWidth << " pixels...\n";



    DigitalSet ds = loadImageAsDigitalSet(in.pgmInputImage);


    Initialization::Parameters prm = Initialization::API::initParameters(ds,in.optRegionWidth);
    Initialization::Grid* grid = Initialization::API::readGridFromFile(in.gridObjectFile);

    Terms::Term scTerm = SquaredCurvature::API::prepare(prm,*grid,in.sqWeight);
    //Terms::Term dataTerm = DataFidelity::API::prepare(prm,grid,in.dataWeight);

    Terms::Term mergedTerm = scTerm;//Terms::API::merge(dataTerm,scTerm);

    exportThirdOrder(in.outputPath,ds,*grid,mergedTerm);


    return 0;
}