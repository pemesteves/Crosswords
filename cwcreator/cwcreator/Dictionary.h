#pragma once
#include <string>
#include <map>
#include <vector>

class Dictionary
{
public:
	Dictionary(const std::string &dictionaryFileName); //Construtor do dicion�rio (map)
	bool validWord(const std::string &word); //Verifica se a palavra � v�lida 
	std::vector<std::string> searchWords(std::string &wildcardWord, const int &size, const std::vector<std::string> &v); //Procura palavras
	std::vector <std::string> synOfWord(const std::string &key); //Devolve o vetor com a palavra + os sin�nimos da palavra
	std::string getDictionaryName(); //Retorna o nome do dicion�rio
private:
	std::map <std::string, std::vector<std::string>> dictionary; //Dicion�rio: string -> palavras; vector<string> -> vetor com os sin�nimos das palavras
	std::string dictionaryName; //Nome do dicion�rio
	bool wildcardMatch(const char *str, const char *strWild); //Fun��o compara uma string "normal" com uma string que cont�m wildcards(*)
	bool validLine(const std::string &line); //Verifica se uma linha do dicon�rio � v�lida
};

