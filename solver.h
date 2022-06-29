#include "solver.hpp"

using namespace std;

map<char, map<int, vector<int>>> master;
//this is mapped to every letter. for every letter, records index in which is shows up in word
//and records word in index form within thelist

vector<Word> thelist, guesses;
vector<char> valid, invalid;
vector<string> deleted;

string guessbool = "";


//FUNCTIONS

void thelistmaker(){
    cout << "MAKING THE- AND MASTER- LIST..." << endl << endl;
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
//CURRENT: yellows are probably considered now, need to consider the greys
vector<int> intersections(const list<vector<int>> &ilist){
    unordered_map<int, int> checker; //element(wordidx) to counter
    vector<int> ireturn, first, second;
    const int isize = ilist.size();

    //sections are the different list of words that have character at a certain position.
    for (auto &sections : ilist){
            for (const auto &wordidx : sections) checker[wordidx] += 1;
    }

    //what can be done is to use the set difference stl function. have to do it in a way where the list is created temp in loop
    //then updates the main by using set difference again. so double set difference per char.
    for (const auto &e : checker) 
        if (checker[e.first] == isize) ireturn.push_back(e.first);

    return ireturn;
}//END intersections()

void onlyvalids(vector<int> &resultant){
    for (int &idx : resultant){
        for (char &c : thelist[idx].word){
            vector<char>::iterator invr = find(invalid.begin(), invalid.end(), c);
            if (invr != invalid.end()){
                resultant.erase(remove(resultant.begin(), resultant.end(), idx), resultant.end());
            }
        }
    }
}//END onlyvalids

void checkguess(Word &word, int attempts){
    cout << "0: wrong | 1:wrong place | 2:correct: ";
    getline(cin >> ws, guessbool);

    vector<pair<char, int>> yellow; yellow.reserve(5);
    vector<int> initial =  {0,1,2,3,4}; int ideleted = 0;
    list<vector<int>> tointersect, toinvalid;
    //toinvalid vould be the invalid list replacement

    for (int i = 0; i < 5; i++){
        if (guessbool[i] == '2'){
            valid.push_back(word.word[i]);

            //remove pos i from initial and insert vector of words from char at pos i into intersector
            tointersect.push_back(master[word.word[i]][i]);

            //delete green idx from initial
            vector<int>::iterator initit = find(initial.begin(), initial.end(), i);
            if (initit != initial.end()) initial.erase(initial.begin() + i - ideleted++);

            word.word[i] = toupper(word.word[i]);
        }
        else if(guessbool[i] == '1'){
            valid.push_back(word.word[i]);

            yellow.emplace_back(word.word[i], i);
        }
        else if (guessbool[i] == '0'){
            toinvalid.push_back(master[word.word[i]][i]);
            invalid.push_back(word.word[i]);
        }
        // else if ((guessbool[i]) > '2' || (guessbool[i]) < '0') throw runtime_error("Invalid character bool input");
        else throw runtime_error("==== Invalid character bool input ====");
    }

    //this is yellow checking, after the for loop because yellow cna show before greens or greys
    for (pair<char, int> &y : yellow){
        for (int &d : initial){
            if (y.second != d) tointersect.push_back(master[(word.word[y.first])][y.first]);
        }
    }
    
    //post finding intersections
    vector<int> resultant = intersections(tointersect); //considers greens and yellows

    onlyvalids(resultant);

    suggest(resultant, deleted, thelist);

    //THIS PART DOES NOT NEED TO BE INCLUDED. ONLY FOR VISUALIZATION
    if (!yellow.empty()){
        cout << "THESE LETTERS ARE IN WRONG PLACE (inorder): ";
        for (auto &c : yellow) cout << c.first << ' ';
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

            attempts++;
        }catch (runtime_error &e){
            cerr << e.what() << endl;
        }

        showguesses(guesses);

        cout << "YOUR ATTEMPTS: " << attempts << endl << endl;
    }//END while loop

    if (attempts == 7) return "you lost, lol";
    return "IN WORKS: finished run";
}//END run()