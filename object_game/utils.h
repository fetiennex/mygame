#include <array>
#include <exception>
#include <cmath>
#include <algorithm>

class Point;
double fromPiToMinusPi(double angle);
struct linear_function {
    std::array<double, 2> y; // Y=y.at(0)*X + y.at(1)
    int x; // case X = cste
    bool isx;
};
struct coords_bool {
    int y;
    bool who;
};
struct directions {
    bool left;
    bool right;
    bool up;
    bool down;
};

struct MissingException : public std::exception{
    const char * what () const throw (){
        return "Edges are missing eachother";
    }
};
struct JoinedException : public std::exception{
    const char * what () const throw (){
        return "Edges are joined. Pick a point knowing that";
    }
};
linear_function create_linear_function(int x1,int y1,int x2, int y2);
linear_function create_linear_function(Point * ptA, Point * ptB);
bool joined_check_if_inter(int yy1, int yy2, int yy3, int yy4);
