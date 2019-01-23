#ifndef LPMODEL_OBJECTIVE_H
#define LPMODEL_OBJECTIVE_H

#include <fstream>
#include <LPModel/terms/model/MultiIndex.h>
#include <map>
#include <LPModel/initialization/model/Grid.h>
#include "LPModel/terms/model/Term.h"

namespace LPModel
{
    namespace Objective
    {
        typedef Initialization::Grid Grid;
        typedef unsigned long Index;

        typedef std::map<Index,double> VariableMap;
        typedef VariableMap::const_iterator VariableMapIterator;


        typedef Utils::MultiIndex<Index> MyMultiIndex;
        typedef std::map<MyMultiIndex,Index> MultiToUniqueMap;
        typedef MultiToUniqueMap::const_iterator MultiToUniqueMapIterator;


        void writeObjective(std::ofstream& ofs,
                            Terms::Term::GeneralMap::const_iterator begin,
                            Terms::Term::GeneralMap::const_iterator end);

        void writeObjective(std::ofstream& ofs,
                            VariableMapIterator begin,
                            VariableMapIterator end);


        void writeLinearizationConstraints(std::ofstream& ofs,
                                           int& startConstraintNum,
                                           MultiToUniqueMapIterator begin,
                                           MultiToUniqueMapIterator end);

        void writeLinearizationConstraint(std::ofstream& ofs,
                                          int& startConstraintNum,
                                          const MyMultiIndex& multiIndex,
                                          const Index& uniqueIndex);

        void writeBounds(std::ofstream& ofs,
                         VariableMapIterator begin,
                         VariableMapIterator end);

        void writeBounds(std::ofstream &ofs,
                         Grid::EdgeMap::const_iterator begin,
                         Grid::EdgeMap::const_iterator end);

        void writeBounds(std::ofstream &ofs,
                         Grid::PixelMap::const_iterator begin,
                         Grid::PixelMap::const_iterator end);

        void writeBinaries(std::ofstream& ofs,
                           const Grid& grid);
    }
}

#endif //LPMODEL_OBJECTIVE_H
