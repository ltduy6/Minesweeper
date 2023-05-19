#include "Icon.h"

Icon::Icon(float x, float y)
{
	this->textureHappy.loadFromFile("images/happiness.png"); 
	this->textureSad.loadFromFile("images/sad.png"); 
	this->textureCool.loadFromFile("images/cool.png"); 
	this->sprite.setTexture(this->textureHappy); 
	this->sprite.setColor(sf::Color::White); 
	this->sprite.setPosition(x, y);
}

Icon::~Icon()
{
}

void Icon::update(bool state)
{
	if (state == false)
		this->sprite.setTexture(this->textureSad); 
	else
		this->sprite.setTexture(this->textureCool);
}

void Icon::render(sf::RenderTarget* target)
{
	target->draw(this->sprite);
}
