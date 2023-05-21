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

class Icon
{
private:
	sf::Texture textureHappy;
	sf::Texture textureSad; 
	sf::Texture textureCool;
	sf::Sprite sprite; 
public:
	Icon(int state, float x, float y); 
	~Icon(); 

	void update(int state); 
	void render(sf::RenderTarget* target); 
};

