#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For time()
#include <algorithm>  // For std::find
#include <iterator>   // For std::begin and std::end
using namespace std;

// Function to convert note name (such as A, A#, Bb) and octave to pitch number (0 - 100)
int noteToPitch(string note, int octave) {
    // Normalize the note to handle enharmonics
    if (note == "Bb") note = "A#";
    if (note == "Db") note = "C#";
    if (note == "Eb") note = "D#";
    if (note == "Fb") note = "E";
    if (note == "Gb") note = "F#";
    if (note == "Ab") note = "G#";

    int pitch = -1;  // Default invalid pitch value

    // Map note name to corresponding chromatic scale number
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

    // If invalid note name is passed, return -1 to indicate error
    if (pitch == -1) {
        cerr << "Invalid note name: " << note << endl;
        return -1;
    }

    // Return the pitch number, factoring in the octave
    return pitch + (octave) * 12;
}

// Structure to hold note data
struct Note {
    int pitch;
    int length;

    Note(int p, int l) : pitch(p), length(l) {}
};

// Function to print note in the format (pitch, length)
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

// Function to convert Note array to a vector of strings and print it
void readNotation(const vector<Note>& m) {  // Pass by const reference
    vector<string> abc;  // A single vector to hold the string representations of the notes
    for (int i = 0; i < m.size(); i++) {
        abc.push_back(toLetter(m[i].pitch) + " " + toLength(m[i].length));  // Combine pitch and length into one string
    }
    for (int i = 0; i < abc.size() - 1; i++) {
        cout << abc[i];
        cout << ", ";
    }
    cout << abc[abc.size() - 1];  // Corrected this line to print the last element
}

int IScore(const vector<Note>& m) {  // Pass by const reference
    int score = 0;
    for (int i = 0; i < m.size() - 1; i++) {
        score += 12 - abs(m[i].pitch - m[i + 1].pitch) % 12;
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
                if(upNdown < 3){
                    score += 10;
                }
                else{
                    score += 10 / upNdown;
                }
                upNdown = 1;
            }
        } else {
            if (m[i].pitch > m[i + 1].pitch) {
                upNdown += 1;
            } else {
                up = true;
                if(upNdown < 3){
                    score += 10;
                }
                else{
                    score += 10 / upNdown;
                }
                upNdown = 1;
            }
        }
    }
    if(upNdown < 3){
        score += 10;
    }
    else{
        score += 10 / upNdown;
    }
    return score;
}

int FitnessScore(const vector<Note>& m) {
    int IWeight = 1; // Melody Weight
    int RWeight = 1; // Rhythmic Weight
    int VWeight = 1; // Variety Weight
    return ((IWeight * IScore(m) + RWeight * RScore(m) + VWeight * VScore(m))/m.size());
}

vector<Note> generateMelody(int size) {
    srand(static_cast<unsigned int>(time(0)));
    int p;
    int l;
    int lengths[] = {1, 2, 4, 8, 16};
    vector<Note> melody;
    
    // Generate the first note with a random pitch
    p = rand() % 100 + 1;
    l = lengths[rand() % 5];
    melody.push_back(Note(p, l));

    // Generate subsequent notes, each within ±12 steps of the previous note's pitch
    for (int i = 1; i < size; i++) {
        int prevPitch = melody[i - 1].pitch;
        
        // Ensure the next note's pitch is within 12 steps of the previous note's pitch
        p = prevPitch + (rand() % 25 - 12);  // Random number between -12 and +12
        p = max(0, min(100, p));  // Ensure pitch stays within a reasonable range (e.g., 0 to 100)

        l = lengths[rand() % 5];
        melody.push_back(Note(p, l));
    }

    return melody;
}

// =========================
// Parent Selection Function (Roulette Wheel Selection)
// =========================
vector<Note> selectParent(const vector<vector<Note>>& population, const vector<int>& fitnessScores){
    int totalFitness = 0;
    for (int score : fitnessScores){
        totalFitness += score;
    }
    int pick = rand() % totalFitness;
    int runningSum = 0;
    for (int i = 0; i < population.size(); i++){
        runningSum += fitnessScores[i];
        if (runningSum >= pick){
            return population[i];
        }
    }
    return population.back();
}

// =========================
// Crossover Function
// =========================
vector<Note> crossover(const vector<Note>& parent1, const vector<Note>& parent2){
    vector<Note> child;
    for (int i = 0; i < parent1.size(); i++){
        if (rand() % 2 == 0){
            child.push_back(parent1[i]);
        } else {
            child.push_back(parent2[i]);
        }
    }
    return child;
}

// Mutation function
void mutate(vector<Note>& melody, int mutationRate = 10) {
    int lengths[] = {1, 2, 4, 8, 16};

    for (int i = 0; i < melody.size(); i++) {
        if (rand() % 100 < mutationRate) {
            int pitchChange = rand() % 13 - 6;  // Random value between -6 and 6
            melody[i].pitch += pitchChange;

            if (melody[i].pitch < 0) {
                melody[i].pitch = 0;
            } else if (melody[i].pitch > 100) {
                melody[i].pitch = 100;
            }

            int currentLengthIndex = find(begin(lengths), end(lengths), melody[i].length) - begin(lengths);
            int change = rand() % 3 - 1;  // Random value between -1, 0, and 1
            currentLengthIndex += change;

            if (currentLengthIndex < 0) currentLengthIndex = 0;
            if (currentLengthIndex > 4) currentLengthIndex = 4;

            melody[i].length = lengths[currentLengthIndex];
        }
    }
}

// =========================
// Evolve Function
// =========================
void evolveMelodies(int populationSize = 10, int generations = 20, int melodySize = 8){
    srand(time(0));
    vector<vector<Note>> population;
    int bestIndex = 0;
    vector<int> fitnessScores;
    vector<vector<Note>> newPopulation;
    vector<Note> parent1;
    vector<Note> parent2;
    vector<Note> child;
    vector<Note> bestMelody;

    // Generate initial population
    for (int i = 0; i < populationSize; i++) {
        population.push_back(generateMelody(melodySize));
    }

    // Evolve the population over generations
    for (int gen = 0; gen < generations; gen++) {
        fitnessScores.clear();
        for (const auto& melody : population) {
            fitnessScores.push_back(FitnessScore(melody));
        }

        bestIndex = 0;
        for (int i = 1; i < fitnessScores.size(); i++) {
            if (fitnessScores[i] > fitnessScores[bestIndex]) {
                bestIndex = i;
            }
        }

        bestMelody = population[bestIndex];

        newPopulation.clear();
        newPopulation.push_back(bestMelody); // Keep the best melody

        while (newPopulation.size() < populationSize) {
            parent1 = selectParent(population, fitnessScores);
            parent2 = selectParent(population, fitnessScores);

            child = crossover(parent1, parent2);

            mutate(child);

            newPopulation.push_back(child);
        }

        population = newPopulation;

        cout << "Generation " << gen + 1 << ": Best Melody Fitness = " << fitnessScores[bestIndex] << endl;

        cout << "Best Melody: " << endl;
        readNotation(bestMelody);  // This will print the best melody notation directly

        cout << "\nFinal Fitness Score: " << FitnessScore(bestMelody) << endl;
    }
}

int main() {
    evolveMelodies();
    cout << "\n\n\n";
     // Test with F#3 and C6
     string note1 = "A";
     int octave1 = 0;
 
     string note2 = "C";
     int octave2 = 8;
 
     int pitch1 = noteToPitch(note1, octave1);
     int pitch2 = noteToPitch(note2, octave2);
 
     cout << "\nTest 1: " << note1 << octave1 << " -> Pitch: " << pitch1;
 
     cout << "\nTest 2: " << note2 << octave2 << " -> Pitch: " << pitch2;
    return 0;
}
