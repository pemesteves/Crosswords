#pragma once

#include <string>
#include <vector>
#include <map>

class Board
{
public:
	Board(const unsigned int lines, const unsigned int columns); //Criação de um tabuleiro através do número de linhas e colunas
	Board(std::ifstream &file); //Retira o tabuleiro de um ficheiro criado anteriormente pelo programa cwcreator
	void showBoard(); //Mostra o tabuleiro
	void saveBoard(const std::string &dictionaryFileName); //Guarda o tabuleiro num ficheiro do tipo bXXX.txt
	bool existsWord(const unsigned int &l, const unsigned int &c, const char &d); //Verifica se já existe uma palavra na posição dada
	bool existsWord(const std::string &word); //Verifica se a palavra já foi incluída no tabuleiro
	void putWord(std::string &word, const char &line, const char &column, const char &direction); //Função que coloca uma palavra no tabuleiro 
	bool validWord(const std::string &word, const char &line, const char &column, const char &direction); //Função que verifica se uma palavra é válida no tabuleiro
	unsigned int getLines(); //Retorna o número de linhas
	unsigned int getColumns(); //Retorna o número de colunas
	void removeWord(const char &line, const char &column, const char &direction); //Remove uma palavra do tabuleiro
	char getChar(const char &line, const char &column); //Retorna o carater da posição dada
	void getWords(std::vector<std::string> &v); //Retorna no vetor passado como referência, todas as palavras já colocadas no tabuleiro
	std::vector<std::string> getWords(); //Função que retorna todas as palavras do tabuleiro
	bool isFull(); //Verifica se o tabuleiro está cheio
	void setNumberBoard(); //Altera o numberBoard
	void putBlack(); //Coloca todos os pontos do board como #
private:
	std::vector<std::vector<char>> board; //Tabuleiro onde são inseridas as palavras
	std::map <std::string, std::string> words; //map com as posições e as palavras do tabuleiro
	static unsigned int numberBoard; //Número do tabuleiro
	unsigned int numLines; //Númeor de linhas do tabuleiro
	unsigned int numColumns; //Número de colunas do tabuleiro
	bool wordFits(size_t wordSize, char line, char column, char direction); //Função que verifica se uma palavra cabe num certo espaço
	bool anotherWordVert(unsigned int inicialLine, unsigned int finalLine, unsigned int column); //Verifica se existe outra palavra na vertical
	bool anotherWordHor(unsigned int inicialColumn, unsigned int finalColumn, unsigned int line); //Verifica se existe outra palavra na horizontal
	bool compareWords(char direction, unsigned int inicialLine, unsigned int inicialColumn, std::string word); //Compara uma palava com o que está no tabuleiro
};