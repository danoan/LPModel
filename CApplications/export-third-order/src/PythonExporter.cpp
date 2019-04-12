#include "PythonExporter.h"

namespace PythonExporter
{
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
                      const SparseMatrix<2> S,
                      const SparseMatrix<3> T,
                      const SparseMatrix<2>& C,
                      const std::vector<double>& c)
    {
        writeVector(ofs,"_U",U);
        writeSparseMatrix(ofs,"_S",S);
        writeSparseMatrix(ofs,"_T",T);
        writeSparseMatrix(ofs,"_C",C);
        writeVector(ofs,"_c",c);
    }

}