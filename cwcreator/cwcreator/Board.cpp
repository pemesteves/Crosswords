#include "Board.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

using namespace std;

//Declaração de funções que estão definidas no ficheiro cwcreator.cpp
//Estas funções estão aqui definidas, pois são usadas neste ficheiro
void setcolor(unsigned int color);
void setcolor(unsigned int color, unsigned int background_color);

unsigned int Board::numberBoard = 1; //Número do tabuleiro será inicializado apenas uma vez, pois é uma função static

Board::Board(const unsigned int lines, const unsigned int columns) //Criação de um tabuleiro através do número de linhas e colunas
{
	numLines = lines;
	numColumns = columns;

	board.resize(numLines);

	for (size_t i = 0; i < board.size(); i++)
	{
		board.at(i).resize(numColumns);
	}

	for (size_t i = 0; i < numLines; i++) //Inicialmente todas as posições conterão pontos 
		for (size_t j = 0; j < numColumns; j++)
			board.at(i).at(j) = '.';
}

Board::Board(ifstream &file) //Retira o tabuleiro de um ficheiro criado anteriormente pelo programa cwcreator
{
	string line;

	vector <char> v;
	vector <vector <char>> b;

	board.resize(0); //board é reinicializado

	//Retira o tabuleiro do ficheiro
	while (!file.eof())
	{
		getline(file, line);
		if (line.length() == 0) //Se ao percorrer o ficheiro encontrar uma linha vazia, significa que já foi carregado todo o ficheiro
			break;

		v.resize(0);

		copy(line.begin(), line.end(), back_inserter(v)); //Função que copia a string line para um vector<char> v

		board.push_back(v);
	}

	numColumns = v.size();
	numLines = board.size();

	string position;
	string word;
	pair<string, string> p;

	//Retira as posições e as palavras que estão no tabuleiro do ficheiro
	while (!file.eof())
	{
		getline(file, line);

		if (line.length() == 0)
			break;

		position = line.substr(0, 3);
		word = line.substr(4, line.length() - 4);

		p = make_pair(position, word); //par posição - palavra
		words.insert(p); //words contém pares com as posições e a palavra na posição
	}
}

void Board::putWord(string &word, const char &line, const char &column, const char &direction) //Função que coloca uma palavra no tabuleiro e, consequentemente, coloca a sua posição inicial e os sinónimos no map
{
	ostringstream pos; //String stream que ajudará a criar a posição
	string position; //String com a posição da palavra
	vector<string> synonyms; //Vetor com os sinónimos
	pair<string, string> p;

	if (board.at((int)line - 65).at((int)column - 97) == '#')
		cout << "You can't put a word onto a #";
	else if (wordFits(word.length(), line, column, direction))
	{
		if (direction == 'V')
		{
			if (!anotherWordVert((int)line - 65, (int)line + word.length() - 65, (int)column - 97)) //Coloca a palavra normalmente
			{
				int wordPosition;
				if ((int)line - 65 != 0) //Caso não esteja na primeira posição da tabela
				{
					if (board.at((int)line - 66).at((int)column - 97) == '.')
						board.at((int)line - 66).at((int)column - 97) = '#';
				}
				wordPosition = 0;
				for (unsigned int i = (unsigned int)line - 65; i < (unsigned int)line + word.length() - 65; i++)
				{
					board.at(i).at((int)column - 97) = word.at(wordPosition);
					wordPosition++;
				}

				if ((int)line - 65 + word.length() < numLines) //Caso a última letra não esteja na última posição da linha
				{
					if (board.at((int)line - 65 + word.length()).at((int)column - 97) == '.')
						board.at((int)line - 65 + word.length()).at((int)column - 97) = '#';
				}

				//Colocar sinónimos no map
				pos << line << column << direction;
				position = pos.str();
				p = make_pair(position, word); //Cria par com a posição e a palavra para colocar no map words
				words.insert(p);
			}
			else //Coloca a palavra se conseguir (se encaixar com a letra já existente)
			{
				if (compareWords(direction, (int)line, (int)column, word)) //Igual ao anterior: possível colocar na mesma condição com um or (||), mas atrasaria o programa caso se colocasse a palavra normalmente
				{
					int wordPosition;
					if ((int)line - 65 != 0) //Caso não esteja na primeira posição da tabela
					{
						if (board.at((int)line - 66).at((int)column - 97) == '.')
							board.at((int)line - 66).at((int)column - 97) = '#';
					}

					wordPosition = 0;

					for (unsigned int i = (unsigned int)line - 65; i < (unsigned int)line + word.length() - 65; i++)
					{
						board.at(i).at((int)column - 97) = word.at(wordPosition);
						wordPosition++;
					}

					if ((int)line - 65 + word.length() < numLines) //Caso a última letra não esteja na última posição da linha
					{
						if (board.at((int)line - 65 + word.length()).at((int)column - 97) == '.')
							board.at((int)line - 65 + word.length()).at((int)column - 97) = '#';
					}

					//Colocar sinónimos no map
					pos << line << column << direction;
					position = pos.str();
					p = make_pair(position, word); //Cria par com a posição e a palavra para colocar no map words
					words.insert(p);
				}
				else
					cout << "Invalid word for this postion.";
			}
		}
		else
		{
			if (!anotherWordHor((int)column - 97, (int)column + word.length() - 97, (int)line - 65))  //Coloca a palavra normalmente
			{
				int wordPosition;

				if ((int)column - 97 != 0) //Caso não esteja na primeira posição da tabela
				{
					if (board.at((int)line - 65).at((int)column - 98) == '.')
						board.at((int)line - 65).at((int)column - 98) = '#';
				}

				wordPosition = 0;

				for (unsigned int i = (unsigned int)column - 97; i < (unsigned int)column + word.length() - 97; i++)
				{
					board.at((int)line - 65).at(i) = word.at(wordPosition);
					wordPosition++;
				}

				if ((int)column - 97 + word.length() < numColumns) //Caso a última letra não esteja na última posição da coluna
				{
					if (board.at((int)line - 65).at((int)column - 97 + word.length()) == '.')
						board.at((int)line - 65).at((int)column - 97 + word.length()) = '#';
				}

				//Colocar sinónimos no map
				pos << line << column << direction;
				position = pos.str();
				p = make_pair(position, word); //Cria par com a posição e a palavra para colocar no map words
				words.insert(p);
			}
			else //Coloca a palavra se conseguir (se encaixar com a letra já existente)
			{
				if (compareWords(direction, (int)line, (int)column, word))
				{
					int wordPosition;

					if ((int)column - 97 != 0) //Caso não esteja na primeira posição da tabela
					{
						if (board.at((int)line - 65).at((int)column - 98) == '.')
							board.at((int)line - 65).at((int)column - 98) = '#';
					}

					wordPosition = 0;

					for (unsigned int i = (unsigned int)column - 97; i < (unsigned int)column + word.length() - 97; i++)
					{
						board.at((int)line - 65).at(i) = word.at(wordPosition);
						wordPosition++;
					}

					if ((int)column - 97 + word.length() < numColumns) //Caso a última letra não esteja na última posição da coluna
					{
						if (board.at((int)line - 65).at((int)column - 97 + word.length()) == '.')
							board.at((int)line - 65).at((int)column - 97 + word.length()) = '#';
					}
					//Colocar sinónimos no map
					pos << line << column << direction;
					position = pos.str();
					p = make_pair(position, word); //Cria par com a posição e a palavra para colocar no map words
					words.insert(p);
				}
				else
					cout << "Invalid word for this postion.";
			}
		}
	}
	else
		cout << "The word is too big";
}

void Board::showBoard() //Mostra o tabuleiro 
{
	cout << endl << endl;

	cout << setw(2) << "   ";

	setcolor(12);
	for (size_t i = 0; i < board.at(0).size(); i++)
	{
		cout << setw(3) << (char)(i + 97);
	}

	for (size_t i = 0; i < board.size(); i++)
	{
		setcolor(12, 0);
		cout << endl << setw(2) << (char)(i + 65) << " ";
		setcolor(0, 7);
		for (size_t j = 0; j < board.at(i).size(); j++)
		{
			if (board.at(i).at(j) == '#')
			{
				setcolor(12, 7);
				cout << setw(3) << board.at(i).at(j);
				setcolor(0, 7);
			}
			else
				cout << setw(3) << board.at(i).at(j);
		}
		cout << " ";

		setcolor(0); //Mudar a cor para preto para prevenir a extenção do fundo quando se aumenta a tela da linha de comandos
		cout << "."; //cout de um eapaço: pode ser qualquer outro carater, desde que esteja à frente do tabuleiro e não interfira com o mesmo
	}

	setcolor(15, 0);
	cout << endl << endl;
}

void Board::saveBoard(const string &dictionaryFileName) //Guarda o tabuleiro num ficheiro de texto
{
	string num = to_string(numberBoard); //Número do tabuleiro será incluído no nome do ficheiro

	while (num.length() < 3) //O número tem que ter 3 carateres para ser do tipo XXX
	{
		num = "0" + num;
	}

	ofstream newBoard("b" + num + ".txt"); //ficheiro do tipo bXXX.txt

	string line;

	newBoard << dictionaryFileName << endl << endl; //Nome do dicionário é colocado no ficheiro de texto, juntamente com uma linha em branco

	for (size_t i = 0; i < board.size(); i++) //Coloca-se o tabuleiro do ficheiro
	{
		line.resize(0);
		for (size_t j = 0; j < board.at(i).size(); j++)
			line = line + board.at(i).at(j);

		newBoard << line << endl;
	}

	newBoard << endl; //Linha vazia no tabuleiro

	for (auto e : words) //Coloca-se as posições seguidas das palavras no ficheiro
	{
		newBoard << e.first << " " << e.second << endl;
	}

	newBoard.close();

	numberBoard++; //Adicona-se 1 ao número do ficheiro
}

bool Board::wordFits(size_t wordSize, char line, char column, char direction) //Função que verifica se uma palavra cabe num certo espaço -> retorna falso se começa na posição errada ou tem comprimento inadequado à posição
{
	unsigned int numl, numc; //número da linha e da coluna
	bool fits = false;
	if (direction == 'V')
	{
		numl = (int)line - 65;

		if (numLines - numl >= wordSize)
			fits = true;
	}
	else
	{
		numc = (int)column - 97;

		if (numColumns - numc >= wordSize)
			fits = true;
	}
	return fits;
}

bool Board::existsWord(const unsigned int &l, const unsigned int &c, const char &d) //Verifica se já existe uma palavra na posição dada
{
	if (board.at(l - 65).at(c - 97) == '.') // Se tiver um . não existe palavra
		return false;
	else if (d == 'V')
	{
		if (board.at(l - 64).at(c - 97) != '#') //Se contiver um # na 2ª posição não pode colocar a palavra, caso contrário há outra condições que verificam se pode ou não
			return false;
		else
			return true;
	}
	else
	{
		if (board.at(l - 65).at(c - 96) != '#') //Se contiver um # na 2ª posição não pode colocar a palavra, caso contrário há outra condições que verificam se pode ou não
			return false;
		else
			return true;
	}
}

bool Board::existsWord(const string &word) //Verifica se a palavra já foi incluída no tabuleiro
{
	for (auto e : words)
	{
		if (e.second == word)
			return true;
	}

	return false;
}

bool Board::anotherWordVert(unsigned int inicialLine, unsigned int finalLine, unsigned int column) //Verifica se existe outra palavra na mesma coluna (retorna true se encontrar)
{
	unsigned int i;
	for (i = inicialLine; i < finalLine; i++)
	{
		if (board.at(i).at(column) != '.')
			return true; //Existe uma letra de outra palavra nessa coluna
	}

	if (i < numLines - 1)
	{
		if (board.at(i).at(column) != '.' && board.at(i).at(column) != '#') //Acontece se a palavra tem outro carater à frente
			return true;
	}

	return false;
}

bool Board::anotherWordHor(unsigned int inicialColumn, unsigned int finalColumn, unsigned int line) //Verifica se existe outra palavra na mesma linha (retorna true se encontrar)
{
	unsigned int i;

	for (i = inicialColumn; i < finalColumn; i++)
	{
		if (board.at(line).at(i) != '.')
			return true; //Existe uma letra de outra palavra nessa coluna
	}

	if (i < numColumns - 1)
	{
		if (board.at(line).at(i) != '.' && board.at(line).at(i) != '#')  //Acontece se a palavra tem outro carater à frente
			return true;
	}
	return false;
}

bool Board::compareWords(char direction, unsigned int inicialLine, unsigned int inicialColumn, string word) //Compara a palavra a colocar com as letras que já se encontram no tabuleiro
{
	if (direction == 'V')
	{
		int i = 0;
		size_t j = inicialLine - 65;
		for (j; j < inicialLine + word.length() - 65; j++)
		{
			if (board.at(j).at(inicialColumn - 97) == '.')
			{
				i++;
				continue;
			}
			else if (board.at(j).at(inicialColumn - 97) == word.at(i))
			{
				i++;
				continue;
			}
			else
				return false; //Palavra não encaixa com as letras já existentes
		}

		return true;
	}
	else
	{
		int i = 0;
		size_t j = inicialColumn - 97;
		for (j; j < inicialColumn + word.length() - 97; j++)
		{
			if (board.at(inicialLine - 65).at(j) == '.')

			{
				i++;
				continue;
			}
			else if (board.at(inicialLine - 65).at(j) == word.at(i))
			{
				i++;
				continue;
			}
			else
				return false; //Palavra não encaixa com as letras já existentes
		}

		return true;
	}
}

unsigned int Board::getLines() //Devolve o número de linhas
{
	return numLines;
}

unsigned int Board::getColumns() //Devolve o número de colunas
{
	return numColumns;
}

void Board::removeWord(const char &line, const char &column, const char &direction) //Remove a palavra que se encontra numa determinada posição
{
	//O método de remoção das palavras consiste em eliminar o par que contém a posição especificada
	//Após a eliminação deste par são colocadas todas as outras palavras no tabuleiro

	ostringstream pos;
	string position;
	pos << line << column << direction;
	position = pos.str();

	if (words.find(position) != words.end())
	{
		map<string, string>::iterator it = words.find(position);
		words.erase(it); //Remoção do par

		for (unsigned int i = 0; i < numLines; i++)
			for (unsigned int j = 0; j < numColumns; j++)
				board.at(i).at(j) = '.'; //Todas as posições da tabela ficarão com um ponto

		for (auto e : words)
		{
			putWord(e.second, e.first.at(0), e.first.at(1), e.first.at(2)); //Coloca cada palavra no board
		}
	}
	else
		cout << endl << "There is no word in this position";
}

char Board::getChar(const char &line, const char &column) //Retorna o carater da posição dada
{
	return board.at((int)line - 65).at((int)column - 97);
}

void Board::getWords(vector<string> &v) //Retorna no vetor passado como referência, todas as palavras já colocadas no tabuleiro
{
	for (auto e : words)
		v.push_back(e.second);
}

bool Board::validWord(const string &word, const char &line, const char &column, const char &direction) //Função que verifica se uma palavra é válida no tabuleiro
{
	if (wordFits(word.length(), line, column, direction))
	{
		if (direction == 'V')
		{
			if (!anotherWordVert((int)line - 65, (int)line + word.length() - 65, (int)column - 97)) //Coloca a palavra normalmente
				return true;
			else //Coloca a palavra se conseguir (se encaixar com a letra já existente)
			{
				if ((int)line + word.length() - 65 < numLines)
				{
					if (board.at((int)line + word.length() - 65).at((int)column - 97) != '.')
						return false;
					else if (compareWords(direction, (int)line, (int)column, word)) //Igual ao anterior: possível colocar na mesma condição com um or (||), mas atrasaria o programa caso se colocasse a palavra normalmente
						return true;
					else
						return false;
				}
				else if (compareWords(direction, (int)line, (int)column, word)) //Igual ao anterior: possível colocar na mesma condição com um or (||), mas atrasaria o programa caso se colocasse a palavra normalmente
					return true;
				else
					return false;
			}
		}
		else
		{
			if (!anotherWordHor((int)column - 97, (int)column + word.length() - 97, (int)line - 65))  //Coloca a palavra normalmente
				return true;
			else //Coloca a palavra se conseguir (se encaixar com a letra já existente)
			{
				if ((int)column + word.length() - 97 < numColumns)
				{
					if (board.at((int)line - 65).at((int)column + word.length() - 97) != '.')
						return false;
					else if (compareWords(direction, (int)line, (int)column, word))
						return true;
					else
						return false;
				}
				else if (compareWords(direction, (int)line, (int)column, word))
					return true;
				else
					return false;
			}
		}
	}
	else
		return false;
}

bool Board::isFull() //Função que verifica se o tabuleiro está cheio
{
	for (unsigned int i = 0; i < numLines; i++)
		for (unsigned int j = 0; j < numColumns; j++)
		{
			if (board.at(i).at(j) == '.') //Caso o tabuleiro contenha algum ponto, não está cheio, retornando falso
				return false;
		}

	return true;
}

void Board::setNumberBoard() //Altera o número do tabuleiro conforme os que já estão guardados
{
	ifstream file;
	string number;

	unsigned int num = 1;

	while (true)
	{
		number = to_string(num);
		while (number.length() < 3)
		{
			number = "0" + number;
		}

		file.open("b" + number + ".txt"); //ficheiro do tipo bXXX.txt

		if (file.fail())
		{
			numberBoard = num;
			break;
		}

		num++;
		file.close();
	}
}

void Board::putBlack() //Coloca todos os pontos restantes no tabuleiro como um #
{
	for (size_t i = 0; i < numLines; i++)
		for (size_t j = 0; j < numColumns; j++)
		{
			if (board.at(i).at(j) == '.')
				board.at(i).at(j) = '#';
		}
}

vector<string> Board::getWords() //Função que retorna todas as palavras do tabuleiro
{
	/*
	  As palavras do tabuleiro podem ser:
			-colocadas pelo utilizador;
			-formadas pela interseção/junção das palavras inseridas pelo utilizador
	*/

	string word = "";
	vector <string> v;

	//Palavras horizontais
	for (size_t i = 0; i < numLines; i++)
		for (size_t j = 0; j < numColumns; j++)
		{
			if (j == numColumns - 1) //Se está na última posição da coluna
			{
				//É inserida uma palavra no vetor v se tiver comprimento superior a 1

				if (word.length() > 0 && board.at(i).at(j) != '.' && board.at(i).at(j) != '#') //Palavra tem comprimento igual ou maior a 1 e não tem um . ou # na última posição
				{
					word = word + board.at(i).at(j);
					v.push_back(word);
					word.resize(0);
				}
				else if (word.length() > 1) //palavra tem comprimento superior a 1
				{
					v.push_back(word);
					word.resize(0);
				}
				else
					continue;
			}
			else if (board.at(i).at(j) == '.' || board.at(i).at(j) == '#') //Se encontra um ponto ou um #
			{
				if (word.length() <= 1) //É ignorada a palavra se tiver comprimento igual ou inferior a 1 
				{
					word.resize(0);
					continue;
				}
				else //Senão é colocada no vetor v
				{
					v.push_back(word);
					word.resize(0);
				}
			}
			else //Caso geral: adiciona-se a letra à palavra
				word = word + board.at(i).at(j);
		}

	word.resize(0);

	//Palavras Verticais 
	for (size_t i = 0; i < numColumns; i++)
		for (size_t j = 0; j < numLines; j++)
		{
			if (j == numLines - 1) //Se está na última posição da linha
			{
				//É inserida uma palavra no vetor v se tiver comprimento superior a 1

				if (word.length() > 0 && board.at(j).at(i) != '.' && board.at(j).at(i) != '#') //Palavra tem comprimento igual ou maior a 1 e não tem um . ou # na última posição
				{
					word = word + board.at(j).at(i);
					v.push_back(word);
					word.resize(0);
				}
				else if (word.length() > 1) //palavra tem comprimento superior a 1
				{
					v.push_back(word);
					word.resize(0);
				}
				else
					continue;
			}
			else if (board.at(j).at(i) == '.' || board.at(j).at(i) == '#') //Se encontra um ponto ou um #
			{
				if (word.length() <= 1) //É ignorada a palavra se tiver comprimento igual ou inferior a 1 
				{
					word.resize(0);
					continue;
				}
				else //Senão é colocada no vetor v
				{
					v.push_back(word);
					word.resize(0);
				}
			}
			else //Caso geral: adiciona-se a letra à palavra
				word = word + board.at(j).at(i);
		}

	return v;
}