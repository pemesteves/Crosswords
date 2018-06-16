#include "Dictionary.h"
#include <fstream>
#include <map>
#include <utility>
#include <time.h>
#include <iostream>

using namespace std;

void capitalLetters(string &str); //Função que coloca todas as letras de uma string em maiúsculas

Dictionary::Dictionary(const string &dictionaryFileName) //Criador do dicionário
{
	ifstream dic(dictionaryFileName);
	dictionaryName = dictionaryFileName;

	if (dic.fail()) //Se o dicionário não existe, o programa fecha
	{
		cerr << "Input file opening failed.\n";
		exit(1); //Por vezes é preferível parar o programa com um exit code != 0
	}

	cout << "Extracting dictionary..." << endl;

	string LineOfFile;
	vector <string> line; //Vetor com as palavras da linha
	string key; //key do map: 1ª palavra de cada linha - até aos ':'

	pair <string, vector<string>> p;

	string word;

	while (!dic.eof())
	{
		getline(dic, LineOfFile);

		line.resize(0);

		if (!validLine(LineOfFile)) //Se a linha não é válida, não é extraída para o map
			continue;

		if (LineOfFile.find(':') != string::npos) //Se a linha tem ':' é tratada como uma nova entrada do map
		{
			key = LineOfFile.substr(0, LineOfFile.find(':'));
			capitalLetters(key);
			LineOfFile = LineOfFile.substr(LineOfFile.find(':') + 2);
			while (true)
			{
				if (LineOfFile.find(',') == string::npos) //Caso já não haja ',' na linha
				{
					capitalLetters(LineOfFile);
					line.push_back(LineOfFile);
					break;

				}
				else //Se a linha ainda tiver ',' executa este pedaço de código
				{
					word = LineOfFile.substr(0, LineOfFile.find(',')); //substrOfLine passa a ser a primeira palavra da linha

					if (word == "")
						LineOfFile = LineOfFile.substr(LineOfFile.find(',') + 2);
					else if (word.at(word.length() - 1) == ' ') //Pode haver algum caso em que a palavra tem um espaço antes da ',' , que é tratado aqui
					{
						capitalLetters(word);
						line.push_back(word.substr(0, word.find(' ')));
						LineOfFile = LineOfFile.substr(LineOfFile.find(',') + 2);
					}
					else if (LineOfFile.at(LineOfFile.length() - 1) == ',') //Trata outro caso em que a linha termina em ','
						LineOfFile.at(LineOfFile.length() - 1) = '\n';
					else //Qualquer outro caso (palavras inválidas em linhas com várias palavras)
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
		else //Caso a linha não tenha uma key
		{
			while (true)
			{
				if (LineOfFile.find(',') == string::npos) //Caso já não haja ',' na linha
				{
					capitalLetters(LineOfFile);
					dictionary[key].push_back(LineOfFile);
					break;

				}
				else //Se a linha ainda tiver ',' executa este pedaço de código
				{
					word = LineOfFile.substr(0, LineOfFile.find(',')); //substrOfLine passa a ser a primeira palavra da linha

					if (word.at(word.length() - 1) == ' ') //Pode haver algum caso em que a palavra tem um espaço antes da ',' , que é tratado aqui
					{
						capitalLetters(word);
						dictionary[key].push_back(word.substr(0, word.find(' ')));
						LineOfFile = LineOfFile.substr(LineOfFile.find(',') + 2);
					}
					else if (LineOfFile.at(LineOfFile.length() - 1) == ',') //Trata outro caso em que a linha termina em ','
						LineOfFile.at(LineOfFile.length() - 1) = '\n';
					else //Qualquer outro caso (palavras inválidas em linhas com várias palavras)
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

bool Dictionary::validLine(const string &line) //Função que verifica se uma linha é válida
{
	for (int i = 0; i < line.length(); i++)
	{
		//A linha é válida se não contiver carateres diferentes de espaço, dois pontos, vírgula ou alfanuméricos
		if (line.at(i) != ' ' && line.at(i) != ':' && !isalpha(line.at(i)) && line.at(i) != ',')
			return false;
	}

	return true;
}

vector <string> Dictionary::synOfWord(const string &key) //Retorna os sinónimos das palavras
{
	return dictionary[key];
}

bool Dictionary::validWord(const string &word) //Verifica se uma palavra exite no dicionário
{
	if (dictionary.count(word)) //dictionary.count(word) conta o número de ocorrências de word em dictionary: como se trata de um map, o máximo é 1
	{
		return true;
	}

	return false;
}

vector<string> Dictionary::searchWords(string &wildcardWord, const int &size, const vector<string> &v) //Procura palavras que respeitem tanto o size como a wildcardWord
{
	vector <string> words; //Vetor que será devolvido

	for (auto e : dictionary) //Percorre o dicionário
	{
		if (wildcardMatch(e.first.c_str(), wildcardWord.c_str())) //Compara a string com a palavra do map: se forem iguais realiza o código sguinte
		{
			if (e.first.length() <= size) //Se a palavra do dicionário tiver o tamanho dado ou menor, é colocada no vetor, caso ainda não esteja
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

bool Dictionary::wildcardMatch(const char *str, const char *strWild) //Função retirada do anexo C do pdf com as especificações do projeto 1 (comentários com // provenientes do mesmo anexo)
{
	/*
	Função compara uma string "normal" com uma string que contém wildcards (*)
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

string Dictionary::getDictionaryName() //Retorna o nome do dicionário
{
	return dictionaryName;
}