
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
using namespace std::filesystem;


/*********************************************************************
*   getCurrentPath()
*   Returns the full current working directory path.
*   Pass in a string vector to get each individual path element.
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
*   displayStringVector()
*   Displays a string vector for debugging purposes.
*********************************************************************/
void displayStringVector(vector<string> stringVector)
{
    for (vector<string>::iterator iter = stringVector.begin(); iter != stringVector.end(); iter++)
    {
        cout << *iter << endl;
    }
}

/*********************************************************************
* convertStringToPathVector()
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
*   reverseStack()
*   Creates a new stack reversed not entirely necessarily
*   because we are just comparing strings to see if they are the same 
*   but is nice for testing.
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
*   createCanonString()
*   Canonicilation function.
*   Takes the vector<string> path and creates a canon string.
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
*   checkForHomograph()
*   Checks if two strings are homograph paths.
*********************************************************************/
bool checkForHomograph(string firstPath, string secondPath)
{
    // convert firstPath to vector
    vector<string> firstPathVector = convertStringToPathVector(firstPath);
    
    // convert secondPath to vector
    vector<string> secondPathVector = convertStringToPathVector(secondPath);
    
    string firstPathCanon = createCanonString(firstPathVector);
    string secondPathCanon = createCanonString(secondPathVector);
    
    // cout << "results from compare: " << firstPathCanon.compare(secondPathCanon) << "\n\n"; // 0 - strings match
    cout << "TEST START:\n\n";
    cout << "String 1: " << firstPathCanon << "\n\n";
    cout << "String 2: " << secondPathCanon << "\n\n";

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
string fullPath()
{
    string firstPath = "test.txt";
    string secondPath = "E:/ComputerSecurityCS453/CSecurityLabs/week5labStack/HomographDetector/test.txt";

    if (checkForHomograph(firstPath, secondPath))
    {
        return "The paths are homographs\n";
    }
    else
    {
        return "The paths are not homographs\n";
    }

    return "";

}

/*********************************************************************
*   test case for using the mirror image of the filepath
*********************************************************************/
string mirrorPath()
{

    string firstPath = "test.txt";
    string secondPath = "test.txt";

    if (checkForHomograph(firstPath, secondPath))
    {
        return "The paths are homographs\n";
    }
    else
    {
        return "The paths are not homographs\n";
    }

    return "";
}

/*********************************************************************
*   Test case for a back middle path
*********************************************************************/
string backMiddlePath()
{

    string firstPath = "test.txt";
    string secondPath = "../../week5labStack/../week5labStack/HomographDetector/test.txt";

    if (checkForHomograph(firstPath, secondPath))
    {
        return "The paths are homographs\n";
    }
    else
    {
        return "The paths are not homographs\n";
    }

    return "";
}

/*********************************************************************
*   test case for using the back keystroke on the filepath
*********************************************************************/
string backOneFolder()
{

    string firstPath = "test.txt";
    string secondPath = "../HomographDetector/test.txt";

    if (checkForHomograph(firstPath, secondPath))
    {
        return "The paths are homographs\n";
    }
    else
    {
        return "The paths are not homographs\n";
    }

    return "";

}

/*********************************************************************
*   Test case for a broken path
*********************************************************************/
string brokenPath()
{

    string firstPath = "test.txt";
    string secondPath = "/HomographDetector/test.txt";

    if (checkForHomograph(firstPath, secondPath))
    {
        return "The paths are homographs\n";
    }
    else
    {
        return "The paths are not homographs\n";
    }

    return "";
}

/*********************************************************************
*  displayMenu()
*  Called by interact()
*  Displays menu options.
*********************************************************************/
void displayMenu()
{
    cout << "Options:\n"
        << "   A  Execute test cases\n"
        << "   B  Execute manual test\n"
        << "   Q  Quit\n";

    return;
}

/*********************************************************************
*  displayPrompt()
*  Displays passed message.
*********************************************************************/
void displayPrompt(string message)
{
    cout << message << "\n";
    return;
}

/*********************************************************************
*  displayResult()
*  Called by interact()
*  Displays results.
*********************************************************************/
void displayResult(string message)
{
    cout << "TEST RESULT: ";
    cout << message << "\n\n";
    return;
}

/*********************************************************************
*  displayError()
*  Called by interact()
*  Displays error message.
*********************************************************************/
void displayError(string message)
{
    cout << message << "\n\n";
    return;
}

/*********************************************************************
*  displayHeader()
*  Called by interact()
*  Displays app identifier.
*********************************************************************/
void displayHeader()
{
    cout << "\n************ CSE 453 - Week 5 - HOMOGRAPHS ************\n\n";
    return;
}

/*********************************************************************
*  clearScreen()
*  Called by interact()
*  Clears the screen.
*********************************************************************/
void clearScreen()
{
    if (system("CLS")) { system("clear"); }
    return;
}

/*********************************************************************
*   alltest()
*   Main driver for the test cases.
*   Tests the .. (double dot) method.
*   Tests the full path E: Etc.
*   Tests partial paths without E: Etc.
*********************************************************************/
void alltest()
{
    displayResult(fullPath());
    displayResult(backMiddlePath());
    displayResult(backOneFolder());
    displayResult(mirrorPath());
    displayResult(brokenPath());

    return;

}

/*********************************************************************
*   testPaths()
*   Compares two paths.
*********************************************************************/
string testPaths()
{
    char path1[255] = "";
    char path2[255] = "";
    clearScreen();
    displayHeader();

    displayPrompt("Enter first path:");
    cin.getline(path1, 255);
    if (cin.fail()) {
        cin.clear();
        cin.ignore();
    }
    if (path1[0] == '\0') {
        clearScreen();
        displayHeader();
        return "";
    }

    clearScreen();
    displayHeader();
    cout << "First path: " << path1 << "\n\n";
    displayPrompt("Enter second path:");
    cin.getline(path2, 255);
    if (cin.fail()) {
        cin.clear();
        cin.ignore();
    }
    if (path2[0] == '\0') {
        clearScreen();
        displayHeader();
        return "";
    }

    clearScreen();
    displayHeader();
    cout << "First path: " << path1 << "\n";
    cout << "Second path: " << path2 << "\n\n";

    // process input
    if (checkForHomograph(path1, path2))
    {
        return "The paths are homographs";
    }
    else
    {
        return "The paths are not homographs";
    }

    return "";
}

/*********************************************************************
*   interact(), until user types "Q".
*   Called by main.
*********************************************************************/
void interact()
{
    clearScreen();
    displayHeader();
    displayMenu();

    char answer[2] = "";
    string returnString;
    do
    {
        if (cin.fail()) // bad input
        {
            cin.clear();
            cin.ignore();
            continue;
        }

        cin.getline(answer, 2);
        if (islower(answer[0])) { answer[0] = toupper(answer[0]); }
        returnString.clear();
        switch (answer[0])
        {
        case '\0': // no input
            clearScreen();
            displayHeader();
            displayError("ERROR: Invalid command");
            displayMenu();
            break;

        case 'A': // Execute test cases
            clearScreen();
            displayHeader();
            alltest();
            displayMenu();
            break;

        case 'B': // Execute manual test
            clearScreen();
            displayHeader();
            returnString = testPaths();
            if (returnString.empty()) {
                displayError("ERROR: testPaths() returned empty");
            }
            else {
                displayResult(returnString);
            };
            displayMenu();
            break;

        case 'Q': // Quit
            clearScreen();
            break;

        default:
            clearScreen();
            displayHeader();
            displayError("ERROR: Invalid command");
            displayMenu();
            break;
        }
    } while (answer[0] != 'Q' && answer[0] != 'q'); // Q = quit

    return;
}

/*********************************************************************
*   Main driver of the program
*********************************************************************/
int main()
{
    interact();

    return 0;
}
