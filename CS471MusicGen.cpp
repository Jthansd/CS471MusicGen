#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For time()
#include <algorithm>  // For std::find
#include <iterator>   // For std::begin and std::end
using namespace std;

int noteToPitch(string note, int octave) {
    if (note == "Bb") note = "A#";
    if (note == "Db") note = "C#";
    if (note == "Eb") note = "D#";
    if (note == "Fb") note = "E";
    if (note == "Gb") note = "F#";
    if (note == "Ab") note = "G#";

    int pitch = -1;
    if (note == "C") pitch = 0;
    else if (note == "C#") pitch = 1;
    else if (note == "D") pitch = 2;
    else if (note == "D#") pitch = 3;
    else if (note == "E") pitch = 4;
    else if (note == "F") pitch = 5;
    else if (note == "F#") pitch = 6;
    else if (note == "G") pitch = 7;
    else if (note == "G#") pitch = 8;
    else if (note == "A") pitch = 9;
    else if (note == "A#") pitch = 10;
    else if (note == "B") pitch = 11;

    if (pitch == -1) {
        cerr << "Invalid note name: " << note << endl;
        return -1;
    }

    return pitch + (octave * 12);  // Octave starts on C
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
    string note[] = {"C", "C#", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "B"};
    int indexN = pitch % 12;
    int indexO = pitch / 12;
    return note[indexN] + "(" + to_string(indexO) + ")";
}

string toLength(int l) {
    switch (l) {
        case 1: return "W";
        case 2: return "H";
        case 4: return "Q";
        case 6: return "QDot";
        case 8: return "E";
        case 16: return "S";
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
        if(m[i].pitch == m[i+1].pitch){
            score -= 5;
        }
        else if(abs(m[i].pitch - m[i + 1].pitch) % 12 < 8){
            score += 10 - abs(m[i].pitch - m[i + 1].pitch) % 12;
        }
    }
    return score;
}

int RScore(const vector<Note>& m) {
    int lengths[] = {1, 2, 4, 6, 8, 16};
    int dif, length1, length2;
    int score = 0;
    bool offbeat = false;
    int offbeatLength = 0;
    for (int i = 0; i < m.size() - 1; i++) {
        for (int j = 0; j < 5; j++) {
            if (lengths[j] == m[i].length) length1 = j;
            if (lengths[j] == m[i + 1].length) length2 = j;
        }
        dif = abs(length1 - length2);
        if(m[i].length == 8){
            switch(dif){
                case 0: score += 10; break;
                case 1: score += 8; break;
                case 2: score += 4; break;
                case 3: score -= 2; break;
                case 4: score -= 4; break;
            }
        }
        else if(m[i].length == 6){
            switch(dif){
                case 0: score += 4; break;
                case 1: score += 8; break;
                case 2: score += 10; break;
                case 3: score -= 2; break;
                case 4: score -= 4; break;
            }
        }
        else if(m[i].length == 4){
            switch(dif){
                case 0: score += 8; break;
                case 1: score += 10; break;
                case 2: score += 4; break;
                case 3: score -= 2; break;
                case 4: score -= 4; break;
            }
        }
        else{
            switch(dif){
                case 0: score -= 2; break;
                case 1: score += 0; break;
                case 2: score += 10; break;
                case 3: score += 8; break;
                case 4: score -= 4; break;
            }
        }

        if(m[i].length == 8 || m[i].length == 6){
            if(offbeat == false){
                offbeat = true;
                offbeatLength++;
            }
            else{
                offbeat = false;
                offbeatLength = 0;
            }
        }
        else{
            if(offbeat == false){
                score += 10;
            }
            else if(offbeat == true){
                if(offbeatLength < 3)
                    score += 5;
                else{
                    score -= 5;
                }
            }   
        }
    }
    return score/2;
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

bool isInKey(const Note& n, int key){
    int scale[7] = {0, 2, 4, 5, 7, 9, 11};  // Degrees of the major scale
    int pitchInScale = n.pitch % 12;
    for(int i = 0; i < 7; i++){
        if((scale[i] + key - 1) % 12 == pitchInScale){
            return true;
        } 
    }
    return false;
}

/*int KScore(const vector<Note>& m, int k) {
    int score = 0;
    int scale[7] = {0, 2, 4, 5, 7, 9, 11};  // Degrees of the major scale

    // Shift scale based on key (e.g., if key is 1, A major, the scale is [0, 2, 4, 5, 7, 9, 11] etc.)
    for (int i = 0; i < m.size(); i++) {
        if(isInKey(m[i], k) == true){
            score += 10;
        }
        else{
            score -= 10;
        }
    }
    return score;
}

*/




int FitnessScore(const vector<Note>& m, int key) {
    int IWeight = 1, RWeight = 1, VWeight = 1/*, KWeight = 1*/;
    return((IWeight * IScore(m) + RWeight * RScore(m) + VWeight * VScore(m)/* + KWeight * KScore(m, key)*/)/m.size());
}

// =========================
// Updated generateMelody with pitch range limits
// =========================
vector<Note> generateMelody(int size, int minPitch, int maxPitch, int key) {
    srand(static_cast<unsigned int>(time(0)));

    const int LSIZE = 4;
    int lengths[LSIZE] = {2, 4, 6, 8};
    vector<Note> melody;
    vector<int> scalePitches;
    int p, l;

    // --- Step 1: Build major scale semitone offsets ---
    int majorScaleSteps[7] = {0, 2, 4, 5, 7, 9, 11}; // Major scale relative to tonic
    int basePitch = key - 1;  // 0 = A, 1 = Bb, ..., 11 = G#

    // --- Step 2: Populate all scale tones within pitch range ---
    for (int octave = 0; octave < 11; octave++) {
        for (int i = 0; i < 7; i++) {
            int pitch = basePitch + majorScaleSteps[i] + 12 * octave;
            if (pitch >= minPitch && pitch <= maxPitch) {
                scalePitches.push_back(pitch);
            }
        }
    }

    // --- Sanity check: make sure we have notes to work with ---
    if (scalePitches.empty()) return melody;

    // --- Step 3: Find tonic in target octave ---
    const int targetOctave = 4;
    int tonicPitch = basePitch + (12 * targetOctave);
    if (tonicPitch < minPitch) tonicPitch = minPitch;
    if (tonicPitch > maxPitch) tonicPitch = maxPitch;

    // If that pitch isn't in the scale (due to clamping), choose closest tonic in scale
    if (find(scalePitches.begin(), scalePitches.end(), tonicPitch) == scalePitches.end()) {
        for (int pitch : scalePitches) {
            if ((pitch % 12) == (basePitch % 12)) {
                tonicPitch = pitch;
                break;
            }
        }
    }

    // --- First Note: tonic ---
    l = lengths[rand() % LSIZE];
    melody.push_back(Note(tonicPitch, l));

    // --- Intermediate Notes ---
    for (int i = 1; i < size - 1; i++) {
        // Choose a scale tone near the previous one
        int prevPitch = melody[i - 1].pitch;

        // Collect scale tones within a +/-7 semitone range
        vector<int> nearby;
        for (int pitch : scalePitches) {
            if (abs(pitch - prevPitch) <= 7) {
                nearby.push_back(pitch);
            }
        }

        // Pick randomly from nearby options
        if (!nearby.empty()) {
            p = nearby[rand() % nearby.size()];
        } else {
            p = scalePitches[rand() % scalePitches.size()];
        }

        l = lengths[rand() % LSIZE];
        melody.push_back(Note(p, l));
    }

    // --- Final Note: tonic again, in closest octave to second-last note ---
    int secondLastPitch = melody[melody.size() - 1].pitch;
    int secondLastOctave = secondLastPitch / 12;
    int bestTonic = -1;
    int bestDistance = 999;

    for (int pitch : scalePitches) {
        if ((pitch % 12) == (basePitch % 12)) {
            int octave = pitch / 12;
            int dist = abs(octave - secondLastOctave);
            if (dist < bestDistance) {
                bestTonic = pitch;
                bestDistance = dist;
            }
        }
    }

    if (bestTonic == -1) {
        bestTonic = tonicPitch;
    }

    melody.push_back(Note(bestTonic, 2));  // 2 = half note

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

void mutate(vector<Note>& melody, int minPitch, int maxPitch, int key, int mutationRate = 10) {
    int lengths[] = {2, 4, 6, 8}; // Valid rhythmic lengths
    const int maxAttempts = 100;

    // --- Build valid pitches in the major scale for the key ---
    vector<int> scalePitches;
    int basePitch = key - 1;  // A = 0, Bb = 1, ..., G# = 11
    int majorScaleSteps[7] = {0, 2, 4, 5, 7, 9, 11};

    for (int octave = 0; octave < 11; octave++) {
        for (int i = 0; i < 7; i++) {
            int pitch = basePitch + majorScaleSteps[i] + 12 * octave;
            if (pitch >= minPitch && pitch <= maxPitch) {
                scalePitches.push_back(pitch);
            }
        }
    }

    for (int i = 0; i < melody.size(); i++) {
        if (rand() % 100 < mutationRate) {
            // --- Mutate Pitch (skip first and last note) ---
            if (i != 0 && i != melody.size() - 1) {
                int attempts = 0;
                bool mutated = false;

                while (attempts < maxAttempts && !mutated) {
                    // Pick a new pitch in the key within ±6 semitones of current
                    int currentPitch = melody[i].pitch;

                    vector<int> candidates;
                    for (int p : scalePitches) {
                        if (abs(p - currentPitch) <= 6) {
                            candidates.push_back(p);
                        }
                    }

                    if (!candidates.empty()) {
                        int newPitch = candidates[rand() % candidates.size()];

                        // Check interval with neighbors
                        if ((i < melody.size() - 1 && abs(newPitch - melody[i + 1].pitch) <= 7) &&
                            (i > 0 && abs(newPitch - melody[i - 1].pitch) <= 7)) {
                            melody[i].pitch = newPitch;
                            mutated = true;
                        }
                    }

                    attempts++;
                }
            }

            // --- Mutate Length ---
            int currentLength = melody[i].length;
            int index = find(begin(lengths), end(lengths), currentLength) - begin(lengths);

            int delta = (rand() % 3) - 1;  // -1, 0, or +1
            int newIndex = max(0, min(3, index + delta));
            melody[i].length = lengths[newIndex];
        }
    }
}






// Evolve Function with range
void evolveMelodies(int populationSize, int generations, int melodySize, int key,  int minPitch = 42, int maxPitch = 69) {
    srand(time(0));

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
            mutate(child, minPitch, maxPitch, key);
            newPopulation.push_back(child);
        }

        population = newPopulation;

        cout << "\nGeneration " << gen + 1 << ": Best Melody Fitness = " << fitnessScores[bestIndex] << endl;
        cout << "Best Melody: ";
        readNotation(bestMelody);
        cout << "\nIScore = " << IScore(bestMelody) << "\nRScore = " << RScore(bestMelody) << "\nVScore = " << VScore(bestMelody) /*<< "\nKScore =" << KScore(bestMelody, key)*/ << "\nFinal Fitness Score: " << FitnessScore(bestMelody, key) << endl;
    }
}

int main() {
    int key;
    int length;
    cout << "Welcome to the AI Music Creator!\nLet's customize our music to your taste :3\nFirst off, what Key would you like your music to be in?\n(1) C major\n(2) C#/Db major\n(3) D major\n(4) D#/Eb major\n(5) E major\n(6) F major\n(7) F#/Gb major\n(8) G major\n(9) G#/Ab major\n(10) A major\n(11) A#/Bb major\n(12) B major\nEnter a number" << endl;
    cin >> key;

    string pitchWithOctave = toLetter(key - 1);
    
    int parenPos = pitchWithOctave.find('(');

    if (parenPos != -1) {
        pitchWithOctave = pitchWithOctave.substr(0, parenPos);  // Get only the pitch name (before '(')
    }
    int numNotes;
    cout << "Good choice! Now how many notes would you like to generate? (4-64)";
    cin >> numNotes;
    cout << "Nice! Generating melodies in the key of " << pitchWithOctave << " major, with " << numNotes << " notes!" << endl;

    /*vector<Note> melody1 = {Note(0, 4), Note(2, 4), Note(4, 4), Note(5, 4), Note(7, 4), Note(9, 4), Note(11, 4)};

    int I = IScore(melody1);
    int R = RScore(melody1);
    int V = VScore(melody1);
    int K = KScore(melody1, key);
    int score = FitnessScore(melody1, key);

    cout <<"\nIScore = " << I << "\nRScore = " << R << "\nVscore = " << V << "\nKscore (Should be 70) = " << K << "\nTotal Score = " << score;
    
    */

    evolveMelodies(10, 20, numNotes, key);

    /*cout << "\n\nPitch Tests:\n";
    string note1 = "F#"; int octave1 = 3;
    string note2 = "E"; int octave2 = 6;

    int pitch1 = noteToPitch(note1, octave1);
    int pitch2 = noteToPitch(note2, octave2);

    cout << note1 << octave1 << " -> Pitch: " << pitch1 << endl;
    cout << note2 << octave2 << " -> Pitch: " << pitch2 << endl;

    */
    return 0;
}
