#include "Board.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

using namespace std;

//Declara��o de fun��es que est�o definidas no ficheiro cwcreator.cpp
//Estas fun��es est�o aqui definidas, pois s�o usadas neste ficheiro
void setcolor(unsigned int color);
void setcolor(unsigned int color, unsigned int background_color);

unsigned int Board::numberBoard = 1; //N�mero do tabuleiro ser� inicializado apenas uma vez, pois � uma fun��o static

Board::Board(const unsigned int lines, const unsigned int columns) //Cria��o de um tabuleiro atrav�s do n�mero de linhas e colunas
{
	numLines = lines;
	numColumns = columns;

	board.resize(numLines);

	for (size_t i = 0; i < board.size(); i++)
	{
		board.at(i).resize(numColumns);
	}

	for (size_t i = 0; i < numLines; i++) //Inicialmente todas as posi��es conter�o pontos 
		for (size_t j = 0; j < numColumns; j++)
			board.at(i).at(j) = '.';
}

Board::Board(ifstream &file) //Retira o tabuleiro de um ficheiro criado anteriormente pelo programa cwcreator
{
	string line;

	vector <char> v;
	vector <vector <char>> b;

	board.resize(0); //board � reinicializado

	//Retira o tabuleiro do ficheiro
	while (!file.eof())
	{
		getline(file, line);
		if (line.length() == 0) //Se ao percorrer o ficheiro encontrar uma linha vazia, significa que j� foi carregado todo o ficheiro
			break;

		v.resize(0);

		copy(line.begin(), line.end(), back_inserter(v)); //Fun��o que copia a string line para um vector<char> v

		board.push_back(v);
	}

	numColumns = v.size();
	numLines = board.size();

	string position;
	string word;
	pair<string, string> p;

	//Retira as posi��es e as palavras que est�o no tabuleiro do ficheiro
	while (!file.eof())
	{
		getline(file, line);

		if (line.length() == 0)
			break;

		position = line.substr(0, 3);
		word = line.substr(4, line.length() - 4);

		p = make_pair(position, word); //par posi��o - palavra
		words.insert(p); //words cont�m pares com as posi��es e a palavra na posi��o
	}
}

void Board::putWord(string &word, const char &line, const char &column, const char &direction) //Fun��o que coloca uma palavra no tabuleiro e, consequentemente, coloca a sua posi��o inicial e os sin�nimos no map
{
	ostringstream pos; //String stream que ajudar� a criar a posi��o
	string position; //String com a posi��o da palavra
	vector<string> synonyms; //Vetor com os sin�nimos
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
				if ((int)line - 65 != 0) //Caso n�o esteja na primeira posi��o da tabela
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

				if ((int)line - 65 + word.length() < numLines) //Caso a �ltima letra n�o esteja na �ltima posi��o da linha
				{
					if (board.at((int)line - 65 + word.length()).at((int)column - 97) == '.')
						board.at((int)line - 65 + word.length()).at((int)column - 97) = '#';
				}

				//Colocar sin�nimos no map
				pos << line << column << direction;
				position = pos.str();
				p = make_pair(position, word); //Cria par com a posi��o e a palavra para colocar no map words
				words.insert(p);
			}
			else //Coloca a palavra se conseguir (se encaixar com a letra j� existente)
			{
				if (compareWords(direction, (int)line, (int)column, word)) //Igual ao anterior: poss�vel colocar na mesma condi��o com um or (||), mas atrasaria o programa caso se colocasse a palavra normalmente
				{
					int wordPosition;
					if ((int)line - 65 != 0) //Caso n�o esteja na primeira posi��o da tabela
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

					if ((int)line - 65 + word.length() < numLines) //Caso a �ltima letra n�o esteja na �ltima posi��o da linha
					{
						if (board.at((int)line - 65 + word.length()).at((int)column - 97) == '.')
							board.at((int)line - 65 + word.length()).at((int)column - 97) = '#';
					}

					//Colocar sin�nimos no map
					pos << line << column << direction;
					position = pos.str();
					p = make_pair(position, word); //Cria par com a posi��o e a palavra para colocar no map words
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

				if ((int)column - 97 != 0) //Caso n�o esteja na primeira posi��o da tabela
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

				if ((int)column - 97 + word.length() < numColumns) //Caso a �ltima letra n�o esteja na �ltima posi��o da coluna
				{
					if (board.at((int)line - 65).at((int)column - 97 + word.length()) == '.')
						board.at((int)line - 65).at((int)column - 97 + word.length()) = '#';
				}

				//Colocar sin�nimos no map
				pos << line << column << direction;
				position = pos.str();
				p = make_pair(position, word); //Cria par com a posi��o e a palavra para colocar no map words
				words.insert(p);
			}
			else //Coloca a palavra se conseguir (se encaixar com a letra j� existente)
			{
				if (compareWords(direction, (int)line, (int)column, word))
				{
					int wordPosition;

					if ((int)column - 97 != 0) //Caso n�o esteja na primeira posi��o da tabela
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

					if ((int)column - 97 + word.length() < numColumns) //Caso a �ltima letra n�o esteja na �ltima posi��o da coluna
					{
						if (board.at((int)line - 65).at((int)column - 97 + word.length()) == '.')
							board.at((int)line - 65).at((int)column - 97 + word.length()) = '#';
					}
					//Colocar sin�nimos no map
					pos << line << column << direction;
					position = pos.str();
					p = make_pair(position, word); //Cria par com a posi��o e a palavra para colocar no map words
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

		setcolor(0); //Mudar a cor para preto para prevenir a exten��o do fundo quando se aumenta a tela da linha de comandos
		cout << "."; //cout de um eapa�o: pode ser qualquer outro carater, desde que esteja � frente do tabuleiro e n�o interfira com o mesmo
	}

	setcolor(15, 0);
	cout << endl << endl;
}

void Board::saveBoard(const string &dictionaryFileName) //Guarda o tabuleiro num ficheiro de texto
{
	string num = to_string(numberBoard); //N�mero do tabuleiro ser� inclu�do no nome do ficheiro

	while (num.length() < 3) //O n�mero tem que ter 3 carateres para ser do tipo XXX
	{
		num = "0" + num;
	}

	ofstream newBoard("b" + num + ".txt"); //ficheiro do tipo bXXX.txt

	string line;

	newBoard << dictionaryFileName << endl << endl; //Nome do dicion�rio � colocado no ficheiro de texto, juntamente com uma linha em branco

	for (size_t i = 0; i < board.size(); i++) //Coloca-se o tabuleiro do ficheiro
	{
		line.resize(0);
		for (size_t j = 0; j < board.at(i).size(); j++)
			line = line + board.at(i).at(j);

		newBoard << line << endl;
	}

	newBoard << endl; //Linha vazia no tabuleiro

	for (auto e : words) //Coloca-se as posi��es seguidas das palavras no ficheiro
	{
		newBoard << e.first << " " << e.second << endl;
	}

	newBoard.close();

	numberBoard++; //Adicona-se 1 ao n�mero do ficheiro
}

bool Board::wordFits(size_t wordSize, char line, char column, char direction) //Fun��o que verifica se uma palavra cabe num certo espa�o -> retorna falso se come�a na posi��o errada ou tem comprimento inadequado � posi��o
{
	unsigned int numl, numc; //n�mero da linha e da coluna
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

bool Board::existsWord(const unsigned int &l, const unsigned int &c, const char &d) //Verifica se j� existe uma palavra na posi��o dada
{
	if (board.at(l - 65).at(c - 97) == '.') // Se tiver um . n�o existe palavra
		return false;
	else if (d == 'V')
	{
		if (board.at(l - 64).at(c - 97) != '#') //Se contiver um # na 2� posi��o n�o pode colocar a palavra, caso contr�rio h� outra condi��es que verificam se pode ou n�o
			return false;
		else
			return true;
	}
	else
	{
		if (board.at(l - 65).at(c - 96) != '#') //Se contiver um # na 2� posi��o n�o pode colocar a palavra, caso contr�rio h� outra condi��es que verificam se pode ou n�o
			return false;
		else
			return true;
	}
}

bool Board::existsWord(const string &word) //Verifica se a palavra j� foi inclu�da no tabuleiro
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
		if (board.at(i).at(column) != '.' && board.at(i).at(column) != '#') //Acontece se a palavra tem outro carater � frente
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
		if (board.at(line).at(i) != '.' && board.at(line).at(i) != '#')  //Acontece se a palavra tem outro carater � frente
			return true;
	}
	return false;
}

bool Board::compareWords(char direction, unsigned int inicialLine, unsigned int inicialColumn, string word) //Compara a palavra a colocar com as letras que j� se encontram no tabuleiro
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
				return false; //Palavra n�o encaixa com as letras j� existentes
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
				return false; //Palavra n�o encaixa com as letras j� existentes
		}

		return true;
	}
}

unsigned int Board::getLines() //Devolve o n�mero de linhas
{
	return numLines;
}

unsigned int Board::getColumns() //Devolve o n�mero de colunas
{
	return numColumns;
}

void Board::removeWord(const char &line, const char &column, const char &direction) //Remove a palavra que se encontra numa determinada posi��o
{
	//O m�todo de remo��o das palavras consiste em eliminar o par que cont�m a posi��o especificada
	//Ap�s a elimina��o deste par s�o colocadas todas as outras palavras no tabuleiro

	ostringstream pos;
	string position;
	pos << line << column << direction;
	position = pos.str();

	if (words.find(position) != words.end())
	{
		map<string, string>::iterator it = words.find(position);
		words.erase(it); //Remo��o do par

		for (unsigned int i = 0; i < numLines; i++)
			for (unsigned int j = 0; j < numColumns; j++)
				board.at(i).at(j) = '.'; //Todas as posi��es da tabela ficar�o com um ponto

		for (auto e : words)
		{
			putWord(e.second, e.first.at(0), e.first.at(1), e.first.at(2)); //Coloca cada palavra no board
		}
	}
	else
		cout << endl << "There is no word in this position";
}

char Board::getChar(const char &line, const char &column) //Retorna o carater da posi��o dada
{
	return board.at((int)line - 65).at((int)column - 97);
}

void Board::getWords(vector<string> &v) //Retorna no vetor passado como refer�ncia, todas as palavras j� colocadas no tabuleiro
{
	for (auto e : words)
		v.push_back(e.second);
}

bool Board::validWord(const string &word, const char &line, const char &column, const char &direction) //Fun��o que verifica se uma palavra � v�lida no tabuleiro
{
	if (wordFits(word.length(), line, column, direction))
	{
		if (direction == 'V')
		{
			if (!anotherWordVert((int)line - 65, (int)line + word.length() - 65, (int)column - 97)) //Coloca a palavra normalmente
				return true;
			else //Coloca a palavra se conseguir (se encaixar com a letra j� existente)
			{
				if ((int)line + word.length() - 65 < numLines)
				{
					if (board.at((int)line + word.length() - 65).at((int)column - 97) != '.')
						return false;
					else if (compareWords(direction, (int)line, (int)column, word)) //Igual ao anterior: poss�vel colocar na mesma condi��o com um or (||), mas atrasaria o programa caso se colocasse a palavra normalmente
						return true;
					else
						return false;
				}
				else if (compareWords(direction, (int)line, (int)column, word)) //Igual ao anterior: poss�vel colocar na mesma condi��o com um or (||), mas atrasaria o programa caso se colocasse a palavra normalmente
					return true;
				else
					return false;
			}
		}
		else
		{
			if (!anotherWordHor((int)column - 97, (int)column + word.length() - 97, (int)line - 65))  //Coloca a palavra normalmente
				return true;
			else //Coloca a palavra se conseguir (se encaixar com a letra j� existente)
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

bool Board::isFull() //Fun��o que verifica se o tabuleiro est� cheio
{
	for (unsigned int i = 0; i < numLines; i++)
		for (unsigned int j = 0; j < numColumns; j++)
		{
			if (board.at(i).at(j) == '.') //Caso o tabuleiro contenha algum ponto, n�o est� cheio, retornando falso
				return false;
		}

	return true;
}

void Board::setNumberBoard() //Altera o n�mero do tabuleiro conforme os que j� est�o guardados
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

vector<string> Board::getWords() //Fun��o que retorna todas as palavras do tabuleiro
{
	/*
	  As palavras do tabuleiro podem ser:
			-colocadas pelo utilizador;
			-formadas pela interse��o/jun��o das palavras inseridas pelo utilizador
	*/

	string word = "";
	vector <string> v;

	//Palavras horizontais
	for (size_t i = 0; i < numLines; i++)
		for (size_t j = 0; j < numColumns; j++)
		{
			if (j == numColumns - 1) //Se est� na �ltima posi��o da coluna
			{
				//� inserida uma palavra no vetor v se tiver comprimento superior a 1

				if (word.length() > 0 && board.at(i).at(j) != '.' && board.at(i).at(j) != '#') //Palavra tem comprimento igual ou maior a 1 e n�o tem um . ou # na �ltima posi��o
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
				if (word.length() <= 1) //� ignorada a palavra se tiver comprimento igual ou inferior a 1 
				{
					word.resize(0);
					continue;
				}
				else //Sen�o � colocada no vetor v
				{
					v.push_back(word);
					word.resize(0);
				}
			}
			else //Caso geral: adiciona-se a letra � palavra
				word = word + board.at(i).at(j);
		}

	word.resize(0);

	//Palavras Verticais 
	for (size_t i = 0; i < numColumns; i++)
		for (size_t j = 0; j < numLines; j++)
		{
			if (j == numLines - 1) //Se est� na �ltima posi��o da linha
			{
				//� inserida uma palavra no vetor v se tiver comprimento superior a 1

				if (word.length() > 0 && board.at(j).at(i) != '.' && board.at(j).at(i) != '#') //Palavra tem comprimento igual ou maior a 1 e n�o tem um . ou # na �ltima posi��o
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
				if (word.length() <= 1) //� ignorada a palavra se tiver comprimento igual ou inferior a 1 
				{
					word.resize(0);
					continue;
				}
				else //Sen�o � colocada no vetor v
				{
					v.push_back(word);
					word.resize(0);
				}
			}
			else //Caso geral: adiciona-se a letra � palavra
				word = word + board.at(j).at(i);
		}

	return v;
}