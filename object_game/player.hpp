#include <array>
#include <string>

class Point;
struct directions;

class Player {
    private:
        Point * pos;
        Point * v;
        Point * a;
        double vlim;
        double max_a;
        double radius;
    public:
        void go(directions directions);
        void step();
        Point * getV();
        Point collideWall(Edge * e, bool is_above);
        Player(Point * pos0, Point * v0, Point * a0, double vlim, double max_a, double radius);
};