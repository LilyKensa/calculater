#include <iostream>
#include <string>
#include <math.h> 

#define ll long long int
#define usll unsigned long long int
#define io ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);

using namespace std;

bool isNumber(char& s) {
    switch (s) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case ' ':
        return true;
    }
    return false;
}

bool isNumber(string& s) {
    for (int i = 0; i < s.length(); ++i) {
        if (!isNumber(s[i])) return false;
    }
    return true;
}

bool isLegal(string& s) {
    for (auto i : s) {
        if (isNumber(i)) continue;
        switch (i) {
        case '(':
        case ')':
        case '+':
        case '-':
        case '*':
        case '/':
        case '^':
        case '%':
        case ' ':
            continue;
        default:
            return false;
        }
    }
    return true;
}

int findCharPos(string& s, char c, int from, int step) {
    // findCharPos("(2 * 3)", ')', 0, 1)
    // 0123456789
    // (2 * 3)
    //       ^
    for (int i = from + step; (i < s.length() && i >= 0); i += step) {
        if (s[i] == c) return i;
    }
    return -1;
}

int findFirstCharFromArray(string& s, char* c) {
    // findFirstCharFromArray("2 * 3 + 5", {'*', '/'})
    for (int i = 0; i < s.length(); ++i) {
        for (int j = 0; j < sizeof c; ++j) {
            if (s[i] == c[j]) return i;
        }
    }
    return -1;
}

int findNumberPos(string& s, int from, int step) {
    for (int i = from; (i < s.length() && i >= 0); i += step) {
        if (isNumber(s[i])) return i;
    }
    return -1;
}

string operate(int l, int r, char action) {
    int operated;

    switch (action) {
    case '+':
        operated = l + r;
        break;
    case '-':
        operated = l - r;
        break;
    case '*':
        operated = l * r;
        break;
    case '/':
        operated = (int)(l / r);
        break;
    case '^':
        operated = (int)pow(l, r);
        break;
    case '%':
        operated = l % r;
        break;
    default:
        return "ERROR INVALID OPERATOR!";
    }
    if (operated >= 0) {
        return to_string(operated);
    }
    return (string)("(" + to_string(operated) + ")");
    // (-13)

}

// negate number
bool isNNBrackets(string s) {
    if (!(s[0] == '(' && s[1] == '-' && s[s.length() - 1] == ')')) {
        return false;
    }
    for (int i = 2; i < s.length() - 1; ++i) {
        if (!isNumber(s[i])) {
            return false;
        }
    }
    return true;
}

bool isNNBrackets(string& content, int leftB) {
    // 1 + (-3 * (-2))
    //     ^
    int rightB = findCharPos(content, ')', leftB, 1);
    if (
        !(content[leftB] == '('
            && content[leftB + 1] == '-')
        ) {
        return false;
    }
    for (int i = leftB + 2; i < rightB; ++i) {
        if (!isNumber(content[i])) {
            return false;
        }
    }
    return true;
}

int scanOperators(string& content, char* symbols) {
    int depth = 0;
    // (-2) - 3
    for (int i = 0; i < content.length(); ++i) {
        if (content[i] == '(') depth += 1;
        if (content[i] == ')') depth -= 1;
        if (depth == 0) {
            for (int j = 0; j < sizeof symbols; ++j) {
                if (content[i] == symbols[j]) return i;
            }
        }
    }
    return -1;
}

int findCorrespondingBracket(string& content, int leftB) {
    int depth = 0;
    for (int i = leftB + 1; i < content.length(); ++i) {
        if (content[i] == '(') depth++;
        if (content[i] == ')') {
            depth--;
            if (depth < 0) {
                return i;
            }
        }
    }
    throw - 1;
}

int findLeftNumber(int& l, string& content, int& pos) {
    for (int i = pos; i >= 0; --i) {
        if (content[i] == ')') {
            int leftB = findCharPos(content, '(', i, -1);
            l = leftB;
            return stoi(content.substr(leftB + 1, i - leftB - 1));
        }
        if (isNumber(content[i])) {
            for (int j = i; j >= -1; --j) {
                l = j + 1;
                if (j == -1) return stoi(content.substr(0, i + 1));
                if (!isNumber(content[j])) return stoi(content.substr(j + 1, i - j));
            }
            return content[i] - '0';
        }
    }
    throw - 1;
}

int findRightNumber(int& r, string& content, int& pos) {
    for (int i = pos; i < content.length(); ++i) {
        if (content[i] == '(') {
            int rightB = findCorrespondingBracket(content, i);
            r = rightB + 1;
            return stoi(content.substr(i + 1, rightB - i - 1));
        }
        if (isNumber(content[i])) {
            for (int j = i; j <= content.length(); ++j) {
                r = j;
                if (j == content.length()) return stoi(content.substr(i, j - i));
                if (!isNumber(content[j])) return stoi(content.substr(i, j - i));
            }
            return content[i] - '0';
        }
    }
    throw - 1;
}

char secondaryOperators[2] = { '+', '-' };
char primaryOperators[4] = { '*', '/', '^', '%' };
string calculate(string content) {
    // 2 * 3 - (-2) O
    // 2* (3 * 4) X
    cout << "- calculating " << content << "...\n";
    int pos;

    while (scanOperators(content, primaryOperators) != -1) {
        pos = scanOperators(content, primaryOperators);

        cout << "-- " << content << "\n";
        cout << "-- ";
        for (int i = 0; i < content.length(); ++i) (i == pos) ? cout << "^" : cout << " ";
        cout << "\n";

        int l, r;
        string operated = operate(findLeftNumber(l, content, pos), findRightNumber(r, content, pos), content[pos]);

        content = content.replace(l, r - l, operated);
    }

    while (scanOperators(content, secondaryOperators) != -1) {
        if (
                (
                    isNNBrackets(content, 0) 
                    && findCorrespondingBracket(content, 0) == content.length()
                ) 
                || content[0] == '-' // PROBLEM HERE!
            ) 
            break;
        pos = scanOperators(content, secondaryOperators);

        cout << "-- " << content << "\n";
        cout << "-- ";
        for (int i = 0; i < content.length(); ++i) (i == pos) ? cout << "^" : cout << " ";
        cout << "\n";

        int l, r;
        string operated = operate(findLeftNumber(l, content, pos), findRightNumber(r, content, pos), content[pos]);
        content = content.replace(l, r - l, operated);
    }

    return content;
}

int main() {
    cout
        << "000         00                         00\n"
        << "0000        00                         00\n"
        << "00 00       00                         00\n"
        << "00  00      00                 00      00\n"
        << "00   00     00                 00      00\n"
        << "00    00    00     0000    0000000000  00  00000\n"
        << "00     00   00   00    00      00      0000    00\n"
        << "00      00  00  00      00     00      000      00\n"
        << "00       00 00  00      00     00      00       00\n"
        << "00        0000   00    000     00      00       00\n"
        << "00         000     00000 00    00      00        000\n\n";

    string content = "";
    while (true) {
        cout << "> ";
        getline(cin, content);
        if (content == "exit") break;

        for (int i = 0; i < content.length(); ++i) {
            // 12345
            // 12455
            // 1245
            if (content[i] == ' ') {
                content = content.replace(
                    i,
                    content.length() - i - 1,
                    content.substr(
                        i + 1,
                        content.length() - i - 1
                    )
                );
                content.pop_back();
            }
        }

        int maxDepth = 0;
        for (int i = 0; i < content.length(); ++i) {
            if (content[i] == '(' && !isNNBrackets(content, i)) maxDepth += 1;
        }

        int depth = 0;
        int leftB = -1, rightB = -1;
        while (maxDepth > 0) {
            depth = 0;
            for (int i = 0; i < content.length(); ++i) {
                if (content[i] == '(' && !isNNBrackets(content, i)) {
                    depth += 1;
                    if (depth == maxDepth) {
                        leftB = i;
                        maxDepth -= 1;
                        break;
                    }
                }
            }
            rightB = findCorrespondingBracket(content, leftB);

            cout << "- found the innermost brackets, depth: " << depth << "\n";
            cout << "-- " << content << "\n";
            cout << "-- ";
            for (int i = 0; i < content.length(); ++i) 
                (i == leftB || i == rightB) ? cout << "^" : cout << " ";
            cout << "\n";

            string afterCalculated = calculate(
                content.substr(leftB + 1, rightB - leftB - 1)
            );
            content = content.replace(leftB, rightB + 1 - leftB, afterCalculated);
        }
        string afterCalculated = calculate(content);
        content = afterCalculated;
        cout << "- Answer: " << content << "\n\n";
    }
}