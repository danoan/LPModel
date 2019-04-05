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


/*
 * Given a ternary map, create RELAXATION_AUXILIAR variables for a second order pair.
 * Ex1: (Pixel,Pixel) -> Aux Z
 * Ex2: (Pixel,Linel) -> Aux Z
 */
template<typename TIndex, typename TValue>
typename Linearization<TIndex,TValue>::Input Linearization<TIndex,TValue>::partialLinearization(const Input &input,
                                                                                                const Grid& grid,
                                                                                                const PartialLinearizationPair& plp)
{
    //I am expecting a TernaryMap
    assert(input.begin()->first.size()==3);

    int variableCount;

    Input bm;
    for(auto it=input.begin();it!=input.end();++it)
    {
        variableCount=0;

        LinearIndex otherIndex;
        UIntMultiIndex newMI;
        const UIntMultiIndex& currMI = it->first;
        auto itMI = currMI.begin();
        do
        {
            if( grid.variableMap.at(*itMI)->vt==plp.variableTypes[variableCount] )
            {
                newMI << *itMI;
                ++variableCount;
            }
            else
            {
                otherIndex = *itMI;
            }

            ++itMI;

        }while(variableCount<2 && itMI!=currMI.end());

        if(variableCount!=2) throw std::runtime_error("Variable pair not found!");

        if(uniqueIndexMap.find(newMI)==uniqueIndexMap.end())
        {
            uniqueIndexMap.insert( UniqueElement(newMI,nextIndex));
            reverseUniqueIndexMap.insert( ReverseElement(nextIndex,newMI) );

            linearizationMap.insert( LinearizationElement(nextIndex,it->second) );
            ++nextIndex;
        }

        UIntMultiIndex bmIndex;
        bmIndex << uniqueIndexMap[newMI] << otherIndex;

        if(bm.find(bmIndex)==bm.end()) bm[bmIndex] = it->second;
        else throw std::runtime_error("Not expected state!");
    }
    return bm;
}


template<typename TIndex, typename TValue>
void Linearization<TIndex,TValue>::coupledLinearization(const Input &input)
{
    //I am expecting a TernaryMap
    assert(input.begin()->first.size()==3);

    Input sndOrderLinearization;
    for(auto it=input.begin();it!=input.end();++it)
    {
        const Utils::MultiIndex<unsigned long> &mIndex = it->first;
        auto mit = mIndex.begin();
        unsigned long pixel1 = *mit;++mit;
        unsigned long pixel2 = *mit;++mit;
        unsigned long edge = *mit;

        TIndex pixelLinelMIndex;
        pixelLinelMIndex << pixel1 << edge;
        unsigned long auxIndex = this->uniqueIndexMap[pixelLinelMIndex];

        TIndex sndOrderMIndex;
        sndOrderMIndex << auxIndex << pixel2;

        sndOrderLinearization.insert( UniqueElement( sndOrderMIndex, it->second) );
    }

    this->linearize(sndOrderLinearization);
};