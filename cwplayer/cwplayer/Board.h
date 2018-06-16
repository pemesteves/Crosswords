#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <map>


class Board
{
public:
	Board(std::ifstream &file); //Criador do tabuleiro: retira o tabuleiro de um ficheiro criado pelo programa cwcreator
	void showBoard(); //Função que mostra o tabuleiro
	bool existsWord(const unsigned int &l, const unsigned int &c, const char &d); //Função que verifica se já existe uma palavra na posição dada
	bool existsWord(const std::string &word); //Função que verifica se a palavra já foi incluída no tabuleiro
	void putWord(std::string &word, const char &line, const char &column, const char &direction); //Função que coloca uma palavra no tabuleiro e, consequentemente, coloca a sua posição inicial e os sinónimos no map
	unsigned int getLines(); //Devolve o número de linhas do tabuleiro
	unsigned int getColumns(); //Devolve o número de colunas do tabuleiro
	void removeWord(const char &line, const char &column, const char &direction); //Função que remove uma palavra do tabuleiro
	char getChar(const char &line, const char &column); //Devolve o carater de uma determinada posição
	void getWords(std::vector<std::string> &v); //Guarda no vetor passado como parâmetro todas as palavras que se encontram no board
	std::string getWord(std::string &position); //Devolve a palavra que se encontra em determinada posição
	void getPositions(std::vector<std::string> &v); //Guarda no vetor passado como parâmetro todas as posições que contêm palavras 
	bool isFull(); //Verifica se o tabuleiro está cheio
	void setBoardName(std::string name); //Inicializa a variável boardFileName
	bool compareBoards(); //Compara os dois tabuleiros
	std::string getBoardName(); //Devolve o nome do ficheiro do tabuleiro
private:
	std::vector<std::vector<char>> board; //Solução final - só será usado no fim para comparar com o tabuleiro soluction
	std::vector<std::vector <char>> playerSolution; //Jogámos com a solução que conterá as palavras a preto
	std::map <std::string, std::string> words; //map com as posições e as palavras do tabuleiro
	std::map <std::string, std::string> playerWords; //map com as posições e as palavras do tabuleiro
	std::string boardFileName; //Variável que contém o nome do ficheiro
	unsigned int numLines; //Número de linhas dos tabuleiros
	unsigned int numColumns; //Número de colunas dos tabuleiros
	bool anotherWordVert(unsigned int inicialLine, unsigned int finalLine, unsigned int column); //Verifica se existe outra palavra na mesma coluna 
	bool anotherWordHor(unsigned int inicialColumn, unsigned int finalColumn, unsigned int line); //Verifica se existe outra palavra na mesma linha 
	bool wordFits(size_t wordSize, char line, char column, char direction); //Função que verifica se uma palavra cabe num certo espaço -> retorna falso se começa na posição errada ou tem comprimento inadequado à posição
	bool compareWords(char direction, unsigned int inicialLine, unsigned int inicialColumn, std::string word); //Compara a palavra a colocar com as letras que já se encontram no tabuleiro
};

