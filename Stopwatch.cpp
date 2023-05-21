#include "Stopwatch.h"
#include <string>

Stopwatch::Stopwatch(std::string time, float x, float y)
{
	this->shape.setPosition(sf::Vector2f(x+64, y)); 
	this->shape.setSize(sf::Vector2f(50, 64)); 

	if (!this->font.loadFromFile("Fonts/Roboto-Medium.ttf"))
		throw("ERROR");
	this->text.setFont(this->font); 
	this->timer = time;
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

std::string Stopwatch::getTimer()
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
	time = this->sumStrings(time.substr(0, 3), this->timer);
	return time;
}

std::string Stopwatch::sumStrings(std::string s1, std::string s2) const
{
	std::string res = ""; 
	int rem = 0; 
	std::reverse(s1.begin(), s1.end()); 
	std::reverse(s2.begin(), s2.end()); 
	int i = 0, j = 0; 
	while (i < s1.length() && j < s2.length())
	{
		int x = (s1[i] - '0' + s2[j] - '0' + rem) % 10; 
		rem = (s1[i] - '0' + s2[j] - '0' + rem) / 10; 
		res = res + (char)(x + '0'); 
		i++; 
		j++; 
	}
	while (i < s1.length())
	{
		int x = (s1[i] - '0' + rem) % 10;
		rem = (s1[i] - '0' + rem) / 10;
		res = res + (char)(x + '0');
		i++;
	}
	while (j < s2.length())
	{
		int x = (s2[j] - '0' + rem) % 10;
		rem = (s2[j] - '0' + rem) / 10;
		res = res + (char)(x + '0');
		j++;
	}
	while (rem)
	{
		res = res + (char)(rem % 10 + '0'); 
		rem /= 10; 
	}
	while (res.length() < 3)
		res += '0'; 
	std::reverse(res.begin(), res.end()); 
	return res; 
}

void Stopwatch::update(bool running)
{
	if(running)
	{
		this->text.setString(this->getTimer());
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
