#include "LPModel/lpwriter/LPWriter.h"

using namespace LPModel;

void LPWriter::writePenalties(std::ofstream& ofs,const LinelConstraints& lc)
{
    for(auto it=lc.begin();it!=lc.end();++it)
    {
        const Linel& linel = it->first;
        const LinelIncidence li = it->second;

        StringConstraint sc;
        sc.rhs.push(0);


        writePixelPenalty(sc,li.pixel1);
        writePixelPenalty(sc,li.pixel2);
        writeEdgePenalty(sc,li.edge1);
        writeEdgePenalty(sc,li.edge2);

        while(!sc.lhs.empty()){ ofs << sc.lhs.top(); sc.lhs.pop();}
        //while(!sc.rhs.empty()){ ofs << sc.rhs.top(); sc.rhs.pop();}

    }
}

void LPWriter::writeObjective(std::ofstream& ofs,
                               Terms::Term::GeneralMap::const_iterator begin,
                               Terms::Term::GeneralMap::const_iterator end)
{
    for(auto it=begin;it!=end;++it)
    {
        if(it->second>0)
            ofs << " + ";
        else if(it->second<0)
            ofs << " - ";
        else
            continue;

        ofs << fabs(it->second) << " "; //Coefficient

        auto itMI = it->first.begin();
        while( (itMI+1)!=it->first.end() )
        {
            ofs << "x" << *itMI << " * ";
            ++itMI;
        }
        ofs << "x" << *itMI;
    }
}

void LPWriter::writeObjective(std::ofstream &ofs,
                              CoefficientMapIterator begin,
                              CoefficientMapIterator end)
{
    for(auto it=begin;it!=end;++it)
    {
        if(it->second>0)
            ofs << " + ";
        else if(it->second<0)
            ofs << " - ";
        else
            continue;

        ofs << fabs(it->second) << " " << "x" << it->first;
    }
}

void LPWriter::writeLinearizationConstraint(std::ofstream &ofs,
                                             int& startConstraintNum,
                                             const MyMultiIndex &multiIndex,
                                             const Index& uniqueIndex)
{
    for(auto it=multiIndex.begin();it!=multiIndex.end();++it)
    {
        ofs << "c" << startConstraintNum++ << ": " << "x" << uniqueIndex << " - x"  <<  *it << " <= 0\n";
    }

    ofs << "c" << startConstraintNum++ << ": " << "x" << uniqueIndex;
    for(auto it=multiIndex.begin();it!=multiIndex.end();++it)
    {
        ofs << " - x" << *it;
    }

    ofs << " >= - " << multiIndex.size()-1 << "\n";
}

void LPWriter::writeLinearizationConstraints(std::ofstream &ofs,
                                              int& startConstraintNum,
                                              MultiToUniqueMapIterator begin,
                                              MultiToUniqueMapIterator end)
{
    for(auto it=begin;it!=end;++it)
    {
        writeLinearizationConstraint(ofs,startConstraintNum,it->first,it->second);
    }
}

void LPWriter::writeBounds(std::ofstream &ofs,
                           CoefficientMapIterator begin,
                           CoefficientMapIterator end)
{
    for(auto it=begin;it!=end;++it)
    {
        ofs << "0 <= x" << it->first << " <= 1\n";
    }
}


void LPWriter::writeBounds(std::ofstream &ofs,
                            Grid::PixelMap::const_iterator begin,
                            Grid::PixelMap::const_iterator end)
{

    for(auto it=begin;it!=end;++it)
    {
        if(it->second.ct!=Initialization::Pixel::CellType::Variable) continue;
        ofs << "0 <= x" << it->second.varIndex << " <= 1\n";
    }
}

void LPWriter::writeBounds(std::ofstream &ofs,
                            Grid::EdgeMap::const_iterator begin,
                            Grid::EdgeMap::const_iterator end)
{

    for(auto it=begin;it!=end;++it)
    {
        ofs << "0 <= x" << it->second.varIndex << " <= 1\n";
    }
}

void LPWriter::writeBinaries(std::ofstream& ofs,
                              const Grid& grid)
{
    for(auto it=grid.pixelMap.begin();it!=grid.pixelMap.end();++it)
    {
        if(it->second.ct!=Initialization::Pixel::CellType::Variable) continue;
        ofs << "x" << it->second.varIndex << " ";
    }


    //It is enough to impose binray values for pixels.
    for(auto it=grid.edgeMap.begin();it!=grid.edgeMap.end();++it)
    {
        ofs << "x" << it->second.varIndex << " ";
    }

    ofs << "\n";
}

void LPWriter::writeBinaries(std::ofstream& ofs,
                             CoefficientMapIterator begin,
                             CoefficientMapIterator end)
{
    for(auto it=begin;it!=end;++it)
    {
        ofs << "x" << it->first << " ";
    }

    ofs << "\n";
}

void LPWriter::writePixelPenalty(StringConstraint& sc,
                                 const PixelIncidence &pi)
{
    std::ostringstream oss;
    char s=pi.posIncidence?'+':'-';
    if(pi.pixel.ct==Pixel::CellType::AuxiliarFrg)
    {
        double v = sc.rhs.top();sc.rhs.pop();

        if(pi.posIncidence) sc.rhs.push(v+1);
        else sc.rhs.push(v-1);
    }
    else if(pi.pixel.ct==Pixel::CellType::Variable)
    {
        oss << " " <<  s << " 100 " << "x" << pi.pixel.varIndex;

        sc.lhs.push(oss.str());
    }
}

void LPWriter::writeEdgePenalty(StringConstraint& sc,
                                const EdgeIncidence &ei)
{
    std::ostringstream oss;
    char s = ei.posIncidence?'+':'-';
    oss << " " <<  s << " 100 " << "x" << ei.edge.varIndex;

    sc.lhs.push(oss.str());
}

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

void LPWriter::writeFixedLabels(std::ofstream &ofs,
                           int& cIndexStart,
                           const FixedLabels& fixedLabels)
{
    for(auto it=fixedLabels.begin();it!=fixedLabels.end();++it)
    {
        ofs << "c" << cIndexStart++ << ": ";
        ofs << "x" << it->first << " = " << it->second << "\n";
    }
}

void LPWriter::writeLP(const std::string& outputFilePath,
                       const Initialization::Parameters& prm,
                       const Initialization::Grid& grid,
                       const Terms::Term::UnaryMap& um,
                       const MyLinearization& linearization,
                       const int relLevel,
                       const FixedLabels& fixedLabels)
{
    std::cerr << "Writing LP-Program at " << outputFilePath << "\n";

    boost::filesystem::path p(outputFilePath);
    boost::filesystem::create_directories(p.remove_filename());

    std::ofstream ofs(outputFilePath);
    ofs << "Minimize\n obj: ";

    writeObjective(ofs,um.begin(),um.end());
    writeObjective(ofs,linearization.begin(),linearization.end());


    ofs << "\nSubject To\n";
    int constraintNum=1;
    writeLinearizationConstraints(ofs,constraintNum,linearization.ubegin(),linearization.uend());


    Constraints::ClosedAndConnected::LinelConstraints lc;
    Constraints::ClosedAndConnected::closedConnectedContraints(lc,grid);
    LPWriter::writeConstraint(ofs,constraintNum,lc);

    LPWriter::writeFixedLabels(ofs,constraintNum,fixedLabels);

    if(relLevel==LPModel::RELAXATION_NONE)
    {
        ofs << "\nBinaries\n";
        writeBinaries(ofs,linearization.begin(),linearization.end());
        writeBinaries(ofs,grid);
    }else if(relLevel==LPModel::RELAXATION_ORIGINAL)
    {
        ofs << "\nBounds\n";
        writeBounds(ofs,grid.pixelMap.begin(),grid.pixelMap.end());
        writeBounds(ofs,grid.edgeMap.begin(),grid.edgeMap.end());

        ofs << "\nBinaries\n";
        writeBinaries(ofs,linearization.begin(),linearization.end());
    }else if(relLevel==LPModel::RELAXATION_AUXILIAR)
    {
        ofs << "\nBounds\n";
        writeBounds(ofs,linearization.begin(),linearization.end());

        ofs << "\nBinaries\n";
        writeBinaries(ofs,grid);
    }else if(relLevel==LPModel::RELAXATION_ALL)
    {
        ofs << "\nBounds\n";
        writeBounds(ofs,linearization.begin(),linearization.end());
        writeBounds(ofs,grid.pixelMap.begin(),grid.pixelMap.end());
        writeBounds(ofs,grid.edgeMap.begin(),grid.edgeMap.end());
    }




    ofs << "\nEnd\n";

    ofs.flush();
    ofs.close();
}


void LPWriter::writeQP(const std::string& outputFilePath,
                       const Initialization::Parameters& prm,
                       const Initialization::Grid& grid,
                       const Terms::Term::UnaryMap& um,
                       const Terms::Term::BinaryMap& bm,
                       const Terms::Term::BinaryMap& partialL,
                       const MyLinearization& linearization,
                       const int relLevel,
                       const FixedLabels& fixedLabels)
{
    std::cerr << "Writing QP-Program at " << outputFilePath << "\n";

    boost::filesystem::path p(outputFilePath);
    boost::filesystem::create_directories(p.remove_filename());

    std::ofstream ofs(outputFilePath);
    ofs << "Minimize\n obj: ";

    writeObjective(ofs,um.begin(),um.end());
    ofs << " + [ ";
    writeObjective(ofs,bm.begin(),bm.end());
    writeObjective(ofs,partialL.begin(),partialL.end());
    ofs << " ] ";

    ofs << "\nSubject To\n";
    int constraintNum=1;
    writeLinearizationConstraints(ofs,constraintNum,linearization.ubegin(),linearization.uend());

    Constraints::ClosedAndConnected::LinelConstraints lc;
    Constraints::ClosedAndConnected::closedConnectedContraints(lc,grid);
    LPWriter::writeConstraint(ofs,constraintNum,lc);

    LPWriter::writeFixedLabels(ofs,constraintNum,fixedLabels);


    if(relLevel==LPModel::RELAXATION_NONE)
    {
        ofs << "\nBinaries\n";
        writeBinaries(ofs,linearization.begin(),linearization.end());
        writeBinaries(ofs,grid);
    }else if(relLevel==LPModel::RELAXATION_ORIGINAL)
    {
        ofs << "\nBounds\n";
        writeBounds(ofs,grid.pixelMap.begin(),grid.pixelMap.end());
        writeBounds(ofs,grid.edgeMap.begin(),grid.edgeMap.end());

        ofs << "\nBinaries\n";
        writeBinaries(ofs,linearization.begin(),linearization.end());
    }else if(relLevel==LPModel::RELAXATION_AUXILIAR)
    {
        ofs << "\nBounds\n";
        writeBounds(ofs,linearization.begin(),linearization.end());

        ofs << "\nBinaries\n";
        writeBinaries(ofs,grid);
    }else if(relLevel==LPModel::RELAXATION_ALL)
    {
        ofs << "\nBounds\n";
        writeBounds(ofs,linearization.begin(),linearization.end());
        writeBounds(ofs,grid.pixelMap.begin(),grid.pixelMap.end());
        writeBounds(ofs,grid.edgeMap.begin(),grid.edgeMap.end());
    }




    ofs << "\nEnd";

    ofs.flush();
    ofs.close();

}