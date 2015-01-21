#include "SalaryMap.h"
#include "Map.h"
#include <string>
#include <iostream>

SalaryMap::SalaryMap() {}

bool SalaryMap::add(KeyType name, ValueType salary)
{
	if (salary < 0)
		return false;
	else
		return m_Map.insert(name, salary);
}

bool SalaryMap::raise(KeyType name, ValueType pct)
{
	if (pct < -100)
		return false;
	ValueType value;
	if (m_Map.get(name, value))
		return m_Map.update(name, value * (1 + pct / 100));
}

double SalaryMap::salary(KeyType name) const
{
	ValueType salary;
	Map tempMap = m_Map;
	if(tempMap.get(name, salary))
		return salary;
	else
		return -1;
}

int SalaryMap::size() const
{
	return m_Map.size();
}

void SalaryMap::print() const
{
	KeyType name;
	ValueType value;
	Map tempMap = m_Map;
	for (int i =0; i < m_Map.size(); i++)
	{
		if (tempMap.get(i, name, value))
			std::cout << name << " " << value << std::endl;
	}
}
