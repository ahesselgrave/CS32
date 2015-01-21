#include "Map.h"
#include <iostream>
using namespace std;

Map::Map() :m_numPairs(0) {}

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
	if (m_numPairs >= DEFAULT_MAX_ITEMS)
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
	int smallestSize, largestSize;
	if (other.size() > this->size())
	{
		largestSize = other.size();
		smallestSize = this->size();
		other.m_numPairs = smallestSize;
		this->m_numPairs = largestSize;
		for (int j = smallestSize; j < largestSize; j++)  //this Map becomes the larger map, move excess pieces
		{
			this->m_dictionary[j].key = other.m_dictionary[j].key;
			this->m_dictionary[j].value = other.m_dictionary[j].value;
		}
	
	}
	else
	{	
		largestSize = this->size();
		smallestSize = other.size();
		other.m_numPairs = largestSize;
		this->m_numPairs = smallestSize;
		for (int k = smallestSize; k < largestSize; k++) //same process when other becomes bigger
		{
			other.m_dictionary[k].key = this->m_dictionary[k].key;
			other.m_dictionary[k].value = this->m_dictionary[k].value;
		}
	}

	for (int i = 0; i < smallestSize; i++) //Everything in between
	{
		KeyType otherKey = other.m_dictionary[i].key;
		ValueType otherValue = other.m_dictionary[i].value;

		other.m_dictionary[i].key = this->m_dictionary[i].key;
		other.m_dictionary[i].value = this->m_dictionary[i].value;

		this->m_dictionary[i].key = otherKey;
		this->m_dictionary[i].value = otherValue;
	}

}
  // Exchange the contents of this map with the other one.
//