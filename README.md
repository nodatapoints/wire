# Cellular Wire Automaton
## Installation
Get [SFML](https://www.sfml-dev.org/).
```bash
$ sudo apt install libsfml-dev
```
Clone the repository and build the source
```bash
$ git clone https://github.com/nodatapoints/wire
$ cd wire
/wire $ make
/wire $ ./wire
```

## About
A minimalistic cellular automaton capable of simulating logic circuits.

### The Rules
In an infinite grid of square cells, there are for states:
1. **Empty**
2. **Wire**
3. **Blocking**
4. **Active**

At each point in time, every cell is in _exactly one_ state. As time progresses, the states change from one generation to the next.<br>
All cells change simultaneously according to the following rules:

__In the next generation, a cell will change to state__
1. __... Empty if it is Empty__
2. __... Wire if it is Blocking__
3. __... Blocking if it is Active__
4. __... Active if it is Wire and it has an odd number of active neighbors with a shared edge.__

### The Controls

There are two modes:
* The _Edit Mode_ allows you to edit cells while time is halted. You will start in this mode.
* The _Run Mode_ allows you to simulate what you've built.

#### Edit Mode
The gray square is your _cursor_. On left click, you will set a wire at the cursor position. On right click you will delete it. Pressing <kbd>Shift</kbd> while clicking left will set an active cell.

When holding <kbd>Ctrl</kbd>, you can make a rectangular _selection_. Clicking will set the whole selection like it would with a single cell.<br>
Pressing <kbd>X</kbd> cuts the current selection, <kbd>C</kbd> copies it. You will see the selection hover and move with the cursor. Pressing <kbd>V</kbd> pastes the clipboard at the current position.
Once you're done, <kbd>Esc</kbd> will cancel pasting.

#### Run Mode
When pressing <kbd>Space</kbd>, the simulation will start running. It can be paused and continued with <kbd>P</kbd>. When paused, you can run it step by step using <kbd>S</kbd>.<br>
To start in paused, press <kbd>S</kbd> instead of <kbd>Space</kbd>.

To exit the Run Mode, press <kbd>Space</kbd> or <kbd>Escape</kbd> again. Otherwise it will exit automatically once there are no active cells present.<br>
After exiting, the state before running will be resumed.

To exit the game, press <kbd>Q</kbd>