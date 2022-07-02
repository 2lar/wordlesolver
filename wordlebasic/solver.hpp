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
            throw runtime_error("==== Invalid word size: "+x+" ====\n");
        }
        for (char &c : x){
            if (!isalpha(c)) throw runtime_error("==== Input needs to be all alphabetical ====");
        }
        word = x;
    }
};//END word struct

//beautify with these functions;
void suggest(unordered_set<int> &resultant, vector<string> &deleted, vector<Word> &thelist, unordered_map<char, int> &frequencies){

    map<string, int> scores;
    pair<string, int> highscore = {"", 0};
    for (auto &i : resultant){
        int ascore = 0;
        for (char &c : thelist[i].word){
            ascore += frequencies[c];
        }
        scores[thelist[i].word] = ascore;
        if (ascore > highscore.second){
            highscore.first = thelist[i].word;
            highscore.second = ascore;
        }
    }
        
    cout << "\nYour list of candidate answers: ";
    for (auto &i : resultant){
        cout << thelist[i].word << ' ';
    }cout << endl;

    cout << "\nTHIS IS ONE SUGGESTION (first in list): ";

    for (int r : resultant){
        bool brek = false;
        if (deleted.empty()) break;
        //iterates over the deleted ones so that they don't print as a suggestion
        vector<string>::iterator w = find(deleted.begin(), deleted.end(), thelist[r].word);
    }

    for (int r : resultant){
        cout << thelist[r].word;
        break;
    }cout << endl;

    cout << "SUGGESTION WITH HIGHEST SCORE: " << highscore.first << endl;

    //this is for random pick
    vector<int> therand;
    therand.insert(therand.end(), resultant.begin(), resultant.end());

    int random = rand() % therand.size();
    cout << "THIS IS RANDOM SUGGESTION: " << thelist[therand[random]].word << endl;
}

void showguesses(vector<Word> &guesses){
    cout << "\nYOUR GUESSES:\n------------\n";
    for (Word &w : guesses){
        for (char &c : w.word){
            cout << c << ' ';
        }cout << endl;
    }cout << endl;
}
