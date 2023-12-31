/**
 * ScreenManager class implementation
 */

#include "screenManager.hpp"
#include "../screens/difficultySelection.hpp"
#include "../screens/fleetPlacement.hpp"
#include "../screens/gameModeSelection.hpp"
#include "../screens/gameOver.hpp"
#include "../screens/gameplay.hpp"
#include "../screens/homepage.hpp"
#include "../screens/instructions.hpp"
#include "../screens/intermediary.hpp"

using screen::ScreenManager;
using screen::Screens;

ScreenManager::ScreenManager() {
    State::gui = std::make_unique<sf::RenderWindow>(sf::VideoMode(State::width, State::height), "Battleship", sf::Style::Titlebar | sf::Style::Close);
    State::gui->setFramerateLimit(60);

    screenList[Homepage] = &Homepage::getInstance();
    screenList[Instructions] = &Instructions::getInstance();
    screenList[GameModeSelection] = &GameModeSelection::getInstance();
    screenList[DifficultySelection] = &DifficultySelection::getInstance();
    screenList[FleetPlacement] = &FleetPlacement::getInstance();
    screenList[Gameplay] = &Gameplay::getInstance();
    screenList[Intermediary] = &Intermediary::getInstance();
    screenList[GameOver] = &GameOver::getInstance();
}

void ScreenManager::run() {
    State::gui->setKeyRepeatEnabled(false);

    while (State::gui->isOpen()) {
        screenList[State::getCurrentScreen()]->run();
    }
}
