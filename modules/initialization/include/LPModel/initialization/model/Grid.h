#ifndef LPMODEL_INITIALIZATION_GRID_H
#define LPMODEL_INITIALIZATION_GRID_H

#include <istream>
#include <ostream>

#include <LPModel/initialization/control/CPixel.h>
#include <LPModel/initialization/control/CLinel.h>
#include <LPModel/initialization/control/CEdge.h>

#include "Pixel.h"
#include "Edge.h"
#include "Linel.h"

namespace LPModel
{
    namespace Initialization
    {
        struct Grid
        {
            typedef DGtal::Z2i::DigitalSet DigitalSet;
            typedef DGtal::Z2i::Point Point;

            typedef Pixel::PixelMap PixelMap;
            typedef Edge::EdgeMap EdgeMap;
            typedef Linel::LinelMap LinelMap;

            typedef std::pair<Point,Pixel> PixelMapElement;
            typedef std::pair<Point,Linel> LinelMapElement;
            typedef std::pair<int,Edge> EdgeMapElement;

            typedef std::map<unsigned long, const IVariable*> VariableMap;

            Grid():pixelMap(_pixelMap),
                   edgeMap(_edgeMap),
                   linelMap(_linelMap),
                   variableMap(_variableMap){}

            Grid(const DigitalSet& ds,
                 const DigitalSet& trustFrg):pixelMap(_pixelMap),
                                             edgeMap(_edgeMap),
                                             linelMap(_linelMap),
                                             variableMap(_variableMap)
            {
                CPixel::createPixelMap(_pixelMap,ds);
                CLinel::createLinelSet(_linelMap,
                                       trustFrg,
                                       ds,
                                       _pixelMap);
                CEdge::createEdgeMap(_edgeMap,_pixelMap,_linelMap);

                addInVariableMap(_pixelMap);
                addInVariableMap(_edgeMap);
            }

            template<class TIVariableMap>
            void addInVariableMap(const TIVariableMap& IVarMap)
            {
                for(auto it=IVarMap.begin();it!=IVarMap.end();++it)
                {
                    const IVariable* ivar = &(it->second);
                    _variableMap[ivar->varIndex] = ivar;
                }
            }

            inline void addInPixelMap(const PixelMapElement& pme){_pixelMap.insert(pme);}
            inline void addInEdgeMap(const EdgeMapElement& eme){_edgeMap.insert(eme);}
            inline void addInLinelMap(const LinelMapElement& lme){_linelMap.insert(lme);}

            friend std::ifstream& operator>>(std::ifstream& is, Grid** grid);
            friend std::ofstream& operator<<(std::ofstream& os, const Grid& grid);

            const PixelMap& pixelMap;
            const EdgeMap& edgeMap;
            const LinelMap& linelMap;

            const VariableMap& variableMap;

        private:
            PixelMap _pixelMap;
            EdgeMap _edgeMap;
            LinelMap _linelMap;

            VariableMap _variableMap;
        };

    }
}

#endif //LPMODEL_INITIALIZATION_GRID_H
