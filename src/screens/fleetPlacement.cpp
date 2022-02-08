/**
 * Front-end class that defines the behaviour of the Fleet Placement screens
 */

#include "fleetPlacement.hpp"
#include "gameplay.hpp"

using screen::FleetPlacement;
using std::get;

std::unique_ptr<FleetPlacement> FleetPlacement::instance = nullptr;

FleetPlacement::FleetPlacement() : ScreenTemplate() {
    // Data required for all the SFML objects on this screen
    const vector<string> texturePaths = {
            "FleetPlacementBackground.png",// Background textures
            "FleetPlacementP1Background.png",
            "FleetPlacementP2Background.png",

            "BattleShip.png",// Ship textures
            "AircraftCarrier.png",
            "Destroyer.png",
            "Submarine.png",
            "PatrolBoat.png",
            "RowBoat.png",

            "IdleReadyButton.png",// Button idle and active textures
            "ActiveReadyButton.png",
            "IdleRandomizeButton.png",
            "ActiveRandomizeButton.png",
            "IdleInstructionsButton.png",
            "ActiveInstructionsButton.png",
    };
    const vector<sprite> sprites = {
            {sf::Vector2f(0, 0), sf::Vector2f(5, 5), BackgroundDefaultTexture},// Background sprites
            {sf::Vector2f(0, 0), sf::Vector2f(5, 5), BackgroundP1Texture},
            {sf::Vector2f(0, 0), sf::Vector2f(5, 5), BackgroundP2Texture},

            {sf::Vector2f(21 * 5, 84 * 5), sf::Vector2f(5, 5), BattleshipTexture},// Ship sprites
            {sf::Vector2f(42 * 5, 92 * 5), sf::Vector2f(5, 5), AircraftCarrierTexture},
            {sf::Vector2f(61 * 5, 100 * 5), sf::Vector2f(5, 5), DestroyerTexture},
            {sf::Vector2f(61 * 5, 35 * 5), sf::Vector2f(5, 5), SubmarineTexture},
            {sf::Vector2f(42 * 5, 43 * 5), sf::Vector2f(5, 5), PatrolBoatTexture},
            {sf::Vector2f(21 * 5, 51 * 5), sf::Vector2f(5, 5), RowBoatTexture},
    };
    const vector<button> buttons = {
            {sf::Vector2f(320 * 5, 124 * 5), sf::Vector2f(5, 5), IdleReadyButtonTexture, ActiveReadyButtonTexture},
            {sf::Vector2f(328 * 5, 76 * 5), sf::Vector2f(5, 5), IdleRandomizeButtonTexture, ActiveRandomizeButtonTexture},
            {sf::Vector2f(352 * 5, 12 * 5), sf::Vector2f(5, 5), IdleInstructionsButtonTexture, ReadyInstructionsButtonTexture},
    };

    // Initialize SFML objects
    this->resources = ScreenResourceManager("fleetPlacement", texturePaths, sprites, buttons);

    this->layoutGenerated = false;
}

void FleetPlacement::addCoord(vector<Coordinate> &coordinates, const int x, const int y) {
    if (x >= 0 && x < Grid::size && y >= 0 && y < Grid::size) {
        coordinates.emplace_back(x, y);
    }
}


void FleetPlacement::randomize() {
    // Start with a temporary grid of open spots (false = not occupied)
    bool grid[Grid::size][Grid::size];
    for (auto &row : grid) {
        for (bool &square : row) {
            square = false;
        }
    }

    // The order the ships are randomly deployed in
    static shipNames allShips[6] = {
            shipNames::Battleship,
            shipNames::AircraftCarrier,
            shipNames::Destroyer,
            shipNames::Submarine,
            shipNames::PatrolBoat,
            shipNames::RowBoat,
    };
    for (shipNames ship : allShips) {
        int size = shipSize(ship);
        while (true) {
            bool horizontal = randomInt(0, 1) % 2 != 0;// Whether the ship is horizontal or vertical

            // Determine a random x and y position for the ship (it will be on the board, but might already be occupied)
            const int maxBoardIndex = Grid::size - 1;  // The maximum coordinate index on the board
            const int maxShipStart = Grid::size - size;// The maximum coordinate index to start the ship at so that it fits on the board
            int x = randomInt(0, horizontal ? maxShipStart : maxBoardIndex);
            int y = randomInt(0, horizontal ? maxBoardIndex : maxShipStart);

            // Create a list of square to check
            vector<Coordinate> shipSquares;// Squares occupied by this ship and adjacent ones

            if (horizontal) {
                for (int i = 0; i < size; ++i) {
                    addCoord(shipSquares, x + i, y);// Actual ship
                }
                addCoord(shipSquares, x - 1, y);// Square left of the leftmost part
                for (int i = 0; i < size; ++i) {
                    addCoord(shipSquares, x + i, y - 1);// 1 square above
                    addCoord(shipSquares, x + i, y + 1);// 1 square below
                }
                addCoord(shipSquares, x + size, y);// Square right of the rightmost part
            } else {
                for (int i = 0; i < size; ++i) {
                    addCoord(shipSquares, x, y + i);// Actual ship
                }
                addCoord(shipSquares, x, y - 1);// Square above the topmost part
                for (int i = 0; i < size; ++i) {
                    addCoord(shipSquares, x - 1, y + i);// 1 square left
                    addCoord(shipSquares, x + 1, y + i);// 1 square right
                }
                addCoord(shipSquares, x, y + size);// Square below the bottom part
            }

            // Check if all the ship squares are available
            bool invalid = false;
            for (Coordinate c : shipSquares) {
                if (grid[c.getY()][c.getX()]) {
                    invalid = true;
                    break;
                }
            }
            if (invalid) continue;// If not, try again

            // The ship is valid; now place it down
            ships[ship] = {shipSquares.at(0), horizontal};// Ship coordinate and orientation
            for (int i = 0; i < size; ++i) {
                // Only set the actual squares this ship is occupying
                // It is ok if there is only one square in between ships
                Coordinate curr = shipSquares.at(i);
                grid[curr.getY()][curr.getX()] = true;
            }
            break;
        }
    }
}

void FleetPlacement::updateFleetLayout() {
    sf::Sprite &battleship = resources.getSprite(Battleship);
    sf::Sprite &aircraftCarrier = resources.getSprite(AircraftCarrier);
    sf::Sprite &destroyer = resources.getSprite(Destroyer);
    sf::Sprite &submarine = resources.getSprite(Submarine);
    sf::Sprite &patrolBoat = resources.getSprite(PatrolBoat);
    sf::Sprite &rowBoat = resources.getSprite(RowBoat);

    if (get<1>(this->ships[shipNames::Battleship]) == 1) {
        battleship.setPosition(sf::Vector2f((224 + (get<0>(this->ships[shipNames::Battleship]).getX() * 16)) * 5,
                                            (28 + (get<0>(this->ships[shipNames::Battleship]).getY() * 16)) * 5));
        battleship.setRotation(90.f);
    } else {
        battleship.setPosition(sf::Vector2f((128 + (get<0>(this->ships[shipNames::Battleship]).getX() * 16)) * 5,
                                            (28 + (get<0>(this->ships[shipNames::Battleship]).getY() * 16)) * 5));
        battleship.setRotation(0);
    }

    if (get<1>(this->ships[shipNames::AircraftCarrier]) == 1) {
        aircraftCarrier.setPosition(sf::Vector2f((208 + (get<0>(this->ships[shipNames::AircraftCarrier]).getX() * 16)) * 5,
                                                 (28 + (get<0>(this->ships[shipNames::AircraftCarrier]).getY() * 16)) * 5));
        aircraftCarrier.setRotation(90.f);
    } else {
        aircraftCarrier.setPosition(sf::Vector2f((128 + (get<0>(this->ships[shipNames::AircraftCarrier]).getX() * 16)) * 5,
                                                 (28 + (get<0>(this->ships[shipNames::AircraftCarrier]).getY() * 16)) * 5));
        aircraftCarrier.setRotation(0);
    }

    if (get<1>(this->ships[shipNames::Destroyer]) == 1) {
        destroyer
                .setPosition(sf::Vector2f((192 + (get<0>(this->ships[shipNames::Destroyer]).getX() * 16)) * 5,
                                          (28 + (get<0>(this->ships[shipNames::Destroyer]).getY() * 16)) * 5));
        destroyer.setRotation(90.f);
    } else {
        destroyer.setPosition(sf::Vector2f((128 + (get<0>(this->ships[shipNames::Destroyer]).getX() * 16)) * 5,
                                           (28 + (get<0>(this->ships[shipNames::Destroyer]).getY() * 16)) * 5));
        destroyer.setRotation(0);
    }

    if (get<1>(this->ships[shipNames::Submarine]) == 1) {
        submarine.setPosition(sf::Vector2f((176 + (get<0>(this->ships[shipNames::Submarine]).getX() * 16)) * 5,
                                           (28 + (get<0>(this->ships[shipNames::Submarine]).getY() * 16)) * 5));
        submarine.setRotation(90.f);
    } else {
        submarine.setPosition(sf::Vector2f((128 + (get<0>(this->ships[shipNames::Submarine]).getX() * 16)) * 5,
                                           (28 + (get<0>(this->ships[shipNames::Submarine]).getY() * 16)) * 5));
        submarine.setRotation(0);
    }

    if (get<1>(this->ships[shipNames::PatrolBoat]) == 1) {
        patrolBoat.setPosition(sf::Vector2f((160 + (get<0>(this->ships[shipNames::PatrolBoat]).getX() * 16)) * 5,
                                            (28 + (get<0>(this->ships[shipNames::PatrolBoat]).getY() * 16)) * 5));
        patrolBoat.setRotation(90.f);
    } else {
        patrolBoat.setPosition(sf::Vector2f((128 + (get<0>(this->ships[shipNames::PatrolBoat]).getX() * 16)) * 5,
                                            (28 + (get<0>(this->ships[shipNames::PatrolBoat]).getY() * 16)) * 5));
        patrolBoat.setRotation(0);
    }

    if (get<1>(this->ships[shipNames::RowBoat]) == 1) {
        rowBoat.setPosition(sf::Vector2f((144 + (get<0>(this->ships[shipNames::RowBoat]).getX() * 16)) * 5,
                                         (28 + (get<0>(this->ships[shipNames::RowBoat]).getY() * 16)) * 5));
        rowBoat.setRotation(90.0);
    } else {
        rowBoat.setPosition(sf::Vector2f((128 + (get<0>(this->ships[shipNames::RowBoat]).getX() * 16)) * 5,
                                         (28 + (get<0>(this->ships[shipNames::RowBoat]).getY() * 16)) * 5));
        rowBoat.setRotation(0);
    }
}

void FleetPlacement::resetFleetLayout() {
    static map<spriteNames, sf::Vector2f> positions = {
            {Battleship, sf::Vector2f(21 * 5, 84 * 5)},
            {AircraftCarrier, sf::Vector2f(42 * 5, 92 * 5)},
            {Destroyer, sf::Vector2f(61 * 5, 100 * 5)},
            {Submarine, sf::Vector2f(61 * 5, 35 * 5)},
            {PatrolBoat, sf::Vector2f(42 * 5, 43 * 5)},
            {RowBoat, sf::Vector2f(21 * 5, 51 * 5)},
    };

    // Reset each ship's position and rotation
    for (auto &ship : positions) {
        resources.getSprite(ship.first).setPosition(ship.second);
        resources.getSprite(ship.first).setRotation(0);
    }
}

void FleetPlacement::update() {
    sf::Vector2f mousePosition = State::getMousePosition();

    resources.getButton(Ready).updateButtonState(mousePosition);
    resources.getButton(Randomize).updateButtonState(mousePosition);
    resources.getButton(Instructions).updateButtonState(mousePosition);
}

void FleetPlacement::poll() {
    sf::RenderWindow &gui = *State::gui;
    sf::Event &event = State::event;

    while (gui.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                gui.close();
                break;
            case sf::Event::MouseButtonReleased:
                if ((event.mouseButton.button == sf::Mouse::Left) && (resources.getButton(Ready).getButtonState())) {
                    Gameplay *gameplayInstance = &Gameplay::getInstance();
                    if (State::gameMode == State::GameMode::SINGLE_PLAYER) {
                        gameplayInstance->setP1Grid(ships);
                        this->resetFleetLayout();
                        this->layoutGenerated = false;
                        this->randomize();
                        gameplayInstance->setP2Grid(ships);
                        State::changeScreen(Screens::GAMEPLAY);
                        break;
                    } else {
                        if (State::player == State::Player::P1) {
                            gameplayInstance->setP1Grid(ships);
                            this->resetFleetLayout();
                            this->layoutGenerated = false;
                            State::player = State::Player::P2;
                            State::changeScreen(Screens::INTERMEDIARY);
                            break;
                        } else {
                            gameplayInstance->setP2Grid(ships);
                            this->resetFleetLayout();
                            this->layoutGenerated = false;
                            State::player = State::Player::P1;
                            State::changeScreen(Screens::INTERMEDIARY);
                            break;
                        }
                    }
                } else if ((event.mouseButton.button == sf::Mouse::Left) && (resources.getButton(Randomize).getButtonState())) {
                    this->randomize();
                    this->updateFleetLayout();
                    this->layoutGenerated = true;
                    break;
                } else if ((event.mouseButton.button == sf::Mouse::Left) && (resources.getButton(Instructions).getButtonState())) {
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

void FleetPlacement::render() {
    sf::RenderWindow &gui = *State::gui;
    gui.clear();

    if (State::gameMode == State::SINGLE_PLAYER) {
        gui.draw(resources.getSprite(BackgroundDefault));
    } else {
        if (State::player == State::Player::P1) {
            gui.draw(resources.getSprite(BackgroundP1));
        } else {
            gui.draw(resources.getSprite(BackgroundP2));
        }
    }

    if (layoutGenerated) {
        resources.getButton(Ready).render(gui);
    }

    resources.getButton(Randomize).render(gui);
    resources.getButton(Instructions).render(gui);

    gui.draw(resources.getSprite(Battleship));
    gui.draw(resources.getSprite(AircraftCarrier));
    gui.draw(resources.getSprite(Destroyer));
    gui.draw(resources.getSprite(Submarine));
    gui.draw(resources.getSprite(PatrolBoat));
    gui.draw(resources.getSprite(RowBoat));

    if (State::getCurrentScreen() == Screens::FLEET_PLACEMENT) {
        gui.display();
    }
}

FleetPlacement &screen::FleetPlacement::getInstance() {
    if (instance == nullptr) {
        instance.reset(new FleetPlacement());
    }
    return *instance;
}
