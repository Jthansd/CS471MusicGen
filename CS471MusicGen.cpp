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
/// =========================
// Parent Selection Function (Roulette Wheel Selection)
// =========================
// This function selects a parent melody from the population using "roulette wheel selection."
// In this method, melodies with higher fitness scores have a greater chance of being selected.
// The selection process works by:
// 1. Summing up all fitness scores to get a total fitness value.
// 2. Picking a random number within this total fitness.
// 3. Iterating through the melodies and selecting the first one whose cumulative fitness exceeds the random pick.

vector<Note> selectParent(const vector<vector<Note>>& population, const vector<int>& fitnessScores){
    int totalFitness = 0;
    // Calculate the total fitness of the population
    for (int score : fitnessScores){
        totalFitness += score;
    }
    // pick a random number between 0 and totalFitness
    // This number will determine which parent to select based on their fitness scores
    int pick = rand() % totalFitness;
    int runningSum = 0;
// iterate through the population and select a parent based on the random pick
    for (int i = 0; i < population.size(); i++){
        runningSum += fitnessScores[i];
        // If the running sum exceeds the random pick, select this melody as a parent
        // This means that the melody has a higher chance of being selected if it has a higher fitness score
        if (runningSum >= pick){
            return population[i];
        }
    }
    return population.back(); // Fallback in case of an error
}
// =========================
// Crossover Function
// =========================
// This function combines two parent melodies to create a new child melody.
// The crossover is done by randomly choosing notes from either parent.
// The process:
// 1. Iterate through each note in the melody.
// 2. With 50% probability, pick the note from parent1; otherwise, pick from parent2.
// This creates a mix of both parent melodies in the child.
vector<Note> crossover(const vector<Note>& parent1, const vector<Note>& parent2){
    
    vector<Note> child;
    for (int i = 0; i < parent1.size(); i++){
        if (rand() % 2 == 0){ // 50% chance to pick from parent1 or parent2
            child.push_back(parent1[i]);
        } else {            // 50% chance to pick from parent2
            child.push_back(parent2[i]);
        }
    }
    return child;
}
//mutation function
// This function mutates a melody by randomly changing the pitch or length of a note
void mutate(vector<Note>& melody, int mutationRate = 10){
    int lengths[] = {1, 2, 4, 8, 16};
    for(int i = 0; i < melody.size(); i++){
        if(rand() % 100 < mutationRate){
            // Randomly change pitch
            melody[i].pitch = rand() % 100 + 1; // Random pitch between 1 and 100
            melody[i].length = lengths[rand() % 5]; // Random length from the lengths array
        }
    }
}
// =========================
// Evolve Function
// =========================
// This function evolves a population of melodies by performing three main steps:
// 1. **Parent Selection**: Choose two parent melodies from the population based on their fitness scores.
// 2. **Crossover**: Combine the selected parents' melodies to create child melodies.
// 3. **Mutation**: Introduce small random changes to the child melodies to maintain genetic diversity.
void evolveMelodies(int populationSize = 10, int generations = 20, int melodySize = 8){
    srand(time(0)); // Seed the random number generator
    vector<vector<Note>> population;

    //step 1: generate initial population
    for (int i = 0; i < populationSize; i++){
        population.push_back(generateMelody(melodySize));
    }
    //step 2: evolve the population over generations
    for (int gen = 0; gen < generations; gen++){
        // evaluate fitness of each melody
        vector<int> fitnessScores;
        for (const auto& melody : population){
            fitnessScores.push_back(FitnessScore(melody));
        }
        // create a new generation
        vector<vector<Note>> newPopulation;
        // elitism: keep the best melody
        int bestIndex = 0;
        for (int i = 1; i < fitnessScores.size(); i++){
            if(fitnessScores[i] > fitnessScores[bestIndex]){
                bestIndex = i;
            }
            newPopulation.push_back(population[bestIndex]); // save the best melody
            // fill the rest of the new population
            while(newPopulation.size() < populationSize){
                // select parents using roulette wheel selection
                vector<Note> parent1 = selectParent(population, fitnessScores);
                vector<Note> parent2 = selectParent(population, fitnessScores);
                // perform crossover
                vector<Note> child = crossover(parent1, parent2);
                // mutate the child
                mutate(child);
                // add the child to the new population
                newPopulation.push_back(child);
            }
            //replace the old population with the new one
            population = newPopulation;
            //print best melody of the generation
            cout << "Generation " << gen + 1 << ": Best Melody Fitness = " << fitnessScores[bestIndex] << endl;

        }
        // Print the best melody of the generation
        int bestIndex = 0;
        for (int i = 1; i < fitnessScores.size(); i++){
            if(fitnessScores[i] > fitnessScores[bestIndex]){
                bestIndex = i;
            }
        }
        vector<Note> bestMelody = population[bestIndex];
        vector<string> bestNotation = readNotation(bestMelody);
        // display best melody
        cout << "Best Melody: " << endl;
        for (int i = 0; i < bestMelody.size() - 1; i++){
            printNote(bestMelody[i]);
            if (i < bestMelody.size() - 1) {
                cout << ", ";
            }
            // Display the ABC notation
            for(int i = 0; i < bestNotation.size() - 1; i++){
                cout << bestNotation[i] << ", ";
                if(i < bestNotation.size() - 1){
                    cout << ", ";
                }
                //show fitness score
                cout << "\nFinal Fitness Score: " << FitnessScore(bestMelody) << endl;
            }
        }
    }
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
    
    // Call the evolveMelodies function to start the evolution process
    evolveMelodies();

    return 0;
}
