#ifndef LPMODEL_DISPUTILS_H
#define LPMODEL_DISPUTILS_H

#include <DGtal/io/boards/Board2D.h>
#include <DGtal/io/writers/GenericWriter.h>

#include <LPModel/initialization/model/Parameters.h>
#include <SCaBOliC/Core/display.h>

namespace LPModel
{
    namespace Utils
    {
        typedef Initialization::Parameters Parameters;
        typedef DGtal::Z2i::DigitalSet DigitalSet;

        void exportODRModel(const Parameters &prm, const std::string& outputPath);

        void exportImageFromDigitalSet(const DigitalSet& ds,
                                       const std::string& outputFilepath);

        DigitalSet loadImageAsDigitalSet(const std::string& imageFilePath);
    }
}

#endif //LPMODEL_DISPUTILS_H
