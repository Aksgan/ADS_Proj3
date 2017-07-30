#include <iostream>
#include <fstream>
#include <cctype>
#include <cstring>
#include <string>
#include <sstream>
#include<istream>
#include<fstream>
#include<vector>

#include "HashTable.h"
#include"Trie.h"

#pragma warning(disable : 4996)	//to use strtok
using namespace std;

string SpellCheck_returnWord(HashTable<string>& hashTable, string word);
int SpellCheck_returnCount(HashTable<string>& hashTable, string word);
string Upper_Lower_Case(string word)
{
	for (unsigned x = 0; x < word.length(); ++x)
	{
		word[x] = tolower(word[x]);
	}
	return word;
}

std::string NumberToString(int Number)
{
	std::ostringstream ss;
	ss << Number;
	return ss.str();
}



int main()
{

	cout << "\t---ADS Project 3---\n";

	//Start of Dictionary.txt
	const int TABLE_SIZE = 20000;
	string Input_File, currWord, line, fixedWord;
	const char* punctuations = " ,.-\':;?()+*/\\%$#!\"@^&[]<>1234567890=|";
	ifstream infile;
	HashTable<string> hashTable(TABLE_SIZE);
	Trie* trie = new Trie();
	infile.open("dictionary.txt");
	if (infile.fail())
	{
		cout << "ERROR - Dictionary.txt read fail\n";
		//exit(1);
		return 0;
	}

	while (infile >> currWord)
	{
		trie->addWord(currWord);
		// for repeat words
		if (!hashTable.Count(currWord))
		{
			hashTable.Insert(currWord);
		}
	}
	infile.close();

	cout << "\nDictionary.txt loaded!";
	//End of dictionary

	// Start of tobeSpellCheck_returnWord.txt
	ifstream file("TobeSpellChecked.txt", ios::in);
	int i = 0;
	if (file.is_open())
	{
		while (!file.eof())
		{
			getline(file, line);

			Input_File.append(line);
			Input_File.push_back(' ');
			i++;
		}

		file.close();
		Input_File.push_back('\0');
		cout << "\nTobeSpellCheck.txt Loaded!";
	}
	else
	{
		std::cout << "ERROR - TobeSpellCheck.txt read fail";
		return 0;
	}

	//End of TobeSpellCheck_returnWorded.txt



	int option;
	bool again = true;

//	while (again) {
		cout << "\n\nFor Spell Check - Enter 1\nFor Auto-complete - Enter 2\nFor Document Retrival Enter 3\n";
		cin >> option;

		// file search
		if (option == 3) {
			int file_n = 0;

			vector<HashTable<string>> FileSet;
			cout << "\nEnter FileSet files Count";
			int file_count;
			cin >> file_count;
			cout << "\nLoading FileSet";
			while (file_n < file_count+1)
			{
				string line;	bool flag = 0;
				string file_name = "FileSet/";
				file_name += NumberToString(file_n);
				string data;
				file_name += ".txt";
	
				ifstream file(file_name, ios::in);
				if (file.is_open())
				{
					while (!file.eof())  //read from file and store in array
					{
						getline(file, line);
						data.append(line);
						data.push_back(' ');
					}
					file.close();
				}
				else
				{
					std::cout << "\nerror loading FileSet";
					return 0;
				}
				
				data.push_back('\0');
				file_n++;
				HashTable<string> ht(20000);
				ht.filename = file_name;
				char* splitInput = strtok(const_cast<char*>(data.c_str()), punctuations);
				while (splitInput != NULL)
				{
					if (!ht.Count(splitInput))
					{
						ht.Insert(splitInput);
					}
					splitInput = strtok(NULL, punctuations);
				}
				FileSet.push_back(ht);
			}
			cout << "\n FileSet Loaded!";
			char* splitInput = strtok(const_cast<char*>(Input_File.c_str()), punctuations);
			while (splitInput != NULL)
			{
				currWord = splitInput;
				currWord = Upper_Lower_Case(currWord);
				fixedWord = SpellCheck_returnWord(hashTable, currWord);
				if (currWord != fixedWord)
					cout << "\nSpelling Error,Replacing : " << currWord << " with " << fixedWord;
				else
					cout << "\n" << fixedWord;
				splitInput = strtok(NULL, punctuations);

				cout << endl;
				for (int i = 0; i < FileSet.size(); i++) {
					if (FileSet[i].Count(fixedWord) > 0)
						cout << FileSet[i].filename << ",  ";
				}
				cout << endl;


			}
		}
		//autocomplete
		else if (option == 2) {
			string prefix;
			cout << "Enter prefix word : ";
			cin >> prefix;
			trie->autocomplete(prefix);
			cout << "\n";
		}

		//autocorrect
		else if (option == 1) {
			int result = 0;
			char* splitInput = strtok(const_cast<char*>(Input_File.c_str()), punctuations);
			while (splitInput != NULL)
			{
				currWord = splitInput;
				currWord = Upper_Lower_Case(currWord);
				result += SpellCheck_returnCount(hashTable, currWord);
				splitInput = strtok(NULL, punctuations);
			}
			if (result > 0)
			{
				cout << "Total Incorrect words: " << result << endl;
				result = 0;
			}


		}

//		cout << "\nAgain? (Y/N)";
//		char op;
//		cin >> op;
//		if (op == 'N')
//			again = false;
//	}

	return 0;

}// end of main


string SpellCheck_returnWord(HashTable<string>& hashTable, string word)
{
	int result = 0;
	int suggestion = 0;
	string remove[256];
	int numRemove = 0;
	//	int word_count = 0;
	for (int word_count = 0; word_count < 1; word_count++) {
		if (!hashTable.Count(word))
		{
			++result;
			for (unsigned x = 0; x < word.length(); ++x)
			{
				string modified_word = word;
				for (char c = 'a'; c <= 'z'; ++c)
				{
					modified_word[x] = c;
					if (hashTable.Count(modified_word))
					{
						return modified_word;
						remove[numRemove++] = modified_word;
						++suggestion;
						hashTable.Remove(modified_word);
					}

					string insert_word = word.substr(0, x) + c + word.substr(x);
					if (hashTable.Count(insert_word))
					{
						remove[numRemove++] = insert_word;
						++suggestion; return insert_word;
						hashTable.Remove(insert_word);
					}
				}
			}
			for (unsigned x = 0; x < word.length() - 1; ++x)
			{
				string transposition = word.substr(0, x) + word[x + 1] + word[x] + word.substr(x + 2);
				if (hashTable.Count(transposition))
				{
					return transposition;
					remove[numRemove++] = transposition;
					++suggestion;
					hashTable.Remove(transposition);

				}
				string deletion = word.substr(0, x) + word.substr(x + 1);
				if (hashTable.Count(deletion))
				{
					return deletion;
					remove[numRemove++] = deletion;
					++suggestion;
					hashTable.Remove(deletion);
				}
			}
			while (numRemove >= 0)
			{
				hashTable.Insert(remove[numRemove--]);
			}
			if (suggestion < 1)
			{}
		}
		return word;
	}
}
int SpellCheck_returnCount(HashTable<string>& hashTable, string word)
{
	int result = 0;
	int suggestion = 0;
	string remove[256];
	int numRemove = 0;

	if (!hashTable.Count(word))
	{
		++result;
		cout << "Incorrect Word: " << word << ", Did you Mean: ";

		for (unsigned x = 0; x < word.length(); ++x)
		{
			string modified_word = word;
			for (char c = 'a'; c <= 'z'; ++c)
			{
				//modified_word
				modified_word[x] = c;
				if (hashTable.Count(modified_word))
				{
					cout << modified_word << ", ";
					remove[numRemove++] = modified_word;
					++suggestion;
					// remove the entry so it isnt displayed multiple times
					hashTable.Remove(modified_word);
				}

				string insert_word = word.substr(0, x) + c + word.substr(x);
				if (hashTable.Count(insert_word))
				{
					cout << insert_word << ", ";
					remove[numRemove++] = insert_word;
					++suggestion;
					// remove the entry so it isnt displayed multiple times
					hashTable.Remove(insert_word);
				}
			}
		}
		// transposition & deletion	
		for (unsigned x = 0; x < word.length() - 1; ++x)
		{
			// transposition 
			string transposition = word.substr(0, x) + word[x + 1] + word[x] + word.substr(x + 2);
			if (hashTable.Count(transposition))
			{
				cout << transposition << ", ";
				remove[numRemove++] = transposition;
				++suggestion;
				// remove the entry so it isnt displayed multiple times
				hashTable.Remove(transposition);

			}
			// deletion
			string deletion = word.substr(0, x) + word.substr(x + 1);
			if (hashTable.Count(deletion))
			{
				cout << deletion << ", ";
				remove[numRemove++] = deletion;
				++suggestion;
				// remove the entry so it isnt displayed multiple times
				hashTable.Remove(deletion);
			}
		}
		// place the removed items back inside the hash table
		while (numRemove >= 0)
		{
			hashTable.Insert(remove[numRemove--]);
		}
		if (suggestion < 1)
		{
			cout << "Spelling Error, no word found in Dictionary!";
		}
		cout << endl << endl;
	}
	return result;
}