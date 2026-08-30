#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#define NAME_LEN 40
#define LINE_LEN 160

typedef struct {
    int id;
    char name[NAME_LEN];
    int score;
    int missions;
} Team;

int findTeamIndex(const Team teams[], int size, int id);
int ensureCapacity(Team **teams, int *capacity, int required);
int addTeam(Team **teams, int *size, int *capacity, Team candidate);
int recordMission(Team teams[], int size, int id, int points);
int deleteTeam(Team teams[], int *size, int id);
void sortLeaderboard(Team teams[], int size);
void displayTeams(const Team teams[], int size);
int loadTeams(const char *filename, Team **teams, int *size, int *capacity);
int saveTeams(const char *filename, const Team teams[], int size);
void readText(const char *prompt, char text[], int limit);
int readInt(const char *prompt, int *value);

int main(void)
{
    Team *teams = nullptr;
    int size = 0;
    int capacity = 0;
    int choice;

    loadTeams("teams.txt", &teams, &size, &capacity);

    do {
        std::cout << "\n=== CAMPUS QUEST LEADERBOARD ===\n"
                  << "1. Register a team\n2. Record mission points\n3. Find a team\n"
                  << "4. Remove a team\n5. Show leaderboard\n6. Save and exit\n";

        if (!readInt("Choose: ", &choice)) {
            std::cout << "Invalid menu input.\n";
            continue;
        }

        if (choice == 1) {
            Team candidate = {0, "", 0, 0};
            readInt("Team ID: ", &candidate.id);
            readText("Team name: ", candidate.name, NAME_LEN);
            
            if (addTeam(&teams, &size, &capacity, candidate))
                std::cout << "Team registered successfully.\n";
            else
                std::cout << "Registration failed.\n";

        } else if (choice == 2) {
            int id, points;
            readInt("Team ID: ", &id);
            readInt("Mission points (1-100): ", &points);
            
            if (recordMission(teams, size, id, points))
                std::cout << "Mission points recorded.\n";
            else
                std::cout << "Failed to record mission points.\n";

        } else if (choice == 3) {
            int id;
            readInt("Team ID: ", &id);
            int idx = findTeamIndex(teams, size, id);
            
            if (idx != -1) {
                std::cout << "Found [Index " << idx << "]: ID " << teams[idx].id 
                          << " | Name: " << teams[idx].name 
                          << " | Score: " << teams[idx].score 
                          << " | Missions: " << teams[idx].missions << "\n";
            } else {
                std::cout << "Team not found.\n";
            }

        } else if (choice == 4) {
            int id;
            readInt("Team ID: ", &id);
            
            if (deleteTeam(teams, &size, id))
                std::cout << "Team removed successfully.\n";
            else
                std::cout << "Team not found.\n";

        } else if (choice == 5) {
            sortLeaderboard(teams, size);
            displayTeams(teams, size);

        } else if (choice == 6) {
            if (!saveTeams("teams.txt", teams, size))
                std::cout << "Warning: data could not be saved.\n";
            else
                std::cout << "Data saved successfully.\n";
        } else {
            std::cout << "Choose a number from 1 to 6.\n";
        }
    } while (choice != 6);

    std::free(teams);
    teams = nullptr;
    return 0;
}

int findTeamIndex(const Team teams[], int size, int id)
{
    if (teams == nullptr || size <= 0) return -1;
    
    for (int i = 0; i < size; i++) {
        if (teams[i].id == id) return i;
    }
    return -1;
}

int ensureCapacity(Team **teams, int *capacity, int required)
{
    if (required <= *capacity) return 1;

    int newCap = (*capacity == 0) ? 2 : (*capacity * 2);
    while (newCap < required) {
        newCap *= 2;
    }

    Team *temp = (Team *)std::realloc(*teams, newCap * sizeof(Team));
    if (temp == nullptr) return 0;

    *teams = temp;
    *capacity = newCap;
    return 1;
}

int addTeam(Team **teams, int *size, int *capacity, Team candidate)
{
    if (candidate.id <= 0 || std::strlen(candidate.name) == 0) return 0;
    if (candidate.score < 0 || candidate.missions < 0) return 0;
    if (findTeamIndex(*teams, *size, candidate.id) != -1) return 0;

    if (!ensureCapacity(teams, capacity, *size + 1)) return 0;

    (*teams)[*size] = candidate;
    (*size)++;
    return 1;
}

int recordMission(Team teams[], int size, int id, int points)
{
    if (points < 1 || points > 100) return 0;
    
    int index = findTeamIndex(teams, size, id);
    if (index == -1) return 0;

    teams[index].score += points;
    teams[index].missions += 1;
    return 1;
}

int deleteTeam(Team teams[], int *size, int id)
{
    int index = findTeamIndex(teams, *size, id);
    if (index == -1) return 0;

    for (int i = index; i < *size - 1; i++) {
        teams[i] = teams[i + 1];
    }
    (*size)--;
    return 1;
}

void sortLeaderboard(Team teams[], int size)
{
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            bool swapNeeded = false;
            
            if (teams[j].score < teams[j + 1].score) {
                swapNeeded = true;
            } else if (teams[j].score == teams[j + 1].score && 
                       teams[j].missions < teams[j + 1].missions) {
                swapNeeded = true;
            }

            if (swapNeeded) {
                Team temp = teams[j];
                teams[j] = teams[j + 1];
                teams[j + 1] = temp;
            }
        }
    }
}

void displayTeams(const Team teams[], int size)
{
    if (size == 0) {
        std::cout << "No teams registered.\n";
        return;
    }

    std::cout << "\n----------------------------------------------------\n";
    std::printf("%-6s %-8s %-22s %-8s %-8s\n", "Rank", "ID", "Name", "Score", "Missions");
    std::cout << "----------------------------------------------------\n";

    for (int i = 0; i < size; i++) {
        std::printf("%-6d %-8d %-22s %-8d %-8d\n", 
                    i + 1, teams[i].id, teams[i].name, teams[i].score, teams[i].missions);
    }
    std::cout << "----------------------------------------------------\n";
}

int loadTeams(const char *filename, Team **teams, int *size, int *capacity)
{
    FILE *file = std::fopen(filename, "r");
    if (!file) return 0;

    char line[LINE_LEN];
    while (std::fgets(line, sizeof(line), file) != nullptr) {
        line[std::strcspn(line, "\r\n")] = '\0';
        if (line[0] == '\0') continue;

        Team cand = {0, "", 0, 0};
        if (std::sscanf(line, "%d|%39[^|]|%d|%d", &cand.id, cand.name, &cand.score, &cand.missions) != 4) {
            continue;
        }

        if (cand.id <= 0 || cand.name[0] == '\0' || cand.score < 0 || cand.missions < 0) {
            continue;
        }

        addTeam(teams, size, capacity, cand);
    }

    std::fclose(file);
    return 1;
}

int saveTeams(const char *filename, const Team teams[], int size)
{
    FILE *file = std::fopen(filename, "w");
    if (!file) return 0;

    for (int i = 0; i < size; i++) {
        if (std::fprintf(file, "%d|%s|%d|%d\n", 
                         teams[i].id, teams[i].name, teams[i].score, teams[i].missions) < 0) {
            std::fclose(file);
            return 0;
        }
    }

    return (std::fclose(file) == 0);
}

void readText(const char *prompt, char text[], int limit)
{
    std::cout << prompt;
    if (fgets(text, limit, stdin) != NULL)
        text[std::strcspn(text, "\n")] = '\0';
}

int readInt(const char *prompt, int *value)
{
    char line[LINE_LEN], extra;
    std::cout << prompt;
    if (fgets(line, sizeof line, stdin) == NULL) return 0;
    return sscanf(line, "%d %c", value, &extra) == 1;
}