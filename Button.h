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

enum button_states{ BTN_IDLE = 0, BTN_HOVER, BTN_ACTIVE };

class Button
{
private:
	short unsigned buttonState; 

	sf::RectangleShape shape; 
	sf::Font font; 
	sf::Text text; 
	std::string nameButton; 

	sf::Color idleColor; 
	sf::Color hoverColor; 
	sf::Color activeColor; 

public:
	Button(float x, float y, float width, float height, std::string text);
	~Button(); 

	// Accessors
	const bool isPressed() const; 
	const bool isReleased(const sf::Vector2f mousePos) const;
	const std::string getText(); 

	//Functions
	void update(const sf::Vector2f mousePos); 
	void changeText(std::string newText);
	void render(sf::RenderTarget* target); 
};

