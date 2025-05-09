#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>  
#include <ctime>  
#include <algorithm> 
#include <iterator> 
#include <utility>
#include <fstream>
using namespace std;

/*
Welcome to the CS471 MusicGen Project, group 10, Jonathan Wilson, Elijah Munoz, Lorenzo Hobbs
*/
/*======================================================================================================
int noteToPitch(string note, int octave)

This function is used to translate a pitch and octave into its numerical value.
For example noteToPitch("C", 0) would return the value 0. noteToPitch("C", 1) would return the value 12

string note = note name I.E. "A", "B", "C", etc.

int octave = value indicating octave of the note. Although this value can techinically be any number, 
music generally only takes place between octave 0 and 8

=========================================================================================================*/
/*==============================================================================
struct Note

We use a struct as a structure for notes to be created with. Notes hold the values "pitch" and "length"
which represent the Notes pitch and note length respectively

int pitch = holds any value although realistically should only be assigned a value 0 - 100. 0 represents pitch C(0), 100 represents E(8)

int length = a value representing note length. Values include "1, 2, 4, 6, 8, 16" to represent whole notes thorugh half notes

=================================================================================*/
struct Note {
    int pitch;
    int length;
    Note(int p, int l) : pitch(p), length(l) {}
};

string xmlLength(int l) {
    switch (l) {
        case 1: return "whole";
        case 2: return "half";
        case 4: return "quarter";
        case 6: return "quarter"; // dotted quarter
        case 8: return "eighth";
        case 16: return "16th";
        default: return "quarter";
    }
}

void exportToMusicXML(const vector<Note>& melody, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file for writing.\n";
        return;
    }

    file << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
    file << "<!DOCTYPE score-partwise PUBLIC \"-//Recordare//DTD MusicXML 3.1 Partwise//EN\" \"http://www.musicxml.org/dtds/partwise.dtd\">\n";
    file << "<score-partwise version=\"3.1\">\n";
    file << "<part-list>\n";
    file << "  <score-part id=\"P1\">\n";
    file << "    <part-name>Music</part-name>\n";
    file << "  </score-part>\n";
    file << "</part-list>\n";
    file << "<part id=\"P1\">\n";
    file << "  <measure number=\"1\">\n";
    file << "    <attributes>\n";
    file << "      <divisions>1</divisions>\n";
    file << "      <key><fifths>0</fifths></key>\n";
    file << "      <time><beats>4</beats><beat-type>4</beat-type></time>\n";
    file << "      <clef><sign>G</sign><line>2</line></clef>\n";
    file << "    </attributes>\n";

    for (const Note& n : melody) {
        int pitch = n.pitch;
        int octave = pitch / 12;
        int index = pitch % 12;
        string step;
        switch(index) {
            case 0: step = "C"; break;
            case 1: step = "C"; break; // C#
            case 2: step = "D"; break;
            case 3: step = "D"; break; // Eb
            case 4: step = "E"; break;
            case 5: step = "F"; break;
            case 6: step = "F"; break; // F#
            case 7: step = "G"; break;
            case 8: step = "G"; break; // Ab
            case 9: step = "A"; break;
            case 10: step = "A"; break; // Bb
            case 11: step = "B"; break;
        }

        string alter = "";
        if (index == 1 || index == 3 || index == 6 || index == 8 || index == 10) alter = "1";

        file << "    <note>\n";
        file << "      <pitch>\n";
        file << "        <step>" << step << "</step>\n";
        if (!alter.empty()) file << "        <alter>" << alter << "</alter>\n";
        file << "        <octave>" << octave << "</octave>\n";
        file << "      </pitch>\n";
        file << "      <duration>1</duration>\n";
        file << "      <type>" << xmlLength(n.length) << "</type>\n";
        if (n.length == 6) file << "      <dot/>\n"; // dotted quarter
        file << "    </note>\n";
    }

    file << "  </measure>\n";
    file << "</part>\n";
    file << "</score-partwise>\n";
    file.close();
}



int noteToPitch(string note, int octave) {
    //normalize pitch names. I.E. Bb and A# are enharmonics thus musically the same, 
    //however for this function we will only work with sharps(#) rather than flats (b)
    if (note == "Bb") note = "A#";
    if (note == "Db") note = "C#";
    if (note == "Eb") note = "D#";
    if (note == "Fb") note = "E";
    if (note == "Gb") note = "F#";
    if (note == "Ab") note = "G#";

    int pitch = -1;//used for exception handling if the pitch entered was invalid

    //search for the pitch using the note name and assign it its base value
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

    //pitch was an invalid entry (Not A - G)
    if (pitch == -1) {
        cerr << "Invalid note name: " << note << endl;
        return -1;
    }

    //calculates the pitch numerical value based on its note name and octave
    return pitch + (octave * 12);  
}




/*==============================================================================
void printNote(Note n)

void function used to print notes in the format (pitch, length) I.E. (24, 8)

Note n is passed and is a single Note using the note structure

=================================================================================*/
void printNote(Note n) {
    cout << "(" << n.pitch << ", " << n.length << ")";
}

/*==============================================================================
string toLetter(int pitch)

Is passed the value of pitch fromm a Note and returns the pitch in english in the format "Note name (Octave)" I.E. "C(3)"

int pitch = holds any value although realistically should only be assigned a value 0 - 100. 0 represents pitch C(0), 100 represents E(8)

=================================================================================*/
string toLetter(int pitch) {
    string note[] = {"C", "C#", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "B"};
    int indexN = pitch % 12;//find the pitch index in the note array
    int indexO = pitch / 12;//finds the octave value
    return note[indexN] + "(" + to_string(indexO) + ")";
}

/*==============================================================================
string toLength(int l)

Passed the numerical value of the length of a note and returns the length of the note in english

int l = a value representing note length. Values include "1, 2, 4, 6, 8, 16" to represent whole notes thorugh half notes

=================================================================================*/
string toLength(int l) {
    switch (l) {
        case 1: return "W";//Whole note
        case 2: return "H";//Half note
        case 4: return "Q";//Quarter note
        case 6: return "QDot";//Dotted Quarter note
        case 8: return "E";//Eighth note
        case 16: return "S";//Sixteenth note
        default: return "Unknown";//None of these
    }
}

/*==============================================================================
void readNotation(const vector<Note>& m)

converts all the notes in a single melody from "pitch" and "length" numerical values to english

const vector<Note>& m = a vector of notes
=================================================================================*/
void readNotation(const vector<Note>& m) {
    vector<string> abc;//new vector used to temporarily store the Notes in english
    for (int i = 0; i < m.size(); i++) {//cycles through all the notes in the melody
        abc.push_back(toLetter(m[i].pitch) + " " + toLength(m[i].length));//calls toLetter, and toLength to translate aspects of Note into english
    }
    for (int i = 0; i < abc.size() - 1; i++) {//cycles through the vector of english notes and displays them
        cout << abc[i];
        cout << ", ";
    }
    cout << abc[abc.size() - 1];
}

/*==============================================================================
int IScore(const vector<Note>& m) 

First of 3 scoring functions. IScore assigns a score to any given melody based
on its Intervals between notes. 

Gives a greater score to notes that are closer together in pitch

const vector<Note>& m = a vector of notes
=================================================================================*/
int IScore(const vector<Note>& m) {
    int score = 0;//initialize score
    for (int i = 0; i < m.size() - 1; i++) {//cycles through the entire melody
        if(m[i].pitch == m[i+1].pitch){//if two pitches in a row are the same, we penalize the melody
            score -= 0;
        }
        else if(abs(m[i].pitch - m[i + 1].pitch) % 12 < 8){//otherwise we increase the score based on the difference between the two notes
            score += 10 - abs(m[i].pitch - m[i + 1].pitch) % 12;
        }
    }
    return score;
}

/*==============================================================================
int RScore(const vector<Note>& m)

Next scoring function gives the melody a score based on its Rythm.
Gives a greater score to notes that are closer together in note length
Additionally, scores wether the melody has lots of notes that are syncopated (offbeat). 
Notes arent penalized for being offbeat until they are offbeat for more than 3 notes.

Special instruction:
In testing, we found that melodies were often generated with a lot of the same 
note lengths in a row, which is ok with faster notes (quarters or eighth notes),
but sound weird with long notes (Whole notes, hald notes) So this function was
updated to give a personilized score note only depending on what the difference in
note length is, but also what the starting length was. This way, melodies will not be punished
for having repeated short notes but will be punished for repeated long notes

const vector<Note>& m = a vector of notes
=================================================================================*/
int RScore(const vector<Note>& m) {
    int lengths[] = {1, 2, 4, 6, 8, 16};//used to find the index of the note length of the note currently being checked
    int dif, length1, length2;//dif stores the difference in note length based on their index in the lengths[]. Lengths1 & 2 are used to temporarily store the index of the two notes being evaluated
    int score = 0;//initialize score
    bool offbeat = false;//boolean variable used to determine iff the current melody is offbeat
    int offbeatLength = 0;//tracks the amount of notes that the melody has been  offbeat for
    for (int i = 0; i < m.size() - 1; i++) {//cycles through the entire melody
        for (int j = 0; j < 5; j++) {//cycles through the length of the lengths[] array
            if (lengths[j] == m[i].length) length1 = j;//if the length of the current note in the melody matches a length in the lengths[], we store the index into length1
            if (lengths[j] == m[i + 1].length) length2 = j;//if tthe length of the next note in the melody matches a length in lengths[], we store the index in length2
        }
        dif = abs(length1 - length2);//find the difference in lengths[] indexes
        if(m[i].length == 8){//if the note length is an eighth note
            switch(dif){//scoring is based on what the intial note length is
                case 0: score += 10; break;//same length
                case 1: score += 8; break;//length is 1 note length away
                case 2: score += 4; break;//2 lengths away
                case 3: score -= 2; break;//3 lengths away
                case 4: score -= 4; break;//4 lengths away
            }
        }
        else if(m[i].length == 6){//dotted quarter note
            switch(dif){
                case 0: score += 4; break;
                case 1: score += 8; break;
                case 2: score += 10; break;
                case 3: score -= 2; break;
                case 4: score -= 4; break;
            }
        }
        else if(m[i].length == 4){//quarter note
            switch(dif){
                case 0: score += 8; break;
                case 1: score += 10; break;
                case 2: score += 4; break;
                case 3: score -= 2; break;
                case 4: score -= 4; break;
            }
        }
        else{
            switch(dif){//anything longer than a quarter note
                case 0: score -= 2; break;
                case 1: score += 0; break;
                case 2: score += 10; break;
                case 3: score += 8; break;
                case 4: score -= 4; break;
            }
        }

        if(m[i].length == 8 || m[i].length == 6){//if note length is 8th note or dotted quarter, then an offbeat is started (these are the only notes that can create syncopation)
            if(offbeat == false){//if offbeat was previously false
                offbeat = true;//set offbeat to true
                offbeatLength++;//offbeat length counter increases
            }
            else{
                offbeat = false;//if we were already on an offbeat, then another offbeat note would put us back on beat
                offbeatLength = 0;//reset offbeat counter
            }
        }
        else{//if we are not given an offbeat note
            if(offbeat == false){//and if offbeat was already false
                score += 10;//increase the score for being on beat
            }
            else if(offbeat == true){//if we were on offbeats, we stay offbeat
                if(offbeatLength < 3)//for as long as we have only been offbeat for 2 or less notes
                    score += 5;//increase the score
                else{
                    score -= 5;//if we were offbeat for longer than 2 notes, penalize the melody
                }
            }   
        }
    }
    return score/2; //since this is essentially 2 scoring functions, we divide the score in half or else Rscore would HEAVILY influence overall score
}

/*==============================================================================
int VScore(const vector<Note>& m)

Last scoring function used to calculate final score.
VScore calculates score based on variety of the melody. Specifically, the more that the melody
rises and falls in pitch, the higher score that we give. This discourages stagnation in notes,
as well as promotes lots of variety in melody.

const vector<Note>& m = a vector of notes
=================================================================================*/
int VScore(const vector<Note>& m) {
    int upNdown = 1;//upNdown tracks for how many notes the melody has been moving up or down
    bool up = m[0].pitch < m[1].pitch;//boolean value is true when piitch is rising and false when it is falling
    int score = 0;//initialize score
    for (int i = 0; i < m.size() - 1; i++) {//cycles through melody
        if (up) {//if we are previously moving up
            if (m[i].pitch < m[i + 1].pitch) {//if current notes are also moving up
                upNdown++;//increase counter
            } else {//if we switch directions
                up = false;//up is false
                score += upNdown < 3 ? 10 : 10 / upNdown;//score increases depending on how many times we had previosly moved up before now
                upNdown = 1;//reset upNdown
            }
        } else {//if previously moving down
            if (m[i].pitch > m[i + 1].pitch) {//if pitch is still moving down
                upNdown++;//increase upNdown
            } else {//if pitch switched directions
                up = true;//up is true
                score += upNdown < 3 ? 10 : 10 / upNdown;//score increases depending on how many times previously moved down
                upNdown = 1;//reset upNdown
            }
        }
        
    }
    score += upNdown < 3 ? 10 : 10 / upNdown;
    return score;
}

/*==============================================================================
bool isInKey(const Note& n, int key)

This function is used to determine if a given note is in the given key signature

const vector<Note>& m = a vector of notes
int key = value of the key signature to be used (1 - 12)
=================================================================================*/
bool isInKey(const Note& n, int key){
    int scale[7] = {0, 2, 4, 5, 7, 9, 11};//degrees of the major scale
    int pitchInScale = n.pitch % 12;//piitch is converted to its base value
    for(int i = 0; i < 7; i++){//loops through all the values in scale[] to find which scale degree is used
        if((scale[i] + key - 1) % 12 == pitchInScale){//if the scale degree is found
            return true;//the pitch is in key
        } 
    }
    return false; //else the pitch is not in key
}

//unused scoring function. Gives a score to a melody depending on how many notes in the melody are in the given key sig
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



/*==============================================================================
int FitnessScore(const vector<Note>& m, int key)

This function calls all the scoring functions and returns the total fitness score of a melody

const vector<Note>& m = a vector of notes
int key = value of the key signature to be used (1 - 12)
=================================================================================*/
int FitnessScore(const vector<Note>& m, int key) {
    int IWeight = 1, RWeight = 1, VWeight = 1/*, KWeight = 1*/; // weight is an optional variable we can use to increase the importance of any of the scoring functions
    return((IWeight * IScore(m) + RWeight * RScore(m) + VWeight * VScore(m)/* + KWeight * KScore(m, key)*/));
}

/*==============================================================================
vector<Note> generateMelody(int size, int minPitch, int maxPitch, int key)

This function randomly generates a melody by following a few rules/parameters.
Generates melodies using notes that are within a given range, and key sig.

int size = amount of notes desired to be generated
int min pitch = minimum pitch value desired to be generated from
int maxPitch = max pitch value desired to be generated from
int key = value of the key signature to be used (1 - 12)
=================================================================================*/
vector<Note> generateMelody(int size, int minPitch, int maxPitch, int key) {
    srand(static_cast<unsigned int>(time(0)));

    const int LSIZE = 4;
    int lengths[LSIZE] = {2, 4, 6, 8};//picks from 4 kinds of note lengths (Whole notes and sixteenth notes were left out)
    vector<Note> melody;//create a vector to store the melody
    vector<int> scalePitches;//used to store all the notes we are able to choose from
    int p, l;//stores pitch and note length

    int majorScaleSteps[7] = {0, 2, 4, 5, 7, 9, 11}; // values represent the scale degrees in a major scale
    int basePitch = key - 1; //base pitch of notes to be generated is the value of the key sig - 1

    for (int octave = 0; octave < 9; octave++) { //loops until we reach octave 8
        for (int i = 0; i < 7; i++) {//loops until all the scale degrees have been searched through
            int pitch = basePitch + majorScaleSteps[i] + 12 * octave;//base pitch plus the major scale degree and octave is used to determine pitch
            if (pitch >= minPitch && pitch <= maxPitch) {//if the pitch is within the scope
                scalePitches.push_back(pitch);//add it to scale pitches to be generated with
            }
        }
    }

    if (scalePitches.empty()) return melody;//checks to confirm that there are any notes to be generated from

    const int targetOctave = 4;//base octave starts at 4 to generate the first note in this octave
    int tonicPitch = basePitch + (12 * targetOctave);//tonic (or base/initial) pitch is calculated based on the base pitch and octave
    if (tonicPitch < minPitch) tonicPitch = minPitch;//checks that the toniic pitch is within scope
    if (tonicPitch > maxPitch) tonicPitch = maxPitch;


    if (find(scalePitches.begin(), scalePitches.end(), tonicPitch) == scalePitches.end()) {//if the tonic pitch was outside of scope, we find the nearest note to it
        for (int pitch : scalePitches) {
            if ((pitch % 12) == (basePitch % 12)) {
                tonicPitch = pitch;
                break;
            }
        }
    }
    
    l = lengths[rand() % LSIZE]; //first note generated is the tonic pitch but with a random note length
    melody.push_back(Note(tonicPitch, l));

    for (int i = 1; i < size - 1; i++) {//loops for the rest of the notes until the last note
        int prevPitch = melody[i - 1].pitch;//stores the value of the previous pitch

        vector<int> nearby;
        for (int pitch : scalePitches) {//loops to find pitches that are within the key signature and within 7 semitones away
            if (abs(pitch - prevPitch) <= 7) {
                nearby.push_back(pitch);
            }
        }

        //picks randomly from nearby options
        if (!nearby.empty()) {
            p = nearby[rand() % nearby.size()];
        } else {
            p = scalePitches[rand() % scalePitches.size()];
        }

        l = lengths[rand() % LSIZE];
        melody.push_back(Note(p, l));
    }

    //Final note is generated based on tonic pitch but with an octave closest to the octave of the second to last pitch
    int secondLastPitch = melody[melody.size() - 1].pitch;
    int secondLastOctave = secondLastPitch / 12;
    int bestTonic = -1;
    int bestDistance = 999;

    //loops to find a a usable pitch that is closest to the second to last note and in a good octave
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

    melody.push_back(Note(bestTonic, 2));// 2 = half note

    return melody;
}


/*==============================================================================
vector<Note> selectParent(const vector<vector<Note>>& population, const vector<int>& fitnessScores)

This function is used to select the most fit parent melody for crossover to create children melodies

const vector<vector<Note>>& population = a vector of melodies that are cycled through to determine the most fit parent
const vector<int>& fitnessScores = a vector of the melodies fitness scores to determine the most fit parent
=================================================================================*/
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

pair<vector<Note>, vector<Note>> crossover(const vector<Note>& parent1, const vector<Note>& parent2, int crossoverPoint) {
    vector<Note> child1, child2;

    for (int i = 0; i < parent1.size(); i++) {
        if (i <= crossoverPoint) {
            child1.push_back(parent1[i]);
            child2.push_back(parent2[i]);
        } else {
            child1.push_back(parent2[i]);
            child2.push_back(parent1[i]);
        }
    }

    return {child1, child2};
}

/*==============================================================================
void mutate(vector<Note>& melody, int minPitch, int maxPitch, int key, int mutationRate = 10)

This function randomly picks notes within a melody to mutate. Meaning that the pitch or note length may change
This introduces random change in to a population to prevent stagnation and increase fitness of the melodies.

vector<Note>& melody = vector of notes representing a melody
int min pitch = minimum pitch value desired to be generated from
int maxPitch = max pitch value desired to be generated from
int key = value of the key signature to be used (1 - 12)
=================================================================================*/
void mutate(vector<Note>& melody, int minPitch, int maxPitch, int key, int mutationRate = 10) {
    int lengths[] = {2, 4, 6, 8}; // Valid rhythmic lengths
    const int maxAttempts = 100;//a counter for how many attempts are allowed at finding a valid pitch for mutation

    //Builds valid pitches in the major scale for the key
    vector<int> scalePitches;
    int basePitch = key - 1;// C = 0, Db = 1, ...
    int majorScaleSteps[7] = {0, 2, 4, 5, 7, 9, 11};

    for (int octave = 0; octave < 9; octave++) {//loops through valid octaves to determine all possibe notes to choose from
        for (int i = 0; i < 7; i++) {
            int pitch = basePitch + majorScaleSteps[i] + 12 * octave;
            if (pitch >= minPitch && pitch <= maxPitch) {
                scalePitches.push_back(pitch);
            }
        }
    }

    for (int i = 0; i < melody.size(); i++) {//loops through the entire melody
        if (rand() % 100 < mutationRate) {
            //Mutate Pitch (skip first and last note)
            if (i != 0 && i != melody.size() - 1) {
                int attempts = 0;
                bool mutated = false;

                while (attempts < maxAttempts && !mutated) {
                    //pick a new pitch in the key within 6 semitones of current
                    int currentPitch = melody[i].pitch;

                    vector<int> candidates;
                    for (int p : scalePitches) {
                        if (abs(p - currentPitch) <= 6) {
                            candidates.push_back(p);
                        }
                    }

                    if (!candidates.empty()) {
                        int newPitch = candidates[rand() % candidates.size()];

                        //Check interval with neighbors
                        if ((i < melody.size() - 1 && abs(newPitch - melody[i + 1].pitch) <= 7) &&
                            (i > 0 && abs(newPitch - melody[i - 1].pitch) <= 7)) {
                            melody[i].pitch = newPitch;
                            mutated = true;
                        }
                    }

                    attempts++;
                }
            }

            //mutate length
            int currentLength = melody[i].length;
            int index = find(begin(lengths), end(lengths), currentLength) - begin(lengths);

            int delta = (rand() % 3) - 1;  // -1, 0, or +1
            int newIndex = max(0, min(3, index + delta));
            melody[i].length = lengths[newIndex];
        }
    }
}





/*==============================================================================
void evolveMelodies(int populationSize, int generations, int melodySize, int key,  int minPitch = 42, int maxPitch = 69)

This function starts the music generation process. It calls generate melody, mutate, select parent and crossover in order
to evaulate meodies, mutate them, and create new populations

int populationSize = how many melodies to generate
int generations = how many generations of melodies to cycle through
int melodySize = how many notes each melody should have
int key = value of the key signature to be used
int min Pitch = min value of pitch to be selected. For this program we will use 42 because it is the lowest pitch a trumpet can play
int max Pitch = max value of pitch to be selected. For this program we will use 69 because that is the highest note in our comfortable playing range from trumpet
=================================================================================*/
vector<Note> evolveMelodies(int populationSize, int generations, int melodySize, int key, int minPitch = 42, int maxPitch = 69) {

    srand(time(0));

    vector<vector<Note>> population;
    vector<int> fitnessScores;
    vector<vector<Note>> newPopulation;
    vector<Note> parent1, parent2, bestMelody;

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
        newPopulation.push_back(bestMelody); // Elitism: keep best melody

        while (newPopulation.size() < populationSize) {
            vector<Note> parent1 = selectParent(population, fitnessScores);
            vector<Note> parent2 = selectParent(population, fitnessScores);

            // Fast check: If fitness differs, melodies must differ
            bool parentsDiffer = (FitnessScore(parent1, key) != FitnessScore(parent2, key));

            // If fitness is same, compare note-by-note
            if (!parentsDiffer) {
                parentsDiffer = parent1.size() != parent2.size();
                for (size_t i = 0; !parentsDiffer && i < parent1.size(); i++) {
                    if (parent1[i].pitch != parent2[i].pitch || parent1[i].length != parent2[i].length) {
                        parentsDiffer = true;
                    }
                }
            }

            if (parentsDiffer) {
                cout << "\n--- Parent 1 (Fitness: " << FitnessScore(parent1, key) << ") ---\n";
                readNotation(parent1);
                cout << "\n--- Parent 2 (Fitness: " << FitnessScore(parent2, key) << ") ---\n";
                readNotation(parent2);
            }

            /* Rest of your existing code (crossover/mutation) remains unchanged */
            int crossoverPoint = 1 + rand() % (melodySize - 2);
            auto [child1, child2] = crossover(parent1, parent2, crossoverPoint);
            mutate(child1, minPitch, maxPitch, key);
            mutate(child2, minPitch, maxPitch, key);

            if (parentsDiffer) {
                cout << "\n>>> Child 1 <<<\n";
                readNotation(child1);
                cout << "\n>>> Child 2 <<<\n";
                readNotation(child2);
                cout << "\n-------------------";
            }

            newPopulation.push_back(child1);
            if (newPopulation.size() < populationSize) {
                newPopulation.push_back(child2);
            }
        }
        population = newPopulation;

        cout << "\nGeneration " << gen + 1 << ": Best Melody Fitness = " << fitnessScores[bestIndex] << endl;
        cout << "Best Melody: ";
        readNotation(bestMelody);
        cout << "\nIScore = " << IScore(bestMelody)
             << "\nRScore = " << RScore(bestMelody)
             << "\nVScore = " << VScore(bestMelody)
             << "\nFinal Fitness Score: " << FitnessScore(bestMelody, key) << endl;
    }
    exportToMusicXML(bestMelody, "best_melody.musicxml");
    cout << "Exported best melody to best_melody.musicxml\n";
    std::cout << "Returning melody with " << bestMelody.size() << " notes." << std::endl;

    return bestMelody;


}

int main() {
    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(640, 480, "AlgoRhythm", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale = 1.3f;  
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    const char* keys[] = {
        "C major", "C#/Db major", "D major", "D#/Eb major", "E major",
        "F major", "F#/Gb major", "G major", "G#/Ab major", "A major",
        "A#/Bb major", "B major"
    };
    int selectedKey = 0;
    int numNotes = 8;
    bool generated = false;
    vector<Note> generatedNotes; // Store the best melody here

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame(); 

        ImGui::SetNextWindowSize(ImVec2(720, 480), ImGuiCond_FirstUseEver);
        ImGui::Begin(" AlgoRhythm - AI Melody Generator");
        
        ImGui::TextColored(ImVec4(1, 0.7f, 0.2f, 1), "Compose a unique melody using AI!");
        ImGui::Spacing();
        ImGui::Separator();
        
        ImGui::BeginGroup();  // LEFT SIDE - Controls
        ImGui::Text(" Key Selection");
        ImGui::Combo("##key", &selectedKey, keys, IM_ARRAYSIZE(keys));
        ImGui::Spacing();
        
        ImGui::Text(" Number of Notes");
        ImGui::SliderInt("##notes", &numNotes, 4, 64);
        ImGui::Spacing();
        
        if (ImGui::Button(" Generate Melody", ImVec2(200, 0))) {
            generatedNotes = evolveMelodies(10, 50, numNotes, selectedKey + 1);
            std::cout << "Generated notes: " << generatedNotes.size() << std::endl;
            generated = true;
        }
        ImGui::SameLine();
        if (ImGui::Button(" Export XML", ImVec2(120, 0)) && !generatedNotes.empty()) {
            exportToMusicXML(generatedNotes, "best_melody.musicxml");
        }
        ImGui::EndGroup();
        
        ImGui::SameLine();
        
        ImGui::BeginGroup();  // RIGHT SIDE - Output
        ImGui::Text("🎼 Melody Output");
        ImGui::Separator();
        
        if (!generatedNotes.empty()) {
            for (const Note& n : generatedNotes) {
                string noteStr = toLetter(n.pitch) + " (" + toLength(n.length) + ")";
                ImGui::BulletText("%s", noteStr.c_str());
            }
        } else {
            ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "No melody generated yet.");
        }
        ImGui::EndGroup();
        
        ImGui::End();
        
        ImGui::Render();

        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        glViewport(0, 0, w, h);
        glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
