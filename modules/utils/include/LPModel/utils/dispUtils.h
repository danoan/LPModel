#ifndef LPMODEL_DISPUTILS_H
#define LPMODEL_DISPUTILS_H

#include <DGtal/io/boards/Board2D.h>
#include <LPModel/initialization/model/Parameters.h>
#include <SCaBOliC/Core/display.h>

namespace LPModel
{
    namespace Utils
    {
        typedef Initialization::Parameters Parameters;

        void exportODRModel(const Parameters &prm, const std::string& outputPath);
    }
}

#endif //LPMODEL_DISPUTILS_H
