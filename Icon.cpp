#include "Icon.h"

Icon::Icon(int state, float x, float y)
{
	this->textureHappy.loadFromFile("images/happiness.png"); 
	this->textureSad.loadFromFile("images/sad.png"); 
	this->textureCool.loadFromFile("images/cool.png"); 
	if (state == 1)
		this->sprite.setTexture(this->textureCool);
	else if(state == 0)
		this->sprite.setTexture(this->textureSad);
	else
		this->sprite.setTexture(this->textureHappy);
	this->sprite.setColor(sf::Color::White); 
	this->sprite.setPosition(x, y);
}

Icon::~Icon()
{
}

void Icon::update(int state)
{
	if (state == 0)
		this->sprite.setTexture(this->textureSad); 
	else if(state == 1)
		this->sprite.setTexture(this->textureCool);
	else
		this->sprite.setTexture(this->textureHappy);
}

void Icon::render(sf::RenderTarget* target)
{
	target->draw(this->sprite);
}
