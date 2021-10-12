/****
* Homograph detector
* The user enters two file paths. The program then determines if they are homographs (the same filepath but look different)
*********/

#include <iostream>
#include <string>
#include <filesystem>
using namespace std;
using namespace filesystem;

// returns the full current working directory path.. Pass in a string vector to get each individual path element
string getCurrentPath(vector<string>& pathVector)
{
    // this uses the std::filesystem library to get the current working directory path
    path currentPath = current_path();
    path::iterator currentPathIter;

    string currentPathString = currentPath.generic_string();

    for (currentPathIter = currentPath.begin(); currentPathIter != currentPath.end(); currentPathIter++)
    {
        pathVector.push_back((*currentPathIter).generic_string());
    }

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
void convertStringToPathVector(string path, vector<string> & outputVector)
{
    stringstream stream(path);
    string parsed;

    // we need to know what to split on... Probably just depends on the test cases we write.. 
    // Typically we see directories with a backslash so /somepath/somefile.txt so if we write all of the tests like that it should work fine! :)
    while (getline(stream, parsed, '/'))
    {
        // lots of error handling could work well here.... :)
        if (parsed == "" || parsed == " ") 
            continue;

        // add the parsed string to our vector
        outputVector.push_back(parsed);
    }
}

/** find where the second path starts in the fullPath --
*
* examples
*
* secondPath: ./text.txt
* firstElement: './'
*
* we should start at the end of the current working directory because we started with './'
*
*
* secondPath: ../homograph/text.txt
* firstElement: '../'
*
* we should start at the element before the end of the current working directory because '../'
*
* secondPath: /homograph/text.txt
* firstElement: 'homograph'
*
* we need to find the directory /homograph in our full path vector and return the iterator there.
*/
// determines where the startingpoint is in the attempted homograph attack
vector<string>::iterator determinePathStartingPoint(vector<string>& secondPathVector, vector<string>::iterator &secondPathVectorIter, vector<string>& fullPathVector)
{
    string firstElement = secondPathVector.front();

    // not sure if this is all of the special cases we would need to look for
    if (firstElement == ".")
    {
        secondPathVectorIter = ++secondPathVector.begin();
        return --fullPathVector.end();
    }
    else if (firstElement == "..")
    {
        secondPathVectorIter = ++secondPathVector.begin();
        return --(--fullPathVector.end());
    }

    // we didn't find a special case above so we need to find the position in the full path vector
    for (vector<string>::iterator iter = fullPathVector.begin(); iter != fullPathVector.end(); iter++)
    {
        if (*iter == firstElement)
        {
            secondPathVectorIter = secondPathVector.begin();
            return iter;
        }
    }
    

    return fullPathVector.end();
}

// checks if two strings are homograph paths
bool checkForHomograph(string firstPath, string secondPath)
{
    // get the current working directory
    vector<string> fullPathVector;
    string fullPath = getCurrentPath(fullPathVector);

    // add the first path to the end of the currentPath vector
    fullPathVector.push_back(firstPath);

    vector<string> secondPathVector;
    convertStringToPathVector(secondPath, secondPathVector);

    
    vector<string>::iterator iter;
    vector<string>::iterator fullPathStartingIter = determinePathStartingPoint(secondPathVector, iter, fullPathVector);
    

    // we didn't find anything in the path so it must not be a homograph
    if (fullPathStartingIter == fullPathVector.end())
    {
        return false;
    }

    // loop through our secondPathVector
    for (; iter != secondPathVector.end(); iter++)
    {  
        if (*iter == "..")
        {
            fullPathStartingIter--;
        }
        else
        { 
            // check if it is equal
            if (*fullPathStartingIter != *iter)
            {
                return false;
            }

            fullPathStartingIter++;
        }
    }

    if (*--fullPathVector.end() == *--secondPathVector.end() && fullPathStartingIter == fullPathVector.end())
    {
        return true;
    }

    return false;
}

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



