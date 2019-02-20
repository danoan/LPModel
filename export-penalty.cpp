#include <iostream>

#include <LPModel/linearization/model/Linearization.h>
#include <DGtal/io/writers/GenericWriter.h>
#include <LPModel/terms/model/Term.h>
#include <LPModel/terms/data/CData.h>
#include <LPModel/utils/dispUtils.h>
#include <LPModel/terms/API.h>

#include "LPModel/initialization/API.h"
#include "LPModel/initialization/Shapes.h"

#include "LPModel/terms/sqc/CSqc.h"
#include "LPModel/constraints/ClosedAndConnected.h"
#include "LPModel/lpwriter/LPWriter.h"

#include "boost/filesystem.hpp"

using namespace LPModel;
using namespace LPModel::Terms;

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

void saveObjects(const std::string& outputPath,
                 const DigitalSet& dsOriginal,
                 const Initialization::Grid& grid)
{
    std::cerr << "Saving Objects\n";

    Initialization::API::save(dsOriginal,outputPath+"/prm.pgm");
    Initialization::API::save(grid,outputPath+"/grid.obj");
}

std::string resolveLPOutputFilePath(const std::string& outputFolder,
                                    const std::string& imageFilePath)
{
    boost::filesystem::path p(imageFilePath);
    return outputFolder + "/" + p.stem().string() + ".lp";
}

typedef std::vector< std::vector<double> > SquareMatrix;
typedef std::vector< std::vector<double> > ConstraintMatrix;

void initSquareMatrix(SquareMatrix& m, unsigned long int size)
{
    m.resize(size);
    for(auto it=m.begin();it!=m.end();++it)
    {
        it->resize(size);
        for(auto it2=it->begin();it2!=it->end();++it2) *it2=0;
    }
}

void initConstraintMatrix(ConstraintMatrix& m, unsigned long int numRows, unsigned long int numCols)
{
    m.resize(numRows);
    for(auto it=m.begin();it!=m.end();++it)
    {
        it->resize(numCols);
        for(auto it2=it->begin();it2!=it->end();++it2) *it2=0;
    }
}

void consistentPixel(const Constraints::ClosedAndConnected::PixelIncidence& pi,
                     unsigned long int cNum,
                     ConstraintMatrix& C,
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
        if(pi.posIncidence) C[cNum][pi.pixel.varIndex] = 1;
        else C[cNum][pi.pixel.varIndex] = -1;
    }
}

void consistentEdge(const Constraints::ClosedAndConnected::EdgeIncidence& ei,
                    unsigned long int numC,
                    ConstraintMatrix& C,
                    std::vector<double>& c)
{
    if(ei.posIncidence) C[numC][ei.edge.varIndex] = 1;
    else C[numC][ei.edge.varIndex] = -1;
}


void writeVector(std::ofstream& ofs,
                 const std::string nameVar,
                 const std::vector<double>& v)
{
    if(v.size()==0) return;

    auto it = v.begin();
    ofs << nameVar << " = [ " << *it; ++it;
    for(;it!=v.end();++it)
    {
        ofs << "," << *it;
    }
    ofs << "]\n\n";
}

void writeMatrix(std::ofstream& ofs,
                 const std::string nameVar,
                 const SquareMatrix& m)
{
    if(m.size()==0) return;

    ofs << nameVar << " = [ ";
    for(auto itR = m.begin();itR!=m.end();++itR)
    {
        auto itC=itR->begin();
        ofs << " [ " << *itC; ++itC;
        for(;itC!=itR->end();++itC)
        {
            ofs << "," << *itC ;
        }
        ofs << " ] ";
        if(itR+1!=m.end()) ofs << ", ";
    }
    ofs << " ] \n\n";
}

void exportPython(std::ofstream& ofs,
                  const std::vector<double>& U,
                  const SquareMatrix& P1,
                  const SquareMatrix& P2,
                  const ConstraintMatrix& Z,
                  const std::vector<double>& z,
                  const ConstraintMatrix& C,
                  const std::vector<double>& c)
{
    writeVector(ofs,"_U",U);
    writeMatrix(ofs,"_P1",P1);
    writeMatrix(ofs,"_P2",P2);
    writeMatrix(ofs,"_Z",Z);
    writeVector(ofs,"_z",z);
    writeMatrix(ofs,"_C",C);
    writeVector(ofs,"_c",c);
}

int main(int argc, char* argv[])
{
    if(argc<6)
    {
        std::cerr <<"Expected pgm-input-image levels sq-weight data-weight output-path\n";
        exit(1);
    }

    std::string pgmInputImage = argv[1];
    int levels = std::atoi(argv[2]);
    double sqWeight = std::atof( argv[3] );
    double dataWeight = std::atof( argv[4] );
    LPWriter::RelaxationLevel relLevel = (LPWriter::RelaxationLevel) std::atoi( argv[5] );
    std::string outputPath = argv[6];

    boost::filesystem::create_directories(outputPath);
    std::cerr << "Preparing LP for image: " << pgmInputImage << "\n"
              << "with sq-weight=" << sqWeight << "; data-weight=" << dataWeight << "\n"
              << "with relaxation level=" << relLevel
              << "; and " << levels << " levels...\n";



    DigitalSet ds = loadImageAsDigitalSet(pgmInputImage);


    Initialization::Parameters prm = Initialization::API::initParameters(ds,levels);
    Utils::exportODRModel(prm,outputPath+"/odr-model.eps");


    Initialization::Grid grid = Initialization::API::createGrid(prm.odrModel.optRegion,
                                                                prm);

    Terms::Term scTerm = SquaredCurvature::API::prepare(prm,grid,sqWeight);
    //Terms::Term dataTerm = DataFidelity::API::prepare(prm,grid,dataWeight);

    Terms::Term mergedTerm = scTerm;//Terms::API::merge(dataTerm,scTerm);


    unsigned long nextIndex = grid.pixelMap.size()+grid.linelMap.size()+grid.edgeMap.size();
    LPWriter::MyLinearization linearization(nextIndex);
    Terms::Term::BinaryMap partialL = linearization.partialLinearization(mergedTerm.ternaryMap);

    unsigned long int numPixelPairs = 0;
    for(auto it=linearization.ubegin();it!=linearization.uend();++it) ++numPixelPairs;

    unsigned long int numSlack = numPixelPairs*3;
    unsigned long int numVars = grid.pixelMap.size() -3 + grid.edgeMap.size() + numPixelPairs + numSlack;

    unsigned long int slackStart = nextIndex+numPixelPairs;

    std::vector<double> U(numVars);
    {
        for(auto it=U.begin();it!=U.end();++it) *it=0;
        const Term::UnaryMap& um = mergedTerm.unaryMap;
        for(auto it=um.begin();it!=um.end();++it)
        {
            Term::UIntMultiIndex mIndex = it->first;
            auto mit = mIndex.begin();
            unsigned long int pixel = *mit;

            U[pixel] = mergedTerm.unaryMap.at(mIndex);
        }
    }

    SquareMatrix P1;
    {
        initSquareMatrix(P1,numVars);
        const Term::BinaryMap& bm = mergedTerm.binaryMap;
        for(auto it=bm.begin();it!=bm.end();++it)
        {
            const Term::UIntMultiIndex& mIndex = it->first;
            auto mit = mIndex.begin();
            unsigned long int pixel = *mit;++mit;
            unsigned long int edge = *mit;

            P1[pixel][edge] = it->second;
        }
    }


    SquareMatrix P2;
    {
        initSquareMatrix(P2,numVars);
        const Term::BinaryMap& bm = partialL;
        for(auto it=bm.begin();it!=bm.end();++it)
        {
            const Term::UIntMultiIndex& mIndex = it->first;
            auto mit = mIndex.begin();
            unsigned long int edge = *mit;++mit;
            unsigned long int pixelPair = *mit;

            P2[pixelPair][edge] = it->second;
        }
    }

    //RHS constants missing
    ConstraintMatrix Z;
    std::vector<double> z;
    {
        unsigned long int slackBaseIndex = slackStart;
        unsigned long int baseIndex = 0;
        initConstraintMatrix(Z,numSlack,numVars);
        z.resize(numSlack);
        for(auto it=linearization.ubegin();it!=linearization.uend();++it)
        {
            const Term::UIntMultiIndex& mIndex = it->first;
            auto mit = mIndex.begin();
            unsigned long int pixel1 = *mit; ++mit;
            unsigned long int pixel2 = *mit;
            unsigned long int auxVarIndex = it->second;

            Z[baseIndex*3][pixel1]=-1;
            Z[baseIndex*3][auxVarIndex]=1;
            Z[baseIndex*3][slackBaseIndex+baseIndex*3]=1;
            z[slackBaseIndex+baseIndex*3]=0;

            Z[baseIndex*3+1][pixel2]=-1;
            Z[baseIndex*3+1][auxVarIndex]=1;
            Z[baseIndex*3+1][slackBaseIndex+baseIndex*3+1]=1;
            z[slackBaseIndex+baseIndex*3+1]=0;

            Z[baseIndex*3+2][pixel1]=-1;
            Z[baseIndex*3+2][pixel2]=-1;
            Z[baseIndex*3+2][auxVarIndex]=1;
            Z[baseIndex*3+2][slackBaseIndex+baseIndex*3+2]=-1;
            z[slackBaseIndex+baseIndex*3+2]=-1;

            if(baseIndex*3+2>=numSlack) throw std::runtime_error("");
            if(slackBaseIndex+baseIndex*3+2>=numVars) throw std::runtime_error("");
            if(auxVarIndex>=numVars) throw std::runtime_error("");
            if(pixel1>=numVars) throw std::runtime_error("");
            if(pixel2>=numVars) throw std::runtime_error("");
        }
    }

    ConstraintMatrix C;
    std::vector<double> c;
    {
        initConstraintMatrix(C,grid.edgeMap.size()/2,numVars);
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



    std::string lpOutputFilePath = resolveLPOutputFilePath(outputPath,pgmInputImage);
    LPWriter::writeQP(lpOutputFilePath,prm,grid,mergedTerm.unaryMap,mergedTerm.binaryMap,partialL,linearization,relLevel);

    saveObjects(outputPath,ds,grid);


    std::string pythonFilePath = outputPath + "/model.py";
    std::ofstream ofs(pythonFilePath);

    ofs << "numPixels = " << grid.pixelMap.size() - 3 << "\n";
    ofs << "numEdges = " << grid.edgeMap.size() << "\n";
    ofs << "numPixelPairs = " << numPixelPairs << "\n";
    ofs << "numSlackVars = " << numSlack << "\n";
    ofs << "numVars = " << numVars  << "\n";

    exportPython(ofs,U,P1,P2,Z,z,C,c);

    ofs.close();

    return 0;
}