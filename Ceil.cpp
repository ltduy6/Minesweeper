#include "Ceil.h"

Ceil::Ceil(char icon, int i, int j)
{
	this->buttonState = BTN_IDLE; 
	this->texture.loadFromFile("images/tiles.jpg");
	this->sprite.setTexture(texture);
	this->size = 32; 
	this->icon = icon; 
	this->idleColor = this->sprite.getColor(); 
	this->hoverColor = sf::Color(160, 160, 160, 255); 
	this->activeColor = this->sprite.getColor(); 

	int posGet = 0;
	switch (icon)
	{
	case '_': // empty ceil before clicking
		posGet = 10;
		break;
	case '-': // empty ceil after clicking
		posGet = 0;
		break;
	case '*': // mine ceil
		posGet = 9;
		break;
	case '#': // flag ceil
		posGet = 11;
		break;
	default: // number ceil
		posGet = icon - '0';
		break;
	}
	this->sprite.setTextureRect(sf::IntRect(posGet * this->size, 0, this->size, this->size));
	this->sprite.setScale(2, 2); 
	this->sprite.setPosition((i) * this->size * 2, (j) * this->size * 2);
	this->sprite.setColor(this->idleColor); 
}

Ceil::~Ceil()
{
}

const bool Ceil::isPressed() const
{
	if (this->buttonState == BTN_ACTIVE)
		return true;

	return false;
}

const bool Ceil::isIn(const sf::Vector2f mousePos) const
{
	if (this->sprite.getGlobalBounds().contains(mousePos))
		return true;
	return false;
}

const bool Ceil::isEqual(char icon) const
{
	return (this->icon == icon);
}

char Ceil::getIcon() const
{
	return this->icon;
}

void Ceil::fillCeil(const char icon)
{
	this->icon = icon; 

	int posGet = 0;
	switch (icon)
	{
	case '_': // empty ceil before clicking
		posGet = 10;
		break;
	case '-': // empty ceil after clicking
		posGet = 0;
		break;
	case '*': // mine ceil
		posGet = 9;
		break;
	case '#': // flag ceil
		posGet = 11;
		break;
	default: // number ceil
		posGet = icon - '0';
		break;
	}
	this->sprite.setTextureRect(sf::IntRect(posGet * this->size, 0, this->size, this->size));
	this->sprite.setScale(2, 2);
}

void Ceil::update(const sf::Vector2f mousePos)
{
	this->buttonState = BTN_IDLE; 

	if (this->sprite.getGlobalBounds().contains(mousePos))
	{
		this->buttonState = BTN_HOVER; 

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			this->buttonState = BTN_ACTIVE; 
		}
	}

	switch (this->buttonState)
	{
	case BTN_IDLE:
		this->sprite.setColor(this->idleColor); 
		break; 
	case BTN_HOVER:
		this->sprite.setColor(this->hoverColor); 
		break; 
	case BTN_ACTIVE:
		this->sprite.setColor(this->activeColor); 
		break; 
	default:
		this->sprite.setColor(sf::Color::Cyan); 
		break; 
	}
}

void Ceil::render(sf::RenderTarget* target)
{
	target->draw(this->sprite); 
}
