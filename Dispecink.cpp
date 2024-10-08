#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>

const double PI = 3.1415926;
void drawArc(sf::RenderWindow* window, int x, int y, int r, double start, double len){
    std::vector<sf::Vertex> vert;
    for(double i = 0; i <= len; i += len / 10){
        vert.push_back(sf::Vertex(sf::Vector2f(x + r * cos((start - len * (i / len)) / 180.f * PI), y + r * sin((start - len * (i / len)) / 180.f * PI))));
    }
    window->draw(&vert[0], vert.size(), sf::LineStrip);
}

const int wwidth = 800, wheight = 600;
int offsetX = 400, offsetY = 300;
int scale = 50;
int counter = 1;
class Track{
public:
    enum Type{STRAIGHT=1,CURVE=2};
    int width, height;
    int startId;
    Type type;
    Track(){}
    Track(Type type, int width, int height, int startId){
        this->type = type;
        this->width = width;
        this->height = height;
        this->startId = startId;
    }
};

std::map<int,Track> tracks;

int main(){
    sf::Font font;
    if(!font.loadFromFile("font.ttf")){
        std::cerr << "Could not load font 'font.ttf'" << std::endl;
        exit(-2);
    }
    sf::RenderWindow window(sf::VideoMode(wwidth, wheight), "Dispecink");
    window.setFramerateLimit(30);
    tracks = std::map<int,Track>();
    tracks[0] = Track(Track::Type::STRAIGHT, 3, 0, -1);//node 0
    tracks[1] = Track(Track::Type::CURVE, 3, 3, 0);
    tracks[2] = Track(Track::Type::STRAIGHT, 0, 3, 0);

    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
            } else if(event.type == sf::Event::KeyPressed){
                char key = event.key.code;
                if(key == sf::Keyboard::Q){
                    window.close();
                }
            } else if(event.type == sf::Event::MouseMoved){
                int mx = event.mouseMove.x, my = event.mouseMove.y;
            }
        }

        window.clear(sf::Color::Black);
        for(const auto& t : tracks){
            int x = 0, y = 0;
            Track t2 = t.second;
            while(t2.startId != -1){
                t2 = tracks[t2.startId];
                x += t2.width;
                y += t2.height;
            }
            x = x * scale + offsetX;
            y = y * scale + offsetY;
            if(t.second.type == Track::Type::STRAIGHT){
                sf::Vertex vert[] = {sf::Vertex(sf::Vector2f(x,y)),sf::Vertex(sf::Vector2f(x+t.second.width*scale,y+t.second.height*scale))};
                window.draw(vert, 2, sf::LineStrip);
            }else if(t.second.type == Track::Type::CURVE){
                drawArc(&window, x, y - t.second.height * scale, t.second.width * scale, 90, 90);
            }
        }
        //drawArc(&window, 400, 300, 100, 0, 180);
        window.display();
    }

    return 0;
}