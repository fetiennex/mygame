#include <vector>
#include <string>

class Point;
class Edge;

class Form {
    private:
        std::vector<Edge *> edgelist;// = [e1,e2,e3,e4,e5]
        std::vector<std::vector<Form *>> convex_decomposition;
        std::vector<Form *> triangle_decomposition;
    public:
        std::vector<Point *> getPoints();
        std::vector<Edge *> getEdges();
        Edge * getNextEdge(Point * summit, Edge * previous_edge);
        
        std::vector<Form *> triangleDecomposition();
        void setTriangleDecomp();
        std::vector<Form *> getTriangleDecomp();

        std::vector<std::vector<Form *>> convexDecomposition();
        Form * getConvexEnv();
        void setConvexDecomp();
        std::vector<std::vector<Form *>> getConvexDecomp();

        std::string print();
        void cycleEdges();
        bool isAbove();
        Form(std::vector<Edge *> edges);
};