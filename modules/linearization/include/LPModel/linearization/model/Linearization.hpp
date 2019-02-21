#include "LPModel/linearization/model/Linearization.h"

using namespace LPModel;

template<typename TIndex, typename TValue>
void Linearization<TIndex,TValue>::linearize(const Input &input)
{
    for(auto it=input.begin();it!=input.end();++it,++nextIndex)
    {
        uniqueIndexMap.insert( UniqueElement(it->first,nextIndex));
        reverseUniqueIndexMap.insert( ReverseElement(nextIndex,it->first) );

        linearizationMap.insert( LinearizationElement(nextIndex,it->second) );
    }
}


template<typename TIndex, typename TValue>
typename Linearization<TIndex,TValue>::Input Linearization<TIndex,TValue>::partialLinearization(const Input &input)
{
    //I am expecting a TernaryMap
    assert(input.begin()->first.size()==3);

    Input bm;
    for(auto it=input.begin();it!=input.end();++it)
    {
        const UIntMultiIndex& currMI = it->first;

        auto itMI = currMI.begin();
        UIntMultiIndex newMI;
        for(int i=0;i<2;++i,++itMI)
        {
            newMI << *itMI;
        }
        LinearIndex edgeIndex = *itMI;  //Among pixels and edges, edges has the highest index
        ++itMI;

        if(uniqueIndexMap.find(newMI)==uniqueIndexMap.end())
        {
            uniqueIndexMap.insert( UniqueElement(newMI,nextIndex));
            reverseUniqueIndexMap.insert( ReverseElement(nextIndex,newMI) );

            linearizationMap.insert( LinearizationElement(nextIndex,it->second) );
            ++nextIndex;
        }

        UIntMultiIndex bmIndex;
        bmIndex << uniqueIndexMap[newMI] << edgeIndex;

        if(bm.find(bmIndex)==bm.end()) bm[bmIndex] = it->second;
        else throw std::runtime_error("Not expected state!");
    }
    return bm;
}

