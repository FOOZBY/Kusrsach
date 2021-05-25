#include "func.h"

int main()
{
    setlocale(0,"");
    string pascal_programm;
    string chosed_changes;
    bool chose;
    chosed_changes = possible_changes();
    chose = menu();
    if (chose)//1-file,0-ruchkami
        pascal_programm=reading_from_file();
    else
        pascal_programm=manualy();
    cout << "Программа на C++" << endl << endl;
    change_to_c_plus_plus(pascal_programm, chosed_changes);
    cout << endl;
    system("pause");
}