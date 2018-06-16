#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <map>


class Board
{
public:
	Board(std::ifstream &file); //Criador do tabuleiro: retira o tabuleiro de um ficheiro criado pelo programa cwcreator
	void showBoard(); //Fun��o que mostra o tabuleiro
	bool existsWord(const unsigned int &l, const unsigned int &c, const char &d); //Fun��o que verifica se j� existe uma palavra na posi��o dada
	bool existsWord(const std::string &word); //Fun��o que verifica se a palavra j� foi inclu�da no tabuleiro
	void putWord(std::string &word, const char &line, const char &column, const char &direction); //Fun��o que coloca uma palavra no tabuleiro e, consequentemente, coloca a sua posi��o inicial e os sin�nimos no map
	unsigned int getLines(); //Devolve o n�mero de linhas do tabuleiro
	unsigned int getColumns(); //Devolve o n�mero de colunas do tabuleiro
	void removeWord(const char &line, const char &column, const char &direction); //Fun��o que remove uma palavra do tabuleiro
	char getChar(const char &line, const char &column); //Devolve o carater de uma determinada posi��o
	void getWords(std::vector<std::string> &v); //Guarda no vetor passado como par�metro todas as palavras que se encontram no board
	std::string getWord(std::string &position); //Devolve a palavra que se encontra em determinada posi��o
	void getPositions(std::vector<std::string> &v); //Guarda no vetor passado como par�metro todas as posi��es que cont�m palavras 
	bool isFull(); //Verifica se o tabuleiro est� cheio
	void setBoardName(std::string name); //Inicializa a vari�vel boardFileName
	bool compareBoards(); //Compara os dois tabuleiros
	std::string getBoardName(); //Devolve o nome do ficheiro do tabuleiro
private:
	std::vector<std::vector<char>> board; //Solu��o final - s� ser� usado no fim para comparar com o tabuleiro soluction
	std::vector<std::vector <char>> playerSolution; //Jog�mos com a solu��o que conter� as palavras a preto
	std::map <std::string, std::string> words; //map com as posi��es e as palavras do tabuleiro
	std::map <std::string, std::string> playerWords; //map com as posi��es e as palavras do tabuleiro
	std::string boardFileName; //Vari�vel que cont�m o nome do ficheiro
	unsigned int numLines; //N�mero de linhas dos tabuleiros
	unsigned int numColumns; //N�mero de colunas dos tabuleiros
	bool anotherWordVert(unsigned int inicialLine, unsigned int finalLine, unsigned int column); //Verifica se existe outra palavra na mesma coluna 
	bool anotherWordHor(unsigned int inicialColumn, unsigned int finalColumn, unsigned int line); //Verifica se existe outra palavra na mesma linha 
	bool wordFits(size_t wordSize, char line, char column, char direction); //Fun��o que verifica se uma palavra cabe num certo espa�o -> retorna falso se come�a na posi��o errada ou tem comprimento inadequado � posi��o
	bool compareWords(char direction, unsigned int inicialLine, unsigned int inicialColumn, std::string word); //Compara a palavra a colocar com as letras que j� se encontram no tabuleiro
};

