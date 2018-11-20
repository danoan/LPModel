#ifndef LPMODEL_ITERATORMANAGER_H
#define LPMODEL_ITERATORMANAGER_H

#include <DIPaCUS/derivates/Misc.h>

#include <LPModel/initialization/model/Parameters.h>
#include <LPModel/initialization/model/Grid.h>

namespace LPModel
{
    namespace Terms
    {
        namespace SquaredCurvature
        {
            namespace Internal
            {
                namespace Linel
                {
                    namespace IteratorManager
                    {
                        typedef Initialization::Parameters Parameters;
                        typedef Initialization::Grid Grid;

                        typedef DGtal::Z2i::Point Point;
                        typedef DGtal::Z2i::KSpace KSpace;

                        typedef std::function<void(const Point& linel, const Point&,const Point&)> BinaryCallback;
                        typedef std::function<void(const Point& linel, const Point&,const Point&,const Point&)> TernaryCallback;

                        void run(const Parameters& prm,
                                 const Grid& grid,
                                 const BinaryCallback& bcbk,
                                 const TernaryCallback& tcbk);
                    }
                }
            }
        }
    }
}

#endif //LPMODEL_ITERATORMANAGER_H
