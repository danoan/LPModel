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

double SolutionAssignment::skipCommentLines(std::ifstream& ifs,
                                            const Terms::SquaredCurvature::Constants& sqc)
{
    char buffer[256];
    char firstChar;
    double value=0;

    ifs.get(firstChar);
    while(firstChar=='#')
    {
        do
        {
            ifs.getline(buffer, 256, '\n');
            if( strncmp(buffer," Objective value =",18) == 0)
            {
                value = std::atof( (buffer+19) );
            }
        }while(ifs.gcount()==256);
        ifs.get(firstChar);
    };

    ifs.unget();
    return value*sqc.F + sqc.F*sqc.W;
    
}

SolutionAssignment::DigitalSet SolutionAssignment::readSolution(const std::string &solutionFile,
                                                                const Parameters &parameters,
                                                                const Grid &grid)
{
    Terms::SquaredCurvature::Constants sqc = Terms::SquaredCurvature::CConstants::setConstants(parameters);

    ReversePixelMap rpm;
    reversePixelMap(rpm,grid);

    DigitalSet dsOutput(parameters.odrModel.domain);
    dsOutput.insert(parameters.odrModel.trustFRG.begin(),parameters.odrModel.trustFRG.end());


    std::ifstream ifs(solutionFile);

    Index varIndex;
    Point pixelCoords;
    double value;
    char xPrefix;
    
    double objectiveValue = skipCommentLines(ifs,sqc);

    std::vector<int> varValue;
    std::unordered_map<Point, unsigned int> pointToVar;
    while(!ifs.eof())
    {
        ifs >> xPrefix;
        ifs >> varIndex;
        ifs >> value;


        if(rpm.find(varIndex)==rpm.end()) continue; //Not pixel variable;

        if(value>=0.5)
        {
            varValue.push_back(1);
        }
        else
        {
            varValue.push_back(0);
        }


        pixelCoords = Point(rpm.at(varIndex).col, rpm.at(varIndex).row);
        assert( parameters.odrModel.trustFRG( pixelCoords )==false );


        pointToVar[pixelCoords]=varValue.size()-1;
    }

    DigitalSet backTransformed(parameters.odrModel.domain);

    parameters.handle.solutionSet(backTransformed,dsOutput,parameters.odrModel,varValue.data(),pointToVar);
    return DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(backTransformed);
}