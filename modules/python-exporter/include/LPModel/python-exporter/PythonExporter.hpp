#include "PythonExporter.h"

template<int TDim>
void writeSparseMatrix(std::ofstream& ofs,
                       const std::string nameVar,
                       const SparseMatrix<TDim>& m)
{
    int i=0;
    for(;i<TDim;++i)
    {
        std::string dimName = nameVar + "_dim" + std::to_string(i);
        writeVector(ofs,dimName,m.coeff[i]);
    }

    std::string valuesName = nameVar + "_values";
    writeVector(ofs,valuesName,m.values);

}

template< class TNumber >
void writeVector(std::ofstream& ofs,
                 const std::string nameVar,
                 const std::vector<TNumber>& v)
{
    if(v.size()==0) return;

    auto it = v.begin();
    ofs << nameVar << " = [ " << *it; ++it;
    for(;it!=v.end();++it)
    {
        ofs << "," << *it;
    }
    ofs << "]\n\n";
}