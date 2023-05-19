#include "Flag.h"
#include <string>

Flag::Flag(float x, float y, int numFlags)
{
	this->numFlags = numFlags; 
	if (!this->font.loadFromFile("Fonts/Roboto-Medium.ttf"))
		throw("ERROR");
	this->text.setFont(this->font); 
	this->text.setString(std::to_string(this->numFlags));
	this->text.setFillColor(sf::Color::Black); 
	this->text.setCharacterSize(30); 
	this->text.setPosition(x + 64, y + 17);

	this->texture.loadFromFile("images/flag.png");
	this->sprite.setTexture(this->texture); 
	this->sprite.setTextureRect(sf::IntRect(0, 0, this->texture.getSize().y, this->texture.getSize().y));
	this->sprite.setColor(sf::Color::White);
	this->sprite.setPosition(x, y);
}

Flag::~Flag()
{
}

void Flag::update(int numFlags)
{
	this->text.setString(std::to_string(numFlags)); 
}

void Flag::render(sf::RenderTarget* target)
{
	target->draw(this->sprite); 
	target->draw(this->text);
}
