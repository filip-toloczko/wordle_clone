/* ---------------------------------------------
Program 3: Wordle Reload

Course: CS 141, Spring 2022. Tues 9am lab
System: Windows and Mac using Replit
Authors: Filip Toloczko and Vladyslav Symonenko
---------------------------------------------
*/
#include <iostream>
#include <cctype>
#include <ctime>
#include <fstream>
#include <cassert>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

//Function used to display the rules for the game
void displayGameRules() {
  cout<< endl << "Program 3: Wordle Reload"<<endl
      <<"CS 141, Spring 2022, UIC"<<endl
      <<"\n"
      <<"The objective of this game is to guess the randomly selected"<<endl
      <<"word within a given number of attempts. You can select either"<<endl
      <<"a three or five word board."<<endl
      <<"At the conlusion of the game, stats will be displayed."<<endl
      <<"Indicators will be given if characters of the user entered"<<endl
      <<"word are reflected in the guessed word."<<endl
      <<"  - If the character is in the correct position, the character"<<endl
      <<"    will display as an uppercase value."<<endl
      <<"  - If the character is within the random word, the character"<<endl
      <<"    will display as a lowercase value."<<endl
      <<"  - If you enter a character that is not in the word, an asterisk '*'"<<endl
      <<"    will display."<<endl
      <<"\n";
}

//Function that instructs how to play the game
void optionPreview(int numLetters, int numAttempts, int timeLimit) {
    cout << "\nTo get started, enter your first " << numLetters << " letter word." << endl
         << "You have " << numAttempts << " attempts to guess the random word." << endl
         << "The timer will start after your first word entry." << endl
         << "Try to guess the word within " << timeLimit << " seconds." << endl;
}

//Function that displays the game menu
void displayMenu() {
  cout << endl << "Select a menu option:" << endl
       << "   1. To play Wordle Reload 3 letter play" << endl
       << "   2. To play Wordle Reload 5 letter play" << endl
       << "   3. Exit the program" << endl;
}

//This function opens the dictionary file and stores the content into a vector
void getWordleWords(vector<string>& wordlewords) {
  ifstream inStream;
  inStream.open("wordlewords.txt");
  assert(inStream.fail() == false);
  string wordleWord;
  
  while(inStream >> wordleWord) {
    wordlewords.push_back(wordleWord);
  }
  inStream.close();
}

//This function chooses a random word to guess for the game
void getWordToGuess(string& wordToGuess, int numLetters, vector<string> wordlewords) {
  int index;
  
  if (numLetters == 3) {
    index = rand()%1065;
  }
  else {
    index = rand()%(wordlewords.size() - 1065) + 1065;
  }
  
  wordToGuess = wordlewords.at(index);
}

//This function performs a binary search to check if the user input is in the dictionary
bool binarySearchString(string searchWord, vector<string> dictionary) {
  int mid;
  int low;
  int high;
  int searchResult;
  // Initialize low and high variables
  switch (searchWord.size()) {
    case 3:
      low = 0;
      high = 1064;
      break;
    case 5:
      low = 1065;
      high = dictionary.size() - 1;
  }
  //Write appropriate while condition below
  while (high >= low) {
    // Calculate the middle of low and high
    mid = (high + low) / 2;
    // Save the compared result between 1) the middle index element of dictionary and 2) searchWord
    searchResult = searchWord.compare(dictionary.at(mid));
    
    if (searchResult > 0) {
      low = mid + 1;
    }
    else if (searchResult < 0) {
      high = mid - 1;
    }
    else {
      return false; // found
    }
  }
  return true; // not found
}

//Function that stores and displays the users attempts
void displayFormattedAttempts(string wordToGuess, string userWord, string &userAttemptsFormatted){
  string userWordFormatted = "";
  char letterFormatted;
  int i;
  
  for (i=0; i < userWord.size(); i++) {
    if (userWord[i] == wordToGuess[i]) {
      letterFormatted = toupper(userWord[i]);
      userWordFormatted = userWordFormatted + " [ " + letterFormatted + " ] ";
    }
    else if (wordToGuess.find(userWord[i]) != std::string::npos) {
      letterFormatted = tolower(userWord[i]);
      userWordFormatted = userWordFormatted + " [ " + userWord[i] + " ] ";
    }
    else {
      userWordFormatted = userWordFormatted +" [ * ] ";
    }
  }
  
  userAttemptsFormatted += userWordFormatted + "\n";
  cout << "\n" << "\n";
  cout << userAttemptsFormatted;
}

//Main function that runs the program
int main() {
  //display game rules
  displayGameRules();
  //random seed
  srand(1);
  //initialize variables
  vector<string> wordlewords;
  getWordleWords(wordlewords);
  int wordsGuessed = 0;
  int longestStreak = 0;
  int currentStreak = 0;
  int overallTimeSpent = 0;
  int option;
  int numLetters;
  int numAttempts;
  int attempt;
  int timeLimit;
  time_t startTime;
  int elapsedSeconds;
  string wordToGuess;
  string userWord;
  string userAttemptsFormatted;

  //while loops that plays the game
  while (true) {
    //display menu options
    displayMenu();
    
    //get option from user
    cout << "Your choice --> ";
    cin >> option;

    //This option ends the game and displays the users stats
    if (option == 3) {
      cout << endl << "Overall Stats:" << endl;
      cout << "  - You guessed: " << wordsGuessed << endl;
      cout << "  - Your longest streak is: " << longestStreak << endl;
      cout << "  - Average word completion time: ";

      //If statement to handle whether or not the user has successfully completed a game
      if (wordsGuessed == 0) {
        cout << "N/A" << endl;
      }
      else {
        cout << overallTimeSpent / wordsGuessed << endl;
      }
      cout << "Exiting program" << endl;
      break;
    }
    //This option initiates the 3 letter game
    else if (option == 1) {
      numLetters = 3;
      numAttempts = 4;
      timeLimit = 20;
    }
    //This option initiates the 5 letter game.
    else if (option == 2) {
      numLetters = 5;
      numAttempts = 6;
      timeLimit = 40;
    }
    //If the user enters an invalid menu option, display an error message
    else {
      cout << "Invalid option. Try again." << endl;
      continue;
    }

    //call functions to display instructions and get random word
    optionPreview(numLetters, numAttempts, timeLimit);
    getWordToGuess(wordToGuess, numLetters, wordlewords);

    //asign values to variables
    attempt = 0;
    elapsedSeconds = 0;
    userAttemptsFormatted = "";
    
    //While loop to check for game conditions
    while (true) {
      //get input word from user
      cout << endl << "Please enter word -->  ";
      cin >> userWord;
      for (int i = 0; i<userWord.size(); i++){
        userWord[i] = tolower(userWord[i]);
      }
      if (attempt > 0) {
        elapsedSeconds = difftime(time(NULL), startTime);
      }
      //check if user ran out of time and entered invalid word
      if (userWord.size() != numLetters) {
        cout << "Invalid word entry - please enter a word that is " << numLetters << " characters long." << endl;
        continue;
      }
      //check if the user input is in the dictionary
      if (binarySearchString(userWord, wordlewords)) {
        cout << endl << "Not a playable word, please select another word." << endl;
        continue;
      }

      //Display the game guesses output 
      displayFormattedAttempts(wordToGuess, userWord, 
      userAttemptsFormatted);
      attempt++;

      //Check if the user has run out time
      if (elapsedSeconds > timeLimit) {
        cout << endl << "Your time has expired.  Try again." << endl;
        cout << "  - You are " << elapsedSeconds - timeLimit << " seconds over the " 
             << timeLimit << " second time limit." << endl;
        currentStreak = 0;
        break;
      }
      //Check if the user has guessed the correct word
      else if (userWord == wordToGuess) {
        cout << endl << "Nice Work!  You guessed the correct word" << endl;
        cout << "  - You completed the board in: " << elapsedSeconds << " seconds." << endl;
        cout << "  - It took you " << attempt << "/" << numAttempts << " attempts." << endl;
        wordsGuessed += 1;
        overallTimeSpent += elapsedSeconds;
        currentStreak += 1;
        break;
      }
      //Check if the user has run out of attempts
      else if (attempt == numAttempts) {
        cout << endl << "Maximum amount of attempts have been reached. Try again." << endl;
        currentStreak = 0;
        break;
      }
      if (attempt == 1) {
        startTime = time(NULL);
      }
    }
    if (currentStreak > longestStreak) {
      longestStreak = currentStreak;
    }
  }
  return 0;
}