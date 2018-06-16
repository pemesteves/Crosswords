#pragma once

#include <string>
#include <vector>
#include <map>

class Board
{
public:
	Board(const unsigned int lines, const unsigned int columns); //Cria��o de um tabuleiro atrav�s do n�mero de linhas e colunas
	Board(std::ifstream &file); //Retira o tabuleiro de um ficheiro criado anteriormente pelo programa cwcreator
	void showBoard(); //Mostra o tabuleiro
	void saveBoard(const std::string &dictionaryFileName); //Guarda o tabuleiro num ficheiro do tipo bXXX.txt
	bool existsWord(const unsigned int &l, const unsigned int &c, const char &d); //Verifica se j� existe uma palavra na posi��o dada
	bool existsWord(const std::string &word); //Verifica se a palavra j� foi inclu�da no tabuleiro
	void putWord(std::string &word, const char &line, const char &column, const char &direction); //Fun��o que coloca uma palavra no tabuleiro 
	bool validWord(const std::string &word, const char &line, const char &column, const char &direction); //Fun��o que verifica se uma palavra � v�lida no tabuleiro
	unsigned int getLines(); //Retorna o n�mero de linhas
	unsigned int getColumns(); //Retorna o n�mero de colunas
	void removeWord(const char &line, const char &column, const char &direction); //Remove uma palavra do tabuleiro
	char getChar(const char &line, const char &column); //Retorna o carater da posi��o dada
	void getWords(std::vector<std::string> &v); //Retorna no vetor passado como refer�ncia, todas as palavras j� colocadas no tabuleiro
	std::vector<std::string> getWords(); //Fun��o que retorna todas as palavras do tabuleiro
	bool isFull(); //Verifica se o tabuleiro est� cheio
	void setNumberBoard(); //Altera o numberBoard
	void putBlack(); //Coloca todos os pontos do board como #
private:
	std::vector<std::vector<char>> board; //Tabuleiro onde s�o inseridas as palavras
	std::map <std::string, std::string> words; //map com as posi��es e as palavras do tabuleiro
	static unsigned int numberBoard; //N�mero do tabuleiro
	unsigned int numLines; //N�meor de linhas do tabuleiro
	unsigned int numColumns; //N�mero de colunas do tabuleiro
	bool wordFits(size_t wordSize, char line, char column, char direction); //Fun��o que verifica se uma palavra cabe num certo espa�o
	bool anotherWordVert(unsigned int inicialLine, unsigned int finalLine, unsigned int column); //Verifica se existe outra palavra na vertical
	bool anotherWordHor(unsigned int inicialColumn, unsigned int finalColumn, unsigned int line); //Verifica se existe outra palavra na horizontal
	bool compareWords(char direction, unsigned int inicialLine, unsigned int inicialColumn, std::string word); //Compara uma palava com o que est� no tabuleiro
};