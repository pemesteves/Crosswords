#pragma once
#include <string>
#include <map>
#include <vector>

class Dictionary
{
public:
	Dictionary(const std::string &dictionaryFileName); //Construtor do dicionário (map)
	bool validWord(const std::string &word); //Verifica se a palavra é válida 
	std::vector<std::string> searchWords(std::string &wildcardWord, const int &size, const std::vector<std::string> &v); //Procura palavras
	std::vector <std::string> synOfWord(const std::string &key); //Devolve o vetor com a palavra + os sinónimos da palavra
	std::string getDictionaryName(); //Retorna o nome do dicionário
private:
	std::map <std::string, std::vector<std::string>> dictionary; //Dicionário: string -> palavras; vector<string> -> vetor com os sinónimos das palavras
	std::string dictionaryName; //Nome do dicionário
	bool wildcardMatch(const char *str, const char *strWild); //Função compara uma string "normal" com uma string que contém wildcards(*)
	bool validLine(const std::string &line); //Verifica se uma linha do diconário é válida
};

