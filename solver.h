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
            cerr << "Invalid word size: "+x+"\n"; //user defined literal operator to iterate through each char
            exit(1);
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
}//END thelistmaker