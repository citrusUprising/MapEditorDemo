#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
using namespace std;

//Global Variables
bool isSaving = false;
bool isLoading = false;
sf::Texture texture;
sf::Texture rules;
const int xMax = 64;
const int yMax = 64;
//typedef int[][] terrain; //flag

int main()
{
    // create the window
    sf::ContextSettings settings;
    sf::RenderWindow window(sf::VideoMode(940, 640), "My Original Map (Do not steal)", sf::Style::Default, settings);
    window.setFramerateLimit(60);

    // variables
    float tempXmap = 0;
    float tempYmap = 0;
    float tempXmenu = 0;
    float tempYmenu = 0;
    int map[xMax][yMax];
    sf::Vector2f tempPosMap;
    sf::Vector2f tempPosMenu;
    int menu[3][4];
    int mouseSelection=0;
    int brushMode=0;

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

        //mouse 
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            if (sf::Mouse::getPosition(window).x <= 300 && sf::Mouse::getPosition(window).y <= 320) {  //checks if mouse is in menu
                //use menu to change mouseSelection
                mouseSelection = menu[sf::Mouse::getPosition(window).x/100][sf::Mouse::getPosition(window).y / 80];
                brushMode = 0;
            }
            else if (sf::Mouse::getPosition(window).x > 300){
                 switch (brushMode) {
                    //samples current selected map area
                    case 1:
                        mouseSelection = map[(sf::Mouse::getPosition(window).x-300)/ 10][sf::Mouse::getPosition(window).y / 10];
                        brushMode = 0;
                    break;
                    //erases current selected map area
                    case 2:
                        map[(sf::Mouse::getPosition(window).x-300)/ 10][sf::Mouse::getPosition(window).y / 10]=0;
                    break;
                    //changes map based on current mouseSelection
                    case 0:
                    default:
                        map[(sf::Mouse::getPosition(window).x-300)/ 10][sf::Mouse::getPosition(window).y / 10]=mouseSelection;
                        break;
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
        window.draw(sf::RectangleShape(sf::Vector2f(300.f, 640.f)));
        
        //draw menu here
        for (int i = 0; i < 3; i++) {  //iterates through all map items
            for (int j = 0; j < 4; j++) {
                tempXmenu = i * 100+15; //offsets items to match menu scale
                tempYmenu = j * 80+5;
                tempPosMenu = sf::Vector2f(tempXmenu, tempYmenu);
                window.draw(drawItem(tempPosMenu, menu[i][j], 7));
            }
        }

        // draw map here...
        for (int i = 0; i < xMax; i++) {  //iterates through all map items
            for (int j = 0; j < yMax; j++) {
                tempXmap = i*10+300; //offsets items to match map scale
                tempYmap = j*10;
                tempPosMap = sf::Vector2f(tempXmap, tempYmap);
                window.draw(drawItem(tempPosMap, map[i][j], 1));
            }
        }

        //draw instructions
        window.draw(instruction);

        //draw mouse icon here
        if (mouseSelection != 0 && brushMode != 2) {
            window.draw(drawItem(
                sf::Vector2f(
                (float)sf::Mouse::getPosition(window).x,
                (float)sf::Mouse::getPosition(window).y),
                mouseSelection, 5));
        }

        // end the current frame
        window.display();
    }

    return 0;
}

sf::Sprite drawItem(sf::Vector2f pos, int item = 0, int size = 5) {
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setPosition(pos);
    sprite.setScale(sf::Vector2f(size, size));
    switch (item) {
        case 1:
            sprite.setTextureRect(sf::IntRect(0, 0, 10, 10));
            break;

        case 2:
            sprite.setTextureRect(sf::IntRect(0, 10, 10, 10));
            break;
            
        case 3:
            sprite.setTextureRect(sf::IntRect(0, 20, 10, 10));
            break;

        case 4:
            sprite.setTextureRect(sf::IntRect(10, 0, 10, 10));
            break;

        case 5:
            sprite.setTextureRect(sf::IntRect(10, 10, 10, 10));
            break;

        case 6:
            sprite.setTextureRect(sf::IntRect(10, 20, 10, 10));
        break;

        case 7:
            sprite.setTextureRect(sf::IntRect(20, 0, 10, 10));
            break;

        case 8:
            sprite.setTextureRect(sf::IntRect(20, 10, 10, 10));
            break;

        case 9:
            sprite.setTextureRect(sf::IntRect(20, 20, 10, 10));
            break;

        case 10:
            sprite.setTextureRect(sf::IntRect(30, 0, 10, 10));
            break;

        case 11:
            sprite.setTextureRect(sf::IntRect(30, 10, 10, 10));
            break;

        case 12:
            sprite.setTextureRect(sf::IntRect(30, 20, 10, 10));
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