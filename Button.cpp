#include "Button.h"

Button::Button(float x, float y, float width, float height, std::string text)
{
	this->buttonState = BTN_IDLE; 

	this->shape.setPosition(sf::Vector2f(x, y)); 
	this->shape.setSize(sf::Vector2f(width, height));

	if (!this->font.loadFromFile("Fonts/Roboto-Medium.ttf"))
		throw("ERROR");
	this->text.setFont(this->font); 
	this->nameButton = text; 
	this->text.setString(this->nameButton); 
	this->text.setFillColor(sf::Color::White); 
	this->text.setCharacterSize(30); 
	this->text.setPosition(
		this->shape.getPosition().x + (this->shape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f,
		this->shape.getPosition().y + (this->shape.getGlobalBounds().height / 2.f) - this->text.getGlobalBounds().height / 1.f
	);

	this->idleColor = sf::Color(70, 70, 70, 200);
	this->hoverColor = sf::Color(150, 150, 150, 255);
	this->activeColor = sf::Color(20, 20, 20, 200);

	this->shape.setFillColor(this->idleColor); 
}

Button::~Button()
{
}

const bool Button::isPressed() const
{
	if (this->buttonState == BTN_ACTIVE)
		return true; 

	return false;
}

const bool Button::isReleased(const sf::Vector2f mousePos) const
{
	if (this->shape.getGlobalBounds().contains(mousePos))
		return true; 
	return false;
}

const std::string Button::getText()
{
	return this->nameButton; 
}

void Button::update(const sf::Vector2f mousePos)
{
	/*Update the booleans for hover and pressed*/

	// Idle
	this->buttonState = BTN_IDLE; 

	// Hover
	if (this->shape.getGlobalBounds().contains(mousePos))
	{
		this->buttonState = BTN_HOVER; 

		// Pressed
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			this->buttonState = BTN_ACTIVE; 
		}
	}

	switch (this->buttonState)
	{
	case BTN_IDLE:
		this->shape.setFillColor(this->idleColor); 
		break; 

	case BTN_HOVER:
		this->shape.setFillColor(this->hoverColor); 
		break; 

	case BTN_ACTIVE:
		this->shape.setFillColor(this->activeColor); 
		break; 

	default:
		this->shape.setFillColor(sf::Color::Red); 
		break; 

	}
}

void Button::changeText(std::string newText)
{
	this->nameButton = newText;
	this->text.setString(newText);
	this->text.setFillColor(sf::Color::White);
	this->text.setCharacterSize(30);
	this->text.setPosition(
		this->shape.getPosition().x + (this->shape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f,
		this->shape.getPosition().y + (this->shape.getGlobalBounds().height / 2.f) - this->text.getGlobalBounds().height / 1.f
	);
}

void Button::render(sf::RenderTarget* target)
{
	target->draw(this->shape);
	target->draw(this->text);
}
