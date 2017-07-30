#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Node {
public:
	Node() { mContent = ' '; mMarker = false; }
	~Node() {}
	char content() { return mContent; }
	void setContent(char c) { mContent = c; }
	bool wordMarker() { return mMarker; }
	void setWordMarker() { mMarker = true; }
	Node* findChild(char c);
	void appendChild(Node* child) { mChildren.push_back(child); }
	vector<Node*> children() { return mChildren; }
	char mContent;
	bool mMarker;
	vector<Node*> mChildren;
};

class Trie {
public:
	Trie();
	~Trie();
	void addWord(string s);
	bool searchWord(string s);
	//	void deleteWord(string s);
	void autocomplete(string s);
	void recurseprint(Node* n, string s);
private:
	Node* root;
};