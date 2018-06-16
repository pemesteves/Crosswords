#pragma once

#include <string>
#include <vector>
#include <map>

class Dictionary
{
public:
	Dictionary(std::string dictionaryFileName); //Construtor do dicionário (map)
	bool validWord(std::string word); //Verifica se a palavra é válida 
	std::vector <std::string> synOfWord(std::string key); //Devolve o vetor com os sinónimos da palavra
	std::string showSynonym(std::string word); //Mostra um sinónimos aleatório da palavra dada
	void setSynonyms(std::vector<std::string> &v); //Cria o map dos sinónimos
private:
	std::map <std::string, std::vector<std::string>> dictionary; //Dicionário: string -> palavras; vector<string> -> vetor com os sinónimos das palavras
	std::map <std::string, std::vector <std::string>> synonyms; //Map que contém cada palavra que está no tabuleiro e os seus respetivos sinónimos: à medida que os sinónimos são usados, são eliminados do vetor
	bool wildcardMatch(const char *str, const char *strWild); //Função que compara duas strings, com wildcards
	bool validLine(const std::string &line); //Funçaõ que verifica se uma linha do dicionário é válida
};