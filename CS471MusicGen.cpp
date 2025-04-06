#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For time()
#include <algorithm>  // For std::find
#include <iterator>   // For std::begin and std::end
using namespace std;

// Function to convert note name and octave to pitch number
int noteToPitch(string note, int octave) {
    if (note == "Bb") note = "A#";
    if (note == "Db") note = "C#";
    if (note == "Eb") note = "D#";
    if (note == "Fb") note = "E";
    if (note == "Gb") note = "F#";
    if (note == "Ab") note = "G#";

    int pitch = -1;
    if (note == "A") pitch = 0;
    else if (note == "A#") pitch = 1;
    else if (note == "B") pitch = 2;
    else if (note == "C") pitch = 3;
    else if (note == "C#") pitch = 4;
    else if (note == "D") pitch = 5;
    else if (note == "D#") pitch = 6;
    else if (note == "E") pitch = 7;
    else if (note == "F") pitch = 8;
    else if (note == "F#") pitch = 9;
    else if (note == "G") pitch = 10;
    else if (note == "G#") pitch = 11;

    if (pitch == -1) {
        cerr << "Invalid note name: " << note << endl;
        return -1;
    }

    return pitch + (octave * 12);
}

// Note structure
struct Note {
    int pitch;
    int length;
    Note(int p, int l) : pitch(p), length(l) {}
};

// Printing helper
void printNote(Note n) {
    cout << "(" << n.pitch << ", " << n.length << ")";
}

string toLetter(int pitch) {
    string note[] = {"A", "Bb", "B", "C", "C#", "D", "Eb", "E", "F", "F#", "G", "Ab"};
    int indexN = pitch % 12;
    int indexO = pitch / 12;
    return note[indexN] + "(" + to_string(indexO) + ")";
}

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

void readNotation(const vector<Note>& m) {
    vector<string> abc;
    for (int i = 0; i < m.size(); i++) {
        abc.push_back(toLetter(m[i].pitch) + " " + toLength(m[i].length));
    }
    for (int i = 0; i < abc.size() - 1; i++) {
        cout << abc[i];
        cout << ", ";
    }
    cout << abc[abc.size() - 1];
}

int IScore(const vector<Note>& m) {
    int score = 0;
    for (int i = 0; i < m.size() - 1; i++) {
        score += 12 - abs(m[i].pitch - m[i + 1].pitch) % 12;
    }
    return score;
}

int RScore(const vector<Note>& m) {
    int lengths[] = {1, 2, 4, 8, 16};
    int dif, length1, length2;
    int score = 0;
    for (int i = 0; i < m.size() - 1; i++) {
        for (int j = 0; j < 5; j++) {
            if (lengths[j] == m[i].length) length1 = j;
            if (lengths[j] == m[i + 1].length) length2 = j;
        }
        dif = abs(length1 - length2);
        switch (dif) {
            case 0: score += 10; break;
            case 1: score += 8; break;
            case 2: score += 6; break;
            case 3: score += 4; break;
            case 4: score += 2; break;
        }
    }
    return score;
}

int VScore(const vector<Note>& m) {
    int upNdown = 1;
    bool up = m[0].pitch < m[1].pitch;
    int score = 0;
    for (int i = 0; i < m.size() - 1; i++) {
        if (up) {
            if (m[i].pitch < m[i + 1].pitch) {
                upNdown++;
            } else {
                up = false;
                score += upNdown < 3 ? 10 : 10 / upNdown;
                upNdown = 1;
            }
        } else {
            if (m[i].pitch > m[i + 1].pitch) {
                upNdown++;
            } else {
                up = true;
                score += upNdown < 3 ? 10 : 10 / upNdown;
                upNdown = 1;
            }
        }
    }
    score += upNdown < 3 ? 10 : 10 / upNdown;
    return score;
}

int KScore(const vector<Note>& m, int k) {
    int score = 0;
    int scale[7] = {0, 2, 4, 5, 7, 9, 11};  // Degrees of the major scale

    // Shift scale based on key (e.g., if key is 1, A major, the scale is [0, 2, 4, 5, 7, 9, 11] etc.)
    for (int i = 0; i < m.size(); i++) {
        int pitchInScale = m[i].pitch % 12;
        for (int j = 0; j < 7; j++) {
            // Check if pitchInScale matches any degree in the scale
            if (pitchInScale == (scale[j] + (k - 1)) % 12) {
                score += 10;
                break;  // If we find a match, no need to check other scale degrees
            }
        }
    }
    return score;
}


int FitnessScore(const vector<Note>& m, int key) {
    int IWeight = 1, RWeight = 1, VWeight = 1, KWeight = 1;
    return((IWeight * IScore(m) + RWeight * RScore(m) + VWeight * VScore(m) + KWeight * KScore(m, key))/m.size());
}

// =========================
// Updated generateMelody with pitch range limits
// =========================
vector<Note> generateMelody(int size, int minPitch, int maxPitch, int key) {
    srand(static_cast<unsigned int>(time(0)));
    int p, l;
    int const LSIZE = 3;
    int lengths[LSIZE] = {2, 4, 8};
    vector<Note> melody;
    int prevPitch;
    int pitchOffset;

    // Calculate the base pitch of the key
    int basePitch = key - 1;  // e.g., key 1 = A, key 2 = Bb, etc.

    // Randomly select an octave for the first note within the specified range
    int octave = rand() % ((maxPitch / 12) - (minPitch / 12) + 1) + (minPitch / 12); 
    p = basePitch + 12 * octave;  // Adjust base pitch with the random octave
    l = lengths[rand() % 3];  // Random length for the note
    melody.push_back(Note(p, l));  // Add the first note

    // Generate the rest of the melody
    for (int i = 1; i < size - 1; i++) {
        prevPitch = melody[i - 1].pitch;
        pitchOffset = rand() % 15 - 7;   // range: -7 to +7
        p = prevPitch + pitchOffset;
        p = max(minPitch, min(maxPitch, p));  // Ensure pitch is within the allowed range
        l = lengths[rand() % 3];  // Random length for the note
        melody.push_back(Note(p, l));  // Add the next note
    }

    // Randomly select an octave for the last note within the specified range
    octave = rand() % ((maxPitch / 12) - (minPitch / 12) + 1) + (minPitch / 12); 
    p = basePitch + 12 * octave;  // Adjust base pitch with the random octave
    l = lengths[rand() % 3];  // Random length for the note
    melody.push_back(Note(p, l));  // Add the last note

    return melody;
}



vector<Note> selectParent(const vector<vector<Note>>& population, const vector<int>& fitnessScores){
    int totalFitness = 0;
    for (int score : fitnessScores) totalFitness += score;
    int pick = rand() % totalFitness;
    int runningSum = 0;
    for (int i = 0; i < population.size(); i++) {
        runningSum += fitnessScores[i];
        if (runningSum >= pick) return population[i];
    }
    return population.back();
}

vector<Note> crossover(const vector<Note>& parent1, const vector<Note>& parent2){
    vector<Note> child;
    for (int i = 0; i < parent1.size(); i++){
        child.push_back(rand() % 2 == 0 ? parent1[i] : parent2[i]);
    }
    return child;
}

void mutate(vector<Note>& melody, int mutationRate = 10) {
    int lengths[] = {1, 2, 4, 8, 16}; // Note lengths: 1 = Whole, 2 = Half, 4 = Quarter, 8 = Eighth, 16 = Sixteenth
    int pitchChange;
    int currentLengthIndex;
    int change;
    const int maxAttempts = 100; // Maximum attempts to prevent infinite loop

    for (int i = 0; i < melody.size(); i++) {
        if (rand() % 100 < mutationRate) {
            int attempts = 0;  // Counter for the number of attempts

            // Keep trying pitchChange until it's within allowed range relative to neighbor(s)
            do {
                pitchChange = rand() % 13 - 6; // range [-6, 6]
                attempts++;
                
                // If we've attempted too many times, break out of the loop to avoid infinite looping
                if (attempts > maxAttempts) {
                    break;
                }
            } while (
                (i < melody.size() - 1 && abs((melody[i].pitch + pitchChange) - melody[i + 1].pitch) > 7) ||
                (i > 0 && abs((melody[i].pitch + pitchChange) - melody[i - 1].pitch) > 7)
            );

            // If we exited due to maxAttempts, skip this mutation for this note
            if (attempts <= maxAttempts) {
                melody[i].pitch += pitchChange;
                melody[i].pitch = max(0, min(100, melody[i].pitch)); // Clamp to [0, 100]
            }

            // Mutate duration, but do not allow index 0 or 4 (whole or sixteenth)
            currentLengthIndex = find(begin(lengths), end(lengths), melody[i].length) - begin(lengths);
            // Limit change to +/-1, but ensuring that we do not change to index 0 (Whole) or index 4 (Sixteenth)
            do {
                change = rand() % 3 - 1; // -1, 0, or 1
                currentLengthIndex = max(1, min(3, currentLengthIndex + change)); // Avoid index 0 and 4
            } while (currentLengthIndex == 0 || currentLengthIndex == 4);
            
            melody[i].length = lengths[currentLengthIndex];
        }
    }
}




// Evolve Function with range
void evolveMelodies(int populationSize, int generations, int melodySize, int key) {
    srand(time(0));

    
    int minPitch = noteToPitch("F#", 3); // F#3
    int maxPitch = noteToPitch("A", 5);  // E6

    vector<vector<Note>> population;
    vector<int> fitnessScores;
    vector<vector<Note>> newPopulation;
    vector<Note> parent1, parent2, child, bestMelody;

    for (int i = 0; i < populationSize; i++) {
        population.push_back(generateMelody(melodySize, minPitch, maxPitch, key));
    }

    for (int gen = 0; gen < generations; gen++) {
        fitnessScores.clear();
        for (const auto& melody : population)
            fitnessScores.push_back(FitnessScore(melody, key));

        int bestIndex = max_element(fitnessScores.begin(), fitnessScores.end()) - fitnessScores.begin();
        bestMelody = population[bestIndex];
        newPopulation.clear();
        newPopulation.push_back(bestMelody);

        while (newPopulation.size() < populationSize) {
            parent1 = selectParent(population, fitnessScores);
            parent2 = selectParent(population, fitnessScores);
            child = crossover(parent1, parent2);
            mutate(child);
            newPopulation.push_back(child);
        }

        population = newPopulation;

        cout << "\nGeneration " << gen + 1 << ": Best Melody Fitness = " << fitnessScores[bestIndex] << endl;
        cout << "Best Melody: ";
        readNotation(bestMelody);
        cout << "\nFinal Fitness Score: " << FitnessScore(bestMelody, key) << endl;
    }
}

int main() {
    int key;
    int length;
    cout << "Welcome to the AI Music Creator!\nLet's customize our music to your taste :3\nFirst off, what Key would you like your music to be in?\n(1) A major\n(2) A#/Bb major\n(3) B major\n(4) C major\n(5) C#/Db major\n(6) D major\n(7) D#/Eb major\n(8) E major\n(9) F major\n(10) F#/Gb major\n(11) G major\n(12) G#/Ab major\nEnter a number" << endl;
    cin >> key;

    string pitchWithOctave = toLetter(key - 1);
    
    int parenPos = pitchWithOctave.find('(');

    if (parenPos != -1) {
        pitchWithOctave = pitchWithOctave.substr(0, parenPos);  // Get only the pitch name (before '(')
    }

    cout << "Good choice! Generating melodies in the key of " << pitchWithOctave << " major" << endl;

    vector<Note> melody1 = {Note(0, 4), Note(2, 4), Note(4, 4), Note(5, 4), Note(7, 4), Note(9, 4), Note(11, 4)};

    int I = IScore(melody1);
    int R = RScore(melody1);
    int V = VScore(melody1);
    int K = KScore(melody1, key);
    int score = FitnessScore(melody1, key);

    cout <<"\nIScore = " << I << "\nRScore = " << R << "\nVscore = " << V << "\nKscore (Should be 70) = " << K << "\nTotal Score = " << score;
    
    /*
    evolveMelodies(10, 20, 16, key);

    cout << "\n\nPitch Tests:\n";
    string note1 = "F#"; int octave1 = 3;
    string note2 = "E"; int octave2 = 6;

    int pitch1 = noteToPitch(note1, octave1);
    int pitch2 = noteToPitch(note2, octave2);

    cout << note1 << octave1 << " -> Pitch: " << pitch1 << endl;
    cout << note2 << octave2 << " -> Pitch: " << pitch2 << endl;
    */

    return 0;
}
