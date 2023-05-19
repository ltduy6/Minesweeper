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

class Flag
{
private:
	sf::Sprite sprite;
	sf::Texture texture;
	sf::Font font;
	sf::Text text;

	int numFlags; 
public:
	Flag(float x, float y, int numFlags); 
	~Flag(); 

	void update(int numFlags); 
	void render(sf::RenderTarget* target); 
};

