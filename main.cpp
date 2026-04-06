#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Create a struct that houses the description of the the word that the user will try to guess
typedef struct DescriptionNode {
    char text[256];
    struct DescriptionNode *next;
} DescriptionNode;
 
/*
Create a struct that houses the word itself which also houses the description, hence the struct member descHead
with this you can create a list of words that contains a list of description
*/ 

typedef struct WordEntry {
    char word[50];
    DescriptionNode *descHead; 
    struct WordEntry *nextWord;
} WordEntry;

/* Create a function that is used to add a list of description to the words that has the W: flag. This way you can use
it to load into the memory, the words that you have made as well as their description on the word.txt */
void addDescription(WordEntry *word, const char *newDesc) {
    DescriptionNode *newNode = (DescriptionNode*)malloc(sizeof(DescriptionNode));
    if (!newNode) return;
    
    strcpy(newNode->text, newDesc);
    newNode->next = NULL;

    if (word->descHead == NULL) {
        word->descHead = newNode;
    } else {
        DescriptionNode *current = word->descHead;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

/* Create a function that will make an instance of the word list on the memory so that the words.txt words can be used
and the game can be based from */
WordEntry* loadGameData(const char *filename) {

    // opens the file with the name "filename" which is on the main function - directly referes to words.txt
    FILE *file = fopen(filename, "r");
    /* if the file is not found then it will say could not find "words.txt" However do remember that the file 
    that will be used is on the same folder as the main program */
    if (!file) {
        printf("Error: Could not find %s\n", filename);
        return NULL;
    }

    // Create a buffer to put each line of the file into
    char line[300];

    // Create an instance of the word list
    WordEntry *root = NULL;

    // Create an instance of the word node to be presented as the current node that you are in
    WordEntry *currentW = NULL;

    /*loop into the file, the main gist of fgets is it stops if it sees the line terminator. What the 
    function inside the while loop condition does it it copies each line one-by-one until it goes to the 
    end of file EOF to which there was no longer a line to copy. */
    while (fgets(line, sizeof(line), file)) {
        // Remove the line terminator for preparation of using the strncmp
        line[strcspn(line, "\r\n")] = 0; 

        /* Look at the line if it has W: in it. If it has then it will be stored as a new word, and if it is
        not then it will be stored as the description of the current word that it is in. */
        if (strncmp(line, "W:", 2) == 0) {
            WordEntry *newW = (WordEntry*)malloc(sizeof(WordEntry));
            strcpy(newW->word, line + 2);
            newW->descHead = NULL;
            newW->nextWord = NULL;

            /*
            if the root of the word list that we made is NULL then we can make the first word as the head of the list. However,
            note that the pointer of the head of the list actually resides in the stack here for the moment but the nodes themselves
            actually resides in the heap by the use of Malloc.
            */ 
            if (root == NULL) root = newW;

            /*If the root of the word list is not empty then proceed to populate the next */
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
    /* Afte using the file, it is needed to close it and we have to return the root of the word list that we made
    for us to use it */
    fclose(file);
    return root;
}

// This is now the game logic function where you will pass the list of word that you have made in your memory using the  functions above
void playGame(WordEntry *list) {
    // Check if the list is empty
    if (!list) return;

    // by using the pointer it initializes the first word, because the first node inside the list is the pointer itself (this is similar to arrays)
    WordEntry *current = list;
    char guess[50];

    // this loops through the list to make sure you are able to have all the tests done
    while (current != NULL) {
        printf("\n--- NEW WORD --- \n");
        DescriptionNode *d = current->descHead;
        
        // Loop through the linked list of descriptions as hints
        /*
        well this should be changed in the first place because Originally in my code, I want to have hints only up to 1 and
        and up it only after answering the words right and not just loop all the way in. And additionally there has to be
        a tally for scores. [candidate of change]
        */ 
        int hintCount = 1;
        while (d != NULL) {
            printf("Hint %d: %s\n", hintCount++, d->text);
            d = d->next;
        }

        // get input from user
        printf("Your guess: ");
        scanf("%s", guess);

        // compare user input to the current word
        /* There is also a catch here since we want to have a three life for our user and everytime he answers erroneously we 
        will deduct his life and if he got zero life we will take one of his limbs (but remember they are human so they have four limbs
        totalling his life up to 12) [candidate of change]*/
        if (strcasecmp(guess, current->word) == 0) {
            printf("CORRECT!\n");
        } else {
            printf("WRONG. The answer was %s.\n", current->word);
        }
        
        current = current->nextWord;
    }
    printf("\nGame Over! Thanks for playing.\n");
}
void freeer(DescriptionNode * d){
    if(d == NULL)
        return;
    freeer(d->next);
    free(d);
}
void freeerW(WordEntry* w){
    if(w == NULL)
    return;
    freeerW(w->nextWord);
    free(w);
}
int main() {
    WordEntry *myGame = loadGameData("words.txt");
    if (myGame) {
        playGame(myGame);
    }
    
    while(myGame){
        DescriptionNode * d = myGame->descHead;
        freeer(d);
        myGame = myGame->nextWord;
    }
    freeerW(myGame);
    
    // Note: In a professional app, you'd write a function 
    // to free() all nodes here to prevent memory leaks.
    return 0;
}