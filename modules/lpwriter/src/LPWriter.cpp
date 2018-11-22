#include "LPModel/lpwriter/LPWriter.h"

using namespace LPModel;

void LPWriter::writePixel(std::ofstream &ofs,
                          const PixelIncidence &pi)
{
    if(pi.pixel.ct==Pixel::CellType::Auxiliar) return;
    char s = pi.posIncidence?' ':'-';
    ofs << " " <<  s << " " << "x" << pi.pixel.varIndex;
}

void LPWriter::writeEdge(std::ofstream &ofs,
                         const EdgeIncidence &ei)
{
    char s = ei.posIncidence?'+':'-';
    ofs << " " <<  s << " " << "x" << ei.edge.varIndex;
}

void LPWriter::writeConstraint(std::ofstream &ofs,
                               int& cIndexStart,
                               const LinelConstraints &lc)
{
    for(auto it=lc.begin();it!=lc.end();++it)
    {
        const Linel& linel = it->first;
        const LinelIncidence li = it->second;

        ofs << "c" << cIndexStart++ << ":";
        writePixel(ofs,li.pixel1);
        writePixel(ofs,li.pixel2);
        writeEdge(ofs,li.edge1);
        writeEdge(ofs,li.edge2);
        ofs << " = 0\n";
    }
}