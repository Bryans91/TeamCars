#pragma once
class GameController
{
public:
	GameController();
	void initUI();
	void startGame();
	void setGameRunning(bool);
	double getDelta();
	~GameController();
private:
	void gameLoop();
	void doGameUpdates(double); // double = deltaTime
};

