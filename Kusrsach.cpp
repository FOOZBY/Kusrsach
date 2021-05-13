#include "func.h"

int main()
{
    setlocale(0, "");
    SetConsoleCP(1251); // Ввод с консоли в кодировке 1251
    //SetConsoleOutputCP(1251);
    string pascal_programm;
    bool chose;
    chose = menu();
    if (chose)//1-file,0-ruchkami
        pascal_programm=reading_from_file();
    else
        pascal_programm=manualy();
    change_to_c_plus_plus(pascal_programm);
    //cout << program_to_main(pascal_programm);
    //cout << endl << var(pascal_programm);
    system("pause");
}
