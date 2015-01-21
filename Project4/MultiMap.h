#ifndef MULTIMAP_DEFINED
#define MULTIMAP_DEFINED

#include <string>
#include <iostream>
//MultiMap from string to unsigned int
class MultiMap
{
	struct ListNode
	{
		ListNode(unsigned int i);
		unsigned int value;
		ListNode* prev;
		ListNode* next;
	};
	struct Node
	{
		Node(std::string key, unsigned int value);
		~Node();
		std::string key;
		ListNode* head;
		Node* left;
		Node* right;
		Node* parent;
		size_t numValues;
		void push_back(unsigned int i);
	};
	std::string rootAddress;
	Node* root;
	size_t m_size;

	MultiMap(const MultiMap& other);
	MultiMap& operator=(const MultiMap& rhs);
	Node* find(std::string key, Node* cur) const;
	void deleteAll(Node* cur); //recursive deletion function called by destructor and clear

public:
	class Iterator
	{
	public:
		Iterator(); //creates invalid iterator
		Iterator(Node* p); //can create valid iterator
		bool valid() const;
		std::string getKey() const;
		unsigned int getValue() const;
		bool next();
		bool prev();
		void printInfo();
	private:
		Node* ptr;
		ListNode* listPtr;
	};

	MultiMap();
	~MultiMap();
	void clear();
	void insert(std::string key, unsigned int value);
	Iterator findEqual(std::string key) const;
	Iterator findEqualOrSuccessor(std::string key) const;
	Iterator findEqualOrPredecessor(std::string key) const;
};

#endif