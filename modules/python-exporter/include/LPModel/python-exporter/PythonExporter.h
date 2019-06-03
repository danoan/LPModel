#ifndef EXPORT_CVXOPT_PYTHONEXPORTER_H
#define EXPORT_CVXOPT_PYTHONEXPORTER_H

#include <iostream>
#include <string>
#include <vector>

#include "DGtal/helpers/StdDefs.h"

#include "LPModel/initialization/model/Grid.h"

namespace LPModel
{
    namespace PythonExporter
    {
        typedef DGtal::Z2i::DigitalSet DigitalSet;
        typedef LPModel::Initialization::Pixel Pixel;
        typedef LPModel::Initialization::Grid::PixelMap PixelMap;

        typedef std::vector< std::vector<double> > SquareMatrix;
        typedef std::vector< std::vector<double> > ConstraintMatrix;

        template<int TDim>
        struct SparseMatrix
        {
            std::vector<unsigned long int> coeff[TDim];
            std::vector<double> values;

            void insert(std::initializer_list<unsigned long int> pCoeffs,double pValue)
            {
                assert(pCoeffs.size()==TDim);

                int i=0;
                auto it = pCoeffs.begin();
                for(;i<TDim;++i,++it)
                {
                    coeff[i].push_back(*it);
                }

                values.push_back(pValue);
            }
        };


        template<class TNumber>
        void writeVector(std::ofstream& ofs,
                         const std::string nameVar,
                         const std::vector<TNumber>& v);


        template<int TDim>
        void writeSparseMatrix(std::ofstream& ofs,
                               const std::string nameVar,
                               const SparseMatrix<TDim>& m);


        void exportDigitalSetAsMatrix(std::ofstream& ofs,
                                      const DigitalSet& ds);


        void exportPixelMap(std::ofstream& ofs,
                            const PixelMap& pixelMap);

        void exportPython(std::ofstream& ofs,
                          const std::vector<double>& U,
                          const SparseMatrix<2>& P1,
                          const SparseMatrix<2>& P2,
                          const SparseMatrix<2>& Z,
                          const std::vector<double>& z,
                          const SparseMatrix<2>& C,
                          const std::vector<double>& c);

        void exportPython(std::ofstream& ofs,
                          const std::vector<double>& U,
                          const std::vector<double>& V,
                          const SparseMatrix<2>& P,
                          const SparseMatrix<2>& Z,
                          const std::vector<double>& z,
                          const SparseMatrix<2>& C,
                          const std::vector<double>& c);

        void exportPython(std::ofstream& ofs,
                          const std::vector<double>& U,
                          const std::vector<double>& V,
                          const SparseMatrix<2>& Z,
                          const std::vector<double>& z,
                          const SparseMatrix<2>& C,
                          const std::vector<double>& c);

        void exportPython(std::ofstream& ofs,
                          const std::vector<double>& U,
                          const SparseMatrix<2> S,
                          const SparseMatrix<3> T,
                          const SparseMatrix<2>& C,
                          const std::vector<double>& c);

        #include "PythonExporter.hpp"
    }
}

#endif //EXPORT_CVXOPT_PYTHONEXPORTER_H
