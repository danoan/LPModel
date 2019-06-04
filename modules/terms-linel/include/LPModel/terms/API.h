#ifndef LPMODEL_TERMSLINEL_API_H
#define LPMODEL_TERMSLINEL_API_H

#include <LPModel/terms/model/Term.h>

namespace LPModel
{
    namespace Terms
    {
        namespace API
        {
            Term merge(Term& term1, const Term& term2);
            void mergeMap(Term::GeneralMap& mergedMap,
                          const Term::GeneralMap& inputMap1,
                          const Term::GeneralMap& inputMap2);
        }
    }
}

#endif //LPMODEL_TERMS_API_H
