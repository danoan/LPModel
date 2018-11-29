#include "LPModel/utils/dispUtils.h"

using namespace LPModel;

void Utils::exportODRModel(const Parameters &prm, const std::string& outputPath)
{
    SCaBOliC::Core::Display::DisplayODR(prm.odrModel,outputPath);
}