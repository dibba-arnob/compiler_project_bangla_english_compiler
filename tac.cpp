
#ifndef TAC_CPP
#define TAC_CPP

#include <iostream>
#include <string>
#include <vector>
#include "node.cpp"
using namespace std;

// Three-Address Code: Instructions
// 1. integer a = 5 ---> a = 5; --- copy
// 2. d = a+b+c ---> t1 = a+b; t2 = t1 + c; d = t2 ---- binary operation
// 3. ptr d ---> print d ---- printing

// base TAC instruction:
class TAC {
public:
    virtual string toString() {
        return "";
    }
    virtual ~TAC() {}
};

// 1. integer a = 5 ---> a = 5; --- copy
class TACCopy : public TAC {
public:
    string dest;
    string src;

    TACCopy(string dest, string src) {
        this->dest = dest;
        this->src = src;
    }

    string toString() override {
        return dest + " = " + src;
    }
};

// 2. d = a+b+c ---> t1 = a+b; t2 = t1 + c; d = t2 ---- binary operation
class TACBinOp : public TAC {
public:
    string dest;
    string left;
    string op;
    string right;

    TACBinOp(string dest, string left, string op, string right) {
        this->dest = dest;
        this->left = left;
        this->op = op;
        this->right = right;
    }

    string toString() override {
        return dest + " = " + left + " " + op + " " + right;
    }
};

// 3. ptr d ---> print d ---- printing
class TACPrint : public TAC {
public:
    string src;

    TACPrint(string src) {
        this->src = src;
    }

    string toString() override {
        return "print " + src;
    }
};

// TAC generator: AST theke tac instruction gula toiri kore
class TACGenerator {
public:
    vector<TAC*> code;
    int tempCount = 0;

    string newTemp() {
        tempCount += 1;
        return "t" + to_string(tempCount);
    }

    // ekta expr node theke tar "result name" ber kore (variable name, literal, ba notun temp)
    // r ekhon TACBinOp instruction gula code te jog hoy jodi dorkar hoy
    string gen_expr(Node* node) {
        IntNode* asInt = dynamic_cast<IntNode*>(node);
        IdNode* asId = dynamic_cast<IdNode*>(node);
        BinOpNode* asBinOp = dynamic_cast<BinOpNode*>(node);

        if (asInt != nullptr) {
            return asInt->value;
        }
        else if (asId != nullptr) {
            return asId->value;
        }
        else if (asBinOp != nullptr) {
            string leftVal = gen_expr(asBinOp->left);
            string rightVal = gen_expr(asBinOp->right);
            string temp = newTemp();
            code.push_back(new TACBinOp(temp, leftVal, asBinOp->op, rightVal));
            return temp;
        }
        else {
            return "";
        }
    }

    void gen_stmt(Node* node) {
        DecNode* asDec = dynamic_cast<DecNode*>(node);       // DecNode age check, AssignNode er age
        AssignNode* asAssign = dynamic_cast<AssignNode*>(node);
        PrtNode* asPrt = dynamic_cast<PrtNode*>(node);
        IfNode* asIf = dynamic_cast<IfNode*>(node);
        WhileNode* asWhile = dynamic_cast<WhileNode*>(node);
        ProgNode* asProg = dynamic_cast<ProgNode*>(node);

        if (asDec != nullptr) {
            if (asDec->val != nullptr) {
                string val = gen_expr(asDec->val);
                code.push_back(new TACCopy(asDec->id, val));
            }
        }
        else if (asAssign != nullptr) {
            string val = gen_expr(asAssign->val);
            code.push_back(new TACCopy(asAssign->id, val));
        }
        else if (asPrt != nullptr) {
            string val = gen_expr(asPrt->expr);
            code.push_back(new TACPrint(val));
        }
        else if (asIf != nullptr) {
            for (int i = 0; i < (int)asIf->thenStmts.size(); i++) {
                gen_stmt(asIf->thenStmts[i]);
            }
            for (int i = 0; i < (int)asIf->elseStmts.size(); i++) {
                gen_stmt(asIf->elseStmts[i]);
            }
        }
        else if (asWhile != nullptr) {
            for (int i = 0; i < (int)asWhile->stmts.size(); i++) {
                gen_stmt(asWhile->stmts[i]);
            }
        }
        else if (asProg != nullptr) {
            for (int i = 0; i < (int)asProg->stmts.size(); i++) {
                gen_stmt(asProg->stmts[i]);
            }
        }
    }

    void printCode() {
        for (int i = 0; i < (int)code.size(); i++) {
            cout << code[i]->toString() << endl;
        }
    }
};

#endif
