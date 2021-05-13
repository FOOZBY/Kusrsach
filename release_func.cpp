#include "func.h"



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

string program_to_main(string & pascal_program)//done
{
    int i = pascal_program.find("Var");
    pascal_program.erase(0, i);
    return "#include <iostream>\n#include <string>\nusing namespace std;\nint main()\n{\n";
}//done
string var(string& pascal_program)//done
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
    return var_cpp;
}//done
string write_or_writeln(string& pascal_program, bool type)//done
{
    pascal_program.erase(0, 5);
    string content="", temp;
    int i = 0;
    while (pascal_program[i] != '\n')
    {
        if (pascal_program[i] == '(')
        {
            if (pascal_program[i+1] == 39)
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
        return "cout << " + content + ";\n";
    else
        return "cout << " + content + ";";
}//done
string readln(string& pascal_program)
{
    pascal_program.erase(0, 7);
    string content = "";
    int i = 0;
    while (pascal_program[i]!=')')
    {
        content += pascal_program[i];
        i++;
    }
    pascal_program.erase(0, pascal_program.find("\n"));
    return "cin >> " + content + ";\n";
}
string operations(string& pascal_program)
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
    return content + "\n";
}
string end(string& pascal_program)
{
    pascal_program.erase(0, pascal_program.length());

    return "return 0;\n}";
}

string change_to_c_plus_plus(string &pascal_program)
{
    int length = pascal_program.length();
    string keyword = "";
    bool program_check, var_check, write_check, writeln_check, readln_check, operation_check, end_check;
    int i = 0;
    while (i != length)
    {
        if (keyword == "Program")
        {
            cout << program_to_main(pascal_program);
            program_check = 1;
            keyword.clear();
            i = 0;
        }
        if (keyword == "Var")
        {
            cout << var(pascal_program);
            var_check = 1;
            keyword.clear();
            i = 0;
        }
        if (keyword == "Begin")
        {
            keyword.clear();
            i = 0;
        }
        if (keyword == "Write")//0-write,1-writeln
        {
            if (pascal_program[i] == 'l')
            {
                cout << write_or_writeln(pascal_program, 1);
                writeln_check = 1;
                keyword.clear();
                i = 0;
            }
            else
            {
                cout << write_or_writeln(pascal_program, 0);
                write_check = 1;
                keyword.clear();
                i = 0;
            }
        }
        if (keyword == "Readln")
        {
            cout << readln(pascal_program);
            readln_check = 1;
            keyword.clear();
            i = 0;
        }
        if (keyword == "End.")
        {
            cout << end(pascal_program);
            end_check = 1;
            keyword.clear();
            i = 0;
            break;
        }
        if (keyword.find_first_of("+-*/:=") != string::npos)
        {
            cout << operations(pascal_program);
            operation_check = 1;
            keyword.clear();
            i = 0;
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
    return "";
}