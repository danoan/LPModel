#include "PythonExporter.h"

namespace PythonExporter
{
    void initSquareMatrix(SquareMatrix& m, unsigned long int size)
    {
        m.resize(size);
        for(auto it=m.begin();it!=m.end();++it)
        {
            it->resize(size);
            for(auto it2=it->begin();it2!=it->end();++it2) *it2=0;
        }
    }

    void initConstraintMatrix(ConstraintMatrix& m, unsigned long int numRows, unsigned long int numCols)
    {
        m.resize(numRows);
        for(auto it=m.begin();it!=m.end();++it)
        {
            it->resize(numCols);
            for(auto it2=it->begin();it2!=it->end();++it2) *it2=0;
        }
    }

    void writeVector(std::ofstream& ofs,
                     const std::string nameVar,
                     const std::vector<double>& v)
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

    void writeMatrix(std::ofstream& ofs,
                     const std::string nameVar,
                     const SquareMatrix& m)
    {
        if(m.size()==0) return;

        ofs << nameVar << " = [ ";
        for(auto itR = m.begin();itR!=m.end();++itR)
        {
            auto itC=itR->begin();
            ofs << " [ " << *itC; ++itC;
            for(;itC!=itR->end();++itC)
            {
                ofs << "," << *itC ;
            }
            ofs << " ] ";
            if(itR+1!=m.end()) ofs << ", ";
        }
        ofs << " ] \n\n";
    }

    void writeSparseMatrix(std::ofstream& ofs,
                           const std::string nameVar,
                           const SparseMatrix& m)
    {
        std::string valuesName = nameVar + "_values";
        writeVector(ofs,valuesName,m.values);

        std::string rowsName = nameVar + "_row";
        writeVector(ofs,rowsName,m.rows);

        std::string colsName = nameVar + "_cols";
        writeVector(ofs,colsName,m.cols);

    }

    void exportDigitalSetAsMatrix(std::ofstream& ofs,
                                  const DigitalSet& ds)
    {
        typedef DGtal::Z2i::Domain Domain;
        typedef DGtal::Z2i::Point Point;

        const Domain& domain = ds.domain();
        Point lb = domain.lowerBound();
        Point ub = domain.upperBound();

        int startR = ub[1];
        int endR = lb[1];
        int startC = lb[0];
        int endC = ub[0];

        ofs << "digitalModel=[";
        for(int r=startR;r>=endR;--r)
        {
            ofs << "[";
            for(int c=startC;c<=endC;++c)
            {
                if(ds(Point(c,r))) ofs << "255";
                else ofs << "0";

                if(c!=endC) ofs << ",";
            }
            ofs << "]";
            if(r!=endR) ofs << ",";
        }
        ofs << "]\n\n";

    }


    void exportPixelMap(std::ofstream& ofs,
                        const PixelMap& pixelMap)
    {
        auto it=pixelMap.begin();
        ofs << "pixelMap={";

        const Pixel& pixel = it->second;
        if( pixel.ct == Pixel::CellType::Variable )
        {
            ofs << pixel.varIndex << ":{'row':" << (pixel.row+1)/2 << ",'col':" << (pixel.col+1)/2 << "}";
        }
        ++it;
        while(it!=pixelMap.end())
        {
            const Pixel& pixel = it->second;
            if( pixel.ct == Pixel::CellType::Variable )
            {
                if(it!=pixelMap.end()) ofs << ",";
                ofs << pixel.varIndex << ":{'row':" << (pixel.row+1)/2 << ",'col':" << (pixel.col+1)/2 << "}";
            }
            ++it;
        }

        ofs << "}\n\n";

    }

    void exportPython(std::ofstream& ofs,
                      const std::vector<double>& U,
                      const SparseMatrix& P1,
                      const SparseMatrix& P2,
                      const SparseMatrix& Z,
                      const std::vector<double>& z,
                      const SparseMatrix& C,
                      const std::vector<double>& c)
    {
        writeVector(ofs,"_U",U);
        writeSparseMatrix(ofs,"_P1",P1);
        writeSparseMatrix(ofs,"_P2",P2);
        writeSparseMatrix(ofs,"_Z",Z);
        writeVector(ofs,"_z",z);
        writeSparseMatrix(ofs,"_C",C);
        writeVector(ofs,"_c",c);
    }

    void exportPython(std::ofstream& ofs,
                      const std::vector<double>& U,
                      const std::vector<double>& V,
                      const SparseMatrix& P,
                      const SparseMatrix& Z,
                      const std::vector<double>& z,
                      const SparseMatrix& C,
                      const std::vector<double>& c)
    {
        writeVector(ofs,"_U",U);
        writeVector(ofs,"_V",V);
        writeSparseMatrix(ofs,"_P",P);
        writeSparseMatrix(ofs,"_Z",Z);
        writeVector(ofs,"_z",z);
        writeSparseMatrix(ofs,"_C",C);
        writeVector(ofs,"_c",c);
    }

    void exportPython(std::ofstream& ofs,
                      const std::vector<double>& U,
                      const std::vector<double>& V,
                      const SparseMatrix& Z,
                      const std::vector<double>& z,
                      const SparseMatrix& C,
                      const std::vector<double>& c)
    {
        writeVector(ofs,"_U",U);
        writeVector(ofs,"_V",V);
        writeSparseMatrix(ofs,"_Z",Z);
        writeVector(ofs,"_z",z);
        writeSparseMatrix(ofs,"_C",C);
        writeVector(ofs,"_c",c);
    }
}