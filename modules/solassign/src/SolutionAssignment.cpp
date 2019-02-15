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

void SolutionAssignment::reverseEdgeMap(ReverseEdgeMap &rpm,
                                         const Grid &grid)
{
    typedef std::pair<Index,Edge> Element;
    for(auto it=grid.edgeMap.begin();it!=grid.edgeMap.end();++it)
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

SolutionAssignment::SolutionPairVector SolutionAssignment::solutionPairVector(const std::string &solutionFile,
                                                                              const Parameters &parameters,
                                                                              const Grid& grid)
{
    Terms::SquaredCurvature::Constants sqc = Terms::SquaredCurvature::CConstants::setConstants(parameters);

    std::vector<SolutionPair> spVector;

    std::ifstream ifs(solutionFile);

    Index varIndex;
    double value;
    char xPrefix;

    double objectiveValue = skipCommentLines(ifs,sqc);

    while(xPrefix!='x') ifs >> xPrefix;
    while(!ifs.eof())
    {
        ifs >> varIndex;
        ifs >> value;

        spVector.push_back( SolutionPair(varIndex,value));

        xPrefix=' ';
        while(xPrefix!='x' && !ifs.eof()) ifs >> xPrefix;
    }

    std::sort(spVector.begin(),spVector.end(),[](const SolutionPair& e1, const SolutionPair& e2){ return e1.first < e2.first;});
    return spVector;
}

SolutionAssignment::SolutionVector SolutionAssignment::solutionVector(const SolutionPairVector& spv,
                                                                      const Parameters& parameters,
                                                                      const Grid& grid)
{
    ReversePixelMap rpm;
    reversePixelMap(rpm,grid);

    std::vector<int> varValue;
    for(auto it=spv.begin();it!=spv.end();++it)
    {
        const Index& varIndex = it->first;
        const double& value = it->second;
        if(rpm.find(varIndex)!=rpm.end()) //continue; //Not pixel variable;
        {
            if (value >= 0.5) {
                varValue.push_back(1);
            } else {
                varValue.push_back(0);
            }
        }
    }

    return varValue;
}

SolutionAssignment::DigitalSet SolutionAssignment::readSolution(const std::string &solutionFile,
                                                                const Parameters &parameters,
                                                                const Grid &grid)
{
    SolutionPairVector spVector = solutionPairVector(solutionFile,parameters,grid);
    std::ofstream ofs(solutionFile+".sorted");
    for(auto it=spVector.begin();it!=spVector.end();++it)
    {
        ofs << it->first << "\t" << it->second << "\n";
    }
    ofs.flush();
    ofs.close();

    SolutionVector solVector = solutionVector(spVector,parameters,grid);

    ReversePixelMap rpm;
    reversePixelMap(rpm,grid);
    
    Point pixelCoords;
    std::unordered_map<Point, unsigned int> pointToVar;
    int curr=0;
    for(auto it=spVector.begin();it!=spVector.end();++it)
    {
        const Index& varIndex = it->first;
        const double& value = it->second;
        if(rpm.find(varIndex)!=rpm.end()) //continue; //Not pixel variable;
        {
            pixelCoords = Point(rpm.at(varIndex).col, rpm.at(varIndex).row);
            assert(parameters.odrModel.trustFRG(pixelCoords) == false);
            pointToVar[pixelCoords]=curr;            
            ++curr;
        }
    }
    
    DigitalSet dsOutput(parameters.odrModel.domain);
    dsOutput.insert(parameters.odrModel.trustFRG.begin(),parameters.odrModel.trustFRG.end());

    DigitalSet backTransformed(parameters.odrModel.domain);

    parameters.handle.solutionSet(backTransformed,dsOutput,parameters.odrModel,solVector.data(),pointToVar);
    return DIPaCUS::Transform::bottomLeftBoundingBoxAtOrigin(backTransformed);
}