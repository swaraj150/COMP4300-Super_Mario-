#include<iostream>
#include "GameEngine.h"
#include<SFML/Graphics.hpp> 



int main() {
	//std::cout << "Hi\n";
	GameEngine g("./bin/assets/assets.txt");
	g.run();
    
    
    //sf::Texture image;
    //if (!image.loadFromFile("C:/Users/Public/Documents/coding/Assignment3/bin/images/megaman/megaStand.png")) {
    //    std::cout << "Failed to load image. Error: "  << std::endl;
    //    return 1; // Return an error code
    //}
    //else {
    //    std::cout << "NOT Failed to load image. Error: " << std::endl;

    //}

 /*   sf::Texture tex;
    tex.loadFromImage(image);*/


}
