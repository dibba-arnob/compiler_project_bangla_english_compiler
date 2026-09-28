#ifndef VISUALIZER_CPP
#define VISUALIZER_CPP

#include <iostream>
#include <string>
#include "node.cpp"
using namespace std;

// puro parse tree ta indented vabe print kore, shob node type handle kore:
void tree(Node* node, int level = 0) {
    string pre = "";
    for (int i = 0; i < level; i++) pre += "---";

    if (node == nullptr) {
        cout << pre << "(null)" << endl;
        return;
    }

    IntNode* asInt = dynamic_cast<IntNode*>(node);
    IdNode* asId = dynamic_cast<IdNode*>(node);
    BinOpNode* asBinOp = dynamic_cast<BinOpNode*>(node);
    DecNode* asDec = dynamic_cast<DecNode*>(node);       // DecNode age check korte hobe, AssignNode er age
    AssignNode* asAssign = dynamic_cast<AssignNode*>(node);
    PrtNode* asPrt = dynamic_cast<PrtNode*>(node);
    IfNode* asIf = dynamic_cast<IfNode*>(node);
    WhileNode* asWhile = dynamic_cast<WhileNode*>(node);
    ProgNode* asProg = dynamic_cast<ProgNode*>(node);

    if (asInt != nullptr) {
        cout << pre << asInt->toString() << endl;
    }
    else if (asId != nullptr) {
        cout << pre << asId->toString() << endl;
    }
    else if (asBinOp != nullptr) {
        cout << pre << asBinOp->op << endl;
        tree(asBinOp->left, level + 1);
        tree(asBinOp->right, level + 1);
    }
    else if (asDec != nullptr) {
        cout << pre << "declare(" << asDec->t << " " << asDec->id << ")" << endl;
        if (asDec->val != nullptr) {
            tree(asDec->val, level + 1);
        }
    }
    else if (asAssign != nullptr) {
        cout << pre << "assign(" << asAssign->id << ")" << endl;
        tree(asAssign->val, level + 1);
    }
    else if (asPrt != nullptr) {
        cout << pre << "print" << endl;
        tree(asPrt->expr, level + 1);
    }
    else if (asIf != nullptr) {
        cout << pre << "if" << endl;
        tree(asIf->cond, level + 1);
        cout << pre << "then:" << endl;
        for (int i = 0; i < (int)asIf->thenStmts.size(); i++) {
            tree(asIf->thenStmts[i], level + 1);
        }
        if (asIf->hasElse) {
            cout << pre << "else:" << endl;
            for (int i = 0; i < (int)asIf->elseStmts.size(); i++) {
                tree(asIf->elseStmts[i], level + 1);
            }
        }
    }
    else if (asWhile != nullptr) {
        cout << pre << "while" << endl;
        tree(asWhile->cond, level + 1);
        cout << pre << "do:" << endl;
        for (int i = 0; i < (int)asWhile->stmts.size(); i++) {
            tree(asWhile->stmts[i], level + 1);
        }
    }
    else if (asProg != nullptr) {
        cout << pre << "program" << endl;
        for (int i = 0; i < (int)asProg->stmts.size(); i++) {
            tree(asProg->stmts[i], level + 1);
        }
    }
    else {
        cout << pre << "Unknown Node: " << node->toString() << endl;
    }
}

#endif
