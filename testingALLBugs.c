#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <conio.h>
#include <direct.h>
#include <io.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>

#define RESET "\033[0m"
#define RED "\033[91m"
#define GREEN "\033[92m"
#define YELLOW "\033[93m"
#define BLUE "\033[94m"
#define CYAN "\033[96m"
#define WHITE "\033[97m"
#define BOLD "\033[1m"
#define DIM "\033[2m"

// System Constants
#define Total_users 100
#define Total_userName_length 30
#define Total_Password_length 30
#define Total_fileName_length 50
#define Total_absolutePath_length 100
#define MAX_CMD_LEN 256
#define MAX_HISTORY 100

int i=0;

// User structure
typedef struct {
    char username[Total_userName_length];
} User;

// Global variables
User users_arr[Total_users];
int totalUserCount = 0;
char currentUser[Total_userName_length] = "";
char currentPath[Total_absolutePath_length] = "";
char history[MAX_HISTORY][MAX_CMD_LEN];
int history_count = 0;

// Function prototypes
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
void terminalMenu();
void fileManagerMenu();
void calculator();
void notesApp();
void logoutUser();
void maskPassword(char *password, int maxLength);
void print_error(char *message);
void print_success(char *message);
char* virtualToReal(const char *virtualPath);
char* resolvePath(const char *current, const char *target);
void cmd_cd(char *dir);
void cmd_touch(char *filename);
void cmd_cat(char *filename);
void cmd_mkdir(char *dirname);
void cmd_dir();
void cmd_help();
void cmd_whoami();
void cmd_history();
void cmd_date();
void cmd_system();
void cmd_dogsay(char *text);
void createFile();
void modifyFile();
void deleteFile();
void viewFile();

int main() {
    loadUsers();
    srand(time(0));
    clearScreen();
    printf(BOLD CYAN "\n\t\t\t=== Welcome to HybridOS ===\n" RESET);
    printf("\n");
    printf(BOLD CYAN"\t\t  ¦¦¦+   ¦¦¦   +¦¦+   ¦¦¦+   ¦¦+   ¦¦+     ¦¦¦¦¦¦+    ¦¦¦¦¦¦¦+\n "RESET);
    printf(BOLD CYAN"\t\t  ¦¦¦¦+ ¦¦¦¦   ¦¦¦¦   ¦¦¦¦+  ¦¦¦   ¦¦¦    ¦¦+---¦¦+   ¦¦+----+\n "RESET);
    printf(BOLD CYAN"\t\t  ¦¦+¦¦¦¦+¦¦   ¦¦¦¦   ¦¦+¦¦+ ¦¦¦   ¦¦¦    ¦¦¦   ¦¦¦   ¦¦¦¦¦¦¦+\n "RESET);
    printf(BOLD CYAN"\t\t  ¦¦¦+¦¦++¦¦   ¦¦¦¦   ¦¦¦+¦¦+¦¦¦   ¦¦¦    ¦¦¦   ¦¦¦   +----¦¦¦\n "RESET);
    printf(BOLD CYAN"\t\t  ¦¦¦ +-+ ¦¦   ¦¦¦¦   ¦¦¦ +¦¦¦¦¦   ¦¦¦    +¦¦¦¦¦¦++   ¦¦¦¦¦¦¦¦\n "RESET);
    printf(BOLD CYAN"\t\t  +-+     ++   -++-+  -+-+  +--+   +-+     +-----+    +------+\n "RESET);
    printf("\n");
    printf(YELLOW" \t\t                  Mini Operating System v1.0\n"RESET);
    printf(YELLOW"\t\t                    Booting system...\n\n"RESET);
    printf("Press any key to Continue.\n");
    getch();

    int choice;
    do {
        clearScreen();
        printf(BOLD CYAN "\n=== HybridOS ===\n" RESET);
        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf(GREEN "Enter your choice: " RESET);
        if (scanf("%d", &choice) != 1) {
            bufferClear();
            printf(RED "Invalid input! Try again.\n" RESET);
            getch();
            continue;
        }
        bufferClear();

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                if (loginUser()) {
                    mainMenu();
                }
                break;
            case 3:
                saveUsers();
                printf(GREEN "Thank you for using HybridOS!\n" RESET);
                printf("Press any key to exit.\n");
                getch();
                return 0;
            default:
                printf(RED "Invalid choice! Try again.\n" RESET);
                getch();
        }
    } while (1);

    return 0;
}

void clearScreen() {
    system("cls");
}

void bufferClear() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // Clear buffer
    }
    if (c == EOF) {
        clearerr(stdin);
    }
}

void maskPassword(char *password, int maxLength) {
    int i = 0;
    char ch;
    while ((ch = getch()) != '\r' && i < maxLength - 1) {
        if (ch == '\b') {
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

void print_error(char *message) {
    printf(RED "Error: %s\n" RESET, message);
}

void print_success(char *message) {
    printf(GREEN "Success: %s\n" RESET, message);
}

void loadUsers() {
    FILE *file = fopen("masterFile.dat", "rb");
    if (file == NULL) {
        printf(CYAN "No existing users found.\n" RESET);
        return;
    }
    totalUserCount = fread(users_arr, sizeof(User), Total_users, file);
    printf(CYAN "Loaded %d users.\n" RESET, totalUserCount);
    fclose(file);
    printf("Press any key to continue.\n");
    getch();
}

void saveUsers() {
    FILE *file = fopen("masterFile.dat", "wb");
    if (file == NULL) {
        printf(RED "Error saving user data!\n" RESET);
        getch();
        return;
    }
    fwrite(users_arr, sizeof(User), totalUserCount, file);
    fclose(file);
}

void registerUser() {
    clearScreen();
    if (totalUserCount >= Total_users) {
        printf(RED "Maximum number of users reached!\n" RESET);
        printf("Press any key to continue.\n");
        getch();
        return;
    }

    char username[Total_userName_length];
    char password[Total_Password_length];
    char filePath[Total_absolutePath_length];

    printf(BOLD CYAN "\n=== User Registration ===\n" RESET);
    printf("Username: ");
    if (fgets(username, Total_userName_length, stdin) == NULL) {
        printf(RED "Error reading username!\n" RESET);
        printf("Press any key to continue.\n");
        getch();
        return;
    }
    username[strcspn(username, "\n")] = '\0';

    for (i = 0; i < totalUserCount; i++) {
        if (strcmp(users_arr[i].username, username) == 0) {
            printf(RED "Username already exists!\n" RESET);
            printf("Press any key to continue.\n");
            getch();
            return;
        }
    }

    printf("Password: ");
    maskPassword(password, Total_Password_length);

    if (_mkdir(username) == -1 && errno != EEXIST) {
        printf(RED "Error creating user directory: %s\n" RESET, strerror(errno));
        printf("Press any key to continue.\n");
        getch();
        return;
    }

    snprintf(filePath, Total_absolutePath_length, "%s\\impFile.txt", username);
    FILE *file = fopen(filePath, "w");
    if (file == NULL) {
        printf(RED "Error creating credentials file: %s\n" RESET, strerror(errno));
        printf("Press any key to continue.\n");
        getch();
        return;
    }
    fprintf(file, "Username: %s\nPassword: %s\n", username, password);
    fclose(file);

    strncpy(users_arr[totalUserCount].username, username, Total_userName_length - 1);
    users_arr[totalUserCount].username[Total_userName_length - 1] = '\0';
    totalUserCount++;
    saveUsers();
    printf(GREEN "\nRegistration completed.\n" RESET);
    printf("Press any key to continue.\n");
    getch();
}

int loginUser() {
    clearScreen();
    char username[Total_userName_length];
    char password[Total_Password_length];
    char filePath[Total_absolutePath_length];
    char storedUsername[Total_userName_length];
    char storedPassword[Total_Password_length];

    printf(BOLD CYAN "\n=== User Login ===\n" RESET);
    printf("Username: ");
    if (fgets(username, Total_userName_length, stdin) == NULL) {
        printf(RED "Error reading username!\n" RESET);
        printf("Press any key to continue.\n");
        getch();
        return 0;
    }
    username[strcspn(username, "\n")] = '\0';

    int userIndex = -1;
    for (i = 0; i < totalUserCount; i++) {
        if (strcmp(users_arr[i].username, username) == 0) {
            userIndex = i;
            break;
        }
    }
    if (userIndex == -1) {
        printf(RED "User not found.\n" RESET);
        printf("Press any key to continue.\n");
        getch();
        return 0;
    }

    if (_access(username, 0) != 0) {
        printf(RED "User directory not found. Account removed.\n" RESET);
        for (i = userIndex; i < totalUserCount - 1; i++) {
            users_arr[i] = users_arr[i + 1];
        }
        totalUserCount--;
        saveUsers();
        printf("Press any key to continue.\n");
        getch();
        return 0;
    }

    printf("Password: ");
    maskPassword(password, Total_Password_length);

    snprintf(filePath, Total_absolutePath_length, "%s\\impFile.txt", username);
    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        printf(RED "Couldn't access credentials file.\n" RESET);
        printf("Press any key to continue.\n");
        getch();
        return 0;
    }
    fscanf(file, "Username: %s\nPassword: %s\n", storedUsername, storedPassword);
    fclose(file);

    if (strcmp(password, storedPassword) == 0) {
        strncpy(currentUser, username, Total_userName_length - 1);
        currentUser[Total_userName_length - 1] = '\0';
        snprintf(currentPath, Total_absolutePath_length, "/%s", username);
        printf(GREEN "Login successful! Welcome, %s!\n" RESET, username);
        printf("Press any key to continue.\n");
        getch();
        return 1;
    } else {
        printf(RED "Invalid password.\n" RESET);
        printf("Press any key to continue.\n");
        getch();
        return 0;
    }
}

void logoutUser() {
    currentUser[0] = '\0';
    currentPath[0] = '\0';
    history_count = 0;
    printf(GREEN "Logged out successfully.\n" RESET);
    printf("Press any key to continue.\n");
    getch();
}

void mainMenu() {
    int choice;
    do {
        clearScreen();
        printf(BOLD CYAN "\n=== Main Menu === (User: %s)\n" RESET, currentUser);
        printf("1. Games\n");
        printf("2. Terminal\n");
        printf("3. File Manager\n");
        printf("4. Calculator\n");
        printf("5. Notes\n");
        printf("6. Logout\n");
        printf(GREEN "Enter your choice: " RESET);
        if (scanf("%d", &choice) != 1) {
            bufferClear();
            printf(RED "Invalid input! Try again.\n" RESET);
            getch();
            continue;
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
                printf(RED "Invalid choice. Press any key to continue.\n" RESET);
                getch();
        }
    } while (1);
}

void gameMenu() {
    int choice;
    do {
        clearScreen();
        printf(BOLD CYAN "\n=== Game Menu ===\n" RESET);
        printf("1. Quiz Game\n");
        printf("2. Tic Tac Toe\n");
        printf("3. Back to Main Menu\n");
        printf(GREEN "Enter your choice: " RESET);
        if (scanf("%d", &choice) != 1) {
            bufferClear();
            printf(RED "Invalid input! Try again.\n" RESET);
            getch();
            continue;
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
                printf(RED "Invalid choice. Press any key to continue.\n" RESET);
                getch();
        }
    } while (1);
}

void quizGame() {
    typedef struct {
        char question[200];
        char options[4][100];
        int rightAns;
    } quizQn;

    quizQn Questions[] = {
        {"What is the capital of USA?", {"London", "Washington DC", "Kathmandu", "Delhi"}, 1},
        {"Which planet is known as the Red Planet?", {"Venus", "Mars", "Jupiter", "Saturn"}, 1},
        {"What is 2 + 2?", {"3", "4", "5", "6"}, 1},
        {"Who painted the Mona Lisa?", {"Vincent van Gogh", "Pablo Picasso", "Leonardo da Vinci", "Michelangelo"}, 2},
        {"What is the largest mammal?", {"Elephant", "Blue Whale", "Giraffe", "Polar Bear"}, 1}
    };
    int score = 0;
    int totalQns = sizeof(Questions) / sizeof(Questions[0]);
    clearScreen();
    printf(BOLD CYAN "\n=== Quiz Game ===\n" RESET);
    printf("Answer the following questions:\n\n");
    for (i = 0; i < totalQns; i++) {
        printf("Question %d: %s\n", i + 1, Questions[i].question);
        for (int j = 0; j < 4; j++) {
            printf("%d. %s\n", j + 1, Questions[i].options[j]);
        }

        int answer;
        printf(GREEN "Your answer (1-4): " RESET);
        if (scanf("%d", &answer) != 1 || answer < 1 || answer > 4) {
            bufferClear();
            printf(RED "Invalid input! Assuming wrong answer.\n" RESET);
        } else {
            bufferClear();
            if (answer == Questions[i].rightAns + 1) {
                printf(GREEN "Correct!\n" RESET);
                score++;
            } else {
                printf(RED "Wrong! The correct answer was %d. %s\n" RESET,
                       Questions[i].rightAns + 1, Questions[i].options[Questions[i].rightAns]);
            }
        }
        printf("Press any key to continue.\n");
        getch();
    }
    printf(GREEN "Quiz completed! Your score: %d/%d\n" RESET, score, totalQns);
    printf("Press any key to return.\n");
    getch();
}

void ticTacToe() {
    char board[3][3];
    int row, col;
    char player = 'X';
    int moves = 0;

    for (i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = ' ';
        }
    }

    clearScreen();
    printf(BOLD CYAN "\n=== Tic Tac Toe ===\n" RESET);
    printf("Player 1: X\nPlayer 2: O\n");
    printf("Enter row (1-3) and column (1-3) to make your move, or 'q' to quit.\n\n");

    while (1) {
        printf("    1   2   3\n");
        for (i = 0; i < 3; i++) {
            printf("%d | %c | %c | %c |\n", i + 1, board[i][0], board[i][1], board[i][2]);
            if (i < 2) printf("  ---+---+---\n");
        }

        printf(GREEN "Player %c's turn. Enter row and column (or 'q' to quit): " RESET, player);
        char input[10];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            bufferClear();
            printf(RED "Invalid input! Try again.\n" RESET);
            printf("Press any key to continue.\n");
            getch();
            continue;
        }

        input[strcspn(input, "\n")] = '\0';
        if (strcmp(input, "q") == 0 || strcmp(input, "Q") == 0) {
            printf(GREEN "Game aborted. Returning to main menu.\n" RESET);
            printf("Press any key to continue.\n");
            getch();
            return;
        }

        if (sscanf(input, "%d %d", &row, &col) != 2 || row < 1 || row > 3 || col < 1 || col > 3) {
            bufferClear();
            printf(RED "Invalid position! Try again.\n" RESET);
            printf("Press any key to continue.\n");
            getch();
            continue;
        }
        bufferClear();

        if (board[row - 1][col - 1] != ' ') {
            printf(RED "Position already taken! Try again.\n" RESET);
            printf("Press any key to continue.\n");
            getch();
            continue;
        }

        board[row - 1][col - 1] = player;
        moves++;

        int win = 0;
        for (i = 0; i < 3; i++) {
            if (board[i][0] == player && board[i][1] == player && board[i][2] == player) {
                win = 1;
                break;
            }
        }
        for (int j = 0; j < 3; j++) {
            if (board[0][j] == player && board[1][j] == player && board[2][j] == player) {
                win = 1;
                break;
            }
        }
        if (board[0][0] == player && board[1][1] == player && board[2][2] == player) {
            win = 1;
        }
        if (board[0][2] == player && board[1][1] == player && board[2][0] == player) {
            win = 1;
        }

        if (win) {
            clearScreen();
            printf("    1   2   3\n");
            for (i = 0; i < 3; i++) {
                printf("%d | %c | %c | %c |\n", i + 1, board[i][0], board[i][1], board[i][2]);
                if (i < 2) printf("  ---+---+---\n");
            }
            printf(GREEN "Player %c wins! Press any key to return.\n" RESET, player);
            getch();
            break;
        }

        if (moves == 9) {
            clearScreen();
            printf("    1   2   3\n");
            for (i = 0; i < 3; i++) {
                printf("%d | %c | %c | %c |\n", i + 1, board[i][0], board[i][1], board[i][2]);
                if (i < 2) printf("  ---+---+---\n");
            }
            printf(GREEN "It's a draw! Press any key to return.\n" RESET);
            getch();
            break;
        }

        player = (player == 'X') ? 'O' : 'X';
    }
}

void calculator() {
    double num1, num2, result;
    char op;

    clearScreen();
    printf(BOLD CYAN "\n=== Simple Calculator ===\n" RESET);
    while (1) {
        printf(GREEN "Enter an operation (+, -, *, /) or 'q' to quit: " RESET);
        if (scanf(" %c", &op) != 1) {
            bufferClear();
            printf(RED "Invalid input! Try again.\n" RESET);
            printf("Press any key to continue.\n");
            getch();
            continue;
        }
        bufferClear();

        if (op == 'q' || op == 'Q') break;

        printf(GREEN "Enter first number: " RESET);
        if (scanf("%lf", &num1) != 1) {
            bufferClear();
            printf(RED "Invalid number! Try again.\n" RESET);
            printf("Press any key to continue.\n");
            getch();
            continue;
        }
        bufferClear();

        printf(GREEN "Enter second number: " RESET);
        if (scanf("%lf", &num2) != 1) {
            bufferClear();
            printf(RED "Invalid number! Try again.\n" RESET);
            printf("Press any key to continue.\n");
            getch();
            continue;
        }
        bufferClear();

        switch (op) {
            case '+':
                result = num1 + num2;
                break;
            case '-':
                result = num1 - num2;
                break;
            case '*':
                result = num1 * num2;
                break;
            case '/':
                if (num2 == 0) {
                    printf(RED "Error: Division by zero!\n" RESET);
                    printf("Press any key to continue.\n");
                    getch();
                    continue;
                }
                result = num1 / num2;
                break;
            default:
                printf(RED "Invalid operator!\n" RESET);
                printf("Press any key to continue.\n");
                getch();
                continue;
        }

        printf(GREEN "Result: %.2lf %c %.2lf = %.2lf\n" RESET, num1, op, num2, result);
        printf("Press any key to continue.\n");
        getch();
    }
}

void notesApp() {
    int choice;
    do {
        clearScreen();
        printf(BOLD CYAN "\n=== Notes App ===\n" RESET);
        printf("1. Create New Note\n");
        printf("2. View Note\n");
        printf("3. Back to Main Menu\n");
        printf(GREEN "Enter your choice: " RESET);
        if (scanf("%d", &choice) != 1) {
            bufferClear();
            printf(RED "Invalid input! Try again.\n" RESET);
            getch();
            continue;
        }
        bufferClear();

        switch (choice) {
            case 1:
                createFile();
                break;
            case 2:
                viewFile();
                break;
            case 3:
                return;
            default:
                printf(RED "Invalid choice. Press any key to continue.\n" RESET);
                getch();
        }
    } while (1);
}

char* virtualToReal(const char *virtualPath) {
    static char realPath[Total_absolutePath_length];
    snprintf(realPath, Total_absolutePath_length, ".%s", virtualPath);
    for (char *p = realPath; *p; p++) {
        if (*p == '/') *p = '\\';
    }
    return realPath;
}

char* resolvePath(const char *current, const char *target) {
    static char newPath[Total_absolutePath_length];
    char fullPath[Total_absolutePath_length];
    char components[50][Total_fileName_length];
    int count = 0;

    if (target[0] == '/') {
        snprintf(fullPath, Total_absolutePath_length, "/%s%s", currentUser, target);
    } else {
        snprintf(fullPath, Total_absolutePath_length, "%s/%s", current, target);
    }

    char *token = strtok(fullPath, "/");
    while (token != NULL) {
        if (strcmp(token, "") == 0 || strcmp(token, ".") == 0) {
            // Ignore
        } else if (strcmp(token, "..") == 0) {
            if (count > 1) count--; // Don't go above /<username>
        } else {
            strncpy(components[count], token, Total_fileName_length - 1);
            components[count][Total_fileName_length - 1] = '\0';
            count++;
        }
        token = strtok(NULL, "/");
    }

    newPath[0] = '\0';
    for (i = 0; i < count; i++) {
        strcat(newPath, "/");
        strcat(newPath, components[i]);
    }
    if (count == 0) strcpy(newPath, "/");
    return newPath;
}

void terminalMenu() {
    clearScreen();
    printf(BOLD CYAN "\n=== HybridOS Terminal === (type 'exit' to return)\n" RESET);
    while (1) {
        printf(BOLD GREEN "%s@hybridos" RESET ":" BOLD BLUE "%s" RESET "$ ", currentUser, currentPath);
        char command[MAX_CMD_LEN];
        if (fgets(command, MAX_CMD_LEN, stdin) == NULL) {
            bufferClear();
            printf(RED "Failed to read command.\n" RESET);
            printf("Press any key to continue.\n");
            getch();
            continue;
        }
        command[strcspn(command, "\n")] = '\0';

        if (command[0] == '\0') continue;

        if (history_count < MAX_HISTORY) {
            strncpy(history[history_count], command, MAX_CMD_LEN - 1);
            history[history_count][MAX_CMD_LEN - 1] = '\0';
            history_count++;
        }

        char *token = strtok(command, " ");
        if (token == NULL) continue;
        for (i = 0; token[i]; i++) {
            token[i] = tolower(token[i]);
        }
        char *args = strtok(NULL, "");

        if (strcmp(token, "exit") == 0) {
            break;
        } else if (strcmp(token, "help") == 0) {
            cmd_help();
        } else if (strcmp(token, "clear") == 0) {
            clearScreen();
        } else if (strcmp(token, "cd") == 0) {
            cmd_cd(args);
        } else if (strcmp(token, "touch") == 0) {
            cmd_touch(args);
        } else if (strcmp(token, "cat") == 0) {
            cmd_cat(args);
        } else if (strcmp(token, "mkdir") == 0) {
            cmd_mkdir(args);
        } else if (strcmp(token, "dir") == 0) {
            cmd_dir();
        } else if (strcmp(token, "whoami") == 0) {
            cmd_whoami();
        } else if (strcmp(token, "history") == 0) {
            cmd_history();
        } else if (strcmp(token, "date") == 0) {
            cmd_date();
        } else if (strcmp(token, "system") == 0) {
            cmd_system();
        } else if (strcmp(token, "dogsay") == 0) {
            cmd_dogsay(args);
        } else {
            printf(RED "Command not found. Type 'help' for commands.\n" RESET);
        }
        printf("Press any key to continue.\n");
        getch();
    }
}

void cmd_cd(char *dir) {
    if (dir == NULL || strlen(dir) == 0) {
        snprintf(currentPath, Total_absolutePath_length, "/%s", currentUser);
        print_success("Changed to home directory");
        return;
    }

    char *newPath = resolvePath(currentPath, dir);
    char *realPath = virtualToReal(newPath);
    if (_access(realPath, 0) == 0) {
        strcpy(currentPath, newPath);
        print_success("Directory changed");
    } else {
        print_error("Directory not found");
    }
}

void cmd_touch(char *filename) {
    if (filename == NULL || strlen(filename) == 0) {
        print_error("Usage: touch <filename>");
        return;
    }

    while (isspace(*filename)) filename++;
    char *end = filename + strlen(filename) - 1;
    while (end > filename && isspace(*end)) *(end--) = '\0';

    if (strlen(filename) == 0) {
        print_error("Please specify a filename");
        return;
    }

    char fullPath[Total_absolutePath_length];
    snprintf(fullPath, Total_absolutePath_length, "%s/%s", currentPath, filename);
    char *realPath = virtualToReal(fullPath);
    FILE *file = fopen(realPath, "w");
    if (file == NULL) {
        print_error("Failed to create file");
        return;
    }
    fclose(file);
    print_success("File created");
}

void cmd_cat(char *filename) {
    if (filename == NULL || strlen(filename) == 0) {
        print_error("Usage: cat <filename>");
        return;
    }

    char fullPath[Total_absolutePath_length];
    snprintf(fullPath, Total_absolutePath_length, "%s/%s", currentPath, filename);
    char *realPath = virtualToReal(fullPath);
    FILE *file = fopen(realPath, "r");
    if (file == NULL) {
        print_error("File not found");
        return;
    }
    printf(CYAN "Contents of %s:\n" RESET, filename);
    printf(DIM "----------------------------------------\n" RESET);
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file)) {
        printf("%s", buffer);
    }
    fclose(file);
    printf(DIM "----------------------------------------\n" RESET);
}

void cmd_mkdir(char *dirname) {
    if (dirname == NULL || strlen(dirname) == 0) {
        print_error("Usage: mkdir <directory_name>");
        return;
    }

    while (isspace(*dirname)) dirname++;
    char *end = dirname + strlen(dirname) - 1;
    while (end > dirname && isspace(*end)) *(end--) = '\0';

    if (strlen(dirname) == 0) {
        print_error("Please specify a directory name");
        return;
    }

    char fullPath[Total_absolutePath_length];
    snprintf(fullPath, Total_absolutePath_length, "%s/%s", currentPath, dirname);
    char *realPath = virtualToReal(fullPath);
    if (_mkdir(realPath) == 0) {
        print_success("Directory created");
    } else {
        print_error("Failed to create directory");
    }
}

void cmd_dir() {
    char *realPath = virtualToReal(currentPath);
    DIR *dir = opendir(realPath);
    if (dir == NULL) {
        print_error("Cannot open directory");
        return;
    }
    printf(BOLD "Contents of %s:\n" RESET, currentPath);
    struct dirent *entry;
    int count = 0;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        count++;
    }
    rewinddir(dir);
    printf(CYAN "Total %d items\n\n" RESET, count);
    int index = 1;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        struct stat st;
        char fullPath[Total_absolutePath_length];
        snprintf(fullPath, Total_absolutePath_length, "%s/%s", currentPath, entry->d_name);
        char *realFullPath = virtualToReal(fullPath);
        stat(realFullPath, &st);
        if (S_ISDIR(st.st_mode)) {
            printf(BOLD BLUE "%d. %s/\n" RESET, index++, entry->d_name);
        } else {
            printf(WHITE "%d. %s" RESET " " DIM "(%ld bytes, Created: %s)" RESET,
                   index++, entry->d_name, st.st_size, ctime(&st.st_ctime));
        }
    }
    closedir(dir);
    printf("\n");
}

void cmd_help() {
    printf(BOLD YELLOW "\n=== HybridOS Command Reference ===\n" RESET);
    printf(CYAN "----------------------------------------\n" RESET);
    printf(BOLD "File System Commands:\n" RESET);
    printf("  cd <dir>      - Change directory\n");
    printf("  touch <file>  - Create empty file\n");
    printf("  cat <file>    - Display file contents\n");
    printf("  mkdir <dir>   - Create directory\n");
    printf("  dir           - List directory contents\n");
    printf(BOLD "Utility Commands:\n" RESET);
    printf("  whoami        - Show current user\n");
    printf("  history       - Show command history\n");
    printf("  date          - Show current date/time\n");
    printf("  system        - Show system information\n");
    printf("  dogsay <text> - Display text with dog ASCII art\n");
    printf("  help          - Show this help\n");
    printf("  exit          - Return to main menu\n");
    printf(CYAN "----------------------------------------\n" RESET);
}

void cmd_whoami() {
    printf(GREEN "Current user: %s\n" RESET, currentUser);
}

void cmd_history() {
    printf(BOLD YELLOW "Command History\n" RESET);
    printf(CYAN "----------------------------------------\n" RESET);
    for (i = 0; i < history_count; i++) {
        printf("%d %s\n", i + 1, history[i]);
    }
    printf("\n");
}

void cmd_date() {
    time_t now = time(NULL);
    char *time_str = ctime(&now);
    time_str[strlen(time_str) - 1] = '\0';
    printf(CYAN "Current Date & Time: %s\n" RESET, time_str);
}

void cmd_system() {
    printf(BOLD YELLOW "System Information\n" RESET);
    printf(CYAN "----------------------------------------\n" RESET);
    printf("OS: HybridOS v1.0\n");
    printf("User: %s\n", currentUser);
    printf("Current Directory: %s\n", currentPath);
    printf("\n");
}

void cmd_dogsay(char *text) {
    if (text == NULL || strlen(text) == 0) {
        print_error("Usage: dogsay <text>");
        return;
    }
    printf(CYAN " %s \n" RESET, text);
    printf(" \\ \n");
    printf("  \\ \n");
    printf("    /\\_/\\ \n");
    printf("   ( o o ) \n");
    printf("   =( I )= \n");
}

void fileManagerMenu() {
    int choice;
    do {
        clearScreen();
        printf(BOLD CYAN "\n=== File Manager ===\n" RESET);
        printf("Current Directory: %s\n", currentPath);
//        printf("1. List Files\n");
        printf("1. Create File\n");
        printf("2. Modify File\n");
        printf("3. Delete File\n");
        printf("4. View File\n");
        printf("5. Back to Main Menu\n");
        printf(GREEN "Enter your choice: " RESET);
        if (scanf("%d", &choice) != 1) {
            bufferClear();
            printf(RED "Invalid input! Try again.\n" RESET);
            getch();
            continue;
        }
        bufferClear();

        switch (choice) {
            case 1:
                createFile();
                break;
            case 2:
                modifyFile();
                break;
            case 3:
                deleteFile();
                break;
            case 4:
                viewFile();
                break;
            case 5:
                return;
            default:
                printf(RED "Invalid choice. Press any key to continue.\n" RESET);
                getch();
        }
    } while (1);
}

void createFile() {
    clearScreen();
    printf(GREEN "Enter filename: " RESET);
    char filename[Total_fileName_length];
    if (fgets(filename, Total_fileName_length, stdin) == NULL) {
        printf(RED "Error reading filename!\n" RESET);
        printf("Press any key to continue.\n");
        getch();
        return;
    }
    filename[strcspn(filename, "\n")] = '\0';

    if (strlen(filename) == 0) {
        print_error("Please specify a filename");
        printf("Press any key to continue.\n");
        getch();
        return;
    }

    char fullPath[Total_absolutePath_length];
    snprintf(fullPath, Total_absolutePath_length, "%s/%s", currentPath, filename);
    char *realPath = virtualToReal(fullPath);
    if (_access(realPath, 0) == 0) {
        print_error("File already exists");
        printf("Press any key to continue.\n");
        getch();
        return;
    }

    printf(GREEN "Enter content (type 'END' on a new line to finish):\n" RESET);
    FILE *file = fopen(realPath, "w");
    if (file == NULL) {
        print_error("Failed to create file");
        printf("Press any key to continue.\n");
        getch();
        return;
    }
    char line[1024];
    while (fgets(line, sizeof(line), stdin)) {
        line[strcspn(line, "\n")] = '\0';
        if (strcmp(line, "END") == 0) break;
        fprintf(file, "%s\n", line);
    }
    fclose(file);
    print_success("File created");
    printf("Press any key to continue.\n");
    getch();
}

void modifyFile() {
    clearScreen();
    cmd_dir();
    printf(GREEN "Enter filename to modify: " RESET);
    char filename[Total_fileName_length];
    if (fgets(filename, Total_fileName_length, stdin) == NULL) {
        printf(RED "Error reading filename!\n" RESET);
        printf("Press any key to continue.\n");
        getch();
        return;
    }
    filename[strcspn(filename, "\n")] = '\0';

    if (strlen(filename) == 0) {
        print_error("Please specify a filename");
        printf("Press any key to continue.\n");
        getch();
        return;
    }

    char fullPath[Total_absolutePath_length];
    snprintf(fullPath, Total_absolutePath_length, "%s/%s", currentPath, filename);
    char *realPath = virtualToReal(fullPath);
    if (_access(realPath, 0) != 0) {
        print_error("File not found");
        printf("Press any key to continue.\n");
        getch();
        return;
    }

    printf(CYAN "\nCurrent content of %s:\n" RESET, filename);
    printf(DIM "----------------------------------------\n" RESET);
    FILE *file = fopen(realPath, "r");
    if (file == NULL) {
        printf(DIM "(Unable to read file)\n" RESET);
    } else {
        char buffer[1024];
        while (fgets(buffer, sizeof(buffer), file)) {
            printf("%s", buffer);
        }
        fclose(file);
    }
    printf(DIM "----------------------------------------\n" RESET);

    printf(GREEN "Enter new content (type 'END' on a new line to finish):\n" RESET);
    char line[1024];
    file = fopen(realPath, "w");
    if (file == NULL) {
        print_error("Failed to modify file");
        printf("Press any key to continue.\n");
        getch();
        return;
    }
    while (fgets(line, sizeof(line), stdin)) {
        line[strcspn(line, "\n")] = '\0';
        if (strcmp(line, "END") == 0) break;
        fprintf(file, "%s\n", line);
    }
    fclose(file);
    print_success("File modified");
    printf("Press any key to continue.\n");
    getch();
}

void deleteFile() {
    clearScreen();
    cmd_dir();
    printf(GREEN "Enter filename to delete: " RESET);
    char filename[Total_fileName_length];
    if (fgets(filename, Total_fileName_length, stdin) == NULL) {
        printf(RED "Error reading filename!\n" RESET);
        printf("Press any key to continue.\n");
        getch();
        return;
    }
    filename[strcspn(filename, "\n")] = '\0';

    if (strlen(filename) == 0) {
        print_error("Please specify a filename");
        printf("Press any key to continue.\n");
        getch();
        return;
    }

    char fullPath[Total_absolutePath_length];
    snprintf(fullPath, Total_absolutePath_length, "%s/%s", currentPath, filename);
    char *realPath = virtualToReal(fullPath);
    struct stat st;
    if (stat(realPath, &st) != 0) {
        print_error("File or directory not found");
        printf("Press any key to continue.\n");
        getch();
        return;
    }

    if (S_ISDIR(st.st_mode)) {
        if (_rmdir(realPath) != 0) {
            print_error("Failed to delete directory");
        } else {
            print_success("Directory deleted");
        }
    } else {
        if (remove(realPath) != 0) {
            print_error("Failed to delete file");
        } else {
            print_success("File deleted");
        }
    }
    printf("Press any key to continue.\n");
    getch();
}

void viewFile() {
    clearScreen();
    cmd_dir();
    printf(GREEN "Enter filename to view: " RESET);
    char filename[Total_fileName_length];
    if (fgets(filename, Total_fileName_length, stdin) == NULL) {
        printf(RED "Error reading filename!\n" RESET);
        printf("Press any key to continue.\n");
        getch();
        return;
    }
    filename[strcspn(filename, "\n")] = '\0';

    if (strlen(filename) == 0) {
        print_error("Please specify a filename");
        printf("Press any key to continue.\n");
        getch();
        return;
    }

    char fullPath[Total_absolutePath_length];
    snprintf(fullPath, Total_absolutePath_length, "%s/%s", currentPath, filename);
    char *realPath = virtualToReal(fullPath);
    FILE *file = fopen(realPath, "r");
    if (file == NULL) {
        print_error("File not found");
        printf("Press any key to continue.\n");
        getch();
        return;
    }
    printf(CYAN "\nContents of %s:\n" RESET, filename);
    printf(DIM "----------------------------------------\n" RESET);
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file)) {
        printf("%s", buffer);
    }
    fclose(file);
    printf(DIM "----------------------------------------\n" RESET);
    printf("Press any key to continue.\n");
    getch();
}
