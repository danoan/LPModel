#ifndef LPMODEL_OBJECTIVE_H
#define LPMODEL_OBJECTIVE_H

#include <fstream>
#include <LPModel/utils/model/MultiIndex.h>
#include <map>
#include <LPModel/initialization/model/Grid.h>

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
                         const Grid& grid,
                         VariableMapIterator begin,
                         VariableMapIterator end);
    }
}

#endif //LPMODEL_OBJECTIVE_H
