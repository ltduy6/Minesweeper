#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <ctime>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

class Stopwatch
{
private:
	sf::RectangleShape shape;
	sf::Sprite sprite; 
	sf::Texture texture; 
	sf::Font font;
	sf::Text text;
	std::string timer;

	sf::Clock clock; 
	sf::Time elapsedTime;
public:
	Stopwatch(std::string time, float x, float y); 
	~Stopwatch();

	std::string getTimer(); 
	std::string sumStrings(std::string s1, std::string s2) const; 
	void update(bool running); 
	void render(sf::RenderTarget* target);
};

