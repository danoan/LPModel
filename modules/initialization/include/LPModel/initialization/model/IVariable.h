#ifndef LPMODEL_IVARIABLE_H
#define LPMODEL_IVARIABLE_H

namespace LPModel
{
    namespace Initialization
    {
        struct IVariable
        {
            enum VariableType{Pixel,Edge};

            IVariable(VariableType vt,
                      const unsigned long varIndex):vt(vt),
                                                    varIndex(varIndex)

            {}


            const unsigned long varIndex;
            VariableType vt;
        };
    }
}

#endif //LPMODEL_IVARIABLE_H
