#include "LPModel/opt-model/utils.h"

namespace LPModel
{
    namespace OptModel
    {
        void displayODR(const OptModel& ODR,std::string outputPath)
        {
            DGtal::Board2D board;
            board << DGtal::SetMode(ODR.original.className(),"Paving");

            std::string specificStyle = ODR.original.className() + "/Paving";

            board << DGtal::SetMode(ODR.original.className(),"Paving");

            board << DGtal::CustomStyle(specificStyle, new DGtal::CustomColors(DGtal::Color::Blue, DGtal::Color::Blue));
            board << ODR.trustFRG;

            board << DGtal::CustomStyle(specificStyle, new DGtal::CustomColors(DGtal::Color::Green, DGtal::Color::Green));
            board << ODR.optRegion;

            board << DGtal::CustomStyle(specificStyle, new DGtal::CustomColors(DGtal::Color::Red, DGtal::Color::Red));
            board << ODR.appRegion;

            board.saveEPS( outputPath.c_str() );
        }
    }
}
