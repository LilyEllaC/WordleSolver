//including
#include <iostream>
#include <cctype>
#include <string>
#include <vector>
#include <cstdlib>
#include <set>
#include <fstream>
using namespace std;

//functions
//getting the word list
set <string> getWordleWords() {
    set <string> goodWords;       
    set <string> newWords;
    ifstream file("options.txt");    
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
bool isInString(vector <string> list, string letter){
    bool inIt=false;
    for (int i=0; i<list.size(); i++){
        if (list[i]==letter){
            inIt=true;
        }
    }
    return inIt;
}


void gettingLetters(vector <char> &colourLetters, string colour, string alphabet){
    char letter;
    //getting the letters
    cout<<"\033[0;1mPlease enter a "<<colour<<" letter that you have (when done enter 1): ";
    cin>>letter;
    letter=toupper(letter);

    while (letter!='1'){
        if (alphabet.find(letter)<26){
            colourLetters.push_back(letter);
        } else {
            cout<<"That isn't a letter. \n";
        }
        cout<<"Please enter another "<<colour<<" letter: ";
        cin>>letter;
        letter=toupper(letter);
    }
}

//getting placements
void placements(vector <char> greens, vector <char> yellows, vector <int> &greenPlacements, vector <string> &yellowPlacements){
    //greens
    int placement;
    for (int i=0; i<greens.size(); i++){
        cout<<"Where is your "<<greens[i]<<"? ";
        cin>>placement;
        while (placement>5||placement<1||isInInt(greenPlacements, placement)){
            cout<<"That isn't a possible placement. Please try again.";
            cin>>placement;
        }
        greenPlacements.push_back(placement);
    }

    //yellows
    for (int i=0; i<yellows.size(); i++){
        yellowPlacements.push_back("");
        placement=6;
        cout<<"Where can your "<<yellows[i]<<" not be? (enter your number, click enter, then enter the next. enter -1 to stop.) \n";
        while (placement!=-1){
            cin>>placement;
            while ((placement>5||placement<1||isInString(yellowPlacements, to_string(placement)))&&placement!=-1){
                cout<<"That isn't a possible placement. Please try again.";
                cin>>placement;
            }
            if (placement!=-1){
                yellowPlacements[i].append(to_string(placement));
            }
        }
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
        for (int i=0; i<greens.size(); i++){
            included=false;
            letter=greens[i];
            //in word
            for (int j=0; j<5; j++){
                if (letter==word[j]){
                    included=true;
                    j=6;
                }
            }
            if (!included){
                allIncluded=false;
                i=greens.size()+1;
            }
            rightSpot=false;
            //placements
            if (greens[i]==word[greenPlacements[i]-1]){
                rightSpot=true;
            }
            if (!rightSpot){
                i=greens.size()+1;
            }
        }
        
        //making sure it has the yellow letters it needs to
        if (allIncluded&&rightSpot){
            for (int i=0; i<yellows.size(); i++){
                included=false;
                letter=yellows[i];
                for (int j=0; j<5; j++){
                    if (letter==word[j]){
                        included=true;
                        j=6;
                    }
                }
                if (!included){
                    allIncluded=false;
                    i=yellows.size()+1;
                }
                //placements
                for (int j=0; j<yellowPlacements[i].length(); j++){
                    fprintf(stderr, "In yellow for loop");
                    cout<<"    Place: "<<yellowPlacements[i];
                    number=yellowPlacements[i];
                    if (yellows[i]==word[number[j]-49]){
                        rightSpot=false;
                    }
                }
                if (!rightSpot){
                    i=greens.size()+1;
                }
            }
        }
        if (allIncluded&&rightSpot){
            possibles.push_back(word);
        }
    }
}


//main
int main(){
    //variables
    string letter, alphabet="QWERTYUIOPASDFGHJKLZXCVBNM";
    int placement;
    vector <char> greens, grays, yellows, possibleLetters;
    vector <string> possibleWords, yellowPlacements;
    vector <int> greenPlacements;
    set <string> totalOptions=getWordleWords();

    //getting the letters
    gettingLetters(greens, "\033[0;32mgreen\033[0;1m", alphabet);
    gettingLetters(yellows, "\033[0;93myellow\033[0;1m", alphabet);
    gettingLetters(grays, "\033[0;98mgray\033[0;1m", alphabet);
        
    //getting placements
    placements(greens, yellows, greenPlacements, yellowPlacements);
    
    //going through all words
    for (int i=0; i<totalOptions.size(); i++){
        auto it = totalOptions.begin();
        // Advance the iterator to the random spot
        advance(it, i); 
        //going to elimintation 
        eliminating(possibleWords, grays, greens, yellows, *it, greenPlacements, yellowPlacements);
    }
    cout<<"\nSucess! The size of the list is "<<possibleWords.size()<<"\n\n";
    cout<<"\nSome options for the words are :"<<possibleWords[rand()%possibleWords.size()]<<"\n"<<possibleWords[rand()%possibleWords.size()]<<"\n"<<possibleWords[rand()%possibleWords.size()]<<"\n"<<possibleWords[rand()%possibleWords.size()];

    return 0;
}