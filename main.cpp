#include <iostream>
#include <fstream>
#include <sstream>
#include <windows.h>
#include "token.cpp"
#include "lexer.cpp"
#include "node.cpp"
#include "parser.cpp"
#include "visualizer.cpp"
#include "symtable.cpp"
#include "semantic.cpp"
#include "tac.cpp"
using namespace std;

string readFile(string path, bool &ok) {
    ifstream file(path, ios::binary);
    if (!file.is_open()) {
        ok = false;
        cout << "Tried to open: " << path << endl;
        return "";
    }
    stringstream buffer;
    buffer << file.rdbuf();
    ok = true;
    return buffer.str();
}

// AST ghure ghure DecNode/AssignNode gula symbol table e bosay:
void buildSymbols(Node* node, SymbolTable &table) {
    if (node == nullptr) return;

    DecNode* asDec = dynamic_cast<DecNode*>(node);       // DecNode age check korte hobe, AssignNode er age
    AssignNode* asAssign = dynamic_cast<AssignNode*>(node);
    IfNode* asIf = dynamic_cast<IfNode*>(node);
    WhileNode* asWhile = dynamic_cast<WhileNode*>(node);
    ProgNode* asProg = dynamic_cast<ProgNode*>(node);

    if (asDec != nullptr) {
        string err;
        string val = (asDec->val != nullptr) ? asDec->val->toString() : "";
        bool ok = table.define(asDec->id, asDec->t, val, err);
        if (!ok) {
            cout << "SymbolError: " << err << endl;
        }
    }
    else if (asAssign != nullptr) {
        string err;
        string val = (asAssign->val != nullptr) ? asAssign->val->toString() : "";
        bool ok = table.update(asAssign->id, val, err);
        if (!ok) {
            cout << "SymbolError: " << err << endl;
        }
    }
    else if (asIf != nullptr) {
        for (int i = 0; i < (int)asIf->thenStmts.size(); i++) {
            buildSymbols(asIf->thenStmts[i], table);
        }
        for (int i = 0; i < (int)asIf->elseStmts.size(); i++) {
            buildSymbols(asIf->elseStmts[i], table);
        }
    }
    else if (asWhile != nullptr) {
        for (int i = 0; i < (int)asWhile->stmts.size(); i++) {
            buildSymbols(asWhile->stmts[i], table);
        }
    }
    else if (asProg != nullptr) {
        for (int i = 0; i < (int)asProg->stmts.size(); i++) {
            buildSymbols(asProg->stmts[i], table);
        }
    }
    // PrtNode/IntNode/IdNode/BinOpNode -> kono symbol toiri hoy na, tai kichu korar nai
}

int main(int argc, char* argv[]) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    string path = (argc > 1) ? argv[1] : "code.txt";
    bool fileOk = false;
    string code = readFile(path, fileOk);

    if (!fileOk) {
        return 1;
    }

    Lexer l(code);
    string e;
    l.tokenize(e);

    if (e != "") {
        cout << e << endl;
        return 1;
    }

    cout << "----- TOKENS -----" << endl;
    for (int i = 0; i < (int)l.tokens.size(); i++) {
        cout << l.tokens[i].toString() << endl;
    }
    cout << "------------------" << endl;

    Parser p(l.tokens);
    Node* ast = p.parse();

    if (p.error != "") {
        cout << p.error << endl;
        return 1;
    }

    cout << "----- PARSE TREE (string) -----" << endl;
    cout << ast->toString() << endl;

    cout << "----- PARSE TREE (diagram) -----" << endl;
    tree(ast);

    cout << "----- SYMBOL TABLE -----" << endl;
    SymbolTable table;
    buildSymbols(ast, table);
    table.printSymbols();

    cout << "----- SEMANTIC ANALYSIS -----" << endl;
    ProgNode* prog = dynamic_cast<ProgNode*>(ast);
    if (prog != nullptr) {
        SemanticAnalyzer analyzer;
        analyzer.analyze(prog);

        if (analyzer.errors.empty()) {
            cout << "No semantic errors found." << endl;
        } else {
            for (int i = 0; i < (int)analyzer.errors.size(); i++) {
                cout << "SemanticError: " << analyzer.errors[i] << endl;
            }
        }

        cout << "----- SYMBOL TABLE (from semantic analyzer) -----" << endl;
        analyzer.symtab.printSymbols();

        cout << "----- THREE ADDRESS CODE -----" << endl;
        if (analyzer.errors.empty()) {
            TACGenerator tacGen;
            tacGen.gen_stmt(prog);
            tacGen.printCode();
        } else {
            cout << "Skipped: fix semantic errors first." << endl;
        }
    }

    return 0;
}