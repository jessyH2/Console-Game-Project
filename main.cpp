#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TIME_LIMIT 15
#define ROWS 5
#define COLS 6  // 5 * 6 = 30 days total

// --- DATA STRUCTURES ---

// Nested Linked List for Hints
typedef struct DescriptionNode {
    char text[256];
    struct DescriptionNode *next;
} DescriptionNode;

// Word Head Node
typedef struct WordEntry {
    char word[50];
    DescriptionNode *descHead; 
    struct WordEntry *nextWord;
} WordEntry;

// Matrix Node for the 30-day Trial
typedef struct {
    int day_num;
    int score;
    int limbs_lost;
    int survived; // 1 for yes, 0 for dead
} HistoryNode;

typedef struct {
    int hearts;
    int hints_needed;
    int limbs;
    int total_score;
    int consecutive_wins;
} PlayerState;

// --- FUNCTIONS ---

void addHint(WordEntry *word, const char *text) {
    DescriptionNode *newNode = (DescriptionNode*)malloc(sizeof(DescriptionNode));
    strcpy(newNode->text, text);
    newNode->next = NULL;
    if (!word->descHead) word->descHead = newNode;
    else {
        DescriptionNode *temp = word->descHead;
        while(temp->next) temp = temp->next;
        temp->next = newNode;
    }
}

WordEntry* initVocabulary() {
    // Manually building a few for the demo, 
    // but you can use the File Parser from the previous step here!
    WordEntry *w1 = (WordEntry*)malloc(sizeof(WordEntry));
    strcpy(w1->word, "POINTER");
    w1->descHead = NULL; w1->nextWord = NULL;
    addHint(w1, "Stores memory addresses.");
    addHint(w1, "Uses the * operator.");
    addHint(w1, "Vital for dynamic allocation.");
    addHint(w1, "Can be NULL.");
    return w1;
}

void displayHistoryMatrix(HistoryNode matrix[ROWS][COLS]) {
    printf("\n=== 30-DAY CALENDAR LOG ===\n");
    printf("Day\tScore\tLimbs\tStatus\n");
    printf("------------------------------\n");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (matrix[i][j].day_num > 0) {
                printf("%d\t%d\t%d\t%s\n", 
                    matrix[i][j].day_num, 
                    matrix[i][j].score, 
                    4 - matrix[i][j].limbs_lost,
                    matrix[i][j].survived ? "ALIVE" : "DEAD");
            }
        }
    }
}

int main() {
    WordEntry *vocabulary = initVocabulary(); // Replace with file loader
    HistoryNode trial[ROWS][COLS] = {0};       // The Required Matrix
    PlayerState p = {3, 4, 4, 0, 0};           // hearts, hints, limbs, score, streak
    
    int currentDay = 1;
    WordEntry *it = vocabulary;

    while (currentDay <= 30 && p.limbs > 0 && it) {
        // Map 1D day to 2D Matrix Coordinates
        int r = (currentDay - 1) / COLS;
        int c = (currentDay - 1) % COLS;
        
        trial[r][c].day_num = currentDay;

        // Sunday Rule (Every 7th day)
        if (currentDay % 7 == 0) {
            printf("\n[SUNDAY: PEACE OF MIND]");
            p.hearts = 1;
            p.hints_needed = 3;
        } else {
            p.hearts = 3; 
            p.hints_needed = 4;
        }

        printf("\nDAY %d | Score: %d | Limbs: %d\n", currentDay, p.total_score, p.limbs);
        
        // Display hints from the linked list
        DescriptionNode *h = it->descHead;
        for(int i=0; i < p.hints_needed && h; i++) {
            printf("Hint %d: %s\n", i+1, h->text);
            h = h->next;
        }

        // Time Pressure Input
        time_t start = time(NULL);
        char guess[50];
        printf("GUESS (15s): ");
        scanf("%s", guess);

        if (difftime(time(NULL), start) > TIME_LIMIT) {
            printf("TOO SLOW! Heart lost.\n");
            p.hearts--;
        } else if (strcasecmp(guess, it->word) == 0) {
            printf("CORRECT!\n");
            p.total_score += 10;
            p.consecutive_wins++;
            
            if (p.consecutive_wins % 3 == 0) p.hints_needed++;
            if (p.consecutive_wins >= 10 && p.limbs < 4) {
                p.limbs++;
                p.consecutive_wins = 0;
            }
            it = it->nextWord; // Move to next word
        } else {
            printf("WRONG! Heart lost.\n");
            p.hearts--;
            p.consecutive_wins = 0;
        }

        if (p.hearts <= 0) {
            p.limbs--;
            printf("A LIMB WAS CUT! %d left.\n", p.limbs);
            p.hearts = 3; // Reset for next limb
        }

        // Save state to Matrix Node
        trial[r][c].score = p.total_score;
        trial[r][c].limbs_lost = 4 - p.limbs;
        trial[r][c].survived = (p.limbs > 0);

        currentDay++;
    }

    displayHistoryMatrix(trial);
    return 0;
}