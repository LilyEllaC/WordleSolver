//including
#include <iostream>
#include <cctype>
#include <string>
#include <vector>
#include <cstdlib>
#include <set>
#include <fstream>
#include <cmath>
#include <algorithm>
#include "solver.h"
using namespace std;

//functions
//getting the word list
set <string> getWordleWords(string fileName) {
    set <string> goodWords;       
    set <string> newWords;
    ifstream file(fileName);    
    string word;                   
    
    // Read each word from file and add to set
    while (file >> word) {
        goodWords.insert(word);             
    }

    //capitalizing
    for (const std::string& word : goodWords) {
        std::string upperWord = word; 
        for (char& c : upperWord) {
            c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        }
        newWords.insert(upperWord);
    }
    return newWords;                       
}
//integers
bool isInInt(vector <int> list, int letter){
    bool inIt=false;
    for (int i=0; i<list.size(); i++){
        if (list[i]==letter){
            inIt=true;
        }
    }
    return inIt;
}

//strings
bool isInChar(vector <char> list, char letter){
    bool inIt=false;
    for (int i=0; i<list.size(); i++){
        if (list[i]==letter){
            inIt=true;
        }
    }
    return inIt;
}

//getting green letters
void greenLetters(vector <char> &greens, vector <int> &greenPlacement, string alphabet){
    string word;
    bool redo=false;
    cout<<"\033[0;1mPlease enter your \033[0;32mgreen\033[0;1m letters (ex. --A-E ): ";
    cin>>word;

    //checking for problems
    while (word.length()!=5){
        cout<<"Please try again. You should enter dashes for the non green letters and the letters in the positions they should be. \nIf you don't have any green letters enter 5 dashes: ";
        cin>>word;
        
    }
    do{
        redo=false;
        for (int i=0; i<5; i++){
            if (word[i]!='-'){
                if (alphabet.find(toupper(word[i]))>26){
                    redo=true;
                }
            }
        }
        if (redo){
            cout<<"You need to enter a letter, no other symbols, and it needs to be 5 characters long please: ";
            cin>>word;
        }
    } while (redo);
    //putting the letters into the vectors
    for (int i=0; i<5; i++){
        if (word[i]!='-'){
            word[i]=toupper(word[i]);
            greens.push_back(word[i]);
            greenPlacement.push_back(i+1);
        }
    }
}

//getting the gray and yellow letters
void gettingLetters(vector <char> &colourLetters, string colour, string alphabet, vector <char> greens, vector <char> yellows){
    string allLetters;
    char letter;
    //getting the letters
    cout<<"\033[0;1mPlease enter all the "<<colour<<" letter that you have (if none, enter 1): ";
    cin>>allLetters;
    if (allLetters!="1"){
        for (int i=0; i<allLetters.length(); i++){
            letter=toupper(allLetters[i]);
            //making sure it is a letter
            if (alphabet.find(letter)<26&&!isInChar(greens, letter)&&!isInChar(yellows, letter)){
                colourLetters.push_back(letter);
            }
        }
    }
}

//getting yellow placements
void placements(vector <char> yellows, vector <string> &yellowPlacements){
    //greens
    string placement;
    bool redo;

    //yellows
    for (int i=0; i<yellows.size(); i++){
        yellowPlacements.push_back("");
        placement=6;
        cout<<"Where can your "<<yellows[i]<<" not be? (ex. A-A--): ";
        cin>>placement;
        while (placement.length()!=5){
            cout<<"That isn't how you should enter it, sorry. \nFor example if you know your A can't be 1st or 3rd, enter A-A--): ";
            cin>>placement;
        }

        //checking for wrong enters.
        do{
            redo=false;
            for (int j=0; j<5; j++){
                if (placement[j]!='-'&&toupper(placement[j])!=yellows[i]){
                        redo=true;
                }
            }
            if (redo){
                cout<<"You need to enter only the letter "<<yellows[i]<<" or dashes and you need to enter all 5 characters please. \nFor example if you know your A can't be 1st or 3rd, enter A-A--):";
                cin>>placement;
            }
        } while (redo);

        //adding
        for (int j=0; j<5; j++){
            if (placement[j]!='-'){
                yellowPlacements[i].append(to_string(j+1));
            }
        }
            
        }
    /*cout<<"The yellow letters are: \n";
    for (int i=0; i<yellows.size(); i++){
        cout<<yellows[i]<<": "<<yellowPlacements[i]<<endl;
    }*/
}

//seeing if they want all answers or just possible answers
void wantsAllWords(set <string> &totalOptions){
    string answer;
    cout<<"\nDo you want all possible 5 letter words, or just the 5 letter words that could be the answer? (enter 'p' for possible words, enter 'a' for only answers): ";
    cin>>answer;
    while(answer!="p"&&answer!="a"&&answer!="P"&&answer!="A"){
        cout<<answer<<" is not an acceptable response. Please try again: ";
        cin>>answer;
    }
    if (answer=="a"||answer=="A"){
        totalOptions=getWordleWords("answers.txt");
    } else if (answer=="p"||answer=="P"){
        totalOptions=getWordleWords("options.txt");
    }
}

//getting the possible words
void eliminating (vector <string> &possibles, vector <char> grays, vector <char> greens, vector <char> yellows, string word, vector <int> greenPlacements, vector <string> yellowPlacements){
    bool bad=false;
    bool included, rightSpot, allIncluded=true;
    string number;
    char letter;
    //going through the gray letters
    for (int i=0; i<grays.size(); i++){
        letter=grays[i];
        //going through the word's letters
        for (int j=0; j<5; j++){
            if (letter==word[j]){
                bad=true;
            }
        }
    }
    //doesn't have the worng letters
    if (!bad){
        //making sure it has the green letters it needs to
        greensRight(greens, included, letter, word, allIncluded, rightSpot, greenPlacements);

        //making sure it has the yellow letters it needs to
        if (allIncluded&&rightSpot){
            for (int i=0; i<yellows.size(); i++){
                included=false;
                letter=yellows[i];
                hasLetter(letter, word, included);
                if (!included){
                    allIncluded=false;
                    i=yellows.size()+1;
                }
            }
            if (allIncluded){
                //are they in the right spot
                for (int i=0; i<yellowPlacements.size(); i++){
                    rightSpot=true;
                    //placements
                    for (int j=0; j<yellowPlacements[i].length(); j++){
                        number=yellowPlacements[i];
                        if (yellows[i]==word[number[j]-49]){
                            rightSpot=false;
                            j=10;
                        }
                    }
                    if (!rightSpot){
                        i=greens.size()+1;
                    }
                }
            }
        }
        if (allIncluded&&rightSpot){
            possibles.push_back(word);
        }
    }
}

//making sure the word has the green letters in the right places
void greensRight(std::vector<char> &greens, bool &included, char &letter, std::string &word, bool &allIncluded, bool &rightSpot, std::vector<int> &greenPlacements){
    for (int i = 0; i < greens.size(); i++){
        included = false;
        letter = greens[i];
        // in word
        for (int j = 0; j < 5; j++){
            if (letter == word[j]){
                included = true;
                j = 6;
            }
        }
        if (!included){
            allIncluded = false;
            i = greens.size() + 1;
        }
        rightSpot = false;

        // placements
        if (greens[i] == word[greenPlacements[i] - 1]){
            rightSpot = true;
        }
        if (!rightSpot){
            i = greens.size() + 1;
        }
    }
    //having no greens still work
    if (greens.size()==0){
        rightSpot=true;
    }
}

//checking if it has the letter
void hasLetter(char letter, std::string &word, bool &included){
    for (int j = 0; j < 5; j++){
        if (letter == word[j]){
            included = true;
            j = 6;
        }
    }
} 

//getting letter percentages
void letterPercents(vector <string> possibleWords, string alphabet, vector <float> &letterPercentages){
    vector <int> letterCount;
    float percent;
    //getting the vector big enough
    for (int i=0; i<26; i++){
        letterCount.push_back(0);
    }
    for (int i=0; i<possibleWords.size(); i++){
        for (int j=0; j<26; j++){
            if (possibleWords[i].find(alphabet[j])<5){
                letterCount[j]+=1;
            }
        }
    }
    for (int i=0; i<26; i++){
        percent=round(float(letterCount[i])/float(possibleWords.size())*10000)/100;
        letterPercentages.push_back(percent);
    }
}

//sorting the percentages
void sorting(vector <float> &percents){
    sort(percents.begin(), percents.end(), greater <float>());
}

// instructionn
void instructions(){
    cout<<"\nWhen you are asked to enter your green letters and for example you know that the A is 3rd and the T is last you would enter --A-T (could be capitalized or not). ";
    cout<<"\nThe dashes mean you don't know which letter goes in that spot so if you don't know the positions of any letters just enter five dashes. ";
    cout<<"\nYou will be asked to enter your yellow letters and if there are none, enter 1. ";
    cout<<"\nIf you know that there is an L and an E, you would enter LE.";
    cout<<"\nFor the gray letters you will enter all of the letters you know are not in the word and if there are no gray letters enter 1.";
    cout<<"\ne.x. QWYUIOH";
    cout<<"\nIf you accidently enter a number or enter a yellow or green letter into the gray letters, it will just ignore it.";
    cout<<"\nIt will then ask you if you want to see any 5 letter words that follows the criteria or just the word that could be the answer.";
    cout<<"\nEnter 'p' for possible words, and enter 'a' for just the wordle answers. ";
    cout<<"\nIt will then show you which letters appear in any of the words and what percentage of the possible words they are in.";
    cout<<"\nThen it will show the possible words following the criteria.\n\n";
}

void runTest(){
    vector <char> greens={'A'};
    vector <char> grays;
    vector <char> yellows={'B'};
    vector <char> possibleLetters;
    set <string> totalOptions=getWordleWords("options.txt");
   // set <string> totalOptions={"BREAK"};

    vector <int> greenPlacements={3};

    vector <string> yellowPlacements ={"12"};
    vector <string> possibleWords;
    //going through all words
    for (int i=0; i<totalOptions.size(); i++){
        auto it = totalOptions.begin();
        // Advance the iterator to the random spot
        advance(it, i); 
        //going to elimintation 
        eliminating(possibleWords, grays, greens, yellows, *it, greenPlacements, yellowPlacements);
    }
    if (possibleWords.size()>0){
        cout<<"broken, this should not be reached";

    }
    for (int i=0; i<possibleWords.size(); i++){
        cout<<possibleWords[i]<<endl;
    }
    
}


//main
int main(){
    //runTest();
    //variables
    string letter, wantInstructions, playAgain="y", alphabet="ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int placement, counter;
    vector <char> greens, grays, yellows, possibleLetters;
    vector <string> possibleWords, yellowPlacements;
    vector <int> greenPlacements;
    vector <float> letterPercentages;
    set <string> totalOptions;

    //instructions
    cout<<"\033[0;1mIf you need instructions enter y, otherwise enter n: ";
    cin>>wantInstructions;
    if (wantInstructions=="y"){
        instructions();
    }

    while (playAgain=="y"){
        //getting the letters
        greenLetters(greens, greenPlacements, alphabet);
        gettingLetters(yellows, "\033[0;93myellow\033[0;1m", alphabet, greens, {' ', ' '});
        gettingLetters(grays, "\033[0;98mgray\033[0;1m", alphabet, greens, yellows);
        
        //getting placements
        placements(yellows, yellowPlacements);
        
        //seeing which words they want
        wantsAllWords(totalOptions);

        //going through all words
        for (auto it:totalOptions){
            eliminating(possibleWords, grays, greens, yellows, it, greenPlacements, yellowPlacements);
        }

        //outputting words
        cout<<"\n\nThere are "<<possibleWords.size()<<" words. \n";
        cout<<"\nThe words are:\n";
        counter=0;
        for (int i=0; i<possibleWords.size(); i++){
            cout<<possibleWords[i]<<"\t";
            counter+=1;
            if (counter==10){
                counter=0; 
                cout<<"\n";
            }
        }

        //dealing with what if it is 0
        if (possibleWords.size()==0){
            cout<<"Sorry, there doesn't seem to be any options. Please try again. \nIt is possible that you entered one of your green or yellow letters in with the gray letters.";
        }
        //making sure that there are words.
        if (possibleWords.size()!=0){
            //outputting percentages
            letterPercents(possibleWords, alphabet, letterPercentages);
            counter=0;
            cout<<"\n\n";
            //sorting(letterPercentages);
            for (int i=0; i<26; i++){
                if (letterPercentages[i]!=0){
                    printf("%c: %3.2f% \t",alphabet[i], letterPercentages[i]);
                    counter+=1;
                }
                if (counter==10){
                    counter=0; 
                    cout<<"\n";
                }
            }
        }

        //playing again
        cout<<"\n\nWould you like to play again? (y/n): ";
        cin>>playAgain;
        
        if (playAgain=="y"){
            greens={};
            grays={};
            yellows={};
            possibleWords={};
            yellowPlacements={};
            greenPlacements={};
            letterPercentages={};
            system("clear");
        }
    }

    return 0;
}