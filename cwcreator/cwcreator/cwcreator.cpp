// cwcreator.cpp : define o ponto de entrada para o aplicativo do console.
//

#include "Dictionary.h"
#include "Board.h"
#include <iostream>
#include <time.h>
#include <Windows.h>
#include <stdlib.h>
#include <sstream>
#include <iomanip>
#include <fstream>

//Definição dos números que correspondem a cada cor -> serão utilizados nas funções setcolor
#define BLACK 0 
#define CYAN 3
#define LIGHTGRAY 7
#define LIGHTBLUE 9
#define LIGHTRED 12  
#define WHITE 15

unsigned int numCouts = 122; //Constante que define o número de - ou = que aparecem no ecrão ao dividir as diferentes fazes do programa

using namespace std;

void instructions(); //Função que mostra as instruções
void options(); //Função que mostra as opções
void createPuzzle(); //Função para criar um novo puzzle
void resumePuzzle(); //Função para continuar um puzzle já criado anteriormente
void Puzzle(Board &b, Dictionary &d); //Função onde se cria o puzzle
void setcolor(unsigned int color); //Altera a cor das letras para uma cor específica: fundo fica a preto
void setcolor(unsigned int color, unsigned int background_color); //Permite alterar a cor das letras e do fundo
void capitalLetters(string &str); //Função que coloca todas as letras de uma string em maiúsculas
void crosswords();  //Função que mostra CROSSWORDS CREATOR no início do programa

int main()
{
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE); //Abre a janela da console maximizada

	setcolor(12);
	crosswords(); //Mostra CROSSWORDS CREATOR no ecrã
	setcolor(9);
	for (int i = 0; i < numCouts; i++)
		cout << "=";
	setcolor(15);
	cout << endl;

	instructions(); //Mostra instruções

	setcolor(9);
	for (int i = 0; i < numCouts; i++)
		cout << "-";
	setcolor(15);
	cout << endl;

	do
	{
		options(); //Mostra opções

		int option;

		bool validOption;

		do
		{
			do
			{
				cin >> option; //Se não for um inteiro é repetido até que o utilizador introduza um número inteiro
				if (cin.fail())
				{
					cin.clear();
					cin.ignore(100000, '\n');
				}
			} while (cin.fail());

			cout << endl;

			switch (option)
			{
			case 1: //Opção 1: criar um puzzle novo
				createPuzzle();
				validOption = true;
				break;
			case 2: //Opção 2: continuar um puzzle já começado anteriormente
				resumePuzzle();
				validOption = true;
				break;
			case 3: //Opção 3: mostrar instruções
				instructions();
				validOption = true;
				break;
			case 0: //Opção 0: sair do programa
				exit(1); //sai do programa
				break;
			default: //Qualquer outra opção: repetir o ciclo
				validOption = false;
				cout << "Invalid option. Try again..." << endl;
			}
		} while (!validOption);

		cout << endl << endl;
		setcolor(9);
		for (int i = 0; i < numCouts; i++)
			cout << "-";
		setcolor(15);
		cout << endl;
	} while (true);
	return 0;
}

void instructions() //Mostrar as instruções
{
	setcolor(12);
	cout << "INSTRUCTIONS:" << endl << endl;
	setcolor(15);
	cout << "The objective of this program is to create a board to play crosswords" << endl;
	cout << "You have to input the position of the board and the word you want to put on the board : " << endl << endl;

	cout << "Position ( LCD / CTRL-Z = stop )" << endl;
	cout << " LCD stands for Line Column and Direction" << endl;
	cout << "The line and the direction should be an uppercase letter and the column should be a lowercase letter" << endl << endl;

	cout << "Word ( - = remove / ? = help )" << endl;
	cout << " - remove the word after the sign" << endl;
	cout << " ? show any possible word for the corresponding position" << endl;
	cout << "You have another option (+) that says if you have wrong words formed by the others" << endl;
	cout << "You can only finish the board if you don't have wrong words in it" << endl;
	cout << "You can't overlap words that not combine and you can't overlap any hash" << endl << endl;

	cout << "When the board is full, which means you can't put any more word in it, the board creator will end" << endl;
	cout << "Besides that you can create a new board or just leave the program" << endl << endl;

	cout << "If you choose to save the board it will be save on a file like this \"bXXX.txt\"" << endl;
	cout << "Please make sure you save the board or all your unsaved progress will be lost" << endl;
	cout << "To prevent this, if you don't press 'N' or 'n' the program will save the board" << endl << endl;

	setcolor(3);
	cout << "Have fun!!!" << endl << endl;
	setcolor(15);
}

void options() //Mostrar as opções
{
	setcolor(12);
	cout << "OPTIONS:" << endl;
	setcolor(15);
	cout << "1 - Create puzzle" << endl;
	cout << "2 - Resume puzzle" << endl;
	cout << "3 - Instructions" << endl;
	cout << "0 - Exit" << endl << endl;

	cout << "Option? ";
}

void createPuzzle() //Criar um puzzle novo
{
	setcolor(9);
	for (int i = 0; i < numCouts; i++)
		cout << "-";
	setcolor(12);
	cout << endl << "CREATE PUZZLE" << endl;
	setcolor(9);
	for (int i = 0; i < numCouts; i++)
		cout << "-";
	setcolor(15);
	unsigned int cols, lines;
	cout << endl;

	do
	{
		cout << "Number of columns: ";
		cin >> cols; //Lê o número de colunas
		if (cin.fail() || cols >= 27)
		{
			cout << "Try again..." << endl; //Mostra esta mensagem para o utilizador ter a certeza que se enganou ao introduzir o número de colunas
			cin.clear();
			cin.ignore(100000, '\n');
		}
	} while (cin.fail() || cols >= 27); //Número de colunas não pode ser superior ou igual a 27, pois apenas há 26 letras no alfabeto


	do
	{
		cout << "Number of lines: ";
		cin >> lines; //Lê o número de linhas
		if (cin.fail() || lines >= 27)
		{
			cout << "Try again..." << endl; //Mostra esta mensagem para o utilizador ter a certeza que se enganou ao introduzir o número de linhas
			cin.clear();
			cin.ignore(100000, '\n');
		}
	} while (cin.fail() || lines >= 27); //Número de linhas não pode ser superior ou igual a 27, pois apenas há 26 letras no alfabeto

	Board b(lines, cols); //Inicialização do tabuleiro com linhas e colunas

	b.setNumberBoard(); //Inicializa o número do tabuleiro, que será utilizado para no final do programa gravar o tabuleiro 

	string fileName;

	cout << "Dictionary File Name: ";
	cin >> fileName;

	Dictionary d(fileName); //Inicialização do dicionário (fileName -> nome do dicionário)

	Puzzle(b, d); //Criação do tabuleiro
}

void resumePuzzle() //Tirar o tabuleiro de um ficheiro de texto
{
	setcolor(9);
	for (int i = 0; i < numCouts; i++)
		cout << "-";
	setcolor(12);
	cout << endl << "RESUME PUZZLE" << endl;
	setcolor(9);
	for (int i = 0; i < numCouts; i++)
		cout << "-";
	setcolor(15);
	ifstream file;
	string fileName;
	bool invalidFile;

	do
	{
		cout << endl << "Board File Name: ";

		cin >> fileName; //Leitura do nome do tabuleiro

		file.open(fileName);

		if (file.fail()) //Caso o ficheiro não exista, o utilizador tem a opção de introduzir outro nome para o tabuleiro
		{
			cerr << "This file doesn't exist. Please make sure you write well the name of file that contains the board." << endl;
		}
		else if (fileName.length() != 8) //Nome do ficheiro tem que ser do tipo bXXX.txt
		{
			cout << "The name of the board is invalid." << endl;
			invalidFile = true;
		}
		else
		{
			invalidFile = false;
		}
	} while (file.fail() || invalidFile);

	string dictionaryFileName;

	getline(file, dictionaryFileName);

	Dictionary d(dictionaryFileName); //Inicialização do dicionário

	getline(file, dictionaryFileName); //Retira a linha vazia

	Board b(file); //Inicialização do tabuleiro através de um ficheiro

	b.setNumberBoard(); //Número do tabuleiro será atualizado

	Puzzle(b, d); //Criação do tabuleiro
}

void Puzzle(Board &b, Dictionary &d) //Criar o puzzle: colocar palavras no tabuleiro
{
	b.showBoard();

	string position;
	istringstream iss;
	char line, column, direction;

	string word;

	do //Introdução da palavra 
	{
		if (!b.isFull()) //Caso o tabuleiro esteja cheio (não contenha pontos), é possível continuar a criar o tabuleiro
		{
		contBoard: //Se o tabuleiro tiver palavras erradas salta para aqui
			cout << "Position ( LCD / CTRL-Z = stop ) ? ";

			if (!(cin >> position)) //Se o utilizador introduzir CTRL-Z 
			{
				//Limpar o cin que continha o CTRL + Z para não aparecer a mensagem
				cin.clear();
				cin.ignore(10000, '\n');
				break;
			}
			else if (position.length() != 3) //Cada posição tem que ter 3 carateres: linha, coluna e direção
			{
				cerr << endl << "Invalid position" << endl;
				continue;
			}

			line = position.at(0);
			line = toupper(line); //Linha tem que ser maiúscula

			if ((int)line - 65 >= b.getLines())//Se o utilizador colocar uma letra superior à do número de linhas, a posição é inválidas
			{
				cout << endl << "The board doesn't have this number of lines." << endl;
				continue;
			}
			column = position.at(1);
			column = tolower(column); //Coluna tem que ser minúscula

			if ((int)column - 97 >= b.getColumns()) //Se o utilizador colocar uma letra superior à do número de colunas, a posição é inválida
			{
				cout << endl << "The board doesn't have this number of columns." << endl;
				continue;
			}
			direction = position.at(2);
			direction = toupper(direction); //Direção também é uma letra maiúscula

			if (direction != 'V' && direction != 'H') //Só existem duas direções: V - vertical; H - horizontal
			{
				cout << endl << "This direction doesn't exist." << endl;
				continue;
			}

			cout << "Word ( - = remove / ? = help ) .. ? ";
			cin >> word;

			capitalLetters(word); //word fica em maiúsculas, sendo assim introduzida no tabuleiro

			if (word != "-" && word != "?" && word != "+") //Se a palavra não for um - ou um + ou um ?, é lido como uma palavra a colocar no tabuleiro
			{
				if (b.existsWord(word)) //Verifica se o jogador já colocou essa palavra no seu tabuleiro
				{
					cout << "You have already put this word on the board." << endl;
					continue;
				}
				else if (b.existsWord(line, column, direction)) //Se existir uma palavra na posição indicada, continua o ciclo (ignorando tudo o resto) e volta a pedir uma posição
					continue;
				else if (d.validWord(word)) //Verifica se a palavra existe no dicionário
				{
					b.putWord(word, line, column, direction); //Coloca a palavra no tabuleiro, na posição indicada por line, column e direction
				}
				else
					cout << "Word doesn't exist in the dictionary." << endl;
			}
			else if (word == "-")
			{
				b.removeWord(line, column, direction); //Remove a palavra que se encontra na posição line, column, direction
			}
			else if (word == "+") //Verificar se existe uma palavra inválida na posição dada
			{
				word.resize(0); //word não é necessário neste bocado de código, sendo assim reutilizada

				if (b.getChar(line, column) == '#' || b.getChar(line, column) == '.') //Caso na posição dada esteja um # ou um . não existe nenhuma palavra
					cout << "There is no word in this position" << endl;
				else if (direction == 'V')
				{
					for (size_t i = (int)line - 65; i < b.getLines(); i++)
					{
						if (b.getChar((char)(i + 65), column) == '#' || b.getChar((char)(i + 65), column) == '.') //Enquanto não encontra um # ou um . existe uma palavra
							break;
						else
							word = word + b.getChar((char)(i + 65), column); //Criação da palavra através da concatenação
					}

					if (d.validWord(word)) //Verifica se a palavra encontrada existe no dicionário
						cout << "The word " << word << " is valid" << endl;
					else
						cout << "The word " << word << " is invalid" << endl;
				}
				else
				{
					for (size_t i = (int)column - 97; i < b.getColumns(); i++)
					{
						if (b.getChar(line, (char)(i + 97)) == '#' || b.getChar(line, (char)(i + 97)) == '.') //Enquanto não encontra um # ou um . existe uma palavra
							break;
						else
							word = word + b.getChar(line, (char)(i + 97)); //Criação da palavra através da concatenação
					}

					if (d.validWord(word)) //Verifica se a palavra encontrada existe no dicionário
						cout << "The word " << word << " is valid" << endl;
					else
						cout << "The word " << word << " is invalid" << endl;
				}

			}
			else //?
			{
				string wildcard;
				ostringstream w;
				char c;

				c = b.getChar(line, column);

				if (c == '.') //Se na posição indicada estiver um . a palavra poderá ter qualquer formato
					wildcard = "*";
				else if (c != '#') //Se tiver uma letra, a palavra será do tipo c*, sendo c um char
				{
					w << c;
					wildcard = w.str();
					wildcard = wildcard + "*";
				}
				else //Caso tenha um #, não se pode colocar nenhum palavra na posição
				{
					cout << "You can't put words on this position." << endl;
					continue;
				}

				int size; //Variável que conterá o tamanho máximo da palavra

				if (direction == 'V')
				{
					size = b.getLines() - (int)line - 64;
				}
				else
					size = b.getColumns() - (int)column - 96;

				cout << endl;

				vector<string> words;
				b.getWords(words); //words conterá todas as palavras já colocadas no tabuleiro

				vector <string> helpWords = d.searchWords(wildcard, size, words); //helpWords conterá todas as palavras que cumprem os requisitos word e size, e existem no dicionário

				unsigned int numWords = 0;

				for (size_t i = 0; i < helpWords.size(); i++)
				{
					capitalLetters(helpWords.at(i));

					if (b.validWord(helpWords.at(i), line, column, direction)) //Verifica se cada palavra é válida no tabuleiro
					{
						cout << setw(20) << helpWords.at(i);
						numWords++;
					}

					if (numWords == 10)
					{
						cout << endl;
						numWords = 0;
					}
				}
			}

			b.showBoard(); //Mostra o tabuleiro
		}
		else //Caso o tabuleiro não contenha nenhum ponto
		{
			cout << "Your board is full" << endl;
			break;
		}
	} while (true);

	//Verificar se existem palavras inválidas
	vector <string> allBoardWords;

	allBoardWords = b.getWords(); //allBoardWords conterá todas as palavras do tabuleiro mais aquelas que se formam com as palavras inseridas

	for (size_t i = 0; i < allBoardWords.size(); i++)
	{
		if (!d.validWord(allBoardWords.at(i))) //Se alguma palavra do dicionário não é válida
		{
			cout << "You have invalid words in your board. Try to remove words from your board until you have only valid words." << endl;
			goto contBoard;
		}
	}

	//O código apenas chega a este ponto caso não haja palavras inválidas

	char option;

	cout << "Do you want to save the board?(Y/N) "; //Utilizador tem a opção de escolher se quer ou não guardar o tabuleiro
	//Só é possível gravar, caso não tenha palavras erradas

	do
	{
		cin >> option;
		if (cin.fail())
		{
			cout << "Try again..." << endl;
			cin.clear();
			cin.ignore(100000, '\n');
		}
	} while (cin.fail() || (toupper(option) != 'N' && toupper(option) != 'Y')); //Opção apenas poderá ser Y ou N

	if (toupper(option) == 'Y') //Se a opção for Y o tabuleiro é guardado
	{
		cout << endl << "Do you want to continue later(C) or finhish the board(F)? (If you choose the second option you can't modify the board later even if you try to open his file) ";
		bool validOption = false; //Apenas se tornará true se a opção for C ou F

		do
		{
			cin >> option; //option foi definido anteriormente como um char

			switch (toupper(option))
			{
			case 'C': //Se a opção for C, o tabuleiro é guardado normalmente
				b.saveBoard(d.getDictionaryName());
				validOption = true;
				break;
			case 'F': //Se a opção for F, o tabuleiro é acabado de preencher com # e posteriormente é guardado
				b.putBlack();
				b.saveBoard(d.getDictionaryName());
				validOption = true;
				break;
			default: //Caso geral: opção diferente, o ciclo continua
				validOption = false;
			}
		} while (!validOption);
	}
}

void setcolor(unsigned int color) //Função que muda a cor da letra, conforme o inteiro que lhe é passado
{
	HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hcon, color);
}

void setcolor(unsigned int color, unsigned int background_color) //Função que muda a cor da letra e o fundo do ecrã conforme os inteiros que lhe são passados
{
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	if (background_color == BLACK)
		SetConsoleTextAttribute(hCon, color);
	else
		SetConsoleTextAttribute(hCon, color | BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED);
}

void capitalLetters(string &str) //Função que coloca todas as letras de uma string em maiúsculas
{
	for (size_t i = 0; i < str.length(); i++)
	{
		str.at(i) = toupper(str.at(i));
	}
}

void crosswords() //Função que mostra CROSSWORDS CREATOR no início do programa -> o facto de a barra / ter que ser escrita como // faz com que pareça desformatado no código
{
	cout << " _____                              _    _                  _		 _____                          _" << endl;
	cout << "/ ____|                            | |  | |                | |		/ ____|                        | |" << endl;
	cout << "| |     _ __  _____   ___   ___    | |  | |  _____  _ __  _| | ___	| |       _ __    ___    __ _  | |_    ___    _ __" << endl;
	cout << "| |    | '__||  _  | | __| | __|   | |__| | |  _  || '__|  _ || __|	| |      | '__|  / _ \\  / _` | | __|  / _ \\  | '__|" << endl;
	cout << "| |____| |   | |_| | |__ | |__ |   |      | | |_| || |  | |_|||__ |	| |____  | |    |  __  | (_| | | |_  | (_) | | |" << endl;
	cout << "\\______|_|   |_____| |___| |___|   |__/\\__| |_____||_|  |____||___|	\\______| |_|    \\___ |  \\__,_|  \\__|  \\___/  |_| " << endl;
}