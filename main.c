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
    DescriptionNode *newNode = malloc(sizeof(DescriptionNode));
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

WordEntry* loadGameData(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open %s\n", filename);
        return NULL;
    }

    char line[300];
    WordEntry *root = NULL;
    WordEntry *currentW = NULL;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = 0; // Remove newline

        if (strncmp(line, "W:", 2) == 0) {
            WordEntry *newW = malloc(sizeof(WordEntry));
            strcpy(newW->word, line + 2);
            newW->descHead = NULL;
            newW->nextWord = NULL;

            if (root == NULL) root = newW;
            else {
                WordEntry *temp = root;
                while(temp->nextWord) temp = temp->nextWord;
                temp->nextWord = newW;
            }
            currentW = newW;
        } else if (strncmp(line, "D:", 2) == 0 && currentW != NULL) {
            addDescription(currentW, line + 2);
        }
    }
    fclose(file);
    return root;
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

int main() {
    WordEntry *vocabulary = loadGameData("words.txt");
    if (!vocabulary) return 1;

    HistoryNode trial[ROWS][COLS] = {0};
    PlayerState p = {3, 4, 4, 0, 0}; // 3 hearts per limb, 4 hints, 4 limbs
    
    int currentDay = 1;
    WordEntry *it = vocabulary;

    while (currentDay <= 30 && p.limbs > 0 && it) {
        
        int r = (currentDay - 1) / COLS;
        int c = (currentDay - 1) % COLS;
        trial[r][c].day_num = currentDay;

        
        if (currentDay % 7 == 0) {
            printf("\n[SUNDAY: PEACE OF MIND - 1 Heart Only]");
            p.hearts = 1;
            p.hints_allowed = 3;
        } else {
            p.hearts = 3; 
            p.hints_allowed = 1;
        }

        printf("\nDAY %d | Total Score: %d | Limbs Left: %d\n", currentDay, p.total_score, p.limbs);
        
        DescriptionNode *h = it->descHead;
        for(int i=0; i < p.hints_allowed && h; i++) {
            printf("Hint %d: %s\n", i+1, h->text);
            h = h->next;
        }

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
            
            if (p.consecutive_wins % 3 == 0) p.hints_allowed++;
            if (p.consecutive_wins >= 10 && p.limbs < 4) {
                p.limbs++;
                printf("A LIMB GREW BACK!\n");
                p.consecutive_wins = 0;
            }
            it = it->nextWord; 
        } else {
            printf("WRONG! Heart lost.\n");
            p.hearts--;
            p.consecutive_wins = 0;
        }

        // Limb Loss Logic
        if (p.hearts <= 0) {
            p.limbs--;
            printf("!!! A LIMB WAS REMOVED !!! (%d remaining)\n", p.limbs);
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