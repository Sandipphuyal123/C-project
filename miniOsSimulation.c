#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <conio.h>
#include <direct.h>
// ==================================================GLOBAL VARIABLES=================================================================
#define Total_users 100
#define Total_userName_length 30
#define Total_Password_length 30
#define Total_fileName_length 50
#define Total_files 100
#define Total_absolutePath_length 100

typedef struct{
    char username[Total_userName_length];
    char password[Total_Password_length];
    int isActive;
}User;

// File structure for file manager (simulated)
typedef struct{
    char name[Total_fileName_length];
    char fileContent[2000]; // Simulated file content
    int size; // Simulated file size in bytes
    int isDirectory;
}FileEntry;

User users_arr[Total_users];/*This will create a bridge type which will contain registered users
in array memory and also from the .dat file */
int totalUserCount = 0;
char currentUser[Total_userName_length] = "";
FileEntry files[Total_files];
int fileCount = 0;
char currentPath[Total_absolutePath_length] = "/home"; // Simulated current directory
// ================================= FUNCTION PROTOTYPE===========================================================================
void loadUsers();
void saveUsers();
void clearScreen();
void bufferClear();
void registerUser();
int loginUser();
void mainMenu();
void gameMenu();
void quizGame();
void ticTacToe();
void terminalmenu();
void fileManagerMenu();
void calculator();
void notesApp();
void logoutUser();
void fileManagerMenu();
void displayFiles();
void createFile();
void modifyFile();
void deleteFile();
void viewFile();
void maskPassword(char *Password,int maxLength);
//====================================================================  INT MAIN IS HERE============================================================

int main(){
    loadUsers();
    srand(time(0)); // Seed random for quiz game

    int choice;
    do {
        clearScreen();
        printf("\n=== Welcome to Simple OS Interface ===\n");
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("[DETECT]: Enter your choice....\n");
        if (scanf("%d", &choice) != 1) {
            bufferClear();
            printf("\n[DETECT]: Invalid Argument! Please try again....\n");
            continue;
        }
        bufferClear();

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                if (loginUser()) {
                    printf("This is in the testing Phase!\n");
                    printf("everything is correct up untill this moment!\n");
                    mainMenu();
                }
                break;
            case 3:
                saveUsers();
                printf("[DETECT]: Closing system pending....\n[DETECT]: Completed! Thank you for using our services!\n");
                printf("Press any key to close.\n");
                getch();
                return 0;
            default:
                printf("\n[DETECT]: Invalid Argument! Please try again....\n");
        }
    } while (1);

    return 0;
}
//This clears the screen with the help of the headerfile stdlib.h
void clearScreen() {
    system("cls");
}

void maskPassword(char *password, int maxLength) {
    int i = 0;
    char ch;
    while ((ch = getch()) != '\r' && i < maxLength - 1) { // '\r' means enter key or \n key in c programming
        if (ch == '\b') { // '\b' is Backspace so we can simulate the edits of the text in * format
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';
    printf("\n");
}
void bufferClear() {
    int c;
    while ((c=getchar())!= '\n'&&c!= EOF) {
        //This does nothing except of eating the characters from c
    }
    if (c==EOF) {
        //This reset the endofthefile state to the original
        clearerr(stdin);
    }
}

void loadUsers(){
    FILE *file=fopen("masterFile.dat","rb");
    if(file ==NULL){
        perror("FILE Couldn't be accessed");
        return;
    }
    totalUserCount=fread(users_arr,sizeof(User),Total_users,file);
    printf("[DEBUG]: Searching Registered Users.\n");
    printf("[DETECT]: Successfully Found %d users\n",totalUserCount);
    printf("Press a Key to Continue....\n");
    getch();
    fclose(file);
}

void saveUsers(){
    FILE *file=fopen("masterFile.dat","wb");
    if(file==NULL){
        perror("Error saving user data");
        printf("Press a key to continue....\n");
        getch();
        return;
    }
    fwrite(users_arr,sizeof(User),totalUserCount,file);
    printf("[DETECT]: Users Saved successfully.\n");
    fclose(file);
}

void registerUser(){
    clearScreen();
    //This checks if the registered user have exceed their 
    if(totalUserCount>=Total_users){
        printf("The Maxinun number of registered users have been reached!\n");
        printf("Press any key to continue.\n");
        getch();
        return;
    }
    char userName[Total_userName_length], Password[Total_Password_length],filePath[Total_absolutePath_length];
    int i=0;
    printf("\n====== User Registration ======\n");
    printf("Username: ");
    if (fgets(userName, Total_userName_length, stdin) == NULL) {
        printf("Error reading username!\n");
        printf("Press any key to continue.\n");
        getch();
        return;
    }
    userName[strcspn(userName, "\n")]='\0';
    // This block Checks if username already exists
    for (i = 0; i < totalUserCount; i++) {
        if (strcmp(users_arr[i].username, userName) == 0) {
            printf("Username already exists!\n");
            printf("Press any key to continue.\n");
            getch();
            return;
        }
    }
    printf("Password: ");
    maskPassword(Password, Total_Password_length);

    if (mkdir(userName) == -1) {
        perror("Error creating directory");
        printf("Press any key to continue.\n");
        getch();
        return;
    }
    // Simulated a absolute file path so commands like cd work
    sprintf(filePath, "%s\\impFile.txt", userName);
    FILE *file = fopen(filePath, "w");
    if (file == NULL) {
        perror("Error creating file");
        printf("Press any key to continue.\n");
        getch();
        return;
    }
    fprintf(file, "Username: %s\nPassword: %s\n", userName,Password);
    fclose(file);

    // Add new user to array
    strncpy(users_arr[totalUserCount].username,userName,Total_userName_length-1);
    users_arr[totalUserCount].username[Total_userName_length-1] ='\0';
    strncpy(users_arr[totalUserCount].password, Password, Total_Password_length-1);
    users_arr[totalUserCount].password[Total_Password_length - 1] ='\0';
    users_arr[totalUserCount].isActive = 0;
    totalUserCount++;
    saveUsers();
    printf("\n[DETECT]: Registration completed....\n");
    printf("Press any key to continue.\n");
    getch();
}

int loginUser() {
    clearScreen();
    char userName[Total_userName_length],Password[Total_Password_length],filePath[Total_absolutePath_length];
    char storedUsername[Total_userName_length],storedPassword[Total_Password_length];
    int i=0;
    printf("\n=== User Login ===\n");
    printf("this is just the testing version\n");
    printf("Username: ");
    if(fgets(userName,Total_userName_length,stdin)==NULL) {
        printf("Error reading username!\n");
        printf("Press any key to continue.\n");
        getch();
        return 0;
    }
    userName[strcspn(userName, "\n")] = '\0';

    printf("Password: ");
    maskPassword(Password, Total_Password_length);

    // This block Check in-memory users first
    for (i = 0; i < totalUserCount; i++) {
        if (strcmp(users_arr[i].username,userName)==0&&strcmp(users_arr[i].password,Password)==0) {
            strncpy(currentUser, userName, Total_userName_length-1);
            currentUser[Total_userName_length-1] ='\0';
            users_arr[i].isActive = 1;
            printf("Login successful! Welcome, %s! Press any key to continue.\n",userName);
            getch();
            return 1;
        }
    }

    // This check Check file-based storage
    sprintf(filePath,"%s\\impFile.txt",userName);
    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        perror("Couldn't access file");
        printf("Press any key to continue.\n");
        getch();
        return 0;
    }

    fscanf(file,"Username: %s\nPassword: %s\n", storedUsername, storedPassword);
    fclose(file);

    if (strcmp(userName,storedUsername) == 0 && strcmp(Password, storedPassword)== 0) {
        // This block Adds to in-memory users if not already present
        if (totalUserCount < Total_users) {
            strncpy(users_arr[totalUserCount].username, userName,Total_userName_length-1);
            users_arr[totalUserCount].username[Total_userName_length-1] ='\0';
            strncpy(users_arr[totalUserCount].password, Password, Total_Password_length-1);
            users_arr[totalUserCount].password[Total_Password_length - 1] = '\0';
            users_arr[totalUserCount].isActive = 1;
            totalUserCount++;
            saveUsers();
        }
        strncpy(currentUser, userName,Total_userName_length- 1);
        currentUser[Total_userName_length- 1] = '\0';
        printf("successfully Logged in\n");
        printf("Press any key to continue.\n");
        getch();
        return 1;
    }

    printf("Invalid Username or Password\n");
    printf("Press any key to continue.\n");
    getch();
    return 0;
}
// Logout user
void logoutUser() {
    int i=0;
    for (i = 0; i < totalUserCount; i++) {
        if (strcmp(users_arr[i].username, currentUser) == 0) {
            users_arr[i].isActive = 0;
            break;
        }
    }
    currentUser[0] = '\0';
    printf("Logged out successfully. Press any key to continue.\n");
    getch();
}

void mainMenu(){
    int choice;
    do {
        clearScreen();
        printf("\n=== Main Menu === (User: %s)\n", currentUser);
        printf("1. Games\n");
        printf("2. Terminal\n");
        printf("3. File Manager\n");
        printf("4. Calculator\n");
        printf("5. Notes\n");
        printf("6. Logout\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            bufferClear();
            choice = -1;
        }
        bufferClear();

        switch (choice) {
            case 1:
                gameMenu();
                break;
            case 2:
                terminalMenu();
                break;
            case 3:
                fileManagerMenu();
                break;
            case 4:
                calculator();
                break;
            case 5:
                notesApp();
                break;
            case 6:
                logoutUser();
                return;
            default:
                printf("Invalid choice. Press any key to continue.\n");
                getch();
        }
    } while (currentUser[0] != '\0'); 
}

void gameMenu() {
    int choice;
    do {
        clearScreen();
        printf("\n=== Game Menu ===\n");
        printf("1. Quiz Game\n");
        printf("2. Tic Tac Toe\n");
        printf("3. Back to Main Menu\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            bufferClear();
            choice = -1;
        }
        bufferClear();

        switch (choice) {
            case 1:
                quizGame();
                break;
            case 2:
                ticTacToe();
                break;
            case 3:
                return;
            default:
                printf("Invalid choice. Press any key to continue.\n");
                getch();
        }
    } while (1);
}


void quizGame(){
    typedef struct{
        char question[200];
        char options[4][100];
        int rightAns;
    }quizQn;

    quizQn Questions[]={
        {"What is the capital of USA?", {"London","washington dc", "Kathmandu","delhi"},1},
        {"Which planet is known as the Red Planet?", {"Venus", "Mars", "Jupiter", "Saturn"}, 1},
        {"What is 2 + 2?",{"3","4","5","6"},1},
        {"Who painted the Mona Lisa?", {"Vincent van Gogh", "Pablo Picasso", "Leonardo da Vinci", "Michelangelo"}, 2},
        {"What is the largest mammal?", {"Elephant", "Blue Whale", "Giraffe", "Polar Bear"}, 1}
    };
    int score=0,i=0,j=0;
    int totalQns=sizeof(Questions)/sizeof(Questions[0]);
    clearScreen();
    printf("\n====== Quiz Game ======\n");
    printf("Answer the following questions:\n\n");
    for (i = 0; i < totalQns; i++) {
        printf("Question %d: %s\n", i + 1,Questions[i].question);
        for (j = 0; j < 4; j++) {
            printf("%d. %s\n", j + 1,Questions[i].options[j]);
        }

        int answer;
        printf("Your answer (1-4): ");
        if (scanf("%d", &answer) != 1||answer<1|| answer> 4) {
            bufferClear();
            printf("Invalid input! Assuming wrong answer.\n");
        } else {
            bufferClear();
            if (answer==Questions[i].rightAns+1) {
                printf("Correct!\n");
                score++;
            } else {
                printf("Wrong! The correct answer was %d. %s\n",
                       Questions[i].rightAns + 1,
                       Questions[i].options[Questions[i].rightAns]);
            }
        }
        printf("Press any key to continue.\n");
        getch();
    }
}
// =================================Tic-Tac-Toe game===========================================================================

void ticTacToe() {
    char board[3][3];
    int row, col;
    char player='X';
    int moves =0,i=0,j=0;

    // Initialize board
    for (i =0;i <3;i++) {
        for (j =0;j<3;j++) {
            board[i][j] = ' ';
        }
    }

    clearScreen();
    printf("\n=== Tic Tac Toe ===\n");
    printf("Player 1: X\nPlayer 2: O\n");
    printf("Enter row (1-3) and column (1-3) to make your move, or 'q' to quit.\n\n");

    while (1) {
        // Display board with numbered rows and columns
        printf("    1   2   3\n");
        for (i = 0;i <3;i++) {
            printf("%d | %c | %c | %c |\n", i+1, board[i][0],board[i][1], board[i][2]);
            if (i < 2) printf("  ---+---+---\n");
        }

        // Get move
        printf("Player %c's turn. Enter row and column (or 'q' to quit): ", player);
        char input[10];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            bufferClear();
            printf("Invalid input! Try again.\n");
            printf("Press any key to continue.\n");
            getch();
            continue;
        }

        // Check for quit
        input[strcspn(input,"\n")]='\0';
        if (strcmp(input, "q") == 0||strcmp(input,"Q")==0) {
            printf("Game aborted. Returning to main menu.\n");
            printf("Press any key to continue.\n");
            getch();
            return;
        }

        // Parse row and column
        if (sscanf(input, "%d %d",&row,&col)!= 2||row<1||row>3||col<1||col>3) {
            bufferClear();
            printf("Invalid position! Try again.\n");
            printf("Press any key to continue.\n");
            getch();
            continue;
        }
        bufferClear();

        if (board[row -1][col- 1]!=' ') {
            printf("Position already taken! Try again.\n");
            printf("Press any key to continue.\n");
            getch();
            continue;
        }

        // Make move
        board[row - 1][col - 1]=player;
        moves++;

        // Check for win
        int win = 0;
        // Check rows
        for (i = 0;i<3;i++) {
            if (board[i][0]==player&&board[i][1]==player&&board[i][2]==player){
                win = 1;
                break;
            }
        }
        // Check columns
        for (j = 0; j < 3; j++) {
            if (board[0][j]==player&&board[1][j]==player&&board[2][j]==player){
                win = 1;
                break;
            }
        }
        // Check diagonals
        if (board[0][0]== player&& board[1][1]==player&& board[2][2]==player) {
            win=1;
        }
        if (board[0][2] == player && board[1][1] == player && board[2][0] == player) {
            win= 1;
        }

        if (win) {
            clearScreen();
            printf("    1   2   3\n");
            for(i=0;i<3;i++) {
                printf("%d | %c | %c | %c |\n", i + 1, board[i][0],board[i][1], board[i][2]);
                if (i<2) printf("  ---+---+---\n");
            }
            printf("Player %c wins! Press any key to return.\n",player);
            getch();
            break;
        }

        if (moves==9) {
            clearScreen();
            printf("    1   2   3\n");
            for (i = 0; i < 3; i++) {
                printf("%d | %c | %c | %c |\n", i + 1, board[i][0], board[i][1], board[i][2]);
                if (i < 2) printf("  ---+---+---\n");
            }
            printf("It's a draw! Press any key to return.\n");
            getch();
            break;
        }

        // Switch player
        player= (player== 'X') ? 'O' : 'X';
    }
}
// ================================= \\\Calculator\\\ ===========================================================================
void calculator() {
    double num1, num2, result;
    char op;

    clearScreen();
    printf("\n=== Simple Calculator ===\n");
    while (1) {
        printf("Enter an operation (+, -, *, /) or 'q' to quit: ");
        if (scanf(" %c", &op) != 1) {
            bufferClear();
            printf("Invalid input! Try again.\n");
            printf("Press any key to continue.\n");
            getch();
            continue;
        }
        bufferClear();

        if (op == 'q' || op == 'Q') break;

        printf("Enter first number: ");
        if (scanf("%lf", &num1) != 1) {
            bufferClear();
            printf("Invalid number! Try again.\n");
            printf("Press any key to continue.\n");
            getch();
            continue;
        }
        bufferClear();

        printf("Enter second number: ");
        if (scanf("%lf",&num2)!=1) {
            bufferClear();
            printf("Invalid number! Try again.\n");
            printf("Press any key to continue.\n");
            getch();
            continue;
        }
        bufferClear();

        switch (op) {
            case '+':
                result= num1+num2;
                break;
            case '-':
                result= num1-num2;
                break;
            case '*':
                result= num1*num2;
                break;
            case '/':
                if (num2==0) {
                    printf("Error: Division by zero!\n");
                    printf("Press any key to continue.\n");
                    getch();
                    continue;
                }
                result=num1/num2;
                break;
            default:
                printf("Invalid operator!\n");
                printf("Press any key to continue.\n");
                getch();
                continue;
        }

        printf("Result: %.2lf %c %.2lf = %.2lf\n", num1,op,num2,result);
        printf("Press any key to continue.\n");
        getch();
    }
}

// =================================  Notes app (simulated)===========================================================================
void notesApp() {
    int choice,i=0;
    do {
        clearScreen();
        printf("\n=== Notes App ===\n");
        printf("1. Create New Note\n");
        printf("2. View Note\n");
        printf("3. Back to Main Menu\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            bufferClear();
            choice = -1;
        }
        bufferClear();

        switch (choice) {
            case 1:
                if (fileCount >= Total_files) {
                    printf("Maximum number of files reached!\n");
                    printf("Press any key to continue.\n");
                    getch();
                    break;
                }
                printf("Enter note filename (without extension): ");
                char filename[Total_fileName_length];
                if (fgets(filename, Total_fileName_length, stdin) == NULL) {
                    printf("Error reading filename!\n");
                    printf("Press any key to continue.\n");
                    getch();
                    break;
                }
                filename[strcspn(filename, "\n")] = '\0';

                // Check if filename exists
                for (i = 0; i < fileCount; i++) {
                    if (strcmp(files[i].name, filename) == 0) {
                        printf("File already exists!\n");
                        printf("Press any key to continue.\n");
                        getch();
                        break;
                    }
                }

                printf("Enter note content (type 'END' on a new line to finish):\n");
                char content[1000] = "";
                char line[100];
                while (fgets(line, sizeof(line), stdin)) {
                    line[strcspn(line, "\n")] = '\0';
                    if (strcmp(line, "END") == 0) break;
                    if (strlen(content) + strlen(line) + 1 < sizeof(content)) {
                        strcat(content, line);
                        strcat(content, "\n");
                    } else {
                        printf("Content too large! Stopping input.\n");
                        break;
                    }
                }

                strncpy(files[fileCount].name, filename, Total_fileName_length - 1);
                files[fileCount].name[Total_fileName_length - 1] = '\0';
                strncpy(files[fileCount].fileContent, content, 1000 - 1);
                files[fileCount].fileContent[1000 - 1] = '\0';
                files[fileCount].size = strlen(files[fileCount].fileContent);
                files[fileCount].isDirectory = 0;
                fileCount++;
                printf("Note saved successfully! Press any key to continue.\n");
                getch();
                break;

            case 2:
                displayFiles();
                printf("Enter note number to view (1-%d): ", fileCount);
                int index;
                if (scanf("%d", &index) != 1 || index < 1 || index > fileCount) {
                    bufferClear();
                    printf("Invalid file number!\n");
                    printf("Press any key to continue.\n");
                    getch();
                    break;
                }
                bufferClear();
                printf("\n=== %s ===\n", files[index - 1].name);
                printf("%s\n", files[index - 1].fileContent);
                printf("=== End of Note ===\n");
                printf("Press any key to continue.\n");
                getch();
                break;

            case 3:
                return;

            default:
                printf("Invalid choice. Press any key to continue.\n");
                getch();
        }
    } while (1);
}

// Terminal menu
void terminalMenu() {
    char command[100];
    int i=0;
    clearScreen();
    printf("\n=== Simple Terminal === (type 'exit' to return)\n");
    while (1) {
        printf("%s@simple-term:%s$ ", currentUser, currentPath);
        if (!fgets(command, sizeof(command), stdin)) {
            bufferClear();
            printf("Failed to read command.\n");
            printf("Press any key to continue.\n");
            getch();
            continue;
        }
        command[strcspn(command,"\n")]='\0';

        if (command[0]=='\0') {
            continue; // Skip empty input
        }
        if (strcmp(command,"exit") == 0) {
            break;
        } else if (strcmp(command,"help") == 0) {
            printf("Available commands:\n");
            printf("help - Show this help message\n");
            printf("clear - Clear the screen\n");
            printf("time - Show current time\n");
            printf("date - Show current date and time\n");
            printf("dir - List files\n");
            printf("pwd - Show current directory\n");
            printf("cd <path> - Change directory (simulated)\n");
            printf("echo <content> > <filename> - Create a file with content\n");
            printf("system - Show system information\n");
            printf("exit - Return to main menu\n");
            printf("Press any key to continue.\n");
            getch();
        } else if (strcmp(command, "clear") == 0) {
            clearScreen();
        } else if (strcmp(command, "time") == 0) {
            time_t t = time(NULL);
            printf("Current time: %s",ctime(&t));
            printf("Press any key to continue.\n");
            getch();
        } else if (strcmp(command,"date") == 0) {
            time_t t=time(NULL);
            struct tm *tm_info=localtime(&t);
            char buffer[50];
            strftime(buffer, 50,"%Y-%m-%d %H:%M:%S",tm_info);
            printf("Current date and time: %s\n", buffer);
            printf("Press any key to continue.\n");
            getch();
        } else if (strcmp(command,"dir")==0) {
            displayFiles();
            printf("Press any key to continue.\n");
            getch();
        } else if (strcmp(command, "pwd")== 0) {
            printf("Current directory: %s\n", currentPath);
            printf("Press any key to continue.\n");
            getch();
        } else if (strncmp(command, "cd ", 3)== 0) {
            char *path = command + 3;
            // Trim leading/trailing spaces
            while (isspace(*path)) path++;
            char *end = path + strlen(path)-1;
            while (end > path && isspace(*end)) *(end--)='\0';

            if (strlen(path) == 0) {
                printf("Please specify a directory.\n");
            } else if (strcmp(path, "..") == 0) {
                // Simulate moving up one directory
                char *lastSlash = strrchr(currentPath, '/');
                if (lastSlash != NULL && lastSlash != currentPath) {
                    *lastSlash = '\0';
                    if (strlen(currentPath) == 0) strcpy(currentPath, "/");
                }
                printf("Changed directory to: %s\n", currentPath);
            } else {
                // Simulate changing to a new directory
                char newPath[Total_absolutePath_length];
                snprintf(newPath, Total_absolutePath_length, "%s/%s", currentPath, path);
                strncpy(currentPath, newPath, Total_absolutePath_length - 1);
                currentPath[Total_absolutePath_length - 1] = '\0';
                printf("Changed directory to: %s\n", currentPath);
            }
            printf("Press any key to continue.\n");
            getch();
        } else if (strncmp(command, "echo ", 5) == 0) {
            // Parse echo command: echo <content> > <filename>
            char *contentStart = command + 5;
            char *redirect = strstr(contentStart, " > ");
            if (!redirect) {
                printf("Invalid echo syntax. Use: echo <content> > <filename>\n");
                printf("Press any key to continue.\n");
                getch();
                continue;
            }

            // Extract content and filename
            *redirect = '\0';
            char *filename = redirect + 3;
            // Trim spaces
            while (isspace(*filename)) filename++;
            char *end = filename + strlen(filename) - 1;
            while (end > filename && isspace(*end)) *(end--) = '\0';
            while (isspace(*contentStart)) contentStart++;
            end = contentStart + strlen(contentStart) - 1;
            while (end > contentStart && isspace(*end)) *(end--) = '\0';

            if (strlen(filename) == 0) {
                printf("Please specify a filename.\n");
                printf("Press any key to continue.\n");
                getch();
                continue;
            }

            if (fileCount >= Total_files) {
                printf("Maximum number of files reached!\n");
                printf("Press any key to continue.\n");
                getch();
                continue;
            }

            // Check if file exists
            int fileIndex = -1;
            for (i = 0; i < fileCount; i++) {
                if (strcmp(files[i].name, filename) == 0) {
                    fileIndex = i;
                    break;
                }
            }

            if (fileIndex != -1) {
                // File exists, overwrite
                strncpy(files[fileIndex].fileContent, contentStart, 1000 - 1);
                files[fileIndex].fileContent[1000 - 1] = '\0';
                files[fileIndex].size = strlen(files[fileIndex].fileContent);
                printf("File %s overwritten successfully.\n", filename);
            } else {
                // Create new file
                strncpy(files[fileCount].name, filename, Total_fileName_length - 1);
                files[fileCount].name[Total_fileName_length - 1] = '\0';
                strncpy(files[fileCount].fileContent, contentStart, 1000 - 1);
                files[fileCount].fileContent[1000 - 1] = '\0';
                files[fileCount].size = strlen(files[fileCount].fileContent);
                files[fileCount].isDirectory = 0;
                fileCount++;
                printf("File %s created successfully.\n", filename);
            }
            printf("Press any key to continue.\n");
            getch();
        } else if (strcmp(command, "system") == 0) {
            printf("System Information:\n");
            printf("OS: SimpleOS v1.0 (Simulated)\n");
            printf("User: %s\n", currentUser);
            printf("Current Directory: %s\n", currentPath);
            printf("Total Files: %d\n", fileCount);
            printf("Memory Used: %d bytes (simulated)\n", fileCount * sizeof(FileEntry));
            printf("Press any key to continue.\n");
            getch();
        } else {
            printf("Command not found. Type 'help' for available commands.\n");
            printf("Press any key to continue.\n");
            getch();
        }
    }
}

void fileManagerMenu() {
    int choice;
    do {
        clearScreen();
        printf("\n=== File Manager ===\n");
        printf("1. List Files\n");
        printf("2. Create File\n");
        printf("3. Modify File\n");
        printf("4. Delete File\n");
        printf("5. View File\n");
        printf("6. Back to Main Menu\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            bufferClear();
            choice = -1;
        }
        bufferClear();

        switch (choice) {
            case 1:
                displayFiles();
                break;
            case 2:
                createFile();
                break;
            case 3:
                modifyFile();
                break;
            case 4:
                deleteFile();
                break;
            case 5:
                viewFile();
                break;
            case 6:
                return;
            default:
                printf("Invalid choice. Press any key to continue.\n");
                getch();
        }
        printf("Press any key to continue.\n");
        getch();
    } while (1);
}

// Display files (simulated) with size
void displayFiles() {
    int i=0;
    clearScreen();
    printf("\n=== File List ===\n");
    if (fileCount== 0) {
        printf("No files.\n");
    } else {
        for (i= 0; i<fileCount; i++) {
            printf("%d. %s %s (Size: %d bytes)\n", i + 1,
                   files[i].isDirectory ? "[DIR]" : "[FILE]",
                   files[i].name, files[i].size);
        }
    }
    printf("Press any key to continue.\n");
    getch();
}
// Create file (simulated)
void createFile() {
    int i=0;
    clearScreen();
    if (fileCount >= Total_files) {
        printf("Maximum number of files reached!\n");
        printf("Press any key to continue.\n");
        getch();
        return;
    }

    printf("Enter filename: ");
    char filename[Total_fileName_length];
    if (fgets(filename, Total_fileName_length,stdin)==NULL) {
        printf("Error reading filename!\n");
        printf("Press any key to continue.\n");
        getch();
        return;
    }
    filename[strcspn(filename, "\n")] = '\0';

    // This block Check if filename exists
    for (i = 0; i < fileCount; i++) {
        if (strcmp(files[i].name, filename) == 0) {
            printf("File already exists!\n");
            printf("Press any key to continue.\n");
            getch();
            return;
        }
    }

    printf("Enter content (type 'END' on a new line to finish):\n");
    char content[1000] = "";
    char line[100];
    while (fgets(line, sizeof(line), stdin)) {
        line[strcspn(line, "\n")] = '\0';
        if (strcmp(line, "END") == 0) break;
        if (strlen(content) + strlen(line) + 1 < sizeof(content)) {
            strcat(content, line);
            strcat(content, "\n");
        } else {
            printf("Content too large! Stopping input.\n");
            break;
        }
    }

    strncpy(files[fileCount].name, filename, Total_fileName_length - 1);
    files[fileCount].name[Total_fileName_length - 1] = '\0';
    strncpy(files[fileCount].fileContent, content, 1000 - 1);
    files[fileCount].fileContent[1000 - 1] = '\0';
    files[fileCount].size = strlen(files[fileCount].fileContent);
    files[fileCount].isDirectory = 0;
    fileCount++;
    printf("File created successfully! Press any key to continue.\n");
    getch();
}
// Modify file (simulated)
void modifyFile() {
    clearScreen();
    displayFiles();
    if (fileCount == 0) {
        printf("No files to modify.\n");
        printf("Press any key to continue.\n");
        getch();
        return;
    }

    printf("Enter file number to modify (1-%d): ", fileCount);
    int index;
    if (scanf("%d", &index) != 1 || index < 1 || index > fileCount) {
        bufferClear();
        printf("Invalid file number!\n");
        printf("Press any key to continue.\n");
        getch();
        return;
    }
    bufferClear();

    printf("\nCurrent content of %s:\n", files[index-1].name);
    printf("%s\n", files[index - 1].fileContent);
    printf("Current size: %d bytes\n", files[index-1].size);

    printf("\n=== Modify File ===\n");
    printf("1. Add content\n");
    printf("2. Change size\n");
    printf("3. Cancel\n");
    printf("Enter your choice: ");
    int choice;
    if (scanf("%d", &choice) != 1) {
        bufferClear();
        choice = -1;
    }
    bufferClear();

    switch (choice) {
        case 1: // Add content
            printf("Enter additional content (type 'END' on a new line to finish):\n");
            char newContent[1000] = "";
            char line[100];
            while (fgets(line, sizeof(line), stdin)) {
                line[strcspn(line, "\n")] = '\0';
                if (strcmp(line, "END") == 0) break;
                if (strlen(newContent) + strlen(line) + 1 < sizeof(newContent)) {
                    strcat(newContent, line);
                    strcat(newContent, "\n");
                } else {
                    printf("Content too large! Stopping input.\n");
                    break;
                }
            }

            if (strlen(files[index - 1].fileContent) + strlen(newContent) < 1000) {
                strcat(files[index - 1].fileContent, newContent);
                files[index - 1].size = strlen(files[index - 1].fileContent);
                printf("Content added successfully! New size: %d bytes\n", files[index-1].size);
            } else {
                printf("Cannot add content: File size limit reached.\n");
            }
            printf("Press any key to continue.\n");
            getch();
            break;

        case 2: // Change size
            printf("Enter new size (in bytes, max 999): ");
            int newSize;
            if (scanf("%d", &newSize) != 1 || newSize < 0 || newSize > 999) {
                bufferClear();
                printf("Invalid size!\n");
                printf("Press any key to continue.\n");
                getch();
                return;
            }
            bufferClear();

            files[index - 1].size = newSize;
            // Truncate content if new size is smaller
            if (files[index- 1].size<strlen(files[index-1].fileContent)){
                files[index- 1].fileContent[files[index-1].size]='\0';
            }
            printf("Size updated to %d bytes.\n", files[index - 1].size);
            printf("Press any key to continue.\n");
            getch();
            break;

        case 3:
            printf("Modification cancelled.\n");
            printf("Press any key to continue.\n");
            getch();
            break;

        default:
            printf("Invalid choice.\n");
            printf("Press any key to continue.\n");
            getch();
    }
}
// Delete file (simulated)
void deleteFile() {
    int i=0;
    clearScreen();
    displayFiles();
    if (fileCount == 0) {
        printf("No files to delete.\n");
        printf("Press any key to continue.\n");
        getch();
        return;
    }

    printf("Enter file number to delete (1-%d): ", fileCount);
    int index;
    if (scanf("%d", &index) != 1 || index < 1 || index > fileCount) {
        bufferClear();
        printf("Invalid file number!\n");
        printf("Press any key to continue.\n");
        getch();
        return;
    }
    bufferClear();

    for (i = index - 1; i < fileCount - 1; i++) {
        files[i] = files[i + 1];
    }
    fileCount--;
    printf("File deleted successfully! Press any key to continue.\n");
    getch();
}
// View file (simulated)
void viewFile() {
    clearScreen();
    displayFiles();
    if (fileCount == 0) {
        printf("No files to view.\n");
        printf("Press any key to continue.\n");
        getch();
        return;
    }

    printf("Enter file number to view (1-%d): ",fileCount);
    int index;
    if (scanf("%d", &index) != 1||index< 1||index>fileCount) {
        bufferClear();
        printf("Invalid file number!\n");
        printf("Press any key to continue.\n");
        getch();
        return;
    }
    bufferClear();

    printf("\n===== File Content ====\n");
    if (files[index - 1].fileContent[0] == '\0') {
        printf("(Empty file)\n");
    } else {
        printf("%s", files[index-1].fileContent);
        if (files[index-1].fileContent[strlen(files[index-1].fileContent)-1]!='\n') {
            printf("\n");
        }
    }
    printf("===== End of File =====\n");
    printf("Size: %d bytes\n", files[index-1].size);
    printf("Press any key to continue.\n");
    getch();
}
