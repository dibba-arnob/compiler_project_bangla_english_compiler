#ifndef LEXER_CPP
#define LEXER_CPP

#include <string>
#include <vector>
#include "token.cpp"
using namespace std;

// lexer:
class Lexer {
public:
    string code;
    int pos;
    int line;
    vector<Token> tokens;

    Lexer(string code) {
        // BOM (EF BB BF) thakle shuru theke sore felo:
        if (code.size() >= 3 &&
            (unsigned char)code[0] == 0xEF &&
            (unsigned char)code[1] == 0xBB &&
            (unsigned char)code[2] == 0xBF) {
            code = code.substr(3);
        }
        this->code = code;
        pos = 0;
        line = 0;
    }

    // ekta full utf-8 character return kore (1 theke 4 byte hote pare)
    // bangla okkhor 3 byte er hoy, tai ekbar e puro character ta nite hobe
    string get_char() {
        if (pos >= (int)code.length()) {
            return "";   // "" mane None (kono char nai)
        }
        unsigned char first = code[pos];
        int len = 1;
        if ((first & 0x80) == 0x00) len = 1;        // ASCII (0xxxxxxx)
        else if ((first & 0xE0) == 0xC0) len = 2;    // 2 byte utf-8
        else if ((first & 0xF0) == 0xE0) len = 3;    // 3 byte utf-8 (bangla eikhane pore)
        else if ((first & 0xF8) == 0xF0) len = 4;    // 4 byte utf-8

        if (pos + len > (int)code.length()) len = 1; // safety

        return code.substr(pos, len);
    }

    // c ekta okkhor (letter) kina check kore, bangla ba english dutoi
    bool is_letter(string c) {
        if (c.length() == 0) return false;
        if (c.length() == 1) return isalpha((unsigned char)c[0]) != 0;
        return true; // multi-byte char mane bangla/onno script er okkhor
    }

    // c ekta digit (0-9) kina check kore
    bool is_digit_char(string c) {
        if (c.length() == 1) return isdigit((unsigned char)c[0]) != 0;
        return false;
    }

    void advance() {
        string c = get_char();
        if (c == "\n") {
            line += 1;
        }
        pos += c.length();
    }

    string tokenize(string &error) {
        error = "";
        while (true) {
            string c = get_char();

            if (c == "") {
                break;
            }
            else if (c == " " || c == "\t" || c == "\n" || c == "\r") {
                advance();
            }
            else if (is_digit_char(c)) {
                string num = "";
                while (c != "" && is_digit_char(c)) {
                    num += c;
                    advance();
                    c = get_char();
                }
                tokens.push_back(Token(int_lit, num, line));
            }
            else if (is_letter(c)) {
                string words = "";
                while (c != "" && (is_letter(c) || is_digit_char(c) || c == "_")) {
                    words += c;
                    advance();
                    c = get_char();
                }

                // english keyword:
                if (words == "integer") {
                    tokens.push_back(Token(int_, "", line));
                }
                else if (words == "ptr") {
                    tokens.push_back(Token(ptr, "", line));
                }
                else if (words == "if") {
                    tokens.push_back(Token(if_, "", line));
                }
                else if (words == "else") {
                    tokens.push_back(Token(else_, "", line));
                }
                else if (words == "while") {
                    tokens.push_back(Token(while_, "", line));
                }
                // bangla keyword:
                else if (words == "পূর্ণ") {
                    tokens.push_back(Token(int_, "", line));
                }
                else if (words == "লিখ") {
                    tokens.push_back(Token(ptr, "", line));
                }
                else if (words == "যদি") {
                    tokens.push_back(Token(if_, "", line));
                }
                else if (words == "নাহলে") {
                    tokens.push_back(Token(else_, "", line));
                }
                else if (words == "যতক্ষণ") {
                    tokens.push_back(Token(while_, "", line));
                }
                else {
                    tokens.push_back(Token(id_, words, line));
                }
            }
            else if (c == ":") {
                tokens.push_back(Token(assign, c, line));
                advance();
            }
            else if (c == "+") {
                tokens.push_back(Token(pls, c, line));
                advance();
            }
            else if (c == "-") {
                tokens.push_back(Token(mns, c, line));
                advance();
            }
            else if (c == "*") {
                tokens.push_back(Token(mt, c, line));
                advance();
            }
            else if (c == "/") {
                tokens.push_back(Token(div_, c, line));
                advance();
            }
            else if (c == "(") {
                tokens.push_back(Token(lpr, c, line));
                advance();
            }
            else if (c == ")") {
                tokens.push_back(Token(rpr, c, line));
                advance();
            }
            else if (c == "{") {
                tokens.push_back(Token(lbr, c, line));
                advance();
            }
            else if (c == "}") {
                tokens.push_back(Token(rbr, c, line));
                advance();
            }
            else if (c == "=") {
                tokens.push_back(Token(eq, c, line));
                advance();
            }
            else if (c == "<") {
                tokens.push_back(Token(lt, c, line));
                advance();
            }
            else if (c == ">") {
                tokens.push_back(Token(gt, c, line));
                advance();
            }
            else {
                error = "LexerError: Illegal Char : '" + c + "' at line " + to_string(line);
                break;
            }
        }

        tokens.push_back(Token(eof_, "", line));
        return error;
    }
};

#endif