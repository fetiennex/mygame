#include "./world.hpp"
#include "./point.hpp"
#include "./edge.hpp"
#include "./form.hpp"
#include <iostream>
Point * World::addPoint(int x1, int x2){
    Point * new_point = new Point(x1,x2);
    all_points.push_back(new_point);
    return new_point;

}
Point * World::addPoint(Point* pt){
    Point * new_point = new Point(pt);
    all_points.push_back(new_point);
    return new_point;
}
/* Edge * World::addEdge(Point * pt1, Point * pt2){
    Edge * new_edge = new Edge(pt1,pt2);
    all_edges.push_back(new_edge);
    return new_edge;
} */
Form* World::addForm(std::vector<Edge *> edges){
    Form * new_form = new Form(edges);
    all_forms.push_back(new_form);
    return new_form;

};
void World::translateAll(Point * p){
    for (auto it = all_points.begin(); it != all_points.end(); it++){
        (*it) -> translate(p);
    }
}
std::vector<Point *> World::allPoints(){
    return all_points;
};
std::vector<Edge *> World::allEdges(){
    std::vector<Edge *> res;
    for (auto it = all_forms.begin(); it != all_forms.end(); it++){
        std::vector<Edge *> form_edges = (*it) -> getEdges();
        res.insert(res.end(),form_edges.begin(), form_edges.end());
    }
    return res;
};
std::vector<Form *> World::allForms(){
    return all_forms;
};
World::World(){
     all_forms = {};
     all_points = {};
 }
