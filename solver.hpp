#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <array>
#include <fstream>
#include <sstream>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <list>

using namespace std;

struct Word{
    string word;

    Word(string x = "     "){
        if (x.size() != 5){
            // cerr << "Invalid word size: "+x+"\n"; //user defined literal operator to iterate through each char
            throw runtime_error("==== Invalid word size: "+x+" ====\n");
        }
        for (char &c : x){
            if (!isalpha(c)) throw runtime_error("==== Input needs to be all alphabetical ====");
        }
        word = x;
    }
};//END word struct



//beautify with these functions;
void suggest(vector<int> &resultant, vector<string> &deleted, vector<Word> &thelist){
    cout << "THIS IS ONE SUGGESTION: ";

    for (int r : resultant){
        bool brek = false;
        if (deleted.empty()) break;
        //iterates over the deleted ones so that they don't print as a suggestion
        vector<string>::iterator w = find(deleted.begin(), deleted.end(), thelist[r].word);
        if (w != deleted.end()){
            continue;
        }
        cout << thelist[r].word;
        break;
    }cout << endl;
}

void showguesses(vector<Word> &guesses){
    cout << "YOUR GUESSES:\n------------\n";
    for (Word &w : guesses){
        for (char &c : w.word){
            cout << c << ' ';
        }cout << endl;
    }cout << endl;
}