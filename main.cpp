/* This is the console executable that makes use of the BullCow class
This acts as the view in a MVC pattern, and is responsible for all
user interaction. For game logic see the FBullCowGame class.
*/
#pragma once

#include <iostream>
#include <string>
#include "FBullCowGame.h"

// to make syntax Unreal friendly
using FText = std::string;
using int32 = int;

// function prototypes as outside a class
void PrintIntro();
void PlayGame();
void PrintGameSummary();
FText GetValidGuess();
bool AskToPlayAgain();

FBullCowGame BCGame; // make instance of new game, which we re-use across plays

// entry point of the application
int main()
{
	bool bResponse = false;
	do {
		PrintIntro();
		PlayGame();
		bResponse = AskToPlayAgain();
	} 
	while (bResponse);
	std::cout << "\nThanks for playing.\n";
	return 0;  // exit the application
}

void PrintIntro() {
	std::cout << "\n\nWelcome to Bulls and Cows, a fun word game.\n\n";
	std::cout << "         }___{       ___ \n";
	std::cout << "        <(o o)>    <(o o)> \n";
	std::cout << "  /-------\\ /        \\ /-------\\ \n";
	std::cout << " / | BULL |O          O|  COW  |\\ \n";
	std::cout << "*  |-,----|            |-----UU| * \n";
	std::cout << "   ^      ^            ^       ^ \n\n";
	std::cout << "Can you guess the " << BCGame.GetHiddenWordLength();
	std::cout << " letter isogram I'm thinking of?\n";
	return;
}

// plays a single game to completion
void PlayGame()
{	
	BCGame.Reset();
	int32 MaxTries = BCGame.GetMaxTries();

	
	// loops asking for guesses while the game 
	// is NOT won and there are still tries remaining
	while (!BCGame.IsGameWon() && BCGame.GetCurrentTry() <= MaxTries) {
		FText Guess = GetValidGuess();

		// submit valid guess to the game and receive counts
		FBullCowCount BullCowCount = BCGame.SubmitValidGuess(Guess);

		std::cout << "Bulls = " << BullCowCount.Bulls;
		std::cout << ". Cows = " << BullCowCount.Cows << "\n";
	}
	
	PrintGameSummary();
	return;
}

// loop continuously until user gives a valid guess
FText GetValidGuess() 
{
	FText Guess = "";
	EGuessStatus Status = EGuessStatus::Invalid_Status;
	do {
		// get a guess from the player
		int32 CurrentTry = BCGame.GetCurrentTry();
		std::cout << "\nTry " << CurrentTry << " of " << BCGame.GetMaxTries() << ". Enter your guess: ";
		getline(std::cin, Guess);

		Status = BCGame.CheckGuessValid(Guess);
		switch (Status)
		{
		case EGuessStatus::Wrong_Length:
			std::cout << "Please enter a " << BCGame.GetHiddenWordLength() << " letter word.\n\n";
			break;
		case EGuessStatus::Not_Isogram:
			std::cout << "Please enter an isogram (no letters repeated).\n\n";
			break;
		case EGuessStatus::Not_Lowercase:
			std::cout << "Please use only lowercase letters (no spaces).\n\n";
			break;
		default:
			// assume the guess is valid
			break;
		}
	} while (Status != EGuessStatus::OK); // keep looping until we get no errors
	return Guess;
}

bool AskToPlayAgain()
{
	std::cout << "Do you want to play again with the same word (y/n)? ";
	FText Response = "";
	getline(std::cin, Response);
	return ((Response[0] == 'y') || (Response[0] == 'Y'));
}

void PrintGameSummary()
{
	if (BCGame.IsGameWon() == true)
	{
		std::cout << "\nCongratulations, you won!\n\n";
	}
	else
	{
		std::cout << "\nBad luck, you didn't guess the word.\n\n";
	}
	return;
}
