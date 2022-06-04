#include <array>
#include <string>

class Point;
class Edge {
    private:
        Point * pt1;
        Point * pt2;
    public:
        std::array<Point *, 2> getPoints();
        Point collide(Edge * e);
        bool equals(Edge * e);
        bool isConnectedTo(Edge * e);
        void swapPoints();
        std::string print();
        Edge(Point * ptt1, Point * ptt2);
        Edge(Edge * e);

};
