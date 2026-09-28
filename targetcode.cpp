#ifndef TARGETCODE_CPP
#define TARGETCODE_CPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdio>
#include "token.cpp"
#include "tac.cpp"
using namespace std;

// TAC er operator token ke asol python operator e convert kore:
string opToPython(string op) {
    if (op == pls) return "+";
    if (op == mns) return "-";
    if (op == mt) return "*";
    if (op == div_) return "/";
    if (op == eq) return "==";
    if (op == lt) return "<";
    if (op == gt) return ">";
    return op; // na mile jeta ache seta e rekhe dao
}

// target code: tac gula theke runnable python code banay
class TargetCode {
public:
    vector<TAC*> instructions;
    string target;

    TargetCode(vector<TAC*> instructions) {
        this->instructions = instructions;
    }

    string generate() {
        vector<string> lines;

        for (int i = 0; i < (int)instructions.size(); i++) {
            TACPrint* asPrint = dynamic_cast<TACPrint*>(instructions[i]);
            TACBinOp* asBinOp = dynamic_cast<TACBinOp*>(instructions[i]);
            TACCopy* asCopy = dynamic_cast<TACCopy*>(instructions[i]);

            if (asPrint != nullptr) {
                // print b ---> print(b) in python
                lines.push_back("print(" + asPrint->src + ")");
            }
            else if (asBinOp != nullptr) {
                // t1 = t2 + t3 ---> same as python, but operator convert kore niti hobe
                string pyOp = opToPython(asBinOp->op);
                lines.push_back(asBinOp->dest + " = " + asBinOp->left + " " + pyOp + " " + asBinOp->right);
            }
            else if (asCopy != nullptr) {
                // a = t0 ---> same as python
                lines.push_back(asCopy->dest + " = " + asCopy->src);
            }
            else {
                lines.push_back(instructions[i]->toString());
            }
        }

        string result = "";
        for (int i = 0; i < (int)lines.size(); i++) {
            result += lines[i];
            if (i != (int)lines.size() - 1) {
                result += "\n";
            }
        }
        return result;
    }

    void save(string file = "output.py") {
        string py = generate();
        ofstream f(file, ios::binary); // binary mode, jate bangla variable names thik thake
        f << py << "\n";
        f.close();
    }

    void run() {
        // python3 output.py chalay ebong output console e dekhay
        FILE* pipe = popen("python3 output.py 2>&1", "r");
        if (pipe == nullptr) {
            cout << "Error: could not run python3" << endl;
            return;
        }

        char buffer[256];
        string result = "";
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            result += buffer;
        }

        int status = pclose(pipe);
        cout << result;
        cout << "Exit code: " << status << endl;
    }
};

#endif
