#include <iostream>
#include "config.h"
#include "charmatrix.h"
#include "LinkedListNode.h"
#include "binarysearch.h"
#include "priorityqueue.h"
#include "hash_table.h"
#include <vector>
using std::cout;
using std::cin;
using std::string;
using std::vector;
using std::runtime_error;
using std::endl;
#include <unistd.h>



vector<string> g_lex;

CharMatrix g_grid;
CharMatrix used;
HashTable g_hash_table;

extern int comparisons;

void test(){

    string word0 = "alpha";
    string word = "beta";
    string word1 = "charlie";
    string word2 = "delta";
    string word3 = "echo";
    string word4 = "foxtrot";
    
    LinkedList list1;
    LinkedList list2;

    list1.push_back(word2);
    list1.push_back(word0);
    list1.push_back(word3);
    list2.push_back(word);
    list2.push_back(word1);
    list2.push_back(word4);

    list1.print();
    list2.print();

    list1.merge(list2);

    list1.print();

    list1.sort();


    cout << endl;
    list2.print();
    cout << endl;
    cout << list1.size() << endl;
    cout << endl;
    list1.print();

}

void merge_sort(){

    LinkedList merge_sort;

    while(g_lex.size() > 0){
        merge_sort.push_back(g_lex.back());
        g_lex.pop_back();
    }

    merge_sort.print();
    merge_sort.sort();
    merge_sort.print();

    while(merge_sort.size() > 0){
        g_lex.push_back(merge_sort.pop_front());
    }
    comparisons += 1;
    cout << "comparisons:" << comparisons << endl;
}

// Count from 0 to n-1
void count(int n)
{
    for (int i = 0; i < n; i++)
    {
        cout << i << " ah-ah-ah!" << endl;
    }
}

void fill_lexicon(){

while (true)
{
    if (!config.quiet){
    for (int i = 0; i < 5; i++)
    {
        cout << endl;
    }
    cout << "Please enter another word" << endl;
    cout << "> ";
    }
    string word;
    getline(cin, word);
    if (word.size() == 0)
    {
        break;
    }
    if(word.compare("-") == 0)
    {
        g_lex.pop_back();
    }
    else{
    g_lex.push_back(word);
    }
    if (!config.quiet){
    cout << "So far, the words you have entered are:" << endl;
    for (unsigned int i = 0; i < g_lex.size(); i++)
    {
        cout << i << ". " << g_lex[i] << endl;
    }
    }
}
}

void load_char_matrix()
{
    if (!config.quiet)
    {
        cout << "Please enter a grid of characters." << endl;
        cout << "All rows should have the same length." << endl;
        cout << "When you are done, just press Enter." << endl;
    }
    vector<string> grid;
    while(true)
    {
        string row;
        getline(cin, row);
        if (row.compare("") == 0)
            break;
        if (grid.size() > 0 && row.size() != grid[0].size())
            throw runtime_error("Rows in a CharMatrix must all have the same size!");
        grid.push_back(row);
    }
    int height = grid.size();
    int width = 0;
    if (height > 0)
        width = grid[0].size();
    g_grid.resize(width, height);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            g_grid.put(x, y, grid[y][x]);
        }
    }
}

void print_char_matrix(){
    for (int y = 0; y < g_grid.height(); y++)
    {
        for (int x = 0; x < g_grid.width(); x++)
        {
            cout << g_grid.get(x, y);
        }
        cout << endl;
    }
}

void charfill(int x, int y, char c, int max_depth){
    if (config.debug)
    {
    usleep(100000);
    for (int i = 0; i < 20; i++)
        cout << endl;
    print_char_matrix();
    cout.flush();
    }

    char targ = g_grid.get(x, y);

    if (targ == c){
        return;
    }
    
    g_grid.put(x, y, c);

    if (x > 0 && g_grid.get(x - 1, y) == targ)
        charfill(x - 1, y, c, max_depth - 1);

    if (x < g_grid.width() - 1 && g_grid.get(x + 1, y) == targ)
        charfill(x + 1, y, c, max_depth - 1);

    if (y > 0 && g_grid.get(x, y - 1) == targ)
        charfill(x, y - 1, c, max_depth - 1);

    if (y < g_grid.height() - 1 && g_grid.get(x, y + 1) == targ)
        charfill(x, y + 1, c, max_depth - 1);



}

void flood_fill(){
    string word;
    cout << "Please enter a starting column:" << endl;
    cout << "> ";
    getline(cin, word);
    int x = stoi(word);
    cout << "Please enter a starting row:" << endl;
    cout << "> ";
    getline(cin, word);
    int y = stoi(word);
    cout << "Please enter a fill character:" << endl;
    cout << "> ";
    getline(cin, word);
    int c = word[0];
    cout << "Please enter the max fill depth:" << endl;
    cout << "> ";
    getline(cin, word);
    int max_depth = stoi(word);
    charfill(x, y, c, max_depth);
}

bool find_word(string word, int col, int row, unsigned int index){
    if (index == word.length())
        return true;

    if (used.get(col, row) != '.'){
        return false;
    }


    if (g_grid.get(col, row) != word[index]){
        return false;
    }
    for (int i = -1; i <= 1; i++){
        for (int j = -1; j <= 1; j++){
            if (i == 0 && j == 0){
                continue;
            }
            int currrow = row + i;
            int currcol = col + j;

            used.put(col, row, 'x');

            if((currrow >= 0 && currrow < g_grid.height()) && (currcol >= 0 && currcol < g_grid.width())){
                if (find_word(word, currcol, currrow, index + 1)){
                    used.put(col, row, '.');
                    return true;
                }
            }
        }
    }
    used.put(col, row, '.');

    return false;

}

void boggle(){

    bool found = false;


    used.resize(g_grid.width(), g_grid.height());


    for(unsigned int i = 0; i < g_lex.size(); i++){
        string word = g_lex.at(i);

        for (int i = 0; i < used.height(); i++){
            for (int j = 0; j < used.width(); j++){
                    used.put(j, i, '.');
            }
        }

        for (int row = 0; row < g_grid.height(); row++){
            for (int col = 0; col < g_grid.width(); col++){
                if (g_grid.get(col, row) == word[0]){
                    if (find_word(word, col, row, 0)){
                        cout << word << endl;
                        break;
                        found = true;
                    }
                }
            }
            if(found){break;}
        }
    }


}

Dataset g_dataset;
void load_csv()
{
    cout << "Please enter a filename of a .csv file:" << endl;
    cout << "> ";
    string filename;
    getline(std::cin, filename);
    g_dataset.load_csv(filename);
    g_dataset.index_data();
}

void query(){

    // Ask for query parameters
    cout << "Please enter a column index:" << endl;
    cout << "> ";
    string column_str;
    getline(std::cin, column_str);
    int column = stoi(column_str);
    cout << "Please enter a starting value:" << endl;
    cout << "> ";
    string start;
    getline(std::cin, start);
    cout << "Please enter an ending value:" << endl;
    cout << "> ";
    string end;
    getline(std::cin, end);

    // Perform the query
    g_dataset.query(start, end, column); 

}

void priority_test(){

    PriorityQueue queue;

    string val1 = "salmon";
    string val2 = "money";
    string val3 = "tarantula";
    string val4 = "banana";
    string val5 = "zebra";
    string val6 = "antelope";

    queue.insert(val1);
    queue.insert(val2);
    queue.insert(val3);
    queue.insert(val4);
    queue.insert(val5);
    queue.insert(val6);

    

    

    queue.print();
    cout << endl;
    string temp = queue.pop_first();
    cout << temp << endl;
    queue.print();


}

PriorityQueue g_priority_queue;

void insert_into_priority_queue()
{
    cout << "Please enter a string:" << endl;
    cout << "> ";
    string value;
    getline(std::cin, value);
    g_priority_queue.insert(value);
}

void pop_first_from_priority_queue()
{
    string value = g_priority_queue.pop_first();
    cout << value << endl;
}

void insert_into_hash_table()
{
    cout << "Please enter a key:" << endl;
    cout << "> ";
    string key;
    getline(std::cin, key);
    cout << "Please enter a value:" << endl;
    cout << "> ";
    string value;
    getline(std::cin, value);
    g_hash_table.insert(key, value);
}

void query_hash_table()
{
    cout << "Please enter a key:" << endl;
    cout << "> ";
    string key;
    getline(std::cin, key);
    string value = g_hash_table.query(key);
    cout << value << endl;
}

// Entry point
int main(int argc, char** argv)
{

    config.parse_flags(argc, argv);
    log("---Running in debug mode---");

    while (true)
{
    for (int i = 0; i < 5; i++)
    {
        cout << endl;
    }
    cout << "Gavin Henderson's PF2 projects" << endl << endl;
    cout << "Lexicon size: " << g_lex.size() << endl << endl;
    cout << "0. Quit" << endl;
    cout << "1. Fill lexicon" << endl;
    cout << "2. Tear Down Lexicon" << endl;
    cout << "3. Load char matrix" << endl;
    cout << "4. Print char matrix" << endl;
    cout << "5. Flood fill" << endl;
    cout << "6. Boggle" << endl;
    cout << "7. Linked List" << endl;
    cout << "8. Merge Sort" << endl;
    cout << "9. Load CSV file" << endl;
    cout << "10. Query" << endl;
    cout << "11. Test Priority Queue" << endl;
    cout << "12. Insert into priority queue" << endl;
    cout << "13. pop_first from priority queue" << endl;
    cout << "14. Insert into hash table" << endl;
    cout << "15. Query hash table" << endl;
    cout << "> ";
    string option;
    getline(cin, option);
    if (option.compare("0") == 0) {
        break;
    } else if (option.compare("1") == 0) {
        fill_lexicon();
    } else if (option.compare("2") == 0){
       while (g_lex.size() > 0){
        cout << g_lex.back() << endl;
        g_lex.pop_back();
       }
    } else if (option.compare("3") == 0){
        load_char_matrix();
    } else if (option.compare("4") == 0){
        print_char_matrix();
    } else if (option.compare("5") == 0){
        flood_fill();
    }else if(option.compare("6") == 0){
        boggle();
    }else if(option.compare("7") == 0){
        test();
    }else if(option.compare("8") == 0){
        merge_sort();
    }else if(option.compare("9") == 0){
        load_csv();
    }else if(option.compare("10") == 0){
        query();
    }else if (option.compare("11") == 0){
        priority_test();
    }else if (option.compare("12") == 0){
        insert_into_priority_queue();
    }else if (option.compare("13") == 0){
        pop_first_from_priority_queue();
    }else if (option.compare("14") == 0){
        insert_into_hash_table();
    }else if (option.compare("15") == 0){
        query_hash_table();
    }else {
        cout << option << " was not one of the options. Quitting." << endl;
        break;
    }
}



    return 0;
}