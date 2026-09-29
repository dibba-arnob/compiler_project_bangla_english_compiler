#ifndef NODE_CPP
#define NODE_CPP

#include <string>
#include <vector>
using namespace std;

// base node (shob node er base class):
class Node {
public:
    virtual string toString() {
        return "";
    }
    virtual ~Node() {}
};

// define node: E --> T + T|T - T; T --> F * F|F / F ; F --> num | id | (E)
class IntNode : public Node {
public:
    string value;

    IntNode(string value) {
        this->value = value;
    }

    string toString() override {
        return value;
    }
};

class IdNode : public Node {
public:
    string value;

    IdNode(string value) {
        this->value = value;
    }

    string toString() override {
        return value;
    }
};

class BinOpNode : public Node {
public:
    Node* left;
    string op;
    Node* right;

    BinOpNode(Node* left, string op, Node* right) {
        this->left = left;
        this->op = op;
        this->right = right;
    }

    string toString() override {
        return "(" + left->toString() + " " + op + " " + right->toString() + ")";
    }
};

// assignment: a : 10
class AssignNode : public Node {
public:
    string id;
    Node* val;

    AssignNode(string id, Node* val) {
        this->id = id;
        this->val = val;
    }

    string toString() override {
        return "assign(" + id + " : " + val->toString() + ")";
    }
};

// declaration: integer a : 10
class DecNode : public AssignNode {
public:
    string t;   // suppose to be 'integer'

    DecNode(string t, string id, Node* val = nullptr) : AssignNode(id, val) {
        this->t = t;
    }

    string toString() {
        if (val != nullptr) {
            return "declare(" + t + " " + id + " : " + val->toString() + ")";
        } else {
            return "declare(" + t + " " + id + ")";
        }
    }
};

// print: prt a*c
class PrtNode : public Node {
public:
    Node* expr;

    PrtNode(Node* expr) {
        this->expr = expr;
    }

    string toString() override {
        return "print(" + expr->toString() + ")";
    }
};

// notun node (new nodes): if / while
class IfNode : public Node {
public:
    Node* cond;
    vector<Node*> thenStmts;
    vector<Node*> elseStmts;
    bool hasElse;

    IfNode(Node* cond, vector<Node*> thenStmts, vector<Node*> elseStmts, bool hasElse) {
        this->cond = cond;
        this->thenStmts = thenStmts;
        this->elseStmts = elseStmts;
        this->hasElse = hasElse;
    }

    string toString() override {
        string result = "if(" + cond->toString() + "){\n";
        for (int i = 0; i < (int)thenStmts.size(); i++) {
            result += "  " + thenStmts[i]->toString() + "\n";
        }
        result += "}";

        if (hasElse) {
            result += " else {\n";
            for (int i = 0; i < (int)elseStmts.size(); i++) {
                result += "  " + elseStmts[i]->toString() + "\n";
            }
            result += "}";
        }
        return result;
    }
};

class WhileNode : public Node {
public:
    Node* cond;
    vector<Node*> stmts;

    WhileNode(Node* cond, vector<Node*> stmts) {
        this->cond = cond;
        this->stmts = stmts;
    }

    string toString() override {
        string result = "while(" + cond->toString() + "){\n";
        for (int i = 0; i < (int)stmts.size(); i++) {
            result += "  " + stmts[i]->toString() + "\n";
        }
        result += "}";
        return result;
    }
};

// program:
class ProgNode : public Node {
public:
    vector<Node*> stmts;

    ProgNode(vector<Node*> stmts) {
        this->stmts = stmts;
    }

    string toString() override {
        string result = "program{\n";
        for (int i = 0; i < (int)stmts.size(); i++) {
            result += "  " + stmts[i]->toString() + "\n";
        }
        result += "}";
        return result;
    }
};

#endif
