#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <ctime>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include "Button.h"
#include "Ceil.h"
#include "Stopwatch.h"
#include "Flag.h"
#include "Icon.h"

/*
	Game engine
*/

class Game
{
private:
	// variable 
	// window
	sf::RenderWindow* window; 
	sf::Event event;
	sf::VideoMode videoMode;
	sf::Font font; 

	std::map<std::string, Button*> buttons; 
	std::map<std::string, Button*> levels; 
	std::vector<std::vector<Ceil*>> ceil;
	Stopwatch* stopwatch;
	Flag* flag;
	Icon* icon;
	Button* currentLevel;

	// Game logic
	sf::Vector2i mousePosWindow; 
	sf::Vector2f moustPosView; 
	int pos_x, pos_y; 
	std::pair<int, int> posButton[3]; 

	bool endGame;
	bool newGame;
	bool continueGame;
	bool startMenu;
	bool delay;
	bool runningGame;
	bool startGame;
	bool firstClick;
	bool putFlag; 
	bool isChoosing; 
	int currentIndexMove;
	float scale;
	int size{ 32 };
	int width{9}, height{9};
	int numMines{10};
	int state; 
	std::string timer;
	std::vector<std::string> showGrid;
	std::vector<std::string> hideGrid; 

	// private functions
	void loadVariable(); 
	void loadWindow(); 
	void InitFont(); 
	void InitButtons(); 
	void InitStopWatch();
	void InitFlag();
	void InitNewWindow();
	void InitIcon();
	void InitLevel();
	void InitCurrentLevel();
public:
	// constuctors & destructors
	Game(); 
	virtual ~Game(); 

	// Accessors
	const bool isContinue() const;
	const bool running() const; 
	const bool EndGame() const; 
	const bool WinGame() const; 
	// Functions
	
	void generateGrid();
	void placeMines(int row, int col);
	bool isMine(int row, int col);
	bool isValid(int row, int col);
	int countAdjacentMines(int i, int j); 
	void unlockEmptyCeil(int row, int col); 
	void fillGrid(); 
	void temp();

	void pollEvents(); 
	void updateMousePosition(); 
	void updateGrid();
	void updateButtons(); 
	void updateLevel();
	void updateCurrentLevel();
	void update();

	void displayGrid();
	void renderButtons();  
	void renderLevel();
	void render();

	void loadData();
	void saveGame();
	void deleteGame();
	void deleteGrid();
	void initCurrentIndexMove();
	void loadVariableAgain(); 
};

