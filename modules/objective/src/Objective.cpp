#include "LPModel/objective/model/Objective.h"

using namespace LPModel;

void Objective::writeObjective(std::ofstream& ofs,
                               Terms::Term::GeneralMap::const_iterator begin,
                               Terms::Term::GeneralMap::const_iterator end)
{
    for(auto it=begin;it!=end;++it)
    {
        if(it->second>0)
            ofs << " + ";
        else if(it->second<0)
            ofs << " - ";
        else
            continue;

        ofs << fabs(it->second) << " " << "x" << *( it->first.begin() );
    }
}

void Objective::writeObjective(std::ofstream &ofs,
                               VariableMapIterator begin,
                               VariableMapIterator end)
{
    for(auto it=begin;it!=end;++it)
    {
        if(it->second>0)
            ofs << " + ";
        else if(it->second<0)
            ofs << " - ";
        else
            continue;

        ofs << fabs(it->second) << " " << "x" << it->first;
    }
}

void Objective::writeLinearizationConstraint(std::ofstream &ofs,
                                             int& startConstraintNum,
                                             const MyMultiIndex &multiIndex,
                                             const Index& uniqueIndex)
{
    for(auto it=multiIndex.begin();it!=multiIndex.end();++it)
    {
        ofs << "c" << startConstraintNum++ << ": " << "x" << uniqueIndex << " - x"  <<  *it << " <= 0\n";
    }

    ofs << "c" << startConstraintNum++ << ": " << "x" << uniqueIndex;
    for(auto it=multiIndex.begin();it!=multiIndex.end();++it)
    {
        ofs << " - x" << *it;
    }

    ofs << " >= - " << multiIndex.size()-1 << "\n";
}

void Objective::writeLinearizationConstraints(std::ofstream &ofs,
                                              int& startConstraintNum,
                                              MultiToUniqueMapIterator begin,
                                              MultiToUniqueMapIterator end)
{
    for(auto it=begin;it!=end;++it)
    {
        writeLinearizationConstraint(ofs,startConstraintNum,it->first,it->second);
    }
}

void Objective::writeBounds(std::ofstream &ofs,
                            const Grid& grid,
                            VariableMapIterator begin,
                            VariableMapIterator end)
{
    for(auto it=begin;it!=end;++it)
    {
        ofs << "0 <= x" << it->first << " <= 1\n";
    }

    for(auto it=grid.pixelMap.begin();it!=grid.pixelMap.end();++it)
    {
        if(it->second.ct==Initialization::Pixel::CellType::Auxiliar) continue;
        ofs << "0 <= x" << it->second.varIndex << " <= 1\n";
    }


    for(auto it=grid.edgeMap.begin();it!=grid.edgeMap.end();++it)
    {
        ofs << "0 <= x" << it->second.varIndex << " <= 1\n";
    }
}