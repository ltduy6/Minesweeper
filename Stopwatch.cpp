#include "Stopwatch.h"
#include <string>

Stopwatch::Stopwatch(float x, float y)
{
	this->shape.setPosition(sf::Vector2f(x+64, y)); 
	this->shape.setSize(sf::Vector2f(50, 64)); 

	if (!this->font.loadFromFile("Fonts/Roboto-Medium.ttf"))
		throw("ERROR");
	this->text.setFont(this->font); 
	this->timer = "000";
	this->text.setString(this->timer); 
	this->text.setFillColor(sf::Color::Black); 
	this->text.setCharacterSize(30); 
	this->text.setPosition(
		this->shape.getPosition().x + (this->shape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f,
		this->shape.getPosition().y + (this->shape.getGlobalBounds().height / 3.f) - this->text.getGlobalBounds().height / 4.f
	);

	this->shape.setFillColor(sf::Color::White);

	this->clock.restart();
	this->elapsedTime = sf::Time::Zero; 
	
	this->texture.loadFromFile("images/stopwatch.png");
	this->sprite.setTexture(this->texture); 
	this->sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
	this->sprite.setColor(sf::Color::White);
	this->sprite.setPosition(sf::Vector2f(x, y));
}

Stopwatch::~Stopwatch()
{
}

std::string Stopwatch::getTimer() const
{
	return this->timer.substr(0, 3); 
}

void Stopwatch::update(bool running)
{
	if(running)
	{
		this->elapsedTime += this->clock.restart();
		std::string time = std::to_string(this->elapsedTime.asSeconds()); 
		int i = 0; 
		while (std::isdigit(time[i]))
			i++; 
		if (i == 1)
			time = "00" + time;
		else if (i == 2)
			time = "0" + time;
		this->text.setString(time.substr(0, 3));
	}
	else {
		this->clock.restart();
	}
}

void Stopwatch::render(sf::RenderTarget* target)
{
	target->draw(this->sprite);
	target->draw(this->shape);
	target->draw(this->text);
}
