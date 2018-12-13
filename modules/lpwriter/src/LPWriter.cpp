#include "LPModel/lpwriter/LPWriter.h"

using namespace LPModel;

void LPWriter::writePixel(StringConstraint& sc,
                          const PixelIncidence &pi)
{
    std::ostringstream oss;
    char s=pi.posIncidence?'+':'-';
    if(pi.pixel.ct==Pixel::CellType::AuxiliarFrg)
    {
        double v = sc.rhs.top();sc.rhs.pop();

        if(pi.posIncidence) sc.rhs.push(v-1);
        else sc.rhs.push(v+1);
    }
    else if(pi.pixel.ct==Pixel::CellType::Variable)
    {
        oss << " " <<  s << " " << "x" << pi.pixel.varIndex;

        sc.lhs.push(oss.str());
    }
}

void LPWriter::writeEdge(StringConstraint& sc,
                         const EdgeIncidence &ei)
{
    std::ostringstream oss;
    char s = ei.posIncidence?'+':'-';
    oss << " " <<  s << " " << "x" << ei.edge.varIndex;

    sc.lhs.push(oss.str());
}

void LPWriter::writeConstraint(std::ofstream &ofs,
                               int& cIndexStart,
                               const LinelConstraints &lc)
{

    for(auto it=lc.begin();it!=lc.end();++it)
    {
        const Linel& linel = it->first;
        const LinelIncidence li = it->second;

        StringConstraint sc;
        sc.rhs.push(0);


        writePixel(sc,li.pixel1);
        writePixel(sc,li.pixel2);
        writeEdge(sc,li.edge1);
        writeEdge(sc,li.edge2);

        ofs << "c" << cIndexStart++ << ":";
        while(!sc.lhs.empty()){ ofs << sc.lhs.top(); sc.lhs.pop();}
        ofs << " = ";
        while(!sc.rhs.empty()){ ofs << sc.rhs.top(); sc.rhs.pop();}
        ofs << "\n";

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
    Objective::writeBounds(ofs,linearization.begin(),linearization.end());

    ofs << "\nBinaries\n";
    Objective::writeBinaries(ofs,grid);

    ofs << "\nEnd";

    ofs.flush();
    ofs.close();
}