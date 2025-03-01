// cwplayer.cpp : define o ponto de entrada para o aplicativo do console.
//
#include "Dictionary.h"
#include "Board.h"
#include "Player.h"
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

unsigned int numCouts = 121; //Constante que define o número de - ou = que aparecem no ecrão ao dividir as diferentes fazes do programa

using namespace std;

void setcolor(unsigned int color); //Altera a cor das letras para uma cor específica: fundo fica a preto
void setcolor(unsigned int color, unsigned int background_color); //Permite alterar a cor das letras e do fundo
void capitalLetters(string &str); //Função que coloca todas as letras de uma string em maiúsculas
void instructions(); //Função que mostra as instruções
void options(); //Função que mostra as opções
void playCrosswords(); //Função que começa o jogo: cria o jogador, o tabuleiro e o dicionário
void Puzzle(Board b, Dictionary d, Player p); //Função onde se desenvolve o jogo
void crosswords(); //Função que mostra CROSSWORDS PLAYER no início do programa
void showSynonyms(Board &b, Dictionary &d); //Função que mostra os sinónimos iniciais, dividindo os verticais dos horizontais

int main()
{
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE); //Abre a janela da console maximizada

	srand(time(NULL)); //Inicialização da função random

	setcolor(12);

	crosswords(); //Mostra CROSSWORDS PLAYER no ecrã
	setcolor(9);
	for (int i = 0; i < numCouts; i++)
		cout << "=";
	setcolor(15);
	cout << endl;

	instructions(); //Mostra as instruções

	setcolor(9);
	for (int i = 0; i < numCouts; i++)
		cout << "-";
	setcolor(15);
	cout << endl;

	do
	{
		options(); //Mostra as opções

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

			switch (option)
			{
			case 1: //Opção 1: jogar crosswords
				cout << endl;
				playCrosswords();
				validOption = true;
				break;
			case 2: //Opção 2: mostrar instruções
				cout << endl;
				instructions();
				validOption = true;
				break;
			case 0: //Opção 0: sair do programa
				exit(1); //sai do programa
				break;
			default: //Qualquer outra opção: repete o ciclo
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
	return 0;
}

//As próximas duas funções são provenientes do pdf com as especificações do projeto
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
//Fim das funções que são provenientes do pdf com as especificações do projeto

void capitalLetters(string &str) //Função que coloca todas as letras de uma string em maiúsculas
{
	for (size_t i = 0; i < str.length(); i++)
	{
		str.at(i) = toupper(str.at(i));
	}
}

void instructions() //Mostrar as instruções
{
	setcolor(12);
	cout << "INSTRUCTIONS:" << endl << endl;
	setcolor(15);

	cout << "Position ( LCD / CTRL-Z = stop )" << endl;
	cout << " LCD stands for Line Column and Direction" << endl << endl;

	cout << "Word ( - = remove / ? = help )" << endl;
	cout << " - remove the word after the sign" << endl;
	cout << " ? show another synonym of the word in the position" << endl;
	cout << "You can't overlap words that not combine and you can't overlap any hash" << endl << endl;

	cout << "When the board is full, which means you can't put any more word in it, the game will end" << endl;
	cout << "Besides that you can leave the game if you press CTRL-Z" << endl << endl;

	cout << "If you resolve the puzzle your data will be saved in a file like bXXX_p.txt" << endl;
	cout << "Please make sure you don't put any special character like accent characters in your name " << endl;
	cout << "If you do that, the program may save the data uncorrectly" << endl << endl;

	setcolor(3);
	cout << "Have fun!!!" << endl << endl;
	setcolor(15);
}

void options() //Mostrar as opções
{
	setcolor(12);
	cout << "OPTIONS:" << endl;
	setcolor(15);
	cout << "1 - Play Crosswords" << endl;
	cout << "2 - Instructions" << endl;
	cout << "0 - Exit" << endl << endl;

	cout << "Option? ";
}

void playCrosswords() //Função onde se desenvolve a parte prinicipal do jogo: recolhe o nome do jogador e o nome do tabuleiro, inicializando também o dicionário
{
	setcolor(9);
	for (int i = 0; i < numCouts; i++)
		cout << "-";
	setcolor(12);
	cout << endl << "CROSSWORDS GAME" << endl;
	setcolor(9);
	for (int i = 0; i < numCouts; i++)
		cout << "-";
	setcolor(15);

	ifstream file;
	string fileName;

	string playerName;
	char option;
	do
	{
		cout << endl << "What's your name? (Please make sure you don't put any special character like accent characters in your name) ";
		cin.clear();
		cin.ignore(100000, '\n');
		getline(cin, playerName); //Programa recolhe todos os nomes escritos para o nome do jogador
		cout << playerName << ", is that your name?(Y/N) ";
		do
		{
			cin >> option;
			if (cin.fail())
			{
				cin.clear();
				cin.ignore(10000, '\n');
			}
		} while (cin.fail());
	} while (toupper(option) != 'Y');

	Player p(playerName); //Criação do jogador: coloca o nome do jogador e inicializa o número de ajudas(0)

	cout << "Have fun " << p.getName() << "." << endl;
	cout << "But don't forget that we are counting your time. We only start when the truly game begins..." << endl;
	cout << "Good luck!" << endl;

	do //Leitura do ficheiro que contém o tabuleiro e o nome do dicionário
	{
		cout << endl << "Board File Name: ";

		cin >> fileName;

		file.open(fileName);

		if (file.fail()) //Caso não abra o ficheiro, o jogador pode introduzir outro nome
		{
			cerr << "This file doesn't exist. Please make sure you write well the name of file that contains the board." << endl;
		}

	} while (file.fail());

	string dictionaryFileName; //Nome do dicionário

	getline(file, dictionaryFileName);

	Dictionary d(dictionaryFileName); //Criação do dicionário

	getline(file, dictionaryFileName); //Retira a linha vazia

	Board b(file); //Criação do tabuleiro

	b.setBoardName(fileName); //Associa o nome do ficheiro, do tipo bXXX, ao tabuleiro b

	Puzzle(b, d, p); //Jogo
}

void Puzzle(Board b, Dictionary d, Player p) //Jogo: colocar palavras no tabuleiro
{
	b.showBoard(); //Mostra o tabuleiro inicial

	vector<string> words;
	b.getWords(words); //words terá as palavras todas que estavam inicialmente no tabuleiro

	d.setSynonyms(words); //Cria um map no dicionário com as palavras iniciais e um vetor com os seus sinónimos

	string position;
	char line, column, direction;

	string word;

	showSynonyms(b, d); //Mostra os sinónimos iniciais

	p.startTime(); //Começa a contar o tempo do jogador

	do //Introdução da palavra 
	{
		if (!b.isFull()) //Se o tabuleiro não está cheio, o jogador pode jogar
		{
		resolvePuzzle: //Etiqueta que é utilizada quando o tabuleiro está cheio, mas o utilizador quer continuar a jogar, podendo remover palavras
			cout << "Position ( LCD / CTRL-Z = stop ) ? ";

			if (!(cin >> position)) //Se o utilizador introduzir CTRL-Z 
			{
				//Limpar o cin que continha o CTRL + Z para não aparecer a mensagem
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "Are you sure you want to quit? (Y/N) ";

				char option;
				do
				{
					cin >> option;
					if (cin.fail())
					{
						cout << "Try again..." << endl;
						cin.clear();
						cin.ignore(100000, '\n');
					}
					else
						toupper(option);
				} while (cin.fail());

				if (option == 'Y')
					break;
				else
					continue;
			}
			else if (position.length() != 3) //Cada posição tem que ter 3 carateres: linha, coluna e direção
			{
				cerr << endl << "Invalid position" << endl;
				continue;
			}

			line = toupper(position.at(0)); //Linha tem que ser maiúscula

			if ((int)line - 65 >= b.getLines()) //Se o utilizador colocar uma letra superior à do número de linhas, a posição é inválida
			{
				cout << endl << "The board doesn't have this number of lines." << endl;
				continue;
			}
			column = tolower(position.at(1)); //Coluna tem que ser minúscula

			if ((int)column - 97 >= b.getColumns()) //Se o utilizador colocar uma letra superior à do número de colunas, a posição é inválida
			{
				cout << endl << "The board doesn't have this number of columns." << endl;
				continue;
			}

			direction = toupper(position.at(2)); //Direção também é uma letra maiúscula

			if (direction != 'V' && direction != 'H') //Só existem duas direções: V - vertical; H - horizontal
			{
				cout << endl << "This direction doesn't exist." << endl;
				continue;
			}

			if (b.getChar(line, column) == '#') //Se na posição estiver um #, o utilizador não pode colocar nada
			{
				cout << "You can't put a word on this position..." << endl;
				continue;
			}

			cout << "Word ( - = remove / ? = help ) .. ? ";
			cin >> word;

			capitalLetters(word); //word fica em maiúsculas, sendo assim introduzida no tabuleiro

			if (word != "-" && word != "?") //Se a palavra não for um - ou um ?, é lido como uma palavra a colocar no tabuleiro
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
					cout << "Word doesn't exist in the dictionary." << endl; //Caso nenhuma condição anterior se verifique, significa que a palavra não existe no dicionário
			}
			else if (word == "-")
			{
				b.removeWord(line, column, direction); //Remove a palavra que se encontra na posição line, column, direction
			}
			else //? - mostra outro sinónimo
			{
				ostringstream oss;
				oss << line << column << direction;
				position = oss.str(); //Volta a juntar a posição numa string: não se usa a variável já criada, pois pode não estar com a capitalização devida
				string word = b.getWord(position); //word = palavra que no tabuleiro com a solução se encontra naquela posição

				word = d.showSynonym(word); //word será o sinónimo de word

				if (word.length() == 0)
					cout << "There are no more synonyms to show" << endl;
				else
				{
					cout << word << endl; //Mostra o novo sinónimo, caso exista
					p.setNumberclues(); //Adiciona 1 ao número de ajudas pedidas pelo jogador
				}
			}
			b.showBoard(); //Mostra o tabuleiro sempre que se altera faz alguma operação
		}
		else //O tabuleiro está cheio: sai do ciclo de colocar palavras
		{

			if (b.compareBoards()) //Compara a solução verdadeira com a apresentada pelo jogador e realiza este pedaço de código caso seja igual
			{
				time_t t = time(NULL) - p.getStartTime(); //Guarda o tempo do jogador na variável t
				unsigned int clues = p.getNumberClues(); //Guarda o número de ajudas pedidas na variável clues
				cout << "Congratulations, " << p.getName() << "! You guess all the words in " << t << " seconds and you get " << clues << " clues" << endl;
				string nameOfFile = b.getBoardName() + "_p.txt"; //Cria o nome do ficheiro onde será guardado os resultados do utilizador
				cout << "Your time and the number of clues will be saved in " << nameOfFile << " file";

				ofstream out;

				out.open(nameOfFile, ios_base::app); //Abre o ficheiro nameOfFile, colocando os dados novos após o que já tinha sido colocado
				//O app na instrução anterior significa que o ficheiro não vai ser criado novamente e o novo resultado será colocado no final do ficheiro, caso este já tenha sido criado

				out << p.getName() << ": " << "time - " << t << "; clues - " << clues << endl;

				out.close(); //Fecha o dicionário que abriu para guardar os dados do jogador

				break;
			}
			else //Se a solução está incorreta, o jogador tem ainda a opção de tentar outra vez, removendo palavra da sua solução
			{
				cout << "You have wrong words in your board. Do you want to see it and try to resolve it? (Y/N) ";
				char option;

				do
				{
					cin >> option;
					if (cin.fail())
					{
						cout << "Try again..." << endl;
						cin.clear();
						cin.ignore(100000, '\n');
					}
				} while (cin.fail() || (toupper(option) != 'N' && toupper(option) != 'Y'));

				if (option == 'Y') //Se o jogador escolher voltar a jogar
					goto resolvePuzzle; //Programa salta para a etiqueta resolvePuzzle -> dentro do ciclo onde se pode colocar e remover palavras, tendo assim a oportunidade de tentar acabar o puzzle
				else
					break;
			}
		}
	} while (true);
}

void crosswords() //Função que mostra CROSSWORDS PLAYER no início do programa -> o facto de a barra / ter que ser escrita como // faz com que pareça desformatado no código
{
	cout << " _____                              _    _                  _		 ____    _" << endl;
	cout << "/ ____|                            | |  | |                | |		|  __\\  | |" << endl;
	cout << "| |     _ __  _____   ___   ___    | |  | |  _____  _ __  _| | ___	| |__)| | |   __ _   _   _    ___   _ __" << endl;
	cout << "| |    | '__||  _  | | __| | __|   | |__| | |  _  || '__|  _ || __|	| ___/  | |  / _` | | | | |  / _ \\ | '__|" << endl;
	cout << "| |____| |   | |_| | |__ | |__ |   |      | | |_| || |  | |_|||__ |	| |     | | | (_| | | |_| | |  __/ | |" << endl;
	cout << "\\______|_|   |_____| |___| |___|   |__/\\__| |_____||_|  |____||___|	|_|     |_| \\___,_| \\___, |  \\___| |_|" << endl;
	cout << "										             __/  |" << endl;
	cout << "										            |____/" << endl;
}

void showSynonyms(Board &b, Dictionary &d) //Função que mostra os sinónimos iniciais
{
	vector <string> words;
	vector <string> positions;

	b.getWords(words); //Função que vai buscar as palavras da solução e as coloca no vetor words
	b.getPositions(positions); //Função que vai buscar as posições da solução e as coloca no vetor positions

	cout << "VERTICAL: " << endl;

	for (size_t i = 0; i < positions.size(); i++) //Mostra 1 sinónimo de cada palavra vertical
	{
		if (positions.at(i).at(2) == 'V')
			cout << "	" << positions.at(i).substr(0, 2) << " - " << d.showSynonym(words.at(i)) << endl;
	}

	cout << endl << "HORIZONTAL: " << endl;

	for (size_t i = 0; i < positions.size(); i++) //Mostra 1 sinónimo de cada palavra horizontal
	{
		if (positions.at(i).at(2) == 'H')
			cout << "	" << positions.at(i).substr(0, 2) << " - " << d.showSynonym(words.at(i)) << endl;
	}
	cout << endl;
}