#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <fstream>

using namespace std;
/* The point of this program is to take in a text file and tell you how many unique words it has.
    It can be set to avoid text in brackets and to consider two words that are the same except for 
    first letter capitalization to be the same or different (ie Hi ==/!= hi). It can take off non 
    letters from the back of a word and from the front. It is just a small program that I wrote 
    when a friend and I heard a very repetitive song and wanted to know how repetitive it was.
    It runs quickly, and was able to process a text file of the Bible (4.416 MB in a text file 
    and 10s of thousands of lines) in roughly 3 seconds (it would depend on the computer of course). 
    It is most likely not a fully completed work, but it was correct from the testing that I did. 
    It is just a cool little project that I decided to do, and shows some of my programming style. 
    I hope that you enjoy it!
*/

void analysis(unordered_map<string, int> tempMap);
string lastChar(string last);

int main()
{
    cout << "Input the file name: ";
    string temp;
    cin >> temp;
    ifstream file;
    file.open(temp);
    if (!file.is_open()) {
        file.close();
        cout << "This file does not exist" << endl;
        exit(1);
    }
    // Reading in the file name and then checking that it exists

    char in;
    bool include, cap;
    cout << "Include text in brackets or square brackets? (y/n): ";
    cin >> in;

    if (in == 'y') {
        include = true;
    }
    else if (in == 'n') {
        include = false;
    }
    else {
        file.close();
        cout << "Error: invalid character" << endl;
        exit(1);
    }
    // Checking if the user wants to include the text in brackets/square brackets and setting values accordingly

    cout << "Should capitalized words be the same as lowercase (hi = Hi)? (y/n): ";
    cin >> in;

    if (in == 'y') {
        cap = true;
    }
    else if (in == 'n') {
        cap = false;
    }
    else {
        file.close();
        cout << "Error: invalid character" << endl;
        exit(1);
    }
    // Checking if the user wants capitalized and lowercase words to be the same, setting values accordingly

    unordered_map<string, int> text;
    unordered_map<string, int>::iterator iter;
    int words = 0;

    while (file >> temp) {
        char firstChar = temp[0];
        if (firstChar == '(' || firstChar == '[' || firstChar == '{') {
            if (firstChar == '(') {
                temp.erase(0);
            }
            else if (!include) {
                if (temp[temp.length() - 1] == ']' || temp[temp.length() - 1] == '}') {
                    continue;
                }

                while (file >> temp) {
                    if (temp[temp.length() - 1] == ']' || temp[temp.length() - 1] == '}') {
                        break;
                    }
                }

                continue;
            }
            else {
                temp.erase(0);
            }
        }
        else if (firstChar > 122 || firstChar < 48 || (firstChar > 90 && firstChar < 97)
            || (firstChar > 57 && firstChar < 65)) {
            temp = temp.substr(1, temp.length() - 1);
        }
        // Checking to see if the first letter is an enclosing character then if it is not a letter/number
        // and dealing with it according to the user settings/process of program
        
        if (cap &&  temp[0] >= 65 && temp[0] <= 90) {
            temp[0] += 32;
        }

        temp = lastChar(temp);

        if (temp == "") {
            continue;
        }
        
        ++words;
        // Uncapitalizing the first letter (if the user wants it so), and then taking off non letter/number
        // end characters, checking if it is just a blank word then, and incrementing the word count

        iter = text.find(temp);

        if (iter == text.end()) {
            text.insert(make_pair(temp, 1));
        }
        else {
            ++iter->second;
        }
        // Assigning the iterator to the word if it is found in the hash table and adding it if it is not
    }

    file.close();

    cout << "This text has " << text.size() << " unique words vs " << words << " words in this text" << endl << endl; 

    cout << "Input a word to find out how many instances of this word are in this text.\nType \"QUIT\" to exit and \"ANALYSIS\" to find out word occurence statistics: ";
    // Closing the file, adding data output, and then explaining the next data section

    while (cin >> temp) {
        if (temp == "QUIT") {
            break;
        }
        else if (temp == "ANALYSIS") {
            analysis(text);
            cout << "Input a new word to search: ";
            continue;
        }

        if (cap && temp[0] >= 65 && temp[0] <= 90) {
            temp[0] += 32;
        }
        //Checking if the input is a key word and then uncapitalizing it, if the user wants it so

        iter = text.find(temp);

        if (iter == text.end()) {
            cout << "The word \"" << temp << "\" could not be found in the text" << endl;
        }
        else {
            cout << "The word \"" << temp << "\" occurs " << iter->second << " times in the text" << endl;
        }

        cout << "Input a new word to search: ";
        // Seeing if the input word is in the hash table and then outputting accordingly, 
        // asks for more input and accepts it in the while condition
    }

    return 0;
}


// This next function iterates over the hash table and gives back however many words have fewer instances
// than the number the user puts in. It also finds the largest word and the one with the most occurrences
void analysis(unordered_map<string, int> tempMap) {
    unordered_map<string, int>::iterator search;

    int number, count = 0, max = 0, longest = 0;
    string maxWord, longestWord;
    cout << endl << "Input a number to search how many words had fewer instances than that number: ";
    cin >> number;

    for (search = tempMap.begin(); search != tempMap.end(); ++search) {
        if (search->second > max) {
            max = search->second;
            maxWord = search->first;
        }

        if (search->first.length() > longest) {
            longestWord = search->first;
            longest = search->first.length();
        }

        if (search->second < number) {
            ++count;
        }
    }

    cout << count << " words occurred fewer than " << number << " times" << endl;
    cout << "The most used word was \"" << maxWord << "\" with " << max << " uses" << endl;
    cout << "The longest word used was \"" << longestWord << "\"" << endl;
}

// This function checks if the input string is empty then checks if the end has characters
// that are not letters or numbers, then takes them off and returns the string
string lastChar(string last) {
    if (last.empty()) {
        return last;
    }
    else if (last[last.length() - 1] > 122 || last[last.length() - 1] < 48 || (last[last.length() - 1] > 90 &&
        last[last.length() - 1] < 97) || (last[last.length() - 1] > 57 && last[last.length() - 1] < 65)) {
        last.pop_back();
        last = lastChar(last);
    }
    
    return last;
}
