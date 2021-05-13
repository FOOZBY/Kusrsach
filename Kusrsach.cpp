#include "func.h"

int main()
{
    setlocale(0,"");
    string pascal_programm;
    bool chose;
    chose = menu();
    if (chose)//1-file,0-ruchkami
        pascal_programm=reading_from_file();
    else
        pascal_programm=manualy();
    cout << "Программа на C++" << endl << endl;
    change_to_c_plus_plus(pascal_programm);
    cout << endl;
    system("pause");
}
