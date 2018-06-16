#include "Board.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

using namespace std;

//Declara��o de fun��es que est�o definidas no ficheiro cwplayer.cpp
//Estas fun��es est�o aqui definidas, pois s�o usadas neste ficheiro
void setcolor(unsigned int color);
void setcolor(unsigned int color, unsigned int background_color);

Board::Board(ifstream &file) //Criador do tabuleiro: retira o tabuleiro de um ficheiro criado pelo programa cwcreator
{
	string line;

	vector <char> v; //Vetor que conter� cada linha do tabuleiro

	board.resize(0); //Inicialmente o tabuleiro n�o tem elementos

	while (!file.eof())
	{
		getline(file, line);
		if (line.length() == 0)
			break;
		/*Embora seja usado um while(!file.eof()), o ciclo acaba quando o programa encontrar uma linha vazia
		  Isto acontece, pois em qualquer ficheiro do tipo bXXX.txt, criado pelo programa cwcreator, o tabuleiro � criado seguido de uma linha vazia
		  e por fim as palavras todas que nele se encontram*/

		v.resize(0);

		/*A fun��o seguinte copia a string line para o vector<char> v
		  Notar atr�s a utiliza��o de v.resize(0)
		  Sem esta chamada, o conte�do do vetor v, n�o seria atualizado, o que faria com que � medida que se fazia o b.push_back(v) no final deste bloco de c�digo,
		  o n�mero de colunas aumentaria*/
		copy(line.begin(), line.end(), back_inserter(v));

		board.push_back(v); //push_back de cada linha do tabuleiro
	}

	numColumns = v.size(); //N�mero de colunas � dado pelo tamanho de qualquer vector <char>, uma vez que todas as linhas t�m o mesmo n�mero de colunas
	numLines = board.size(); //O n�mero de linhas � dado pelo tamanho do vector <vector <char>>

	string position; //Posi��o reitrada do ficheiro
	string word; //Palavra retirada do ficheiro
	pair<string, string> p; //Par que ser� criado sempre com uma posi��o e respetiva palavra
	while (!file.eof()) //Introdu��o das palavras retiradas do ficheiro (ap�s o tabuleiro) num map chamado words
	{
		getline(file, line);

		if (line.length() == 0)
			break;

		/*Como cada linha retirada do ficheiro quando se est� a retirar as palavras, cont�m a posi��o seguida de um espa�o e logo a segui a palavra,
		  � utilizada a fun��o substr associada a string para retirar cada posi��o e cada palavra. Espa�o � eliminado*/

		position = line.substr(0, 3); //Posi��o tem 3 carateres: linha, coluna e dire��o. Come�a na posi��o 0 da string
		word = line.substr(4, line.length() - 4); //Palavra come�a na posi��o 4 (0, 1 e 2 -> dire��o; 3 -> espa�o) e o seu tamanho ser� o tamanho da linha - 4

		//Cria��o e inser��o do par com a posi��o e a palavra
		p = make_pair(position, word);
		words.insert(p);
	}

	//playerSolution ser� um tabuleiro com o mesmo tamanho de board
	playerSolution.resize(numLines);
	for (size_t i = 0; i < numLines; i++)
		playerSolution.at(i).resize(numColumns);

	/*Criar o tabuleiro que o utilizador ver�:
			-Quando no board temos um '#' ou um '.', no playerSolution ser� igualmente um '#' (adi��o do ponto pois o utilizador no cwcreator poder� ter-se esquecido de finalizar o seu tabuleiro
			-Quando no board temos qualquer letra, ser� um '.' no playerSolution
	Permite fazer com que o utilizador consiga visualizar perfeitamente o s�tio onde tem que colocar a sua palavra
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

void Board::putWord(string &word, const char &line, const char &column, const char &direction) //Fun��o que coloca uma palavra no tabuleiro e, consequentemente, coloca a sua posi��o inicial e os sin�nimos no map
{
	ostringstream pos; //String stream que ajudar� a criar a posi��o
	string position; //String com a posi��o da palavra
	vector<string> synonyms; //Vetor com os sin�nimos
	pair<string, string> p;

	if (playerSolution.at((int)line - 65).at((int)column - 97) == '#') //Se na posi��o escolhida estiver um '#' o utilizador n�o conseguir� introduzir a palavra
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

				//Colocar sin�nimos no map
				pos << line << column << direction;
				position = pos.str();
				p = make_pair(position, word);
				playerWords.insert(p);
			}
			else //Coloca a palavra se conseguir (se encaixar com a letra j� existente)
			{
				if (compareWords(direction, (int)line, (int)column, word)) //Igual ao anterior: poss�vel colocar na mesma condi��o com um or (||), mas atrasaria o programa caso se colocasse a palavra normalmente
				{
					int wordPosition;

					wordPosition = 0;
					for (unsigned int i = (unsigned int)line - 65; i < (unsigned int)line + word.length() - 65; i++)
					{
						playerSolution.at(i).at((int)column - 97) = word.at(wordPosition);
						wordPosition++;
					}

					//Colocar sin�nimos no map
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

				//Colocar sin�nimos no map
				pos << line << column << direction;
				position = pos.str();
				p = make_pair(position, word);
				playerWords.insert(p);
			}
			else //Coloca a palavra se conseguir (se encaixar com a letra j� existente)
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

					//Colocar sin�nimos no map
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

		setcolor(0); //Mudar a cor para preto para prevenir a exten��o do fundo quando se aumenta a tela da linha de comandos
		cout << "."; //cout de um eapa�o: pode ser qualquer outro carater, desde que esteja � frente do tabuleiro e n�o interfira com o mesmo
	}

	setcolor(15, 0);
	cout << endl << endl;
}

bool Board::wordFits(size_t wordSize, char line, char column, char direction) //Fun��o que verifica se uma palavra cabe num certo espa�o -> retorna falso se come�a na posi��o errada ou tem comprimento inadequado � posi��o
{
	if (direction == 'V')
	{
		if ((int)line - 65 == 0) //Palavra come�a na 1� posi��o
		{
			for (size_t i = 0; i < wordSize; i++)
			{
				if (playerSolution.at((int)line - 65 + i).at((int)column - 97) == '#') //Se na 1� posi��o houver um #, � inv�lida
					return false;
			}

			if ((int)line - 65 + wordSize != numLines)
			{
				if (playerSolution.at((int)line - 65 + wordSize).at((int)column - 97) != '#') //Se na posi��o a seguir � �ltima letra, j� tiver algo diferente de um #, � inv�lida
					return false;
			}

			return true;
		}
		else if (playerSolution.at((int)line - 66).at((int)column - 97) != '#') //Se antes da palavra tiver algo diferente de um #, � inv�lida
		{
			return false;
		}
		else if ((int)line - 65 + wordSize != numLines) //Palavra n�o chega � �ltima posi��o
		{
			{
				if (playerSolution.at((int)line - 65 + wordSize).at((int)column - 97) != '#') //Se na posi��o a seguir � �ltima letra, j� tiver algo diferente de um #, � inv�lida
					return false;
			}
			return true;
		}
		else
		{
			for (size_t i = 0; i < wordSize; i++) //Percorre a palavra � procura de #: se encontrar algum � inv�ldia
			{
				if (playerSolution.at((int)line - 65 + i).at((int)column - 97) == '#')
					return false;
			}

			if ((int)line - 65 + wordSize != numLines)
			{
				if (playerSolution.at((int)line - 65 + wordSize).at((int)column - 97) != '#') //Se na posi��o a seguir � �ltima letra, j� tiver algo diferente de um #, � inv�lida
					return false;
			}

			return true;
		}
	}
	else
	{
		if ((int)column - 97 == 0) //Palavra come�a na 1� posi��o
		{
			for (size_t i = 0; i < wordSize; i++)
			{
				if (playerSolution.at((int)line - 65).at((int)column - 97 + i) == '#') //Se na 1� posi��o houver um #, � inv�lida
					return false;
			}

			if ((int)column - 97 + wordSize != numColumns)
			{
				if (playerSolution.at((int)line - 65).at((int)column - 97 + wordSize) != '#') //Se na posi��o a seguir � �ltima letra, j� tiver algo diferente de um #, � inv�lida
					return false;
			}

			return true;
		}
		else if (playerSolution.at((int)line - 65).at((int)column - 98) != '#') //Se antes da palavra tiver algo diferente de um #, � inv�lida
			return false;
		else if ((int)column - 97 + wordSize != numColumns) //Palavra n�o chega � �ltima posi��o
		{
			{
				if (playerSolution.at((int)line - 65).at((int)column - 97 + wordSize) != '#') //Se na posi��o a seguir � �ltima letra, j� tiver algo diferente de um #, � inv�lida
					return false;
			}
			return true;
		}
		else
		{
			for (size_t i = 0; i < wordSize; i++) //Percorre a palavra � procura de #: se encontrar algum � inv�ldia
			{
				if (playerSolution.at((int)line - 65).at((int)column - 97 + i) == '#')
					return false;
			}

			if ((int)column - 97 + wordSize != numColumns)
			{
				if (playerSolution.at((int)line - 65).at((int)column - 97 + wordSize) != '#') //Se na posi��o a seguir � �ltima letra, j� tiver algo diferente de um #, � inv�lida
					return false;
			}

			return true;
		}
	}
}

bool Board::existsWord(const unsigned int &l, const unsigned int &c, const char &d) //Verifica se j� existe uma palavra na posi��o dada
{
	if (playerSolution.at(l - 65).at(c - 97) == '.') // Se tiver um . n�o existe palavra
		return false;
	else if (d == 'V')
	{
		if (playerSolution.at(l - 64).at(c - 97) != '#') //Se contiver um # na 2� posi��o n�o pode colocar a palavra, caso contr�rio h� outra condi��es que verificam se pode ou n�o
			return false;
		else
			return true;
	}
	else
	{
		if (playerSolution.at(l - 65).at(c - 96) != '#') //Se contiver um # na 2� posi��o n�o pode colocar a palavra, caso contr�rio h� outra condi��es que verificam se pode ou n�o
			return false;
		else
			return true;
	}
}

bool Board::existsWord(const string &word) //Verifica se a palavra j� foi inclu�da no tabuleiro
{
	for (auto e : playerWords)
	{
		if (e.second == word)
			return true;
	}

	return false;
}

bool Board::compareWords(char direction, unsigned int inicialLine, unsigned int inicialColumn, string word) //Compara a palavra a colocar com as letras que j� se encontram no tabuleiro
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
				return false; //Palavra n�o encaixa com as letras j� existentes
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
				return false; //Palavra n�o encaixa com as letras j� existentes
		}

		return true;
	}
}

unsigned int Board::getLines() //Retorna o n�mero de linhas dos tabuleiros
{
	return numLines;
}

unsigned int Board::getColumns() //Retorna o n�mero de colunas dos tabuleiros
{
	return numColumns;
}

void Board::removeWord(const char &line, const char &column, const char &direction) //Remove a palavra que se encontra na posi��o indicada
{
	ostringstream pos;
	string position;
	pos << line << column << direction;
	position = pos.str();

	if (playerWords.find(position) != playerWords.end()) //Se encontrar a pois��o no map playerWords, significa que a havia uma palavra nessa posi��o e, ent�o, apaga-a
	{
		map<string, string>::iterator it = playerWords.find(position);
		string word = playerWords[position];

		playerWords.erase(it); //Remove o par onde est� a posi��o indicada

		for (size_t i = 0; i < numLines; i++) //Coloca o tabuleiro tal como se encontrava no in�cio
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
	else //Se n�o existir nenhum par com a posi��o
		cout << endl << "There is no word in this position";
}

char Board::getChar(const char &line, const char &column) //Retorna o carater da posi��o indicada
{
	return playerSolution.at((int)line - 65).at((int)column - 97);
}

void Board::getWords(vector<string> &v) //Guarda no vetor passado como par�metro todas as palavras que se encontram no board
{
	for (auto e : words)
		v.push_back(e.second);
}

string Board::getWord(string &position) //Devolve a palavra que se encontra em determinada posi��o
{
	return words[position];
}

void Board::getPositions(std::vector<std::string> &v) //Guarda no vetor passado como par�metro todas as posi��es que cont�m palavras 
{
	for (auto e : words)
		v.push_back(e.first);
}

bool Board::isFull() //Verifica se o tabuleiro est� cheio
{
	for (unsigned int i = 0; i < numLines; i++)
		for (unsigned int j = 0; j < numColumns; j++)
		{
			if (playerSolution.at(i).at(j) == '.') //O tabuleiro encontra-se cheio quando n�o t�m nenhum ponto
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
			if (board.at(i).at(j) != playerSolution.at(i).at(j)) //Se todas as posi��es forem iguais, retorna true
				return false;
		}

	return true;
}

string Board::getBoardName() //Retorna o nome do ficheiro
{
	return boardFileName;
}