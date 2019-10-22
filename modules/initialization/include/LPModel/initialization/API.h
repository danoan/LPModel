#ifndef LPMODEL_INITIALIZATION_API_H
#define LPMODEL_INITIALIZATION_API_H

#include <DGtal/io/writers/GenericWriter.h>
#include <string>

#include <DIPaCUS/base/Representation.h>
#include "SCaBOliC/Core/model/ODRModel.h"
#include "SCaBOliC/Core/ODRPixels/ODRPixels.h"
#include "DIPaCUS/derivates/Misc.h"
#include "DIPaCUS/base/Representation.h"

#include "model/Grid.h"
#include "model/Parameters.h"
#include "control/CPixel.h"
#include "control/CEdge.h"
#include "control/CLinel.h"

namespace LPModel
{
    namespace Initialization
    {
        namespace API
        {
            typedef DGtal::Z2i::DigitalSet DigitalSet;
            typedef DGtal::Z2i::Domain Domain;
            typedef DigitalSet::Point Point;
            typedef Point KPoint;

            typedef DIPaCUS::Neighborhood::FourNeighborhoodPredicate FourNeighborhood;
            typedef SCaBOliC::Core::ODRPixels ODRPixels;
            typedef SCaBOliC::Core::ODRModel ODRModel;

            //For test only
            Parameters initParameters(const DigitalSet &originalDS, int optRegionWidth, bool fixedEvenIteration);
            Parameters readParametersFromFile(const std::string& inputFile, int optRegionWidth, bool fixedEvenIteration);


            Parameters initParameters(const DigitalSet &originalDS, int optRegionWidth);
            Grid createGrid(const DigitalSet& ds,
                            const Parameters& prm);

            Parameters readParametersFromFile(const std::string& inputFile, int optRegionWidth);
            void save(const DigitalSet& dsOriginal, const std::string& outputFile);
            
            Grid* readGridFromFile(const std::string& inputFile);
            void save(const Grid& grid, const std::string& outputFile);

            namespace Internal
            {
               
                int boundaryLinels(const DigitalSet& ds);

                DigitalSet extendedOptRegion(const ODRModel& odrModel);
                DigitalSet extendedAppRegion(const DigitalSet& appRegion, const DigitalSet& optRegion);

                bool consistentGrid(const Parameters& prm,
                                    const Grid& grid);

                bool isConnected(const DigitalSet& ds);
            }
        }
    }
}

#endif //LPMODEL_INITIALIZATION_API_H
