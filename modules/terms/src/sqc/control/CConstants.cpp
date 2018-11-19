#include "LPModel/terms/sqc/control/CConstants.h"

using namespace LPModel::Terms::SquaredCurvature;

Constants CConstants::setConstants(const Parameters &prm)
{
    DigitalSet tempBall( Domain( 2*Point(-prm.radius,-prm.radius), 2*Point(prm.radius,prm.radius) ) );
    DIPaCUS::Misc::DigitalBallIntersection::digitalBall(tempBall,Point(0,0),prm.radius);

    double W = 0;
    double C = tempBall.size() / 2.0;
    double F = 9.0 / pow(prm.radius, 6.0);
    double R = prm.radius;

    DIPaCUS::Misc::DigitalBallIntersection DBI = prm.handle.intersectionComputer(prm.radius,
                                                                                 prm.odrModel.trustFRG);

    Constants::UnaryContribution uc;
    Domain domain = prm.odrModel.trustFRG.domain();
    DigitalSet temp(domain);
    for (auto it = prm.odrModel.applicationRegion.begin(); it != prm.odrModel.applicationRegion.end(); ++it) {
        temp.clear();
        DBI(temp, *it);

        double Ij = temp.size();

        W += pow(C, 2);
        W += pow(Ij, 2);
        W += -2 * C * Ij;

        uc[*it] = 1 - 2 * C + 2 * Ij;
    }

    return Constants(W,C,F,R,uc);
}