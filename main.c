#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TIME_LIMIT 15
#define ROWS 5
#define COLS 6 


typedef struct DescriptionNode {
    char text[256];
    struct DescriptionNode *next;
} DescriptionNode;

typedef struct WordEntry {
    char word[50];
    DescriptionNode *descHead; 
    struct WordEntry *nextWord;
} WordEntry;

typedef struct {
    char name[50];
    struct WordEntry *wordHead;
} Category;


typedef struct {
    int day_num;
    int score;
    int limbs_remaining;
    int survived; 
} HistoryNode;

typedef struct {
    int hearts;
    int hints_allowed;
    int limbs;
    int total_score;
    int consecutive_wins;
} PlayerState;


void addDescription(WordEntry *word, const char *newDesc) {
    DescriptionNode *newNode = (DescriptionNode*)malloc(sizeof(DescriptionNode));
    if (!newNode) return;
    strcpy(newNode->text, newDesc);
    newNode->next = NULL;

    if (word->descHead == NULL) {
        word->descHead = newNode;
    } else {
        DescriptionNode *current = word->descHead;
        while (current->next != NULL) current = current->next;
        current->next = newNode;
    }
}

Category* loadGameData(const char *filename, int *categoryCount) {
    FILE *file = fopen(filename, "r");
    if (!file) return NULL;

    char line[300];
    int count = 0;

    // Count the number of categories in the word.txt or the file
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "C:", 2) == 0) count++;
    }
    /* With the use of rewind function we wil go back to the top of the file
    without reinitializing the file to *file again */
    rewind(file); 

    // Allocate the array of categories
    Category *catArray = malloc(sizeof(Category) * count);
    *categoryCount = count;

    int currentCatIdx = -1;
    WordEntry *currentW = NULL;

    // Pass 2: Fill the Array
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = 0;

        if (strncmp(line, "C:", 2) == 0) {
            currentCatIdx++;
            strcpy(catArray[currentCatIdx].name, line + 2);
            catArray[currentCatIdx].wordHead = NULL;
            currentW = NULL; 
        } 
        else if (strncmp(line, "W:", 2) == 0 && currentCatIdx >= 0) {
            WordEntry *newW = malloc(sizeof(WordEntry));
            strcpy(newW->word, line + 2);
            newW->descHead = NULL;
            newW->nextWord = NULL;

            // Link word to the current category in the array
            if (catArray[currentCatIdx].wordHead == NULL) {
                catArray[currentCatIdx].wordHead = newW;
            } else {
                WordEntry *temp = catArray[currentCatIdx].wordHead;
                while(temp->nextWord) temp = temp->nextWord;
                temp->nextWord = newW;
            }
            currentW = newW;
        } 
        else if (strncmp(line, "D:", 2) == 0 && currentW != NULL) {
            addDescription(currentW, line + 2);
        }
    }

    fclose(file);
    return catArray;
}
 
void showMatrix(HistoryNode matrix[ROWS][COLS]) {
    printf("\n\n===== 30-DAY TRIAL RESULTS (MATRIX) =====\n");
    printf("DAY\tSCORE\tLIMBS\tSTATUS\n");
    printf("--------------------------------------\n");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (matrix[i][j].day_num > 0) {
                printf("%d\t%d\t%d\t%s\n", 
                    matrix[i][j].day_num, 
                    matrix[i][j].score, 
                    matrix[i][j].limbs_remaining,
                    matrix[i][j].survived ? "ALIVE" : "DEAD");
            }
        }
    }
}

void printBrainDescGrepHeader() {
    printf("//========================================================================\\\\\n");
    printf("||  ___ ___   _   ___ _  _ ___  ___ ___ ___  ___ ___ ___  ___ ___       ||\n");
    printf("|| | _ ) _ \\ /_\\ |_ _| \\| |   \\| __/ __| _ \\/ __| _ \\ _ \\| __| _ \\      ||\n");
    printf("|| | _ \\   // _ \\ | || .` | |) | _|\\__ \\   / (_ |  _/  _/ _||  _/      ||\n");
    printf("|| |___/_|_/_/ \\_\\___|_|\\_|___/___|___/_|_\\\\___|_|_|_| |___|_|        ||\n");
    printf("||                                                                    ||\n");
    printf("||                >> THE 30-DAY TRIAL OF DEATH <<                     ||\n");
    printf("\\\\========================================================================//\n");
}

int main() {
    // 1. Initialize Random Seed
    srand(time(NULL));

    // 2. Load Data into the Array of Categories
    int totalCategories = 0;
    Category *categories = loadGameData("words.txt", &totalCategories);
    
    if (!categories || totalCategories == 0) {
        printf("Error: No categories loaded from words.txt!\n");
        return 1;
    }

    printBrainDescGrepHeader();

    HistoryNode trial[ROWS][COLS] = {0};
    PlayerState p = {3, 4, 4, 0, 0}; // hearts, hints, limbs, score, streak
    
    int currentDay = 1;

    // The game loop runs for 30 days or until limbs run out
    while (currentDay <= 30 && p.limbs > 0) {
        int r = (currentDay - 1) / COLS;
        int c = (currentDay - 1) % COLS;
        trial[r][c].day_num = currentDay;

        // 3. RANDOMIZER: Pick a random category for the day
        int randomIdx = rand() % totalCategories;
        Category *todaysCategory = &categories[randomIdx];

        // 4. PICK A WORD: For simplicity, we'll pick the first word of that category
        // (If you want a random word within the category, we can add a shuffler later!)
        WordEntry *it = todaysCategory->wordHead;

        if (!it) {
            currentDay++; // Skip if category is empty
            continue;
        }

        // Sunday Rule
        if (currentDay % 7 == 0) {
            printf("\n[SUNDAY: PEACE OF MIND - 1 Heart Only]");
            p.hearts = 1;
            p.hints_allowed = 3;
        } else {
            p.hearts = 3; 
            p.hints_allowed = 1; // Base hints
        }

        printf("\nDAY %d | Category: %s\n", currentDay, todaysCategory->name);
        printf("Score: %d | Limbs: %d | Hearts: %d\n", p.total_score, p.limbs, p.hearts);
        
        // Display hints
        DescriptionNode *h = it->descHead;
        for(int i = 0; i < p.hints_allowed && h; i++) {
            printf("Hint %d: %s\n", i + 1, h->text);
            h = h->next;
        }

        // Time Pressure Logic
        time_t start = time(NULL);
        char guess[50];
        printf("GUESS (15s): ");
        scanf("%s", guess);

        if (difftime(time(NULL), start) > TIME_LIMIT) {
            printf("\nTOO SLOW! Heart lost.\n");
            p.hearts--;
        } else if (strcasecmp(guess, it->word) == 0) {
            printf("\nCORRECT!\n");
            p.total_score += 10;
            p.consecutive_wins++;
            
            if (p.consecutive_wins % 3 == 0) p.hints_allowed++;
            if (p.consecutive_wins >= 10 && p.limbs < 4) {
                p.limbs++;
                printf("MIRACLE: A LIMB GREW BACK!\n");
                p.consecutive_wins = 0;
            }
        } else {
            printf("\nWRONG! Heart lost.\n");
            p.hearts--;
            p.consecutive_wins = 0;
        }

        // Limb Loss Logic
        if (p.hearts <= 0) {
            p.limbs--;
            printf("!!! A LIMB WAS SEVERED !!! (%d remaining)\n", p.limbs);
            if (p.limbs > 0) p.hearts = 3; 
        }

        // Update Matrix State
        trial[r][c].score = p.total_score;
        trial[r][c].limbs_remaining = p.limbs;
        trial[r][c].survived = (p.limbs > 0);

        currentDay++;
    }

    showMatrix(trial);
    return 0;
} 