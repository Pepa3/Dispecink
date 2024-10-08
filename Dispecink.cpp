#include <iostream>
#include <SFML/Graphics.hpp>

int wwidth = 800, wheight = 600;

int main(){
    sf::Font font;
    if(!font.loadFromFile("font.ttf")){
        std::cerr << "Could not load font 'arial.ttf'" << std::endl;
        exit(-2);
    }
    sf::RenderWindow window(sf::VideoMode(wwidth, wheight), "Dispecink");

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
        window.display();
    }

    return 0;
}