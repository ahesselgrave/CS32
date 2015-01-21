// #include "test.h"
#include <iostream>
#include <string>
#include <cassert>
#include "Database.h"
using namespace std;
 
int main(int argc, char *argv[])
{
/*    string filename;

    switch (argc)
    {
      case 1:
        cout << "Enter test script file name: ";
        getline(cin, filename);
        break;
      case 2:
        filename = argv[1];
        break;
      default:
        cout << "Usage: " << argv[0] << " scriptname" << endl;
        return 1;
    }
    
    Test t;
    string problem;

    if (!t.loadAndRunTestScript(filename, problem))
    {
        cout << "Problem running script: " << problem << endl;
        return 1;
    }*/
    Database db;
    cout << "Attempting to load URL/file" << endl;
    if (db.loadFromFile("/home/ahesselgrave/projects/CS32/Project4/census.csv"))
    {
        cout << "URL success! Added " << db.getNumRows() << " rows! Press enter to continue.";
        cin.ignore(1000, '\n');
    }
    vector<Database::SearchCriterion> searchCrit;
    Database::SearchCriterion sd1;
    sd1.fieldName = "LastName";
    sd1.maxValue = "D";
    Database::SearchCriterion sd2;
    sd2.fieldName = "Age";
    sd2.maxValue = "030";
    searchCrit.push_back(sd1);
    searchCrit.push_back(sd2);
    vector<Database::SortCriterion> sortCrit;
    Database::SortCriterion sc1;
    sc1.fieldName = "LastName";
    sc1.ordering = Database::ot_descending;
    Database::SortCriterion sc2;
    sc2.fieldName = "Age";
    sc2.ordering = Database::ot_descending;
    sortCrit.push_back(sc1);
    sortCrit.push_back(sc2);
    vector<int> results;
    int yes = db.search(searchCrit, sortCrit, results);
    cout << yes << endl;
    cin.ignore(100,'\n');
    for (int i = 0; i < results.size(); i++)
    {
        vector<string> rowData;
        if (db.getRow(results[i], rowData))
        {
            for (size_t j = 0; j < rowData.size(); j++)
                cout << rowData[j] << " ";
            cout << endl;
        }
        else
            cout << "Error retrieving row!" << endl;
    }
}