#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
void wordleHelper(const std::string& in, const std::set<std::string>& dict, std::string& word, size_t loc, std:: set<std::string>& results, int floatingCount[26], int remainingFloating);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{ 

		//Set of results to return
		std:: set<std::string> results;
		//The word that we are generating, I will set the word equal to the input initially
		std::string word=in;

		//We have to track how many of each floating letters we have to use, for example if floating ="tt", then floatingCount[19]=2; 
		int floatingCount[26]= {0}; 
		int remainingFloating=0; //This is the total floating letters we still need to place in our word 


		//Now, counting each floating letter and initializing the array and the count:
		for (char c : floating){ //for every character in floating
			floatingCount[c-'a']+=1; //incrementing the array's specific value for the character each time the char is encountered
			remainingFloating+=1; 
		}


		//Calling the helper function
		wordleHelper(in, dict, word, 0, results,  floatingCount, remainingFloating); 

		return results;

}


// Define any helper functions here
void wordleHelper(
		const std::string& in,
    const std::set<std::string>& dict,
		std::string& word,
		size_t loc, 
		std:: set<std::string>& results,
		int floatingCount[26], 
		int remainingFloating)
{

	
	//BASECASE: If we have fulfilled the number of letters, we can do the dictionary and floating checks and return
	if (loc==in.length()){
		if (remainingFloating==0 && dict.find(word)!=dict.end()){
			results.insert(word); 
		}
		return;
	}

	//Early Check to return if there are more floats left than empty spots
	if (remainingFloating> (int)(in.length()-loc) ) {
		return; 
	}
	
	 
	//RECURSION 1: If we have encountered a fixed letter
	if (in[loc]!='-'){
		
		//We don't have to set anything because our word was initially set to the input, therefore our fixed letters are already there
		//We recurse to the next location
		wordleHelper(in, dict, word, loc+1, results, floatingCount, remainingFloating);
	}


	//RECURSION 2: If we have encountered a blank location
	else if (in[loc]=='-'){


			//Early Check to return if there are more floats left than empty spots
		if (remainingFloating> (int)(in.length()-loc) ) {
			return; 
		}
	
		for (char ch= 'a'; ch<='z'; ch++){ //We check for all letters


			word[loc]=ch;
			floatingCount[ch-'a']-=1; //we decrement and check whether this is a floating letter

			bool isFloat=false;
			if (floatingCount[ch-'a']>=0){
				isFloat=true;
			}

			//Now we can decrement our remainingFloating Value
			if (isFloat){
				remainingFloating-=1;
			}

			//We recurse to the next location
			if (remainingFloating<= (int)(in.length()-loc) ) {
				wordleHelper(in, dict, word, loc+1, results, floatingCount, remainingFloating);
			}

			//Now we can backtrack because the combination might not work 
			if (isFloat){
				remainingFloating+=1;
			}

			floatingCount[ch-'a']+=1;
			word[loc]='-'; //We go back to the blank position
		}
	}

}









/* THIS IS MY OLD CODE USING VECTORS, THE CODE ABOVE HAS BETTER RUNTIME 

// Add prototypes of helper functions here
void wordleHelper(const std::string& in, const std::set<std::string>& dict, std::string& word, size_t loc, std:: set<std::string>& results, std::vector<int>& remainingFloating, int blanksLeft,int totalRemaining);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{ 

		//Set of results to return
		std:: set<std::string> results;
		//The word that we are generating
		std::string word="";
		word.reserve(in.size())  ;


		//Calculating the number of blanks
		int totalBlanks =0;
		for (char c : in){
			if (c=='-'){
				totalBlanks++;
			}
		}

		//We can convert our floating characters into a vector of integers to reduce runtime
		std::vector<int> remainingFloating(26,0); 
		int totalRemaining=0;
		for (char c: floating){
			remainingFloating[c-'a']++;
			totalRemaining++;
		}

		//Calling the helper function
		wordleHelper(in, dict, word, 0, results, remainingFloating, totalBlanks, totalRemaining); 

		return results;

}


// Define any helper functions here
void wordleHelper(
		const std::string& in,
    const std::set<std::string>& dict,
		std::string& word,
		size_t loc, 
		std:: set<std::string>& results,
		std::vector<int>& remainingFloating,
		int blanksLeft,
		int totalRemaining)
{


		//If it is greater than the number of blanks left, we return
		if (totalRemaining>blanksLeft){
			return; 
		}


		//BASE CASE: If we have generated a word that is the length of the input
		if (loc==in.size()){

					//Final checks for: floating letters, in the dictionary?
					if (totalRemaining==0 && (dict.find(word)!=dict.end())){				//NOTE: Dictionary is a container so our find will return an iterator
						results.insert(word);
					}
					return; //If the word does not pass the restrictions, we return
			}



		//Keep recursing if our current loc is a fixed char
		if (in[loc]!='-'){
					//We insert the character into the word as it is a set value
					word+= in[loc];

					//We check if this character is a floating letter

					int pos =in[loc] -'a'; 
					if (remainingFloating[pos]>0){
						remainingFloating[pos]--;
						wordleHelper(in, dict,word, loc+1, results, remainingFloating, blanksLeft, totalRemaining-1);
						remainingFloating[pos]++;
					
					}else{
					wordleHelper(in, dict, word, loc+1, results, remainingFloating, blanksLeft, totalRemaining);
					}

					word.pop_back();//Backtracting right after we recurse, ready for the next word to be checked
				
			}

		//Now we try all possible characters, 26 of them
		else {

			if (totalRemaining == blanksLeft){
				for (int i=0; i<26; i++){
					if (remainingFloating[i]>0){
						char c='a'+i;
						word+=c;
						remainingFloating[i]--;
						wordleHelper(in, dict,word, loc+1, results, remainingFloating, blanksLeft-1, totalRemaining-1);
						remainingFloating[i]++;
						word.pop_back();
					}
				}
			}

		else{
			
			for ( char c = 'a'; c<= 'z'; c++){
					int pos =c-'a';
					word+= c;
					if (remainingFloating[pos]>0){ //Only if not floating
						remainingFloating[pos]--;
						wordleHelper(in, dict,word, loc+1, results, remainingFloating, blanksLeft-1, totalRemaining-1);
						remainingFloating[pos]++;
					
					}else{
					wordleHelper(in, dict, word, loc+1, results, remainingFloating, blanksLeft-1, totalRemaining);
					}

					word.pop_back();//Backtracting right after we recurse, ready for the next word to be checked
				
		}
		}
	}
}


*/