#include <vector>

#include "DGtal/helpers/StdDefs.h"

#include <LPModel/initialization/API.h>
#include <DGtal/io/writers/GenericWriter.h>
#include <LPModel/terms/sqc/CSqc.h>
#include <DGtal/io/boards/Board2D.h>
#include "LPModel/solassign/model/SolutionAssignment.h"
#include "LPModel/terms/sqc/control/CConstants.h"
#include "LPModel/terms/sqc/model/Constants.h"

#include "LPModel/nonlinopt/utils/utils.h"

using namespace LPModel;
using namespace DGtal::Z2i;
using namespace NonLinOpt;

typedef DIPaCUS::Representation::Image2D Image2D;

ActiveSetSolver::Vector convertToVector(const std::vector<int>& sv)
{
    ActiveSetSolver::Vector v(sv.size(),1);
    for(int i=0;i<sv.size();++i)
    {
        v.coeffRef(i) = sv[i];
    }

    return v;
}

int main(int argc, char* argv[])
{
    if(argc<5)
    {
        std::cerr << "Usage: initialShapeImage solutionImage optWidth gridObjectFile outputFilepath\n";
        exit(1);
    }

    std::string initialImage = argv[1];
    std::string solutionImage = argv[2];
    int optWidth = std::atoi(argv[3]);
    std::string gridObjectFile = argv[4];
    std::string outputFilepath = argv[5];


    std::cerr << "Reading Parameters Object\n";
    Initialization::Parameters prm( Initialization::API::readParametersFromFile(initialImage,optWidth) );

    std::cerr << "Generating Grid Object\n";
    Initialization::Grid* grid = Initialization::API::readGridFromFile(gridObjectFile);
    LPModel::Terms::Term scTerm = LPModel::Terms::SquaredCurvature::API::prepare(prm,*grid,1.0);

    Point ballBorder;
    {
        Point ptemp(5,5);
        Point ptempImgCoords(2,2);
        Point imgCoordsTranslated = prm.odrModel.toImageCoordinates(ptemp);
        Point translation = ptempImgCoords - imgCoordsTranslated;
        Point lb,ub;
        prm.odrModel.original.computeBoundingBox(lb,ub);

        ballBorder = translation+lb;
    }


    Image2D img = DGtal::GenericReader<Image2D>::import(solutionImage);
    DigitalSet _ds (img.domain());
    DIPaCUS::Representation::imageAsDigitalSet(_ds,img);
    DigitalSet ds = DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(_ds,ballBorder);

    Curve curve;
    DIPaCUS::Misc::computeBoundaryCurve(curve,ds);

    KSpace kspace;
    kspace.init(ds.domain().lowerBound(),ds.domain().upperBound(),true);

    std::vector<int> solution(grid->variableMap.size());
    for(int i=0;i<solution.size();++i) solution[i]=0;

    DigitalSet myDs(ds.domain());

    SCell pixelModel = kspace.sCell(Point(1,1));
    for(auto pix : ds)
    {
        SCell pixSCell = kspace.sCell(pix,pixelModel);
        Point p = kspace.sKCoords( pixSCell );



        if(grid->pixelMap.find(p)!=grid->pixelMap.end())
        {
            if(grid->pixelMap.at(p).ct!=LPModel::Initialization::Pixel::CellType::Variable) continue;
            solution[ grid->pixelMap.at(p).varIndex] = 1.0;
            myDs.insert(pix);
        }
    }

    std::map<Initialization::Linel,std::vector<Initialization::Edge>> linelToEdges;
    for(auto e: grid->edgeMap)
    {
        const Initialization::Linel& linel = e.second.l1;
        if(linelToEdges.find(linel)==linelToEdges.end()) linelToEdges[linel]=std::vector<Initialization::Edge>();

        linelToEdges[linel].push_back(e.second);
    }

    for(auto l:curve)
    {
        Point p = kspace.sKCoords(l);
        if(grid->linelMap.find(p)!=grid->linelMap.end())
        {
            Initialization::Linel linel = grid->linelMap.at(p);
            auto edges = linelToEdges[linel];

            solution[linelToEdges[linel][0].varIndex] = 1.0;
        }
    }

    boost::filesystem::create_directories( boost::filesystem::path(outputFilepath).remove_filename() );
    std::ofstream ofs(outputFilepath);

    ActiveSetSolver solverAS(*grid,scTerm);

    ofs << "Objective value: " << solverAS.objective<adouble>(convertToVector(solution)).value()<< std::endl;
    ofs.flush();
    ofs.close();

    delete grid;

    return 0;

}