#pragma once

#include "FBullCowGame.h"
#include <string>
#include <map>

// to make syntax Unreal friendly
#define TMap std::map
using int32 = int;

FBullCowGame::FBullCowGame() { Reset();	} // default constructor

int32 FBullCowGame::GetCurrentTry() const { return MyCurrentTry; }
int32 FBullCowGame::GetHiddenWordLength() const { return MyHiddenWord.length(); }
bool FBullCowGame::IsGameWon() const { return bGameIsWon; }

int32 FBullCowGame::GetMaxTries() const 
{ 
	TMap<int32, int32> WordLengthToMaxTries{ {3, 4}, {4, 7}, {5, 10}, {6, 15}, {7, 20} };
	return WordLengthToMaxTries[MyHiddenWord.length()]; 
}

void FBullCowGame::Reset()
{
	const FString HIDDEN_WORD = "plane"; // this MUST be an isogram
	MyHiddenWord = HIDDEN_WORD;

	MyCurrentTry = 1;
	bGameIsWon = false;
	return;
}

EGuessStatus FBullCowGame::CheckGuessValid(FString Guess) const
{
	if (!IsIsogram(Guess)) // if the guess isn't an isogram
	{
		return EGuessStatus::Not_Isogram;
	} 
	else if (!IsLowercase(Guess)) // if the guess isn't all lowercase 
	{
		return EGuessStatus::Not_Lowercase;
	}
	else if (Guess.length() != GetHiddenWordLength()) // if the guess length is wrong
	{
		return EGuessStatus::Wrong_Length;
	}
	else 
	{
		return EGuessStatus::OK;
	}
}

// receives a VALID guess, increments turn, and returns count
FBullCowCount FBullCowGame::SubmitValidGuess(FString Guess)
{
	MyCurrentTry++;
	FBullCowCount BullCowCount;
	int32 WordLength = MyHiddenWord.length(); // assuming same length as guess

	// loop through all letters in the hidden word

	for (int32 MHWChar = 0; MHWChar < WordLength; MHWChar++) { // My Hidden Word Character
		// compare letters against the guess
		for (int32 GChar = 0; GChar < WordLength; GChar++) { // Guess Word Character
			// if they match then
			if (Guess[MHWChar] == MyHiddenWord[GChar])
			{
				if (MHWChar == GChar) // if they're in the same place
				{
					BullCowCount.Bulls++; // increment bulls
				}
				// or else
				else
				{
					BullCowCount.Cows++; // must be a cow
				}
			}
		}
	}
	if (BullCowCount.Bulls == WordLength) 
	{
		bGameIsWon = true;
	}
	else
	{
		bGameIsWon = false;
	}
	return BullCowCount;
}

// check if the word is an isogram and return true if it is
bool FBullCowGame::IsIsogram(FString Word) const
{
	// treat 0 and 1 letter words as isograms
	if (Word.length() <= 1) { return true; }

	TMap<char, bool> LetterSeen; // set up our map
	for (auto Letter : Word) // for all letters of the word
	{
		Letter = tolower(Letter); // handle mixed-case words

		if (LetterSeen[Letter]) { return false; } // it's NOT an isogram
		else { LetterSeen[Letter] = true; } // add the letter to the map as 'seen'	
				
	}

	return true; // for example, in cases where \0 is entered
}

// check if word is all lowercase, isn't 0 length or contains '\0' or space - return true
bool FBullCowGame::IsLowercase(FString Word) const
{
//	if (Word.length() < 1) { return true; } // zero length words

	for (auto Letter : Word) // for all letters of the word
	{
		if (!islower(Letter)) { return false; } // letter is NOT lowercase
//		else if (Letter = ' ') { return false; } // Spaces can't be included
	}
	return true;
}
