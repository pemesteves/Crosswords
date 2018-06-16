#pragma once
#include <string>

class Player
{
public:
	Player(std::string name); //Criador do jogador - coloca o nome deste
	void startTime(); //"Ativa" a variável time
	time_t getStartTime(); //Devolve a variável time
	void setNumberclues(); //Adiciona um ao número de ajudas pedidas
	unsigned int getNumberClues(); //Devolve a variável numberClues
	std::string getName(); //Devove o nome do jogador
private:
	std::string playerName; //Nome do jogador
	time_t sTime;//Segundo em que o jogador começou a jogar 
	unsigned int numberClues; //Número de ajudas adicionais que o jogador usa
};

