#include "Dictionary.h"
#include <fstream>
#include <map>
#include <utility>
#include <time.h>
#include <iostream>

using namespace std;

void capitalLetters(string &str); //Função que coloca todas as letras de uma string em maiúsculas

Dictionary::Dictionary(string dictionaryFileName) //Criador do dicionário
{
	ifstream dic(dictionaryFileName);

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
			key = LineOfFile.substr(0, LineOfFile.find(':')); //A key é a palavra que está até aos ':'
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

vector <string> Dictionary::synOfWord(string key) //Retorna o vetor com os sinónimos de cada palavra
{
	return dictionary[key];
}

bool Dictionary::validWord(string word) //Função que verifica se uma palavra é válida no dicionário
{
	if (dictionary.count(word)) //Se encontrar a palavra no map, é válida
	{
		return true;
	}

	return false;
}

bool Dictionary::wildcardMatch(const char *str, const char *strWild) //Função retirada do anexo C do pdf com as especificações do projeto 1 (comentários provenientes do mesmo anexo)
{
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

string Dictionary::showSynonym(string word) //Mostra um sinónimo aleatório da palavra dada 
{
	string help;

	if (synonyms[word].size() == 0) //Quando o vetor dos sinónimos está vazio, não há sinónimos para mostrar
	{
		help.resize(0); //help é uma string vazia -> não há sinónimos
	}
	else
	{
		help = synonyms[word].at(rand() % synonyms[word].size()); //Retira um sinónimo aleatório

		vector<string>::iterator positionToErase = find(synonyms[word].begin(), synonyms[word].end(), help);

		synonyms[word].erase(positionToErase); //Elimina o sinónimo retirado
	}
	return help;
}

void Dictionary::setSynonyms(vector<string> &v) //Cria o map synonyms
{
	pair<string, vector<string>> p;

	for (size_t i = 0; i < v.size(); i++)
	{
		p = make_pair(v.at(i), synOfWord(v.at(i))); //Par composto pela palavra e o vetor de sinónimos dessa palavra

		synonyms.insert(p);
	}
}