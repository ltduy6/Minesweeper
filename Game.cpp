#include "Game.h"
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <queue>

// Init
void Game::loadVariable()
{
	this->window = nullptr; 
	// Game logic
	this->width = 9;
	this->height = 9;
	this->numMines = 10;
	this->currentIndexMove = 0;
	this->state = 0;
	this->startGame = false;
	this->endGame = false;
	this->continueGame = this->isContinue(); 
	this->startMenu = true; 
	this->newGame = false;
	this->delay = false; 
	this->runningGame = false; 
	this->firstClick = false; 
	this->putFlag = false;
	this->isChoosing = false; 
	this->timer = "000";
	this->textLevel = "Easy";
	this->state = -1; 
}
void Game::loadWindow()
{
	this->videoMode.height = 800; 
	this->videoMode.width = 800; 

	this->window = new sf::RenderWindow(this->videoMode, "Minesweeper"); 

	this->window->setFramerateLimit(60);

	this->scale = this->videoMode.width / (this->size * this->width); 

	this->posButton[0] = { this->videoMode.width / 2.5, this->videoMode.height / 4 }; 
	this->posButton[1] = { this->videoMode.width / 2.5, this->videoMode.height / 4 + this->videoMode.height / 16 + this->videoMode.height / 8 }; 
	this->posButton[2] = { this->videoMode.width / 2.5, this->posButton[1].second + this->videoMode.height / 16 + this->videoMode.height / 8 }; 
}
void Game::InitButtons()
{
	for (auto x : this->buttons)
		delete x.second; 
	this->buttons["NEW_GAME"] = new Button(this->posButton[0].first, this->posButton[0].second, this->videoMode.width / 5, this->videoMode.height / 13, "New Game");
	
	if (this->isContinue())
	{
		this->buttons["CONTINUE"] = new Button(this->posButton[1].first, this->posButton[1].second, this->videoMode.width / 5, this->videoMode.height / 13, "Continue");

		this->buttons["EXIT_GAME"] = new Button(this->posButton[2].first, this->posButton[2].second, this->videoMode.width / 5, this->videoMode.height / 13, "Quit");
	}
	else {
		this->buttons["EXIT_GAME"] = new Button(this->posButton[1].first, this->posButton[1].second, this->videoMode.width / 5, this->videoMode.height / 13, "Quit");
	}
}
void Game::InitFont()
{
	if (!this->font.loadFromFile("Fonts/Roboto-Medium.ttf"))
		throw("ERROR"); 
}
void Game::InitStopWatch()
{
	delete this->stopwatch; 
	this->stopwatch = new Stopwatch(this->timer, (this->width + 1) * this->size , this->height * this->size * 2);
}
void Game::InitFlag()
{
	delete this->flag; 
	this->flag = new Flag((this->width - 2.5) * this->size, this->height * this->size * 2, this->numMines);
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
	delete this->icon; 
	this->icon = new Icon(this->state, (this->width - 1) * this->size * 2, this->height * this->size * 2);
}
void Game::InitLevel()
{
	for (auto x : this->levels)
		delete x.second; 
	this->levels["Easy"] = new Button(0, (this->height - 1) * this->size * 2, this->size * 4, this->size * 2, "Easy");
	this->levels["Medium"] = new Button(0, (this->height - 2) * this->size * 2, this->size * 4, this->size * 2, "Medium");
	this->levels["Hard"] = new Button(0, (this->height - 3) * this->size * 2, this->size * 4, this->size * 2, "Hard");
}
void Game::InitCurrentLevel()
{
	delete this->currentLevel;
	this->currentLevel = new Button(0, this->height * this->size * 2, this->size * 4, this->size * 2, this->textLevel);
}

// Create grid
void Game::generateGrid()
{
	// empty ceil
	for (int i = 0; i < this->height; ++i)
	{
		std::vector<Ceil*> newVector; 
		std::string lineHide = ""; 
		for (int j = 0; j < this->width; ++j)
		{
			lineHide += '-'; 
			Ceil* newCeil = new Ceil('_', i, j);
			newVector.push_back(newCeil);
		}
		this->ceil.push_back(newVector);
		this->hideGrid.push_back(lineHide); 
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
	this->InitLevel();
	this->InitCurrentLevel();
}

Game::~Game()
{
	for (auto x : this->buttons)
		delete x.second; 
	for (auto x : this->levels)
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
	delete this->currentLevel; 
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

const bool Game::isContinue() const
{
	std::ifstream fin("images/Data.txt");
	std::string line;
	std::getline(fin, line, '\n');
	fin.close();
	if (line != "0")
		return true;
	return false;
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
			this->currentLevel->update(this->moustPosView);
			if (this->isChoosing)
			{
				this->updateLevel();
			}
			if (this->endGame == false)
			{
				if (!this->isChoosing)
				{
					this->updateGrid();
				}

				if (this->WinGame() == true)
				{
					this->runningGame = false;
					this->endGame = true;
					this->state = 1;
				}

				this->flag->update(this->numMines);
				this->icon->update(this->state);
				this->stopwatch->update(this->runningGame);
			}
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
			if(this->startMenu == false && this->endGame == false)
				this->saveGame();
			if (this->endGame)
				this->deleteGame(); 
			this->window->close(); 
			break; 
		case sf::Event::KeyPressed:
			if (this->event.key.code == sf::Keyboard::Escape)
			{
				if (this->startMenu == false && this->endGame == false)
					this->saveGame();
				this->window->close(); 
			}
			break;
		case sf::Event::MouseButtonReleased:
			if (this->startMenu)
			{
				if (this->buttons["EXIT_GAME"]->isReleased(moustPosView))
					this->window->close();
			}
			if (!this->newGame)
			{
				if (this->buttons["NEW_GAME"]->isReleased(this->moustPosView))
				{
					this->delay = true;
					this->startMenu = false;
					this->InitNewWindow();
					this->generateGrid();
					this->InitStopWatch();
					this->InitFlag();
					this->InitIcon();
					this->newGame = true; 
					this->continueGame = false; 
					break; 
				}
			}
			if (this->continueGame)
			{
				if (this->buttons["CONTINUE"]->isReleased(moustPosView))
				{
					if (this->startGame == false)
					{
						this->delay = true;
						this->startMenu = false;
						this->startGame = true;
						this->firstClick = true;
						this->continueGame = false;
						this->loadData();
						this->InitNewWindow();
						this->InitStopWatch();
						this->InitFlag();
						this->InitIcon();
						this->initCurrentIndexMove();
						this->InitLevel();
						this->InitCurrentLevel();
						this->runningGame = false;
						break; 
					}
				}
			}
			if (this->startMenu == false)
			{
				if (this->currentLevel->isReleased(this->moustPosView))
				{
					this->isChoosing = !this->isChoosing; 
					this->runningGame = !this->runningGame; 
					break; 
				}
				if (this->isChoosing == true)
				{
					if (this->levels["Easy"]->isReleased(this->moustPosView) ||
						this->levels["Medium"]->isReleased(moustPosView) ||
						this->levels["Hard"]->isReleased(moustPosView))
					{
						this->deleteGrid();
						if (this->levels["Easy"]->isReleased(this->moustPosView))
						{
							this->width = 9; 
							this->height = 9; 
							this->numMines = 10; 
						}
						if (this->levels["Medium"]->isReleased(this->moustPosView))
						{
							this->width = 16;
							this->height = 16;
							this->numMines = 40;
						}
						if (this->levels["Hard"]->isReleased(this->moustPosView))
						{
							this->width = 24;
							this->height = 24;
							this->numMines = 99;
						}
						this->updateCurrentLevel(); 
						this->InitNewWindow();
						this->generateGrid();
						this->loadVariableAgain();
						this->InitStopWatch();
						this->InitFlag();
						this->InitIcon();
						this->InitLevel(); 
						this->isChoosing = false; 
						this->runningGame = false; 
						break; 
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
			break; 
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
							if (this->hideGrid[i][j] == '*')
								this->ceil[i][j]->fillCeil(this->hideGrid[i][j]);
						}
					this->runningGame = false; 
					this->endGame = true; 
					this->state = 0; 
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
}
void Game::updateLevel()
{
	for (auto x : this->levels)
		x.second->update(this->moustPosView); 

}
void Game::updateCurrentLevel()
{
	delete this->currentLevel; 
	for (auto x : this->levels)
	{
		if (x.second->isReleased(this->moustPosView))
		{
			this->currentLevel = new Button(0, this->height * this->size * 2, this->size * 4, this->size * 2, x.first);
			this->textLevel = x.first; 
			break;
		}
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
void Game::renderLevel()
{
	for (auto x : this->levels)
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
		this->currentLevel->render(this->window); 
		if (this->isChoosing)
		{
			this->renderLevel();
		}
	}

	this->window->display();
}

// Save
void Game::saveGame()
{
	std::ofstream fout("images/Data.txt"); 
	fout << this->height << ' '; 
	fout << this->width << ' '; 
	fout << this->numMines << ' ';
	fout << this->state << '\n';
	fout << this->stopwatch->getTimer() << '\n';
	fout << this->textLevel << '\n';

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
	fout.close();
}

void Game::deleteGame()
{
	std::ofstream fout("images/Data.txt");
	fout << "0";
	fout.close();
}

void Game::loadData()
{
	std::ifstream fin("images/Data.txt");
	fin >> this->height; 
	fin >> this->width;
	fin >> this->numMines;
	fin >> this->state; 
	fin.get();
	std::string line;
	std::getline(fin, this->timer, '\n');
	std::getline(fin, this->textLevel, '\n');
	for (int i = 0; i < this->height; ++i)
	{
		std::getline(fin, line, '\n');
		this->hideGrid.push_back(line);
	}
	for (int i = 0; i < this->height; ++i)
	{
		std::vector<Ceil*> newVector;
		std::getline(fin, line, '\n');
		for (int j = 0; j < this->width; ++j)
		{
			Ceil* newCeil = new Ceil(line[j], i, j);
			newVector.push_back(newCeil);
		}
		this->ceil.push_back(newVector);
	}
	fin.close();
}

void Game::initCurrentIndexMove()
{
	for(int i = 0; i < this->height; ++i)
		for(int j = 0; j < this->width; ++j)
			if (this->ceil[i][j]->isEqual('_') == false)
			{
				this->currentIndexMove = 1; 
				return; 
			}
	this->currentIndexMove = 0;
}

void Game::deleteGrid()
{
	for (auto& x : this->hideGrid)
	{
		x.clear(); 
	}
	this->hideGrid.clear();
	for (auto& x : this->ceil)
	{
		for (auto y : x)
			delete y; 
		x.clear(); 
	}
	this->ceil.clear();
}

void Game::loadVariableAgain()
{
	this->currentIndexMove = 0;
	this->startGame = false;
	this->endGame = false;
	this->runningGame = false;
	this->firstClick = false;
	this->putFlag = false;
	this->isChoosing = false;
	this->continueGame = false; 
	this->timer = "000";
	this->state = -1;
}



















