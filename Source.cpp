#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
using namespace std;

//Global Variables
bool isSaving = false;
bool isLoading = false;
sf::Texture texture;
sf::Texture rules;
const int xMax = 16;
const int yMax = 16;
//typedef int[][] terrain; //flag

int main()
{
    // create the window
    sf::ContextSettings settings;
    sf::RenderWindow window(sf::VideoMode(812, 512), "My Original Map (Do not steal)", sf::Style::Default, settings);
    window.setFramerateLimit(60);

    // variables
    float tempXmap = 0;
    float tempYmap = 0;
    float tempXmenu = 0;
    float tempYmenu = 0;
    float mouseSize = 1;
    int map[xMax][yMax];
    sf::Vector2f tempPosMap;
    sf::Vector2f tempPosMenu;
    int menu[3][4];
    int mouseSelection=0;
    int brushMode=0;
    sf::RectangleShape brush (sf::Vector2f(mouseSize,mouseSize));

    //various methods
    sf::Sprite drawItem(sf::Vector2f, int, int);
    void saveLevel(string, int[][yMax]);
    void loadLevel(string, int[][yMax]);

    //populate map to prevent undefined errors
    for (int i = 0; i < xMax; i++) {
        for (int j = 0; j < yMax; j++) {
            map[i][j] = 0;
        }
    }


    //populate menu
    //iterates through each menu slot to place the next consecutive tile type
    int tempItem = 1;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            menu[i][j] = tempItem++;
        }
    }

    //load sprites from file
    if (!texture.loadFromFile("Sprites.png"))
    {
 
    }
    texture.setRepeated(false);

    //load rules texture
    if (!rules.loadFromFile("Rules.png"))
    {

    }
    rules.setRepeated(false);
    sf::Sprite instruction;
    instruction.setTexture(rules);
    instruction.setPosition(sf::Vector2f(0,320));

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        //load level from text
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::L) && !isLoading) {
            isLoading = true;
            string fileName;
            cout << "what level would you like to load?\n";
            cin >> fileName;
            loadLevel(fileName,map);
        }


        //change brush
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            brushMode = 0;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            brushMode = 1;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
            brushMode = 2;
        }

        //change brush size
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            mouseSize+=0.4;
            if (mouseSize > 128)mouseSize = 128;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            mouseSize-=0.4;
            if (mouseSize < 1)mouseSize = 1;
        }
        brush.setSize(sf::Vector2f(mouseSize, mouseSize));
        brush.setOrigin(mouseSize/2.f, mouseSize/2.f);
        brush.setPosition(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
        brush.setFillColor(sf::Color(0, 0, 0, 0));
        brush.setOutlineThickness(2.f);
        brush.setOutlineColor(sf::Color(0, 0, 0));

        //mouse 
        if (sf::Mouse::getPosition(window).x >= 0 &&
            sf::Mouse::getPosition(window).x <= 812 &&
            sf::Mouse::getPosition(window).y >= 0 &&
            sf::Mouse::getPosition(window).y <= 512) {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                if (sf::Mouse::getPosition(window).x <= 300 && sf::Mouse::getPosition(window).y <= 320) {  //checks if mouse is in menu
                    //use menu to change mouseSelection
                    mouseSelection = menu[sf::Mouse::getPosition(window).x / 100][sf::Mouse::getPosition(window).y / 80];
                    brushMode = 0;
                }
                else if (sf::Mouse::getPosition(window).x > 300) {
                    switch (brushMode) {
                        //samples current selected map area
                    case 1:
                        mouseSelection = map[(sf::Mouse::getPosition(window).x - 300) / 32][sf::Mouse::getPosition(window).y / 32];
                        brushMode = 0;
                        break;
                        //erases current selected map area
                    case 2:
                        //map[(sf::Mouse::getPosition(window).x-300)/ 50][sf::Mouse::getPosition(window).y / 50]=0;
                        for (int i = ((sf::Mouse::getPosition(window).x - mouseSize / 2 - 300) / 32);
                            i <= ((sf::Mouse::getPosition(window).x + mouseSize / 2 - 300) / 32);
                            i++) {
                            for (int j = ((sf::Mouse::getPosition(window).y - mouseSize / 2) / 32);
                                j <= ((sf::Mouse::getPosition(window).y + mouseSize / 2) / 32);
                                j++) {
                                map[i][j] = 0;
                            }
                        }
                        break;
                        //changes map based on current mouseSelection
                    case 0:
                    default:
                        for (int i = ((sf::Mouse::getPosition(window).x - mouseSize / 2 - 300) / 32);
                            i <= ((sf::Mouse::getPosition(window).x + mouseSize / 2 - 300) / 32);
                            i++) {
                            for (int j = ((sf::Mouse::getPosition(window).y - mouseSize / 2) / 32);
                                j <= ((sf::Mouse::getPosition(window).y + mouseSize / 2) / 32);
                                j++) {
                                map[i][j] = mouseSelection;
                            }
                        }
                        break;
                    }
                }
            }
        }

        //save level to text
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !isSaving) {
            isSaving = true;
            string saveName;
            cout << "what would you like to name your level?\n";
            cin >> saveName;
            saveLevel(saveName,map);
        }

        
        // clear the window with green color
        window.clear(sf::Color::Green);

        //draw menu border
        window.draw(sf::RectangleShape(sf::Vector2f(300.f, 512.f)));
        
        //draw menu here
        for (int i = 0; i < 3; i++) {  //iterates through all map items
            for (int j = 0; j < 4; j++) {
                tempXmenu = i * 100+18; //offsets items to match menu scale
                tempYmenu = j * 80+8;
                tempPosMenu = sf::Vector2f(tempXmenu, tempYmenu);
                window.draw(drawItem(tempPosMenu, menu[i][j], 2));
            }
        }

        // draw map here...
        for (int i = 0; i < xMax; i++) {  //iterates through all map items
            for (int j = 0; j < yMax; j++) {
                tempXmap = i*32+300; //offsets items to match map scale
                tempYmap = j*32;
                tempPosMap = sf::Vector2f(tempXmap, tempYmap);
                window.draw(drawItem(tempPosMap, map[i][j], 1));
            }
        }

        //draw instructions
        window.draw(instruction);

        //draw mouse icon here
        window.draw(brush);


        // end the current frame
        window.display();
    }

    return 0;
}

sf::Sprite drawItem(sf::Vector2f pos, int item = 0, int size = 1) {
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setPosition(pos);
    sprite.setScale(sf::Vector2f(size, size));
    switch (item) {
        case 1:
            sprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
            break;

        case 2:
            sprite.setTextureRect(sf::IntRect(0, 32, 32, 32));
            break;
            
        case 3:
            sprite.setTextureRect(sf::IntRect(0, 64, 32, 32));
            break;

        case 4:
            sprite.setTextureRect(sf::IntRect(32, 0, 32, 32));
            break;

        case 5:
            sprite.setTextureRect(sf::IntRect(32, 32, 32, 32));
            break;

        case 6:
            sprite.setTextureRect(sf::IntRect(32, 64, 32, 32));
        break;

        case 7:
            sprite.setTextureRect(sf::IntRect(64, 0, 32, 32));
            break;

        case 8:
            sprite.setTextureRect(sf::IntRect(64, 32, 32, 32));
            break;

        case 9:
            sprite.setTextureRect(sf::IntRect(64, 64, 32, 32));
            break;

        case 10:
            sprite.setTextureRect(sf::IntRect(96, 0, 32, 32));
            break;

        case 11:
            sprite.setTextureRect(sf::IntRect(96, 32, 32, 32));
            break;

        case 12:
            sprite.setTextureRect(sf::IntRect(96, 64, 32, 32));
            break;

        default:
            sprite.setColor(sf::Color(0, 0, 0, 0));
        break;
    }
    return sprite;
}

void saveLevel(string name, int map[][yMax]) {
    ofstream myfile;
    myfile.open(name + ".txt");
    string line = "";
    for (int i = 0; i < xMax; i++) {
        for (int j = 0; j < yMax; j++) {
            line += '0'+map[i][j];
        }
    }
    myfile << line << "\n";
    myfile.close();
    //cout << line;
    isSaving = false;
}

void loadLevel(string name, int map[][yMax]) {
    string line;
    ifstream myfile (name+".txt");
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            cout << line << endl;
            for (int i = 0; i < xMax; i++) {
                for (int j = 0; j < yMax; j++) {
                    int temp = (i * yMax) + j;
                    char c = line[temp];
                    //cout << c;
                    map[i][j] = c - '0';
                }
            }
        }
        myfile.close();
    }
    else cout << "Unable to open file";
    isLoading = false;
}