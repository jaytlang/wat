# WAT

hi, greetings, welcome!

this doesn't exactly fit in with the rest of my systems-oriented github repositories. but you know what? if anything this shows my ability to produce semi-quality code under pressure.

yesterday my friend dared me to write a tetris clone in C, given under 24 hours to complete the task. i accepted on the condition that i get a chocolate bar upon completion as reward - who would pass that up?

i ended up being pretty busy that day with other projects, but was able to work on the project that night. now, i wasn't about to give up that chocolate bar (hershey's man, am i right?), one caffeinated drink led to another, and i wound up with this with about 5 hours to spare.

it was at this point that i decided, why not go full send and throw in wiimote support for a true gamer experience? so in true software engineering fashion, more caffeine went in and out came a functional game. now we're on to implementing two player, because we can!

in this repo you'll find two things going on: tetris-one and tetris. tetris-one is the finished one-player variant, complete with wiimote support. tetris.c is under development following its predecessor's success - i'd like to demo a two-player tetris game! wiimote support through wiiuse is janky once you start polling >1 wiimote, but i think it can work.

TL;DR: sleep is for the weak. can i have my chocolate now?

## gameplay

you can find the executables in the bin folder. wiimote controls are as follows:
  - D-Pad: lateral/downward movement
  - B: rotate shape
  - A: force quit

additionally, you can head into the source and change the number of rows/cols as much as you'd like. this is within the first few lines of tetris-one.c and looks like this:

```
#define ROWS 20
#define COLS 10
```

obviously changes require recompilation to take effect, which brings me to my next topic...

## how to build

wait, you're actually interested in running this thing?!

make sure you have the ncurses library installed, as well as the wiiuse library for wiimote support. both of these are pretty easy to find - what's harder is a stock Nintendo Wiimote or a third party one with all necessary protocols implemented. manufacturers cut corners so I recommend the former.

once you have everything on hand...

```
make
cd bin && ./tetris-one
```

easy. the script will take care of wiimote discovery and get the game on!


