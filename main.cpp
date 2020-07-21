#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

sf::ConvexShape createArea(float angle, float distance){

    sf::ConvexShape area;
    sf::CircleShape circle(distance);
    // std::cout << "cpointcount" << circle.getPointCount() << std::endl; 

    
    float alpha_t = (2*M_PI)/200;
    int i = 0;
    int areacount = static_cast<int>(angle/ alpha_t) + 2;
    area.setPointCount(areacount);
    // std::cout << "area count" << areacount << std::endl;

    area.setPoint( 0, sf::Vector2f(0, 0));

    while ( i < areacount - 1)
    {   
        area.setPoint( i+1, sf::Vector2f(distance*cos(i*alpha_t), distance*sin(i*alpha_t)));
        // std::cout << "area point x=" << area.getPoint(i).x << " , y=" << area.getPoint(i).y << std::endl;

        i++;
    }
    area.setRotation(angle/2);
    
    return area;
}

float getAngle(float xa, float xb, float ya, float yb){
    return atan2(yb-ya,xb-xa);
}



int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "SFML works!");
    sf::ConvexShape area1 = createArea(M_PI/4, 100);
    sf::ConvexShape area2 = createArea(M_PI/4, 100);
    sf::CircleShape circle(30.f);
    sf::CircleShape circle2(30.f);
    circle.setFillColor(sf::Color::Red);
    circle2.setFillColor(sf::Color::Cyan);
    area1.setPosition(400,400);
    area2.setPosition(400,400);
    area2.setFillColor(sf::Color::Green);
    circle.setOrigin(30.f,30.f);
    circle2.setOrigin(30.f,30.f);
    circle.setPosition(400,400);
    // area1.setPosition(sf::Vector2f(-100,0));
    // sf::ConvexShape area2 = createArea(M_PI/2, 50);
    // std::cout << "area count=" << area2.getPointCount() << std::endl;
    // area2.setPosition(sf::Vector2f(+100,0));
    // sf::ConvexShape area3 = createArea(M_PI/3, 150);
    // std::cout << "area count=" << area3.getPointCount() << std::endl;
    // area3.setPosition(sf::Vector2f(0,+100));
    // sf::ConvexShape area4 = createArea(M_PI/3, 350);
    // std::cout << "area count=" << area3.getPointCount() << std::endl;
    // area4.setPosition(sf::Vector2f(100,100));


    
    float teta = 0.f;
    float x_player = circle.getPosition().x;
    float y_player = circle.getPosition().y;
    float x_cible = circle2.getPosition().x;
    float y_cible = circle2.getPosition().y;
    int touche = 0;
    

    while (window.isOpen())
    {
        x_player = circle.getPosition().x;
        y_player = circle.getPosition().y;
        x_cible = circle2.getPosition().x;
        y_cible = circle2.getPosition().y;

        circle2.setFillColor(sf::Color::Cyan);
        if (touche>0){
            circle2.setFillColor(sf::Color::Yellow);
            touche--;
        }
        

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            
            if (event.type == sf::Event::MouseMoved)
            {
            
            
                float dist = std::sqrt((event.mouseMove.x - 400)*(event.mouseMove.x - 400) + (event.mouseMove.y - 400)*(event.mouseMove.y - 400) );
                if (dist > 30.f)
                {
                    teta = asin(30.f/dist);
                    area2 = createArea((M_PI/4) + 2 * teta, 100);
                    area2.setPosition(400,400);
                    area2.setFillColor(sf::Color::Green);
                }
                
                
                // std::cout << "angle" << (360.f / (2* M_PI)) * getAngle(400, event.mouseMove.x,400,event.mouseMove.y) << std::endl;
                // std::cout << "dist" << dist << std::endl;
                // std::cout << "teta" << teta << std::endl;
            
                area1.setRotation( (360.f / (2* M_PI)) * (getAngle(400, event.mouseMove.x,400,event.mouseMove.y) - M_PI/8));
                area2.setRotation( (360.f / (2* M_PI)) * (getAngle(400, event.mouseMove.x,400,event.mouseMove.y)- ((M_PI/4) + 2 * teta)/2));
                // shape.setPosition(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
                // std::cout << area1.getPointCount() << std::endl;

                // area1.setPosition(sf::Vector2f(event.mouseButton.x-100 ,event.mouseButton.y));
                // area2.setPosition(sf::Vector2f(event.mouseButton.x +100, event.mouseButton.y));
                // area3.setPosition(sf::Vector2f(event.mouseButton.x, event.mouseButton.y+100));
                // circle2.setPosition(sf::Vector2f(400,event.mouseMove.y));

                
            }
            if (event.type == sf::Event::MouseButtonPressed){
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    float dist = std::sqrt((x_cible - x_player)*(x_cible - x_player) + (y_cible - y_player)*(y_cible - y_player) );
                    float angle = getAngle(x_player, x_cible, y_player, y_cible)*(180.f / M_PI);
                    if(angle<0){
                        angle += 360;
                    }
                    std::cout << "angle:" << fmod(angle,360.f) << std::endl;
                    float teta = 0;
                    float alpha_0 = area1.getRotation();
                    std::cout << "alpha_0 (avant teta):" << alpha_0 << std::endl;
                    float alpha_1 = area1.getRotation() + (180.f / M_PI)*(M_PI/4);
                    std::cout << "alpha_1 (avant teta):" << alpha_1 << std::endl;
                    if (dist < 100.f + 30.f){
                        teta = asin(30.f/dist)*(180.f /  M_PI);
                        std::cout << "teta:" << teta << std::endl;

                        alpha_0 = alpha_0 - teta;
                        if(alpha_0< 0) {
                            alpha_0 += 360;
                        }
                        std::cout << "alpha_0:" << fmod(alpha_0,360.f) << std::endl;
                        

                        alpha_1 = alpha_1 + teta;
                        if(alpha_1< 0) {
                            alpha_1 += 360;
                        }
                        std::cout << "alpha_1:" << fmod(alpha_1,360.f) << std::endl;
                        if(std::fmod(alpha_1,360.f) < std::fmod(alpha_0,360.f)){
                            if (std::fmod(angle,360.f)>std::fmod(alpha_0,360.f) || std::fmod(angle,360.f)< std::fmod(alpha_1,360.f))
                            {
                                std::cout << "touché" << std::endl;

                                touche += 100;
                            }
                            else
                            {
                                std::cout << "raté" << std::endl;

                            }
                            

                        }
                        else{
                            if (std::fmod(angle,360.f)>std::fmod(alpha_0,360.f) && std::fmod(angle,360.f)< std::fmod(alpha_1,360.f))
                            {
                            std::cout << "touché" << std::endl;
                            touche += 100;

                            }
                            else 
                            {
                                std::cout << "raté" << std::endl;

                            }

                        }
                        

                    } else {

                        std::cout << "raté" << std::endl;
                    }
                }
            }
            if(event.type == sf::Event::KeyPressed){
                if(event.key.code == sf::Keyboard::Up){
                    circle2.setPosition(sf::Vector2f(400,circle2.getPosition().y -10));
                }
                if(event.key.code == sf::Keyboard::Down){
                    circle2.setPosition(sf::Vector2f(400,circle2.getPosition().y +10));
                }
            }

        }

        window.clear();
        window.draw(area2);
        window.draw(area1);
        window.draw(circle);
        window.draw(circle2);
        // window.draw(area2);
        // window.draw(area3);
        // window.draw(area4);
        window.display();
    }

    return 0;
}
