#include <stdio.h>		                                
#include <string.h>		                                
#include <stdlib.h>                                     
#include <stdbool.h>		                            
#include <ctype.h>

struct address_t* listCreate(char inputStr[35]);     
struct address_t* initialListCreate(char inputStr[35]); 

// all fuction names
int listPrint();      //                                  
void initializeListFromFile();                       
int check_For_Choice(int choice);      //                    
int addAddress();                              //         
bool validateAddress(int ipv4Adrs[]);        //           
bool validateAlias(char aliasName[20]);    //             
bool checkUnique(int ipv4Adrs[], char aliasName[]);    // 
int lookUpAddress();                            //        
int updateAddress();                              //      
bool checkUniqueAddr(int ipv4Adrs[]);             //      
void updateAddr(char aliasName[],int ipv4Adrs[]);   //    
int deleteAddress();//
void deleteRecord(char aliasName[]);          //          
int displayAliasForLocation();             //             
bool validateAddressLocation(int ipv4Adrs[]);        //   
void displayAlias(int ipv4Adrs[]);               //       
int saveToFile();                        //               
char* toLowerStr(char str[]);                 //

// structure definition as given in the question
struct address_t                                        

{
    int octet[4];                                       
    char alias[11];                                     
    struct address_t *next;                             
};

struct address_t *head = NULL;                           

int main()
{                                             
    char choice;                                           
    int optTemp;
    initializeListFromFile();                           
    do{

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

        optTemp=choice-'0';

        if(optTemp >= 1 && optTemp <= 7)
            optTemp=check_For_Choice(optTemp);

        else if(optTemp==8)
        {
            printf("QUIT. THANK YOU\n");
            break;
        }

        else
            printf("Invalid input: Try AGAIN (1-8) \n");

    }
    while(!(optTemp >= 1 && optTemp <= 8));
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
            updateAddress();
            break;

        case 4:
            printf("4: Delete address\n");
            deleteAddress();
            break;

        case 5:
            printf("5: Display List\n");
            listPrint();
            break;

        case 6:
            printf("6: Display aliases for location\n");
            displayAliasForLocation();
            break;

        case 7:
            printf("7: Save to File\n");
            saveToFile();
            break;
    }
    return 0;
}

int saveToFile(){                                                          

    FILE *file;
    int x;
    char newfile[16];
    struct address_t *node = head;
    printf("Enter file name: \n");

    fflush(stdin);
    fgets(newfile,16,stdin);
    newfile[strlen(newfile)-1]='\0';

    file =fopen(newfile, "w");                                               

    if (!file){
        printf("Un-expected error while creating file.\n");                                 
        return 0;
    }

    while(node != NULL)
    {
        fprintf(file,"%d.%d.%d.%d %s\n", node->octet[0], node->octet[1], 
            node->octet[2], node->octet[3], toLowerStr(node->alias));
        node = node->next;
    }

    fclose(file);
    printf("Data saved to file.\n");
    return 0;

}

void displayAlias(int ipv4Adrs[])
{
    int count = 0;
    struct address_t *node = head;
    while(node != NULL)
    {
        if(node->octet[0]==*(ipv4Adrs) && node->octet[1]==*(ipv4Adrs+1))
        {
            printf("%s\n",node->alias);
            count++;
        }
        node = node->next;
    }
    if(count < 1){
        printf("The location does not exist within the list\n\n");
    }
    return;

}



int displayAliasForLocation(){                                                              
    char ipStr[25];
    char str[35]="";
    int ipv4Adrs[2];
    char aliasName[20];
    printf("Enter address location : ");

    do
    {
        fflush(stdin);
        fgets(ipStr, 25, stdin);
        sscanf(ipStr, "%d.%d", ipv4Adrs, ipv4Adrs+1);

        if(validateAddressLocation(ipv4Adrs)){                                              
            displayAlias(ipv4Adrs);                                                         
        }

        else{
            printf("Please enter valid IPv4 address : ");
        }

    }while(!validateAddress(ipv4Adrs));                                                     
    return 0;
}

void deleteRecord(char aliasName[20])
{
    struct address_t *node = head;
    struct address_t *prev = NULL;

    if(node==NULL)
    {
        return;
    }

    if (node != NULL && strcmp(toLowerStr(node->alias),toLowerStr(aliasName))==0) {
        head = node->next;
        free(node);
        return;
    }

    while (node != NULL && strcmp(toLowerStr(node->alias),toLowerStr(aliasName))!=0)
    {
            prev = node;
            node = node->next;
    }
    prev->next = node->next;
    free(node);

}



int deleteAddress()
{
    int ipv4Adrss[4];
    bool found=false;
    char aliasName[20]="";
    char newAdrStr[25];
    char sure;
    do
    {
        fflush(stdin);
        fgets(aliasName, 20, stdin);
        aliasName[strlen(aliasName)-1]='\0';

        if(validateAlias(aliasName))
        {
            struct address_t *node = head;
            while(node != NULL)
            {
                if(strcmp(toLowerStr(node->alias),toLowerStr(aliasName))==0)
                {
                   printf("Alias found!! %d.%d.%d.%d %s\n", node->octet[0], node->octet[1], node->octet[2], node->octet[3], node->alias);
                    found=true;

                    printf("Are you sure to delete this record!!(Y/N)\n");
                    do{
                        fflush(stdin);
                        scanf("%c", &sure);
                        if(sure=='Y' || sure=='y'){
                            deleteRecord(aliasName);                                        
                            break;
                        }
                        else if(sure=='N' || sure=='n'){
                            printf("you have selected to not delete this record this time.\n");
                            break;
                        }

                        else{
                            printf("invalid input, Are you sure to delete this record!!(Y/N)\n");
                        }

                    }while(sure!='Y' || sure!='y' || sure!='N' || sure!='n');
                }
                node = node->next;
            }

            if(!found){
                printf("Alias not found.\n");
                listPrint();                                                               
            }
        }
        else{

            printf("Please enter valid alias : ");

        }

    }while(!validateAlias(aliasName));

    return 0;

}

void updateAddr(char aliasName[20], int ipv4Adrs[4])        //update address
{                                       
    struct address_t *node = head;
    while(node != NULL)
    {
        if(strcmp(toLowerStr(node->alias),toLowerStr(aliasName))==0)
        {
            node->octet[0]=*(ipv4Adrs);
            node->octet[1]=*(ipv4Adrs+1);
            node->octet[2]=*(ipv4Adrs+2);
            node->octet[3]=*(ipv4Adrs+3);
        }
        node = node->next;
    }

}



bool checkUniqueAddr(int ipv4Adrs[4]){                                                      

    bool unique = true;

    struct address_t *node = head;

    

    printf("%d",ipv4Adrs[0]);

    printf(".");

    printf("%d",ipv4Adrs[1]);

    printf(".");

    printf("%d",ipv4Adrs[2]);

    printf(".");

    printf("%d",ipv4Adrs[3]);

    

    while(node != NULL){

        if(node->octet[0]==*(ipv4Adrs) && node->octet[1]==*(ipv4Adrs+1) && node->octet[2]==*(ipv4Adrs+2) && node->octet[3]==*(ipv4Adrs+3)){

            unique=false;

        }

        node = node->next;

    }

    return unique;

}



int updateAddress(){                                                                        
    int ipv4Adrss[4];
    bool found=false;
    char aliasName[20]="";
    char newAdrStr[25];

    printf("Enter Alias Name : ");

    do{
        fflush(stdin);
        fgets(aliasName, 20, stdin);
        aliasName[strlen(aliasName)-1]='\0';

        if(validateAlias(aliasName)){                                                       
            struct address_t *node = head;
            while(node != NULL)
            {
                if(strcmp(toLowerStr(node->alias),toLowerStr(aliasName))==0)
                {
                    printf("Alias found!!\n");
                    printf("%d.%d.%d.%d %s\n", node->octet[0], node->octet[1],
                         node->octet[2], node->octet[3], node->alias);
                    found=true;

                    printf("Enter New Address : ");
                    do{
                        fflush(stdin);
                        fgets(newAdrStr, 25, stdin);
                        sscanf(newAdrStr, "%d.%d.%d.%d", ipv4Adrss, ipv4Adrss+1, ipv4Adrss+2, ipv4Adrss+3);

                        if(validateAddress(ipv4Adrss))
                        {                                     
                            if(checkUniqueAddr(ipv4Adrss))
                            {                                 
                                updateAddr(aliasName,ipv4Adrss);                            
                            }

                            else{
                                printf("This Address already exists!\n");
                            }

                        }else{

                              printf("Please enter valid IPv4 address : ");

                        }
                    }while(!validateAddress(ipv4Adrss));
                }
                node = node->next;

            }

            if(!found){

                printf("Alias not found.\n");

            }
        }
        else
        {
            printf("Please enter valid alias : ");
        }

    }while(!validateAlias(aliasName));

    return 0;

}

char* toLowerStr(char str[])
{                                                               
    int i=0;
    char *newStr = (char*)malloc(strlen(str));

    while(str[i]){

        newStr[i]=tolower(str[i]);
        i++;
    }
    return newStr;

}



int lookUpAddress(){                                                                        

    bool found=false;

    char aliasName[20]="";

    printf("Please enter Alias : ");

    do{

        fflush(stdin);

        fgets(aliasName, 20, stdin);

        aliasName[strlen(aliasName)-1]='\0';

        if(validateAlias(aliasName)){                                                       

            struct address_t *node = head;

            while(node != NULL){

                if(strcmp(toLowerStr(node->alias),toLowerStr(aliasName))==0)
                {
                    printf("Alias found!! %d.%d.%d.%d %s\n", node->octet[0], node->octet[1], node->octet[2], node->octet[3], node->alias);
                    found=true;

                }
                node = node->next;
            }
            if(!found)
            {
                printf("Alias not found.\n");

            }
        }
        else
        {
            printf("Please enter valid alias : ");
        }
    }while(!validateAlias(aliasName));
    return 0;

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

        if(validateAddress(ipv4Adrs)){                                                      
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
                        initialListCreate(ipStr);
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

bool checkUnique(int ipv4Adrs[], char aliasName[])
{                                         
    bool unique = true;
    struct address_t *node = head;

    while(node != NULL){
        if(node->octet[0]==*(ipv4Adrs) && node->octet[1]==*(ipv4Adrs+1)
            && node->octet[2]==*(ipv4Adrs+2) && node->octet[3]==*(ipv4Adrs+3))
        {
            printf("\nThis IPv4 Address ");
            unique=false;
            break;
        }

        else if(strcmp(toLowerStr(node->alias),toLowerStr(aliasName))==0){
            printf("\nThis /%s/ Alias", aliasName);
            unique=false;
            break;
        }
        node = node->next;
    }
    return unique;
}

bool validateAlias(char aliasName[20])
{                                                     
    bool valid = true;
    if(strlen(aliasName)>10 || strlen(aliasName)<=0 || strchr(aliasName,' ') || strchr(aliasName,'\t'))
    {
        valid= false;
    }
    return valid;
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

bool validateAddressLocation(int ipv4Adrs[])
{
    bool valid_Address_location = true;

    for(int i=0; i<2; i++){

        if(ipv4Adrs[i] <= 255 && ipv4Adrs[i] >= 0){
            valid_Address_location = true;
        }
        else{
            valid_Address_location = false;
            break;
        }
    }
    return valid_Address_location;
}



void initializeListFromFile(){                                                              

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

            if(NULL != initialListCreate(inputStr)){}
            else{

                printf("Error while creating the node\n");
            }
            i=0;
            memset(inputStr,0,35);
        }
    }while(ch != EOF);
    fclose(filePtr);
}

struct address_t* listCreate(char inputStr[35]){                                            
    struct address_t *curr = NULL;
    struct address_t *node=(struct address_t*)malloc(sizeof(struct address_t));

    if(NULL==node)
    {
        printf("Exception while creating the node\n");
        return NULL;
    }

    sscanf(inputStr, "%d.%d.%d.%d %s", node->octet, node->octet+1, 
        node->octet+2, node->octet+3, node->alias);

    node->next = NULL;
    curr = node;
    head = curr;
    return head;

}



struct address_t* initialListCreate(char inputStr[35]){                                     

    if(NULL == head){

        return (listCreate(inputStr));

    }
    struct address_t *node = (struct address_t*)malloc(sizeof(struct address_t));
    if(NULL == head)
    {
        printf("\nNode creation failed\n");
        return    NULL;
    }
    sscanf(inputStr, "%d.%d.%d.%d %s", node->octet, node->octet+1, 
        node->octet+2, node->octet+3, node->alias);
    node->next = head;
    head = node;
    return head;

}

int listPrint()
{                                                                            
    struct address_t *node = head;
    int count=0;

    printf("\nPrinting all records present::\n");
    while(node != NULL)
    {
        printf("%d.%d.%d.%d %s\n", node->octet[0], node->octet[1], 
            node->octet[2], node->octet[3], node->alias);
        count++;
        node = node->next;

    }
    if(count==0)
        printf("No record present\n\n");

    else
        printf("Displaying %d result(s).\n\n",count);
    return 0;

}