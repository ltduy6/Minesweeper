#include "Game.h"
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <queue>

// Init
bool Game::isContinue()
{
	std::ifstream fin("Data.txt"); 
	std::string line; 
	std::getline(fin, line, '\n'); 
	fin.close();
	return (line == "0 0");
}
void Game::loadVariable()
{
	this->window = nullptr; 
	this->texture.loadFromFile("images/tiles.jpg");
	this->sprite.setTexture(texture);
	// Game logic
	this->width = 9; 
	this->height = 9; 
	this->numMines = 10;
	this->currentIndexMove = 0;
	this->startGame = false;
	this->endGame = false;
	this->continueGame = this->isContinue(); 
	this->startMenu = true; 
	this->newGame = false;
	this->delay = false; 
	this->runningGame = false; 
	this->firstClick = false; 
	this->putFlag = false;

	if (this->continueGame)
	{
		std::ifstream fin("images/Data.txt"); 
		fin >> this->height >> this->width; 
		fin.get(); 
		std::string line; 
		for (int i = 0; i < this->height; ++i)
		{
			std::getline(fin, line, '\n'); 
			for (int j = 0; j < this->width; ++j)
				this->hideGrid[i].push_back(line[j]); 
		}
		for (int i = 0; i < this->height; ++i)
		{
			std::getline(fin, line, '\n'); 
			for (int j = 0; j < this->width; ++j)
			{
				Ceil* newCeil = new Ceil(line[j], i, j); 
				this->ceil[i].push_back(newCeil);
			}
		}
		fin.close(); 
	}
}
void Game::loadWindow()
{
	this->videoMode.height = 800; 
	this->videoMode.width = 800; 

	this->window = new sf::RenderWindow(this->videoMode, "Minesweeper"); 

	this->window->setFramerateLimit(60);

	this->scale = this->videoMode.width / (this->size * this->width); 

	this->posButton[0] = { this->videoMode.width / 4, this->videoMode.height / 4 }; 
	this->posButton[1] = { this->videoMode.width / 4, this->videoMode.height / 4 + this->videoMode.height / 16 + this->videoMode.height / 8 }; 
	this->posButton[2] = { this->videoMode.width / 4, this->posButton[1].second + this->videoMode.height / 16 + this->videoMode.height / 8 }; 
}
void Game::InitButtons()
{
	this->buttons["NEW_GAME"] = new Button(this->posButton[0].first, this->posButton[0].second, this->videoMode.width / 5, this->videoMode.height / 16,
		&this->font, "New Game",
		sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200));
	
	if (this->continueGame)
	{
		this->buttons["CONTINUE"] = new Button(this->posButton[1].first, this->posButton[1].second, this->videoMode.width / 5, this->videoMode.height / 16,
			&this->font, "Continue",
			sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200));

		this->buttons["EXIT_GAME"] = new Button(this->posButton[2].first, this->posButton[2].second, this->videoMode.width / 5, this->videoMode.height / 16,
			&this->font, "Exit",
			sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200));
	}
	else {
		this->buttons["EXIT_GAME"] = new Button(this->posButton[1].first, this->posButton[1].second, this->videoMode.width / 5, this->videoMode.height / 16,
			&this->font, "Quit",
			sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200));
	}
}
void Game::InitFont()
{
	if (!this->font.loadFromFile("Fonts/Roboto-Medium.ttf"))
		throw("ERROR"); 
}
void Game::InitStopWatch()
{
	this->stopwatch = new Stopwatch(this->width * this->size * 2 - 64 - 50, this->height * this->size * 2);
}
void Game::InitFlag()
{
	this->flag = new Flag(0, this->height * this->size * 2, this->numMines);
}
void Game::InitNewWindow()
{
	delete window; 
	this->videoMode.height = this->height * this->size * 2 + this->size * 2 + 1; 
	this->videoMode.width = this->width * this->size * 2;

	this->window = new sf::RenderWindow(this->videoMode, "Minesweeper");

	this->window->setFramerateLimit(60);
}
void Game::InitIcon()
{
	this->icon = new Icon((this->width * this->size * 2) / 2 - this->size, this->height * this->size * 2);
}
// Create grid
void Game::generateGrid()
{
	// empty ceil
	for (int i = 0; i < this->height; ++i)
	{
		std::vector<Ceil*> newVector; 
		std::string lineHide = ""; 
		std::string lineShow = ""; 
		for (int j = 0; j < this->width; ++j)
		{
			lineHide += '-'; 
			lineShow += '_'; 
			Ceil* newCeil = new Ceil('_', i, j);
			newVector.push_back(newCeil);
		}
		this->ceil.push_back(newVector);
		this->hideGrid.push_back(lineHide); 
		this->showGrid.push_back(lineShow);
	}
}
void Game::placeMines(int row, int col)
{
	// Place mines
	for (int i = 0; i < this->numMines; )
	{
		int random = rand() % (this->width * this->height);
		int x = random % this->width;
		int y = random / this->height;
		int distance = (y - row) * (y - row) + (x - col) * (x - col); 

		if (this->hideGrid[y][x] != '*' && distance != 2 && distance != 1 && distance != 0)
		{
			this->hideGrid[y][x] = '*';
			i++;
		}
	}
}
int Game::countAdjacentMines(int i, int j)
{
	int count = 0;
	for (int x = i - 1; x <= i + 1; x++) {
		for (int y = j - 1; y <= j + 1; y++) {
			if (x < 0 || y < 0 || x >= this->height || y >= this->width || (x == i && y == j))
				continue;
			else if (hideGrid[x][y] == '*') {
				count++;
			}
		}
	}
	return count;
}
bool Game::isValid(int row, int col)
{
	return (row >= 0 && row < this->height && col >= 0 && col < this->width);
}
bool Game::isMine(int row, int col)
{
	return (this->hideGrid[row][col] == '*');
}
void Game::fillGrid()
{
	for (int i = 0; i < this->height; ++i)
	{
		for (int j = 0; j < this->width; ++j)
		{
			if (this->hideGrid[i][j] != '*')
			{
				if(this->countAdjacentMines(i, j))
					this->hideGrid[i][j] = (char) (this->countAdjacentMines(i, j) + '0'); 
			}
		}
	}
}

Game::Game()
{
	this->loadVariable(); 
	this->loadWindow(); 
	this->InitFont();
	this->InitButtons();
}

Game::~Game()
{
	for (auto x : this->buttons)
		delete x.second; 

	if (!this->startMenu)
	{
		for (int i = 0; i < this->height; ++i)
			for (int j = 0; j < this->width; ++j)
				delete ceil[i][j];
	}

	delete this->stopwatch;
	delete this->flag;
	delete this->icon;
	delete this->window; 
}

const bool Game::running() const
{
	return this->window->isOpen();
}

const bool Game::EndGame() const
{
	return this->endGame;
}

const bool Game::WinGame() const
{
	for(int i = 0; i < this->height; ++i)
		for (int j = 0; j < this->width; ++j)
		{
			if (this->ceil[i][j]->isEqual('#') && this->hideGrid[i][j] != '*')
				return false; 
			if (this->ceil[i][j]->isEqual('_'))
				return false; 
		}
	return true;
}

// Updates
void Game::update()
{
	this->pollEvents();
	this->updateMousePosition();

	if (this->startMenu)
		this->updateButtons();
	else
	{
		if (this->delay)
		{
			this->updateGrid();
			if (this->WinGame() == true)
			{
				this->runningGame = false; 
				this->icon->update(true);
			}
			this->stopwatch->update(this->runningGame);
			this->flag->update(this->numMines);
		}
	}
}
void Game::pollEvents()
{
	while (this->window->pollEvent(this->event))
	{
		switch (this->event.type)
		{
		case sf::Event::Closed:
			this->saveGame();
			this->deleteGame();
			this->window->close(); 
			break; 
		case sf::Event::KeyPressed:
			if (this->event.key.code == sf::Keyboard::Escape)
				this->window->close(); 
			break;
		case sf::Event::MouseButtonReleased:
			if (this->newGame) {
				if (this->buttons["EASY"]->isReleased(this->moustPosView) ||
					this->buttons["MEDIUM"]->isReleased(moustPosView) ||
					this->buttons["HARD"]->isReleased(moustPosView))
				{
					if (this->startGame == false)
					{
						this->delay = true;
						this->startMenu = false;
						this->generateGrid();
						this->InitStopWatch();
						this->InitFlag();
						this->InitNewWindow();
						this->InitIcon();
						this->startGame = true; 
					}
				}
			}
			if (this->startMenu == false)
			{
				for (int i = 0; i < this->height; ++i)
					for (int j = 0; j < this->width; ++j)
					{
						if (this->ceil[i][j]->isIn(this->moustPosView) && this->firstClick == true)
						{
							this->runningGame = true;
							this->firstClick = false;
						}
						else if (this->ceil[i][j]->isIn(this->moustPosView) && event.mouseButton.button == sf::Mouse::Right && this->ceil[i][j]->isEqual('_'))
						{
							this->ceil[i][j]->fillCeil('#');
							this->numMines--;
						}
						else if (this->ceil[i][j]->isIn(this->moustPosView) && event.mouseButton.button == sf::Mouse::Right && this->ceil[i][j]->isEqual('#'))
						{
							this->ceil[i][j]->fillCeil('_'); 
							this->numMines++; 
						}
					}
			}
		}
	}
}
void Game::updateMousePosition()
{
	this->mousePosWindow = sf::Mouse::getPosition(*this->window); 
	this->moustPosView = this->window->mapPixelToCoords(this->mousePosWindow); 

	this->pos_y = this->mousePosWindow.x / (this->size); 
	this->pos_x = this->mousePosWindow.y / (this->size);
}
void Game::updateGrid()
{
	for(int i = 0; i < this->height; ++i)
		for (int j = 0; j < this->width; ++j)
		{
			this->ceil[i][j]->update(this->moustPosView);
			if (this->ceil[i][j]->isPressed())
			{
				if (this->ceil[i][j]->isEqual('#'))
					continue;
				if (this->currentIndexMove == 0)
				{
					this->placeMines(i, j);
					this->fillGrid();
					this->unlockEmptyCeil(i, j);
					this->currentIndexMove++;
					this->firstClick = true; 
				}
				else if (this->hideGrid[i][j] == '-')
				{
					this->unlockEmptyCeil(i, j);
				}
				else if (this->hideGrid[i][j] == '*')
				{
					for (int i = 0; i < this->height; ++i)
						for (int j = 0; j < this->width; ++j)
						{
							if(this->hideGrid[i][j] == '*')
								this->ceil[i][j]->fillCeil(this->hideGrid[i][j]);
						}
					this->runningGame = false; 
					this->icon->update(false);
				}
				else
				{
					this->ceil[i][j]->fillCeil(this->hideGrid[i][j]);
				}
			}
		}
}
void Game::unlockEmptyCeil(int i, int j)
{
	// Set the square to the number of adjacent mines ( if > 0 )
	if (this->hideGrid[i][j] != '*' && this->hideGrid[i][j] != '-') {
		this->ceil[i][j]->fillCeil(this->hideGrid[i][j]); 
	}
	// Set the square to a revealed blank square 'B' & Recursively update the 8 adjancent squares.
	else {
		this->ceil[i][j]->fillCeil('-');
		for (int x = i - 1; x <= i + 1; x++) {
			for (int y = j - 1; y <= j + 1; y++) {
				if (x < 0 || y < 0 || x >= height || y >= width || (x == i && y == j) || this->ceil[x][y]->isEqual('_') == false)
					continue;
				this->unlockEmptyCeil(x, y);
			}
		}
	}
}
void Game::updateButtons()
{
	for (auto x : this->buttons)
		x.second->update(this->moustPosView);

	if (this->buttons["NEW_GAME"]->isPressed())
	{
		int x = this->posButton[0].first + this->videoMode.width / 8 + this->videoMode.width / 5; 
		this->buttons["EASY"] = new Button(x, this->posButton[0].second - this->videoMode.height / 8 , this->videoMode.width / 5, this->videoMode.height / 16,
			&this->font, "Easy",
			sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200));

		this->buttons["MEDIUM"] = new Button(x, this->posButton[0].second, this->videoMode.width / 5, this->videoMode.height / 16,
			&this->font, "Medium",
			sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200));

		this->buttons["HARD"] = new Button(x, this->posButton[0].second + this->videoMode.height / 8, this->videoMode.width / 5, this->videoMode.height / 16,
			&this->font, "Hard",
			sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200));

		this->newGame = true; 
	}
	
	if (this->newGame)
	{
		if (this->buttons["EASY"]->isPressed())
		{
			this->width = 9;
			this->height = 9;
			this->numMines = 10;
		}
		if (this->buttons["MEDIUM"]->isPressed())
		{
			this->width = 16;
			this->height = 16;
			this->numMines = 40;
		}
		if (this->buttons["HARD"]->isPressed())
		{
			this->width = 24;
			this->height = 24;
			this->numMines = 99;
		}
	}

	if (this->buttons["EXIT_GAME"]->isPressed())
	{
		this->endGame = true; 
	}
}


// Render
void Game::displayGrid()
{
	for (int i = 0; i < this->height; ++i)
		for (int j = 0; j < this->width; ++j)
		{
			this->ceil[i][j]->render(this->window);
		}
}
void Game::renderButtons()
{
	for (auto x : this->buttons)
		x.second->render(this->window); 
}
void Game::render()
{
	this->window->clear(sf::Color::White); 
	if (this->startMenu)
	{
		this->renderButtons();
	}
	else {
		this->displayGrid();
		this->stopwatch->render(this->window);
		this->flag->render(this->window);
		this->icon->render(this->window);
	}

	this->window->display();
}
void Game::temp()
{
	for (int i = 0; i < this->height; ++i)
	{
		for (int j = 0; j < this->width; ++j)
			std::cout << this->hideGrid[i][j];
		std::cout << std::endl;
	}
}

// Save
void Game::saveGame()
{
	std::ofstream fout("images/Data.txt"); 
	fout << this->height << ' ' << this->width << '\n';
	for (int i = 0; i < this->height; ++i)
	{
		for (int j = 0; j < this->width; ++j)
			fout << this->hideGrid[i][j]; 
		fout << '\n';
	}
	for (int i = 0; i < this->height; ++i)
	{
		for (int j = 0; j < this->width; ++j)
			fout << this->ceil[i][j]->getIcon(); 
		fout << '\n';
	}
	fout << this->numMines << '\n';
	fout << this->stopwatch->getTimer() << '\n';
}

void Game::deleteGame()
{
	for (auto x : this->buttons)
		delete x.second;

	if (!this->startMenu)
	{
		for (int i = 0; i < this->height; ++i)
			for (int j = 0; j < this->width; ++j)
				delete ceil[i][j];
	}

	delete this->stopwatch;
	delete this->flag;
	delete this->icon;
	delete this->window;
}


















