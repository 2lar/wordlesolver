#include "solver.hpp"

using namespace std;

unordered_map<char, unordered_map<int, unordered_set<int>>> master;

vector<Word> thelist, guesses;
vector<int> initial = {0,1,2,3,4}; int ideleted = 0;

unordered_map<char, int> frequencies;
unordered_set<int> resultant, invalidlist, thefinal;
unordered_map<int, char> valid;
unordered_set<char> invalidchar, validchar, yellowed;

map<char, vector<int>> freqatpos;

vector<string> deleted;
string guessbool = "";

void test(){
    cout << thelist[0].word << endl;
}

void thelistmaker(){
    cout << "Making the THE and MASTER LIST..." << endl << endl;

    //this is to make the frequency list
    string alphabet = "abcdefghijklmnopqrstuvwxyz";
    for (char &c : alphabet){
        vector<int> freq(5);
        freqatpos[c] = freq;
        frequencies[c] = 0;
    }//END for loop

    thelist.reserve(12973);

    ifstream thewords;
    thewords.open("words.txt");

    string tempword;
    uint32_t counter = 0;
    while (getline(thewords, tempword)){
        thelist.emplace_back(tempword);
        for (int i = 0; i < 5; i++){
            //insert the wordidx into the master
            master[tempword[i]][i].insert(counter);

            freqatpos[tempword[i]][i]++;
            frequencies[tempword[i]]++;
        }counter++;
    }
    firstsuggestions();

}//END listmaker

unordered_set<int> intersections(const list<unordered_set<int>> &ilist){
    unordered_map<int, int> checker;
    unordered_set<int> ireturn;
    const int isize = ilist.size();

    for (const auto &sections : ilist){
        for (const auto &wordidx : sections){
            checker[wordidx] += 1;
        }
    }
    for (const auto &e : checker){
        if (checker[e.first] == isize) ireturn.insert(e.first);
    }

    return ireturn;
}//END intersections()

void onlyvalids(unordered_set<int> &thefinal, unordered_set<int> &invalidlist){
    if (invalidlist.size() == 0){
        return;
    }
    for (auto &key : thefinal){
        unordered_set<int>::iterator ovc = invalidlist.find(key);
        if (ovc != invalidlist.end()){
            thefinal.erase(key);
        }

        for (auto &s : thelist[key].word){
            unordered_set<char>::iterator invc = invalidchar.find(s);
            if (invc != invalidchar.end()){
                thefinal.erase(key);
            }
        }
    }
}//END onlyvalids

void checkguess(Word &word, int attempts){
    cout << "0: wrong | 1:wrong place | 2:correct: ";
    getline(cin >> ws, guessbool);

    vector<pair<char, int>> yellow; yellow.reserve(5);
    unordered_set<int> yellowset;
    list<unordered_set<int>> tointersect;
    
    for (int i = 0; i < 5; i++){
        if (guessbool[i] == '2'){
            valid[i] = word.word[i];
            validchar.insert(word.word[i]);
            tointersect.push_back(master[word.word[i]][i]);
            
            //delete green idx from initial
            vector<int>::iterator initit = find(initial.begin(), initial.end(), i);
            if (initit != initial.end()) initial.erase(initial.begin() + i - ideleted++);

            word.word[i] = toupper(word.word[i]);
        }

        else if (guessbool[i] == '1'){
            yellow.emplace_back(word.word[i], i);
            yellowed.insert(word.word[i]);
        }

        else if (guessbool[i] == '0'){
            //adding EACH word into the invalid list
            for (auto &u : master[word.word[i]][i]){
                invalidlist.insert(u);
            }
            if (validchar.find(word.word[i]) == validchar.end()){
                if (word.word.substr(i+1).find(word.word[i]) == string::npos){
                    invalidchar.insert(word.word[i]);
                }
            }
        }

        else throw runtime_error("==== Invalid character bool input ====");
    }//end for loop

    //ADD INTO FREQUENCY
    for (auto &c : word.word){
         frequencies[c]++;
    }

    //this is yellowchecking
    for (pair<char, int> &y : yellow){
        for (int &d : initial){
            if (y.second != d){
                for (auto &yel : master[word.word[y.second]][d]){
                    yellowset.insert(yel);
                }
            }
        }
    }//END for loop

    //post finding intersections
    if (initial.size() < 5){
        resultant = intersections(tointersect);
        
        if (yellowset.size() != 0){
            thefinal = intersections({resultant, yellowset});
            resultant = thefinal;
        }

        onlyvalids(resultant, invalidlist);
        suggest(resultant, deleted, thelist, frequencies);
      
    }
    else{
        //use the frequenct list to compute the next best suggestion based on 5 invalids.
        cout << "you got nothing remotely close in the first try" << endl;
    }
}//END checkguess

void run(){
    // test();
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

    if (guessbool != "22222" && attempts == 6){
        cout << "lol u failed" << endl;
    }

    cout << "THIS IS RUN\n";

}//END run