#include "func.h"


HANDLE Con = GetStdHandle(STD_OUTPUT_HANDLE);
bool program_check = 0, var_check = 0, write_check = 0, writeln_check = 0, readln_check = 0, operation_check = 0, end_check = 0;
double getnum()//проверка ввода
{
    double value;
    while (!(cin >> value))
    { //Since value is a double, (cin >> value) will be true only if the user enters a valid value that can be put inside a double
        cout << "Please enter a valid value (use '.' instead ','):" << endl;
        cin.clear();
        cin.ignore(32767, '\n');
    }
    cin.ignore(32767, '\n');
    return value;
}

void show_pascal(string program,bool type)
{
    SetConsoleTextAttribute(Con, 11);
    switch (type)
    {
    case 0:
        for (int i = 0; i < program.length(); i++)
            cout << program[i];
        break;
    default:
        for (int i = 0; i < program.length(); i++)
        {
            cout << program[i];
            if (program[i] == ';')
                cout << endl;
        }
        break;
    }
    SetConsoleTextAttribute(Con, 7);
}

bool menu()
{
    cout << "Выберите метод записи исходной программы: " << endl;
    cout << "0. Вручную" << endl;
    cout << "1. Из файла" << endl;
    bool chose = getnum();
    return chose ? 1 : 0;
}

string reading_from_file()
{
    cout << "Введите название файла, если он в директории программы, иначе введите путь к файлу: ";
    string prim_file, pascal_program;
    while (true)
    {
        getline(cin, prim_file);
        ifstream in;
        in.open(prim_file); // окрываем файл для чтения
        if (in.is_open())
        {
            getline(in, pascal_program, '\0');
            cout << endl << "Изначальная программа на Pascal: " << endl;
            show_pascal(pascal_program,0);
            cout << endl << endl;
            return pascal_program;
            break;
        }
        else
        {
            cout << "Файл не существует в указанном месте." << endl;
            cout << "Укажите имя/путь файла корректно" << endl;
            cin.clear();

        }
        cin.seekg(0, ios::end);
        cin.ignore(32767, '\n');
        cin.clear();
        in.close();
    }    // закрываем файл
}
string manualy()
{
    string  pascal_program;
    getline(cin, pascal_program);
    show_pascal(pascal_program,1);
    return pascal_program;
}
string program_to_main(string & pascal_program, bool change)//done
{
    string ret;
    if (change)
    {
        int i = pascal_program.find("Var");
        pascal_program.erase(0, i);
        program_check = 1;
        return "#include <iostream>\n#include <string>\nusing namespace std;\nint main()\n{\n";
    }
    else 
    {
        int i = pascal_program.find("Var");
        ret = pascal_program.substr(0, i);
        pascal_program.erase(0, i);
        return ret;
    }
        
}//done
string var(string& pascal_program, bool change)//done
{
    if (change)
    {
        pascal_program.erase(0, 4);
        string var = "", vars_part_pascal = "", var_cpp = "", temp = "";//имя переменной
        int i = 0;
        while (var.find("Begin") == string::npos)
        {
            if (var.find("Integer;") != string::npos)//нашёл Integer;
            {
                temp = var.substr(0, var.find(":"));
                var_cpp += "int ";
                var_cpp += temp;
                var_cpp += ";\n";
                var = "";
            }
            else if (var.find("Real;") != string::npos)//нашёл Real;
            {
                temp = var.substr(0, var.find(":"));
                var_cpp += "float ";
                var_cpp += temp;
                var_cpp += ";\n";
                var = "";
            }
            else if (var.find("Boolean;") != string::npos)//нашёл Boolean;
            {
                temp = var.substr(0, var.find(":"));
                var_cpp += "bool ";
                var_cpp += temp;
                var_cpp += ";\n";
                var = "";
            }
            else if (var.find("String;") != string::npos)//нашёл String;
            {
                temp = var.substr(0, var.find(":"));
                var_cpp += "string ";
                var_cpp += temp;
                var_cpp += ";\n";
                var = "";
            }
            else if (var.find("Char;") != string::npos)//нашёл Char;
            {
                temp = var.substr(0, var.find(":"));
                var_cpp += "char ";
                var_cpp += temp;
                var_cpp += ";\n";
                var = "";
            }
            var += pascal_program[i];
            i++;
            if (var == "\n")
                var = "";
        }
        pascal_program.erase(0, i);
        var_check = 1;
        return var_cpp;
    }
    else 
    {
        int i = pascal_program.find("Begin");
        string ret = pascal_program.substr(0, i);
        pascal_program.erase(0, i+5);
        return ret;
    }
}//done
string write_or_writeln(string& pascal_program, bool type, bool change)//done
{
    if (change)
    {
        pascal_program.erase(0, 5);
        string content = "", temp;
        int i = 0;
        while (pascal_program[i] != '\n')
        {
            if (pascal_program[i] == '(')
            {
                if (pascal_program[i + 1] == 39)
                {
                    i++;
                    content += '"';
                    while (temp != "'")
                    {
                        i++;
                        content += pascal_program[i];
                        temp = pascal_program[i + 1];
                    }
                    content += '"';
                }
                else while (temp != ")")
                {
                    i++;
                    content += pascal_program[i];
                    temp = pascal_program[i + 1];
                }
            }
            i++;
        }
        pascal_program.erase(0, pascal_program.find("\n"));
        if (type)
        {
            writeln_check = 1;
            return "cout << " + content + " << endl;\n";
        }
        else
        {
            write_check = 1;
            return "cout << " + content + ";\n";
        }
    }
    else
    {
        int i = pascal_program.find("\n");
        string ret = pascal_program.substr(0, i+1);
        pascal_program.erase(0, pascal_program.find("\n"));
        return ret;
    }
}//done
string readln(string& pascal_program, bool change)
{
    if (change)
    {
        pascal_program.erase(0, 7);
        string content = "";
        int i = 0;
        while (pascal_program[i] != ')')
        {
            content += pascal_program[i];
            i++;
        }
        pascal_program.erase(0, pascal_program.find("\n"));
        readln_check = 1;
        return "cin >> " + content + ";\n";
    }
    else
    {
        int i = pascal_program.find("\n");
        string ret = pascal_program.substr(0, i+1);
        pascal_program.erase(0, pascal_program.find("\n"));
        return ret;
    }
}
string operations(string& pascal_program, bool change)
{
    if (change)
    {
        string content = "";
        int i = 0;
        while (pascal_program[i] != '\n')
        {
            content += pascal_program[i];
            i++;
        }
        content.erase(content.find(":"), 1);
        pascal_program.erase(0, pascal_program.find("\n"));
        operation_check = 1;
        return content + "\n";
    }
    else
    {
        int i = pascal_program.find("\n");
        string ret = pascal_program.substr(0, i+1);
        pascal_program.erase(0, i);
        return ret;
    }
}
string end(string& pascal_program, bool change)
{
    if (change)
    {
        pascal_program.erase(0, pascal_program.length());
        end_check = 1;
        return "return 0;\n}";
    }
    else
    {
        return pascal_program;
    }
}

//string change_to_c_plus_plus(string &pascal_program, string chosed_changes)
//{
//    SetConsoleTextAttribute(Con, 11);
//    int length = pascal_program.length();
//    string keyword = "";
//    bool program_check = 0, var_check = 0, write_check = 0, writeln_check = 0, readln_check = 0, operation_check = 0;
//    int i = 0;
//    while (i != length)
//    {
//        if (keyword == "Program")
//        {
//            cout << program_to_main(pascal_program);
//            program_check = 1;
//            keyword.clear();
//            i = 0;
//        }
//        if (keyword == "Var")
//        {
//            cout << var(pascal_program);
//            var_check = 1;
//            keyword.clear();
//            i = 0;
//        }
//        if (keyword == "Write")//0-write,1-writeln
//        {
//            if (pascal_program[i] == 'l')
//            {
//                cout << write_or_writeln(pascal_program, 1);
//                writeln_check = 1;
//                keyword.clear();
//                i = 0;
//            }
//            else
//            {
//                cout << write_or_writeln(pascal_program, 0);
//                write_check = 1;
//                keyword.clear();
//                i = 0;
//            }
//        }
//        if (keyword == "Readln")
//        {
//            cout << readln(pascal_program);
//            readln_check = 1;
//            keyword.clear();
//            i = 0;
//        }
//        if (keyword == "End.")
//        {
//            cout << end(pascal_program);
//            if (program_check)
//            {
//                SetConsoleTextAttribute(Con, 1);
//                cout << endl << endl << "Было использовано преобразование Program" << endl;
//            }
//            if (var_check)
//            {
//                SetConsoleTextAttribute(Con, 2);
//                cout << "Было использовано преобразование Var" << endl;
//            }
//            if (write_check)
//            {
//                SetConsoleTextAttribute(Con, 3);
//                cout << "Было использовано преобразование Write" << endl;
//            }
//            if (writeln_check)
//            {
//                SetConsoleTextAttribute(Con, 4);
//                cout << "Было использовано преобразование Writeln" << endl;
//            }
//            if (readln_check)
//            {
//                SetConsoleTextAttribute(Con, 5);
//                cout << "Было использовано преобразование Readln" << endl;
//            }
//            if (operation_check)
//            {
//                SetConsoleTextAttribute(Con, 6);
//                cout << "Было использовано преобразование Operation" << endl;
//            }
//            SetConsoleTextAttribute(Con, 8);
//            cout << "Было использовано преобразование End" << endl;
//            SetConsoleTextAttribute(Con, 7);
//            break;
//        }
//        if (keyword.find_first_of("+-*/:=") != string::npos)
//        {
//            cout << operations(pascal_program);
//            operation_check = 1;
//            keyword.clear();
//            i = 0;
//        }
//        keyword += pascal_program[i];
//        if (keyword == "\n")
//        {
//            keyword.clear();
//            pascal_program.erase(0, 1);
//            i--;
//        }
//
//        i++;
//    }
//    return "";
//}

void change_to_c_plus_plus(string &pascal_program, string chosed_changes)
{
    if (chosed_changes == "8")
        chosed_changes = "1234567";
    SetConsoleTextAttribute(Con, 11);
    int length = pascal_program.length();
    string keyword = "", final_program = "";

    int i = 0;
    while (i != length)
    {
        if (keyword == "Program")
        {
            final_program += program_to_main(pascal_program, chosed_changes.find("1") != string::npos);
            keyword.clear();
            i = 0;
        }
        if (keyword == "Var")
        {
            final_program += var(pascal_program, chosed_changes.find("2") != string::npos);
            keyword.clear();
            i = 0;
        }
        if (keyword == "Write")//0-write,1-writeln
        {
            if (pascal_program[i] == 'l')
            {
                final_program += write_or_writeln(pascal_program, 1, chosed_changes.find("4") != string::npos);//writeln-4
                keyword.clear();
                i = 0;
            }
            else
            {
                final_program += write_or_writeln(pascal_program, 0, chosed_changes.find("3") != string::npos);//write-3
                keyword.clear();
                i = 0;
            }
        }
        if (keyword == "Readln")
        {
            final_program += readln(pascal_program, chosed_changes.find("5") != string::npos);
            keyword.clear();
            i = 0;
        }
        if (keyword.find_first_of("+-*/:=") != string::npos)
        {
            final_program += operations(pascal_program, chosed_changes.find("6") != string::npos);
            keyword.clear();
            i = 0;
        }
        if (keyword == "End.")
        {
            final_program += end(pascal_program, chosed_changes.find("7") != string::npos);
            break;
        }
        keyword += pascal_program[i];
        if (keyword == "\n")
        {
            keyword.clear();
            pascal_program.erase(0, 1);
            i--;
        }

        i++;
    }
    SetConsoleTextAttribute(Con, 11);
    cout << final_program<< endl<<endl;
    SetConsoleTextAttribute(Con, 7);
    if (program_check)
    {
        SetConsoleTextAttribute(Con, 1);
        cout << endl << endl << "Было использовано преобразование Program" << endl;
    }
    if (var_check)
    {
        SetConsoleTextAttribute(Con, 2);
        cout << "Было использовано преобразование Var" << endl;
    }
    if (write_check)
    {
        SetConsoleTextAttribute(Con, 3);
        cout << "Было использовано преобразование Write" << endl;
    }
    if (writeln_check)
    {
        SetConsoleTextAttribute(Con, 4);
        cout << "Было использовано преобразование Writeln" << endl;
    }
    if (readln_check)
    {
        SetConsoleTextAttribute(Con, 5);
        cout << "Было использовано преобразование Readln" << endl;
    }
    if (operation_check)
    {
        SetConsoleTextAttribute(Con, 6);
        cout << "Было использовано преобразование Operation" << endl;
    }
    if (end_check)
    {
        SetConsoleTextAttribute(Con, 8);
        cout << "Было использовано преобразование End" << endl << endl << endl;
    }
    SetConsoleTextAttribute(Con, 7);
}
string possible_changes()
{
    char quotes = '"';
    string chosed_changes;
    SetConsoleTextAttribute(Con, 1); cout << "1. Program Program_Name;\n";
    SetConsoleTextAttribute(Con, 7); cout << "преобразуется в\n";
    SetConsoleTextAttribute(Con, 1); cout << "#include <iostream>\n#include <string>\nusing namespace std;\nint main()\n{\n" << endl;
    SetConsoleTextAttribute(Con, 2); cout << "2. Var\n X: integer;\n";
    SetConsoleTextAttribute(Con, 7); cout << "преобразуется в\n";
    SetConsoleTextAttribute(Con, 2); cout << "int X; \n" << endl;
    SetConsoleTextAttribute(Con, 3); cout << "3. Write('текст');\n";
    SetConsoleTextAttribute(Con, 7); cout << "преобразуется в\n";
    SetConsoleTextAttribute(Con, 3); cout << "cout << " << quotes << "текст" << quotes << "; \n" << endl;
    SetConsoleTextAttribute(Con, 4); cout << "4. Writeln('текст');\n";
    SetConsoleTextAttribute(Con, 7); cout << "преобразуется в\n";
    SetConsoleTextAttribute(Con, 4); cout << "cout << " << quotes << "текст" << quotes << " << endl; \n" << endl;
    SetConsoleTextAttribute(Con, 5); cout << "5. Readln(X);\n";
    SetConsoleTextAttribute(Con, 7); cout << "преобразуется в\n";
    SetConsoleTextAttribute(Con, 5); cout << "cin >> X; \n" << endl;
    SetConsoleTextAttribute(Con, 6); cout << "6. a := b + c;\n";
    SetConsoleTextAttribute(Con, 7); cout << "преобразуется в\n";
    SetConsoleTextAttribute(Con, 6); cout << "a = b + c; \n" << endl;
    SetConsoleTextAttribute(Con, 8); cout << "7. End.\n";
    SetConsoleTextAttribute(Con, 7); cout << "преобразуется в\n";
    SetConsoleTextAttribute(Con, 8); cout << "return 0\n}\n" << endl;
    SetConsoleTextAttribute(Con, 13); cout << "8. Всё выше перечисленное.\n\n";
    SetConsoleTextAttribute(Con, 7);
    cout << "Введите номера изменений,которые хотите использовать в одну строку (например: 134, будут использованы изменения под номером 1, 3 и 4): ";
    cin >> chosed_changes;
    sort(chosed_changes.begin(), chosed_changes.end());
    for (int i = 0; i < chosed_changes.length(); i++)
    {
        if (chosed_changes[i]== chosed_changes[i+1])
        {
            chosed_changes.erase(i, 1);
            i--;
        }
    }
    return chosed_changes;
}