// ===== HEADER FILES =====
#include <iostream>      // For input/output (cin, cout)
#include <fstream>       // For file input/output (ifstream, ofstream)
#include <sstream>       // For string stream parsing
#include <cstring>       // For string functions like strncpy
#include <cstdlib>       // For memory management (malloc, realloc, free)
#include <string>        // For std::string class
#include <algorithm>     // For sorting and other algorithms
#include <iomanip>       // For std::setw, std::left (aligned table columns)
 
// ===== TEAM STRUCTURE =====
// This struct stores information about ONE team
// Example: Team with ID=101, Name="Dragons", Score=250, Missions=5
typedef struct {
    int id;              // Team ID (must be unique)
    char name[40];       // Team name (max 39 characters + null terminator)
    int score;           // Total points earned from missions
    int missions;        // Total number of missions completed
} Team;
 
// ===== GLOBAL VARIABLES =====
// These manage a dynamic array of teams (like a flexible list that grows/shrinks)
Team* teams = NULL;      // Pointer to the array of teams (starts empty: NULL)
int size = 0;            // Current number of teams in the array
int capacity = 0;        // Maximum teams the array can hold (grows as needed)
 
// ===== FIND TEAM BY ID =====
// Purpose: Search for a team by its ID number
// Returns: The position (index) of the team in the array, OR -1 if not found
// Example: If we search for ID 101 and find it at position 0, return 0
int findTeamIndex(int id) {
    for (int i = 0; i < size; i++) {
        if (teams[i].id == id) return i;  // Found it! Return position
    }
    return -1;  // Not found, return -1
}
 
// ===== DYNAMIC ARRAY MEMORY MANAGEMENT =====
// Purpose: Make sure the array has enough space for new teams
// Strategy: Double the capacity when we run out of space (1→2→4→8→16...)
// This is more efficient than resizing by 1 each time
// Example: If capacity is 5 and we need 6, double it to 10
bool ensureCapacity(int required) {
    if (required <= capacity) return true;  // Already have enough space
 
    // Calculate new capacity by doubling
    int newCapacity = (capacity == 0) ? 1 : capacity;  // Start with 1 if empty
    while (newCapacity < required) {
        newCapacity *= 2;  // Keep doubling until it's big enough
    }
 
    // Resize the memory block using realloc
    Team* temp = (Team*)realloc(teams, newCapacity * sizeof(Team));
    if (temp == NULL) {
        return false;  // Reallocation failed, memory not changed
    }
 
    teams = temp;          // Update pointer to new memory location
    capacity = newCapacity;  // Update capacity
    return true;
}
 
// ===== VALIDATE TEAM DATA =====
// Purpose: Check if team data is valid before adding/modifying
// Checks:
//   - ID must be positive (not 0 or negative)
//   - Name must not be empty and must fit in the 40-char array
//   - Score and missions must be non-negative
// Returns: true if valid, false if any check fails
// Example: validateTeamData(101, "Dragons", 0, 0) → true (all valid)
bool validateTeamData(int id, const std::string& name, int score, int missions) {
    if (id <= 0) return false;  // ID must be positive
    if (name.empty() || name.size() >= sizeof(Team::name)) return false;  // Name must fit
    if (score < 0 || missions < 0) return false;  // No negative scores/missions
    return true;
}
 
// ===== ADD NEW TEAM =====
// Purpose: Register a new team in the system
// Parameters: id, name, score (default=0), missions (default=0)
// Returns: true if added successfully, false if failed
// Checks before adding:
//   1. Validate data (using validateTeamData)
//   2. Check if ID already exists (no duplicates)
//   3. Ensure array has space (grow if needed)
// Example: addTeam(101, "Dragons") → Creates team with ID=101, Name="Dragons", Score=0, Missions=0
bool addTeam(int id, const std::string& name, int score = 0, int missions = 0) {
    if (!validateTeamData(id, name, score, missions)) {
        return false;  // Invalid data
    }
 
    if (findTeamIndex(id) != -1) {
        return false;  // Team with this ID already exists (duplicate)
    }
 
    if (!ensureCapacity(size + 1)) {
        return false;  // No memory available
    }
 
    // Create a new team record
    Team candidate{};  // Initialize all fields to 0/empty
    candidate.id = id;
    // Safe string copy: Copy up to 39 chars, then add null terminator
    strncpy(candidate.name, name.c_str(), sizeof(candidate.name) - 1);
    candidate.name[sizeof(candidate.name) - 1] = '\0';
    candidate.score = score;
    candidate.missions = missions;
 
    // Add to array
    teams[size] = candidate;
    size++;
    return true;
}
 
// ===== RECORD A MISSION =====
// Purpose: Add points to a team's score and increment mission count
// Parameters: team ID, points earned (1-100)
// Returns: true if recorded, false if team not found or points invalid
// Example: recordMission(101, 50) → Team 101 gets +50 points, mission count +1
bool recordMission(int id, int points) {
    if (points < 1 || points > 100) return false;  // Points must be between 1-100
 
    int index = findTeamIndex(id);
    if (index == -1) return false;  // Team not found
 
    teams[index].score += points;    // Add points to score
    teams[index].missions++;         // Increment mission count
    return true;
}
 
// ===== DELETE A TEAM =====
// Purpose: Remove a team from the array
// Strategy: Shift all teams after the deleted one forward by one position
// Returns: true if deleted, false if team not found
// Example: If array is [Team 101, Team 102, Team 103] and we delete 102,
//          result is [Team 101, Team 103] and size becomes 2
bool deleteTeam(int id) {
    int index = findTeamIndex(id);
    if (index == -1) return false;  // Team not found
 
    // Shift all teams after this position forward
    for (int i = index; i < size - 1; i++) {
        teams[i] = teams[i + 1];
    }
 
    size--;  // Decrease count
    return true;
}
 
// ===== SORT LEADERBOARD =====
// Purpose: Sort teams by score (highest first), then by missions (most first)
// Algorithm: Bubble sort (repeatedly compare adjacent teams and swap if needed)
// Sorting rules:
//   1. Higher score comes first (descending)
//   2. If scores are equal, more missions comes first
// Example: Before: [Team A: 100pts/2missions, Team B: 150pts/1mission]
//          After:  [Team B: 150pts/1mission, Team A: 100pts/2missions]
void sortLeaderboard() {
    // Bubble sort algorithm: multiple passes through the array
    for (int i = 0; i < size - 1; i++) {
        // Compare adjacent teams
        for (int j = 0; j < size - i - 1; j++) {
            bool shouldSwap = false;
 
            // Rule 1: If current score is LESS than next, swap (we want higher scores first)
            if (teams[j].score < teams[j + 1].score) {
                shouldSwap = true;
            }
            // Rule 2: If scores are EQUAL, compare missions (we want more missions first)
            else if (teams[j].score == teams[j + 1].score &&
                     teams[j].missions < teams[j + 1].missions) {
                shouldSwap = true;
            }
 
            // Perform the swap if needed
            if (shouldSwap) {
                Team temp = teams[j];
                teams[j] = teams[j + 1];
                teams[j + 1] = temp;
            }
        }
    }
}
 
// ===== DISPLAY LEADERBOARD =====
// Purpose: Print all teams in a formatted table
// Output format: Rank | ID | Name | Score | Missions, each in a fixed-width
// column so values line up under their headers no matter how many digits
// or characters they contain.
// (The old version separated columns with '\t'. Tabs jump to fixed
//  tab-stops, so a 1-digit ID and a 2-digit ID land the cursor in
//  different places and the columns drift out of alignment.)
// Example output:
//   Leaderboard
//   Rank   ID      Name                Score   Missions
//   1      101     Dragons             250     5
//   2      10      Tigers              80      1
void displayTeams() {
    if (size == 0) {
        std::cout << "No teams.\n";
        return;
    }
 
    // Fixed column widths, in characters. Bump nameW up if a team name
    // is ever longer than about 19 characters.
    const int rankW = 7, idW = 8, nameW = 20, scoreW = 8, missionsW = 8;
 
    std::cout << "\nLeaderboard\n" << std::left;
    std::cout << std::setw(rankW)     << "Rank"
               << std::setw(idW)       << "ID"
               << std::setw(nameW)     << "Name"
               << std::setw(scoreW)    << "Score"
               << std::setw(missionsW) << "Missions"
               << "\n";
 
    for (int i = 0; i < size; i++) {
        std::cout << std::setw(rankW)     << i + 1
                   << std::setw(idW)       << teams[i].id
                   << std::setw(nameW)     << teams[i].name
                   << std::setw(scoreW)    << teams[i].score
                   << std::setw(missionsW) << teams[i].missions
                   << "\n";
    }
}
 
// ===== PARSE TEAM DATA FROM FILE LINE =====
// Purpose: Convert a line of text from file into a Team struct
// File format: ID|Name|Score|Missions (separated by pipes |)
// Example: "101|Dragons|250|5" → Team{id:101, name:"Dragons", score:250, missions:5}
// Returns: true if parsing succeeds, false if format is invalid
bool parseTeamLine(const std::string& line, Team& out) {
    std::string idText, name, scoreText, missionsText, extra;
    std::stringstream ss(line);
 
    // Extract 4 fields separated by '|'
    if (!std::getline(ss, idText, '|') ||
        !std::getline(ss, name, '|') ||
        !std::getline(ss, scoreText, '|') ||
        !std::getline(ss, missionsText, '|')) {
        return false;  // Couldn't read all 4 fields
    }
 
    // Check for extra fields (there should be exactly 4 fields)
    if (std::getline(ss, extra, '|')) return false;
 
    // Check that no field is empty
    if (idText.empty() || name.empty() || scoreText.empty() || missionsText.empty()) return false;
 
    try {
        // Convert text fields to integers
        size_t p1 = 0, p2 = 0, p3 = 0;
        int id = std::stoi(idText, &p1);
        int score = std::stoi(scoreText, &p2);
        int missions = std::stoi(missionsText, &p3);
 
        // Ensure the entire string was converted (no leftover characters)
        if (p1 != idText.size() || p2 != scoreText.size() || p3 != missionsText.size()) return false;
 
        // Validate the parsed data
        if (!validateTeamData(id, name, score, missions)) return false;
 
        // Create output team
        out = Team{};
        out.id = id;
        strncpy(out.name, name.c_str(), sizeof(out.name) - 1);
        out.name[sizeof(out.name) - 1] = '\0';
        out.score = score;
        out.missions = missions;
        return true;
    } catch (...) {
        return false;  // Conversion failed (e.g., text is not a valid number)
    }
}
 
// ===== LOAD TEAMS FROM FILE =====
// Purpose: Read teams from a file and load them into memory
// File format: Each line is "ID|Name|Score|Missions"
// Strategy: Load into temporary array first to avoid corruption if load fails
// Returns: true if successful, false if file not found or other error
// Example file (teams.txt):
//   101|Dragons|250|5
//   102|Tigers|200|4
bool loadTeams(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;  // File doesn't exist or can't open
 
    // Create temporary storage for loaded teams
    // This way if something goes wrong, the current teams array stays unchanged
    Team* loaded = NULL;
    int loadedSize = 0;
    int loadedCapacity = 0;
 
    std::string line;
    // Read file line by line
    while (std::getline(file, line)) {
        if (line.empty()) continue;  // Skip empty lines
 
        Team candidate;
        // Try to parse the line
        if (!parseTeamLine(line, candidate)) {
            continue;  // Skip malformed lines
        }
 
        // Check for duplicate IDs (prevent adding same team twice)
        bool duplicate = false;
        for (int i = 0; i < loadedSize; i++) {
            if (loaded[i].id == candidate.id) {
                duplicate = true;
                break;
            }
        }
        if (duplicate) continue;  // Skip this team (already loaded)
 
        // Grow temporary array if needed
        if (loadedSize + 1 > loadedCapacity) {
            int newCapacity = (loadedCapacity == 0) ? 1 : loadedCapacity * 2;
            while (newCapacity < loadedSize + 1) newCapacity *= 2;
 
            Team* temp = (Team*)realloc(loaded, newCapacity * sizeof(Team));
            if (temp == NULL) {
                free(loaded);
                return false;  // Memory error
            }
            loaded = temp;
            loadedCapacity = newCapacity;
        }
 
        loaded[loadedSize++] = candidate;
    }
 
    file.close();
 
    // Replace old teams array with new one
    free(teams);
    teams = loaded;
    size = loadedSize;
    capacity = loadedCapacity;
    return true;
}
 
// ===== SAVE TEAMS TO FILE =====
// Purpose: Write all teams to a file in the format: ID|Name|Score|Missions
// Returns: true if successful, false if file can't be opened or write fails
// Example output file (teams.txt):
//   101|Dragons|250|5
//   102|Tigers|200|4
bool saveTeams(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;  // Can't create/open file
 
    bool ok = true;
    // Write each team on its own line
    for (int i = 0; i < size; i++) {
        file << teams[i].id << '|'
             << teams[i].name << '|'
             << teams[i].score << '|'
             << teams[i].missions << '\n';
        if (!file) {
            ok = false;
            break;
        }
    }
 
    file.close();
    if (!file) ok = false;  // Check for errors during close
    return ok;
}
 
// ===== CLEANUP MEMORY =====
// Purpose: Free all allocated memory before program exits
// Important: Always free dynamic memory to avoid memory leaks!
void cleanup() {
    free(teams);      // Release the teams array from memory
    teams = NULL;     // Set pointer to NULL (safe practice)
    size = 0;         // Reset size
    capacity = 0;     // Reset capacity
}
 
// ===== DISPLAY MENU =====
// Purpose: Show the main menu options to the user
void menu() {
    std::cout << "\n=== Campus Quest: Team Challenge Leaderboard ===\n"
         << "1. Register team\n"
         << "2. Record mission points\n"
         << "3. Find team\n"
         << "4. Remove team\n"
         << "5. Show leaderboard\n"
         << "6. Load teams\n"
         << "7. Save teams\n"
         << "0. Exit\n"
         << "Choice: ";
}
 
// ===== MAIN PROGRAM =====
// Purpose: Main game loop - displays menu and handles user choices
int main() {
    // Group members: add names here before submission.
 
    int choice;
    while (true) {
        menu();
 
        // Read user input (handle invalid input like letters or symbols)
        if (!(std::cin >> choice)) {
            std::cin.clear();                      // Clear error state
            std::cin.ignore(10000, '\n');          // Remove bad input from buffer
            std::cout << "Invalid input.\n";
            continue;
        }
        std::cin.ignore(10000, '\n');  // Remove newline from buffer after reading choice
 
        if (choice == 0) break;  // Exit program
 
        // CHOICE 1: Register team
        if (choice == 1) {
            int id;
            std::string name;
            std::cout << "ID: "; std::cin >> id;
            std::cin.ignore(10000, '\n');
            std::cout << "Name: "; std::getline(std::cin, name);
 
            if (addTeam(id, name)) std::cout << "Team registered.\n";
            else std::cout << "Registration rejected. Check ID/name/duplicate.\n";
        }
        // CHOICE 2: Record mission points
        else if (choice == 2) {
            int id, points;
            std::cout << "Team ID: "; std::cin >> id;
            std::cout << "Mission points (1-100): "; std::cin >> points;
            std::cin.ignore(10000, '\n');
 
            if (recordMission(id, points)) std::cout << "Mission recorded.\n";
            else std::cout << "Mission rejected. Record unchanged.\n";
        }
        // CHOICE 3: Find team
        else if (choice == 3) {
            int id;
            std::cout << "Team ID: "; std::cin >> id;
            std::cin.ignore(10000, '\n');
            int index = findTeamIndex(id);
            if (index == -1) std::cout << "Team not found.\n";
            else std::cout << "Found at logical index " << index << ": " << teams[index].name << "\n";
        }
        // CHOICE 4: Remove team
        else if (choice == 4) {
            int id;
            std::cout << "Team ID: "; std::cin >> id;
            std::cin.ignore(10000, '\n');
            if (deleteTeam(id)) std::cout << "Team removed.\n";
            else std::cout << "Team not found.\n";
        }
        // CHOICE 5: Show leaderboard
        else if (choice == 5) {
            sortLeaderboard();    // Sort before displaying
            displayTeams();       // Display sorted leaderboard
        }
        // CHOICE 6: Load teams from file
        else if (choice == 6) {
            if (loadTeams("teams.txt")) std::cout << "Teams loaded from teams.txt.\n";
            else std::cout << "Load failed.\n";
        }
        // CHOICE 7: Save teams to file
        else if (choice == 7) {
            if (saveTeams("teams.txt")) std::cout << "Teams saved to teams.txt.\n";
            else std::cout << "Save failed.\n";
        }
        // Invalid choice
        else {
            std::cout << "Unknown choice.\n";
        }
    }
 
    cleanup();  // Free memory before exiting
    return 0;
}
 