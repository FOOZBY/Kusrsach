#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <windows.h>
#include <conio.h>

using namespace std;



double getnum();
void show_pascal(string programm,bool type);
bool menu();
string reading_from_file();
string manualy();

///////////////////////////////////////////////////

string program_to_main(string& pascal_program);
string var(string& pascal_program);
string write_or_writeln(string& pascal_program, bool type);
string readln(string& pascal_program);
string operations(string& pascal_program);
string change_to_c_plus_plus(string &pascal_programm);
