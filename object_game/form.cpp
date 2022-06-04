#include "./form.hpp"
#include "./edge.hpp"
#include "./point.hpp"
#include "./world.hpp"
#include "./utils.h"
#include <algorithm>
#include <iostream>

std::vector<Point *> Form::getPoints(){
    std::vector<Point *> ptlist;
    for (auto it = edgelist.begin(); it != edgelist.end(); it++){
        std::array<Point *,2> points = (*it) -> getPoints();
        ptlist.push_back(points.at(0));
    }
    return ptlist;
};
std::vector<Edge *> Form::getEdges(){
    return edgelist;
};
void Form::setConvexDecomp(){
    std::cout << "setting convex decomp " << std::endl;
    convex_decomposition = convexDecomposition();
}
std::vector<std::vector<Form *>> Form::getConvexDecomp(){
    return convex_decomposition;
}

Edge * Form::getNextEdge(Point * summit, Edge * previous_edge){
    //for a square [1,2,3,4] = [[1,2],[2,3],[3,4],[4,1]]
    Edge* res;
    auto my_edge = std::find(edgelist.begin(),edgelist.end(),previous_edge);
    int index_edge = my_edge - edgelist.begin();

    std::vector<Point *> my_pts = getPoints();
    auto my_pt = std::find(my_pts.begin(),my_pts.end(),summit);
    int index_pt = my_pt - my_pts.begin();
    

    if (index_pt == index_edge){ // each edge corresponds to the first point, so when they match it means the next is previous
        int index_edge_res = (index_edge - 1) % edgelist.size();
        res = edgelist.at(index_edge_res);
        return res;
    } else {
        //going
        int index_edge_res = (index_edge + 1) % edgelist.size();
        res = edgelist.at(index_edge_res);
        return res;
    }

};
void Form::cycleEdges(){
    std::rotate(edgelist.begin(),edgelist.begin()+1,edgelist.end());
}
bool Form::isAbove(){
    double totalAngles = 0;
    std::cout << "trying isAbove" << std::endl;
    for (auto it = edgelist.begin(); it != std::prev(edgelist.end()); it++){
        std::array<Point *,2> ptsE = (*it) -> getPoints();
        Point * real_next_summit = ((*(it+1)) -> getPoints()).at(1);
        std::cout << "real_next_summit = " << real_next_summit -> print() << std::endl;
        Point next_summit(real_next_summit);
        Point prev_pt(ptsE.at(0));
        prev_pt.translate(ptsE.at(1));
        std::array<double, 2> prev_polar = prev_pt.getPolar();
        double ttheta = prev_polar.at(1);
        next_summit.translate(ptsE.at(1));
        next_summit.rotate(M_PI - ttheta);
        std::array<double, 2> polar = next_summit.getPolar();
        double theta = polar.at(1);
        totalAngles = totalAngles + theta;
    }
    std::cout << "totalAngles = " << totalAngles << std::endl;
    if (totalAngles>0){
        return true;
    } else {
        return false;
    }
}
void Form::setTriangleDecomp(){
    triangle_decomposition = triangleDecomposition();
};
std::vector<Form *> Form::getTriangleDecomp(){
    return triangle_decomposition;
}
std::vector<Form *> Form::triangleDecomposition(){
    std::vector<Form *> res = {};
    Edge * current_e;
    Edge * next_e;
    bool is_above = isAbove();
    std::vector<Point *> my_points = getPoints();


    while (true){
        current_e = edgelist.at(0);
        next_e = edgelist.at(1);
        std::cout << "trying edges " << current_e -> print()
        << " and " << next_e -> print();
        Point * first = (current_e->getPoints()).at(0);
        Point * last = (next_e->getPoints()).at(1);
        Point * summit = (current_e->getPoints()).at(1);

        Edge my_new_e(last,first);
        bool summit_is_above = summit ->  isAbove(&my_new_e);
        if (summit_is_above == is_above){ //check is summit is on the right side of the edge
            auto any_in_triangle = std::find_if(my_points.begin(),my_points.end(),
            [my_new_e,current_e,next_e,is_above,first,last,summit](Point * p) mutable {
                if (p == first || p == last || p == summit) {return false;}
                if (
                    (p-> isAbove(current_e) == is_above)
                    && (p-> isAbove(next_e) == is_above)
                    && (p-> isAbove(&my_new_e) == is_above)
                ){ // fail : the scanned point is in the new triangle
                    return true;
                }
                return false;
            });
            if (any_in_triangle==my_points.end()){
                std::cout << "found a new edge" << my_new_e.print() << std::endl;
                
                Edge * my_real_e = new Edge(&my_new_e);
                Edge * my_real_e_inverted = new Edge(&my_new_e);
                my_real_e_inverted -> swapPoints();
                
                std::vector<Edge *> triangle_edges = {current_e,next_e,my_real_e};
                Form * triangle = new Form(triangle_edges);
                
                std::vector<Edge *> residue_edges = {};
                residue_edges.push_back(my_real_e_inverted);
                for (auto it=edgelist.begin()+2; it != edgelist.end(); it++){
                    residue_edges.push_back((*it));
                }
                
                Form residue(residue_edges);
                std::cout << "residue = " << residue.print() << std::endl;
                if (residue_edges.size() == 3){
                    //residue is also a triangle : end !
                    res.push_back(triangle);
                    Form * real_residue = new Form(residue_edges);
                    res.push_back(real_residue);
                    return res;
                }
                std::vector<Form *> residue_decomp = residue.triangleDecomposition();
                res.push_back(triangle);
                res.insert(res.end(),residue_decomp.begin(),residue_decomp.end());
                return res;

            } else {
                std::cout << "found summit in triangle: " << (*any_in_triangle) ->print()<<std::endl;
            }
        } else {
            std::cout<<"summit not on correct side, summit_is_above: "<<
            summit_is_above << ", is_above: "
            << is_above << std::endl;
            
        }
        std::cout << "cycling edges on form" << std::endl;
        cycleEdges();
    }
}


Form * Form::getConvexEnv(){
    std::cout << "getting convex env" << std::endl;
    std::vector<Point *> pts = getPoints();
    std::vector<Edge *> res;
    for (auto it = pts.begin(); it != pts.end(); it++){ //à améliorer avec des SIMD
        for (auto itt = pts.begin(); itt != pts.end(); itt++){
            if (itt != it){
                Point pt2(*itt);
                pt2.translate(*it);
                std::array<double, 2> polar = pt2.getPolar();
                double angle = polar.at(1);
                bool is_covering = true;
                for (auto at = pts.begin(); at != pts.end(); at++){
                    if (at != it && at != itt){
                        Point tf_pt(*at);
                        tf_pt.translate(*it);
                        tf_pt.rotate(-angle);
                        std::array<double, 2> coords = tf_pt.getCartesian();
                        //std::cout << "tf coords = " << coords.at(0) << "," << coords.at(1) << std::endl;
                        if (coords.at(1) < -0.5){
                            is_covering = false;
                        }
                    }

                }
                if (is_covering){
                    std::cout << "is covering" << std::endl;
                    Edge * new_edge = new Edge((*it),(*itt));
                    res.push_back(new_edge);
                }
            }
        }
    }
    std::vector<Edge *> sorted_res = {res.at(0)};
    while (sorted_res.size() < res.size()){
        Edge * e1 = sorted_res.back();
        auto isNextEdge = [e1](Edge * e2){
            std::array<Point *, 2> pts_e1 = e1 -> getPoints(); // = {pt11, pt12}
            std::array<Point *, 2> pts_e2 = e2 -> getPoints(); // = {pt21, pt22}
            if (e1 -> equals(e2)){
                std::cout << "same edge" << std::endl;
                return false;
            } else if (pts_e1.at(1) == pts_e2.at(0)){
                return true;
            } else if (pts_e1.at(1) == pts_e2.at(1)){
                e2 -> swapPoints();
                std::cout << "swaping points" << std::endl;
                return true;
            } else {
                return false;
            }
        };
    
        Edge * next_e = (*std::find_if(res.begin(),res.end(),isNextEdge));
        sorted_res.push_back(next_e);
    }
    Form * my_res = new Form(sorted_res);
    std::cout << "convex decomp = " << my_res -> print() << std::endl;
    return my_res;
}

std::vector<std::vector<Form *>> Form::convexDecomposition(){
    std::cout << "new convex decomposition for " << print() << std::endl;
    Form * convex_env = getConvexEnv();
    std::vector<Edge *> convex_edges = convex_env -> getEdges();
    std::vector<Form *> convex_layer;
    std::vector<std::vector<Form *>> result;
    convex_layer.push_back(convex_env);
    result.push_back(convex_layer);
    if (convex_edges.size() == edgelist.size()){
         //result = [[Form * convex_env]]
        std::cout << "form is convex !" << std::endl;
        return result;
    }//forme convexe
    else {
        std::cout << "form is not convex" << std::endl;
        convex_layer.push_back(convex_env);
        std::vector<Edge *> new_edges;
        std::vector<std::vector<Edge *>> old_edges {};
        std::cout << "old eges .size = " << old_edges.size() << std::endl;
        for (auto it = convex_edges.begin(); it != convex_edges.end(); it++){
            auto equals = [it](Edge * e2){
                bool res = (*it) -> equals(e2);
                return res;
            };
            auto is_in_form = std::find_if(edgelist.begin(), edgelist.end(),equals);
            if (is_in_form == edgelist.end()){
                std::cout<< "edge not in form, adding "<< (*it) -> print() << " to new" << std::endl;
                new_edges.push_back((*it));
            }
        }
        Edge * first_edge = edgelist.front();
        auto eequals =  [first_edge](Edge * e2){
                bool res = first_edge -> equals(e2);
                return !res;
            };
        auto iis_in_form = std::find_if(convex_edges.begin(),convex_edges.end(),eequals);
        
        while (iis_in_form == convex_edges.end()) {
            std::cout << "ROTATING" << std::endl;
            std::rotate(edgelist.begin(),edgelist.begin(),edgelist.end());
            first_edge = edgelist.front();
            std::cout << "FIRST EDGE :" << first_edge -> print() << std::endl;
            auto eequals =  [first_edge](Edge * e2){
                bool res = first_edge -> equals(e2);
                return !res;
            };
            iis_in_form = std::find_if(convex_edges.begin(),convex_edges.end(),eequals); 
        }

        for (auto it = edgelist.begin(); it != edgelist.end(); it++){
            auto equals = [it](Edge * e2){
                bool res = (*it) -> equals(e2);
                return res;
            };
            auto is_in_form = std::find_if(convex_edges.begin(), convex_edges.end(),equals);
            if (is_in_form == convex_edges.end()){
                std::cout << "edge not in convex env, adding " << (*it) -> print() <<" to old" << std::endl;
                if (old_edges.size() == 0){
                    std::cout << "size is 0 : first time" << std::endl;
                    std::vector<Edge *> first_group {(*it)};
                    old_edges.push_back(first_group);
                } else {
                    std::cout << "back size before " << old_edges.back().size() << std::endl;
                    old_edges.back().push_back((*it));
                    std::cout << "back size after " << old_edges.back().size() << std::endl;
                }
            } else {
                if (old_edges.size() != 0 && old_edges.back().size() != 0){
                    std::vector<Edge *> new_group {};
                    std::cout << "old size =" << old_edges.size() << std::endl;
                    old_edges.push_back(new_group);
                    std::cout << "new size =" << old_edges.size() << std::endl;
                }
            }
        }
        if (old_edges.back().size() == 0){
            std::cout << "popping back" << std::endl;
            old_edges.pop_back();
        }
 
        std::cout<< "old_edges = [" << std::endl;
        for (auto e = old_edges.begin(); e != old_edges.end(); e++){
            std::cout << "[ " << std::endl;
            for (auto ee = (*e).begin(); ee != (*e).end(); ee++){
                std::cout << (*ee) -> print() << std::endl;
            }
            std::cout << " ]" << std::endl;
        }
        std::cout << "]" << std::endl;


        for (auto it = new_edges.begin(); it != new_edges.end(); it++){
            std::vector<Edge *> new_form_edges;
            std::vector<Edge *> my_old_edges;
            bool needs_swap = false;
            auto my_group = std::find_if(old_edges.begin(),old_edges.end(), [it,needs_swap](std::vector<Edge *> old) mutable {
                Edge * my_first_edge = old.front();
                Edge * my_last_edge = old.back();
                
                std::cout << "group size = " << old.size() << "\n group = [" << std::endl;
                for (auto e = old.begin(); e != old.end(); e++){
                    std::cout << (*e) -> print() << std::endl; 
                }
                
                std::cout << "]\nfirst_e:" << my_first_edge -> print()
                << "\nlast_e" << my_last_edge -> print()
                << "\nme" << (*it) -> print() << std:: endl;
                
                std::array<Point *,2> first_pts = my_first_edge ->getPoints();
                std::array<Point *,2> last_pts = my_last_edge ->getPoints();
                std::array<Point *,2> my_pts = (*it) ->getPoints();
                if (first_pts.at(0) == my_pts.at(0) && last_pts.at(1) == my_pts.at(1)){
                    std::cout << "corresponding group but inversed, swapping" << std::endl;
                    needs_swap = true;
                    return true;

                } else if (first_pts.at(0) == my_pts.at(1) && last_pts.at(1) == my_pts.at(0)) {
                    std::cout << "corresponding group" << std::endl;
                    return true;

                } else {
                    std::cout << "not corresponding, sorry" << std::endl;
                    return false;
                }
            });
            if (my_group == old_edges.end()){std::cout << "end reached ! wtf ???" <<std::endl;}
            my_old_edges = *my_group;
            Edge * first_edge = new Edge(*it);
            if (needs_swap){
                std::cout << "needs swap" << std::endl;
                first_edge -> swapPoints();
            }
            new_form_edges.push_back(first_edge);
            bool is_closed = false;
            bool is_first_try = true;
            while(!is_closed){
                auto next_edge = std::find_if(my_old_edges.begin(),my_old_edges.end(),[new_form_edges](Edge * e){
                    std::cout << "new_form_edges.size =" << new_form_edges.size() << std::endl;
                    Edge * last_edge = new_form_edges.back();
                    std::cout << "last_edge = " << last_edge -> print() << std::endl;
                    bool conn_to = last_edge -> isConnectedTo(e);
                    bool is_equal = e -> equals(last_edge);
                    return (conn_to && !is_equal);
                });
                if (next_edge==my_old_edges.end() && is_first_try){
                    std::cout << "next edge not found but first try so we swap & try again" << std::endl;
                    is_first_try = false;
                    Edge * last_edge = new_form_edges.back();
                    last_edge -> swapPoints();
                } else if (next_edge==my_old_edges.end()) {
                    std::cout << "next edge not found" << std::endl;
                    is_closed = true;
                } else {
                    std::cout << "pushing to new_form_edges" << std::endl;
                    is_first_try = false;
                    new_form_edges.push_back(*next_edge);
                    my_old_edges.erase(next_edge);
                }
            }
            Form residue(new_form_edges);
            std::cout << "residue = " << residue.print() << std::endl;
            
            std::vector<std::vector<Form *>> residue_decompo = residue.convexDecomposition(); // = [[f1,f2],[f3]]
            std::cout << "residue decompo" << std::endl;
            for (auto itt = residue_decompo.begin(); itt != residue_decompo.end(); itt++){ // *itt = [f1,f2]
                int index = itt - residue_decompo.begin();
                if (result.size() <= index + 1 ) { // list not that long yet
                    std::cout << "result size is " << result.size() << "and index is " << index << std::endl;
                    result.push_back(*itt); // result = [[...],[...]].push_back([f1,f2])
                } else {
                    std::vector<Form *> new_convex_layer = result.at(index + 1);
                    std::cout << "old size" << new_convex_layer.size() << std::endl;
                    new_convex_layer.insert(new_convex_layer.end(),(*itt).begin(),(*itt).end());
                    std::cout << "new size" << new_convex_layer.size() << std::endl;
                    result.at(index + 1) = new_convex_layer;
                }
            }
        }
        for (auto res = result.begin(); res != result.end(); res++){
            int index = res - result.begin();
            std::cout << "layer " << index << ": " << std::endl;
            for (auto ress = (*res).begin(); ress != (*res).end(); ress ++){
                int iindex = ress - (*res).begin();
                std::cout << "f" << iindex << ": " << (*ress) -> print() << std::endl;
            }
        }

        return result;
    }
};
std::string Form::print(){
    std::string res = "%{\n";
    for (auto it = edgelist.begin(); it != edgelist.end(); it++){
        int index = it - edgelist.begin();
        std::string e_str = (*it)-> print();
        res = res + "e" + std::to_string(index) + " = " + e_str + "\n";
    }
    res = res + "}%\n";
    return res;
};
Form::Form(std::vector<Edge *> edges){
    edgelist = edges;
    
};
