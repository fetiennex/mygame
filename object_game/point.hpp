#include <array>
#include <string>

class Form;
class Edge;

class Point {
    private:
        double x;
        double y;
        Form * parent;
    public:
        std::array<double,2> getPolar();
        std::array<double,2> getCartesian();
        Form * getParent();
        Point opposite();
        double distance(Point * p);
        void translate(double xx, double yy);
        void translate(Point * pt);
        void rotate(double theta);
        void update(double xx, double yy);
        std::string print();
        std::string precisePrint();
        bool isAbove(Edge * e);
        Point(double xx, double yy);
        Point(Point * p);
};