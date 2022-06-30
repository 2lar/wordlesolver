#include "solver.hpp"
#include <unordered_map>

using namespace std;

unordered_map<char, unordered_map<int, unordered_set<int>>> master;

vector<Word> thelist, guesses;
vector<int> initial =  {0,1,2,3,4}; int ideleted = 0;

unordered_map<char, int> valid, partialinvalid; //pair between the char and pos that is invalid;
unordered_map<char, int> totinvalid;
unordered_set<int> resultant, invalidlist, yellowed, thefinal;

vector<string> deleted;
string guessbool = "";

void test(){
    cout << thelist[0].word << endl;
}

void thelistmaker(){
    cout << "Making the THE and MASTER LIST..." << endl << endl;

    thelist.reserve(12973);

    ifstream thewords;
    thewords.open("words.txt");

    string tempword;
    uint32_t counter = 0;
    while (getline(thewords, tempword)){
        thelist.emplace_back(tempword);
        for (int i = 0; i < 5; i++){
            master[tempword[i]][i].insert(counter);
        }counter++;
    }
    
    firstsuggestions();
}//END listmaker

unordered_set<int> intersections(const list<unordered_set<int>> &ilist){
    unordered_map<int, int> checker;
    unordered_set<int> ireturn;
    const int isize = valid.size();

    for (const auto &sections : ilist){
        for (const auto &wordidx : sections){
            checker[wordidx] += 1;
            // cout << thelist[wordidx].word << endl;
        }
    }

    for (const auto &e : checker){
        if (checker[e.first] == isize) ireturn.insert(e.first);
    }

    // cout << "THIS IS IRETURN" << endl;
    // for (auto &i : ireturn){
    //     cout << thelist[i].word << endl;
    // }

    return ireturn;
}//END intersections()

void onlyvalids(unordered_set<int> &thefinal, unordered_set<int> &invalidlist){
    for (auto &key : thefinal){
        unordered_set<int>::iterator ovc = invalidlist.find(key);
        if (ovc != invalidlist.end()){
            thefinal.erase(key);
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
            valid[word.word[i]] = i;

            //remove pos i from initial and insert vector of words from char at pos i into intersector
            // for (auto &u : master[word.word[i]][i]){
            //     cout << thelist[u].word << endl;
            // }
            tointersect.push_back(master[word.word[i]][i]);
            

            //delete green idx from initial
            vector<int>::iterator initit = find(initial.begin(), initial.end(), i);
            if (initit != initial.end()) initial.erase(initial.begin() + i - ideleted++);

            word.word[i] = toupper(word.word[i]);
        }

        else if (guessbool[i] == '1'){
            yellow.emplace_back(word.word[i], i);
        }

        else if (guessbool[i] == '0'){
            for (auto &u : master[word.word[i]][i]){
                invalidlist.insert(u);
            }
        }

        else throw runtime_error("==== Invalid character bool input ====");
    }//end for loop

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

    // cout << "THIS IS TO INTERESECT" << endl;
    // for (auto &c : tointersect){
    //     for (auto &k : c){
    //         cout << thelist[k].word << endl;
    //     }
    // }cout << endl;

    // cout << "THIS IS YELLOWSET" << endl;
    // for (auto k : yellowset){
    //     cout << thelist[k].word << endl;
    // }cout << endl;

    //post finding intersections
    // cout << "THIS IS RESULTANT" << endl;
    resultant = intersections(tointersect);
    // for (auto &p : resultant){
    //     cout << thelist[p].word << ' ';
    // }

    cout << "THIS IS FINAL INTERSECTIONS: ";
    thefinal = intersections({resultant, yellowset});
    for (auto &f : thefinal){
        cout << thelist[f].word << ' ';
    }cout << endl;

    onlyvalids(thefinal, invalidlist);

    suggest(thefinal, deleted, thelist);

}//END checkguess

string run(){
    test();

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

            // cout << "THIS IS invalidlist: ";
            // for (auto &i : invalidlist){
            //     cout << thelist[i].word << " ";
            // }
            // cout << endl << endl;

            // cout << "THIS IS totinvalid: ";
            // for (auto &i : totinvalid){
            //     cout << '(' << i.first << ", " << i.second << ')';
            // }
            // cout << endl << endl;

            attempts++;
        }catch (runtime_error &e){
            cerr << e.what() << endl;
        }

        showguesses(guesses);

        cout << "YOUR ATTEMPTS: " << attempts << endl << endl;
    }//END while loop

    return "THIS IS RUN\n";

}//END run
