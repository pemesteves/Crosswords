#include "Player.h"
#include <string>
#include <time.h>

using namespace std;

Player::Player(string name) //Criador do jogador - coloca o nome deste
{
	playerName = name;
	numberClues = 0;
}

void Player::startTime() //Come�a a contar o tempo que o jogador demora a resolver o puzzle
{
	sTime = time(NULL);
}

time_t Player::getStartTime()//Devolve a vari�vel time
{
	return sTime;
}

void Player::setNumberclues() //Adiciona um ao n�mero de ajudas pedidas
{
	numberClues++;
}

unsigned int Player::getNumberClues() //N�mero de ajudas adicionais que o jogador usa
{
	return numberClues;
}

string Player::getName() //Devolve o nome do jogador
{
	return playerName;
}