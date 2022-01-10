/**
 * File: difficultySelection.cpp
 * Description: Front-end class that defines the behaviour of the Difficulty selection screens
 */

#include "difficultySelection.hpp"

using screen::DifficultySelection;

DifficultySelection *DifficultySelection::instance = nullptr;

DifficultySelection::DifficultySelection() : ScreenTemplate() {
    loadTexture(this->difficultyBackgroundTexture, "difficultySelection/DifficultyBackground.png");
    loadTexture(this->idleEasyButtonTexture, "difficultySelection/IdleEasyButton.png");
    loadTexture(this->activeEasyButtonTexture, "difficultySelection/ActiveEasyButton.png");
    loadTexture(this->idleHardButtonTexture, "difficultySelection/IdleHardButton.png");
    loadTexture(this->activeHardButtonTexture, "difficultySelection/ActiveHardButton.png");
    loadTexture(this->idleBackButtonTexture, "difficultySelection/IdleBackButton.png");
    loadTexture(this->activeBackButtonTexture, "difficultySelection/ActiveBackButton.png");
    loadTexture(this->idleInstructionsButton, "difficultySelection/IdleInstructionsButton.png");
    loadTexture(this->activeInstructionsButton, "difficultySelection/ActiveInstructionsButton.png");

    setSprite(sf::Vector2f(0, 0), sf::Vector2f(5, 5), this->difficultyBackgroundTexture, this->backgroundSprite);

    this->easyButton = new Button(sf::Vector2f(88 * 5, 92 * 5), sf::Vector2f(5, 5), this->idleEasyButtonTexture, this->activeEasyButtonTexture);
    this->hardButton = new Button(sf::Vector2f(200 * 5, 92 * 5), sf::Vector2f(5, 5), this->idleHardButtonTexture, this->activeHardButtonTexture);
    this->backButton = new Button(sf::Vector2f(320 * 5, 12 * 5), sf::Vector2f(5, 5), this->idleBackButtonTexture, this->activeBackButtonTexture);
    this->instructionsButton = std::make_unique<Button>(sf::Vector2f(352 * 5, 12 * 5), sf::Vector2f(5, 5), this->idleInstructionsButton, this->activeInstructionsButton);
}

DifficultySelection &screen::DifficultySelection::getInstance() {
    if (instance == nullptr) {
        instance = new DifficultySelection();
    }
    return *instance;
}

void DifficultySelection::update() {
    sf::Vector2f mousePosition = State::getMousePosition();
    this->easyButton->updateButtonState(mousePosition);
    this->hardButton->updateButtonState(mousePosition);
    this->backButton->updateButtonState(mousePosition);
    this->instructionsButton->updateButtonState(mousePosition);
}

void DifficultySelection::poll() {
    sf::RenderWindow &gui = *State::gui;
    sf::Event &event = State::event;

    while (gui.pollEvent(event)) {
        switch (event.type) {

            case sf::Event::Closed:
                gui.close();
                break;

            case sf::Event::MouseButtonReleased:
                if ((event.mouseButton.button == sf::Mouse::Left) && (this->easyButton->getButtonState())) {
                    State::difficulty = State::Difficulty::EASY;
                    State::changeScreen(Screens::FLEET_PLACEMENT);
                    break;
                } else if ((event.mouseButton.button == sf::Mouse::Left) && (this->hardButton->getButtonState())) {
                    State::difficulty = State::Difficulty::HARD;
                    State::changeScreen(Screens::FLEET_PLACEMENT);
                    break;
                } else if ((event.mouseButton.button == sf::Mouse::Left) && (this->backButton->getButtonState())) {
                    State::changeScreen(Screens::GAME_MODE_SELECTION);
                    break;
                } else if ((event.mouseButton.button == sf::Mouse::Left) && (this->instructionsButton->getButtonState())) {
                    State::changeScreen(Screens::INSTRUCTIONS);
                    break;
                } else {
                    break;
                }

            default:
                break;
        }
    }
}

void DifficultySelection::render() {
    sf::RenderWindow &gui = *State::gui;
    gui.clear();

    gui.draw(this->backgroundSprite);
    easyButton->render(gui);
    hardButton->render(gui);
    backButton->render(gui);
    instructionsButton->render(gui);

    if (State::getCurrentScreen() == Screens::DIFFICULTY_SELECTION) {
        gui.display();
    }
}
