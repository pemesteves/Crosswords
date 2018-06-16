#pragma once

#include <string>
#include <vector>
#include <map>

class Dictionary
{
public:
	Dictionary(std::string dictionaryFileName); //Construtor do dicion�rio (map)
	bool validWord(std::string word); //Verifica se a palavra � v�lida 
	std::vector <std::string> synOfWord(std::string key); //Devolve o vetor com os sin�nimos da palavra
	std::string showSynonym(std::string word); //Mostra um sin�nimos aleat�rio da palavra dada
	void setSynonyms(std::vector<std::string> &v); //Cria o map dos sin�nimos
private:
	std::map <std::string, std::vector<std::string>> dictionary; //Dicion�rio: string -> palavras; vector<string> -> vetor com os sin�nimos das palavras
	std::map <std::string, std::vector <std::string>> synonyms; //Map que cont�m cada palavra que est� no tabuleiro e os seus respetivos sin�nimos: � medida que os sin�nimos s�o usados, s�o eliminados do vetor
	bool wildcardMatch(const char *str, const char *strWild); //Fun��o que compara duas strings, com wildcards
	bool validLine(const std::string &line); //Fun�a� que verifica se uma linha do dicion�rio � v�lida
};