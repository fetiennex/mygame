#include "./edge.hpp"
#include "./utils.h"
#include "./point.hpp"
#include <iostream>


std::array<Point *, 2> Edge::getPoints() {
        std::array<Point *, 2> points = {pt1, pt2};
        return points;
};
bool Edge::equals(Edge * e){
    std::array<Point *,2> pts = e -> getPoints();
    if ((pts.at(0) == pt1 && pts.at(1) == pt2) || (pts.at(0) == pt2 && pts.at(1) == pt1)){
        return true;
    } else {
        return false;
    }
};
std::string Edge::print(){
    std::string pt1_str = pt1 -> print();
    std::string pt2_str = pt2 -> print();
    std::string res = "{" + pt1_str + "," + pt2_str + "}";
    return res;
}
bool Edge::isConnectedTo(Edge * e){
    
    std::array<Point *,2> pts = e -> getPoints();
      if ((pts.at(0) == pt2 )/* || pts.at(1) == pt1 ) */){
        std::cout << print() << "is connected to" << e -> print() << std::endl;
        return true;
    } else {
        std::cout << print() << "is NOT connected to" << e -> print() << std::endl;
        return false;
    }
};
Point Edge::collide(Edge * e){
    std::array<Point *, 2> ptsA = getPoints();
    std::array<Point *, 2> ptsB = e -> getPoints();
    Point * a1 = ptsA.at(0);
    Point * a2 = ptsA.at(1);
    Point * b1 = ptsB.at(0);
    Point * b2 = ptsB.at(1);
    //we arbitrarily put ourself in the referencial centered on the first point, rotated to the angle of the second point
    Point new_a2(a2);
    Point new_b1(b1);
    Point new_b2(b2);
    new_a2.translate(a1);
    new_b1.translate(a1);
    new_b2.translate(a1);
    std::array<double, 2> polar = new_a2.getPolar();
    double theta = polar.at(1);
    new_a2.rotate(-theta);
    new_b1.rotate(-theta);
    new_b2.rotate(-theta);
    std::array<double,2> coords_a2 = new_a2.getCartesian();
    std::array<double,2> coords_b1 = new_b1.getCartesian();
    std::array<double,2> coords_b2 = new_b2.getCartesian();
    //we are in the referential.
    if ((coords_b1.at(1) > 0 && coords_b2.at(1) > 0) ||(coords_b1.at(1) < 0 && coords_b2.at(1) < 0)) { //y of both points are of the same sign, so no contact
        //std::cout <<"Missing : same side" << std::endl;
        throw MissingException();
    } else if (coords_b1.at(1)==0 && coords_b2.at(1)==0){
        //aligned !
        //std::cout <<"Missing : aligned" << std::endl;
        throw MissingException();
    } else {
        if (coords_b1.at(0) > coords_b2.at(0)){ //first we ne to order or points by x asc.
            std::array<double, 2> coords_b3;
            coords_b3 = coords_b2;
            coords_b2 = coords_b1;
            coords_b1 = coords_b3;
        }
        double y_b2 = coords_b2.at(1);
        double y_b1 = coords_b1.at(1);

        double dy = y_b2 - y_b1; //delta y to have the increment ratio
        double dx = coords_b2.at(0) - coords_b1.at(0);
        double ndy = -y_b1; // new delta y, to get to 0 it needs to be y1
        double ndx = ndy * (dx/dy); // new delta x = new delta y * (increment ratio)
        double x_res = coords_b1.at(0) +ndx;
        double y_res = coords_b1.at(1) +ndy;
        if (x_res > 0 && x_res < coords_a2.at(0)){ 
            Point res(x_res,y_res);
            res.rotate(theta);
            Point opposite = a1 -> opposite();
            res.translate(&opposite);
            return res;
        } else {
            //std::cout <<"Missing : not in range" << std::endl;
            throw MissingException();
        }
    }
};
void Edge::swapPoints(){
    Point * pt3;
    pt3 = pt1;
    pt1 = pt2;
    pt2 = pt3;
}
Edge::Edge(Point * ptt1, Point * ptt2){
    pt1 = ptt1;
    pt2 = ptt2;
};
Edge::Edge(Edge * e){
    std::array<Point *,2> pts = e -> getPoints();
    pt1 = pts.at(0);
    pt2 = pts.at(1);
}

