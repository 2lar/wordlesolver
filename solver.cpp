#include "solver.h"

using namespace std;

int main(){
    cout << "============\nHellow World\n============\n\n";

    cout << "MAKING THELIST..." << endl << endl;
    thelistmaker(); //make thelist of all words in "words.txt" | contains 12972 of 5 letter words

    cout << "THESE ARE RECOMMENDED START WORDS:\n" << //these words are found online
        "SOARE | ROATE | RAISE | CRANE | SLATE\n" <<
        "SEARE | STARE | SLANT | TRACE | ROAST\n" <<
        "but of course, you are free to choose whatever you like !\n\n"; 

    cout << run();
    return 0;
}