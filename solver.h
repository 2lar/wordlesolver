#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <array>
#include <fstream>
#include <sstream>
#include <map>
#include <unordered_map>

using namespace std;

struct Word{
    //char array of size 6 because there is null terminated char at end
    //we could use size 5 but it could cause some weird things
    //will need to see whether we can use 5 or not
    char word[6];

    Word(string x = "     "){
        if (x.size() != 5){
            // cerr << "Invalid word size: "+x+"\n"; //user defined literal operator to iterate through each char
            throw runtime_error("Invalid word size: "+x+"\n");
        }
        for (uint32_t i = 0; i < 5; i++) word[i] = x[i];
    }
};//END word struct

//THINGS TO KEEP TRACK TO GET SOLTION
const string alphabet = "abcdefghijklmnopqrstuvwxyz";

map<char, map<int, vector<int>>> master;
//this is mapped to every letter. for every letter, records index in which is shows up in word
//and records word in index form within thelist

vector<Word> thelist, guesses;
vector<char> valid, invalid;
vector<int> combine;

string guessbool = "";


//FUNCTIONS
void thelistmaker(){
    cout << "MAKING THE AND MASTER LIST..." << endl << endl;
    //since there are a lot of words, should reserve the amount of space that we will use.
    thelist.reserve(12972);
    
    //create an input file and read in contents
    ifstream thewords;
    thewords.open("words.txt");

    //add contents to thelist
    string tempword;
    uint32_t counter = 0;
    while (getline(thewords, tempword)){
        thelist.push_back(tempword);
        for (int i = 0; i < 5; i++){
            master[tempword[i]][i].push_back(counter);
        }counter++;
    }

    // cout << "THIS IS MASTER SIZE: " << master.size() << endl;
    // cout << "THIS IS 'A' SIZE: " << master['a'].size() << endl;
    // cout << "THIS IS HOW MANY WORDS HAVE A IN FIRST POSITION: " << master['a'][0].size() << endl;
    // cout << "THIS IS THE FIRST WORD WITH A IN THE BEGINNING: " << thelist[master['a'][0][0]].word << endl;
    // cout << "THIS IS VECTOR[4]: " << thelist[4].word << endl;



    //these words are found online
    cout << "THESE ARE RECOMMENDED START WORDS:\n" <<
        "SOARE | ROATE | RAISE | CRANE | SLATE\n" <<
        "SEARE | STARE | SLANT | TRACE | ROAST\n" <<
        "ADIEU | SNARE | RAISE | AESIR | MONKEYU (jk)\n" <<
        "but of course, you are free to choose whatever you like !\n\n"; 

}//END thelistmaker()

void checkguess(Word &word){
    //correctness legend:
    // 0 = black
    // 1 = yellow
    // 2 = green

    cout << "0: wrong | 1:wrong place | 2:correct: ";
    getline(cin >> ws, guessbool);

    vector<char> yellow; yellow.reserve(5);

    for (uint32_t i = 0; i < 5; i++){
        vector<int> combiner, sfinal;
        if (guessbool[i] == '2'){
            word.word[i] = toupper(word.word[i]);

            //find all indices that have char at i and add into combine... EDIT: THIS METHOD IS TOO LONG AND CONSUMING
            //find another one
        }
        else if(guessbool[i] == '1'){
            yellow.push_back(word.word[i]);
        }
        else if ((guessbool[i]) > '2' || (guessbool[i]) < '0') throw runtime_error("Invalid character bool input");


    }

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
            
            //NEED CHECK WORD TO ANSWER // CHECK CORRECTNESS - use helper function
            checkguess(guess);

            guesses.push_back(guess);
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

    return "IN WORKS: finished run";
}//END run()