//Abheesh Gupta
//Project created based on Binary Search Tree utilizing various helper functions

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAXLEN 31

typedef struct itemNode //struct for item node provided 
{
    char name[MAXLEN];
    int count;
    struct itemNode *left, *right;
}itemNode;

typedef struct treeNameNode //struct for tree name node provided
{
    char treeName[MAXLEN];
    struct treeNameNode *left, *right;
    itemNode *theTree;
}treeNameNode;

//function prototypes- creates items
void inorder(treeNameNode *temp, FILE *fp);
treeNameNode *createTreeNameNode (char name[MAXLEN]);
treeNameNode *insertNameTree(treeNameNode *nameRoot, treeNameNode *temp_node);
treeNameNode *searchNameNode(treeNameNode *root, char *treeName);
itemNode *createItem(char treeName[MAXLEN], char name[MAXLEN], int count);
itemNode *insertItem(itemNode *root, itemNode *tempItem); 
//function prototypes- prints trees
treeNameNode *buildNameTree(FILE *fp, int n);
void readTrees(treeNameNode *root, int i);
void itemTraversalInOrder(itemNode *root, FILE *fp);
void traverse_in_traverse(treeNameNode *root, FILE *fp);
//function prototypes- queries
itemNode *find (itemNode *search, char element[MAXLEN]);
int count_before (itemNode *root, char element[MAXLEN]);
int height (itemNode *root); //helper function for check_balance
int check_balance (itemNode *root);
int countItems(itemNode *root);
//function prototypes- item deletion
int isLeaf(itemNode *node);
int hasOnlyLeftChild(itemNode *node);
int hasOnlyRightChild(itemNode *node);
itemNode *parent (itemNode *root, itemNode *node);
itemNode *minVal(itemNode *root);
itemNode *deleteItem (itemNode *root, char querieItem[MAXLEN]);
//function prototypes- tree deletion
int isLeafTree(treeNameNode *node);
int hasOnlyLeftChildTree(treeNameNode *node);
int hasOnlyRightChildTree(treeNameNode *node);
treeNameNode *parentTree (treeNameNode *root, treeNameNode *node);
treeNameNode *minValTree(treeNameNode *root);
treeNameNode *deleteTree (treeNameNode *root, char treeName[MAXLEN]);
//function prototypes- free items and treenames
void freeItems(itemNode *root);
void freeTree(treeNameNode *root);


int main ()
{
    atexit(report_mem_leak);
    FILE *fp = fopen("out.txt", "w"); //opens file in writing mode

    treeNameNode *nameRoot = NULL;
    //itemNode *itemRoot = NULL;
    treeNameNode *searchNode = NULL;
    itemNode *createItemNode = NULL; //assigns pointers to NULL;

    int N, I, Q;
    int count;
    char tname[MAXLEN], iname[MAXLEN];
    char querie[MAXLEN], treeName[MAXLEN], querieItem[MAXLEN];
    int decrease;

    
    scanf("%d %d %d", &N, &I, &Q); //input for number of tree names, items, and queries
    nameRoot = buildNameTree(fp, N);
    readTrees(nameRoot, I);
    traverse_in_traverse(nameRoot, fp); //function calls which prints items in tree
    printf("\n");
    fprintf(fp, "\n"); 

    for (int c = 0; c < Q; c++) //loop for queries
    {
        scanf("%s ", querie);
        if (strcmp(querie, "find") == 0) //checks if querie entered is find
        {
            scanf("%s %s", treeName, querieItem);
            treeNameNode *searchItem = searchNameNode(nameRoot, treeName);
            if (searchItem == NULL) //checks if treeName exists
            {
                fprintf(fp, "%s does not exist\n", treeName);
                printf("%s does not exist\n", treeName);
            }
            else
            {
                itemNode *search = find(searchItem->theTree, querieItem); //calls find function 
                if (search == NULL) //determines whether item is in tree
                {
                    fprintf(fp, "%s not found in %s\n", querieItem, treeName);
                    printf("%s not found in %s\n", querieItem, treeName);
                }
                else
                {
                    fprintf(fp, "%d %s found in %s\n", search->count, querieItem, treeName);
                    printf("%d %s found in %s\n", search->count, querieItem, treeName);
                }
            }            
        }
        if (strcmp(querie, "count_before") == 0) //checks whether querie is count_before
        {
            scanf("%s %s ", treeName, querieItem);
            treeNameNode *searchRoot = searchNameNode(nameRoot, treeName);
            if (searchRoot == NULL) //if tree exists
            {
                fprintf(fp, "no items before\n");
                printf("no items before\n");
            }
            else
            {
                int countBefore = count_before(searchRoot->theTree, querieItem); //calls count_before function and assigns to int
                fprintf(fp, "item before %s: %d\n", querieItem, countBefore);
                printf("item before %s: %d\n", querieItem, countBefore);
            }
        }
        if(strcmp(querie, "check_balance") == 0) //checks whether querie is check_balance
        {
            scanf("%s", treeName);
            treeNameNode *searchRoot = searchNameNode(nameRoot, treeName);
            int diff = check_balance(searchRoot->theTree); //calls check balance function and assigns it to int 
            fprintf(fp, "%s: left height %d, right height %d, ", searchRoot->treeName, height(searchRoot->theTree->left), height(searchRoot->theTree->right));
            printf("%s: left height %d, right height %d, ", searchRoot->treeName, height(searchRoot->theTree->left), height(searchRoot->theTree->right));
            if (diff == 0) //determines whether tree is balanced or not 
            {
                fprintf(fp, "difference %d, balanced\n", diff);
                printf("difference %d, balanced\n", diff);
            }
            else
            {
                fprintf(fp, "difference %d, not balanced\n", diff);
                printf("difference %d, not balanced\n", diff);
            }

        }
        if (strcmp(querie, "count") == 0) //checks whether querie is count 
        {
            scanf("%s", treeName);
            treeNameNode *searchRoot = searchNameNode(nameRoot, treeName);
            int totalItemCount = countItems(searchRoot->theTree); //calls countItems and assigns it to int
            fprintf(fp, "%s count %d\n", treeName, totalItemCount);
            printf("%s count %d\n", treeName, totalItemCount);
        }
        if (strcmp(querie, "decrease") == 0) //checks whether querie is decrease
        {
            scanf("%s %s %d", treeName, querieItem, &decrease);
            treeNameNode *searchRoot = searchNameNode(nameRoot, treeName);
            itemNode *searchItem = find(searchRoot->theTree, querieItem); //finds item needed to be decreased
            searchItem->count = searchItem->count - decrease; //count of item updated based on decrease value entered by user
            printf("%s reduced\n", searchItem->name);
            fprintf(fp, "%s reduced\n", searchItem->name);
            if (searchItem->count <= 0) //count less than or equal to 0, removes item from tree
            {
                searchRoot->theTree = deleteItem(searchRoot->theTree, querieItem);
            }
        }
        if (strcmp(querie, "remove") == 0) //checks if querie is remove
        {
            scanf("%s %s", treeName, querieItem);
            treeNameNode *searchRoot = searchNameNode(nameRoot, treeName);
            if (searchRoot == NULL)
            {
                fprintf(fp, "%s does not exist\n", treeName);
                printf("%s does not exist\n", treeName);
            }
            else
            {
                itemNode *removeItem = deleteItem(searchRoot->theTree, querieItem); //calls deleteItem function
                printf("%s deleted from %s\n", querieItem, treeName);
                fprintf(fp, "%s deleted from %s\n", querieItem, treeName);
            }
        }
        if (strcmp(querie, "delete_name") == 0) //checks if querie is delete_name
        {
            scanf("%s", treeName);
            treeNameNode *removeTree = deleteTree(nameRoot, treeName); //calls deleteTree function
            printf("%s deleted\n", treeName);
            fprintf(fp, "%s deleted\n", treeName);
        }

    }

    freeTree(nameRoot);

    fclose(fp);

    return 0;
}

void inorder(treeNameNode *temp, FILE *fp) //function used to print items in order alphabetically
{
    if (temp != NULL)
    {
        inorder(temp->left, fp);
        fprintf(fp, "%s ", temp->treeName);
        printf("%s ", temp->treeName);
        inorder(temp->right, fp);
    }
}

treeNameNode *createTreeNameNode (char name[MAXLEN]) //creates treeNameNode using DMA
{
    treeNameNode *temp = malloc(sizeof(treeNameNode));
    strcpy(temp->treeName, name);
    temp->left = NULL;
    temp->right = NULL;
    temp->theTree = NULL;

    return temp;

}

treeNameNode *insertNameTree(treeNameNode *nameRoot, treeNameNode *temp_node) //functions inserts node into tree alphabetically
{
    if (nameRoot == NULL)
    {
        return temp_node;
    }
    else 
    {
        if (strcmp(nameRoot->treeName, temp_node->treeName) < 0) //uses strcmp to determine where in tree to insert node
        {
            if (nameRoot->right != NULL)
            {
                nameRoot->right = insertNameTree(nameRoot->right, temp_node); 
            }
            else
            {
                nameRoot->right = temp_node;
            }
        }
        else 
        {
            if (nameRoot->left != NULL)
            {
                nameRoot->left = insertNameTree(nameRoot->left, temp_node);
            }
            else
            {
                nameRoot->left = temp_node;
            }
        }
    }
    return nameRoot;

}

treeNameNode *searchNameNode(treeNameNode *root, char *treeName) //search function used to determine where node is in tree
{
    if (root != NULL)
    {
        if (strcmp(root->treeName, treeName) == 0) //reaches here means both names are equal, root returned
        {
            return root;
        }
        else if (strcmp(root->treeName, treeName) > 0)
        {
           return searchNameNode(root->left, treeName); //recursive call for left 
        }
        else
        {
            return searchNameNode(root->right, treeName); //recursive call for right
        }
    }

    return NULL;

}

itemNode *createItem(char treeName[MAXLEN], char name[MAXLEN], int count) //creates item node using DMA
{
    itemNode *tempItem = malloc(sizeof(itemNode));
    strcpy(tempItem->name, name);
    //scanf("%s", tempItem->name);
    tempItem->count = count;
    tempItem->left = NULL;
    tempItem->right = NULL;

    return tempItem;
}


itemNode *insertItem(itemNode *root, itemNode *tempItem) //inserts item node into proper tree and spot alphabetically
{

    if (root == NULL)
    {
        return tempItem;
    }
    
    if (strcmp(root->name, tempItem->name) > 0) //strcmp used to determine where item needs to be placed
    {
        root->left = insertItem(root->left, tempItem);
    }
    else 
    {
        root->right = insertItem(root->right, tempItem);
    }

    return root;

}

treeNameNode *buildNameTree(FILE *fp, int n) //builds the tree name types by calling functions from above
{
    char name[MAXLEN];
    treeNameNode *nameRoot = NULL;
    treeNameNode *temp_node;

    for (int a = 0; a < n; a++)
    {
        scanf("%s", name);
        temp_node = createTreeNameNode(name);
        nameRoot = insertNameTree(nameRoot, temp_node);
    }
    inorder(nameRoot, fp); //used to print the tree in order

    return nameRoot;
}

void readTrees(treeNameNode *root, int i) //used to insert items into appropriate tree
{
    int count;
    char tname[MAXLEN], iname[MAXLEN];
    for (int b = 0; b < i; b++)
    {
        scanf("%s %s %d ", tname, iname, &count);
        itemNode *createItemNode = createItem(tname, iname, count);
        treeNameNode *searchNode = searchNameNode(root, tname);
        searchNode->theTree = insertItem(searchNode->theTree, createItemNode);
    }
}

void itemTraversalInOrder(itemNode *root, FILE *fp) //function used to print items in proper order
{
    if (root != NULL)
    {
        itemTraversalInOrder(root->left, fp);
        fprintf(fp, "%s ", root->name);
        printf("%s ", root->name);
        itemTraversalInOrder(root->right, fp);
    }
}

void traverse_in_traverse(treeNameNode *root, FILE *fp) //function used to print treeNames in order as well as their respective items in order
{
    if (root != NULL)
    {
        traverse_in_traverse(root->left, fp);
        fprintf(fp, "\n###%s###\n", root->treeName);
        printf("\n###%s###\n", root->treeName);
        itemTraversalInOrder(root->theTree, fp);
        traverse_in_traverse(root->right, fp);
    }
}

itemNode *find (itemNode *search, char element[MAXLEN]) //function used to search if item is in trees
{
    if (search != NULL)
    {
        if (strcmp(search->name, element) == 0)
        {
            return search;
        }
        else if(strcmp(search->name, element) > 0)
        {
            return find(search->left, element);
        }
        else
        {
            return find(search->right, element);
        }
    }

    return NULL; //returned if search is not an actual treeName
}

int count_before (itemNode *root, char element[MAXLEN]) //function used to determine total number of items before current item
{
    //int count = 0;

    if(root == NULL)
        return 0;

    if (strcmp(root->name, element) < 0)
    {
        return 1 + count_before(root->left, element) + count_before(root->right, element); //adds an additional one if name of root is less than current element
    }
    else
    {
        return count_before(root->left, element) + count_before(root->right, element);
    }

}

int height (itemNode *root) //helper function used in check_balance by calculating height of subtrees
{
    int leftHeight, rightHeight;

    if (root == NULL) //if no items in tree
        return -1;
    else
    {
        leftHeight = height(root->left); //recursive call to determine height of left
        rightHeight = height(root->right); //recursive call to determine height of right

        if(leftHeight > rightHeight)
            return leftHeight+ 1;

        return rightHeight+ 1;

    }

}

int check_balance (itemNode *root)
{
    
    int left = height(root->left);
    int right = height(root->right); //assigns left and right heights

    int difference = left-right;
    difference = abs(difference); //difference calculated and absolute valued so positive number

    return difference;

}

int countItems(itemNode *root) //function used to count total count of each item
{
    if (root == NULL)
        return 0;
    else
        return root->count + countItems(root->left) + countItems(root->right); //recursive call to calculate total 

}

int isLeaf(itemNode *node) //helper function for itemDeletion used to determine if item being deleted is a leaf node
{
    return (node->left == NULL && node->right == NULL);
}

int hasOnlyLeftChild(itemNode *node) //helper function for itemDeletion used to determine if item being deleted has one left child
{
    return (node->left != NULL && node->right == NULL);
}

int hasOnlyRightChild(itemNode *node) //helper function for itemDeletion used to determine if item being deleted has one right child
{
    return(node->left == NULL && node->right != NULL);
}

itemNode *parent (itemNode *root, itemNode *node) //helper function for itemDeletion used to determine what is the parent of item being deleted
{

    if (strcmp(root->name, node->name) == 0)
    {
        return NULL;
    }
    
    if(root->right == node || root->left == node) //if left or right of root is equal to node, root returned
    {
        return root;
    }

    if (strcmp(root->name, node->name) > 0)
    {
        return parent(root->left, node);
    }
    else if (strcmp(root->name, node->name) < 0)
    {
        return parent(root->right, node);
    }

    return NULL; //returns NULL if parent does not exists
}

itemNode *minVal(itemNode *root) //helper function used in itemDeletion if item being deleted has two children
{
    if (root->left == NULL)
        return root;
    else
        return minVal(root->left);

}


itemNode *deleteItem (itemNode *root, char querieItem[MAXLEN]) //deletes item from tree
{
    //helper pointers through the deletion process
    itemNode *delNode = find(root, querieItem); 
    itemNode *saveNode, *newDelNode;
    char saveQuerie[MAXLEN];
    itemNode *par = parent(root, delNode);
    int delCount;
    
    if (isLeaf(delNode)) //if leaf node deletion process: frees item, assigns parent left/right to NULL so no longer points to item 
    {
        if (par == NULL)
        {
            free(root);
            return NULL;
        }
        else if (strcmp(root->name, querieItem) > 0)
        {
            free(par->left);
            par->left = NULL;
        }
        else
        {
            free(par->right);
            par->right = NULL;
        }
        return root;
    }
    if (hasOnlyLeftChild(delNode))//hasOnlyLeftChild deletion process
    {
        if(par == NULL)
        {
            saveNode = delNode->left;
            free(delNode);
            return saveNode;
        }
        //deletes node if its left child 
        if (strcmp(root->name, querieItem) > 0)
        {
            saveNode = par->left;
            par->left = par->left->left;
            free(saveNode);
        }
        else //deletes node if its right child
        { 
            saveNode = par->right;
            par->right = par->right->left;
            free(saveNode);
        }
        return root;
    }
    if (hasOnlyRightChild(delNode)) //hasOnlyRightChild deletion process
    {
        if (par == NULL)
        {
            saveNode = delNode->right;
            free(delNode);
            return saveNode;
        }
        //deletes node if it is left child
        if (strcmp(root->name, querieItem) > 0)
        {
            saveNode = par->left;
            par->left = par->left->right;
            free(saveNode);
        }
        else //deletes node if it is right child
        {
            saveNode = par->right;
            par->right = par->right->right;
            free(saveNode);
        }
        return root;
    }
    //gets to this point if item has two children
    newDelNode = minVal(delNode->right); //finds new node to delete
    strcpy(saveQuerie, newDelNode->name);
    delCount = newDelNode->count;
    deleteItem(root, saveQuerie); //deletes proper value 
    strcpy(delNode->name, saveQuerie);
    root->count = root->count - delNode->count; //updates count after deletion
    
    return root;

}

//functions below are helper functions used in treeDeletion process (similar to ones used in itemDeletion)

int isLeafTree(treeNameNode *node) //returns 1 if node is leaf

{
    return (node->left == NULL && node->right == NULL);
}

int hasOnlyLeftChildTree(treeNameNode *node) //returns 1 if node has one left child
{
    return (node->left != NULL && node->right == NULL);
}

int hasOnlyRightChildTree(treeNameNode *node) //returns 1 if node has one right child
{
    return(node->left == NULL && node->right != NULL);
}

treeNameNode *parentTree (treeNameNode *root, treeNameNode *node) //returns parent of the node pointed to by the node in the tree
{

    if (strcmp(root->treeName, node->treeName) == 0)
    {
        return NULL;
    }
    
    if(root->right == node || root->left == node)
    {
        return root;
    }

    if (strcmp(root->treeName, node->treeName) > 0)
    {
        return parentTree(root->left, node);
    }
    else if (strcmp(root->treeName, node->treeName) < 0)
    {
        return parentTree(root->right, node);
    }

    return NULL;
}

treeNameNode *minValTree(treeNameNode *root) //returns min value in tree with root
{
    if (root->left == NULL)
        return root;
    else
        return minValTree(root->left);

}

treeNameNode *deleteTree (treeNameNode *root, char treeName[MAXLEN])
{
    //helper pointers through the deletion process
    treeNameNode *delNode = searchNameNode(root, treeName);
    freeItems(delNode->theTree);
    treeNameNode *saveNode, *newDelNode;
    char saveTreeName[MAXLEN];
    treeNameNode *par = parentTree(root, delNode);
    
    if (isLeafTree(delNode)) //case for leaf node 
    {
        if (par == NULL) //deletes only node in tree
        {
            free(root);
            return NULL;
        }
        else if (strcmp(root->treeName, treeName) > 0) //deletes node if left child 
        {
            free(par->left);
            par->left = NULL;
        }
        else //deletes node if right child
        {
            free(par->right);
            par->right = NULL;
        }
        return root;
    }
    if (hasOnlyLeftChildTree(delNode)) //case for node with only left child
    {
        if(par == NULL) //deletes root node
        {
            saveNode = delNode->left;
            free(delNode);
            return saveNode;
        }
        //deletes node if left child
        if (strcmp(root->treeName, treeName) > 0)
        {
            saveNode = par->left;
            par->left = par->left->left;
            free(saveNode);
        }
        else //deletes node if right child
        {
            saveNode = par->right;
            par->right = par->right->left;
            free(saveNode);
        }
        return root;
    }
    if (hasOnlyRightChildTree(delNode)) //case for node with only right child
    {
        if (par == NULL) //deletes root node
        {
            saveNode = delNode->right;
            free(delNode);
            return saveNode;
        }
        //deletes node if left child 
        if (strcmp(root->treeName, treeName) > 0)
        {
            saveNode = par->left;
            par->left = par->left->right;
            free(saveNode);
        }
        else //deletes node if right child
        {
            saveNode = par->right;
            par->right = par->right->right;
            free(saveNode);
        }
        return root;
    }
    //node being deleted has two children
    newDelNode = minValTree(delNode->right); //finds node to delete
    strcpy(saveTreeName, newDelNode->treeName);
    deleteTree(root, saveTreeName);
    strcpy(delNode->treeName, saveTreeName); //restores data to original node that needs to be deleted

    return root;    

}

void freeItems(itemNode *root) //free items recursively 
{
    if(root != NULL)
    {
        freeItems(root->left);
        freeItems(root->right);
        free(root);
    }

}

void freeTree(treeNameNode *root) //frees trees recursively
{
    if(root != NULL)
    {
        freeTree(root->left);
        freeTree(root->right);
        freeItems(root->theTree);
        free(root);
    }

}