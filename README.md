//=====================================================================\\
||    ___ ___   _   ___ _  _ ___  ___ ___ ___  ___ ___ ___  ___ ___   ||
||   | _ ) _ \ /_\ |_ _| \| |   \| __/ __| _ \/ __| _ \ _ \| __| _ \  ||
||   | _ \   // _ \ | || .` | |) | _|\__ \   / (_ |  _/  _/ _||  _/   ||
||   |___/_|_/_/ \_\___|_|\_|___/___|___/_|_\\___|_|_|_| |___|_|      ||
||                                                                    ||
||                   >> THE 30-DAY TRIAL OF DEATH <<                  ||
\\=====================================================================//

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

<!-- DescriptionNode -->
On the first few lines of the code you will notice that we have implemented a struct for the description. This struct contains a pointer to the next struct of the same type making it possible for linked list and easier accessing and creating a list of description connected the word.

<!-- WordEntry -->
After this we also created the second struct of type WordEntry. This is basically a node for the words. These word node contains a pointer to the next word and at the same time it houses the list of descriptions that we made above.

<!-- Rationale of Order of structure -->
It is also significant that we have implemented the nodes for description first and then the nodes for the words. Well, this is because if we hadn't done that the gcc compiler will not be able recognize the description node that is part of word node.



