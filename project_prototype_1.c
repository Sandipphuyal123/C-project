#include<stdio.h>
#include<stdlib.h>
#include<direct.h>
#include<string.h>
#include<conio.h>
#include<dirent.h>

char currentUser[100]="";
void registerUser();
int loginUser();
void mainMenu();
void terminal();

//void gameMenu();
//void fileManagerMenu();
//void calculator();
//void notesApp();
//void logoutUser();

void saveUsers() {
    DIR *dir;
    struct dirent *entry;
    FILE *centralFile, *userFile;
    char filePath[100], storedUsername[100], storedPassword[100];

    // Open the central users file for writing
    centralFile = fopen("users.txt", "w");
    if (centralFile == NULL) {
        perror("Error creating users.txt");
        return;
    }

    // Open the current directory
    dir = opendir(".");
    if (dir == NULL) {
        perror("Error opening directory");
        fclose(centralFile);
        return;
    }

    // Iterate through directory entries
    while ((entry = readdir(dir)) != NULL) {
        // Skip . and .. directories
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Check if it's a directory (basic check by attempting to read impFile.txt)
        sprintf(filePath, "%s\\impFile.txt", entry->d_name);
        userFile = fopen(filePath, "r");
        if (userFile == NULL) {
            continue; // Not a user directory or file not found
        }

        // Read username and password from impFile.txt
        if (fscanf(userFile, "Username: %99s\nPassword: %99s\n", storedUsername, storedPassword) == 2) {
            // Write to central users.txt
            fprintf(centralFile, "Username: %s, Password: %s\n", storedUsername, storedPassword);
        }
        fclose(userFile);
    }

    closedir(dir);
    fclose(centralFile);
}

void clearScreen(){
	system("cls");
}

void clearInputBuffer(){
	int c;
	while((c=getchar())!='\n'&& c!=EOF);
}

void maskPassword(char *password,int maxLength){
	int i=0;
	char ch;
	while((ch=getch())!='\r'&& i<maxLength-1){//  " \r " means enter key in ascii table
		if(ch=='\b'){//  " \b " means backspace in ascii table
			if(i>0){
				i--;
				printf("\b \b");
			}
		}
		else{
			password[i++]=ch;
			printf("*");
		}
		password[i]='\0';
	}
}

int main(){
	clearScreen();
	int choice;
    clearScreen();
    printf("\n");
    printf("\t\t  ¦¦¦+   ¦¦¦   +¦¦+   ¦¦¦+   ¦¦+   ¦¦+     ¦¦¦¦¦¦+    ¦¦¦¦¦¦¦+\n");
    printf("\t\t  ¦¦¦¦+ ¦¦¦¦   ¦¦¦¦   ¦¦¦¦+  ¦¦¦   ¦¦¦    ¦¦+---¦¦+   ¦¦+----+\n");
    printf("\t\t  ¦¦+¦¦¦¦+¦¦   ¦¦¦¦   ¦¦+¦¦+ ¦¦¦   ¦¦¦    ¦¦¦   ¦¦¦   ¦¦¦¦¦¦¦+\n");
    printf("\t\t  ¦¦¦+¦¦++¦¦   ¦¦¦¦   ¦¦¦+¦¦+¦¦¦   ¦¦¦    ¦¦¦   ¦¦¦   +----¦¦¦\n");
    printf("\t\t  ¦¦¦ +-+ ¦¦   ¦¦¦¦   ¦¦¦ +¦¦¦¦¦   ¦¦¦    +¦¦¦¦¦¦++   ¦¦¦¦¦¦¦¦\n");
    printf("\t\t  +-+     ++   -++-+  -+-+  +--+   +-+     +-----+    +------+\n");
    printf("\n");
    printf("\t\t                  Mini Operating System v1.0\n");
    printf("\t\t                    Booting system...\n\n");
    printf("Press any key to Continue.\n");
    getch();

    do{
	    clearScreen();
        printf("\n=== Welcome to Simple OS Interface ===\n");
		printf("\n1. Registration\n2. Login\n3. Exit\n\n");
		printf("[DETECT]:Enter your choice....\n");
		if(scanf("%d",&choice)!=1){
            clearInputBuffer();
            printf("\n[DETECT]: Invalid Argument! Please try again....\n");
            continue;
		}
    	clearInputBuffer();
		switch(choice){
            case 1:
                registerUser();
                break;
            case 2:
            	printf("This is a beta and is still on progress\n");
                if (loginUser()) {// if the code is a success then this will return 1 and goto 1;
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
	}while(1);

	return 0;
}

void registerUser(){
	char userName[100],password[100],filePath[100];
	printf("Username: ");
	fgets(userName,sizeof(userName),stdin);
	userName[strcspn(userName,"\n")]='\0';
	printf("Password: ");
	maskPassword(password,sizeof(password));
	printf("\n");
	if(mkdir(userName)==-1){
		perror("Error Creating file\n");
		return;
	}
	sprintf(filePath,"%s\\impFile.txt",userName);
	FILE *file;
	file=fopen(filePath,"w");
	if(file==NULL){
		perror("Error creating file");
		return;
	}
	fprintf(file,"Username: %s\nPassword: %s\n",userName,password);
	fclose(file);
	printf("\n[DETECT]:Registration completed....");

}

int loginUser(){
	int flag=0;
	char filePath[100],userName[100],password[100],storedUsername[100],storedPassword[100];
	printf("this is just the testing version\n");
	printf("Username: ");
	fgets(userName,sizeof(userName),stdin);
	userName[strcspn(userName,"\n")]='\0';
	printf("Password: ");
	maskPassword(password,sizeof(password));
	printf("\n");
	
	sprintf(filePath,"%s\\impFile.txt",userName);
	FILE *file;
	file=fopen(filePath,"r");
	if(file==NULL){
		perror("Couldn't access file");
		return;
	}
	fscanf(file,"Username: %s\nPassword: %s\n",storedUsername,storedPassword);
	fclose(file);
	if(strcmp(userName,storedUsername)==0&&strcmp(password,storedPassword)==0){
		printf("successfully Loged in\n");
		return flag=1;
	}
	else{
		printf("Invalid Username or Password\n");
		return flag=0;
	}
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
            clearInputBuffer();
            choice = -1;
        }
        clearInputBuffer();

        switch (choice) {
            case 1:
            	printf("This is a beta and is still on progress\n");
//                gameMenu();
                break;
            case 2:
            	printf("This is a beta and is still on progress\n");
                terminal();
                break;
            case 3:
            	printf("This is a beta and is still on progress\n");
//                fileManagerMenu();
                break;
            case 4:
            	printf("This is a beta and is still on progress\n");
//                calculator();
                break;
            case 5:
            	printf("This is a beta and is still on progress\n");
//                notesApp();
                break;
            case 6:
            	printf("This is a beta and is still on progress\n");
//                logoutUser();
                return;
            default:
                printf("Invalid choice. Press any key to continue.\n");
                getch();
        }
    } while (currentUser[0] != '\0');// there is an unknown current user
}

void terminal() {
    char command[256];
    char cwd[1024];

    while (1) {
        getcwd(cwd, sizeof(cwd)); // Get current directory
        printf("\n%s> ", cwd);    // Show path as prompt
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0'; // Remove newline

        // Command: exit
        if (strcmp(command, "exit") == 0) {
            printf("[TERMINAL]: Exiting terminal...\n");
            break;
        }

        // Command: clear
        else if (strcmp(command, "clear") == 0) {
            system("cls");
        }

        // Command: help
        else if (strcmp(command, "help") == 0) {
            printf("Available commands:\n");
            printf("  help    - Show this help message\n");
            printf("  clear   - Clear the screen\n");
            printf("  exit    - Exit terminal\n");
            printf("  echo    - Print text (e.g., echo Hello)\n");
            printf("  dir     - List directory contents\n");
            printf("  pwd     - Show current directory\n");
        }

        // Command: echo
        else if (strncmp(command, "echo ", 5) == 0) {
            printf("%s\n", command + 5);
        }

        // Command: dir
        else if (strcmp(command, "dir") == 0) {
            system("dir");  // Works on Windows
        }

        // Command: pwd
        else if (strcmp(command, "pwd") == 0) {
            printf("%s\n", cwd);
        }

        // Unknown command
        else {
            printf("Unknown command: %s\nType 'help' to list commands.\n", command);
        }
    }
}

