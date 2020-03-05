#include <sstream>

using namespace std;

class Utils {
    public:
        static string encryptScore(int _in);
        static int decryptScore(string _in);
        static string getSaltKey();
}; 
