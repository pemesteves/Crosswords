#include "Board.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

using namespace std;

//Declaração de funções que estão definidas no ficheiro cwplayer.cpp
//Estas funções estão aqui definidas, pois são usadas neste ficheiro
void setcolor(unsigned int color);
void setcolor(unsigned int color, unsigned int background_color);

Board::Board(ifstream &file) //Criador do tabuleiro: retira o tabuleiro de um ficheiro criado pelo programa cwcreator
{
	string line;

	vector <char> v; //Vetor que conterá cada linha do tabuleiro

	board.resize(0); //Inicialmente o tabuleiro não tem elementos

	while (!file.eof())
	{
		getline(file, line);
		if (line.length() == 0)
			break;
		/*Embora seja usado um while(!file.eof()), o ciclo acaba quando o programa encontrar uma linha vazia
		  Isto acontece, pois em qualquer ficheiro do tipo bXXX.txt, criado pelo programa cwcreator, o tabuleiro é criado seguido de uma linha vazia
		  e por fim as palavras todas que nele se encontram*/

		v.resize(0);

		/*A função seguinte copia a string line para o vector<char> v
		  Notar atrás a utilização de v.resize(0)
		  Sem esta chamada, o conteúdo do vetor v, não seria atualizado, o que faria com que à medida que se fazia o b.push_back(v) no final deste bloco de código,
		  o número de colunas aumentaria*/
		copy(line.begin(), line.end(), back_inserter(v));

		board.push_back(v); //push_back de cada linha do tabuleiro
	}

	numColumns = v.size(); //Número de colunas é dado pelo tamanho de qualquer vector <char>, uma vez que todas as linhas têm o mesmo número de colunas
	numLines = board.size(); //O número de linhas é dado pelo tamanho do vector <vector <char>>

	string position; //Posição reitrada do ficheiro
	string word; //Palavra retirada do ficheiro
	pair<string, string> p; //Par que será criado sempre com uma posição e respetiva palavra
	while (!file.eof()) //Introdução das palavras retiradas do ficheiro (após o tabuleiro) num map chamado words
	{
		getline(file, line);

		if (line.length() == 0)
			break;

		/*Como cada linha retirada do ficheiro quando se está a retirar as palavras, contém a posição seguida de um espaço e logo a segui a palavra,
		  é utilizada a função substr associada a string para retirar cada posição e cada palavra. Espaço é eliminado*/

		position = line.substr(0, 3); //Posição tem 3 carateres: linha, coluna e direção. Começa na posição 0 da string
		word = line.substr(4, line.length() - 4); //Palavra começa na posição 4 (0, 1 e 2 -> direção; 3 -> espaço) e o seu tamanho será o tamanho da linha - 4

		//Criação e inserção do par com a posição e a palavra
		p = make_pair(position, word);
		words.insert(p);
	}

	//playerSolution será um tabuleiro com o mesmo tamanho de board
	playerSolution.resize(numLines);
	for (size_t i = 0; i < numLines; i++)
		playerSolution.at(i).resize(numColumns);

	/*Criar o tabuleiro que o utilizador verá:
			-Quando no board temos um '#' ou um '.', no playerSolution será igualmente um '#' (adição do ponto pois o utilizador no cwcreator poderá ter-se esquecido de finalizar o seu tabuleiro
			-Quando no board temos qualquer letra, será um '.' no playerSolution
	Permite fazer com que o utilizador consiga visualizar perfeitamente o sítio onde tem que colocar a sua palavra
	*/
	for (size_t i = 0; i < numLines; i++)
		for (size_t j = 0; j < numColumns; j++)
		{
			if (board.at(i).at(j) == '#' || board.at(i).at(j) == '.')
				playerSolution.at(i).at(j) = '#';
			else
				playerSolution.at(i).at(j) = '.';
		}
}

void Board::putWord(string &word, const char &line, const char &column, const char &direction) //Função que coloca uma palavra no tabuleiro e, consequentemente, coloca a sua posição inicial e os sinónimos no map
{
	ostringstream pos; //String stream que ajudará a criar a posição
	string position; //String com a posição da palavra
	vector<string> synonyms; //Vetor com os sinónimos
	pair<string, string> p;

	if (playerSolution.at((int)line - 65).at((int)column - 97) == '#') //Se na posição escolhida estiver um '#' o utilizador não conseguirá introduzir a palavra
		cout << "You can't put a word onto a #";
	else if (wordFits(word.length(), line, column, direction))
	{
		if (direction == 'V')
		{
			if (!anotherWordVert((int)line - 65, (int)line + word.length() - 65, (int)column - 97)) //Coloca a palavra normalmente
			{
				int wordPosition;

				wordPosition = 0;
				for (unsigned int i = (unsigned int)line - 65; i < (unsigned int)line + word.length() - 65; i++)
				{
					playerSolution.at(i).at((int)column - 97) = word.at(wordPosition);
					wordPosition++;
				}

				//Colocar sinónimos no map
				pos << line << column << direction;
				position = pos.str();
				p = make_pair(position, word);
				playerWords.insert(p);
			}
			else //Coloca a palavra se conseguir (se encaixar com a letra já existente)
			{
				if (compareWords(direction, (int)line, (int)column, word)) //Igual ao anterior: possível colocar na mesma condição com um or (||), mas atrasaria o programa caso se colocasse a palavra normalmente
				{
					int wordPosition;

					wordPosition = 0;
					for (unsigned int i = (unsigned int)line - 65; i < (unsigned int)line + word.length() - 65; i++)
					{
						playerSolution.at(i).at((int)column - 97) = word.at(wordPosition);
						wordPosition++;
					}

					//Colocar sinónimos no map
					pos << line << column << direction;
					position = pos.str();
					p = make_pair(position, word);
					playerWords.insert(p);
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

				wordPosition = 0;
				for (unsigned int i = (unsigned int)column - 97; i < (unsigned int)column + word.length() - 97; i++)
				{
					playerSolution.at((int)line - 65).at(i) = word.at(wordPosition);
					wordPosition++;
				}

				//Colocar sinónimos no map
				pos << line << column << direction;
				position = pos.str();
				p = make_pair(position, word);
				playerWords.insert(p);
			}
			else //Coloca a palavra se conseguir (se encaixar com a letra já existente)
			{
				if (compareWords(direction, (int)line, (int)column, word))
				{
					int wordPosition;

					wordPosition = 0;
					for (unsigned int i = (unsigned int)column - 97; i < (unsigned int)column + word.length() - 97; i++)
					{
						playerSolution.at((int)line - 65).at(i) = word.at(wordPosition);
						wordPosition++;
					}

					//Colocar sinónimos no map
					pos << line << column << direction;
					position = pos.str();
					p = make_pair(position, word);
					playerWords.insert(p);
				}
				else
					cout << "Invalid word for this postion.";
			}
		}
	}
	else
		cout << "This word doesn't fit in this position. Maybe you select a wrong postion or the length of the word isn't correct.";
}

void Board::showBoard() //Mostra o tabuleiro 
{
	cout << endl << endl;

	cout << setw(2) << "   ";

	setcolor(12);
	for (size_t i = 0; i < playerSolution.at(0).size(); i++)
	{
		cout << setw(3) << (char)(i + 97);
	}

	for (size_t i = 0; i < playerSolution.size(); i++)
	{
		setcolor(12, 0);
		cout << endl << setw(2) << (char)(i + 65) << " ";
		setcolor(0, 7);
		for (size_t j = 0; j < playerSolution.at(i).size(); j++)
		{
			if (playerSolution.at(i).at(j) == '#')
			{
				setcolor(12, 7);
				cout << setw(3) << playerSolution.at(i).at(j);
				setcolor(0, 7);
			}
			else
				cout << setw(3) << playerSolution.at(i).at(j);
		}
		cout << " ";

		setcolor(0); //Mudar a cor para preto para prevenir a extenção do fundo quando se aumenta a tela da linha de comandos
		cout << "."; //cout de um eapaço: pode ser qualquer outro carater, desde que esteja à frente do tabuleiro e não interfira com o mesmo
	}

	setcolor(15, 0);
	cout << endl << endl;
}

bool Board::wordFits(size_t wordSize, char line, char column, char direction) //Função que verifica se uma palavra cabe num certo espaço -> retorna falso se começa na posição errada ou tem comprimento inadequado à posição
{
	if (direction == 'V')
	{
		if ((int)line - 65 == 0) //Palavra começa na 1ª posição
		{
			for (size_t i = 0; i < wordSize; i++)
			{
				if (playerSolution.at((int)line - 65 + i).at((int)column - 97) == '#') //Se na 1ª posição houver um #, é inválida
					return false;
			}

			if ((int)line - 65 + wordSize != numLines)
			{
				if (playerSolution.at((int)line - 65 + wordSize).at((int)column - 97) != '#') //Se na posição a seguir à última letra, já tiver algo diferente de um #, é inválida
					return false;
			}

			return true;
		}
		else if (playerSolution.at((int)line - 66).at((int)column - 97) != '#') //Se antes da palavra tiver algo diferente de um #, é inválida
		{
			return false;
		}
		else if ((int)line - 65 + wordSize != numLines) //Palavra não chega à última posição
		{
			{
				if (playerSolution.at((int)line - 65 + wordSize).at((int)column - 97) != '#') //Se na posição a seguir à última letra, já tiver algo diferente de um #, é inválida
					return false;
			}
			return true;
		}
		else
		{
			for (size_t i = 0; i < wordSize; i++) //Percorre a palavra à procura de #: se encontrar algum é inváldia
			{
				if (playerSolution.at((int)line - 65 + i).at((int)column - 97) == '#')
					return false;
			}

			if ((int)line - 65 + wordSize != numLines)
			{
				if (playerSolution.at((int)line - 65 + wordSize).at((int)column - 97) != '#') //Se na posição a seguir à última letra, já tiver algo diferente de um #, é inválida
					return false;
			}

			return true;
		}
	}
	else
	{
		if ((int)column - 97 == 0) //Palavra começa na 1ª posição
		{
			for (size_t i = 0; i < wordSize; i++)
			{
				if (playerSolution.at((int)line - 65).at((int)column - 97 + i) == '#') //Se na 1ª posição houver um #, é inválida
					return false;
			}

			if ((int)column - 97 + wordSize != numColumns)
			{
				if (playerSolution.at((int)line - 65).at((int)column - 97 + wordSize) != '#') //Se na posição a seguir à última letra, já tiver algo diferente de um #, é inválida
					return false;
			}

			return true;
		}
		else if (playerSolution.at((int)line - 65).at((int)column - 98) != '#') //Se antes da palavra tiver algo diferente de um #, é inválida
			return false;
		else if ((int)column - 97 + wordSize != numColumns) //Palavra não chega à última posição
		{
			{
				if (playerSolution.at((int)line - 65).at((int)column - 97 + wordSize) != '#') //Se na posição a seguir à última letra, já tiver algo diferente de um #, é inválida
					return false;
			}
			return true;
		}
		else
		{
			for (size_t i = 0; i < wordSize; i++) //Percorre a palavra à procura de #: se encontrar algum é inváldia
			{
				if (playerSolution.at((int)line - 65).at((int)column - 97 + i) == '#')
					return false;
			}

			if ((int)column - 97 + wordSize != numColumns)
			{
				if (playerSolution.at((int)line - 65).at((int)column - 97 + wordSize) != '#') //Se na posição a seguir à última letra, já tiver algo diferente de um #, é inválida
					return false;
			}

			return true;
		}
	}
}

bool Board::existsWord(const unsigned int &l, const unsigned int &c, const char &d) //Verifica se já existe uma palavra na posição dada
{
	if (playerSolution.at(l - 65).at(c - 97) == '.') // Se tiver um . não existe palavra
		return false;
	else if (d == 'V')
	{
		if (playerSolution.at(l - 64).at(c - 97) != '#') //Se contiver um # na 2ª posição não pode colocar a palavra, caso contrário há outra condições que verificam se pode ou não
			return false;
		else
			return true;
	}
	else
	{
		if (playerSolution.at(l - 65).at(c - 96) != '#') //Se contiver um # na 2ª posição não pode colocar a palavra, caso contrário há outra condições que verificam se pode ou não
			return false;
		else
			return true;
	}
}

bool Board::existsWord(const string &word) //Verifica se a palavra já foi incluída no tabuleiro
{
	for (auto e : playerWords)
	{
		if (e.second == word)
			return true;
	}

	return false;
}

bool Board::compareWords(char direction, unsigned int inicialLine, unsigned int inicialColumn, string word) //Compara a palavra a colocar com as letras que já se encontram no tabuleiro
{
	if (direction == 'V')
	{
		int i = 0;

		for (size_t j = inicialLine - 65; j < inicialLine + word.length() - 65; j++)
		{
			if (playerSolution.at(j).at(inicialColumn - 97) == '.')
			{
				i++;
				continue;
			}
			else if (playerSolution.at(j).at(inicialColumn - 97) == word.at(i))
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

		for (size_t j = inicialColumn - 97; j < inicialColumn + word.length() - 97; j++)
		{
			if (playerSolution.at(inicialLine - 65).at(j) == '.')

			{
				i++;
				continue;
			}
			else if (playerSolution.at(inicialLine - 65).at(j) == word.at(i))
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

unsigned int Board::getLines() //Retorna o número de linhas dos tabuleiros
{
	return numLines;
}

unsigned int Board::getColumns() //Retorna o número de colunas dos tabuleiros
{
	return numColumns;
}

void Board::removeWord(const char &line, const char &column, const char &direction) //Remove a palavra que se encontra na posição indicada
{
	ostringstream pos;
	string position;
	pos << line << column << direction;
	position = pos.str();

	if (playerWords.find(position) != playerWords.end()) //Se encontrar a poisção no map playerWords, significa que a havia uma palavra nessa posição e, então, apaga-a
	{
		map<string, string>::iterator it = playerWords.find(position);
		string word = playerWords[position];

		playerWords.erase(it); //Remove o par onde está a posição indicada

		for (size_t i = 0; i < numLines; i++) //Coloca o tabuleiro tal como se encontrava no início
			for (size_t j = 0; j < numColumns; j++)
			{
				if (board.at(i).at(j) == '#')
					playerSolution.at(i).at(j) = '#';
				else
					playerSolution.at(i).at(j) = '.';
			}

		for (auto e : playerWords) //Coloca todas as palavras restantes no tabuleiro
			putWord(e.second, e.first.at(0), e.first.at(1), e.first.at(2));
	}
	else //Se não existir nenhum par com a posição
		cout << endl << "There is no word in this position";
}

char Board::getChar(const char &line, const char &column) //Retorna o carater da posição indicada
{
	return playerSolution.at((int)line - 65).at((int)column - 97);
}

void Board::getWords(vector<string> &v) //Guarda no vetor passado como parâmetro todas as palavras que se encontram no board
{
	for (auto e : words)
		v.push_back(e.second);
}

string Board::getWord(string &position) //Devolve a palavra que se encontra em determinada posição
{
	return words[position];
}

void Board::getPositions(std::vector<std::string> &v) //Guarda no vetor passado como parâmetro todas as posições que contêm palavras 
{
	for (auto e : words)
		v.push_back(e.first);
}

bool Board::isFull() //Verifica se o tabuleiro está cheio
{
	for (unsigned int i = 0; i < numLines; i++)
		for (unsigned int j = 0; j < numColumns; j++)
		{
			if (playerSolution.at(i).at(j) == '.') //O tabuleiro encontra-se cheio quando não têm nenhum ponto
				return false;
		}

	return true;
}

bool Board::anotherWordVert(unsigned int inicialLine, unsigned int finalLine, unsigned int column) //Verifica se existe outra palavra na mesma coluna (retorna true se encontrar)
{
	for (unsigned int i = inicialLine; i < finalLine; i++)
	{
		if (playerSolution.at(i).at(column) != '.' && playerSolution.at(i).at(column) != '#')
			return true; //Existe uma letra de outra palavra nessa coluna
	}

	return false;
}

bool Board::anotherWordHor(unsigned int inicialColumn, unsigned int finalColumn, unsigned int line) //Verifica se existe outra palavra na mesma linha (retorna true se encontrar)
{
	for (unsigned int i = inicialColumn; i < finalColumn; i++)
	{
		if (playerSolution.at(line).at(i) != '.' && playerSolution.at(line).at(i) != '#')
			return true; //Existe uma letra de outra palavra nessa coluna
	}

	return false;
}

void Board::setBoardName(string name) //Altera o nome do tabuleiro: do tipo bXXX
{
	boardFileName = name.substr(0, 4);
}

bool Board::compareBoards() //Compara o tabuleiro retirado do ficheiro com o tabuleiro criado pelo jogador
{
	for (size_t i = 0; i < numLines; i++)
		for (size_t j = 0; j < numColumns; j++)
		{
			if (board.at(i).at(j) != playerSolution.at(i).at(j)) //Se todas as posições forem iguais, retorna true
				return false;
		}

	return true;
}

string Board::getBoardName() //Retorna o nome do ficheiro
{
	return boardFileName;
}