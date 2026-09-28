#ifndef SYMTABLE_CPP
#define SYMTABLE_CPP

#include <iostream>
#include <string>
#include <map>
using namespace std;

// SYMBOL:
class Symbol {
public:
    string name;
    string type_;
    string value;     // "" mane None (kono value nai)
    bool is_init;

    Symbol(string name, string type_, string value = "") {
        this->name = name;
        this->type_ = type_;
        this->value = value;

        if (value != "") {
            this->is_init = true;
        } else {
            this->is_init = false;
        }
    }

    string toString() {
        string st = is_init ? "initialized" : "uninitialized";
        return name + ", " + type_ + ", " + st;
    }
};
// int a---> variable : a; type : integer; initialized? False

// SYMBOL TABLE:
class SymbolTable {
public:
    map<string, Symbol> vars;

    bool define(string name, string type_, string value, string &err_msg) {
        if (vars.count(name) > 0) {
            err_msg = "'" + name + "' is already declared!";
            return false;
        }
        vars.insert({name, Symbol(name, type_, value)});
        err_msg = "";
        return true;
    }

    bool update(string name, string value, string &err_msg) {
        if (vars.count(name) == 0) {
            err_msg = name + " is not declared!";
            return false;
        }
        vars.at(name).value = value;
        vars.at(name).is_init = true;
        err_msg = "";
        return true;
    }

    // symbol ta pointer hisebe fereay, na pele nullptr
    Symbol* lookup(string name) {
        if (vars.count(name) > 0) {
            return &vars.at(name);
        } else {
            return nullptr;
        }
    }

    bool is_defined(string name) {
        return vars.count(name) > 0;
    }

    bool is_initialized(string name) {
        return vars.at(name).is_init;
    }

    void printSymbols() {
        cout << string(62, '-') << endl;
        cout << "var-name\t\tvar-type\t\tis_initialized" << endl;
        cout << string(62, '-') << endl;
        for (auto &pair : vars) {
            Symbol &s = pair.second;
            string j = s.is_init ? "initialized" : "uninitialized";
            cout << s.name << "\t\t" << s.type_ << "\t\t" << j << endl;
        }
    }

    string toString() {
        string result = "Symbol Table:\n" + string(40, '-') + "\n";
        result += "Name         Type       Status        \n";
        result += string(40, '-') + "\n";
        for (auto &pair : vars) {
            Symbol &s = pair.second;
            string status = s.is_init ? "initialized" : "uninitialized";
            result += s.name + "  " + s.type_ + "  " + status + "\n";
        }
        return result;
    }
};

#endif
