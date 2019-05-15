#ifndef EXPORT_CVXOPT_PYTHONEXPORTER_H
#define EXPORT_CVXOPT_PYTHONEXPORTER_H

#include <iostream>
#include <string>
#include <vector>

#include "DGtal/helpers/StdDefs.h"

#include "LPModel/initialization/model/Grid.h"

namespace PythonExporter
{
    typedef DGtal::Z2i::DigitalSet DigitalSet;
    typedef LPModel::Initialization::Pixel Pixel;
    typedef LPModel::Initialization::Grid::PixelMap PixelMap;

    typedef std::vector< std::vector<double> > SquareMatrix;
    typedef std::vector< std::vector<double> > ConstraintMatrix;

    struct SparseMatrix
    {
        void insert(double row,double col,double value)
        {
            rows.push_back(row);
            cols.push_back(col);
            values.push_back(value);
        }

        std::vector<double> values;
        std::vector<double> rows;
        std::vector<double> cols;
    };

    void initSquareMatrix(SquareMatrix& m, unsigned long int size);

    void initConstraintMatrix(ConstraintMatrix& m, unsigned long int numRows, unsigned long int numCols);

    void writeVector(std::ofstream& ofs,
                     const std::string nameVar,
                     const std::vector<double>& v);

    void writeMatrix(std::ofstream& ofs,
                     const std::string nameVar,
                     const SquareMatrix& m);

    void writeSparseMatrix(std::ofstream& ofs,
                           const std::string nameVar,
                           const SparseMatrix& m);

    void exportDigitalSetAsMatrix(std::ofstream& ofs,
                                  const DigitalSet& ds);


    void exportPixelMap(std::ofstream& ofs,
                        const PixelMap& pixelMap);

    void exportPython(std::ofstream& ofs,
                      const std::vector<double>& U,
                      const SparseMatrix& P1,
                      const SparseMatrix& P2,
                      const SparseMatrix& Z,
                      const std::vector<double>& z,
                      const SparseMatrix& C,
                      const std::vector<double>& c);

    void exportPython(std::ofstream& ofs,
                      const std::vector<double>& U,
                      const std::vector<double>& V,
                      const SparseMatrix& P,
                      const SparseMatrix& Z,
                      const std::vector<double>& z,
                      const SparseMatrix& C,
                      const std::vector<double>& c);

    void exportPython(std::ofstream& ofs,
                      const std::vector<double>& U,
                      const std::vector<double>& V,
                      const SparseMatrix& Z,
                      const std::vector<double>& z,
                      const SparseMatrix& C,
                      const std::vector<double>& c);
}

#endif //EXPORT_CVXOPT_PYTHONEXPORTER_H
