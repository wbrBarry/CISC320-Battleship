![image](https://user-images.githubusercontent.com/61813081/153123375-0fcfd748-583e-4626-b2e8-19e111ffb8e6.png)

# Battleship

This is the main group project for a C++ software course (software design and project management based) I completed in 2021. The group members are Logan Copeland, Reid Moffat, Jacob Wong, Maya Steckly, Kaitlyn Hung, Baorong Wei (myself), and Shirley Hu. We didn't use GitHub for this project, so this GitHub repository was copied from our original repo. 

For this project, we developed a classic battleship game for Player vs. Computer and Player vs. Player using SFML Library, and we designed GUI using Aseprite. I worked as one of the software developers, working on the entity part of the project, as well as some back-end and front-end design. 

A simple video demo: [Battleship Video Demo](https://youtu.be/JmYXjjfe6AE)

While this project overall is pretty simple & straightforward, my team did this mainly to learn more about C++, which we definitely succeeded in. In a few months, I learned a lot of concepts, including properly using pointers, CMAKE, refactoring & software design, thanks to our project architect, Reid. 

## File structure

*SFML: [download](https://www.sfml-dev.org/download.php) | [git repo](https://github.com/SFML/SFML)*

    .
    ├── .github                 # Contains the issue template
    ├── .idea                   # JetBrains (CLion) settings
    ├── .vscode                 # Visual Studio Code settings
    ├── bin
    |   └── battleship.exe      # Compiled binary with the most recent code
    ├── include
    |   └── SFML                # A copy of the SFML library (see above for download)
    ├── res
    |   └── images              # Pixel graphic images for the game's UI
    ├── src
    |   ├── controllers         # Overarching classes that manage and define the screens and game state
    |   ├── entity              # Game entities
    |   ├── enums               # Enumerations for types of screens, ships and grid squares
    |   ├── helpers             # Stateless helper classes and functions
    |   ├── screens             # Each possible game screen
    |   └── main.cpp            # Entry point for the program
    ├── test                    # Tests using google test (incomplete)
    ├── .clang-format           # Formatting settings
    ├── .gitignore
    ├── CMakeLists.txt          # Build script
    ├── LICENSE
    └── README.md
