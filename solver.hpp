#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <array>
#include <fstream>
#include <sstream>
#include <map>

#include <unordered_set>
#include <list>

using namespace std;

void firstsuggestions(){
    //these words are found online
    cout << "THESE ARE RECOMMENDED START WORDS:\n" <<
        "SOARE | ROATE | RAISE | CRANE | SLATE\n" <<
        "SEARE | STARE | SLANT | TRACE | ROAST\n" <<
        "ADIEU | SNARE | RAISE | AESIR | MONKEYU (jk)\n" <<
        "but of course, you are free to choose whatever you like !\n\n"; 
}//END first suggestions

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
void suggest(unordered_set<int> &resultant, vector<string> &deleted, vector<Word> &thelist){
    // cout << "THIS IS IN SUGGEST FUNCTION" << endl;
    cout << "THIS IS RESULTANT: ";
    for (auto &i : resultant){
        cout << thelist[i].word << ' ';
    }cout << endl;

    // cout << "IN DELETED" << endl;
    // for (string &s : deleted){
    //     cout << s << endl;
    // }cout << endl;

    cout << "THIS IS ONE SUGGESTION: ";

    for (int r : resultant){
        bool brek = false;
        if (deleted.empty()) break;
        //iterates over the deleted ones so that they don't print as a suggestion
        vector<string>::iterator w = find(deleted.begin(), deleted.end(), thelist[r].word);
        if (w != deleted.end()){
            cout << "DELETED: " << *w << endl;
            resultant.erase(r);
        }
    }

    for (int r : resultant){
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