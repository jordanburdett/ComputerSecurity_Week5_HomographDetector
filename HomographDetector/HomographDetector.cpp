
/*********************************************************************
* Homograph detector
* The user enters two file paths.
* The program determines if they are homographs.
* Homographs represent the same filepath but look different.
*********************************************************************/

#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <sstream>
#include <fstream>
#include <stack>
using namespace std;
using namespace filesystem;


/*********************************************************************
* returns the full current working directory path.
* Pass in a string vector to get each individual path element.
*********************************************************************/
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
    
    return currentPathString;
}

/*********************************************************************
* displays a string vector for debugging purposes.
*********************************************************************/
void displayStringVector(vector<string> stringVector)
{
    for (vector<string>::iterator iter = stringVector.begin(); iter != stringVector.end(); iter++)
    {
        cout << *iter << endl;
    }
}

/*********************************************************************
* takes a string and converts it to a vector
*********************************************************************/
vector<string> convertStringToPathVector(string path)
{
    vector<string> outputVector;
    stringstream stream(path);
    string parsed;

    // checking if it is unix or windows
    if (path.find(":") != std::string::npos)
    {
        // we found a colon so it must be windows
        while (getline(stream, parsed, '\\'))
        {
            if (parsed == "" || parsed == " ")
                continue;

            // add the parsed string to our vector
            outputVector.push_back(parsed);
        }
    }
    else
    {
        while (getline(stream, parsed, '/'))
        {
            if (parsed == "" || parsed == " ")
                continue;

            // add the parsed string to our vector
            outputVector.push_back(parsed);
        }
    }

   
    
    return outputVector;
}

/*********************************************************************
*   creates a new stack reversed not entirely necessarily
*   because we are just comparing strings to see if they are the same 
*   but is nice for testing
*********************************************************************/

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

/*********************************************************************
*   canonicilation function.
*   Takes the vector<string> path and creates a canon string
*********************************************************************/
string createCanonString(vector<string> path)
{
    // get current working directory
    stack<string> currentWorkingDirectory; 
    getCurrentPath(currentWorkingDirectory); // This comes back as a stack
    
    for (vector<string>::iterator iter = path.begin(); iter != path.end(); iter++)
    {
        // On the first run through we need to check if the entire path was given.
        if (iter == path.begin())
        {
            bool foundColon = (*iter).find(":") != std::string::npos;

            // if we are at the beginning of a path / - for unix C: - for windows - we don't want a current working directory.
            if (*iter == "/" || foundColon)
            {
                currentWorkingDirectory = stack<string>();
            }
        }

        if (*iter == "..") // ".." means to go back a directory -- so we pop
        {
            currentWorkingDirectory.pop();
        }
        else if (*iter == ".") // "." means current directory -- don't do anything
        {
            // do nothing
            continue;
        }
        else if (*iter == "/") // skip ove don't do anything
        {
            continue;
        }
        else
        {
            currentWorkingDirectory.push(*iter); // If we got here that means we have folder or file -- add it to the stack     
        }
    }
    
    stack<string> reversedStack = reverseStack(currentWorkingDirectory); // this is just to make it more human readable
    
    string canonPath = "";
    
    // add the '/' back into the path  
    while (!reversedStack.empty())
    {
        if (reversedStack.top() == "/")
        {
            reversedStack.pop();
            continue;
        }

        // this is the first iteration so we don't want a slash
        if (canonPath == "")
        {
            canonPath += reversedStack.top();
        }
        else 
        {
            canonPath = canonPath + "/" + reversedStack.top();
        }

        reversedStack.pop();
    }
    
    return canonPath;
}

/*********************************************************************
*   checks if two strings are homograph paths.
*********************************************************************/
bool checkForHomograph(string firstPath, string secondPath)
{
    // convert firstPath to vector
    vector<string> firstPathVector = convertStringToPathVector(firstPath);
    
    // convert secondPath to vector
    vector<string> secondPathVector = convertStringToPathVector(secondPath);
    
    string firstPathCanon = createCanonString(firstPathVector);
    string secondPathCanon = createCanonString(secondPathVector);
    
    cout << "results from compare: " << firstPathCanon.compare(secondPathCanon) << endl; // 0 - strings match

    cout << "\nstring 1: " << firstPathCanon << endl;
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

/*********************************************************************
*   first test case, an entire path.
*********************************************************************/
void fullPath()
{
    string firstPath = "test.txt";
    string secondPath = "E:/ComputerSecurityCS453/CSecurityLabs/week5labStack/HomographDetector/test.txt";

    if (checkForHomograph(firstPath, secondPath))
    {
        cout << "\nThe paths are homographs" << endl;
    }
    else
    {
        cout << "\nThe paths are not homographs" << endl;
    }
}

/*********************************************************************
*   test case for using the mirror image of the filepath
*********************************************************************/
void mirrorPath()
{

    string firstPath = "test.txt";
    string secondPath = "test.txt";

    if (checkForHomograph(firstPath, secondPath))
    {
        cout << "\nThe paths are homographs" << endl;
    }
    else
    {
        cout << "\nThe paths are not homographs" << endl;
    }

}

/*********************************************************************
*   Test case for a back middle path
*********************************************************************/
void backMiddlePath()
{

    string firstPath = "test.txt";
    string secondPath = "../../week5labStack/../week5labStack/HomographDetector/test.txt";

    if (checkForHomograph(firstPath, secondPath))
    {
        cout << "\nThe paths are homographs" << endl;
    }
    else
    {
        cout << "\nThe paths are not homographs" << endl;
    }

}

/*********************************************************************
*   test case for using the back keystroke on the filepath
*********************************************************************/
void backOneFolder()
{

    string firstPath = "test.txt";
    string secondPath = "../HomographDetector/test.txt";

    if (checkForHomograph(firstPath, secondPath))
    {
        cout << "\nThe paths are homographs" << endl;
    }
    else
    {
        cout << "\nThe paths are not homographs" << endl;
    }

}

/*********************************************************************
*   Test case for a broken path
*********************************************************************/
void brokenPath()
{

    string firstPath = "test.txt";
    string secondPath = "/HomographDetector/test.txt";

    if (checkForHomograph(firstPath, secondPath))
    {
        cout << "\nThe paths are homographs" << endl;
    }
    else
    {
        cout << "\nThe paths are not homographs" << endl;
    }

}

/*********************************************************************
*   main driver for the test cases.
*********************************************************************/
void alltest()
{

    // testing the .. method
    //testing full path E: etc etc
    // testing partial path etc etc without E:
    fullPath();
    backMiddlePath();
    backOneFolder();
    mirrorPath();
    brokenPath();

}

/*********************************************************************
*   Main driver of the program
*********************************************************************/
int main()
{
    string firstPath = "";
    string secondPath = "";
    string choiceString = "";

    cout << "type 'alltest' for test cases, type 'manual' for standard program function" << endl;
    getline(cin, choiceString);

    if (choiceString == "alltest") 
    {
        alltest();
    }
    else
    {
        cout << "Enter the first path" << endl;
        getline(cin, firstPath);

        cout << "Enter the second path" << endl;
        getline(cin, secondPath);

        cout << "\n\n";

        if (checkForHomograph(firstPath, secondPath))
        {
            cout << "\nThe paths are homographs" << endl;
        }
        else
        {
            cout << "\nThe paths are not homographs" << endl;
        }
    }
}
