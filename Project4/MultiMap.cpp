#include "MultiMap.h"
#include <iostream>
#include <cassert>
using namespace std;

MultiMap::ListNode::ListNode(unsigned int i)
{
	prev = next = nullptr;
	value = i;
}

MultiMap::Node::Node(string key, unsigned int value)
{
	this->key = key;
	left = right = parent = nullptr;
	numValues = 0;
	this->push_back(value);
}

MultiMap::Node::~Node()
{
	if (numValues == 1) //Trivial deletion
	{
		delete head;
		head = nullptr;
	}
	else
	{
		ListNode* p = head;
		while(p != nullptr)
		{
			ListNode* n = p;
			p = p->next;
			delete n;
			n = nullptr;
		}
	}
}

void MultiMap::Node::push_back(unsigned int i)
{
	if (numValues == 0)
		head = new ListNode(i);
	else
	{
		ListNode* p = head;
		while (p->next != nullptr)
			p = p->next;
		p->next = new ListNode(i);
		p->next->prev = p;
	}	
	numValues++;
}

MultiMap::Node* MultiMap::find(string key, Node* cur) const
{
	while(cur)
	{
		if (cur->key == key)
			return cur;
		else if (key < cur->key) cur = cur->left;  //If the key is less than, go left
		else cur = cur->right;                     //If the key is greater than, go right
	}
	return nullptr;              
} 

MultiMap::Iterator::Iterator(): ptr(nullptr), listPtr(nullptr){};

MultiMap::Iterator::Iterator(Node* p)
{
	ptr = p;
	if(ptr)
		listPtr = ptr->head;
}

void MultiMap::Iterator::printInfo()
{
	if(!valid())
		cout << "This node is invalid" << endl;
	else
		cout << "This iterator's key is " << ptr->key << " and it's first value is " << ptr->head->value << endl;
}

bool MultiMap::Iterator::valid() const
{
	return ptr != nullptr;
}

string MultiMap::Iterator::getKey() const
{
	if (ptr == nullptr)
	{
		cout << "Error: iterator is invalid, terminating. Press enter to exit.";
		cin.ignore(1000,'\n');
		exit(1);
	}
	return ptr->key;
}

unsigned int MultiMap::Iterator::getValue() const
{
	if (ptr == nullptr)
	{
		cout << "Error: iterator is invalid, terminating. Press enter to exit.";
		cin.ignore(1000,'\n');
		exit(1);
	}
	return listPtr->value;
}

bool MultiMap::Iterator::next()
{
	if (!valid()) return false;

	if (listPtr->next != nullptr) //There are still more associations in the same node
		listPtr = listPtr->next;
	else                         
	{
		Node* cur = ptr;
		if (cur->right) //A subtree exists here
		{
			cur = cur->right; //Enter the subtree, root at current node
			while(cur->left)  //Keep going down the tree to the left until you hit a leaf
				cur = cur->left;
			ptr = cur;
			listPtr = cur->head;
		}
		else 			//Right node is null, therefore the successor is higher up or does not exist
		{
			while(cur)  //Traverse the parents until a match is found or cur is null
			{
				//The next item in the map will be an ancestor of the current node, and since it is larger it must contain current node on the left
				if (cur->parent && cur->parent->left == cur) //First node with left
					break;					 //Get out, cur is the right node
				cur = cur->parent;
			}
			if (!cur)
			{
				ptr = nullptr;
				listPtr = nullptr;
			}
			else
			{
				ptr = cur->parent;
				listPtr = ptr->head;
			}
		}
	}
	return true;
}

bool MultiMap::Iterator::prev()
{
	if (!valid()) return false;

	if (listPtr->prev != nullptr) //There are still more associations in the same node
		listPtr = listPtr->prev;
	else                         //Find the next node if possible
	{
		Node* cur = ptr;
		if (cur->left)
		{
			cur = cur->left;
			while(cur->right)
				cur = cur->right;
			ptr = cur;
			listPtr = ptr->head;
			while(listPtr->next)
				listPtr = listPtr->next;
		}
		else
		{
			while(cur)
			{
				if (cur->parent && cur->parent->right == cur)
					break;
				cur = cur->parent;
			}
			if (!cur)
			{
				ptr = nullptr;
				listPtr = nullptr;
			}
			else
			{
				ptr = cur->parent;
				listPtr = ptr->head;
				while(listPtr->next)
					listPtr = listPtr->next;
			}
		}

	}
	return true;
}

MultiMap::MultiMap(): root(nullptr), m_size(0){};

MultiMap::~MultiMap()
{
	deleteAll(root);
}

void MultiMap::clear()
{
	deleteAll(root);
	root = nullptr;
}

void MultiMap::deleteAll(Node* cur)
{
	if (cur == nullptr) return;

	deleteAll(cur->left);
	deleteAll(cur->right);
	delete cur;
	cur = nullptr;
}

void MultiMap::insert(string key, unsigned int value)
{
	if (m_size == 0) //No nodes in the map
		root = new Node(key, value);
	else 			 //At least one node, have to traverse
	{
		Node* cur = root;
		Node* p = cur;
		while(cur) //This will find the parent node of the node to be created and inserted
		{
			p = cur;            //Parent pointer
			if (key < cur->key) //key is less than, have to go left
				cur = cur->left;
			else if (key > cur->key) //key is greater than, have to go right
				cur = cur->right;
			else if (key == cur->key) //key is the same, add another value to that node
			{
				cur->push_back(value);
				return;
			}
		}
		//Other cases, have to create a new BST node
		if (key < p->key)   //Insert left
		{	
			p->left = new Node(key, value);
			p->left->parent = p;
		}
		else                //Insert right
		{
			p->right = new Node(key, value);
			p->right->parent = p;
		}
	}
	m_size++;
}

MultiMap::Iterator MultiMap::findEqual(string key) const
{
	Node* cur = find(key, root);
	MultiMap::Iterator itr(cur); //If find returns nullptr, the itr will be invalid
	return itr;
}

MultiMap::Iterator MultiMap::findEqualOrSuccessor(string key) const
{
	Node* cur = root;
	while(cur)
	{
		Node* p = cur; //Parent pointer
		if (cur->key == key)
			break;
		else if (key < cur->key)
		{
			cur = cur->left;
			if(!cur)
			{
				cur = p;
				break;
			}
		}
		else if (key > cur->key)
		{
			cur = cur->right;
			if(!cur) //We hit the end of a subtree
			{
				while (p && p->key < key) //While the node's key is less than what we want
					p = p->parent;
				cur = p;
				break;
			}
		}		
	}
	//cur will either point to the right node or be a nullptr, so let's make a valid/invalid iterator respectively
	MultiMap::Iterator itr(cur);
	return itr;
}

MultiMap::Iterator MultiMap::findEqualOrPredecessor(string key) const
{
	Node* cur = root;
	while (cur)
	{
		Node* p = cur; //Parent pointer
		if (cur->key == key)
			break;
		else if (key < cur->key)
		{
			cur = cur->left;
			if(!cur)
			{
				while (p != nullptr && p->key > key)
					p = p->parent;
				cur = p;
				break;
			}
		}
		else if (key > cur->key)
		{
			cur = cur->right;
			if(!cur) //We hit the end of a subtree
			{
				cur = p;
				break;
			}
		}		
	}
	MultiMap::Iterator itr(cur);
	return itr;
}