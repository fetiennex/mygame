#include <array>
#include <exception>
#include <cmath>
#include <algorithm>
#include <iostream>
#include "./utils.h"
#include "./point.hpp"
#include "./edge.hpp"

double fromPiToMinusPi(double angle){
    if (angle > M_PI){
        return -(angle - M_PI);
    } else {
        return angle;
    };
};
linear_function create_linear_function(double x1,double y1,double x2, double y2){
    std::cout << "x1 =" << x1 <<" y1 =" << y1 <<" x2 =" << x2 <<" y2 =" << y2 << std::endl;
    linear_function res;
    if (x1 == x2){
        res.x = x1;
        res.isx = true;
        return res;
    } else if (y1 == y2) {
        res.y = {0.f,(double)y1};
        res.isx = false;
        return res;
    } else {
        double coeff = ((double)(y1 - y2))/((double)(x1 - x2));
        double offset = (double)y1 - coeff*(double)x1;
        res.y = {coeff,offset};
        res.isx = false;
        return res;
    };
};
linear_function create_linear_function(Point * ptA, Point * ptB){
    std::array<double,2> coordsA = ptA -> getCartesian();
    std::array<double,2> coordsB = ptB -> getCartesian();
    linear_function res = create_linear_function(coordsA.at(0),coordsA.at(1),coordsB.at(0),coordsB.at(1));
    return res;
};

bool joined_check_if_inter(double yy1, double yy2, double yy3, double yy4){
    coords_bool y1;
    y1.y = yy1;
    y1.who = true;
    coords_bool y2;
    y2.y = yy3;
    y2.who = true;
    coords_bool y3;
    y3.y = yy3;
    y3.who = false;
    coords_bool y4;
    y4.y = yy4;
    y4.who = false;

    std::array<coords_bool, 4> y_vals = {y1,y2,y3,y4};
    std::sort(y_vals.begin(),y_vals.end(),[](coords_bool a, coords_bool b){
        return a.y > b.y;
    });
    if (y_vals.at(0).who == y_vals.at(1).who && y_vals.at(1).y != y_vals.at(2).y){
        return false;
    } else {
        return true;
    };
}

