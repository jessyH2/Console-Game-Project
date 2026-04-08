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
at his own fault. Well, he was given three chances per limb after all, and on top of that he can even use another hint to make 
answering the hard word significantly easier. However, this changes on Sundays - as the person becomes [weakened]. To compromise
his weakness, he enters [divinity] having more hint points to use. Additionally, to reward him from being good at answering the
the words, he will gain a limb if he can answer the descriptions 10 times continously, similarly he will also gain hints if he can
answer three times continuously, however [divinity] is the limit. That means he can only have three maximum hint.

### Simplified Game Logic:

- You are a human. (You have four limbs)
- Each limb is equivalent to three hearts and a hint on normal days and one heart and three hints on Sundays.
- If you answered the word right 10 times, you gain one limb (max of four - because you are a human)
- If you answered the word right 3 times, you gain one hint (maximum of three - you are not a god.)
- Your score is recorded in a 30-Day format (representing one month of suffering)

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

This function is basically the creation of the word list that we have. Notice how the addDescription function works? It appends to the last node on the list (as I mentioned before on my description of the word node), similar to that of the function above. The only catch here is that it goes through a file called "words.txt" (in main) and reads if there is a *W:* tag. If there is, it will create an instance of a word node and append it to the last node. If there is no tags like that it will then proceed to the *D:* tag, which will be passed as a string to the function addDescription. Easy right? This is just making a list that contains a list (similar to a matrix which is just an array containing an array). Afterwards, of coures you will need to close the file.

---

*showMatrix*

This function is just for showing the record that you have per day for the 28 days that you are in trial.

---

*main*

This is the implementation of the functions above. As you can see we have created an instance of the word list names vocabulary - by passing the string with the name words.txt (which this string will be used to open the file names "words.txt"). It will also check if the list is empty. If it is, then of course it will end there. 




