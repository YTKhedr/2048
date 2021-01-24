#include<iostream>                      // for cin / cout
#include<iomanip>                       // for setw
#include<conio.h>                       // for getch
#include<ctime>                         // for time
#include<windows.h>                     // for rand / srand / system / sleep / memset / exit / changing colours
#define clearScreen system("cls")       // clears the terminal
#define whiteText SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x07)    // turns the text colour into white
#define redText(text) {SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x04); cout << text; whiteText;}      // turns the text colour into red then white
#define greenText(text) {SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0A); cout << text; whiteText;}    // turns the text colour into green then white
using namespace std;
int width = 2;                          // width of the cell (no. of digits + 1 for spacing)
const int arrSize = 4;                  // size of the grid
int arr[arrSize][arrSize] = {0};        // the grid
int arrDuplicate[arrSize][arrSize];     // the grid's duplicate
int randi;                              // i value of generated number's position
int randj;                              // j value of generated number's position
bool alreadyWon = false;
bool alreadyUndo = true;
void input();                           // function to get input from user
void addNum();                          // function to add a random number in a random position
void up();                              // function to simulate swiping up
void down();                            // function to simulate swiping down
void left();                            // function to simulate swiping left
void right();                           // function to simulate swiping right
void print();                           // function to print the array & instructions
void findWidth();                       // function to get the width of the cell
void sthChanged_apply();                // function to make a duplicate of the array for checking later on
bool sthChanged_check();                // function to check for changes
bool chanceScan();                      // function to check for new move chances
int main()
{
    srand(time(0));                     // assigns the seed for rand()
    while (1)
    {
        addNum();
        print();
        input();
        clearScreen;
    }
    return 0;
}
void input()
{
    int scan1st = getch();              // get 1st scan code
    if (scan1st == 224 || scan1st == 0) // 224 / 0 = 1st part of default / numpad arrow keys scan code
    {
        int scan2nd = getch();          // get 2nd scan code
        if (scan2nd == 72)              // 72 = 2nd part of up arrow key scan code
            up();
        else if (scan2nd == 80)         // 80 = 2nd part of down arrow key scan code
            down();
        else if (scan2nd == 75)         // 75 = 2nd part of left arrow key scan code
            left();
        else if (scan2nd == 77)         // 77 = 2nd part of right arrow key scan code
            right();
        else
        {
            redText("Wrong input!\n")
            input();
        }
        alreadyUndo = false;
    }
    else if (scan1st == 26)             // 26 = Ctrl + Z scan code
        if (alreadyUndo == false)
        {
            for (int i = 0; i < arrSize; i++)
                for (int j = 0; j < arrSize; j++)
                    arr[i][j] = arrDuplicate[i][j]; // Undo Functionality
            alreadyUndo = true;
            clearScreen;
            findWidth();
            print();
            input();
        }
        else
        {
            redText("You can't Undo!\n")
            input();
        }
    else
    {
        redText("Wrong input!\n")
        input();
    }
    findWidth();
}
void findWidth()
{
    int maxNum = arr[0][0];             // maximum number in the array
    for (int i = 0; i < arrSize; i++)
        for (int j = 0; j < arrSize; j++)
            if (maxNum < arr[i][j])
                maxNum = arr[i][j];
    int digitNum = 0;
    while (maxNum != 0)
    {
        maxNum /= 10;
        digitNum++;
    }
    width = digitNum + 1;
}
bool chanceScan()
{
    for (int i = 0; i < arrSize; i++)
        for (int j = 0; j < arrSize; j++)
            if ((i != 0 && arr[i][j] == arr[i - 1][j]) || (i != arrSize - 1 && arr[i][j] == arr[i + 1][j]) || (j != 0 && arr[i][j] == arr[i][j - 1]) || (j != arrSize - 1 && arr[i][j] == arr[i][j + 1]))
                return true;
    return false;
}
void sthChanged_apply()
{
    for (int i = 0; i < arrSize; i++)
        for (int j = 0; j < arrSize; j++)
            arrDuplicate[i][j] = arr[i][j];
}
bool sthChanged_check()
{
    for (int i = 0; i < arrSize; i++)
        for (int j = 0; j < arrSize; j++)
            if (arrDuplicate[i][j] != arr[i][j])
                return false;
    return true;
}
void addNum()
{
    int range[] = {2, 4};               // possible generated number values
    int randomNum = rand() % 2;         // random index for generated number
    int emptyNum = 0;                   // number of empty cells
    for (int i = 0; i < arrSize; i++)
        for (int j = 0; j < arrSize; j++)

            if (arr[i][j] == 0)
                emptyNum++;
            else if (arr[i][j] == 2048 && alreadyWon == false) // check if this is the first appearance of 2048
            {
                print();
                greenText("HURRAY!\n")
                Sleep(2000);
                redText("But you still have to play till you lose XD\n")
                Sleep(2000);
                clearScreen;
                alreadyWon = true;
            }
    struct posArr
    {
        int i;
        int j;
    };
    posArr arrEmpty[emptyNum]; // array to store the i / j values of the empty cells
    int counter = 0;
    for (int i = 0; i < arrSize; i++)
        for (int j = 0; j < arrSize; j++)
            if (arr[i][j] == 0)
            {
                arrEmpty[counter].i = i;
                arrEmpty[counter].j = j;
                counter++;
            }
    int randomPos = rand() % emptyNum; // random index for generated position
    arr[arrEmpty[randomPos].i][arrEmpty[randomPos].j] = range[randomNum]; // assigning the random number in the random position
    randi = arrEmpty[randomPos].i;
    randj = arrEmpty[randomPos].j;
}
void up()
{
    sthChanged_apply();
    bool addCheck = false; // checks if addition has occurred in this loop
    for (int j = 0; j < arrSize; j++) // for adding the cells
    {
        for (int i = 0; i < arrSize - 1; i++)
        {
            int k = i + 1; // the next number (to compare with)
            if (arr[i][j] != 0)
            {
                while (arr[k][j] == 0 && k < arrSize - 1) // keep moving on till you find a number
                    k++;
                if (arr[k][j] == arr[i][j])
                {
                    arr[k][j] = arr[k][j] * 2; // add the 2 cells
                    arr[i][j] = 0; // mimic the movement of the cell
                    addCheck = true;
                }
            }
            if (addCheck == true)
                i = k; // skip one cell
        }
        addCheck = false; // to start a new layer
    }
    for (int j = 0; j < arrSize; j++) // for moving the cells
        for (int i = 0; i < arrSize - 1; i++)
            if (arr[i][j] == 0 && arr[i + 1][j] != 0)
                swap(arr[i][j], arr[i + 1][j]);
            else if (i < arrSize - 2 && arr[i][j] == 0 && arr[i + 1][j] == 0)
            {
                int k = i + 1;
                while (arr[k][j] == 0 && k < arrSize - 1)
                    k++;
                swap(arr[i][j], arr[k][j]);
            }
    if (sthChanged_check()) // that move doesn't add numbers or move cells
    {
        redText("Error, that move does nothing!\n")
        input();
    }
}
void down()
{
    sthChanged_apply();
    bool addCheck = false;
    for (int j = 0; j < arrSize; j++)
    {
        for (int i = arrSize - 1; i > 0; i--)
        {
            int k = i - 1;
            if (arr[i][j] != 0)
            {
                while (arr[k][j] == 0 && k > 0)
                    k--;
                if (arr[k][j] == arr[i][j])
                {
                    arr[k][j] = arr[k][j] * 2;
                    arr[i][j] = 0;
                    addCheck = true;
                }
            }
            if (addCheck == true)
                i = k;
        }
        addCheck = false;
    }
    for (int j = 0; j < arrSize; j++)
        for (int i = arrSize - 1; i > 0; i--)
            if (arr[i][j] == 0 && arr[i - 1][j] != 0)
                swap(arr[i][j], arr[i - 1][j]);
            else if (i > 1 && arr[i][j] == 0 && arr[i - 1][j] == 0)
            {
                int k = i - 1;
                while (arr[k][j] == 0 && k > 0)
                    k--;
                swap(arr[i][j], arr[k][j]);
            }
    if (sthChanged_check())
    {
        redText("Error, that move does nothing!\n")
        input();
    }
}
void left()
{
    sthChanged_apply();
    bool addCheck = false;
    for (int i = 0; i < arrSize; i++)
    {
        for (int j = 0; j < arrSize - 1; j++)
        {
            int k = j + 1;
            if (arr[i][j] != 0)
            {
                while (arr[i][k] == 0 && k < arrSize - 1)
                    k++;
                if (arr[i][k] == arr[i][j])
                {
                    arr[i][k] = arr[i][k] * 2;
                    arr[i][j] = 0;
                    addCheck = true;
                }
            }
            if (addCheck == true)
                j = k;
        }
        addCheck = false;
    }
    for (int i = 0; i < arrSize; i++)
        for (int j = 0; j < arrSize - 1; j++)
            if (arr[i][j] == 0 && arr[i][j + 1] != 0)
                swap(arr[i][j], arr[i][j + 1]);
            else if (j < arrSize - 2 && arr[i][j] == 0 && arr[i][j + 1] == 0)
            {
                int k = j + 1;
                while (arr[i][k] == 0 && k < arrSize - 1)
                    k++;
                swap(arr[i][j], arr[i][k]);
            }
    if (sthChanged_check())
    {
        redText("Error, that move does nothing!\n")
        input();
    }
}
void right()
{
    sthChanged_apply();
    bool addCheck = false;
    for (int i = 0; i < arrSize; i++)
    {
        for (int j = arrSize - 1; j > 0; j--)
        {
            int k = j - 1;
            if (arr[i][j] != 0)
            {
                while (arr[i][k] == 0 && k > 0)
                    k--;
                if (arr[i][k] == arr[i][j])
                {
                    arr[i][k] = arr[i][k] * 2;
                    arr[i][j] = 0;
                    addCheck = true;
                }
            }
            if (addCheck == true)
                j = k;
        }
        addCheck = false;
    }
    for (int i = 0; i < arrSize; i++)
        for (int j = arrSize - 1; j > 0; j--)
            if (arr[i][j] == 0 && arr[i][j - 1] != 0)
                swap(arr[i][j], arr[i][j - 1]);
            else if (j > 1 && arr[i][j] == 0 && arr[i][j - 1] == 0)
            {
                int k = j - 1;
                while (arr[i][k] == 0 && k > 0)
                    k--;
                swap(arr[i][j], arr[i][k]);
            }
    if (sthChanged_check())
    {
        redText("Error, that move does nothing!\n")
        input();
    }
}
void print()
{
    for (int i = 0; i < (((2 + (width * arrSize)) / 2) - 5) + 8; i++) // for positioning the UP arrow symbol
        cout << " ";
    cout << char(30) << endl; // the UP arrow symbol
    for (int i = 0; i < ((2 + (width * arrSize)) / 2) - 5; i++) // for positioning the text & LEFT/RIGHT arrow symbols
        cout << " ";
    cout << "Enter " << char(17) << "   " << char(16) << endl; // the LEFT/RIGHT arrow symbols
    for (int i = 0; i < (((2 + (width * arrSize)) / 2) - 5) + 8; i++) // for positioning the DOWN arrow symbol
        cout << " ";
    cout << char(31) << endl; // the DOWN arrow symbol
    cout << " ";
    for (int i = 0; i < arrSize; i++)
        for (int j = 0; j < width; j++)
            cout << "_"; // for printing the upper border
    cout << endl;
    for (int i = 0; i < arrSize; i++)
    {
        cout << "|"; // for printing the left border
        for (int j = 0; j < arrSize; j++)
            if (arr[i][j] == 0)
            {
                cout << setw(width) << ".";
            }
            else
            {
                if (arr[i][j] >= 2048)
                {
                    cout << setw(width);
                    greenText(arr[i][j])
                }
                else if (i == randi && j == randj) // if it is the generated number
                {
                    cout << setw(width);
                    redText(arr[i][j])
                }
                else
                    cout << setw(width) << arr[i][j];
            }
        cout << "|"; // for printing the right border
        if (i < arrSize - 1)
            cout << endl;
    }
    cout << "\n ";
    for (int i = 0; i < arrSize; i++)
        for (int j = 0; j < width; j++)
            cout << char(196); // for printing the lower border (overscore symbol)
    cout << endl;
    int emptyNum = 0;
    for (int i = 0; i < arrSize; i++)
        for (int j = 0; j < arrSize; j++)
            if (arr[i][j] == 0)
                emptyNum++;
    if (emptyNum == 0 && chanceScan() == false)
    {
        redText("GAME OVER\n")
        Sleep(2000);
        char again;
        cout << "Again? (Y/N)\n";
loop:
        again = getch();
        if (again == 'Y' || again == 'y')
        {
            memset(arr, 0, sizeof(arr)); // for filling the array with zeros
            width = 2;
            clearScreen;
            main();
        }
        else if (again == 'N' || again == 'n')
            exit(0);
        else
        {
            redText("Wrong input!\n")
            goto loop;
        }
    }
}