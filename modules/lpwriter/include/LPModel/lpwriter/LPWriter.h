#ifndef LPMODEL_LPWRITER_H
#define LPMODEL_LPWRITER_H

#include <fstream>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include "LPModel/initialization/model/Parameters.h"
#include "LPModel/linearization/model/Linearization.h"
#include "LPModel/constraints/ClosedAndConnected.h"
#include "LPModel/objective/model/Objective.h"
#include "LPModel/terms/model/Term.h"

namespace LPModel
{
    namespace LPWriter
    {
        typedef Constraints::ClosedAndConnected::LinelConstraints LinelConstraints;


        typedef Constraints::ClosedAndConnected::Linel Linel;
        typedef Constraints::ClosedAndConnected::LinelIncidence LinelIncidence;

        typedef Constraints::ClosedAndConnected::Pixel Pixel;
        typedef Constraints::ClosedAndConnected::PixelIncidence PixelIncidence;
        typedef Constraints::ClosedAndConnected::EdgeIncidence EdgeIncidence;

        typedef Linearization< Terms::Term::UIntMultiIndex,double > MyLinearization;

        struct StringConstraint
        {
            std::stack<std::string> lhs;
            std::stack<double> rhs;
        };

        void writePixel(StringConstraint& sc,
                        const PixelIncidence& pi);

        void writeEdge(StringConstraint& sc,
                       const EdgeIncidence &ei);

        void writeConstraint(std::ofstream& ofs,
                             int& cIndexStart,
                             const LinelConstraints& lc);

        void writeLP(const std::string& outputFilePath,
                     const Initialization::Parameters& prm,
                     const Initialization::Grid& grid,
                     const Terms::Term::UnaryMap& um,
                     const MyLinearization& linearization);
    }
}

#endif //LPMODEL_LPWRITER_H
