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

int FitnessScore(const vector<Note>& m) {
    int IWeight = 1, RWeight = 1, VWeight = 1;
    return ((IWeight * IScore(m) + RWeight * RScore(m) + VWeight * VScore(m)) / m.size());
}

// =========================
// Updated generateMelody with pitch range limits
// =========================
vector<Note> generateMelody(int size, int minPitch, int maxPitch) {
    srand(static_cast<unsigned int>(time(0)));
    int p, l;
    int lengths[] = {1, 2, 4, 8, 16};
    vector<Note> melody;

    p = rand() % (maxPitch - minPitch + 1) + minPitch;
    l = lengths[rand() % 5];
    melody.push_back(Note(p, l));

    for (int i = 1; i < size; i++) {
        int prevPitch = melody[i - 1].pitch;
        int pitchOffset = rand() % 25 - 12;
        p = prevPitch + pitchOffset;
        p = max(minPitch, min(maxPitch, p));
        l = lengths[rand() % 5];
        melody.push_back(Note(p, l));
    }

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
    int lengths[] = {1, 2, 4, 8, 16};
    for (int i = 0; i < melody.size(); i++) {
        if (rand() % 100 < mutationRate) {
            int pitchChange = rand() % 13 - 6;
            melody[i].pitch += pitchChange;
            melody[i].pitch = max(0, min(100, melody[i].pitch));

            int currentLengthIndex = find(begin(lengths), end(lengths), melody[i].length) - begin(lengths);
            int change = rand() % 3 - 1;
            currentLengthIndex = max(0, min(4, currentLengthIndex + change));
            melody[i].length = lengths[currentLengthIndex];
        }
    }
}

// Evolve Function with range
void evolveMelodies(int populationSize = 10, int generations = 20, int melodySize = 8) {
    srand(time(0));

    int minPitch = noteToPitch("F#", 3); // F#3
    int maxPitch = noteToPitch("E", 6);  // E6

    vector<vector<Note>> population;
    vector<int> fitnessScores;
    vector<vector<Note>> newPopulation;
    vector<Note> parent1, parent2, child, bestMelody;

    for (int i = 0; i < populationSize; i++) {
        population.push_back(generateMelody(melodySize, minPitch, maxPitch));
    }

    for (int gen = 0; gen < generations; gen++) {
        fitnessScores.clear();
        for (const auto& melody : population)
            fitnessScores.push_back(FitnessScore(melody));

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
        cout << "\nFinal Fitness Score: " << FitnessScore(bestMelody) << endl;
    }
}

int main() {
    evolveMelodies();

    cout << "\n\nPitch Tests:\n";
    string note1 = "F#"; int octave1 = 3;
    string note2 = "E"; int octave2 = 6;

    int pitch1 = noteToPitch(note1, octave1);
    int pitch2 = noteToPitch(note2, octave2);

    cout << note1 << octave1 << " -> Pitch: " << pitch1 << endl;
    cout << note2 << octave2 << " -> Pitch: " << pitch2 << endl;

    return 0;
}
