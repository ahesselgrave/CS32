#include "newMap.h"
#include <iostream>
using namespace std;

Map::Map()
{
	m_numPairs = 0;
	m_dictionary = new Data[DEFAULT_MAX_ITEMS];
	m_maxSize = DEFAULT_MAX_ITEMS;
}

Map::Map(const int& size)
{
	m_numPairs = 0;
	m_dictionary = new Data[size];
	m_maxSize = size;
}

Map::~Map()
{
	delete [] m_dictionary;
}

Map::Map(const Map& src)
{
	m_numPairs = src.m_numPairs;
	m_maxSize = src.m_maxSize;
	m_dictionary = new Data[m_maxSize];
	
	for (int i = 0; i < m_numPairs; i++)
	{
		m_dictionary[i].key = src.m_dictionary[i].key;
		m_dictionary[i].value = src.m_dictionary[i].value;
	}
}

Map& Map::operator=(const Map& src)
{
	if (&src == this)
		return *this;
	delete [] m_dictionary;
	m_numPairs = src.m_numPairs;
	m_maxSize = src.m_maxSize;
	m_dictionary = new Data[m_maxSize];
	for (int i = 0; i < m_numPairs; i++)
	{
		m_dictionary[i].key = src.m_dictionary[i].key;
		m_dictionary[i].value = src.m_dictionary[i].value;
	}

	return *this;
}

bool Map::empty() const
{
	if (m_numPairs == 0)
		return true;
	else
		return false;
}

int Map::size() const {return m_numPairs;}

bool Map::insert(const KeyType& key, const ValueType& value)
{
	if (m_numPairs >= m_maxSize)
		return false;

	int uniqueCount = 0;
	for (int i = 0; i < m_numPairs; i++)
	{
		if (key == m_dictionary[i].key)
			return false;
		else
			uniqueCount++;
	}
	if (uniqueCount == m_numPairs)
	{
		m_dictionary[m_numPairs].key = key;
		m_dictionary[m_numPairs].value = value;
		m_numPairs++;
		return true;
	}
	else
		return false;
}

bool Map::update(const KeyType& key, const ValueType& value)
{
	for (int i = 0; i < m_numPairs; i++)
	{
		if (key == m_dictionary[i].key)
		{
			m_dictionary[i].value = value;
			return true;
		}
	}
	return false;
}

 bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
 {
 	for (int i = 0; i < m_numPairs; i++)
	{
		if (key == m_dictionary[i].key)
		{
			m_dictionary[i].value = value;
			return true;
		}
	}
	return this->insert(key, value);
 }

bool Map::erase(const KeyType& key)
{	
 	for (int i = 0; i < m_numPairs; i++)
	{
		if (key == m_dictionary[i].key)
		{
			for (int k = i; k < m_numPairs - 1; k++)
				m_dictionary[k] = m_dictionary[k+1];
			m_numPairs--;
			return true;
		}
	}
	return false;
}

bool Map::contains(const KeyType& key) const
{
	for (int i = 0; i < m_numPairs; i++)
		if (key == m_dictionary[i].key)
			return true;
	return false;
}

bool Map::get(const KeyType& key, ValueType& value)
{
	for (int i = 0; i < m_numPairs; i++)
	{
		if (key == m_dictionary[i].key)
		{
			value = m_dictionary[i].value;
			return true;
		}
	}
	return false;
}

bool Map::get(int i, KeyType& key, ValueType& value)
{
	if (i < 0 || i >= m_numPairs)
		return false;
	else
	{
		key = m_dictionary[i].key;
		value = m_dictionary[i].value;
		return true;
	}
}

void Map::swap(Map& other)
{
	Map tempMap = other;
	other = *this;
	cerr << "other is now *this\n";
	*this = tempMap;
	cerr << "*this is now tempMap\n";
	// int smallestSize, largestSize;
	// if (other.size() > this->size())									   //this is smaller than other
	// {
	// 	int smallestSize = this->m_numPairs;
	// 	int smallestMaxSize = this->m_maxSize;
	// 	Data smallTempArray[this->m_maxSize];  

	// 	delete [] this->m_dictionary;					 				   //Delete smaller this array, create new larger one
	// 	cerr << "Deleted this->m_dictionary\n";
	// 	this->m_dictionary = new Data[other.m_maxSize];
	// 	cerr << "this->m_dictionary reinit successful\n";
	// 	this->m_numPairs = other.m_maxSize;									//Change the size of this to be other size
	// 	for (int i = 0; i < other.m_maxSize; i++)
	// 	{
	// 		this->m_dictionary[i].key = other.m_dictionary[i].key;         //Assign everything in other to this
	// 		this->m_dictionary[i].value = other.m_dictionary[i].value;
	// 	}

	// 	delete [] other.m_dictionary;
	// 	cerr << "Deleted other.m_dictionary\n"; 		
	// 	other.m_dictionary = new Data[smallestSize];
	// 	cerr << "other.m_dictionary reinit successful\n";
	// 	other.m_numPairs = smallestSize;
	// 	cerr << "m_numPairs reinit\n";
	// 	other.m_maxSize =  smallestMaxSize;
	// 	cerr << "m_maxSize reinit\n";
	// 	for (int j = 0; j < this->m_maxSize; j++)
	// 	{
	// 		other.m_dictionary[j].key   = smallTempArray->key;     //Assign everything in other to this
	// 		other.m_dictionary[j].value = smallTempArray->value;
	// 	}
	// 	cerr << "Swapped successfully!\n";
	// 	return;
	// }
	// else												 							//Other is either smaller or same size as this
	// {	
	// 	int smallestSize = other.m_numPairs;
	// 	int smallestMaxSize = other.m_maxSize;
	// 	Data smallTempArray[other.m_maxSize];  

	// 	delete [] other.m_dictionary;					 							//Delete smaller this array, create new larger one
	// 	cerr << "Deleted other.m_dictionary\n";
	// 	other.m_dictionary = new Data[this->m_maxSize];
	// 	cerr << "other.m_dictionary reinit successful\n";
	// 	other.m_numPairs = this->m_numPairs;
	// 	other.m_maxSize = this->m_maxSize;        
	// 	for (int i = 0; i < this->m_maxSize; i++)
	// 	{
	// 		other.m_dictionary[i].key   = this->m_dictionary[i].key;         		//Assign everything in this to other
	// 		other.m_dictionary[i].value = this->m_dictionary[i].value;
	// 	}

	// 	delete [] this->m_dictionary; 
	// 	cerr << "Deleted this->m_dictionary\n";				
	// 	this->m_dictionary = new Data[smallestSize];
	// 	cerr << "this->m_dictionary reinit successful\n";
	// 	this->m_numPairs = smallestSize;
	// 	this->m_maxSize  = smallestMaxSize;
	// 	for (int j = 0; j < other.m_maxSize; j++)
	// 	{
	// 		this->m_dictionary[j].key   = smallTempArray->key;         //Assign everything in other to this
	// 		this->m_dictionary[j].value = smallTempArray->value;
	// 		cerr << "Iteration number " << j + 1 << " successful!\n";
	// 	}
	// 	cerr << "Swapped successfully!\n";
	// 	return;
	// }


}
  // Exchange the contents of this map with the other one.
//