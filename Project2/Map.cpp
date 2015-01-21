#include "Map.h"
#include <string>
#include <iostream>
#include <cassert>
using namespace std;

Map::Map()
{
	head = nullptr;
	tail = nullptr;
	m_size = 0;
}

Map::~Map()
{
	if (m_size != 0)
	{
		Node* p = head;
		int counter = 0;
		while(p->next != nullptr) //Traverse list until p points to the last item
		{
			p = p->next;
			delete p->prev;
			counter++;
		}
		delete p;
		delete head;
		delete tail;
	}
}

Map::Map(const Map& src)
{
	if (src.size() == 0)
	{
		head = nullptr;
		tail = nullptr;
		m_size = 0;
	}
	else
	{
		Node* s = src.head;
		Node* p = new Node; 
		head = p;
		p->value = s->value;
		p->key = s->key;
		p->prev = nullptr;
		while (s->next != nullptr)
		{
			s = s->next;
			Node* q = new Node;
			p->next = q; 
			q->prev = p;
			p = q;
			q->key = s->key;
			q->value = s->value;
		}
		p->next = nullptr;
		tail = p;
		m_size = src.size();
	}
}

Map& Map::operator=(const Map& src)
{
	Node* p = head;
	Node* b = p;

	while (p != nullptr) 		    	//Delete and free memory
	{
		p = p->next;  					//Move to the next item, delete the previous
		delete b;
		b = p;
	}

	if (src.size() == 0)                
	{
		head = nullptr;
		tail = nullptr;
		m_size = 0;
		return *this;
	}
	b = new Node;
	head = b;
	p = src.head;
	b->key   = p->key;
	b->value = p->value;
	b->prev  = nullptr;
	p = p->next;
	while(p != nullptr)
	{
		Node* q = new Node;
		q->key    = p->key;
		q->value  = p->value;
		q->prev = b;
		b->next = q;
		b = q;
		p = p->next;
	}
	tail = b;
	m_size = src.size();
	return *this;
}

bool Map::empty() const 
{
	return (m_size == 0 ? true : false);
}

int Map::size() const 
{
	return m_size;
}

bool Map::contains(const KeyType& key) const
{
	if(empty())
		return false;
	Node* p = head;
	while (p != nullptr)
	{
		if(p->key == key) 
			return true;
		p = p->next;
	}
	return false;
}

bool Map::get(const KeyType& key, ValueType& value) const
{
	if(contains(key))
	{
		Node* p = head;
		while (p->key != key)
		{
			p = p->next;
		}
		value = p->value;
		return true;
	}
	else 
		return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
	if (i < 0 || i >= m_size)
		return false;
	Node* p = head;        //Assign p to the first list element, equivalent to the 0th element of an array
	int counter = 0;	   //Counter to find the "ith element"
	while (counter != i)   //Make p point to the ith element
	{
		p = p->next;
		counter++;
	}
	key = p->key;
	value = p->value;
	return true;
}

bool Map::insert(const KeyType& key, const ValueType& value)
{
	if (m_size == 0)
	{
		Node* q = new Node;
		q->key = key;
		q->value = value;
		q->prev = nullptr;
		q->next = nullptr;
		head = q;
		tail = q;
		m_size++;
	}
	else
	{
		if (contains(key))
			return false;
		Node* q = new Node;			
		q->prev = tail;				//Assign q to the spot before tail;
		q->prev->next = q; 			//Assign q to be its previous node's next
		q->next = nullptr;
		q->key = key;
		q->value = value;
		m_size++;
		tail = q;
	}

	return true;
}

bool Map::update(const KeyType& key, const ValueType& value)
{
	if(!contains(key))
		return false;
	else
	{
		Node *p = head;
		while(p->key != key) //Find the node with key
		{
			p = p->next;
		}
		p->value = value;
		return true;
	}
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
	if (contains(key))
		return (update(key, value));
	else
		return (insert(key, value));
}

bool Map::erase(const KeyType& key)
{
	if (m_size == 0 || !contains(key))
		return false;

	Node* p = head;
	if (p->key == key)         //If key is the first item's key
	{
		if (m_size == 1)
		{
			delete p; 
			head = nullptr;
			tail = nullptr;
			m_size--;
			return true;
		}
		else
		{
			head = p->next;
			delete p;
			m_size--;
			return true;
		}
	}
	else
	{
		while(p->next != nullptr)
		{
			if(p->key == key)
				break;
			p = p->next;
		}

		if (p->next == nullptr)    //If p points to the last node
		{
			tail = p->prev;
			p->prev->next = nullptr;
			delete p;
			m_size--;
			return true;
		}
		else
		{
			p->prev->next = p->next;
			p->next->prev = p->prev;
			delete p;
			m_size--;
			return true;
		}
	}
}

void Map::swap(Map& other)
{
	Node* h = head;
	Node* t = tail;
	head = other.head;
	tail = other.tail;
	other.head = h;
	other.tail = t;
}

void Map::dump() const
{
	Node* p = head;
	while (p->next != nullptr)
	{
		cerr << p->key << endl;
		p = p->next;
	}
	cerr << p->key << endl;
}


////////////////////////////////////////////////////////////////////////////////////
/////        Non-member functions
////////////////////////////////////////////////////////////////////////////////////

bool combine(const Map& m1, const Map& m2, Map& result)
{
	bool hasDifferentPairs = true;

	if (m1.empty())
	{
		result = m2;
		return true;
	}
	else if (m2.empty())
	{
		result = m1;
		return true;
	}
	else if (m1.empty() && m2.empty())
	{
		Map empty;
		result = empty;
	}
	else
	{
		while(!result.empty())
		{
			KeyType a;
			ValueType b;
			result.get(0, a, b);
			result.erase(a);
		}
		assert(result.empty());

		if (m1.size() > m2.size()) //m1 has more stuff than m2
		{
			for (int i = 0; i < m1.size(); i++)
			{
				KeyType a, b;
				ValueType c, d;

				m1.get(i, a, c);
				if (m2.contains(a) && !result.contains(a)) 					//If m2 has the same key as m1
				{
					m2.get(a, d);
					if (c != d)
						hasDifferentPairs = false;
					else
						result.insert(a, d);
				}
				else
				{
					result.insert(a, c);
				}

				m2.get(i, a, c);						//Repeat process for m2
				if (m1.contains(a) && !result.contains(a)) 				
				{
					m1.get(a, d);
					if (c != d)
						hasDifferentPairs = false;
					else
						result.insert(a, d);
				}
				else
					result.insert(a, c);

			}
		}
		else
		{
			for (int i = 0; i < m2.size(); i++)
			{
				KeyType a;
				ValueType c, d;
				m1.get(i, a, c);
				if (m2.contains(a) && !result.contains(a)) 					//If m2 has the same key as m1
				{
					m2.get(a, d);
					if (c != d)
						hasDifferentPairs = false;
					else
						result.insert(a, d);
				}
				else
				{
					result.insert(a, c);
				}

				m2.get(i, a, c);						//Repeat process for m2
				if (m1.contains(a) && !result.contains(a)) 				
				{
					m1.get(a, d);
					if (c != d)
						hasDifferentPairs = false;
					else
						result.insert(a, d);
				}
				else
					result.insert(a, c);
			}
		}
	}
	return hasDifferentPairs;
}

/*    
If a key appears in exactly one of m1 and m2, then result must contain a pair consisting of that key and its corresponding value.
If a key appears in both m1 and m2, with the same corresponding value in both, then result must contain a pair with that key and value.

When this function returns, result must contain no pairs other than those required by these rules. (You must not assume result is empty when it is passed in to this function;
it might not be.)

If there exists a key that appears in both m1 and m2, but with different corresponding values, then this function returns false; if there is no key like this,
 the function returns true. Even if the function returns false, result must be constituted as defined by the above rules. 
*/

void subtract(const Map& m1, const Map& m2, Map& result)
{
	while(!result.empty())
	{
		KeyType a;
		ValueType b;
		result.get(0, a, b);
		result.erase(a);
	}
	
	if (m1.empty())
	{
		result = m1;
		return;
	}


	assert(result.empty());

	for (int i = 0; i < m1.size(); i++)
	{
		KeyType a;
		ValueType b;
		m1.get(i, a, b);
		if (!m2.contains(a))
			result.insert(a, b);
	}
}

/*
When this function returns, result must contain a copy of all the pairs in m1 whose keys don't appear in m2;
it must not contain any other pairs. (You must not assume result is empty when it is passed in to this function; it may not be.) 
*/