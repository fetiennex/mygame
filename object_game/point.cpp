#include "./point.hpp"
#include "./form.hpp"
#include "./edge.hpp"
#include <cmath>
#include <iostream>


std::array<double,2> Point::getPolar(){
    double r = std::sqrt(x*x + y*y);
    double theta = atan2(y,x);
    std::array<double,2> res = {r,theta};
    return res;
};
std::array<double,2> Point::getCartesian(){
    std::array<double,2> res = {x,y};
    return res;
};
Form * Point::getParent() {
    return parent;
};
Point Point::opposite(){
    return Point(-x,-y);
}
double Point::distance(Point * p){
    std::array<double,2> pcoords = p -> getCartesian();
    double xx = pcoords.at(0);
    double yy = pcoords.at(1);
    double res = std::sqrt((x - xx)*(x - xx) + (y - yy)*(y - yy));
    return res;
};
void Point::translate(double xx, double yy){
    x = x - xx;
    y = y - yy;
};
void Point::translate(Point * pt){
    std::array<double,2> coords = pt -> getCartesian();
    double xx = coords.at(0);
    double yy = coords.at(1);
    translate(xx,yy);
};
void Point::rotate(double theta){

    double s_theta_x = sin(theta)*x;
    double c_theta_y = cos(theta)*y;
    double s_theta_y = sin(theta)*y;
    double c_theta_x = cos(theta)*x;
    double my_res_y = s_theta_x + c_theta_y;
    double my_res_x = c_theta_x - s_theta_y;
    x = my_res_x;
    y = my_res_y;

};
std::string Point::print(){
    std::string res = "[" + std::to_string((int)x) + "," + std::to_string((int)y) + "]";
    return res;
}
std::string Point::precisePrint(){
    std::string res = "[" + std::to_string(x) + "," + std::to_string(y) + "]";
    return res;
}
bool Point::isAbove(Edge * e){
    //we consider the edge as oriented (pt1 -> pt2)
    std::array<Point *, 2> ptsE = e -> getPoints();
    Point new_pt2(ptsE.at(1));
    new_pt2.translate(ptsE.at(0));
    std::array<double, 2> polar = new_pt2.getPolar();
    double theta = polar.at(1);
    Point new_me(this);
    new_me.translate(ptsE.at(0));
    new_me.rotate(-theta);
    std::array<double, 2> new_coords = new_me.getCartesian();
    if (new_coords.at(1) > 0){
        return true;
    } else {
        return false;
    }

}
void Point::update(double xx, double yy){
    x = xx;
    y = yy;
}
Point::Point(double xx, double yy){
    x = xx;
    y = yy;
};
Point::Point(Point * p){
    std::array<double, 2> coords = p -> getCartesian();
    x = coords.at(0);
    y = coords.at(1);
};