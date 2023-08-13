// Name: Kevin Barrios
// NSHE ID: 2001697903
// Assignment 5 word classification (custom hashMap)
//  implement our custom hashMap structure 
#include "hashMap.h"
#include "hashMap.cpp"
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

//struct of a KeyWord which will be used in our map 
struct KeyWord 
{
	std::string word; // word we are looking for 
	bool seen; // if we have seen this word already
	int cat; // what category does the word belong to 
};
int main()
{
	
	std::string temp; // we use this string to read from std::cin 
	
	// these two int will track our test cases and how many categories we have
	int noAbstracts, noCategories;
	std::cin >> temp;
	noAbstracts = std::stoi(temp);
	
	// as soon as we get number of abstracts we want to start a loop to 
	// perform this procedure for each test case 
	for(int testCase = 0; testCase < noAbstracts; testCase++)
	{
		// get number of categories for current test case 
		std::cin >> temp; 
		noCategories = std::stoi(temp);
		
		//*some variable definitions*//
		// this array will hold how many extact matches we need per cat 
		int noWordsToMatch[noCategories];
		// this array will track what 
		// words we have found and at the end 
		// will have to match how many times we have seen the word 
		int WordsFound[noCategories];
		// this vector will hold our categories names for output 
		std::vector<std::string> CatNames;
		// this is our main map that will allow us to search for key words
		hashMap<std::string, KeyWord> Daisy;
		//*end of some variable definitions*//
		
		// for every categorie we will parse the input and fill up the map 
		for(int x = 0; x < noCategories; x++)
		{
			std::cin >> temp; 
			CatNames.push_back(temp);
			std::cin >> temp; 
			int wordcnt = std::stoi(temp);
			std::cin >> temp; 
			noWordsToMatch[x] = std::stoi(temp);
			WordsFound[x] = 0; // initializes our array to 0
			// for every word we will enter it into the map 
			// with the correct category number
			// the "seen" variable is left false by default
			for(int y = 0; y < wordcnt; y++)
			{
				// get word 
				std::cin >> temp; 
				// convert word to lower case for easy matching
				for(int z = 0; z < temp.size(); z++)
					temp[z] = tolower(temp[z]);
				// insert word to map 
				Daisy[temp] = { temp, 0, x }; // x being our category number 
			}
		}
		
	
		// this line helps our follwing logic by getting rid of first line
		std::getline(std::cin,temp); // first line is empty? 
		
		// this while loop will read our entire abstract and stop at empty line 
		while(std::getline(std::cin,temp))
		{
			if(temp.size() == 0)
				break; // break if abstract is over 
			else
			{
				// MAIN PART OF PROGRAM
				// first we parse the abstract word by word
				// then we search each word 
				// and update each find accordingly 
				
				//"tempWord" is just the current word from abstract that we are searching
				std::string tempWord = ""; 
				int n = temp.size();
				for(int i = 0; i < n; i++)
				{
					if(i == (n-1)) // last character of the line
					{
						//make word 
						if(temp[i] != ' ') // MAKES SURE WE DONT ADD EXTRA SPACE
							tempWord += temp[i];
						//convert to lower case 
						for(int ugh = 0; ugh < tempWord.size(); ugh++)
							tempWord[ugh] = tolower(tempWord[ugh]);
						//TIME TO SEARCH 
						bool nullKeyWord = false;
						if(Daisy[tempWord].word == "")
							nullKeyWord = true; 
						if(!nullKeyWord && Daisy[tempWord].seen == false)
						{
							WordsFound[Daisy[tempWord].cat] += 1;
							Daisy[tempWord].seen = true;
						}
						// search over, reset temp word 
						tempWord = "";
					}
					else if(temp[i] == ' ') // space found so a word is made
					{
						// convert to lower case 
						// SORRY FOR USING "UGH" 
						for(int ugh = 0; ugh < tempWord.size(); ugh++)
							tempWord[ugh] = tolower(tempWord[ugh]);
						//TIME TO SEARCH 
						bool nullKeyWord = false;
						if(Daisy[tempWord].word == "")
							nullKeyWord = true; 
						if(!nullKeyWord && Daisy[tempWord].seen == false)
						{
							WordsFound[Daisy[tempWord].cat] += 1;
							Daisy[tempWord].seen = true;
						}
						// reset temp word 
						tempWord = "";
					}
					else // a word hasn't been found yet
						tempWord += temp[i];
				}
			} // end of current line in abstract 
		} // end of abstract 
		
		// check if we have matches :) 
		std::cout << "Test case " << testCase+1 << std::endl;
		bool catFound = false; // at least one category found 
		for(int x = 0; x < noCategories; x++)
		{
			if(WordsFound[x] == noWordsToMatch[x])
			{
				std::cout << CatNames[x] << std::endl;
				catFound = true;
			}
		}
		if(!catFound) // not a single cat matched
			std::cout << "SQF Problem" << std::endl;
	}
	
	return 0;
}