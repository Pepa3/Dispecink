#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <fstream>
#include <cassert>

const double PI = 3.1415926;
const int wwidth = 800, wheight = 600;

sf::Color color = sf::Color::White;
double offsetX = 200, offsetY = 300;
double scale = 20;
int counter = 1;//TODO:why, find :)

void drawArc(sf::RenderWindow* window, double x, double y, double r, double start, double len){
	std::vector<sf::Vertex> vert;
	for(double i = 0; i <= len; i += 5){
		vert.push_back(sf::Vertex(sf::Vector2f(x + r * cos((start - len * (i / len)) / 180.f * PI), y + r * sin((start - len * (i / len)) / 180.f * PI)),color));
	}
	window->draw(&vert[0], vert.size(), sf::LineStrip);
}

class Track{
public:
	enum Type{STRAIGHT=1,CURVE=2,PARALLEL=3,COUNT=3	};
	double width, height, mod;
	int startId;
	std::pair<double, double> relPos;
	Type type;
	Track(){}
	Track(Type type, double width, double height, std::pair<double, double> relPos, int startId, double mod = 0){
		this->type = type;
		this->width = width;
		this->height = height;
		this->relPos = relPos;
		this->startId = startId;
		this->mod = mod;
	}
};

Track editGhost = Track(Track::Type::CURVE, 2, 2, {}, 0);

std::map<int,Track> tracks;

std::pair<double, double> getPos(Track t){
	auto [x, y] = tracks[t.startId].relPos;
	x += tracks[t.startId].width * scale;
	y += tracks[t.startId].height * scale;
	return {x,y};
}
void render(sf::RenderWindow& window, Track t){
	auto [x, y] = t.relPos;
	int width = t.width, height = t.height, mod = t.mod;
	Track::Type type = t.type;

	sf::Vertex vert[] = {sf::Vertex(sf::Vector2f(x,y),color),sf::Vertex(sf::Vector2f(x + width * scale,y + height * scale),color)};
	switch(type){
	case Track::Type::STRAIGHT:
		window.draw(vert, 2, sf::LineStrip);
		break;
	case Track::Type::CURVE:
		drawArc(&window, x - (mod == 1 ? width * scale : 0), y + (mod == 0 ? height * scale : 0), width * scale, (mod*2-1)* height * width > 0 ? 90 : 0, 90);
		break;
	case Track::Type::PARALLEL://TODO: shrink
		double r = (abs(width) + abs(height)) / 2.f * scale * 5 / 6;// i dont know why but scale-5
		if(mod == 0){//horizontal
			double x1 = x;
			double x2 = x + width * scale;
			if(width > 0){
				double y1 = y + (height < 0 ? -r : r);
				double y2 = y + height * scale - (height < 0 ? -r : r);
				drawArc(&window, x1, y1, r, height > 0 ? -35 : 90, 55);//dont know why either - 45+10°
				drawArc(&window, x2, y2, r, height > 0 ? 145 : -90, 55);
			} else{
				double y1 = y - (height < 0 ? -r : r);
				double y2 = y + height * scale - (height < 0 ? (-r / 25 * 15) : (r / 25 * 15));
				drawArc(&window, x1, y1, r, height > 0 ? 145 : -90, 55);
				drawArc(&window, x2, y2, r, height > 0 ? -35 : 90, 55);
			}
		} else if(mod == 1){//vertical
			double y1 = y;
			double y2 = y + height * scale;
			if(height > 0){
				double x1 = x + (width < 0 ? -r : r);
				double x2 = x + width * scale - (width < 0 ? -r : r);
				drawArc(&window, x1, y1, r, width > 0 ? 180 : 55, 55);
				drawArc(&window, x2, y2, r, width > 0 ? 0 : 235, 55);
			} else{
				double x1 = x - (width < 0 ? -r : r);
				double x2 = x + width * scale - (width < 0 ? (-r / 25 * 15) : (r / 25 * 15));
				drawArc(&window, x1, y1, r, width > 0 ? 0 : 235, 55);
				drawArc(&window, x2, y2, r, width > 0 ? 180 : 55, 55);
			}
		}
		break;
	}
}

int main(){
	sf::Font font;
	if(!font.loadFromFile("font.ttf")){
		std::cerr << "Could not load font 'font.ttf'" << std::endl;
		exit(-2);
	}
	sf::RenderWindow window(sf::VideoMode(wwidth, wheight), "Dispecink");
	window.setFramerateLimit(30);
	tracks = std::map<int,Track>();
	tracks[0] = Track(Track::Type::STRAIGHT, 0, 0, {offsetX, offsetY}, -1);//node 0
	sf::Text editText = sf::Text();
	editText.setFont(font);
	editText.setCharacterSize(18);
	editText.setFillColor(sf::Color::Cyan);
	editText.setString("Width: " + std::to_string(editGhost.width) + ", Height: " + std::to_string(-editGhost.height));

	std::ifstream in = std::ifstream("stanice");
	while(!in.eof() && !in.bad()){
		int id;
		Track t;
		in >> id >> t.startId >> (int&)t.type >> t.width >> t.height;
		if(t.type == Track::Type::CURVE || t.type == Track::Type::PARALLEL) in >> t.mod;
		t.relPos = getPos(t);
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
				} else if(key == sf::Keyboard::Right){
					editGhost.width++;
				} else if(key == sf::Keyboard::Left){
					editGhost.width--;
				} else if(key == sf::Keyboard::Down){
					editGhost.height++;
				} else if(key == sf::Keyboard::Up){
					editGhost.height--;
				}
				editText.setString("Width: "+std::to_string(editGhost.width) +", Height: " + std::to_string(-editGhost.height));
			} else if(event.type == sf::Event::MouseMoved){
				double mx = event.mouseMove.x, my = event.mouseMove.y;
				unsigned best = -1;
				for(const auto& t : tracks){
					Track t2;
					t2.startId = t.first;
					auto [x, y] = getPos(t2);
					double dist = abs(x-mx)+abs(y-my);
					if(dist < best){
						editGhost.startId = t.first;
						editGhost.relPos = getPos(editGhost);
						best = dist;
					}
				}
			} else if(event.type == sf::Event::MouseButtonPressed){
				if(event.mouseButton.button == sf::Mouse::Left){
					
				} else if(event.mouseButton.button == sf::Mouse::Right){
					(*((int*) &editGhost.type))++;
					if(editGhost.type > Track::Type::COUNT)editGhost.type = Track::Type::STRAIGHT;
					switch(editGhost.type){
					case Track::Type::STRAIGHT:
						editGhost.width = 3;
						editGhost.height = 0;
						break;
					case Track::Type::CURVE:
						editGhost.width = 3;
						editGhost.height = 3;
						break;
					case Track::Type::PARALLEL:
						editGhost.width = 4;
						editGhost.height = 2;
						break;
					default:
						assert(false);
					}
					editText.setString("Width: " + std::to_string(editGhost.width) + ", Height: " + std::to_string(-editGhost.height));
				}
			}
		}
		window.clear(sf::Color::Black);
		for(const auto& t : tracks){
			render(window,t.second);
		}
		color = sf::Color::Cyan;
		render(window,editGhost);
		color = sf::Color::White;
		window.draw(editText);
		window.display();
	}

	return 0;
}