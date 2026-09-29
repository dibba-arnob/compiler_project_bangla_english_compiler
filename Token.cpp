#ifndef TOKEN_CPP
#define TOKEN_CPP

#include <string>
using namespace std;

// token gulo (tokens):
string int_ = "পূর্ণসংখ্যা";
string id_ = "পরিচয়";
string assign = "নির্ধারণ";
string int_lit = "পূর্ণসংখ্যা_মান";
string ptr = "মুদ্রণ";
string pls = "যোগ";
string mns = "বিয়োগ";
string mt = "গুণ";
string div_ = "ভাগ";
string lpr = "বাম_বন্ধনী";
string rpr = "ডান_বন্ধনী";
string eof_ = "শেষ";

// notun token (new tokens) for if/else/while:
string if_ = "যদি";          // if
string else_ = "নাহলে";       // else
string while_ = "যতক্ষণ";     // while
string lbr = "বাম_কোষ্ঠক";    // {
string rbr = "ডান_কোষ্ঠক";    // }
string eq = "সমান";           // == (single '=' for simplicity)
string lt = "ছোট";            //
string gt = "বড়";             // >

// TOKEN CLASS:
class Token {
public:
    string type_;
    string value;
    int line;   // -1 mane kono line nai

    Token(string type_, string value, int line) {
        this->type_ = type_;
        this->value = value;
        this->line = line;
    }

    string toString() {
        if (value != "") {
            return "< " + type_ + " , " + value + " >";
        } else {
            return "< " + type_ + " >";
        }
    }
};

#endif