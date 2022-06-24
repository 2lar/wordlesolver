#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <array>
#include <fstream>
#include <sstream>

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
vector<Word> thelist, guesses;
vector<char> valid, invalid;


//FUNCTIONS
void thelistmaker(){
    //since there are a lot of words, should reserve the amount of space that we will use.
    thelist.reserve(12972);
    
    //create an input file and read in contents
    ifstream thewords;
    thewords.open("words.txt");

    //add contents to thelist
    string tempword;
    while (getline(thewords, tempword)) thelist.push_back(tempword);
}//END thelistmaker()

void checkguess(Word &word){
    //correctness legend:
    // 0 = black
    // 1 = yellow
    // 2 = green

    string guessbool;
    cout << "0: wrong | 1:wrong place | 2:correct: ";
    getline(cin >> ws, guessbool);

    vector<char> yellow; yellow.reserve(5);

    for (uint32_t i = 0; i < 5; i++){
        if (guessbool[i] == '2'){
            word.word[i] = toupper(word.word[i]);
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
    while (attempts != 6){
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