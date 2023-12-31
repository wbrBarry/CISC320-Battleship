/**
 * Global game state implementation
 */

#include "state.hpp"

using std::string;

// Initialize the game state variables with their starting values
unique_ptr<sf::RenderWindow> State::gui = nullptr;
sf::Event State::event;
bool State::lockedFlag = false;

State::Player State::player = State::Player::P1;
State::GameMode State::gameMode = State::GameMode::SINGLE_PLAYER;
State::Difficulty State::difficulty = State::Difficulty::EASY;

Screens State::previous = Screens::Homepage;
Screens State::current = Screens::Homepage;

Screens State::getCurrentScreen() {
    return State::current;
}

Screens State::getPreviousScreen() {
    return State::previous;
}

void State::changeScreen(Screens newScreen) {
    if (State::current == newScreen) {
        throw std::invalid_argument("Error: You are trying to swap to the screens that is already being rendered");
    }

    State::previous = State::current;
    State::current = newScreen;
}

void State::previousScreen() {
    switch (State::current) {
        case Screens::Homepage:// Cannot go back from the homepage
            throw std::invalid_argument("Error: You can't go back from the home screen");
        case Screens::Instructions:// Goes back to the previously rendered screens for Instructions
            State::current = State::previous;
            break;
        case Screens::GameModeSelection:// Go back to the homepage for game mode selection
            State::current = Screens::Homepage;
            break;
        case Screens::DifficultySelection:// Go back to game mode selection for difficulty selection
            State::current = Screens::GameModeSelection;
            break;
        default:
            std::map<Screens, std::string> screenNames = {{Screens::Homepage,       "Homepage"},
                                                          {Screens::FleetPlacement, "Fleet Placement"},
                                                          {Screens::Gameplay,       "Gameplay"},
                                                          {Screens::Intermediary,   "Intermediary"},
                                                          {Screens::GameOver,       "Game Over"}};
            string errMsg = "Error: You can't go back from the current screen (" + screenNames[State::current]
                            + "). This exception should no occur- State::previousScreen() was incorrectly called";
            throw std::invalid_argument(errMsg);
    }
}

sf::Vector2f State::getMousePosition() {
    return State::gui->mapPixelToCoords(sf::Mouse::getPosition(*State::gui));
}
