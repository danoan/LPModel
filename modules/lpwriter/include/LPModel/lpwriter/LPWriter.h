#ifndef LPMODEL_LPWRITER_H
#define LPMODEL_LPWRITER_H

#include <fstream>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include "LPModel/initialization/model/Parameters.h"
#include "LPModel/linearization/model/Linearization.h"
#include "LPModel/constraints/ClosedAndConnected.h"
#include "LPModel/terms/model/Term.h"
#include "LPModel/types/types.h"

namespace LPModel
{
    namespace LPWriter
    {
        typedef Initialization::Grid Grid;
        typedef unsigned long Index;

        typedef std::map<Index,double> CoefficientMap;
        typedef CoefficientMap::const_iterator CoefficientMapIterator;

        typedef std::map<Index,double> FixedLabels;

        typedef Utils::MultiIndex<Index> MyMultiIndex;
        typedef std::map<MyMultiIndex,Index> MultiToUniqueMap;
        typedef MultiToUniqueMap::const_iterator MultiToUniqueMapIterator;

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

        void writePenalties(std::ofstream& ofs,const LinelConstraints& lc);

        void writeObjective(std::ofstream& ofs,
                            Terms::Term::GeneralMap::const_iterator begin,
                            Terms::Term::GeneralMap::const_iterator end);

        void writeObjective(std::ofstream& ofs,
                            CoefficientMapIterator begin,
                            CoefficientMapIterator end);

        void writePixel(StringConstraint& sc,
                        const PixelIncidence& pi);

        void writeEdge(StringConstraint& sc,
                       const EdgeIncidence &ei);

        void writePixelPenalty(StringConstraint& sc,
                               const PixelIncidence& pi);

        void writeEdgePenalty(StringConstraint& sc,
                              const EdgeIncidence &ei);

        void writeConstraint(std::ofstream& ofs,
                             int& cIndexStart,
                             const LinelConstraints& lc);

        void writeFixedLabels(std::ofstream &ofs,
                              int& cIndexStart,
                              const FixedLabels& fixedLabels);

        void writeLinearizationConstraint(std::ofstream &ofs,
                                          int& startConstraintNum,
                                          const MyMultiIndex &multiIndex,
                                          const Index& uniqueIndex);

        void writeLinearizationConstraints(std::ofstream &ofs,
                                           int& startConstraintNum,
                                           MultiToUniqueMapIterator begin,
                                           MultiToUniqueMapIterator end);

        void writeBounds(std::ofstream &ofs,
                         CoefficientMapIterator begin,
                         CoefficientMapIterator end);

        void writeBounds(std::ofstream &ofs,
                         Grid::PixelMap::const_iterator begin,
                         Grid::PixelMap::const_iterator end);

        void writeBounds(std::ofstream &ofs,
                         Grid::EdgeMap::const_iterator begin,
                         Grid::EdgeMap::const_iterator end);

        void writeBinaries(std::ofstream& ofs,
                           const Grid& grid);

        void writeBinaries(std::ofstream& ofs,
                           CoefficientMapIterator begin,
                           CoefficientMapIterator end);

        void writeLP(const std::string& outputFilePath,
                     const Initialization::Parameters& prm,
                     const Initialization::Grid& grid,
                     const Terms::Term::UnaryMap& um,
                     const MyLinearization& linearization,
                     const int relLevel,
                     const FixedLabels& fixedLabels=FixedLabels());


        void writeQP(const std::string& outputFilePath,
                     const Initialization::Parameters& prm,
                     const Initialization::Grid& grid,
                     const Terms::Term::UnaryMap& um,
                     const Terms::Term::BinaryMap& partialL,
                     const MyLinearization& linearization,
                     const int relLevel,
                     const FixedLabels& fixedLabels=FixedLabels());
    }
}

#endif //LPMODEL_LPWRITER_H
