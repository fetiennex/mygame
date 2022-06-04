#include "./point.hpp"
#include "./player.hpp"
#include "./edge.hpp"
#include <cmath>
#include <iostream>
#include "./utils.h"

double accToGain(double v_cible,double v,double max_a){
    double speed_to_gain = v_cible - v; //pos ou neg
    double sign = std::abs(speed_to_gain)/speed_to_gain; // = 0 ou 1
    double acc;
    if (std::abs(speed_to_gain) < max_a){
        acc = speed_to_gain;
    } else {
        acc = max_a*sign;
    }
    return acc;
}
void Player::go(directions dirs){
    Point vcible(0,0);
    if (dirs.left){
        Point v_left_cible(-vlim,0);
        Point v_left_cible_o = v_left_cible.opposite();
        vcible.translate(&v_left_cible_o);
    }
    if (dirs.right){
        Point v_right_cible(vlim,0);
        Point v_right_cible_o = v_right_cible.opposite();
        vcible.translate(&v_right_cible_o);
    }
    if (dirs.up){
        Point v_up_cible(0,vlim);
        Point v_up_cible_o = v_up_cible.opposite();
        vcible.translate(&v_up_cible_o);
    }
    if (dirs.down){
        Point v_down_cible(0,-vlim);
        Point v_down_cible_o = v_down_cible.opposite();
        vcible.translate(&v_down_cible_o);
    }
    std::array<double,2> vcible_coords = vcible.getCartesian();
    std::array<double,2> v_coords = v -> getCartesian();
    if (std::abs(vcible_coords.at(0)) == vlim && std::abs(vcible_coords.at(1)) == vlim) {
        std::cout << "trigerring sqrt(2)/2" << std::endl;
        double coeff = (std::sqrt(2)/2);
        double new_x = coeff * vcible_coords.at(0);
        double new_y = coeff * vcible_coords.at(1);
        vcible.update(new_x,new_y);
        vcible_coords = vcible.getCartesian();
        std::cout << "v_cible coords =" << vcible.precisePrint() << std::endl;
    }
    double x_acc_to_gain = accToGain(vcible_coords.at(0),v_coords.at(0),max_a);
    double y_acc_to_gain = accToGain(vcible_coords.at(1),v_coords.at(1),max_a);
    a -> update(x_acc_to_gain,y_acc_to_gain);
};
void Player::step(){
    Point a_op = a -> opposite();
    v -> translate(&a_op);
    Point v_op = v -> opposite();
    pos -> translate(&v_op);
};
Point * Player::getV(){
    return v;
}
Point Player::collideWall(Edge * e, bool form_is_above){
    

    bool pt_is_above = pos -> isAbove(e);
    std::array<Point *,2> edgepoints = e -> getPoints();
    Point e_pt2(edgepoints.at(1));
    Point me(pos);
    Point next_me(&me);
    Point v_o = v -> opposite();
    next_me.translate(&v_o);


    e_pt2.translate(edgepoints.at(0));
    me.translate(edgepoints.at(0));
    next_me.translate(edgepoints.at(0));
    std::array<double, 2> polar = e_pt2.getPolar();
    double angle = polar.at(1);
    me.rotate(angle);
    next_me.rotate(angle);
    Edge trajectory(&me,&next_me);
    
    double upper_limit;
    double lower_limit;
    double left_limit;
    double right_limit;
    if ((form_is_above && pt_is_above) || (!form_is_above && !pt_is_above)){
        //form is on the same side as the point, no interation
        throw MissingException();
    } else {
        //point is on the right side, but is it in the rectangle [-radius,e.length() +radius],[0,e.height +radius?]
        if (form_is_above){
            upper_limit = 0;
            lower_limit = -radius;
        } else {
            upper_limit = radius;
            lower_limit = 0;
        }

    }



}
Player::Player(Point * pos0, Point * v0, Point * a0, double vvlim, double mmax_a, double rradius){
    pos = pos0;
    v = v0;
    a = a0;
    vlim = vvlim;
    max_a = mmax_a;
    radius = rradius;
}
