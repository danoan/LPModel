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
                           const SquareMatrix& m);

    void exportDigitalSetAsMatrix(std::ofstream& ofs,
                                  const DigitalSet& ds);


    void exportPixelMap(std::ofstream& ofs,
                        const PixelMap& pixelMap);

    void exportPython(std::ofstream& ofs,
                      const std::vector<double>& U,
                      const SquareMatrix& P1,
                      const SquareMatrix& P2,
                      const ConstraintMatrix& Z,
                      const std::vector<double>& z,
                      const ConstraintMatrix& C,
                      const std::vector<double>& c);

    void exportPython(std::ofstream& ofs,
                      const std::vector<double>& U,
                      const std::vector<double>& V,
                      const SquareMatrix& P,
                      const ConstraintMatrix& Z,
                      const std::vector<double>& z,
                      const ConstraintMatrix& C,
                      const std::vector<double>& c);

    void exportPython(std::ofstream& ofs,
                      const std::vector<double>& U,
                      const std::vector<double>& V,
                      const ConstraintMatrix& Z,
                      const std::vector<double>& z,
                      const ConstraintMatrix& C,
                      const std::vector<double>& c);
}

#endif //EXPORT_CVXOPT_PYTHONEXPORTER_H
