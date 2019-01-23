#ifndef LPMODEL_SOLUTIONASSIGNEMENT_H
#define LPMODEL_SOLUTIONASSIGNEMENT_H

#include <fstream>
#include <string>

#include <LPModel/initialization/model/Grid.h>
#include <LPModel/initialization/model/Parameters.h>

#include "LPModel/terms/sqc/control/CConstants.h"
#include "LPModel/terms/sqc/model/Constants.h"


namespace LPModel
{
    namespace SolutionAssignment
    {
        typedef Initialization::Parameters Parameters;
        typedef Initialization::Grid Grid;
        typedef Initialization::Pixel Pixel;
        typedef Initialization::Edge Edge;

        typedef DGtal::Z2i::DigitalSet DigitalSet;
        typedef DGtal::Z2i::Point Point;

        typedef unsigned long Index;
        typedef std::map<Index,Pixel> ReversePixelMap;

        typedef std::map<Index,Edge> ReverseEdgeMap;

        double skipCommentLines(std::ifstream& ifs,
                                const Terms::SquaredCurvature::Constants& sqc);

        void reversePixelMap(ReversePixelMap& rpm,
                             const Grid& grid);

        void reverseEdgeMap(ReverseEdgeMap &rpm,
                            const Grid &grid);

        DigitalSet readSolution(const std::string& solutionFile,
                          const Parameters& parameters,
                          const Grid& grid);
    }
}

#endif //LPMODEL_SOLUTIONASSIGNEMENT_H
