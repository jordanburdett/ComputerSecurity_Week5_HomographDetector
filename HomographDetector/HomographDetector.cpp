/****
* Homograph detector
* The user enters two file paths. The program then determines if they are homographs (the same filepath but look different)
*********/

#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <sstream>
#include <fstream>
#include <stack>
using namespace std;
using namespace filesystem;

// returns the full current working directory path.. Pass in a string vector to get each individual path element
string getCurrentPath(stack<string>& pathVector)
{
    // this uses the std::filesystem library to get the current working directory path
    path currentPath = current_path();
    path::iterator currentPathIter;

    string currentPathString = currentPath.generic_string();

    for (currentPathIter = currentPath.begin(); currentPathIter != currentPath.end(); currentPathIter++)
    {
        pathVector.push((*currentPathIter).generic_string());
    }
    
    cout << currentPathString << endl;
    return currentPathString;
}

// displays a string vector for debugging purposes
void displayStringVector(vector<string> stringVector)
{
    for (vector<string>::iterator iter = stringVector.begin(); iter != stringVector.end(); iter++)
    {
        cout << *iter << endl;
    }
}

// takes a string and converts it to a vector
vector<string> convertStringToPathVector(string path)
{
    vector<string> outputVector;
    stringstream stream(path);
    string parsed;

    while (getline(stream, parsed, '/'))
    {
        // lots of error handling could work well here.... :)
        if (parsed == "" || parsed == " ")
            continue;

        // add the parsed string to our vector
        outputVector.push_back(parsed);
    }
    
    return outputVector;
}

// creates a new stack reversed -- not entirely necessarily because we are just comparing strings to see if they are the same but is nice for testing
stack<string> reverseStack(stack<string> stringStack)
{
    stack<string> reversedStack;
    
    while(!stringStack.empty())
    {
        reversedStack.push(stringStack.top());
        stringStack.pop();
    }
    
    return reversedStack;
}

// canonicilation function. Takes the vector<string> path and simplifies it as much as possible.
string createCanonString(vector<string> path)
{
    // get current working directory
    stack<string> currentWorkingDirectory; // I changed the vector here to a stack.
    getCurrentPath(currentWorkingDirectory);
    
    for (vector<string>::iterator iter = path.begin(); iter != path.end(); iter++)
    {
        // There is probably more special characters we need here in order to account for all the cases
        if (*iter == "..")
        {
            currentWorkingDirectory.pop();
        }
        else if (*iter == ".")
        {
            // do nothing
            continue;
        }
        else if (*iter == "/")
        {
            continue;
        }
        else
        {
            currentWorkingDirectory.push(*iter);
        }
    }
    
    stack<string> reversedStack = reverseStack(currentWorkingDirectory);
    
    string canonPath = "";
    
    while (!reversedStack.empty())
    {
        canonPath = canonPath + "/" + reversedStack.top();
        reversedStack.pop();
    }
    
    cout << "The newly created Path: " << canonPath << endl;
    
    return canonPath;
}

// checks if two strings are homograph paths
bool checkForHomograph(string firstPath, string secondPath)
{
    // convert firstPath to vector
    vector<string> firstPathVector = convertStringToPathVector(firstPath);
    
    // convert secondPath to vector
    vector<string> secondPathVector = convertStringToPathVector(secondPath);
    
    string firstPathCanon = createCanonString(firstPathVector);
    string secondPathCanon = createCanonString(secondPathVector);
    
    cout << "results from compare: " << firstPathCanon.compare(secondPathCanon) << endl;
    cout << "string 1: " << firstPathCanon << endl;
    cout << "string 2: " << secondPathCanon << endl;
    if (firstPathCanon.compare(secondPathCanon) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
    
}

// Main driver of the program
int main()
{
    string firstPath = "";
    string secondPath = "";

    cout << "Enter the first path" << endl;
    getline(cin, firstPath);

    cout << "Enter the second path" << endl;
    getline(cin, secondPath);

    if (checkForHomograph(firstPath, secondPath))
    {
        cout << "The paths are homographs" << endl;
    }
    else
    {
        cout << "The paths are not homographs" << endl;
    }
}
