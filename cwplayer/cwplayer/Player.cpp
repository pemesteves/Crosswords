#include "Player.h"
#include <string>
#include <time.h>

using namespace std;

Player::Player(string name) //Criador do jogador - coloca o nome deste
{
	playerName = name;
	numberClues = 0;
}

void Player::startTime() //Começa a contar o tempo que o jogador demora a resolver o puzzle
{
	sTime = time(NULL);
}

time_t Player::getStartTime()//Devolve a variável time
{
	return sTime;
}

void Player::setNumberclues() //Adiciona um ao número de ajudas pedidas
{
	numberClues++;
}

unsigned int Player::getNumberClues() //Número de ajudas adicionais que o jogador usa
{
	return numberClues;
}

string Player::getName() //Devolve o nome do jogador
{
	return playerName;
}