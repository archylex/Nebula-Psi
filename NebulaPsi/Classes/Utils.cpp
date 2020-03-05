#include "Utils.h"

#define SALT_KEY 139

string Utils::encryptScore(int _in) {
    string pOut = "";
    stringstream sStream;

    sStream << hex << (SALT_KEY - _in);
    sStream >> pOut;

    return pOut;
}

int Utils::decryptScore(string _in) {
    int intScore = 0;
    stringstream sStream;
    
    sStream << _in;
    sStream >> hex >> intScore;

    return (SALT_KEY - intScore);
}

string Utils::getSaltKey() {
    string out = "";
    out = encryptScore(SALT_KEY);
    return out;
}
