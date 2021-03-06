#include "Database.h"
#include <vector>
#include <string>
#include <cassert>
#include <fstream>
#include <unordered_set>
#include <cstdlib>
using namespace std;

Database::Database(){}

Database::~Database()
{
	clearAll();
}

void Database::clearAll()
{
	for (int i = 0; i < m_fieldIndex.size(); i++)   //Delete all allocated maps
		delete m_fieldIndex[i];
	m_fieldIndex.clear();
	m_schema.clear();
	m_rows.clear();	
}

bool Database::specifySchema(const vector<FieldDescriptor>& schema)
{
	clearAll();   //First empty the database
	bool atLeastOneIndexed = false;
	int indexIndex = 0;  
	for (int i = 0; i < schema.size(); i++)
	{
		m_schema.push_back(schema[i]);
		if (schema[i].index == Database::it_indexed)
		{
			atLeastOneIndexed = true;
			m_fieldIndex.push_back(new MultiMap);   //Create a new multimap to index
		}
		else
			m_fieldIndex.push_back(nullptr);
	}
	if (!atLeastOneIndexed) //If there wasn't at least one indexed thing, destroy the schema and return false
	{
		clearAll();
		return false;
	}
	return true;
}

bool Database::addRow(const vector<string>& rowOfData)
{
	if (m_schema.size() == 0 || rowOfData.size() != m_schema.size()) //No schema exists right now or row does not match current schema, do nothing
		return false;
	unsigned int rowNum = m_rows.size();
	m_rows.push_back(rowOfData);
	for (int i = 0; i < m_rows[rowNum].size(); i++)
	{
		if (m_schema[i].index == Database::it_indexed) //The schema is indexed, so we need to put it in the multimap
		{
			assert(m_fieldIndex[i] != nullptr);
			m_fieldIndex[i]->insert(m_rows[rowNum][i], rowNum);
		}
	}
	return true;
}

bool Database::loadFromURL(string url)
{
	string contents;
	if(HTTP().get(url, contents))
	{
		size_t numFields = 0;
		Tokenizer line(contents, "\n"); //Tokenize each line;
		string w;
		while(line.getNextToken(w))  //Go through every line
		{
			if (numFields == 0)  //Indicating schema;
			{
				vector<FieldDescriptor> schemaVec;
				Tokenizer schema(w, ",");
				string fields;
				while(schema.getNextToken(fields))
				{
					if (fields[fields.size()-1] == '*') //The field is indexed
					{
						fields = fields.substr(0, fields.size()-1);
						Database::FieldDescriptor fd;
						fd.name = fields;
						fd.index = Database::it_indexed;
						schemaVec.push_back(fd);
					}
					else 
					{
						Database::FieldDescriptor fd;
						fd.name = fields;
						fd.index = Database::it_none;
						schemaVec.push_back(fd);
					}
					numFields++;
				}
				specifySchema(schemaVec);
				continue;
			}

			Tokenizer row(w, ",");
			size_t dataCount = 0;     //Checks to confirm data matches the schema
			string q;
			vector<string> rowVec;
			while(row.getNextToken(q))
			{
				rowVec.push_back(q);
				dataCount++;
			}
			if (dataCount != numFields)
			{
				clearAll();
				return false;
			}
			addRow(rowVec);
		}
		return true;
	}
	else return false;
}

bool Database::loadFromFile(string filename)
{
	ifstream file(filename);
	if(file)  //If a file was loaded
	{
		string w;
		size_t numFields = 0, i = 0;
		while(getline(file, w)) //For every line in the file (no \n)
		{
			if (numFields == 0)  //Indicating schema;
			{
				vector<FieldDescriptor> schemaVec;
				Tokenizer schema(w, ",");
				string fields;
				while(schema.getNextToken(fields))
				{
					if (fields[fields.size()-1] == '*') //The field is indexed
					{
						fields = fields.substr(0, fields.size()-1);
						Database::FieldDescriptor fd;
						fd.name = fields;
						fd.index = Database::it_indexed;
						schemaVec.push_back(fd);
					}
					else 
					{
						Database::FieldDescriptor fd;
						fd.name = fields;
						fd.index = Database::it_none;
						schemaVec.push_back(fd);
					}
					numFields++;
				}
				specifySchema(schemaVec);
				continue;
			}

			Tokenizer row(w, ",");
			size_t dataCount = 0;     //Checks to confirm data matches the schema
			string q;
			vector<string> rowVec;
			while(row.getNextToken(q))
			{
				rowVec.push_back(q);
				dataCount++;
			}
			if (dataCount != numFields)
			{
				clearAll();
				return false;
			}
			addRow(rowVec);
		}
		return true;
	}
	else return false;
}

int Database::getNumRows() const
{
	return m_rows.size();
}

bool Database::getRow(int rowNum, vector<string>& row) const
{
	if (rowNum < 0 || rowNum >= m_rows.size())
		return false;
	row.clear();
	for (int i = 0; i < m_rows[rowNum].size(); i++)
		row.push_back(m_rows[rowNum][i]);
	return true;
}

bool Database::findSchemaIndex(const string& field, int& elem)
{
	for (int i = 0; i < m_schema.size(); i++)
		if (field == m_schema[i].name)
		{
			elem = i;
			return true;
		}
	return false;
}

int Database::search(const std::vector<SearchCriterion>& searchCriteria, const std::vector<SortCriterion>& sortCriteria, std::vector<int>& results)
{
	results.clear();
	if (searchCriteria.size() == 0) //No search criteria passed
		return ERROR_RESULT;

	unordered_set<int> searchedRows;
	for(int i = 0; i < searchCriteria.size(); i++)
	{
		int elem;
		if (findSchemaIndex(searchCriteria[i].fieldName, elem) && m_schema[elem].index == Database::it_indexed) //If we can find this indexed fieldname
		{
			//3 valid cases: Both min and max have a value, min value doesn't have a value, OR max value doesn't have a value.
			if (searchCriteria[i].minValue == "" && searchCriteria[i].maxValue == "") //Check for simple invalid syntax
				return ERROR_RESULT;
			else if(searchCriteria[i].minValue == "") //Min value is empty, return everything previous of maxValue
			{
				if(i == 0) //On the first iteration, grab everything that fits the first criteria and slap it in the set
				{		
					MultiMap::Iterator itr = m_fieldIndex[elem]->findEqualOrPredecessor(searchCriteria[i].maxValue);
					while(itr.valid())
					{
						searchedRows.insert(itr.getValue());
						itr.prev();
					}
				}
				else //Now that we have everything, we need to remove things from the set that don't also match the other criteria.
				{	
					unordered_set<int>::iterator itr = searchedRows.begin();
					while(itr != searchedRows.end())
					{
						if (m_rows[*itr][elem] > searchCriteria[i].maxValue)
							itr = searchedRows.erase(itr);
						else itr++;
					}
				}
			}
			else if(searchCriteria[i].maxValue == "") //Max value is empty, return everything next of minValue
			{
				if(i == 0) //On the first iteration, grab everything that fits the first criteria and slap it in the set
				{		
					MultiMap::Iterator itr = m_fieldIndex[elem]->findEqualOrSuccessor(searchCriteria[i].minValue);
					while(itr.valid())
					{
						searchedRows.insert(itr.getValue());
						itr.next();
					}
				}
				else //Now that we have everything, we need to remove things from the set that don't also match the other criteria.
				{
					unordered_set<int>::iterator itr = searchedRows.begin();
					while(itr != searchedRows.end())
					{
						if (m_rows[*itr][elem] < searchCriteria[i].minValue)
							itr = searchedRows.erase(itr);
						else itr++;
					}
				}
			}
			else 									  //Both min and max 
			{
				if(i == 0) //On the first iteration, grab everything that fits the first criteria and slap it in the set
				{		
					MultiMap::Iterator itr = m_fieldIndex[elem]->findEqualOrSuccessor(searchCriteria[i].minValue);
					while(itr.valid() && itr.getKey() <= searchCriteria[i].maxValue)
					{
						searchedRows.insert(itr.getValue());
						itr.next();
					}
				}
				else //Now that we have everything, we need to remove things from the set that don't also match the other criteria.
				{
					unordered_set<int>::iterator itr = searchedRows.begin();
					while(itr != searchedRows.end())
					{
						if (m_rows[*itr][elem] < searchCriteria[i].minValue || m_rows[*itr][elem] > searchCriteria[i].maxValue)
							itr = searchedRows.erase(itr);
						else itr++;
					}
					cout << searchedRows.size() << endl;
				}
			}
		}
		else
			return ERROR_RESULT;
	}
	
	unordered_set<int>::iterator itr = searchedRows.begin(); //Found all proper rows, now push them into results
	size_t numSearched = 0;
	while(itr != searchedRows.end())
	{
		results.push_back(*itr);
		itr++;
		numSearched++;
	}
	sort(sortCriteria, results);
	return numSearched;
}	

bool Database::leftBeforeRight(const int& left, const int& right, const vector<SortCriterion>& sortCriteria)
{
	int schema_it;
	for (int i = 0; i < sortCriteria.size(); i++)
	{
		if(findSchemaIndex(sortCriteria[i].fieldName, schema_it))
		{
			if(sortCriteria[i].ordering == ot_ascending)
			{
				if(m_rows[left][schema_it] < m_rows[right][schema_it])
					return true;
				else if (m_rows[left][schema_it] > m_rows[right][schema_it])
					return false;
			}
			else  //Descending order
			{
				if(m_rows[left][schema_it] > m_rows[right][schema_it])
					return true;
				else if (m_rows[left][schema_it] < m_rows[right][schema_it])
					return false;
			}
		}
		else return false;
	}
	return true; //They are the same if they make it this far
}

vector<int> Database::merge(const vector<int>& left, const vector<int>& right, const std::vector<Database::SortCriterion>& sortCriteria)
{
	vector<int> mergeResult;
	unsigned int leftEl = 0, rightEl = 0, sortCritIt = 0;
	while(leftEl < left.size() && rightEl < right.size())	
	{
		if(leftBeforeRight(left[leftEl], right[rightEl], sortCriteria))
		{
			mergeResult.push_back(left[leftEl]);
			leftEl++;
		}
		else
		{
			mergeResult.push_back(right[rightEl]);
			rightEl++;
		}
	}

	while (leftEl < left.size())
	{
		mergeResult.push_back(left[leftEl]);
		leftEl++;
	}
	while (rightEl < right.size())
	{
		mergeResult.push_back(right[rightEl]);
		rightEl++;
	}
	return mergeResult;
}

void Database::sort(const vector<SortCriterion>& sortCriteria, vector<int>& results)
{
	if (results.size() == 1)
		return;
	vector<int>::iterator middle = results.begin() + results.size() / 2;   //Gets the middle of the vector

	vector<int> left(results.begin(), middle);
	vector<int> right(middle, results.end());

	sort(sortCriteria, left);
	sort(sortCriteria, right);
	results = merge(left, right, sortCriteria);
}
