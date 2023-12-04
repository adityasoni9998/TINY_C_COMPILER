#ifndef _TRANS_H
#define _TRANS_H

//#include <bits/stdc++.h>         // for using C++ items

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <list>
#include <functional>
#include <iomanip>
#include <string.h>
#include <stack>
#include <fstream>
using namespace std;

extern char* yytext;
extern int yyparse();

// different types of classes required
class expression;
class statement;
class Array;

class sym_type;
class sym;
class sym_table;



class label;    
class quad;

class activation_record;

extern sym *curr_sym;                  // current symbol
extern string curr_type;                // current type
extern vector<quad *> quads;       // array of quads being updated dynamically      
extern int table_count;                 // total number of symbol tables generated till now
extern int temp_count;                  // total number of temporary variables generated till now
extern sym_table * currST;              // pointer to current symbol table
extern sym_table * gblST;               // pointer to global symbol table
extern FILE* yyin;
extern vector<string> stringliterals;

// class symbol type
class sym_type{
    public:
        string type;                    // basic data type
        int width;                      // width
        sym_type *array_type;            // array type (if at all)

        sym_type(string _type, sym_type * _array_type = NULL, int _width = 1);       // constructor
};

// class symbol (each entry of symbol table)
class sym{
    public:
        string name;                    // name of symbol
        sym_type *type;                // pointer to an object of its type
        int size;                       // size occupied
        int offset;                     // offset in table
        sym_table* nested_symtab;       // pointer to nested symbol table (if any)
        string val;                     // value
        // bool is_function;               // check if its a function
        string category;
        sym(string _name, string t = "int" ,string _val = "");
        sym* update(sym_type*);         // update type of existing symbol
        sym* convert(string new_type);  // convert current type of this symbol to new type
};

// class symbol table
class sym_table{
    public:
        string name;            // title of symbol table
        /*check data structure*/
        unordered_map <string,sym> symbols;      // using a hash table for storing the symbols
        sym_table *parent;     // pointer to parent symbol table

        sym_table(string _name = "NULL", sym_table * _parent = NULL);        // constructor

        sym* lookup(string);    // lookup (search)
        void update();          // update   

        activation_record *activationRecord;
        vector <string> parameter_list;    
};

// class Label
class label{
    public:
        string name;                // title of label
        int loc;                    // the location the label is pointing to (index of some quad)
        list <int> nextlist;         // list of dangling goto statements dependent on this label

        label(string _name, int _loc = -1);     // constructor
};

// class Quadruple
class quad{
    public:
        string result,arg1,op,arg2;
        quad(string, string, string = "=", string = "");
};

// class Expression
class expression{
    public: 
        sym *loc;          // pointer to symbol table entry
        string type;        // type of expression - can be boolean OR non-boolean
        list <int> truelist,falselist,nextlist;      // list of dangling true/false exits (for boolean expressions) and next exits(for non-boolean expressions)

        // use default constructor since all values are later populated manually via production rules

        void conv2int();        // convert the expression from its current type to boolean type (to use bool value arithmetically)
        void conv2bool();       // convert the expression from boolean type to integer type (to use arithmetic expressions for decision making)
};

// class Array
class Array{
    public:
        string type; //check
        sym* addr;       // temporary to calculate the offset  
        sym* loc;        // pointer to entry in symbol table
        sym_type* sub_array_type;       // type of subarray generated by this array
};  

// class Statement
class statement{
    public:
        list<int> nextlist;     // list of dangling gotos depending on S.next
};

class activation_record{
    public:
        int net_displacement;
        map <string,int> disp_map;
        activation_record();
};

// emit functions (one for each quad)
void emit(string, string, string = "", string = "");
void emit(string, string, int, string = "");
// void emit(string, string, float, string = "");
void backpatch(list<int>&,int);              // backpatch each goto in the list with the address of the provided index of quad (supplied as int)
void new_backpatch();
list<int> makelist(int);                    // make a list with only one quad index
list<int> merge(list<int>,list<int>);       // merge 2 lists

sym *gentemp(string, string init_value = "");      // generate a temporary variable given its type and initial value (optional)
int next_instr();                       // return next free instruction slot (index of quad)
void change_table(sym_table *);             // change table from current to the given one

/*check parameter types*/
bool check_type(sym *&, sym *&);            // check if the 2 types supplied are identical
bool compare_types(sym_type *,sym_type *);

int get_size(sym_type *);
string format_type(sym_type *);
void print(sym_table *);
void print(quad *);

#endif

