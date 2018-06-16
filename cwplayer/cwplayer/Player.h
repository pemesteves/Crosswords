#pragma once
#include <string>

class Player
{
public:
	Player(std::string name); //Criador do jogador - coloca o nome deste
	void startTime(); //"Ativa" a vari�vel time
	time_t getStartTime(); //Devolve a vari�vel time
	void setNumberclues(); //Adiciona um ao n�mero de ajudas pedidas
	unsigned int getNumberClues(); //Devolve a vari�vel numberClues
	std::string getName(); //Devove o nome do jogador
private:
	std::string playerName; //Nome do jogador
	time_t sTime;//Segundo em que o jogador come�ou a jogar 
	unsigned int numberClues; //N�mero de ajudas adicionais que o jogador usa
};

