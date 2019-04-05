#include <adept.h>
#include <iostream>

using adept::adouble;

adouble algorithm(const adouble x[2])
{
    adouble y = 4.0;
    adouble s = 2.0*x[0] + 3.0*x[1]*x[1];
    y*= sin(s);
    return y;
}


double algorithm_and_gradient(const double xval[2],
                              double dy_dx[2])
{
    adept::Stack stack;
    using adept::adouble;

    adouble x[2] = {xval[0],xval[1]};
    stack.new_recording();

    adouble y = algorithm(x);
    y.set_gradient(1.0);
    stack.compute_adjoint();
    dy_dx[0] = x[0].get_gradient();
    dy_dx[1] = x[1].get_gradient();
}

int main()
{
    double xval[2] = {1,3};
    double dy_dx[2];

    std::cout << algorithm_and_gradient(xval,dy_dx) << std::endl;
    return 0;
}