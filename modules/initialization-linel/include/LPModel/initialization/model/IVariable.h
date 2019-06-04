#ifndef LPMODEL_IVARIABLELINEL_H
#define LPMODEL_IVARIABLELINEL_H

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
