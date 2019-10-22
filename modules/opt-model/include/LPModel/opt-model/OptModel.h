#ifndef LPMODEL_OPTMODEL_H
#define LPMODEL_OPTMODEL_H

#include <DGtal/helpers/StdDefs.h>

namespace LPModel
{
  namespace OptModel
  {
      struct OptModel
      {
          typedef DGtal::Z2i::DigitalSet DigitalSet;

          OptModel(const DigitalSet& original, const DigitalSet &optRegion, const DigitalSet &trustFrg, const DigitalSet &trustBkg, const DigitalSet& appRegion)
                  : original(original),optRegion(optRegion), trustFRG(trustFrg), trustBKG(trustBkg),appRegion(appRegion) {}


          DigitalSet original;
          DigitalSet optRegion;
          DigitalSet trustFRG;
          DigitalSet trustBKG;
          DigitalSet appRegion;
      };
  }
}



#endif //LPMODEL_OPTMODEL_H
