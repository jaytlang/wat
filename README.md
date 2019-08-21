# wat

hi, greetings, welcome!

this doesn't exactly fit in with the rest of my systems-oriented githubrepositories. but you know what? if anything this shows my ability to produce semi-quality code under pressure.

yesterday my friend dared me to write a tetris clone in C, given under 24 hours to complete the task. i accepted on the condition that i get a chocolate bar upon completion as reward - who would pass that up?

i ended up being pretty busy that day with other projects, but was able to work on the project that night. now, i wasn't about to give up that chocolate bar (hershey's man, am i right?), one caffeinated drink led to another, and i wound up with this with about 5 hours to spare.

it was at this point that i decided, why not go full send and throw in wiimote support for a true gamer experience? so in true software engineering fashion, more caffeine went in and out (is still coming) something to work with that. woot!

TL;DR: sleep is for the weak. can i have my chocolate now?

## how to build

wait, you're actually interested in running this thing?!

make sure you have the ncurses library installed, as well as the wiiuselibrary for wiimote support. both of these are pretty easy to find - what's harder is a stock Nintendo Wiimote or a third party one with all necessary protocols implemented. manufacturers cut corners so I recommend the former.

once you have everything on hand...

'''
make
./tetris
'''

easy. the script will take care of wiimote discovery and get the game on!


