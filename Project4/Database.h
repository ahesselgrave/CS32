#ifndef DATABASE_INCLUDED
#define DATABASE_INCLUDED

#include <string>
#include <vector>
#include <unordered_set>
#include "Tokenizer.h"
#include "http.h"
#include "MultiMap.h"

class Database
{
public:
	enum IndexType { it_none, it_indexed };
	enum OrderingType { ot_ascending, ot_descending };

	struct FieldDescriptor
	{
		std::string name;
		IndexType index;
	};

	struct SearchCriterion
	{
		std::string fieldName;
		std::string minValue;
		std::string maxValue;
	};

	struct SortCriterion
	{
		std::string fieldName;
		OrderingType ordering;
	};

	static const int ERROR_RESULT = -1;

	Database();
	~Database();
	bool specifySchema(const std::vector<FieldDescriptor>& schema);
	bool addRow(const std::vector<std::string>& rowOfData);
	bool loadFromURL(std::string url);
	bool loadFromFile(std::string filename);
	int getNumRows() const;
	bool getRow(int rowNum, std::vector<std::string>& row) const;
	int search(const std::vector<SearchCriterion>& searchCriteria,
			   const std::vector<SortCriterion>& sortCriteria,
			   std::vector<int>& results);

private:
	Database(const Database& other);
	Database& operator=(const Database& rhs);
	void clearAll();
	void sort(const std::vector<SortCriterion>& sortCriteria, std::vector<int>& results);
	bool findSchemaIndex(const std::string& field, int& elem);
	bool leftBeforeRight(const int& left, const int& right, const std::vector<SortCriterion>& sortCriteria);
	std::vector<int> merge(const std::vector<int>& left, const std::vector<int>& right, const std::vector<Database::SortCriterion>& sortCriteria);
	std::vector<FieldDescriptor> m_schema;
	std::vector<MultiMap*> m_fieldIndex;
	std::vector<std::vector<std::string> >m_rows;
};

#endif