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
#include "Button.h"

class Ceil
{
private:
	short unsigned buttonState;

	// Shape
	sf::Texture texture; 
	sf::Sprite sprite; 
	int size; 
	char icon; 

	// Color
	sf::Color idleColor;
	sf::Color hoverColor;
	sf::Color activeColor;
public:
	Ceil(char icon, int i, int j); 
	~Ceil();
	
	const bool isPressed() const;
	const bool isIn(const sf::Vector2f mousePos) const;
	const bool isEqual(char icon) const; 

	char getIcon() const; 

	void fillCeil(const char icon); 
	void update(const sf::Vector2f mousePos); 
	void render(sf::RenderTarget* target);
};

