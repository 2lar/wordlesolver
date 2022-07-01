#include "solver.hpp"


using namespace std;

unordered_map<char, unordered_map<int, unordered_set<int>>> master;

vector<Word> thelist, guesses;
vector<int> initial =  {0,1,2,3,4}; int ideleted = 0;

unordered_map<char, int> partialinvalid; //pair between the char and pos that is invalid;
unordered_map<char, int> totinvalid, frequencies;
unordered_set<int> resultant, invalidlist, yellowed, thefinal;
unordered_map<int, char> valid;
unordered_set<char> invalidchar, validchar;

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

    // for (auto &c : alphabet){
    //     cout << c << ": " << frequencies[c] << endl;
    // }
}//END listmaker

unordered_set<int> intersections(const list<unordered_set<int>> &ilist){
    unordered_map<int, int> checker;
    unordered_set<int> ireturn;
    const int isize = valid.size();
    // for (auto &v : valid){
    //     cout << v.first << ' ';
    // }cout << endl;

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
    if (invalidlist.size() == 0){
        // cout << "returning from onlyvalids" << endl;
        return;
    }
    for (auto &key : thefinal){
        unordered_set<int>::iterator ovc = invalidlist.find(key);
        if (ovc != invalidlist.end()){
            // cout << "this is ovc: " << thelist[*ovc].word << endl;
            thefinal.erase(key);
        }

        // cout << "THIS IS INVALID CHAR LIST: ";
        // for (auto &c : invalidchar){
        //     cout << c << " ";
        // }cout << endl;

        // cout << "THIS IS THE WORD: " << thelist[key].word << endl;
        // cout << "THIS IS THE char in the string: ";
        for (auto &s : thelist[key].word){
            // cout << s << " ";
            unordered_set<char>::iterator invc = invalidchar.find(s);
            if (invc != invalidchar.end()){
                // cout << "DELETING BECAUSE IT FOUND INVALID CHAR" << endl;
                thefinal.erase(key);
            }
        }
        // cout << endl;
    }

    // cout << "THEFINAL BEFORE SUGGEST IN ONLYVALID" << endl;
    // for (auto &w : thefinal){
    //     cout << thelist[w].word << endl;
    // }cout << endl;
}//END onlyvalids

void checkguess(Word &word, int attempts){
    cout << "0: wrong | 1:wrong place | 2:correct: ";
    getline(cin >> ws, guessbool);

    vector<pair<char, int>> yellow; yellow.reserve(5);
    unordered_set<int> yellowset;
    list<unordered_set<int>> tointersect;
    

    // cout << "THIS IS VALID SIZE: ";
    // cout << valid.size() << endl;

    for (int i = 0; i < 5; i++){
        if (guessbool[i] == '2'){
            valid[i] = word.word[i];
            validchar.insert(word.word[i]);
            // cout << "UPDATED VALID SIZE: ";
            // cout << valid.size() << endl;
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
            //adding EACH word into the invalid list
            for (auto &u : master[word.word[i]][i]){
                // cout << "INSERT INTO INVALIDLIST: " << thelist[u].word << endl;
                invalidlist.insert(u);
            }
            if (validchar.find(word.word[i]) == validchar.end()){
                // cout << "INSERT INTO INVALIDCHAR: " << word.word[i] << endl;
                if (word.word.substr(i+1).find(word.word[i]) == string::npos){
                    // cout << "adding into invalid char" << endl;
                    invalidchar.insert(word.word[i]);
                }
            }
        }

        else throw runtime_error("==== Invalid character bool input ====");
    }//end for loop

    // cout << "THIS IS VALID SIZE:";
    // cout << valid.size() << endl;

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
    if (initial.size() < 5){
        resultant = intersections(tointersect);

        // cout << "SEEING: ";
        // for (auto &p : resultant){
        //     cout << thelist[p].word << ' ';
        // }cout << endl;

        
        if (yellowset.size() != 0){
            // cout << "YELLOWSET SIZE is not 0: " << yellowset.size() << endl;
            thefinal = intersections({resultant, yellowset});
            resultant = thefinal;
        }


        
        onlyvalids(resultant, invalidlist);
        suggest(resultant, deleted, thelist, frequencies);



        // for (auto &s : scores){
        //     cout << s.first << ": " << s.second << endl;
        // }

        

        //need to make the suggestion based on strongest frequency score.
        
    }
    else{
        //use the frequenct list to compute the next best suggestion based on 5 invalids.
        cout << "you got nothing remotely close in the first try" << endl;
    }
    // cout << "end of checkguess" << endl;
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

            // cout << "pushing into guesses" << endl;
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

        // cout << "MADE IT TO SHOWGUESSES" << endl;
        showguesses(guesses);

        cout << "YOUR ATTEMPTS: " << attempts << endl << endl;
    }//END while loop

    if (guessbool != "22222" && attempts == 6){
        cout << "lol u failed" << endl;
    }

    cout << "THIS IS RUN\n";

}//END run
