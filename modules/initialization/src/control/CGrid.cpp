#include "LPModel/initialization/model/Grid.h"

namespace LPModel{ namespace Initialization{

    std::ofstream& operator<<(std::ofstream& os, const Grid& grid)
    {
        unsigned long pixelMapSize = grid.pixelMap.size();
        os.write( (char*) &pixelMapSize, sizeof(grid.pixelMap.size()));
        std::for_each(grid.pixelMap.begin(),grid.pixelMap.end(),[&os](const Grid::PixelMapElement& pme){os << pme.second;});

        unsigned long linelMapSize = grid.linelMap.size();
        os.write( (char*) &linelMapSize, sizeof(grid.linelMap.size()));
        std::for_each(grid.linelMap.begin(),grid.linelMap.end(),[&os](const Grid::LinelMapElement& lme){os << lme.second;});

        unsigned long edgeMapSize = grid.edgeMap.size();
        os.write( (char*) &edgeMapSize, sizeof(grid.edgeMap.size()));
        std::for_each(grid.edgeMap.begin(),grid.edgeMap.end(),[&os](const Grid::EdgeMapElement& eme){os << eme.second;});

        return os;
    }

    std::ifstream& operator>>(std::ifstream &is, Grid** grid)
    {
        Grid::PixelMap pm;
        Grid::LinelMap lm;
        Grid::EdgeMap em;

        unsigned long pixelMapSize;
        is.read( (char*) &pixelMapSize, sizeof(unsigned long) );
        for(int i=0;i<pixelMapSize;++i)
        {
            Pixel pixel = CPixel::readPixel(is);
            Grid::Point coord(pixel.col,pixel.row);
            pm.insert(Grid::PixelMapElement(coord,pixel));
        }

        CLinel::PixelIndexMap pim;
        for(auto it=pm.begin();it!=pm.end();++it)
        {
            pim.insert( std::pair<unsigned long,Pixel>(it->second.varIndex,it->second) );
        }
        pim.insert( std::pair<unsigned long,Pixel>(-1,Pixel(0,0,-1,Pixel::CellType::Auxiliar)));

        unsigned long linelMapSize;
        is.read( (char*) &linelMapSize, sizeof(unsigned long) );
        for(int i=0;i<linelMapSize;++i)
        {
            Linel linel = CLinel::readLinel(is,pim);
            Grid::Point coord(linel.x,linel.y);
            lm.insert(Grid::LinelMapElement(coord,linel));
        }


        CEdge::LinelIndexMap lim;
        for(auto it=lm.begin();it!=lm.end();++it)
        {
            lim.insert( std::pair<unsigned long,Linel>(it->second.linelIndex,it->second) );
        }

        unsigned long edgeMapSize;
        is.read( (char*) &edgeMapSize, sizeof(unsigned long) );
        for(int i=0;i<edgeMapSize;++i)
        {
            Edge edge = CEdge::readEdge(is,lim);
            em.insert(Grid::EdgeMapElement(edge.varIndex,edge));
        }


        *grid = new Grid(pm,em,lm);
        return is;
    }
}}