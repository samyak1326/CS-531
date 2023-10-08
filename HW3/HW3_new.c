#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

struct address_t* CreateList(char inputstr[45]);
struct address_t* CreateInitialList(char inputstr[45]);

int check_For_Choice(int choice);
int save_to_File();
int displayAliasForLocation();
bool validateAddress(int ipv4Adrs[]);
int addAddress();
void DisplayAlias(int ipv4Adrs[]);
bool validateAdrsLocation(int ipv4Adrs[]);
bool checkUnique(int ipv4Adrs[], char aliasname[]);
char* toLowerStr(char str[]);
int lookUpAddress();
int UpdateAddress();
bool checkUniqueAdrs(int ipv4Adrs[]);
void updateAdrs(int ipv4Adrs[4], char aliasname[]);
int deleteAddress();
int PrintList();
bool validateAlias(char aliasname[20]);
void deleteRecord(char aliasname[20]);

struct address_t 
{ 
int octet[4]; 
char alias[11]; 
struct address_t *next; 
};

struct address_t *head = NULL; 

int main()
{
    char choice ; 
    int tempOption;

    FILE* filePtr;
    int ch;
    char inputStr[35]="";

    filePtr=fopen("CS531_Inet.txt","r");

    if(!filePtr)
    {
        perror("The file /CS531_Inet.txt/ does not exist. Check filename or try AGAIN\n");
        exit(EXIT_FAILURE);
    }
    do{
        int i=0;
        ch = fgetc(filePtr);

        if(ch != EOF && ch != 10)
            inputStr[i++]=ch;

        else{

            if(NULL != CreateInitialList(inputStr)){}
            else{

                printf("Error while creating the node\n");
            }
            i=0;
            memset(inputStr,0,35);
        }
    }while(ch != EOF);
    fclose(filePtr);

    do
    {
        /* code */
        printf("1: Add Address\n");
        printf("2: Look up Address\n");
        printf("3: Update Address\n");
        printf("4: Delete Address\n");
        printf("5: Display List\n");
        printf("6: Display aliases for location\n");
        printf("7: Save to File\n");
        printf("8: Quit\n");

        fflush(stdin);
        printf("\nEnter your Choice: ");
        scanf("%c", &choice);

        tempOption=choice-'0';

        if(tempOption >= 1 && tempOption <= 7)
            tempOption = check_For_Choice(tempOption);

        else if(tempOption==8)
        {
            printf("QUIT. THANK YOU\n");
            break;
        }

        else
            printf("Invalid input: Try AGAIN (1-8) \n");
    } 
    while (!(tempOption >=1 && tempOption <=8));
    return 0; 
}

int check_For_Choice(int choice)
{                                             
    switch(choice){
        case 1:
            printf("1: Add Address\n");
            addAddress();
            break;

        case 2:
            printf("2: Look up address\n");
            lookUpAddress();
            break;

        case 3:
            printf("3: Update addres\n");
            UpdateAddress();
            break;

        case 4:
            printf("4: Delete address\n");
            deleteAddress();
            break;

        case 5:
            printf("5: Display List\n");
            PrintList();
            break;

        case 6:
            printf("6: Display aliases for location\n");
            displayAliasForLocation();
            break;

        case 7:
            printf("7: Save to File\n");
            save_to_File();
            break;
    }
    return 0;
}

struct address_t* CreateList(char inputstr[45])
{                                            
    struct address_t *curr = NULL;
    struct address_t *node=(struct address_t*)malloc(sizeof(struct address_t));

    if(NULL==node)
    {
        printf("Exception while creating the node\n");
        return NULL;
    }

    sscanf(inputstr, "%d.%d.%d.%d %s", node->octet, node->octet+1, 
        node->octet+2, node->octet+3, node->alias);

    node->next = NULL;
    curr = node;
    head = curr;
    return head;

}

struct address_t* CreateInitialList(char inputstr[45])
{
    if(NULL == head)
    {
        return (CreateList(inputstr));
    }

    struct address_t *node = (struct address_t*)malloc(sizeof(struct address_t));
    
    if(NULL == head)
    {
        printf("\nNode creation failed\n");
        return    NULL;
    }
    sscanf(inputstr, "%d.%d.%d.%d %s", node->octet, node->octet+1, 
        node->octet+2, node->octet+3, node->alias);
    node->next = head;
    head = node;
    return head;

}

int save_to_File()          //save the data to new file
{
    FILE *file;
    int x;
    char newFile[20];

    struct address_t *node = head;
    printf("Enter New File Name: \n");

    fflush(stdin);
    fgets(newFile,20, stdin);

    newFile[strlen(newFile)-1]  = '\0';

    file = fopen(newFile, "w");     //write to new file
    if(!file)
    {
        printf("File could not be created. Try again \n");
        return 0;
    }

    while(node != NULL)
    {
        fprintf(file,"%d.%d.%d.%d %s\n", node->octet[0], node->octet[1], 
            node->octet[2], node->octet[3], toLowerStr(node->alias));

        node = node->next;
    }
    fclose(file);
    printf("Data Saved To The FILE");
    return 0;
}
char* toLowerStr(char str[])
{
    int i = 0;
    char *newstr = (char*)malloc(strlen(str));
    while(str[i]){
        newstr[i] = tolower(str[i]);
        i++;
    }
    return newstr;
}

bool validateAddress(int ipv4Adrs[])
{
    bool valid_ipv4=true;

    for(int i=0; i < 4; i++)
    {
        if(ipv4Adrs[i] <= 255 && ipv4Adrs[i] >= 0){
            valid_ipv4 = true;
        }
        else
        {
            valid_ipv4 = false;
            break;
        }
    }
    return valid_ipv4;

}
bool validateAlias(char aliasname[20])
{
    bool valid = true;
    if(strlen(aliasname) > 10 || strlen(aliasname) <= 0 
        || strchr(aliasname, ' ') || strchr(aliasname, '\t')){
            valid = false;
        }
    return valid;
}

bool checkUnique(int ipv4Adrs[], char aliasname[])
{
    bool check = true;
    struct address_t *node = head;

    while(node != NULL){
        if(node->octet[0]==*(ipv4Adrs) && node->octet[1]==*(ipv4Adrs+1)
            && node->octet[2]==*(ipv4Adrs+2) && node->octet[3]==*(ipv4Adrs+3))
        {
            printf("\nThis IPv4 Address ");
            check = false;
            break;
        }

        else if(strcmp(toLowerStr(node->alias),toLowerStr(aliasname))==0){
            printf("\nThis /%s/ Alias", aliasname);
            check = false;
            break;
        }
        node = node->next;
    }
    return check;
}
int addAddress()
{                                                                          
    int ipv4Adrs[4];
    char aliasName[20];
    char ipStr[25];
    char str[35]="";

    printf("Enter a Valid IPv4 Address : ");

    do{
        fflush(stdin);
        fgets(ipStr, 25, stdin);
        sscanf(ipStr, "%d.%d.%d.%d", ipv4Adrs, ipv4Adrs+1, ipv4Adrs+2, ipv4Adrs+3);

        if(validateAddress(ipv4Adrs))
        {                                                      
            printf("Enter the ALIAS name: ");
            do{
                fflush(stdin);
                fgets(aliasName, 15, stdin);
                aliasName[strlen(aliasName)-1]='\0';
                
                if(validateAlias(aliasName))
                {                                               
                    if(checkUnique(ipv4Adrs,aliasName))
                    {                                    
                        strcat(ipStr," ");
                        strcat(ipStr,aliasName);
                        CreateInitialList(ipStr);
                    }
                    else
                        printf(" Already exists!!\n");
                    
                }
                else
                    printf("Please Enter a valid and unique Alias Name : ");
            }
            while(!validateAlias(aliasName));
        }
        else
        {
            printf("\n A valid IPv4 address looks like this: (000.000.000.000) \n" );
            printf("Please enter a valid IPv4 Address : ");
        }

    }
    while(!validateAddress(ipv4Adrs));
    return 0;
}

void DisplayAlias(int ipv4Adrs[])
{
    int cnt = 0;
    struct address_t *node = head;
    while(node != NULL)
    {
        if(node->octet[0]==*(ipv4Adrs) && node->octet[1]==*(ipv4Adrs+1))
        {
            printf("%s\n",node->alias);
            cnt++;
        }
        node = node->next;
    }
    if(cnt < 1){
        printf("This Location Does Not Exist in the Given List\n");
    }
    return;
}

bool validateAdrsLocation(int ipv4Adrs[])
{
    bool check = true;
    for(int i=0; i<2; i++)
    {
        if(ipv4Adrs[i] <= 255 && ipv4Adrs[i] >= 0)
            check = true;
        else{
            check = false;
            break;
        }
    }
    return check;
}

int displayAliasForLocation()
{
    char IpStr[30];
    int ipv4Adrs[4];

    printf("Enter The Address Location: ");
    do
    {
        fflush(stdin);
        fgets(IpStr, 30, stdin);
        sscanf(IpStr, "%d.%d", ipv4Adrs, ipv4Adrs+1);

        if(validateAdrsLocation(ipv4Adrs))
        {                                              
            DisplayAlias(ipv4Adrs);                                                         
        }

        else{
            printf("Please Enter A Valid IPv4 address : ");
        }

    }while(!validateAddress(ipv4Adrs));                                                     
    return 0;
}


int lookUpAddress()
{
    char aliasname[20]="";
    bool check = false;

    printf("Enter the Alias Name to search Address: ");

    do{
        fflush(stdin);
        fgets(aliasname, 20, stdin);
        aliasname[strlen(aliasname)-1] = '\0';
        if(validateAlias(aliasname))
        {
            struct address_t *node = head;

            while (node != NULL)
            {
                if(strcmp(toLowerStr(node->alias),toLowerStr(aliasname))==0)
                {
                    printf("Alias found!! %d.%d.%d.%d %s\n", node->octet[0], node->octet[1], node->octet[2], node->octet[3], node->alias);
                    check = true;
                }
                node = node->next;
            }
            if(!check)
                printf("Alias Name not found. TRY AGAIN \n");
                  
        }
        else
            printf("Please Enter A valid Alias Name: ");  
    
    }while(!validateAlias(aliasname));

    return 0;
}

bool checkUniqueAdrs(int ipv4Adrs[]){
    struct address_t *node = head;

    bool unique = true;
    printf("%d.%d.%d.%d", ipv4Adrs[0], ipv4Adrs[1], ipv4Adrs[2], ipv4Adrs[3]);

    while(node != NULL)
    {
        if(node->octet[0]==*(ipv4Adrs) && node->octet[1]==*(ipv4Adrs+1) && node->octet[2]==*(ipv4Adrs+2) && node->octet[3]==*(ipv4Adrs+3))
        {
            unique=false;
        }
        node = node->next;
    }
}

void updateAdrs(int ipv4Adrs[4], char aliasname[])
{
    struct address_t *node = head;
    while(node != NULL)
    {
        if(strcmp(toLowerStr(node->alias),toLowerStr(aliasname))==0)
        {
           node->octet[0] = ipv4Adrs[0], node->octet[1] = ipv4Adrs[1], node->octet[2] = ipv4Adrs[2], node->octet[3] = ipv4Adrs[3];
        }
        node = node->next;
    }
}

int UpdateAddress()
{
    int ipv4Adrs[5];
    char aliasname[20]= "" ;
    char newAdrs[30];

    bool check =  false;

    printf("Enter the Alias Name: ");
    do
    {
        fflush(stdin);
        fgets(aliasname, 20, stdin);
        aliasname[strlen(aliasname)-1] = '\0';

        if(validateAlias(aliasname)){                                                       
            struct address_t *node = head;
            while(node != NULL)
            {
                if(strcmp(toLowerStr(node->alias),toLowerStr(aliasname))==0)
                {
                    printf("Alias Name is Found.\n");
                    printf("%d.%d.%d.%d %s\n", node->octet[0], node->octet[1],
                         node->octet[2], node->octet[3], node->alias);
                    check=true;

                    printf("Enter New Address : ");
                    do{
                        fflush(stdin);
                        fgets(newAdrs, 25, stdin);
                        sscanf(newAdrs, "%d.%d.%d.%d", ipv4Adrs, ipv4Adrs+1, ipv4Adrs+2, ipv4Adrs+3);

                        if(validateAddress(ipv4Adrs))
                        {                                     
                            if(checkUniqueAdrs(ipv4Adrs))                                
                                updateAdrs(ipv4Adrs, aliasname);                            
                            
                            else
                                printf("This Address already exists!\n");

                        }else{
                              printf("Please enter valid IPv4 address : ");
                        }
                    }while(!validateAddress(ipv4Adrs));
                }
                node = node->next;
            }
            if(!check)
                printf("Alias Name Not Found.\n");
        }
        else
            printf("Please Enter A Valid Alias Name: ");

    } while (!validateAlias(aliasname));
    return 0;
}

int deleteAddress()
{
    int ipv4Adrs[4];
    char aliasname[20] = "";
    char newAdrs[25];
    char ch;
    bool check = false;

    do{
        fflush(stdin);
        fgets(aliasname, 20, stdin);
        aliasname[strlen(aliasname) -1]= '\0';

        if(validateAlias(aliasname))
        {
            struct address_t *node = head;
            while(node != NULL)
            {
                if(strcmp(toLowerStr(node -> alias), toLowerStr(aliasname)) == 0)
                {
                    printf("Alias Name Exists  %d.%d.%d.%d %s\n", node->octet[0], node->octet[1], node->octet[2], node->octet[3], node->alias);
                    check = true;

                    printf("Delete the Record (Y/N): \n");
                    do{
                        fflush(stdin);
                        scanf("%c", &ch);
                        if(ch=='Y' || ch=='y'){
                            deleteRecord(aliasname);                                        
                            break;
                        }
                        else if(ch=='N' || ch=='n'){
                            printf("Record did not get Deleted.\n");
                            break;
                        }
                        else{
                            printf("Invalid Input, Delete the Record (Y/N): \n");
                        }

                    }while(ch!='Y' || ch!='y' || ch!='N' || ch!='n');
                }
                node = node->next;
            }
            if(!check)
            {
                printf("Alias Name not Found\n");
                PrintList();
            }
            
        }
        else
            printf("Enter a Valid Alias Name: ");

    }while(!validateAlias(aliasname));
    return 0;
}

int PrintList()
{
    struct address_t *node = head;
    int cnt= 0 ;

    printf("All Record: \n");
    while( node != NULL)
    {
        printf("%d.%d.%d.%d %s\n", node->octet[0], node->octet[1], 
            node->octet[2], node->octet[3], node->alias);
        cnt++;
        node = node->next;
    }
    if(cnt == 0)
        printf("NO RECORD PRESENT \n\n");

    else
        printf("%d Record(s) Available. \n\n", cnt);
    
    return 0;
}

void deleteRecord(char aliasname[20])
{
    struct address_t *node = head;
    struct address_t *prev = NULL;

    if(node==NULL)
    {
        return;
    }

    if (node != NULL && strcmp(toLowerStr(node->alias),toLowerStr(aliasname))==0) {
        head = node->next;
        free(node);
        return;
    }

    while (node != NULL && strcmp(toLowerStr(node->alias),toLowerStr(aliasname)) != 0)
    {
            prev = node;
            node = node->next;
    }
    prev->next = node->next;
    free(node);

}
