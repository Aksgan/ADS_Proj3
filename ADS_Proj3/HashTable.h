#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
using namespace std;
const int HASH_SIZE = 200;
 
template <class ItemType>
class HashTable
{
public:
	string filename;
	HashTable(int hashSze = HASH_SIZE);
	bool IsEmpty(int key);
	bool  IsFull();
	int Hash(ItemType newItem);
	void Insert(ItemType newItem);
	void Append(int key, ItemType newItem);
	bool Remove(ItemType deleteItem, int key = -1);
	int Count(ItemType searchItem);

private:
	struct node	
	{			
		ItemType currentItem;		
		node* next;	
	};
	node** head; 
	int hashSize; 
	int totElems;
	int* bucketSize;
};
 


template<class ItemType>
HashTable<ItemType>::HashTable(int hashSze)
{
	hashSize = hashSze;
	head = new node*[hashSize];
	bucketSize = new int[hashSize];
	for (int x = 0; x < hashSize; ++x)
	{
		head[x] = NULL;
		bucketSize[x] = 0;
	}
	totElems = 0;
}

template<class ItemType>
bool HashTable<ItemType>::IsEmpty(int key)
{
	if (key >= 0 && key < hashSize)
	{
		return head[key] == NULL;
	}
	return true;
}

template<class ItemType>
bool HashTable<ItemType>::IsFull()
{
	try
	{
		node* location = new node;
		delete location;
		return false;
	}
	catch (std::bad_alloc&)
	{
		return true;
	}
}

template<class ItemType>
int HashTable<ItemType>::Hash(ItemType newItem)
{
	long h = 19937;
	std::stringstream convert;

	// convert the parameter to a string using "stringstream" which is done
	// so we can hash multiple datatypes using only one function
	convert << newItem;
	std::string temp = convert.str();

	for (unsigned x = 0; x < temp.length(); ++x)
	{
		h = (h << 6) ^ (h >> 26) ^ temp[x];
	}
	return abs(h % hashSize);
}

template<class ItemType>
void HashTable<ItemType>::Insert(ItemType newItem)
{
	if (IsFull())
	{
		std::cout<<"\nINSERT ERROR - HASH TABLE FULL\n";
	}
	else
	{
		int key = Hash(newItem);
		Append(key, newItem);
	}
}

template<class ItemType>
void HashTable<ItemType>::Append(int key, ItemType newItem)
{
	if (IsFull())
	{
		//std::cout<<"nAPPEND ERROR - HASH TABLE FULLn";
	}
	else
	{
		node* newNode = new node;  // adds new node
		newNode->currentItem = newItem;
		newNode->next = NULL;

		if (IsEmpty(key))
		{
			head[key] = newNode;
		}
		else
		{
			node* tempPtr = head[key];
			while (tempPtr->next != NULL)
			{
				tempPtr = tempPtr->next;
			}
			tempPtr->next = newNode;
		}
		++bucketSize[key];
		++totElems;
	}
}

template<class ItemType>
bool HashTable<ItemType>::Remove(ItemType deleteItem, int key)
{
	bool isFound = false;
	node* tempPtr;
	if (key == -1)
	{
		key = Hash(deleteItem);
	}

	if (IsEmpty(key))
	{
		//std::cout<<"nREMOVE ERROR - HASH TABLE EMPTYn";
	}
	else if (head[key]->currentItem == deleteItem)
	{
		tempPtr = head[key];
		head[key] = head[key]->next;
		delete tempPtr;
		--totElems;
		--bucketSize[key];
		isFound = true;
	}
	else
	{
		for (tempPtr = head[key]; tempPtr->next != NULL; tempPtr = tempPtr->next)
		{
			if (tempPtr->next->currentItem == deleteItem)
			{
				node* deleteNode = tempPtr->next;
				tempPtr->next = tempPtr->next->next;
				delete deleteNode;
				isFound = true;
				--totElems;
				--bucketSize[key];
				break;
			}
		}
	}
	return isFound;
}

template<class ItemType>
int HashTable<ItemType>::Count(ItemType searchItem)
{
	int key = Hash(searchItem);
	int search = 0;

	if (IsEmpty(key))
	{
	}
	else
	{
		for (node* tempPtr = head[key]; tempPtr != NULL; tempPtr = tempPtr->next)
		{
			if (tempPtr->currentItem == searchItem)
			{
				++search;
			}
		}
	}
	return search;
}

