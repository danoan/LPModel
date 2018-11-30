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

void LPWriter::writeLP(const std::string& outputFilePath,
                       const Initialization::Parameters& prm,
                       const Initialization::Grid& grid,
                       const Terms::Term::UnaryMap& um,
                       const MyLinearization& linearization)
{
    std::cerr << "Writing LP-Program at " << outputFilePath << "\n";

    boost::filesystem::path p(outputFilePath);
    boost::filesystem::create_directories(p.remove_filename());

    std::ofstream ofs(outputFilePath);
    ofs << "Minimize\n obj: ";

    Objective::writeObjective(ofs,um.begin(),um.end());
    Objective::writeObjective(ofs,linearization.begin(),linearization.end());


    ofs << "\nSubject To\n";
    int constraintNum=1;
    Objective::writeLinearizationConstraints(ofs,constraintNum,linearization.ubegin(),linearization.uend());


    Constraints::ClosedAndConnected::LinelConstraints lc;
    Constraints::ClosedAndConnected::closedConnectedContraints(lc,grid);
    LPWriter::writeConstraint(ofs,constraintNum,lc);

    ofs << "\nBounds\n";
    Objective::writeBounds(ofs,grid,linearization.begin(),linearization.end());
    ofs << "\nEnd";

    ofs.flush();
    ofs.close();
}