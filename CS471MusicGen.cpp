#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct Note {
    int pitch;
    int length;

    Note(int p, int l) : pitch(p), length(l) {}
};

void printNote(Note n) {
    cout << "(" << n.pitch << ", " << n.length << ")";
}

// Function to convert pitch to letter notation (A to G)
string toLetter(int pitch) {
    string note[] = {"A", "Bb", "B", "C", "C#", "D", "Eb", "E", "F", "F#", "G", "Ab"};
    string octave[] = {"(0)", "(1)", "(2)", "(3)", "(4)", "(5)", "(6)", "(7)"};
    int indexN = pitch % 12;
    int indexO = pitch/12;
    return note[indexN] + octave[indexO];  // Return the corresponding note letter
}

// Function to convert note length to a string (e.g., "4" for quarter note)
string toLength(int l) {
    switch(l) {
        case 1: return "Whole";
        case 2: return "Half";
        case 4: return "Quarter";
        case 8: return "Eighth";
        case 16: return "Sixteenth";
        default: return "Unknown";
    }
}

// Function to convert Note array to a vector of strings
vector<string> readNotation(Note m[], int size) {
    vector<string> abc;  // A single vector to hold the string representations of the notes
    for (int i = 0; i < size; i++) {
        abc.push_back(toLetter(m[i].pitch) + " " + toLength(m[i].length));  // Combine pitch and length into one string
    }
    return abc;
}

int IScore(Note m[], int size){ //3, 5, 2, 5, 6, 4, 3           3, 7, 7, 7, 2, 6, 3 = 35
    int score = 0;
    for(int i = 0; i < size - 1; i++){
        switch(abs(m[i].pitch - m[i+1].pitch)){
            case 0: score += 5; /*cout << "\nSCORE = INTERVAL + SCORE" << endl;
            cout << score << " = " << 5 << " + " << score - 5; */break; //unison or octave jump 
            case 1: score += 2; /*cout << "\nSCORE = INTERVAL + SCORE" << endl;
            cout << score << " = " << 2 << " + " << score - 2; */ break; //minor second
            case 2: score += 7; /*cout << "\nSCORE = INTERVAL + SCORE" << endl;
            cout << score << " = " << 7 << " + " << score - 7;*/ break; //major second
            case 3: score += 3;/* cout << "\nSCORE = INTERVAL + SCORE" << endl;
            cout << score << " = " << 3 << " + " << score - 3;*/ break; //minor third
            case 4: score += 6;/* cout << "\nSCORE = INTERVAL + SCORE" << endl;
            cout << score << " = " << 6 << " + " << score - 6; */break; //major third
            case 5: score += 7; /*cout << "\nSCORE = INTERVAL + SCORE" << endl;
            cout << score << " = " << 7 << " + " << score - 7; */break; //perfect fourth
            case 6: score += 2; /*cout << "\nSCORE = INTERVAL + SCORE" << endl;
            cout << score << " = " << 2 << " + " << score - 2;*/ break; //augmented fourth
            case 7: score += 6; /*cout << "\nSCORE = INTERVAL + SCORE" << endl;
            cout << score << " = " << 6 << " + " << score - 6;*/ break; //perfect fifth
            case 8: score += 2; /*cout << "\nSCORE = INTERVAL + SCORE" << endl;
            cout << score << " = " << 2 << " + " << score - 2; */break; //minor sixth
            case 9: score += 4; /*cout << "\nSCORE = INTERVAL + SCORE" << endl;
            cout << score << " = " << 4 << " + " << score - 4; */break; //major sixth
            case 10: score += 2; /*cout << "\nSCORE = INTERVAL + SCORE" << endl;
            cout << score << " = " << 4 << " + " << score - 4; */break; //minor seventh
            case 11: score += 2; /*cout << "\nSCORE = INTERVAL + SCORE" << endl;
            cout << score << " = " << 2 << " + " << score - 2; */break; //major seventh
        }
    }
    return score;
}

int RScore(Note m[], int size){ // 1, 1, 1, 3, 2, 1, 1         8, 8, 8, 4, 6, 8, 8 = 50
    int lengths[] = {1, 2, 4, 8, 16};
    int dif, length1, length2;
    int score = 0;
    for(int i = 0; i < size - 1; i++){
        for(int j = 0; j < 5; j++){
            if(lengths[j] == m[i].length){
                length1 = j;
            }
            if(lengths[j] == m[i+1].length){
                length2 = j;
            }
        }
        dif = abs(length1 - length2);
       // cout << "\nDIF = " << dif;
        switch(dif){
            case 0: score += 10; /*cout << "\n+ 10";*/ break;
            case 1: score += 8; /*cout <<"\n+ 8";*/ break;
            case 2: score += 6; /*cout <<"\n+ 6";*/ break;
            case 3: score += 4; /*cout << "\n+ 4";*/ break;
            case 4: score += 2; /*cout <<"\n+ 2";*/ break;
            default: break; // 0 score
        } 
        //cout << "\nCURRENT RSCORE = " << score;
    }
    return score;
}

int VScore(Note m[], int size){
    int upNdown = 1;
    bool up;
    int score = 0;
    if(m[0].pitch < m[1].pitch){
        up = true;
        //cout << "up->";
    }
    else{
        up = false;
        //cout << "down->";
    }
    for(int i = 0; i < size - 1; i++){
        if(up == true){
            if(m[i].pitch < m[i+1].pitch){
                upNdown += 1;
                //cout << "score: " << score << " up+1->";
            }
            else{
                up = false;
                score += 50/upNdown;
                upNdown = 1;
              //  cout << score << " down->";
            }
        }
        
        else{
            if(m[i].pitch > m[i+1].pitch){
                upNdown += 1;
                //cout << score << " down+1->";
            }
            else{
                up = true;
                score += 50/upNdown;
                upNdown = 1;
                //cout << score << " up->";
            }
        }
        
    }
    score += 50/upNdown;
    return score;
}

int FitnessScore(Note m[]){
    int IWeight = 1; //Melody Weight
    int RWeight = 1; //Rythmic Weight
    int VWeight = 1; //Variety Weight
    return (IWeight * IScore(m, sizeof(m)) + RWeight * RScore(m, sizeof(m)) + VWeight * VScore(m, sizeof(m)));
}


int main() {
    Note melody1[] = {Note(38, 4), Note(41, 8), Note(46, 4), Note(48, 8), Note(43, 1), Note(37, 4), Note(41, 2), Note(44, 4)};  
    Note melody2[] = {Note(45, 4), Note(47, 8), Note(50, 4), Note(53, 8), Note(52, 2), Note(49, 4), Note(44, 4), Note(48, 8)};
    Note melody3[] = {Note(35, 4), Note(37, 8), Note(41, 4), Note(43, 8), Note(46, 2), Note(48, 4), Note(44, 4), Note(50, 8)};
    Note melody4[] = {Note(40, 8), Note(43, 4), Note(47, 8), Note(49, 4), Note(52, 2), Note(54, 4), Note(50, 4), Note(48, 8)};
    Note melody5[] = {Note(41, 4), Note(44, 8), Note(47, 4), Note(50, 8), Note(53, 1), Note(44, 4), Note(40, 4), Note(42, 8)};

    int size = sizeof(melody1) / sizeof(melody1[0]);  // Calculate the size of the array
    int size2 = sizeof(melody2) / sizeof(melody2[0]);
    int size3 = sizeof(melody3) / sizeof(melody3[0]);
    int size4 = sizeof(melody4) / sizeof(melody4[0]);
    int size5 = sizeof(melody5) / sizeof(melody5[0]);

    /*vector<string> melodyABC = readNotation(melody1, size);
    for (int i = 0; i < size; i++) {
        cout << melodyABC[i] << ", ";
    }*/
    // Print the melody using printNote
    int scores[4];
    scores[0] = FitnessScore(melody1);
    scores[1] = FitnessScore(melody2);
    scores[2] = FitnessScore(melody3);
    scores[3] = FitnessScore(melody4);
    scores[4] = FitnessScore(melody5);
    int high_score = scores[0];
    int high_score_index;
    //cout << "\nMELODY" << 0 << " SCORE: " << scores[0];
    for(int i = 0; i < 5; i++){
        cout << "\nMELODY" << i + 1 << " SCORE: " << scores[i];
        if(scores[high_score_index] < scores[i+1]){
            high_score = scores[i+1];
            high_score_index = i+1;
        }
    }
    cout << "\nMELODY WITH THE HIGHEST SCORE: Melody" << high_score_index + 1;
    
    /*cout << "SIZE OF MELODY: " << size << endl;
    for (int i = 0; i < size - 1; i++) {
        printNote(melody1[i]);
        cout << ", ";
    }
    printNote(melody1[size - 1]);  // Print the last note without the trailing comma
    
    // Get the notation as a vector of strings
    vector<string> ABCMelody = readNotation(melody1, size);
    
    // Print the notation
    cout << "\nABC Notation:" << endl;
    for (int i = 0; i < size - 1; i++) {
        cout << ABCMelody[i] << ", ";
    }
    cout << ABCMelody[size - 1];

    cout << "\nIScore = " << IScore(melody1, size);
    cout << "\nRScore = " << RScore(melody1, size);
    cout << "\nVScore = " << VScore(melody1, size);
    cout << "\nTotal Score " << FitnessScore(melody1);*/
    return 0;
}
