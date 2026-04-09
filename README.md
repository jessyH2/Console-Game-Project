```
//=====================================================================\\
||    ___ ___   _   ___ _  _ ___  ___ ___ ___  ___ ___ ___  ___ ___   ||
||   | _ ) _ \ /_\ |_ _| \| |   \| __/ __| _ \/ __| _ \ _ \| __| _ \  ||
||   | _ \   // _ \ | || .` | |) | _|\__ \   / (_ |  _/  _/ _||  _/   ||
||   |___/_|_/_/ \_\___|_|\_|___/___|___/_|_\\___|_|_|_| |___|_|      ||
||                                                                    ||
||                   >> THE 28-DAY TRIAL OF DEATH <<                  ||
\\=====================================================================//
```

# Goal of this game:
This is a console based game that me and my partner tried to create to solidify our knowledge on the fundamentals of programming:

### Game description:
This game is about a human trying to survive a 30 day trial to which if he guesses the words wrong he will lost limb and suffer
at his own fault. Well, he was given three chances per limb after all. However, this changes on Sundays - as the person becomes [weakened]. To reward him from being good at answering the
the words, he will gain a limb if he can answer the descriptions 10 times continously and note that his maximum limb is 4 since he is just a human.

### Simplified Game Logic:

- You are a human. (You have four limbs)
- Each limb is equivalent to three hearts on normal days and one heart on Sundays.
- Also you will only suffer the cutting of your limb once per day, so if you lose a limb that day, you will be getting ready for the second.
- If you answered the word right 10 times, you gain one limb (max of four - because you are a human)

- Your score is recorded in a 28-Day format (representing one month of suffering)

### Game Structure and Function Logic:

- #### Structures

<!-- DescriptionNode -->
On the first few lines of the code you will notice that we have implemented a struct for the description. This struct contains a pointer to the next struct of the same type making it possible for linked list and easier accessing and creating a list of description connected the word.

<!-- WordEntry -->
After this we also created the second struct of type WordEntry. This is basically a node for the words. These word node contains a pointer to the next word and at the same time it houses the list of descriptions that we made above.

<!-- CategoryList -->
Well, we cannot also forget about the category to which the player will play. This is to avoid redundancy on the game. This category node will be the twist in these data structures that we implemented in this game. This is because we made it so that it will be stored in an array-like way. The reason for this is to be able to randomize the category per day to suffice the reason on the second sentence in this description.

<!-- Rationale of Order of structure -->
It is also significant that we have implemented the nodes for description first and then the nodes for the words. Well, this is because if we hadn't done that the gcc compiler will not be able recognize the description node that is part of word node.

Next is for defining the struct that will be used for the history (the number of times you have died in a month as well as of course, the number of times you have lived.). With this you can create the matrix of size 7*4 that is similar to the setup of the month Febuary. We will be able to see this in the implementation that will be discussed below.

There is also a struct for the state of the player, which is used to ask the code if is he still alive(still has limbs on them)? Is their hearts all used up? how about their hints? Did they succeeded to answer three rounds? how about ten? Those questions are important to implement the logic of the game.

- #### Functions

---

*addDescription*

This function is used to make a list of descriptions under the same words. How does this work? Well, it starts by making a new description node and then it checks if the word already contains a description node. If it does, it will go up until the end of the description node list and it will append the new description from there. How can it tell if the last node is the last node? Well, it is easy - as the last node's next node is null.

---

*loadGameData*

This function is basically for the creation of the word list that we have that is to be added on the category. Notice how the addDescription function works? It appends to the last node on the list (as I mentioned before on my description of the word node), similar to that of the function above. The only catch here is that it goes through a file called "words.txt" (in main) and reads if there is a *C:* tag firsta and then a *W:* tag, lastly the *D:* tag. If there is a tag for the category, then it will make an instance of the category structure that is to be added to the category array. Then after seeing the word tag it will create an instance of a word node and append it to the last word node seen. If there is no tags like that it will then proceed to the *D:* tag, which will be passed as a string to the function addDescription. Easy right? This is just making array that contains a list that contains a list (similar to a matrix which is just an array containing an array). However, do note that this will be done up until the end of the file. Afterwards, of course you will need to close the file.

---

*showMatrix*

This function is just for showing the record that you have per day for the 28 days that you are in trial.

---

*main*

This where the implementation of the functions above came together. On the first line below the main() function, there is a time parser. This is made to make randomization for the category later. Right below it there can be seen the variable for the count of categories that will be made. This variable will be used later for the same purpose as the random time parser. It is passed as a reference to the loadGameData() function for the purpose of editing it's memory address itself without using too much of stack memory upon the function call and lessening the amount of lines of codes on the program.

The main() function also checks for the categories if it is either really loaded or the total categories is equal to 0 (this means that the words.txt does not exist or there are no categories present, which will hinder the game from being played).

Now as for the HistoryNode matrix (representation of 28 days), we need to initialize it first with 0 to avoid garbage memory to occupy the grids of the matrix. Similar with PlayerState we also need to initialize it with our starting values.

We are starting at currentDay which is Day-1 (things are getting excited huehuehue). There is a while loop and this is also called the main game loop. Without this loop the game cannot function well. This game loop will end when you have passed the 28th day of your trial or when your limbs are all torn off which means you're dead. 

There are two variables that are noticeable right from the start of the main game loop. These are r and c. r will serve as the row and c as the columns. r there is oddly equal to the currentDay-1 Divided by cols. Why is it? Well, this is because of the setup of the currentDay iterator which does not work like a double for loop nested. When the currentDay strikes to 8 it means it is now at the start of the second week, and thus currentDay - 1 = 7 and divided it by 7 it will result to 1 which is the second row on the matrix. This is also analogically similar to the column part but instead of using the divide operator, it uses the modulo operator and the reason is very obvious here (for beginners it just means that it will go to numbers not going above 7, a.k.a. the numbers of the week that is 0-indexed). 

Directly below the rows and columns logic there comes the randomizer. It randomizes to whatever category it will go. With the use of the rand() function seeded by the time(null) function which is just the number of seconds from some certain date up until now, you can make sure that the random index is actually random (the remainder of any number and another number will always not go beyond the second number). And after randomly selecting the category, of course you will need to access the words that are in that category. And by assigning a word node to the head of the nodes inside the category, you can now start the game of answering. However when the category is empty the program will skip it.

Now, for the fun part. Creating a condition that if the day is Sunday, the game will proceed with the [weakened] part, else the game will continue normally.

-- The game will Start --

First the program prints out your stats as well as your state, and it will now initialize a description node for the head of the description that is under your current word. You will be timed here with the use of time(NULL) function. Afterwards a string with the name of guess will be assigned as a string to which you will input to. 

- If you are out of time you will automatically be deducted by one heart and even mocked
- If you have guessed wrong you will also be deducted by one heart. And your consecutive wins will be resetted to 0 (awee).
- **AND IF YOU HAVE LOST ALL YOUR HEARTS, YOU WILL LOSE A LIMB**
- of course you will only lose a limb per day [that's a limit], you can't endure the pain can you?

What will you do? Of coures be fast and correct.

- If you are right and on-time then you have one consecutive win added to your list of consecutive wins. If you have enough consecutive wins then you have the possibility of gaining a miracle limb! (10 consecutive wins).

Then your current score on the day will be recorded by the trial matrix.

After all of this, all your score will be printed out and you will see how you did.
And of course do not forget to free your memory, because it is not used anymore.