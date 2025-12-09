#include "binarysearch.h"
#include <iostream>
#include <string>
#include <fstream>
#include <exception>
#include <algorithm>
#include <cstring>
using namespace std;

extern int smart_compare(std::string& str_a, std::string& str_b);

void Dataset::print_index(int col_num)
{
    cout << "Sorted by column " << col_num << ":" << endl;
    cout << "-------------------" << endl;
    vector< vector<string>*> index = this->indices[col_num];
    for (size_t i = 0; i < index.size(); i++)
    {
        vector<string>& row = *index[i];
        for (size_t j = 0; j < row.size(); j++)
        {
            if (j > 0)
                cout << ", ";
            cout << row[j];
        }
        cout << endl;
    }
}

void Dataset::load_csv(std::string& filename)
{
    // Clear any existing data
    this->data.clear();

    // Open the file
    std::ifstream stream;
    stream.open(filename);
    if (!stream.is_open()) {
        throw std::system_error(errno, std::generic_category(), filename);
    }

    // Read the file
    while (!stream.eof())
    {
        // Read a line
        string s;
        if (!std::getline(stream, s))
            break;
        vector<string> empty;
        this->data.push_back(empty);
        vector<string>& current_row = this->data.back();

        // Break up the line into cells
        while (true)
        {
            // Find the next comma
            size_t pos = s.find(",");
            if (pos == std::string::npos)
            {
                // This is the last cell in the row
                current_row.push_back(s);
                break;
            }
            else
            {
                // This is not the last cell in the row
                current_row.push_back(s.substr(0, pos));
                s.erase(0, pos + 1); // erase the cell and the comma
            }
        }

        // Ensure all rows have the same size
        if (current_row.size() < 1)
        {
            // This row is empty, so just drop it
            this->data.pop_back();
        }
        else if (current_row.size() != this->data[0].size())
        {
            // Uh oh, the row is the wrong size!
            string s = "Error, row " + this->data.size();
            s += " has " + current_row.size();
            s += " elements. Expected " + this->data[0].size();
            throw std::runtime_error(s);
        }
    }

    // Move the first row into this->col_names
    this->col_names = this->data[0];
    this->data.erase(this->data.begin());
}


int Dataset::row_count()
{
    return this->data.size();
}


int Dataset::col_count()
{
    if (row_count() == 0){
        return 0;
    }
    return this->data[0].size();
}


vector<string>& Dataset::get_row(unsigned int i)
{
    if (i >= 0 && i < data.size()){
       return this->data[i];
    }
    else{
        throw runtime_error("Row i is out of range");
    }
}
 
int active_column = 0;

bool custom_less(vector<string>* row1, vector<string>* row2)
{

    int col = active_column;

    string& val1 = (*row1)[col];
    string& val2 = (*row2)[col];

    int compared_val = smart_compare(val1, val2);

    if (compared_val < 0){
        return true;
    }
    else{
        return false;
    }

}



void Dataset::index_data()
{
    // Index the data
    indices.clear();
    indices.resize(this->col_count());
    for (int i = 0; i < this->col_count(); i++)
    {
        // Build an index for column i
        vector< vector<string>* >& index = indices[i];
        for (int j = 0; j < this->row_count(); j++)
            index.push_back(&this->data[j]);

        // Sort the index in column i using smart_compare
        active_column = i;
        std::sort(index.begin(), index.end(), custom_less);
    }
}


int Dataset::binary_search(int col_num, string& target)
{

    int floor = 0;
    int ceiling = this->row_count() - 1;
    int row_num;

    while(true){
        row_num = (floor + ceiling) / 2;

        if (row_num == floor){
            break;
        }

        int cmp = smart_compare((*indices[col_num][row_num])[col_num], target);

        if (cmp < 0){
            floor = row_num;
        }
        else{
            ceiling = row_num;
        }
    }

    while (row_num > 0 && smart_compare((*indices[col_num][row_num - 1])[col_num], target) >= 0){
        row_num--;
    }
    while (row_num < this->row_count() && smart_compare((*indices[col_num][row_num])[col_num], target) < 0){
        row_num++;
    }

    return row_num;

}


void Dataset::query(string start, string end, int column)
{

    if (column < 0 || column >= this->col_count()){
        throw runtime_error("Column # out of range");
    }

    int start_indx = this->binary_search(column, start);

    while (column < this->row_count()){
        vector<string>& curr_row = *indices[column][start_indx];
        string value = curr_row[column];

        if (smart_compare(value, end) >= 0){
            break;
        }

        for (size_t i = 0; i < curr_row.size(); i++){
            if (i > 0){
                cout << ",";
            }
            cout << curr_row[i];
        }
        cout << endl;

        start_indx++;
    }

}
