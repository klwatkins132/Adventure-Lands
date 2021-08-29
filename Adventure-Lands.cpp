/**Kelsy Watkins 
RPG game
This game starts with asking a player their name then starting the first level, Easter Island. In Easter Island there are three tunnel entry options,
Tunnel 1: Play the number guessing game challenge
Tunnel 2: Collect a health Potion
Tunnel 3: Empty room

After completing the first challenge a player can go to the next level Puma PUnku. In this level there are two options initally,
Option 1: Go back to previous level
Option 2: Explore
If a user decides to explore there are two other options
Option 1: Do the challenge
option 2: Unexpected earthquake
The challenge here is hangman and only by guessing the word correctly allows the game to end completly.
If a player dies in the earthquake they are asked whether or not they would like to play again.

A player can move freely between the levels and is given the opportunity to collect or not collect the health potion.
A player can open their backpack at most prompts and see items collected throughout playing the game.

**/

#ifdef _WIN32
#include<Windows.h>
#else
#include<unistd.h>
#endif

#include <string>
#include <ctime>
#include <algorithm>
#include <iostream>  
#include <conio.h>
#include <stdlib.h>
#include <thread>
#include <chrono>
#include <vector>
#include <fstream>

using namespace std;

class EasterIslandProgress {
public:
	bool tunnelOneComplete;
	bool tunnelTwoComplete;
	bool tunnelThreeComplete;
} easterIslandProgress;

class PumaPunkuProgress {
public:
	bool optionOneComplete;
	bool pumaPunkuFirstTime;
	bool usedHealthPotion;
} pumaPunkuProgress;

class Backpack {
public:
	bool healthPotion;
	bool artifact;
} backpack;

int health = 100;
char name[50];
bool error, isHangmanRunning;

//forward declaration
void pumaPunku();
void start( bool firstTime);

void showBackpack() {
	cout << "\n***********************************************************\n" << endl;

	if (backpack.healthPotion) {
		cout << "Potions: Health Potion" << endl;
	}

	if (backpack.artifact) {
		cout << "Artifacts: Ancient Key" << endl;
	}

	if (!backpack.healthPotion && !backpack.artifact) {
		cout << "Looks like your backpack is empty!!" << endl;
	}

	cout << "\n***********************************************************\n" << endl;
}

//get all words in file and store to vector
int getWordsInFile(vector<string> words) {
	return words.size();
}

//get random word in file 
string getRandomWordInFile() {
	
	ifstream file;
	try {
		file.open("wordBank.txt");
	}

	//catch any exception and print out error message
	catch (...) {
		cout << "Error" << endl;
		isHangmanRunning = false;
	}

	string lineInFile;
	vector<string> wordsVector;

	while (getline(file, lineInFile)) {
		wordsVector.push_back(lineInFile);
	}

	srand(time(NULL));

	int wordsLength = getWordsInFile(wordsVector);

	string word = wordsVector.at(rand() % wordsLength);

	return word;
}

//replace underscores with matched letters in word
string formatGameBoardOutput(string word, vector<int> charPosition, char updateLetter) {
	
	vector<char> wordVector(word.length());

	//loop over word and check if user input matches any characters in word
	for (int i = 0; i < word.length(); i++) {
		vector<int>::iterator letterFound = find(charPosition.begin(), charPosition.end(), i);

		//if letter was found replace underscore with letter
		if (letterFound != charPosition.end()) {
			wordVector[i] = updateLetter;
			charPosition.erase(letterFound);
		}
		else {
			wordVector[i] = word[i];
		}
	}
	// convert wordVector to string
	string result(wordVector.begin(), wordVector.end());

	return result;
}

//find index of letter in the word
vector<int> findIndex(string input, char letter) {
	
	vector<int> characterPosition;

	for (int i = 0; i < input.size(); i++) {

		if (input[i] == letter) {
			characterPosition.push_back(i);
		}
	}

	return characterPosition;
}

// starting game 
void startHangmanGame() {
	
	isHangmanRunning = true;

	while (isHangmanRunning) {

		int playOrNot;
		string hiddenWord, fileInput;

		cout << "\t >> Press '1' to play this decipher challenge: " << endl;
		cout << "\t >> Press '2' to go back to the game." << endl;
		cout << "\nEnter your choice: ";
		cin >> playOrNot;

		if (playOrNot == 1) {

			//place underscore for word length in file
			fileInput = getRandomWordInFile();
			for (int j = 0; j < fileInput.length(); j++) {
				hiddenWord += "_";
			}

			bool guessTheWholeWord = false;

			int numberOfTries = fileInput.length() + 1;

			//while still have tries and word hasnt been guessed
			while ((numberOfTries > 0) && (guessTheWholeWord == false)) {

				char letter;

				cout << "\nIf you would like to guess the animal at any time, press '1'." << endl;
				cout << "Number of tries left: " << numberOfTries << endl;
				cout << "\nGuess this animal: " << hiddenWord << endl;
				cout << "Enter a letter you think is in this animals name: ";
				cin >> letter;

				//if user wants to guess word
				if (letter == '1') {
					string guess;
					cout << "What is your guess? ";
					cin >> guess;

					if (fileInput == guess) {
						cout << "You guessed the animal right!" << endl;
						cout << "\nThe animal on the wall was: " << fileInput << endl;
						cout << "\nYOU GUESSED IT AND ACQUIRED AN ANCIENT ARTIFACT!!!" << endl;
						cout << "\nKeep the artifact close to your side, you'll need it in the next challenge." << endl;
						cout << "\nYour one step closer to finding the forbidden treasure " << name << "." << endl;
						cout << "Stay tuned for your next assignemnt, until then, this adventure is now ENDING.";
						cout << "\n" << "\n" << "\n";
						return;
					}

					else {
						cout << "\n***********************************************************\n" << endl;
						cout << "That was not the correct animal." << endl;
						cout << "\n***********************************************************" << endl;

						numberOfTries--;
					}
				}

				//if letter is in word
				else {
					size_t letterFound = fileInput.find(letter);

					if (letterFound != string::npos) {
						cout << "\n-----------------------------------\n" << endl;
						cout << "\tThat letter is in the animals name!" << endl;
						cout << "\n------------------------------------\n";

						vector<int> characterPosition = findIndex(fileInput, letter);

						hiddenWord = formatGameBoardOutput(hiddenWord, characterPosition, letter);

						if (hiddenWord == fileInput) {
							guessTheWholeWord = true;
							break;
						}
					}

					else {
						cout << "\n-------------------------------\n" << endl;
						cout << "\tThat letter is not in the word." << endl;
						cout << "\n-------------------------------\n" << endl;
					}
					numberOfTries--;
				}
			}

			if (numberOfTries != 0) {
				cout << "You guessed the animal right!" << endl;
				cout << "\nThe animal on the wall was: " << fileInput << endl;
				cout << "\nYOU GUESSED IT AND ACQUIRED AN ANCIENT ARTIFACT!!!" << endl;
				cout << "\nKeep the artifact close to your side, you'll need it in the next challenge." << endl;
				cout << "\nYour one step closer to finding the forbidden treasure " << name << "." << endl;
				cout << "Stay tuned for your next assignemnt, until then, this adventure is now ENDING.";
				cout << "\n" << "\n" << "\n";
				return;
			}

			else {
				cout << "\nYou couldn't figure out the animal. Go back and try again!" << endl;
				cout << "\nThe animal on the wall was: " << fileInput << endl;
				cout << "You return back to Puma Punku.\n";
				isHangmanRunning = false;
				pumaPunku();
				return;
			}
		}

		else if (playOrNot == 2) {
			isHangmanRunning = false;
			pumaPunku();
			return;
		}
	}
}

void easterIsland(bool firstTime = false) {

	int tunnelOneChoice = 0, advanceOrStay = 0, tunnelEntry = 0;

	srand(time(NULL));
	int randNumber = rand() % 100 + 1;
	int userInput = 0;

	if (firstTime) {
		cout << "\nYour first destination is Easter Island.\nThese towering figures built by ancient man stand watch over the Earth. Are these stone giants simply a testament to \nhumanity's past or could they depict visitors from another world? Do they serve a technological purpose that \nmodern man is just now unlocking?...." << endl;
		Sleep(4000);
		cout << "\nIt's your job to go there and do some searching around to see if you can find any clues about the forbidden treasure." << endl;
		cout << "\n--------------------------------------------------------\n" << endl;
		Sleep(4000);
	}

	else {
		cout << "\nWelcome back to Easter Island.";
	}

	do {
		error = false;
		cout << "You approach three easter island heads see each one has a tunnel underneath them." << endl;
		cout << "\t >> Press '1' to enter the first tunnel." << endl;
		cout << "\t >> Press '2' to enter the second tunnel." << endl;
		cout << "\t >> Press '3' to enter the third tunnel." << endl;
		cout << "\t >> Press '4' to open your backpack." << endl;
		cout << "\nEnter your choice: ";
		cin >> tunnelEntry;

		if (cin.fail() || (tunnelEntry != 1 && tunnelEntry != 2 && tunnelEntry != 3 && tunnelEntry != 4)) {
			cout << "\n \t \t \t       *****************************\n      " << endl;
			cout << "\t \t \t ****** Please enter a valid interger ******\n" << endl;
			cout << "\t \t\t       *****************************     \n " << endl;
			error = true;
			cin.clear();
			cin.ignore(256, '\n');
			Sleep(4000);
		}

		if (tunnelEntry == 4) {
			showBackpack();
			error = true;
			cin.clear();
			cin.ignore(256, '\n');
		}

	} while (error == 1);

	if (tunnelEntry == 1) {

		if (easterIslandProgress.tunnelOneComplete == true) {
			cout << "\n--------------------------------------------------------\n" << endl;
			cout << "Looks like you've already completed the challenge in here.\n ";
			cout << "\n--------------------------------------------------------\n" << endl;
			
			do {
				error = false;
				cout << "\t >> Press '1' to advance to the next level." << endl;
				cout << "\t >> Press '2' to go back and explore the other tunnels." << endl;
				cout << "\t >> Press '3' to open your backpack." << endl;
				cout << "\nEnter your choice: ";
				cin >> advanceOrStay;

				if (cin.fail() || (advanceOrStay != 1 && advanceOrStay != 2 && advanceOrStay != 3)) {
					cout << "\n \t \t \t       *****************************\n      " << endl;
					cout << "\t \t \t ****** Please enter a valid interger ******\n" << endl;
					cout << "\t \t\t       *****************************     \n " << endl;
					error = true;
					cin.clear();
					cin.ignore(256, '\n');
					Sleep(4000);
				}

				if (tunnelEntry == 3) {
					showBackpack();
					error = true;
					cin.clear();
					cin.ignore(256, '\n');
				}

			} while (error == true);

			if (advanceOrStay == 1) {
				pumaPunku();
				return;
			}

			if (advanceOrStay == 2) {
				easterIsland();
				return;
			}

			easterIsland();
			return;
		}

		do {
			error = false;
			cout << "\n------------------------------------------------------------------------------------------------------------------------" << endl;
			cout << "\nThere seems to be something written on the wall...." << endl;
			cout << "\n------------------------------------------------------------------------------------------------------------------------\n" << endl;
			cout << "\t >> Press '1' to read what it says." << endl;
			cout << "\t >> Press '2' to go back and check out the other tunnels." << endl;
			cout << "\t >> Press '3' to open your backpack." << endl;
			cout << "\nEnter your choice: ";
			cin >> tunnelOneChoice;

			if (cin.fail() || (tunnelOneChoice != 1 && tunnelOneChoice != 2 && tunnelOneChoice != 3)) {
				cout << "\n \t \t \t       *****************************\n      " << endl;
				cout << "\t \t \t ****** Please enter a valid interger ******\n" << endl;
				cout << "\t \t\t       *****************************     \n " << endl;
				error = true;
				cin.clear();
				cin.ignore(256, '\n');
				Sleep(4000);
			}

			if (tunnelOneChoice == 3) {
				showBackpack();
				error = true;
				cin.clear();
				cin.ignore(256, '\n');
			}

		} while (error == true);

	}

	if (tunnelOneChoice == 1) {
		cout << "\n--------------------------------------------------------\n" << endl;
		cout << "THE DOOR SLAMS BEHIND YOU!!" << endl;
		cout <<"\nYou take your lantern out to read what's on the wall...\nIt says if you cannot complete this first challenge you'll remain in this room forever." << endl;
		cout << "Some adventureres have spent their lives trying to guess the right number...can you?\n" << endl;
		Sleep(4000);


		do {
			cout << "Enter a number between 1 and 100: ";
			cin >> userInput;

			do {
				error = false;

				if (cin.fail()) {
					cout << "\n \t \t \t       *****************************\n      " << endl;
					cout << "\t \t \t ****** Please enter a valid interger ******\n" << endl;
					cout << "\t \t\t       *****************************     \n " << endl;
					error = true;
					cin.clear();
					cin.ignore(256, '\n');
					break;
					Sleep(4000);
				}

				if (userInput > randNumber) {
					cout << "That guess was too high, try again!" << endl;
				}

				if (userInput < randNumber) {
					cout << "That guess was too low, try again!" << endl;
				}

				else if (userInput == randNumber) {
					pumaPunkuProgress.pumaPunkuFirstTime = true;
					backpack.artifact = TRUE;

					cout << "\n------------------------------------------------------------------------------------------------------------------------" << endl;
					cout << "\nYou did it " << name << "! The Secret Number was " << randNumber << endl;
					cout << "\nYou've acquired a key for completing this challenge -----------> Key +1" << endl;
					cout << "\n------------------------------------------------------------------------------------------------------------------------\n" << endl;
					Sleep(3000);


					do {
						cout << "\t >> Press '1' to advance to the next level." << endl;
						cout << "\t >> Press '2' to go back and explore the other tunnels" << endl;
						cout << "\t >> Press '3' to open your backpack." << endl;
						cout << "\nEnter your choice: ";
						easterIslandProgress.tunnelOneComplete = true;
						cin >> advanceOrStay;

						if (cin.fail() || (advanceOrStay != 1 && advanceOrStay != 2 && advanceOrStay != 3)) {
							cout << "\n      *****************************      " << endl;
							cout << "****** Please enter a valid interger ******" << endl;
							cout << "      *****************************      \n" << endl;
							error = true;
							cin.clear();
							cin.ignore(256, '\n');
							cout << "\t >> Press '1' to advance to the next level." << endl;
							cout << "\t >> Press '2' to go back and explore the other tunnels" << endl;
							cout << "\t >> Press '3' to open your backpack." << endl;
							cout << "\nEnter your choice: ";
							cin >> advanceOrStay;
							Sleep(4000);
						}

						if (advanceOrStay == 1) {
							pumaPunku();
							return;
						}

						if (advanceOrStay == 2) {
							easterIsland();
							return;
						}

						if (advanceOrStay == 3) {
							showBackpack();
							error = true;
							cin.clear();
							cin.ignore(256, '\n');
						}

					} while (error == true);
				}

			} while (error == true);

		} while (userInput != randNumber);
	}

	if (tunnelOneChoice == 2) {
		easterIsland();
		return;
	}

	if (tunnelEntry == 2) {

		if (easterIslandProgress.tunnelTwoComplete == true) {
			cout << "\n------------------------------------------------------------------------------------------------------------------------\n" << endl;
			cout << "Looks like you've already collected the health potion in here.\nYou leave here and go back to check out the other tunnels\n";
			cout << "\n------------------------------------------------------------------------------------------------------------------------" << endl;


			easterIsland();
			return;
		}

		backpack.healthPotion = TRUE;

		cout << "\n--------------------------------------------------------------------------------" << endl;
		cout << "\nYou've acquired a health potion!! You leave this room and go back to the tunnels. -----------> Health Potion +1" << endl;
		cout << "\n---------------------------------------------------------------------------------" << endl;


		easterIslandProgress.tunnelTwoComplete = true;
		easterIsland();
		return;
	}

	if (tunnelEntry == 3) {

		if (easterIslandProgress.tunnelThreeComplete == true) {
			cout << "\n--------------------------------------------------------------------------------" << endl;
			cout << "\nYou've already been in this room and it's empty. You go back to the other tunnels." << endl;
			cout << "\n--------------------------------------------------------------------------------" << endl;

			easterIsland();
			return;
		}

		cout << "\n------------------------------------------------------------------------------------------\n" << endl;
		cout << "Oh no looks like this is just an empty room!! You leave this room to go back to the tunnels." << endl;
		cout << "\n------------------------------------------------------------------------------------------" << endl;

		easterIslandProgress.tunnelThreeComplete = true;
		easterIsland();
		return;
	}
}

void pumaPunku() {

	int keyHoleChoice, advanceOrStay, useHealthPotion, keepPlaying;

	srand(time(NULL));
	int randNumber = rand() % 100 + 1;
	int userInput = 0;

	if (pumaPunkuProgress.pumaPunkuFirstTime) {

		cout << "\n--------------------------------------------------------\n" << endl;
		cout << "Your second destination is Puma Punku." << endl;
		cout << "\nThe technical finesse and precision displayed in these 131 ton, 25 foot long 17 foot wide stone blocks\nis astounding. Not even a razor blade can slide between the rocks!\nSome of these blocks are finished to 'machine' quality and the holes drilled to perfection.\nThis is supposed to have been achieved by a civilization that had no writing system and was\nignorant of the existence of the wheel.\nSomething doesn't add up." << endl;
		cout << "\nIt's your job to find the ancient relic that supposedly lies somewhere amongst these stones." << endl;
		Sleep(4000);

		pumaPunkuProgress.pumaPunkuFirstTime = false;
	}

	else {
		cout << "\nWelcome back to Puma Punku." << endl;
	}

	do {
		error = false;
		cout << "\t >> Press '1' to go back to Easter Island" << endl;
		cout << "\t >> Press '2' to investigate these stones in front of you." << endl;
		cout << "\t >> Press '3' to open your backpack." << endl;
		cout << "\nEnter your choice: ";
		cin >> advanceOrStay;


		if (cin.fail() || (advanceOrStay != 1 && advanceOrStay != 2 && advanceOrStay != 3)) {
			cout << "\n \t \t \t       *****************************\n      " << endl;
			cout << "\t \t \t ****** Please enter a valid interger ******\n" << endl;
			cout << "\t \t\t       *****************************     \n " << endl;
			error = true;
			cin.clear();
			cin.ignore(256, '\n');
			cout << "\t >> Press '1' to go back to Easter Island" << endl;
			cout << "\t >> Press 2 to investigate these stones in front of you.\n" << endl;
			cout << "\t >> Press 3 to open your backpack." << endl;
			cout << "\nEnter your choice: ";
			cin >> advanceOrStay;
			Sleep(4000);
		}

		if (advanceOrStay == 3) {
			showBackpack();
			error = true;
			cin.clear();
			cin.ignore(256, '\n');
		}

	} while (error == true);

	if (advanceOrStay == 1) {
		easterIsland();
	}

	if (advanceOrStay == 2) {

		do {
			error = false;
			cout << "\nYou notice one of these megalithic stones look very different from the others....\nThere seems to be....keyholes in these stones." << endl;
			cout << "\t >> Press '1' to put your key in the first stone." << endl;
			cout << "\t >> Press '2' to put your key in the second stone." << endl;
			cout << "\t >> Press '3' to open your backpack." << endl;
			cout << "\nEnter your choice: ";
			cin >> keyHoleChoice;

			if (cin.fail() || (keyHoleChoice != 1 && keyHoleChoice != 2 && keyHoleChoice != 3)) {
				cout << "\n \t \t \t       *****************************\n      " << endl;
				cout << "\t \t \t ****** Please enter a valid interger ******\n" << endl;
				cout << "\t \t\t       *****************************     \n " << endl;
				error = true;
				cin.clear();
				cin.ignore(256, '\n');
				cout << "\nYou notice one of these megalithic stones look very different from the others....\nThere seems to be....keyholes in these stones." << endl;
				cout << "\t >> Press '1' to put your key in the first stone." << endl;
				cout << "\t >> Press '2' to put your key in the second stone." << endl;
				cout << "\t >> Press '3' to open your backpack." << endl;
				cout << "\nEnter your choice: ";
				cin >> keyHoleChoice;
				Sleep(4000);
			}

			if (keyHoleChoice == 3) {
				showBackpack();
				error = true;
				cin.clear();
				cin.ignore(256, '\n');
			}

		} while (error == true);

		if (keyHoleChoice == 1) {
			cout << "\n------------------------------------------------------------------------------------------" << endl;
			cout << "\nThe key worked!!!" << endl;
			cout << "\n------------------------------------------------------------------------------------------" << endl;
			Sleep(3000);


			cout << "\nThe stone is opening to reveal a petroglyph of an animal on the inside." << endl;
			cout << "This petroglyph is extremely faded from the passing of time." << endl;
			cout << "It's up to you to decipher what animal this is." << endl;
			Sleep(4000);


			startHangmanGame();
		}

		if (keyHoleChoice == 2) {
			cout << "\n------------------------------------------------------------------------------------------" << endl;
			cout << "\nOH NO AND EARTHQUAKE IS STARTING!!!" << endl;
			cout << "\n------------------------------------------------------------------------------------------" << endl;
			Sleep(4000);

			cout << "\nYou stumble to the ground and a rock falls on your head." << endl;

			health -= 50;

			cout << "Your health is now at " << health << "%. \n";
			Sleep(4000);



			if (health == 0) {
				do {
					error = false;
					cout << "\nYOU HAVE DIED ON THIS ADVENTURE." << endl;
					cout << "\t >> Press '1' to play again." << endl;
					cout << "\t >> Press '2' to quit." << endl;
					cout << "\nEnter your choice: ";

					cin >> keepPlaying;

					if (cin.fail() || (keepPlaying != 1 && keepPlaying != 2)) {
						cout << "\n \t \t \t       *****************************\n      " << endl;
						cout << "\t \t \t ****** Please enter a valid interger ******\n" << endl;
						cout << "\t \t\t       *****************************     \n " << endl;
						error = true;
						cin.clear();
						cin.ignore(256, '\n');
						cout << "\nYOU HAVE DIED ON THIS ADVENTURE." << endl;
						cout << "\t >> Press '1' to play again." << endl;
						cout << "\t >> Press '2' to quit." << endl;
						cout << "\nEnter your choice: ";
						cin >> keepPlaying;
						Sleep(4000);
					}

					if (keepPlaying == 1) {

						easterIslandProgress.tunnelOneComplete = false;
						easterIslandProgress.tunnelTwoComplete = false;
						easterIslandProgress.tunnelThreeComplete = false;
						pumaPunkuProgress.optionOneComplete = false;
						pumaPunkuProgress.pumaPunkuFirstTime = false;
						pumaPunkuProgress.usedHealthPotion = false;
						backpack.healthPotion = false;
						backpack.artifact = false;
						health = 100;

						cout << "/n************************************************" << endl;
						cout << "Welcome back " << name << " ." << endl;

						start(false);

					}

					if (keepPlaying == 2) {
						return;
					}

				} while (error == true);

			}

			if (backpack.healthPotion) {
				do {
					error = false;

					cout << "\t >> Press '1' to use your health potion." << endl;
					cout << "\t >> Press '2' to go back to the beginning of puma punku" << endl;
					cout << "\nEnter your choice: ";

					cin >> useHealthPotion;

						if (cin.fail() || (useHealthPotion != 1 && useHealthPotion != 2)) {
							cout << "\n \t \t \t       *****************************\n      " << endl;
							cout << "\t \t \t ****** Please enter a valid interger ******\n" << endl;
							cout << "\t \t\t       *****************************     \n " << endl;
							error = true;
							cin.clear();
							cin.ignore(256, '\n');
							cout << "\t >> Press '1' to use your health potion.\n" << endl;
							cout << "\t >> Press '2' to try the key in another hole." << endl;
							cout << "\nEnter your choice: " << endl;
							cin >> useHealthPotion;
							Sleep(4000);
						}

						if (pumaPunkuProgress.usedHealthPotion == true) {

							cout << "It looks like you used your only health potion. Your health is at " << health << "% for the rest of the game. BE CAUTIOUS." << endl;
							pumaPunku();
							return;						
						}

						if (useHealthPotion == 1) {
							backpack.healthPotion = FALSE;
							pumaPunkuProgress.usedHealthPotion = false;

							health += 50;

							cout << "\n------------------------------------------------------------------------------------------" << endl;
							cout << "\nYou used your health potion!!" << endl;
							cout << "Your health is now back at " << health << "%. " << endl;
							cout << "You return back to the beginning of Puma Punku.\n";
							cout << "\n------------------------------------------------------------------------------------------" << endl;
							Sleep(4000);
							pumaPunku();
							return;
						}

						if (useHealthPotion == 2) {
							pumaPunku();
							return;
						}

				} while (error == true);
			}

			if (!backpack.healthPotion) {
				cout << "\nBe weary your health is low but I believe if you went back and did some searching around\nyou may find a health potion." << endl;
				cout << "\nYou leave here and go back to the beginning of Puma Punku.";
				pumaPunku();
				return;
			}			
		}
	}
}

void start(bool firstTime) {
	if (firstTime) {
		cout << "What is your name, explorer? ";
		cin.getline(name, 50);
	}

	cout << "\n--------------------------------------------------------" << endl;
	cout << "\nThere's no time to waste, " << name << ". You're being hired to travel to the worlds most mysterious locations.\nWe've been given information for the where abouts of the forbidden treasure." << endl;
	cout << "The forbidden treasure holds the secrets of the creation to all of mankind BUT BE WARNED....\n....no one has ever come back from this exploration." << endl;
	Sleep(4000);

	easterIsland(true);
	return;
}

int main() {
	start(true);
	return 0;
}
