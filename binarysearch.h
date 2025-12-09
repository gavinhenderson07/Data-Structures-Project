#ifndef binarysearch
#define binarysearch

#include <string>
#include <vector>
#include <exception>

class Dataset
{
protected:
    //col             name
    std::vector<std::string> col_names;
    //matrix     row           cell
    std::vector<std::vector<std::string>> data;
    //indexs        col-idx        pointer-to-row
    std::vector<std::vector<std::vector<std::string>*>> indices;

public: 

    void load_csv(std::string& filename);
    int row_count();
    int col_count();
    std::vector<std::string>& get_row(unsigned int i);
    void index_data();
    void print_index(int col_num);
    int binary_search(int col_num, std::string& target);
    void query(std::string start, std::string end, int column);
};











#endif