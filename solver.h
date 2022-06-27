#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <array>
#include <fstream>
#include <sstream>
#include <map>
#include <unordered_map>
#include <list>

using namespace std;

struct Word{
    //char array of size 6 because there is null terminated char at end
    //we could use size 5 but it could cause some weird things
    //will need to see whether we can use 5 or not
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

//THINGS TO KEEP TRACK TO GET SOLUTION
const string alphabet = "abcdefghijklmnopqrstuvwxyz";

map<char, map<int, vector<int>>> master;
//this is mapped to every letter. for every letter, records index in which is shows up in word
//and records word in index form within thelist

vector<Word> thelist, guesses;
vector<char> valid, invalid;
vector<string> deleted;

string guessbool = "";


//FUNCTIONS
void thelistmaker(){
    cout << "MAKING THE AND MASTER LIST..." << endl << endl;
    //since there are a lot of words, should reserve the amount of space that we will use.
    thelist.reserve(12973);
    
    //create an input file and read in contents
    ifstream thewords;
    thewords.open("words.txt");

    //add contents to thelist
    string tempword;
    uint32_t counter = 0;
    while (getline(thewords, tempword)){
        thelist.emplace_back(tempword);
        for (int i = 0; i < 5; i++){
            master[tempword[i]][i].push_back(counter);
        }counter++;
    }

    //these words are found online
    cout << "THESE ARE RECOMMENDED START WORDS:\n" <<
        "SOARE | ROATE | RAISE | CRANE | SLATE\n" <<
        "SEARE | STARE | SLANT | TRACE | ROAST\n" <<
        "ADIEU | SNARE | RAISE | AESIR | MONKEYU (jk)\n" <<
        "but of course, you are free to choose whatever you like !\n\n"; 

}//END thelistmaker()

//takes in a list of vectors of idx and find intersection between all lists
//CURRENT: only finds intersections between greens
vector<int> intersections(const list<vector<int>> &ilist){
    unordered_map<int, int> checker; //element to counter
    vector<int> ireturn;
    const int isize = ilist.size();

    for (const auto &sections : ilist)
        for (const auto &wordidx : sections) checker[wordidx] += 1;

    for (const auto &e : checker)
        if (checker[e.first] == isize) ireturn.push_back(e.first);

    return ireturn;
}//END intersections()

void checkguess(Word &word, int attempts){
    //correctness legend:
    // 0 = black || 1 = yellow || 2 = green

    cout << "0: wrong | 1:wrong place | 2:correct: ";
    getline(cin >> ws, guessbool);

    vector<char> yellow; yellow.reserve(5);
    vector<int> initial =  {0,1,2,3,4}; int ideleted = 0;
    list<vector<int>> tointersect;


    for (int i = 0; i < 5; i++){
        if (guessbool[i] == '2'){
            valid.push_back(word.word[i]);

            //remove pos i from initial and insert vector of words from char at pos i into intersector
            tointersect.push_back(master[(word.word[i])][i]);
            vector<int>::iterator initit = find(initial.begin(), initial.end(), i);
            if (initit != initial.end()) initial.erase(initial.begin() + i - ideleted++);

            // delete from masterlist
            
            //
            word.word[i] = toupper(word.word[i]);
        }
        else if(guessbool[i] == '1'){
            valid.push_back(word.word[i]);

            yellow.push_back(word.word[i]);
            //find any pos still in inital but not i and insert into intersector(the vector of words for the intersection list)
        }
        else if (guessbool[i] == '0'){
            invalid.push_back(word.word[i]);
        }
        // else if ((guessbool[i]) > '2' || (guessbool[i]) < '0') throw runtime_error("Invalid character bool input");
        else throw runtime_error("Invalid character bool input");
    }
    
    vector<int> resultant = intersections(tointersect);

    cout << "THIS IS ONE SUGGESTION: ";
    for (int r : resultant){
        bool brek = false;
        if (deleted.empty()) break;
        vector<string>::iterator w = find(deleted.begin(), deleted.end(), thelist[r].word);
        if (w != deleted.end()){
            continue;
        }
        cout << thelist[r].word;
        break;
    }cout << endl;

    //THIS PART DOES NOT NEED TO BE INCLUDED. ONLY FOR VISUALIZATION
    if (!yellow.empty()){
        cout << "THESE LETTERS ARE IN WRONG PLACE (inorder): ";
        for (char &c : yellow) cout << c << ' ';
        cout << endl;
    }
}//END checkguess


string run(){
    uint32_t attempts = 0;
    while (attempts != 6 && guessbool != "22222"){
        string in;
        cout << "YOUR GUESS ('q' to quit): ";
        getline(cin >> ws, in);
        if (in == "q") break;

        try{
            Word guess(in);
            
            deleted.push_back(in);

            //NEED CHECK WORD TO ANSWER // CHECK CORRECTNESS - use helper function
            checkguess(guess, attempts);

            guesses.push_back(guess);

            //turn this into function later will write in here for now
                //edit wrote it in the checkguess func;
            attempts++;

        }catch (runtime_error &e){
            cerr << e.what() << endl;
        }

        cout << "YOUR GUESSES:\n------------\n";
        for (Word &w : guesses){
            for (char &c : w.word){
                cout << c << ' ';
            }cout << endl;
        }cout << endl;


        cout << "YOUR ATTEMPTS: " << attempts << endl << endl;
    }//END while loop

    if (attempts == 6) cout << "you lost, lol" << endl;
    return "IN WORKS: finished run";
}//END run()