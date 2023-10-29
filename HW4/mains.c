#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

typedef struct address_t{
 int octet[4];
 char alias[11];
 struct address_t *leftChild, *rightChild;

}list;

//using boolean datatype to check duplicate entry
bool checkal, del, upd, dup;

//function declaration
void checkDuplicate(list *head,int address[4] ,char str[11]);
void freeTree(list* node);
list* insertintoTree(list* node, char str[11], int addr[4]);
list* updateData(list* node,char updatename[11]);
list* minValueNode(list* node);
list* deleteAlias(list *node, char deletename[11]);
void savetofile_recursive(FILE *fp, list* temp1);
void savetofile(list *node);
void inOrder(list* node);
void lookupAlias(list *node,int ipnum1, int ipnum2);
list *lookupdata(list *node, char checkname[11]);
void exitFromProg(list *root);




//checking for duplicate entry for any IPv4 or alias name
void checkDuplicate(list *node,int address[4] ,char str[11])
{
  list *temp = node;
  if(temp != NULL){
    if(temp->octet[0] == address[0] && temp->octet[1] == address[1] && temp->octet[2] == address[2] & temp->octet[3] == address[3])
    {
      dup = true;
      return;
    }
    if(strcmp(temp->alias,str) == 0){
      dup = true;
      return;
    }
    checkDuplicate(node->leftChild,address,str);
    checkDuplicate(node->rightChild,address,str);
  }

  return;
}


//checking for duplicate entry for any IPv4 or alias name its different than above funtion
//as it will return 1 when alias is not available in the list
void checkIPDuplicate(list *node,int change[4])
{
  list *temp = node;
  
  if(temp != NULL)
  {
    if(temp->octet[0] == change[0] && temp->octet[1] == change[1] && temp->octet[2] == change[2] & temp->octet[3] == change[3]){
      dup = true;
      return;
    }
    
    checkIPDuplicate(node->leftChild,change);
    checkIPDuplicate(node->rightChild,change);
  }
  return;
}


//function to insert data into the tree
list* insertintoTree(list* node, char str[11], int addr[4])
{
  list* root = node;
  if (node == NULL) {
    node = (struct address_t*) malloc (sizeof(struct address_t));
    strcpy(node->alias,str);
    node->octet[0] = addr[0];
    node->octet[1] = addr[1];
    node->octet[2] = addr[2];
    node->octet[3] = addr[3];
    node->leftChild = node->rightChild = NULL;
    root = node;
  }

  else // insert to the left or right
  {
    if (strcmp(node->alias,str) > 0) 
      node->leftChild = insertintoTree(node->leftChild, str, addr);
    else 
      node->rightChild = insertintoTree(node->rightChild, str, addr);
  }
  return root;
}

////updating list funtion
list* updateData(list* node,char updatename[11]){
 
  list *temp = node;
  int change[4];
  char newalias[11];

  if(temp != NULL){
    if (strcmp(temp->alias,updatename) == 0) { 
        upd = true;
        printf("---------------------------------------------------------");
        printf("\nOld IP: %d.%d.%d.%d\n",temp->octet[0], temp->octet[1], temp->octet[2], temp->octet[3]);
        printf("---------------------------------------------------------\n");
        printf("Enter the new IPv4 Address: \n");
        fscanf(stdin, "%d.%d.%d.%d", &change[0], &change[1], &change[2], &change[3]);
        dup = false;

        if(change[0] < 0 || change[0] > 255 || change[1] < 0 || change[1] > 255 || change[2] < 0 || change[2] > 255 || change[3] < 0 || change[3] > 255){
          printf("You have entered wrong octet values. The value must be between 0 - 255 \n");
          return node;
        }

        checkIPDuplicate(node, change);
  
        if(dup == true){
          printf("Duplicate IP. Please Try Again\n");
        }
        else
        {
        // Replace Alias and IP 
        printf("\n-----------------------\n");
        printf("New address updated.");
        printf("\n-----------------------\n");
        temp->octet[0] = change[0];
        temp->octet[1] = change[1];
        temp->octet[2] = change[2];
        temp->octet[3] = change[3];
        }
        }
        else{
          updateData(temp->leftChild, updatename);
          updateData(temp->rightChild, updatename);
    }
  }

  return node;
} 

/* Given a non-empty binary search tree, return the node with minimum key value found in that tree. Note that the entire tree does not need to be searched. */   
list* minValueNode(list* node)
{
    list* current = node;
    /* loop down to find the leftmost leaf */
    while (current && current->leftChild != NULL)
        current = current->leftChild;
    return current;
}

//delete
list* deleteAlias(list *node, char deletename[11])
{
  list *temp;
  char ch[4];
    char match[] = "y";
    char match2[] = "Y";
    // base case
    if (node == NULL)
        return node;
  
    // If the alias to be deleted is smaller than the root's key, then it lies in left subtree
    else if (strcmp(deletename, (node->alias)) < 0)
    {
        node->leftChild = deleteAlias(node->leftChild, deletename);
    }
  
    // // If the alias to be deleted is greater than the root's key, then it lies in right subtree
    else if (strcmp(deletename, (node->alias)) > 0)
    {
        node->rightChild = deleteAlias(node->rightChild, deletename);
    }
  
    // if key is same as root's key,  then This is the node to be deleted
    else
      {
        if(node->rightChild && node->leftChild)
          {
            temp = minValueNode(node->rightChild);
            strcpy(node->alias,temp->alias); 
            /* As we replaced it with some other node, we have to delete that node */
            node->rightChild = deleteAlias(node->rightChild,temp->alias);
          }
        else
        {
          /* If there is only one or zero children then we can directly remove it from the tree and connect its parent to its child */                
          printf("\nData: %s %d.%d.%d.%d\n",node->alias, node->octet[0], node->octet[1], node->octet[2], node->octet[3]); 
          printf("Enter your choice:(y/n)) \n");
          scanf("%s",ch);
          if(strcmp(ch,match) == 0 || strcmp(ch,match2) == 0)
          {
            temp = node;
            if(node->leftChild == NULL)
              node = node->rightChild;
            else if(node->rightChild == NULL)
              node = node->leftChild;
             
              printf("------------------------------------------------------------------------\n");
              printf("The Ipv4 address along with the Alias name has been deleted from the file.\n");
              printf("------------------------------------------------------------------------\n");
              del = true;
              free(temp);

          }
            else
            {
              del = true;
              printf("-------------------------\n");
              printf("You did not Delete any Entry.\n");
              printf("-------------------------\n");
            }
        }
      }
    return node;
}   
//recursive call to save the file
void savetofile_recursive(FILE *fp, list* temp1)
{
    if(temp1!=NULL)
    {
      savetofile_recursive(fp,temp1->leftChild);
      fprintf(fp, "\n%s %d.%d.%d.%d", temp1->alias, temp1->octet[0], temp1->octet[1], temp1->octet[2], temp1->octet[3]);
      savetofile_recursive(fp,temp1->rightChild);
    }
}
// this is for saving int text file
void savetofile(list *node)
{
    FILE *fp = fopen ("CS531_Inet.txt", "w");

    list *temp = node;
    if(fp==NULL)
    {
      printf("Error\n");
      return;
    }
    else
    {
      savetofile_recursive(fp, temp);
    }
    fclose(fp);
}

//display data in alphabate order (Inorder Traversal)
void inOrder(list* node)
{
  //base case
  if (node == NULL)
    return;

  /* Recur on left subtree */
  inOrder(node->leftChild);
  printf("%s ",node->alias );
  printf("%d.%d.%d.%d\n",node->octet[0],node->octet[1],node->octet[2],node->octet[3] );
  /* Recur on right subtree */
  inOrder(node->rightChild);
}

//looking for any IPv4's alias

void lookupAlias(list *node,int ipnum1, int ipnum2){
  list *temp = node;
  
  //base case
  if(temp != NULL){
    if(temp->octet[0] == ipnum1 && temp->octet[1] == ipnum2 ){
      checkal = true;
      printf("---------------------------\n");
      printf("Alias: %s\n", temp->alias);
    }
    lookupAlias(temp->leftChild, ipnum1, ipnum2);
    lookupAlias(temp->rightChild, ipnum1, ipnum2);
  }

}
//looking for any alias

list *lookupdata(list *node, char checkname[11])
{
  if(node != NULL){
      if(strcmp(node->alias,checkname) == 0){
          return node;
      } 
      else {
          list* foundNode = lookupdata(node->leftChild, checkname);
          if(foundNode == NULL) {
              foundNode = lookupdata(node->rightChild, checkname);
          }
          return foundNode;
        }
  } 
  else 
  {
      return NULL;
  }
}

//exit
void exitFromProg(list *root){
      printf("\n-----------\n");
      printf("Exiting from the program");
      printf("\n-----------\n");
      freeTree(root);
      exit(0);
}

//at last free the heap
void freeTree(list* node) {

    if (node == NULL) return; 
    freeTree(node->leftChild); 
    freeTree(node->rightChild); 
    free(node); 
}

//main funtion
int main()
{
  list address_t;
  list *node = NULL;

  FILE* fp = fopen("CS531_Inet.txt", "r");
  if (fp == NULL) 
  {
      printf("File not found!");
      exit(0);
  }

    while(fscanf(fp,"%s %d.%d.%d.%d", address_t.alias, &address_t.octet[0],&address_t.octet[1], &address_t.octet[2], &address_t.octet[3]) > 0) 
    {
        int ip[] = { 
          address_t.octet[0], 
          address_t.octet[1], 
          address_t.octet[2], 
          address_t.octet[3]
        };  
        node = insertintoTree(node,address_t.alias,ip);
    }
    fclose(fp);

  while(1)
  {

    char updatename[11];
    char deletename[11];
    int ipnum1,ipnum2;
    int ch;
    char checkname[15];

    //Menu to display
    printf("\nMenu\n");

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
    scanf("%d",&ch);

    switch(ch)
    {
      case 1:
          printf("Enter a Valid IPv4 Address : ");
          fscanf(stdin, "%d.%d.%d.%d", &address_t.octet[0],&address_t.octet[1], &address_t.octet[2], &address_t.octet[3]); 
          printf("Enter the ALIAS name: ");
          fscanf(stdin, "%s",address_t.alias); 
          dup = false;

          if(address_t.octet[0] > 0 && address_t.octet[0] < 255 && address_t.octet[1] > 0 && address_t.octet[1] < 255 && address_t.octet[2] > 0 && address_t.octet[2] < 255 && address_t.octet[3] > 0 && address_t.octet[3] < 255)
          {
            int ip[4] = 
            { 
              address_t.octet[0], 
              address_t.octet[1], 
              address_t.octet[2], 
              address_t.octet[3]
            };

            checkDuplicate(node,ip,address_t.alias);

            if(dup == true){
              printf("---------------------------------------\n");
              printf("This is a Duplicate entry. Please Try Again\n");
              printf("---------------------------------------\n");
            }
            else {           
            node = insertintoTree(node,address_t.alias,ip);
            printf("\n--------------\n");
            printf("List Updated");
            printf("\n---------------\n");
            }
          }
          else
          {
            printf("\n----------------------------------------------\n");
            printf("The IPV4 Address is Incorrect. Outer ranger Ip-Address");
            printf("\n----------------------------------------------\n");
          }
        break;
      case 2:
          printf("\n------------------------------\n");
          printf("The Ip AddresscLookup - \n");
          printf("Please enter the Alias name: \n");
          scanf("%s",checkname);

          printf("\n------------------------------\n");
          list *temp = lookupdata(node,checkname);
          if(temp){
            printf("One Entry is Available\n");
            printf("%s: %d.%d.%d.%d",temp->alias, temp->octet[0], temp->octet[1], temp->octet[2], temp->octet[3] );
            printf("\n------------------------------\n");
          }
          else{
            printf("\n------------------------------\n");
            printf("No Alias Name found in the file.\n");
            printf("\n------------------------------\n");
          }
        break;

      case 3:
        
          printf("\n----------\n");
          printf("Update The Ipv4 Address - \n");
          printf("Enter a Valid Alias Name in the File: \n");
          scanf("%s",updatename);
          upd = false;
          node = updateData(node,updatename);
          if(upd == false){
            printf("\n----------\n");
            printf("Alias: %s is not available in the file",updatename);
            printf("\n----------\n");
          }

        break;

      case 4:
        printf("\n----------\n");
          printf("Delete Address - \n");
          printf("Enter Alias name of the IPV4 Address to be Deleted: \n");
          scanf("%s",deletename);
          del = false;
          node = deleteAlias(node,deletename);
          if(del == false){
            printf("\n----------\n");
            printf("This %s Alias name is not available in the list",deletename);
          }
          printf("\n----------\n");
        break;
      
      case 5:
        printf("\n----------\n");
        printf("IPV4 Address with Alias Name Displayed Below:");
        printf("\n-----------------\n");
          inOrder(node);
          printf("\n-----------------\n");
        break;
      
      case 6:
          printf("\n----------\n");
          printf("Alias Lookup - \n");

          printf("Please enter the IPV4 Address (i.e. two values between 0 - 255)\n");
          scanf("%d.%d",&ipnum1,&ipnum2);
          if(ipnum1 > 0 && ipnum1 < 255 && ipnum2 > 0 && ipnum2 < 255){
            checkal = false;
            printf("Location: %d.%d\n", ipnum1, ipnum2);
            lookupAlias(node,ipnum1,ipnum2);

            if(checkal == false){
              printf("\n----------\n");
              printf("No IPV4 Address Available. Please Try Again...\n");
              printf("\n----------\n");
            }
          }
          else{
            printf("--- -----------------------------------------------------\n");
            printf("The IPV4 Address you entered is out of the range. Please Try Again...\n");
            printf("--- -----------------------------------------------------\n");
          }
          
        break;
      
      case 7:
        printf("\n-----------\n");
        printf("Save to File - \n");
        savetofile(node);
        printf("\n-----------\n");
        break;

      case 8:
        printf("QUIT. THANK YOU\n");
        exitFromProg(node);
        break;

      default: 
          printf("\n-------------------------------\n");
          printf("Invalid input: Try AGAIN (1-8) \n");
          printf("\n-------------------------------\n");
          break;
    }
  }
return 0;
}
