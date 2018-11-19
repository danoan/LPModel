#ifndef LPMODEL_UTILS_H
#define LPMODEL_UTILS_H

#include <LPModel/utils/model/BinaryIndex.h>
#include <LPModel/utils/model/TernaryIndex.h>

namespace LPModel
{
    namespace Utils
    {
        BinaryIndex createBinaryIndex();
        TernaryIndex createTernaryIndex();
    }    
    
}

#endif //LPMODEL_UTILS_H
