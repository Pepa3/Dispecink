#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <fstream>

const double PI = 3.1415926;
void drawArc(sf::RenderWindow* window, double x, double y, double r, double start, double len){
	std::vector<sf::Vertex> vert;
	for(double i = 0; i <= len; i += 5){
		vert.push_back(sf::Vertex(sf::Vector2f(x + r * cos((start - len * (i / len)) / 180.f * PI), y + r * sin((start - len * (i / len)) / 180.f * PI))));
	}
	window->draw(&vert[0], vert.size(), sf::LineStrip);
}

const int wwidth = 800, wheight = 600;
int offsetX = 400, offsetY = 300;
double scale = 30;
int counter = 1;
class Track{
public:
	enum Type{STRAIGHT=1,CURVE=2,PARALLEL=3};
	double width, height, mod;
	int startId;
	Type type;
	Track(){}
	Track(Type type, double width, double height, int startId, double mod = 0){
		this->type = type;
		this->width = width;
		this->height = height;
		this->startId = startId;
		this->mod = mod;
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
	tracks[0] = Track(Track::Type::STRAIGHT, 0, 0, -1);//node 0

	std::ifstream in = std::ifstream("stanice");
	while(!in.eof() && !in.bad()){
		int id;
		Track t;
		in >> id >> t.startId >> (int&)t.type >> t.width >> t.height;
		if(t.type == Track::Type::CURVE) in >> t.mod;
		tracks[id] = t;
	}

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

			sf::Vertex vert[] = {sf::Vertex(sf::Vector2f(x,y)),sf::Vertex(sf::Vector2f(x + t.second.width * scale,y + t.second.height * scale))};
			switch(t.second.type){
			case Track::Type::STRAIGHT:
				window.draw(vert, 2, sf::LineStrip);
				break;
			case Track::Type::CURVE:
				drawArc(&window, x - (t.second.mod == 1 ? t.second.width * scale : 0), y - (t.second.mod == 0 ? t.second.height * scale : 0), t.second.width * scale, t.second.height * t.second.width > 0 ? 90 : 0, 90);
				break;
			case Track::Type::PARALLEL:
				double r = (abs(t.second.width) + abs(t.second.height)) / 2.f * 25.f;// i dont know why but scale-5
				double x1 = x;
				double y1 = y + (t.second.height < 0 ? -r : r);
				drawArc(&window, x1, y1, r, t.second.height * t.second.width > 0 ? -35 : 90, 55);//dont know why either - 45+10°
				double x2 = x + t.second.width * scale;
				double y2 = y + t.second.height * scale - (t.second.height < 0 ? -r : r);
				//std::cout << r << " " << x1 << " " << y1 << " " << y << " " << x2 << " " << y2 << std::endl;
				drawArc(&window, x2, y2, r, t.second.height * t.second.width > 0 ? 145 : -90, 55);
				break;
			}
		}
		window.display();
	}

	return 0;
}