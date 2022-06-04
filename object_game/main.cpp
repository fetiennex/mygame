#include <SFML/Graphics.hpp>
#include "./utils.h"
#include "./world.hpp"
#include "./point.hpp"
#include "./edge.hpp"
#include "./form.hpp"
#include "./player.hpp"
#include <iostream>
/* 
struct player {
    Point * pos;
    Point * v;
    Point * a;
    double m;
}; */
void push_back_value(std::array<bool,2> * bools, bool val){
    std::cout << "pushing back value, bools before = ["
    << (*bools).at(0) << "," << (*bools).at(1) << "]" << std::endl;
    std::rotate((*bools).begin(),(*bools).begin()+1,(*bools).end());
    (*bools).at(1) = val;
    std::cout << "pushing back value, bools after = ["
    << (*bools).at(0) << "," << (*bools).at(1) << "]" << std::endl;
}

int main()
{
    int width = 960;
    int height = 1080;
    int x_obs = (width / 2);
    int y_obs = (height/2);
    double standard_d = 10;
    double standard_m = 1;
    
    bool first_point = true;

    sf::RenderWindow window(sf::VideoMode(width, height), "SFML works!");
    World* world = new World();
    Point * curr_pointer = world -> addPoint(0,0);

    Point observer(x_obs,y_obs);
    Point v0(0,0);
    Point a0(0,0);
    double vlim = 5e-2;
    double standard_a = 5e-5;
    Player me(&observer,&v0,&a0,vlim,standard_a,standard_d);

    // std::array<bool,2> up = {false,false};
    // std::array<bool,2> left = {false,false};
    // std::array<bool,2> down = {false,false};
    // std::array<bool,2> right = {false,false};
    // std::array<std::array<bool,2>, 4> directions = {left,right,up,down};
    directions dirs;
    dirs.left = false;
    dirs.right = false;
    dirs.up = false;
    dirs.down = false;

    

    std::vector<Point> collisions;
    Point * last_pointer;
    std::vector<Edge *> my_edgelist;



    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                window.close();
            }
            
            if (event.type == sf::Event::MouseMoved)
            {
                //std::cout << "Mouse moved !" << std::endl;
                //mouse x,y in referential centered on screen

                int new_x = event.mouseMove.x; //- x_obs;
                int new_y = height - event.mouseMove.y;// - y_obs;
                curr_pointer -> update(new_x,new_y);
            }
            if(event.type == sf::Event::MouseButtonPressed){
                std::cout << "Button pressed: " << std::endl;
                if (event.mouseButton.button == sf::Mouse::Left){
                    std::cout << "Left : add edge" << std::endl;
                    if (first_point){
                        Point * new_pointer = world -> addPoint(curr_pointer);
                        last_pointer = &(*new_pointer);
                        std::array<double,2> coords = new_pointer -> getCartesian();
                        std::cout << "new point pos : " << coords.at(0) << "," << height - coords.at(1) << std::endl;
                        std::cout << "desactivating first point" << std::endl;
                        first_point = false;
                    } else {
                        Point * new_pointer = world -> addPoint(curr_pointer);
                        Edge * my_edge = new Edge(last_pointer,new_pointer);
                        std::array<double,2> coords = new_pointer -> getCartesian();
                        std::cout << "new point pos : " << coords.at(0) << "," << height - coords.at(1) << std::endl;
                        my_edgelist.push_back(my_edge);
                        last_pointer = &(*new_pointer);
                    }

                    

                } else if (event.mouseButton.button == sf::Mouse::Right){
                    std::cout << "Right : try collision" << std::endl;
                    Edge visee(&observer,curr_pointer);
                    std::vector<Edge *> all_edges = world -> allEdges();                    
                    for (auto it = all_edges.begin(); it != all_edges.end(); it++){
                        try{
                            Point collision = visee.collide(*it);
                            collisions.push_back(collision);
                        } catch (MissingException& e){
                            std::cout << "Missing !" << std::endl;
                        }
                    }

                } else if (event.mouseButton.button == sf::Mouse::Middle){
                    std::cout << "Middle : finish form" << std::endl;
                    Edge * first_edge = my_edgelist.front();
                    Edge * last_edge = my_edgelist.back();
                    std::array<Point *,2> first_points = first_edge -> getPoints();
                    std::array<Point *,2> last_points = last_edge -> getPoints();
                    Point * my_first_point = first_points.at(0);
                    Point * my_last_point = last_points.at(1);
                    Edge * closing_edge = new Edge(my_last_point,my_first_point);
                    my_edgelist.push_back(closing_edge);
                    Form * new_form = world-> addForm(my_edgelist);
                    std::cout << "new form =" << new_form -> print() << std::endl;
                    //new_form -> setConvexDecomp();
                    new_form -> setTriangleDecomp();
                    my_edgelist.clear();
                    first_point = true; 
                }
            }
            else if (event.type == sf::Event::KeyPressed){
                if(event.key.code == 16){ //q
                    dirs.left = true;
                } else if (event.key.code == 3) {//d
                    dirs.right = true;
                } else if (event.key.code == 18) {//s
                    dirs.down = true;
                } else if (event.key.code == 25) {//z
                    dirs.up = true;
                }
                
            }
            else if (event.type == sf::Event::KeyReleased){
                if(event.key.code == 16){ //q
                    dirs.left = false;
                } else if (event.key.code == 3) {//d
                    dirs.right = false;
                } else if (event.key.code == 18) {//s
                    dirs.down = false;
                } else if (event.key.code == 25) {//z
                    dirs.up = false;
                }
            }
        }
        // accel application
        /* std::array<double,2> v_coords = me.v -> getCartesian();
        if(left.at(1)){
            //q
            if (v_coords.at(0) > -vlim){
                //std::cout << "vx > -vlim" << std::endl;
                Point a(standard_a,0);
                me.a -> translate(&a);
            } else {
                //std::cout << "vx <= -vlim" << std::endl;
                //me.a -> update(0,0);
            }
            //Point p(-1,0);
            //world -> translateAll(&p);
        } else if (left.at(0)){
            if (v_coords.at(0) < 0){
                double diff = v_coords.at(0) +standard_a;
                std::cout << "left released, diff = " << diff << std::endl;
                Point a(0,0);
                if (diff > 0){
                    a.update(v_coords.at(0),0);
                } else {
                    a.update(-standard_a,0);
                }
                std::cout << "a = " << a.print() << std::endl;
                me.a -> translate(&a);
            } else {
                std::cout << "ending left release" << std::endl;
                push_back_value(&left,false);
            }
        }

        if(right.at(1)){
            //d
            if (v_coords.at(0) < vlim){
                //std::cout << "vx < vlim" << std::endl;
                Point a(-standard_a,0);
                me.a -> translate(&a);
            } else {
                //std::cout << "vx >= vlim" << std::endl;
                //me.a -> update(0,0);
            }
            //Point p(1,0);
            //world -> translateAll(&p);
        } else if (right.at(0)){
            if (v_coords.at(0) > 0){
                double diff = v_coords.at(0) -standard_a;
                std::cout << "right released, diff = " << diff << std::endl;
                Point a(0,0);
                if (diff < 0){
                    a.update(v_coords.at(0),0);
                } else {
                    a.update(standard_a,0);
                }
                std::cout << "a = " << a.print() << std::endl;
                me.a -> translate(&a);
            } else {
                std::cout << "ending right release" << std::endl;
                push_back_value(&right,false);
            }
        }

        if (down.at(1)){
            //s
            if (v_coords.at(1) > -vlim){
                //std::cout << "vy > -vlim" << std::endl;
                Point a(0,standard_a);
                me.a -> translate(&a);
            } else {
                //std::cout << "vy <= -vlim" << std::endl;
                //me.a -> update(0,0);
            }
            //Point p(0,-1);
            //world -> translateAll(&p);
        } else if (down.at(0)){
            if (v_coords.at(1) < 0){
                double diff = v_coords.at(1) +standard_a;
                std::cout << "down released, diff = " << diff << std::endl;
                Point a(0,0);
                if (diff > 0){
                    a.update(0,v_coords.at(1));
                } else {
                    a.update(0,-standard_a);
                }
                std::cout << "a = " << a.print() << std::endl;
                me.a -> translate(&a);
            } else {
                std::cout << "ending down release" << std::endl;
                push_back_value(&down,false);
            }
        }

        if (up.at(1)){
            //z
            if (v_coords.at(1) < vlim){
                //std::cout << "vy < vlim" << std::endl;
                Point a(0,-standard_a);
                me.a -> translate(&a);
            } else {
                //std::cout << "vy >= vlim" << std::endl;
                //me.a -> update(0,0);
            }
            //Point p(0,1);
            //world -> translateAll(&p);
        } else if (up.at(0)){
            if (v_coords.at(1) > 0){
                double diff = v_coords.at(1) -standard_a;
                std::cout << "up released, diff = " << diff << std::endl;
                Point a(0,0);
                if (diff < 0){
                    a.update(0,v_coords.at(1));
                } else {
                    a.update(0,standard_a);
                }
                std::cout << "a = " << a.precisePrint() << std::endl;
                me.a -> translate(&a);
                
            } else {
                std::cout << "ending up release" << std::endl;
                push_back_value(&up,false);
            }
        } */
        // movement calcultion
        // v(t+dt) = v(t) + a
        //Point a_opposite = me.a -> opposite();
        //std::cout << " a = " << me.a -> precisePrint() << std::endl;
        //me.v -> translate(&a_opposite);
        //me.a -> update(0,0);
        me.go(dirs);
        me.step();
        Point * my_v  = me.getV();

        world -> translateAll(my_v);

        //render
        window.clear();
        std::vector<Edge *> all_edges = world -> allEdges();
        all_edges.insert(all_edges.end(),my_edgelist.begin(),my_edgelist.end());
        for (auto it = all_edges.begin(); it != all_edges.end(); it ++){ //draw edges
            std::array<Point *,2> points = (*it) -> getPoints();
            std::array<double,2> coordsA = points.at(0) -> getCartesian();
            std::array<double,2> coordsB = points.at(1) -> getCartesian();
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(coordsA.at(0), height - coordsA.at(1))),
                sf::Vertex(sf::Vector2f(coordsB.at(0), height - coordsB.at(1)))
            };
            window.draw(line, 2, sf::Lines);
        }
        std::vector<Form *> all_forms = world -> allForms();

        for (auto it = all_forms.begin(); it != all_forms.end(); it ++){
            std::vector<Form *> triangles = (*it) -> getTriangleDecomp();
            for (auto itt = triangles.begin(); itt != triangles.end(); itt++){
                sf::ConvexShape convex;
                std::vector<Point *> points = (*itt) -> getPoints();
                convex.setPointCount(points.size());
                //std::cout << "count set at" << points.size() << std::endl;
                int incr = 0;
                for (auto i = points.begin(); i != points.end(); i++){
                    std::array<double, 2> coords = (*i) -> getCartesian();
                    convex.setPoint(incr, sf::Vector2f(coords.at(0), height - coords.at(1)));
                    //std::cout << "set pt " << incr << "at " << coords.at(0) << "," << height - coords.at(1) << std::endl;
                    incr++;
                }
                convex.setFillColor(sf::Color::White);
                //convex.setOutlineColor(sf::Color::Yellow);
                //convex.setOutlineThickness(1);
                window.draw(convex);
            }
        }
        
        for (auto it = collisions.begin(); it != collisions.end(); it ++){ // draw collisions
            sf::CircleShape circle(standard_d);
            circle.setOrigin(standard_d,standard_d);
            circle.setFillColor(sf::Color::Red);
            std::array<double,2> coords = it -> getCartesian();
            circle.setPosition(coords.at(0),height - coords.at(1));
            window.draw(circle);
        }
        sf::CircleShape circle_obs(standard_d); // create observer
        circle_obs.setOrigin(standard_d,standard_d);
        circle_obs.setFillColor(sf::Color::Blue);
        std::array<double,2> coords_obs = observer.getCartesian();
        circle_obs.setPosition(coords_obs.at(0),height - coords_obs.at(1));
        window.draw(circle_obs);

        sf::CircleShape circle_curr(standard_d); // create cursor
        circle_curr.setOrigin(standard_d,standard_d);
        circle_curr.setFillColor(sf::Color::Green);
        std::array<double,2> coords_curr = curr_pointer -> getCartesian();
        circle_curr.setPosition(coords_curr.at(0),height - coords_curr.at(1));
        window.draw(circle_curr);

        window.display();

    }


}