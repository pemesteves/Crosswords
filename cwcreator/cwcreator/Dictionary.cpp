#include "Dictionary.h"
#include <fstream>
#include <map>
#include <utility>
#include <time.h>
#include <iostream>

using namespace std;

void capitalLetters(string &str); //Fun��o que coloca todas as letras de uma string em mai�sculas

Dictionary::Dictionary(const string &dictionaryFileName) //Criador do dicion�rio
{
	ifstream dic(dictionaryFileName);
	dictionaryName = dictionaryFileName;

	if (dic.fail()) //Se o dicion�rio n�o existe, o programa fecha
	{
		cerr << "Input file opening failed.\n";
		exit(1); //Por vezes � prefer�vel parar o programa com um exit code != 0
	}

	cout << "Extracting dictionary..." << endl;

	string LineOfFile;
	vector <string> line; //Vetor com as palavras da linha
	string key; //key do map: 1� palavra de cada linha - at� aos ':'

	pair <string, vector<string>> p;

	string word;

	while (!dic.eof())
	{
		getline(dic, LineOfFile);

		line.resize(0);

		if (!validLine(LineOfFile)) //Se a linha n�o � v�lida, n�o � extra�da para o map
			continue;

		if (LineOfFile.find(':') != string::npos) //Se a linha tem ':' � tratada como uma nova entrada do map
		{
			key = LineOfFile.substr(0, LineOfFile.find(':'));
			capitalLetters(key);
			LineOfFile = LineOfFile.substr(LineOfFile.find(':') + 2);
			while (true)
			{
				if (LineOfFile.find(',') == string::npos) //Caso j� n�o haja ',' na linha
				{
					capitalLetters(LineOfFile);
					line.push_back(LineOfFile);
					break;

				}
				else //Se a linha ainda tiver ',' executa este peda�o de c�digo
				{
					word = LineOfFile.substr(0, LineOfFile.find(',')); //substrOfLine passa a ser a primeira palavra da linha

					if (word == "")
						LineOfFile = LineOfFile.substr(LineOfFile.find(',') + 2);
					else if (word.at(word.length() - 1) == ' ') //Pode haver algum caso em que a palavra tem um espa�o antes da ',' , que � tratado aqui
					{
						capitalLetters(word);
						line.push_back(word.substr(0, word.find(' ')));
						LineOfFile = LineOfFile.substr(LineOfFile.find(',') + 2);
					}
					else if (LineOfFile.at(LineOfFile.length() - 1) == ',') //Trata outro caso em que a linha termina em ','
						LineOfFile.at(LineOfFile.length() - 1) = '\n';
					else //Qualquer outro caso (palavras inv�lidas em linhas com v�rias palavras)
					{
						capitalLetters(word);
						line.push_back(word);
						LineOfFile = LineOfFile.substr(LineOfFile.find(',') + 2);
					}
				}
			}

			p = make_pair(key, line);

			dictionary.insert(p); //Inserir o pair p no map dictionary
		}
		else //Caso a linha n�o tenha uma key
		{
			while (true)
			{
				if (LineOfFile.find(',') == string::npos) //Caso j� n�o haja ',' na linha
				{
					capitalLetters(LineOfFile);
					dictionary[key].push_back(LineOfFile);
					break;

				}
				else //Se a linha ainda tiver ',' executa este peda�o de c�digo
				{
					word = LineOfFile.substr(0, LineOfFile.find(',')); //substrOfLine passa a ser a primeira palavra da linha

					if (word.at(word.length() - 1) == ' ') //Pode haver algum caso em que a palavra tem um espa�o antes da ',' , que � tratado aqui
					{
						capitalLetters(word);
						dictionary[key].push_back(word.substr(0, word.find(' ')));
						LineOfFile = LineOfFile.substr(LineOfFile.find(',') + 2);
					}
					else if (LineOfFile.at(LineOfFile.length() - 1) == ',') //Trata outro caso em que a linha termina em ','
						LineOfFile.at(LineOfFile.length() - 1) = '\n';
					else //Qualquer outro caso (palavras inv�lidas em linhas com v�rias palavras)
					{
						capitalLetters(word);
						dictionary[key].push_back(word);
						LineOfFile = LineOfFile.substr(LineOfFile.find(',') + 2);
					}
				}
			}
		}
	}

	dic.close();
}

bool Dictionary::validLine(const string &line) //Fun��o que verifica se uma linha � v�lida
{
	for (int i = 0; i < line.length(); i++)
	{
		//A linha � v�lida se n�o contiver carateres diferentes de espa�o, dois pontos, v�rgula ou alfanum�ricos
		if (line.at(i) != ' ' && line.at(i) != ':' && !isalpha(line.at(i)) && line.at(i) != ',')
			return false;
	}

	return true;
}

vector <string> Dictionary::synOfWord(const string &key) //Retorna os sin�nimos das palavras
{
	return dictionary[key];
}

bool Dictionary::validWord(const string &word) //Verifica se uma palavra exite no dicion�rio
{
	if (dictionary.count(word)) //dictionary.count(word) conta o n�mero de ocorr�ncias de word em dictionary: como se trata de um map, o m�ximo � 1
	{
		return true;
	}

	return false;
}

vector<string> Dictionary::searchWords(string &wildcardWord, const int &size, const vector<string> &v) //Procura palavras que respeitem tanto o size como a wildcardWord
{
	vector <string> words; //Vetor que ser� devolvido

	for (auto e : dictionary) //Percorre o dicion�rio
	{
		if (wildcardMatch(e.first.c_str(), wildcardWord.c_str())) //Compara a string com a palavra do map: se forem iguais realiza o c�digo sguinte
		{
			if (e.first.length() <= size) //Se a palavra do dicion�rio tiver o tamanho dado ou menor, � colocada no vetor, caso ainda n�o esteja
			{
				for (size_t i = 0; i < v.size(); i++)
				{
					if (v.at(i) == e.first)
						goto exitCicle;
				}
				words.push_back(e.first);
			exitCicle:;
			}
			else
			{
				continue;
			}
		}
	}

	return words;
}

bool Dictionary::wildcardMatch(const char *str, const char *strWild) //Fun��o retirada do anexo C do pdf com as especifica��es do projeto 1 (coment�rios com // provenientes do mesmo anexo)
{
	/*
	Fun��o compara uma string "normal" com uma string que cont�m wildcards (*)
	*/

	// We have a special case where string is empty ("") and the mask is "*".
	// We need to handle this too. So we can't test on !*str here.
	// The loop breaks when the match string is exhausted.
	while (*strWild)
	{
		// Single wildcard character
		if (*strWild == '?')
		{
			// Matches any character except empty string
			if (!*str)
				return false;
			// OK next
			++str;
			++strWild;
		}
		else if (*strWild == '*')
		{
			// Need to do some tricks.
			// 1. The wildcard * is ignored.
			// So just an empty string matches. This is done by recursion.
			// Because we eat one character from the match string,
			// the recursion will stop.
			if (wildcardMatch(str, strWild + 1))
				// we have a match and the * replaces no other character
				return true;
			// 2. Chance we eat the next character and try it again,
			// with a wildcard * match. This is done by recursion.
			// Because we eat one character from the string,
			// the recursion will stop.
			if (*str && wildcardMatch(str + 1, strWild))
				return true;
			// Nothing worked with this wildcard.
			return false;
		}
		else
		{
			// Standard compare of 2 chars. Note that *str might be 0 here,
			// but then we never get a match on *strWild
			// that has always a value while inside this loop.
			if (toupper(*str++) != toupper(*strWild++))
				return false;
		}
	}
	// Have a match? Only if both are at the end...
	return !*str && !*strWild;
}

string Dictionary::getDictionaryName() //Retorna o nome do dicion�rio
{
	return dictionaryName;
}