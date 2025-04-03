#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For time()
using namespace std;

// Comment to test the new GitHub commit
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
    int indexN = pitch % 12; 
    int indexO = pitch / 12; 
    return note[indexN] + "(" + to_string(indexO) + ")"; 
}


// Function to convert note length to a string (e.g., "4" for quarter note)
string toLength(int l) {
    switch (l) {
        case 1: return "Whole";
        case 2: return "Half";
        case 4: return "Quarter";
        case 8: return "Eighth";
        case 16: return "Sixteenth";
        default: return "Unknown";
    }
}

// Function to convert Note array to a vector of strings
vector<string> readNotation(const vector<Note>& m) {  // Pass by const reference
    vector<string> abc;  // A single vector to hold the string representations of the notes
    for (int i = 0; i < m.size(); i++) {
        abc.push_back(toLetter(m[i].pitch) + " " + toLength(m[i].length));  // Combine pitch and length into one string
    }
    return abc;
}

int IScore(const vector<Note>& m) {  // Pass by const reference
    int score = 0;
    for (int i = 0; i < m.size() - 1; i++) {
        switch (abs(m[i].pitch - m[i + 1].pitch)) {
            case 0: score += 5; break; // unison or octave jump
            case 1: score += 2; break; // minor second
            case 2: score += 7; break; // major second
            case 3: score += 3; break; // minor third
            case 4: score += 6; break; // major third
            case 5: score += 7; break; // perfect fourth
            case 6: score += 2; break; // augmented fourth
            case 7: score += 6; break; // perfect fifth
            case 8: score += 2; break; // minor sixth
            case 9: score += 4; break; // major sixth
            case 10: score += 2; break; // minor seventh
            case 11: score += 2; break; // major seventh
        }
    }
    return score;
}

int RScore(const vector<Note>& m) {  // Pass by const reference
    int lengths[] = {1, 2, 4, 8, 16};
    int dif, length1, length2;
    int score = 0;
    for (int i = 0; i < m.size() - 1; i++) {
        for (int j = 0; j < 5; j++) {
            if (lengths[j] == m[i].length) {
                length1 = j;
            }
            if (lengths[j] == m[i + 1].length) {
                length2 = j;
            }
        }
        dif = abs(length1 - length2);
        switch (dif) {
            case 0: score += 10; break;
            case 1: score += 8; break;
            case 2: score += 6; break;
            case 3: score += 4; break;
            case 4: score += 2; break;
            default: break; // 0 score
        }
    }
    return score;
}

int VScore(const vector<Note>& m) {  // Pass by const reference
    int upNdown = 1;
    bool up;
    int score = 0;
    if (m[0].pitch < m[1].pitch) {
        up = true;
    } else {
        up = false;
    }
    for (int i = 0; i < m.size() - 1; i++) {
        if (up == true) {
            if (m[i].pitch < m[i + 1].pitch) {
                upNdown += 1;
            } else {
                up = false;
                score += 50 / upNdown;
                upNdown = 1;
            }
        } else {
            if (m[i].pitch > m[i + 1].pitch) {
                upNdown += 1;
            } else {
                up = true;
                score += 25 / upNdown;
                upNdown = 1;
            }
        }
    }
    score += 50 / upNdown;
    return score;
}

int FitnessScore(const vector<Note>& m) {
    int IWeight = 1; // Melody Weight
    int RWeight = 1; // Rhythmic Weight
    int VWeight = 1; // Variety Weight
    return (IWeight * IScore(m) + RWeight * RScore(m) + VWeight * VScore(m));
}

vector<Note> generateMelody(int size) {
    
    srand(static_cast<unsigned int>(time(0)));
    int p;
    int l;
    int lengths[] = {1, 2, 4, 8, 16};
    vector<Note> melody;
    for (int i = 0; i < size; i++) {
        p = rand() % 100 + 1;
        l = lengths[rand() % 5];
        melody.push_back(Note(p, l)); 
    }
    return melody;
}

int main() {
    vector<Note> melody1 = generateMelody(8);
    vector<string> melody1ABC = readNotation(melody1);
    
    // Print the melody using printNote
    cout << "Melody: ";
    for (int i = 0; i < melody1.size() - 1; i++) {
        printNote(melody1[i]);
        cout << ", ";
    }
    printNote(melody1[melody1.size() - 1]);  // Print the last note without the trailing comma
    
    // Get the notation as a vector of strings
    vector<string> ABCMelody = readNotation(melody1);
    
    // Print the notation
    cout << "\nABC Notation:" << endl;
    for (int i = 0; i < melody1ABC.size() - 1; i++) {
        cout << melody1ABC[i] << ", ";
    }
    cout << melody1ABC[melody1ABC.size() - 1];
    
    cout << "\nIScore = " << IScore(melody1);
    cout << "\nRScore = " << RScore(melody1);
    cout << "\nVScore = " << VScore(melody1);
    cout << "\nTotal Score: " << FitnessScore(melody1);
    
    return 0;
}
