#include "LPModel/solassign/model/SolutionAssignment.h"

using namespace LPModel;

void SolutionAssignment::reversePixelMap(ReversePixelMap &rpm,
                                         const Grid &grid)
{
    typedef std::pair<Index,Pixel> Element;
    for(auto it=grid.pixelMap.begin();it!=grid.pixelMap.end();++it)
    {
        rpm.insert( Element( it->second.varIndex,it->second ) );
    }
}

SolutionAssignment::DigitalSet SolutionAssignment::readSolution(const std::string &solutionFile,
                                                                const Parameters &parameters,
                                                                const Grid &grid)
{
    ReversePixelMap rpm;
    reversePixelMap(rpm,grid);

    DigitalSet dsOutput(parameters.odrModel.domain);
    dsOutput.insert(parameters.odrModel.trustFRG.begin(),parameters.odrModel.trustFRG.end());


    std::ifstream ifs(solutionFile);

    Index numVars,varIndex;
    Point pixelCoords;
    double value;

    ifs >> numVars;
    std::vector<int> varValue;
    std::unordered_map<Point, unsigned int> pointToVar;
    for(Index i=0;i<numVars;++i)
    {
        ifs >> varIndex;
        ifs >> value;


        if(rpm.find(varIndex)==rpm.end()) continue; //Not pixel variable;

        if(value>=0.5)
            varValue.push_back(1);
        else
            varValue.push_back(0);

        pixelCoords = Point(rpm.at(varIndex).col, rpm.at(varIndex).row);
        pointToVar[pixelCoords]=varValue.size()-1;
    }

    DigitalSet backTransformed(parameters.odrModel.domain);

    parameters.handle.solutionSet(backTransformed,dsOutput,parameters.odrModel,varValue.data(),pointToVar);
    return backTransformed;
}