#ifndef LPMODEL_LINEARLIZATION_H
#define LPMODEL_LINEARLIZATION_H

#include <map>

namespace LPModel
{
    template<typename TIndex, typename TValue>
    class Linearization
    {
    public:
        typedef std::map<TIndex,TValue> Input;
        typedef unsigned long LinearIndex;

        typedef std::pair<TIndex,LinearIndex> UniqueElement;
        typedef std::map<TIndex,LinearIndex> UniqueIndexMap;

        typedef std::pair<LinearIndex,TIndex> ReverseElement;
        typedef std::map<LinearIndex,TIndex> ReverseUniqueIndexMap;

        typedef std::pair<LinearIndex,TValue> LinearizationElement;
        typedef std::map<LinearIndex,TValue> LinearizationMap;

        typedef typename LinearizationMap::const_iterator LinearizationMapIterator;
        typedef typename UniqueIndexMap::const_iterator UniqueIndexMapIterator;


    public:
        Linearization(LinearIndex nextIndex):nextIndex(nextIndex){}

        void linearize(const Input& input);

        LinearizationMapIterator begin() const{ return linearizationMap.begin(); }
        LinearizationMapIterator end() const{ return linearizationMap.end(); }

        UniqueIndexMapIterator ubegin() const{ return uniqueIndexMap.begin(); }
        UniqueIndexMapIterator uend() const{ return uniqueIndexMap.end(); }

    private:
        LinearIndex nextIndex;

        UniqueIndexMap uniqueIndexMap;
        ReverseUniqueIndexMap reverseUniqueIndexMap;
        LinearizationMap linearizationMap;
    };
}

#include "Linearization.hpp"

#endif //LPMODEL_LINEARLIZATION_H
