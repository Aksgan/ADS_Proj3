#include "Trie.h"


Node* Node::findChild(char c)
{
	for (int i = 0; i < mChildren.size(); i++)
	{
		Node* tmp = mChildren.at(i);
		if (tmp->content() == c)
		{
			return tmp;
		}
	}

	return NULL;
}

Trie::Trie()
{
	root = new Node();
}

Trie::~Trie()
{
	// Free memory
}

void Trie::addWord(string s)
{
	Node* current = root;

	if (s.length() == 0)
	{
		current->setWordMarker(); // an empty word
		return;
	}

	for (int i = 0; i < s.length(); i++)
	{
		Node* child = current->findChild(s[i]);
		if (child != NULL)
		{
			current = child;
		}
		else
		{
			Node* tmp = new Node();
			tmp->setContent(s[i]);
			current->appendChild(tmp);
			current = tmp;
		}
		if (i == s.length() - 1)
			current->setWordMarker();
	}
}


bool Trie::searchWord(string s)
{
	Node* current = root;

	while (current != NULL)
	{
		for (int i = 0; i < s.length(); i++)
		{
			Node* tmp = current->findChild(s[i]);
			if (tmp == NULL)
				return false;
			current = tmp;
		}

		if (current->wordMarker())
			return true;
		else
			return false;
	}

	return false;
}
void Trie::autocomplete(string s)
{
	cout << "\n str lrngh=" << s.length() << "\n";
	Node* current = root;
	Node* prev = root;
	//while (current != NULL)
	//{
	for (int i = 0; i < s.length() + 1; i++)
	{
		//		cout << "\nIter1:" << i;
		Node* tmp = current->findChild(s[i]);
		//		cout << "\nIter2:" << i;
		if (tmp == NULL) {
			s.pop_back();
			if (prev != root) {
				cout << "\nAuto Complete:";
				cout << "\nInit::" << s << "\n";
				recurseprint(prev, s);
			}
			else
				std::cout << "\nNo Suggestions!\n";
			return;
		}
		else {
			//			std::cout << "\nfound Word : " << tmp->content();
			//			cout << "\n iter:" << i;
			prev = tmp;
			current = tmp;

		}
	}
	//}
}

void Trie::recurseprint(Node* node, string s)
{
	//		std::cout << "\nRecurse on: " << node->content()<<"\n";
	if (node->wordMarker())
		cout << "\t" << s << node->content();
	s += node->content();
	//cout << "\n" << node->content() << " Children size: " << node->mChildren.size();
	for (int i = 0; i < node->mChildren.size(); i++)
		recurseprint(node->mChildren[i], s);
	return;
}