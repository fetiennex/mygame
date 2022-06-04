#include <vector>
class Point;
class Edge;
class Form;
class World {
    private:
        std::vector<Point *> all_points;
        //std::vector<Edge *> all_edges;
        std::vector<Form *> all_forms;
    public:
        //Edge * addEdge(Point * pt1, Point * pt2);
        Form * addForm(std::vector<Edge *> edges);
        Point * addPoint(int x1, int x2);
        Point * addPoint(Point * pt);

        void translateAll(Point * p);

        std::vector<Point *> allPoints();
        std::vector<Edge *> allEdges();
        std::vector<Form *> allForms();
        World();
};