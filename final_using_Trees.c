#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node_Individual {
    int id;
    char name[100];
    int age;
    int daily_goal;
    int weekly_step_count[7];
    int ingroup;
    int height;
    struct Node_Individual* left;
    struct Node_Individual* right;
} Node_Individual;

typedef struct Node_Group {
    int group_id;
    int group_size;
    char group_name[100];
    int Weekly_Group_Goal;
    int height;
    Node_Individual **individuals;
    struct Node_Group *left;  
    struct Node_Group *right; 
} Node_Group;

typedef struct LeaderboardNode {
    char group_name[100];
    int total_steps;
    int height;
    struct LeaderboardNode* left;
    struct LeaderboardNode* right;
} LeaderboardNode;



int height(Node_Individual* node) {
    if (node == NULL)
        return 0;
    return node->height;
}


int max(int a, int b) {
    return (a > b) ? a : b;
}


int balance_factor(Node_Individual* node) {
    if (node == NULL)
        return 0;
    return height(node->left) - height(node->right);
}


Node_Individual* rotate_right(Node_Individual* y) {
    Node_Individual* x = y->left;
    Node_Individual* T2 = x->right;

    x->right = y;
    y->left = T2;

 
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}


Node_Individual* rotate_left(Node_Individual* x) {
    Node_Individual* y = x->right;
    Node_Individual* T2 = y->left;

    y->left = x;
    x->right = T2;


    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}


Node_Individual* insert(Node_Individual* root, Node_Individual* new_node) {
   
    if (root == NULL)
        return new_node;

    if (new_node->id < root->id)
        root->left = insert(root->left, new_node);
    else if (new_node->id > root->id)
        root->right = insert(root->right, new_node);
    else 
        return root;

   
    root->height = 1 + max(height(root->left), height(root->right));

    
    int balance = balance_factor(root);


    if (balance > 1 && new_node->id < root->left->id)
        return rotate_right(root);

    
    if (balance < -1 && new_node->id > root->right->id)
        return rotate_left(root);

    if (balance > 1 && new_node->id > root->left->id) {
        root->left = rotate_left(root->left);
        return rotate_right(root);
    }

 
    if (balance < -1 && new_node->id < root->right->id) {
        root->right = rotate_right(root->right);
        return rotate_left(root);
    }

    return root;
}

void display_inorder(Node_Individual* root) {
    if (root != NULL) {
        display_inorder(root->left);
        printf("ID: %d, Name: %s, Age: %d, Daily Goal: %d\n", root->id, root->name, root->age, root->daily_goal);
        printf("Weekly Step Count: ");
        for (int i = 0; i < 7; i++) {
            printf("%d ", root->weekly_step_count[i]);
        }
        printf("\n");
        display_inorder(root->right);
    }
}

Node_Individual* search_individual_avl_tree(Node_Individual* root, int id) {
    while (root != NULL) {
        if (id == root->id) {
            return root;
        } else if (id < root->id) {
            root = root->left;
        } else {
            root = root->right;
        }
    }
    return NULL; 
}


void read_data_and_store_in_avl_tree(const char* filename, Node_Individual** root) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        Node_Individual* new_node = (Node_Individual*)malloc(sizeof(Node_Individual));
        if (new_node == NULL) {
            perror("Memory allocation failed");
            fclose(file);
            return;
        }
        sscanf(line, "%d %s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
               &new_node->id, new_node->name, &new_node->age, &new_node->daily_goal,
               &new_node->weekly_step_count[0], &new_node->weekly_step_count[1],
               &new_node->weekly_step_count[2], &new_node->weekly_step_count[3],
               &new_node->weekly_step_count[4], &new_node->weekly_step_count[5],
               &new_node->weekly_step_count[6], &new_node->ingroup);

        new_node->height = 1;
        new_node->left = NULL;
        new_node->right = NULL;
        new_node->ingroup=0;
        
        *root = insert(*root, new_node);
    }
    fclose(file);
}

void overrideIndividualFile(Node_Individual* root, FILE* individual_file) {
    if (root != NULL) {
        overrideIndividualFile(root->left, individual_file);
        fprintf(individual_file, "%d %s %d %d", root->id, root->name, root->age, root->daily_goal);
        for (int i = 0; i < 7; i++) {
            fprintf(individual_file, " %d", root->weekly_step_count[i]);
        }
        fprintf(individual_file, "\n");
        fflush(individual_file); 
        overrideIndividualFile(root->right, individual_file);
    }
}

Node_Individual* create_individual_node(int id, const char* name, int age, int daily_goal, int weekly_step_count[7]) {
    Node_Individual* new_node = (Node_Individual*)malloc(sizeof(Node_Individual));
    if (new_node == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    new_node->id = id;
    strcpy(new_node->name, name);
    new_node->age = age;
    new_node->daily_goal = daily_goal;
    memcpy(new_node->weekly_step_count, weekly_step_count, sizeof(int) * 7); 
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->height = 1; 
    new_node->ingroup=0;
    return new_node;
}

void display_group_members(Node_Group* root) {
    if (root != NULL) {
        display_group_members(root->left);
        printf("Group ID: %d\n", root->group_id);
        printf("Group Name: %s\n", root->group_name);
        printf("Group Size: %d\n", root->group_size);
        printf("Weekly Group Goal: %d\n", root->Weekly_Group_Goal);
        printf("Group Members:\n");
        for (int i = 0; i < root->group_size; i++) {
            printf("ID: %d, Name: %s, Age: %d, Daily Goal: %d\n",
                   root->individuals[i]->id, root->individuals[i]->name,
                   root->individuals[i]->age, root->individuals[i]->daily_goal);
            printf("Weekly Step Count: ");
            for (int j = 0; j < 7; j++) {
                printf("%d ", root->individuals[i]->weekly_step_count[j]);
            }
            printf("\n");
        }
        printf("\n");
        display_group_members(root->right);
    }
}

int height_group(Node_Group* node) {
    if (node == NULL) {
        return 0;
    }
    return node->height;
}

Node_Group* right_rotate(Node_Group* y) {
    Node_Group* x = y->left;
    Node_Group* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = 1 + max(height_group(y->left), height_group(y->right));
    x->height = 1 + max(height_group(x->left), height_group(x->right));


    return x;
}

Node_Group* left_rotate(Node_Group* x) {
    Node_Group* y = x->right;
    Node_Group* T2 = y->left;

   
    y->left = x;
    x->right = T2;

    x->height = 1 + max(height_group(x->left), height_group(x->right));
    y->height = 1 + max(height_group(y->left), height_group(y->right));

   
    return y;
}

int get_balance(Node_Group* node) {
    if (node == NULL) {
        return 0;
    }
    return height_group(node->left) - height_group(node->right);
}

Node_Group* insert_group_avl_tree(Node_Group* root, Node_Group* new_node) {
    if (root == NULL) {
        return new_node;
    }

    if (new_node->group_id < root->group_id) {
        root->left = insert_group_avl_tree(root->left, new_node);
    } else if (new_node->group_id > root->group_id) {
        root->right = insert_group_avl_tree(root->right, new_node);
    } else {
        return root;
    }

    root->height = 1 + max(height_group(root->left), height_group(root->right));

 
    int balance = get_balance(root);

    if (balance > 1 && new_node->group_id < root->left->group_id) {
        return right_rotate(root);
    }
    if (balance < -1 && new_node->group_id > root->right->group_id) {
        return left_rotate(root);
    }
    if (balance > 1 && new_node->group_id > root->left->group_id) {
        root->left = left_rotate(root->left);
        return right_rotate(root);
    }
    if (balance < -1 && new_node->group_id < root->right->group_id) {
        root->right = right_rotate(root->right);
        return left_rotate(root);
    }

    return root;
}




Node_Group* readFromGroupFile(FILE* group_file, FILE* individual_file, Node_Individual* individual_tree, Node_Group** group_tree) {
    Node_Group* root_group = NULL;
    while (1) {
       
        Node_Group* new_group = (Node_Group*)malloc(sizeof(Node_Group));
        if (!new_group) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }

        if (fscanf(group_file, "%d %d %99s %d", &new_group->group_id, &new_group->group_size, new_group->group_name, &new_group->Weekly_Group_Goal) != 4) {
            free(new_group);
            break;
        }

        new_group->individuals = (Node_Individual**)malloc(new_group->group_size * sizeof(Node_Individual*));
        if (!new_group->individuals) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < new_group->group_size; i++) {
            int id;
            if (fscanf(group_file, "%d", &id) != 1) {
                perror("Error reading individual ID from file");
                free(new_group->individuals);
                free(new_group);
                break;
            }

            Node_Individual* individual_node = search_individual_avl_tree(individual_tree, id);
            if (individual_node == NULL) {
                printf("Individual with ID %d is not registered\n", id);
                new_group->group_size--;
            } else {
                new_group->individuals[i] = individual_node;
                individual_node->ingroup=1;
            }
        }

        new_group->left = NULL;
        new_group->right = NULL;

    
        if (!root_group) {
            root_group = new_group;
        } else {
            root_group = insert_group_avl_tree(root_group, new_group); 
        }
    }

    fclose(group_file);
    return root_group;
}



void addIndividual(Node_Individual** root, FILE* individual_file) {
    int id;
    char name[100];
    int age;
    int daily_goal;
    int weekly_step_count[7];

   
    printf("Enter ID: ");
    scanf("%d", &id);


    if (search_individual_avl_tree(*root, id) != NULL) {
        printf("ID %d already exists. Please enter a unique ID.\n", id);
        return;
    }

    printf("Enter name: ");
    scanf("%99s", name);

    printf("Enter age: ");
    scanf("%d", &age);

    printf("Enter daily goal: ");
    scanf("%d", &daily_goal);

    printf("Enter weekly step count (7 values): ");
    for (int i = 0; i < 7; i++) {
        scanf("%d", &weekly_step_count[i]);
    }

    Node_Individual* new_individual = create_individual_node(id, name, age, daily_goal, weekly_step_count);


    *root = insert(*root, new_individual);
    FILE* ind_file = fopen("dspd_file_individual.txt", "w");
    overrideIndividualFile(*root,ind_file);
}

Node_Group* search_group_avl_tree(Node_Group* root, int group_id) {
    if (root == NULL || root->group_id == group_id) {
        return root;
    }

    if (group_id < root->group_id) {
        return search_group_avl_tree(root->left, group_id);
    } else {
        return search_group_avl_tree(root->right, group_id);
    }
}

void write_group_data_inorder(Node_Group* root, FILE* group_file) {
    if (root != NULL) {
        write_group_data_inorder(root->left, group_file);
        fprintf(group_file, "%d %d %s %d", root->group_id, root->group_size, root->group_name, root->Weekly_Group_Goal);
        for (int i = 0; i < root->group_size; i++) {
            fprintf(group_file, " %d", root->individuals[i]->id);
        }
        fprintf(group_file, "\n");
        write_group_data_inorder(root->right, group_file);
    }
}

void write_group_to_file(Node_Group* root) {
    FILE* group_file = fopen("dspd_file_group.txt", "w");
    if (group_file == NULL) {
        perror("Error opening group file");
        exit(EXIT_FAILURE);
    }

    write_group_data_inorder(root, group_file);

    fclose(group_file);
}


void create_group(Node_Group** group_tree, Node_Individual* individual_tree) {
    Node_Group* group_to_insert = (Node_Group*)malloc(sizeof(Node_Group));
    if (group_to_insert == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    printf("Enter the group details:\n");
    printf("Enter group_id:\n");
    scanf("%d", &group_to_insert->group_id);


    if (search_group_avl_tree(*group_tree, group_to_insert->group_id) != NULL) {
        printf("Error: ID already exists. Please choose a different ID.\n");
        free(group_to_insert);
        return;
    }

    printf("Enter group_size:\n");
    scanf("%d", &group_to_insert->group_size);
    printf("Enter group_name:\n");
    scanf("%s", group_to_insert->group_name);
    printf("Enter Weekly_Group_Goal:\n");
    scanf("%d", &group_to_insert->Weekly_Group_Goal);

    group_to_insert->individuals = (Node_Individual**)malloc(group_to_insert->group_size * sizeof(Node_Individual*));
    if (group_to_insert->individuals == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    printf("Enter Member_id of group_members (each separated by space):\n");
    for (int i = 0; i < group_to_insert->group_size; i++) {
        int id;
        scanf("%d", &id);

        Node_Individual* individual = search_individual_avl_tree(individual_tree, id);

        if (individual != NULL && individual->ingroup == 0) {
            group_to_insert->individuals[i] = individual;
            individual->ingroup = 1; 
        } else {
            fprintf(stderr, "Error: Member ID %d not found or already in a group.\n", id);
            free(group_to_insert->individuals);
            free(group_to_insert);
            return;
        }
    }

    *group_tree = insert_group_avl_tree(*group_tree, group_to_insert);

    write_group_to_file(*group_tree);

    printf("Group added successfully!\n");
}

int get_sum_of_weekly_steps(Node_Individual* individual) {
    int sum_steps = 0;
    for (int i = 0; i < 7; i++) {
        sum_steps += individual->weekly_step_count[i];
    }
    return sum_steps;
}


void display_top3(Node_Individual* top3[]) {
    printf("Top 3 individuals who completed their daily goals and achieved the highest sum of weekly step counts:\n");
    for (int i = 0; i < 3 && top3[i] != NULL; i++) {
        printf("ID: %d, Name: %s, Age: %d, Daily Goal: %d, Weekly Step Count: %d\n",
               top3[i]->id, top3[i]->name, top3[i]->age, top3[i]->daily_goal, get_sum_of_weekly_steps(top3[i]));
    }
}

void update_top3(Node_Individual* individual, Node_Individual* top3[]) {

    int sum_steps = 0;
    for (int i = 0; i < 7; i++) {
        if(individual->weekly_step_count[i]<individual->daily_goal)
        {
            return;
        }
        sum_steps += individual->weekly_step_count[i];
    }

    for (int i = 0; i < 3; i++) {
        if (top3[i] == NULL || sum_steps > get_sum_of_weekly_steps(top3[i])) {
            for (int j = 2; j > i; j--) {
                top3[j] = top3[j - 1];
            }
            top3[i] = individual;
            break;
        }
    }
}


void inorder_traversal(Node_Individual* root, Node_Individual* top3[]) {
    if (root != NULL) {
        inorder_traversal(root->left, top3);

        update_top3(root, top3);

        inorder_traversal(root->right, top3);
    }
}


void Get_top_3(Node_Individual* root) {
    Node_Individual* top3[3] = {NULL}; 
    inorder_traversal(root, top3); 
    display_top3(top3); 
}


void Check_group_achievement(int Group_ID, Node_Group* group_tree) {
    Node_Group* group = search_group_avl_tree(group_tree, Group_ID);

    if (group == NULL) {
        printf("Group with ID %d not found.\n", Group_ID);
        return;
    }

    printf("Group ID: %d\n", group->group_id);
    printf("Group Name: %s\n", group->group_name);
    printf("Group Size: %d\n", group->group_size);
    printf("Group Weekly Goal: %d\n", group->Weekly_Group_Goal);

    printf("Group Members:\n");
    int total_weekly_steps_group = 0;
    for (int i = 0; i < group->group_size; i++) {
        printf("Individual ID: %d, Name: %s\n", group->individuals[i]->id, group->individuals[i]->name);
        printf("Weekly Goal: %d\n", group->individuals[i]->daily_goal);
        int weekly_steps_individual = get_sum_of_weekly_steps(group->individuals[i]);
        printf("Total Weekly Steps: %d\n", weekly_steps_individual);
        total_weekly_steps_group += weekly_steps_individual;
        printf("\n");
    }

    printf("Total Weekly Steps for Group: %d\n", total_weekly_steps_group);


    printf("Group Weekly Goal Status: ");
    if (total_weekly_steps_group >= group->Weekly_Group_Goal) {
        printf("Met\n");
    } else {
        printf("Not Met\n");
    }
}

Node_Group* sorted_group_tree = NULL;
int calculateTotalSteps(Node_Group* group) {
    int total_steps = 0;
    for (int i = 0; i < group->group_size; i++) {
        total_steps += get_sum_of_weekly_steps(group->individuals[i]);
    }
    return total_steps;
}

int height_leaderboard(LeaderboardNode* node) {
    if (node == NULL) {
        return 0;
    }
    return 1 + max(height_leaderboard(node->left), height_leaderboard(node->right));
}

int get_balance_leaderboard(LeaderboardNode* node) {
    if (node == NULL) {
        return 0;
    }
    return height_leaderboard(node->left) - height_leaderboard(node->right);
}

LeaderboardNode* left_rotate_leaderboard(LeaderboardNode* x) {
    LeaderboardNode* y = x->right;
    LeaderboardNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    return y;
}

LeaderboardNode* right_rotate_leaderboard(LeaderboardNode* y) {
    LeaderboardNode* x = y->left;
    LeaderboardNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    return x;
}

LeaderboardNode* insert_leaderboard_tree(LeaderboardNode* root, Node_Group* new_group) {
    if (root == NULL) {
        LeaderboardNode* new_node = (LeaderboardNode*)malloc(sizeof(LeaderboardNode));
        if (new_node == NULL) {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }
        strcpy(new_node->group_name, new_group->group_name);
        new_node->total_steps = calculateTotalSteps(new_group);
        new_node->left = NULL;
        new_node->right = NULL;
        return new_node;
    }

    if (calculateTotalSteps(new_group) >= root->total_steps) {
        root->right = insert_leaderboard_tree(root->right, new_group);
    } else {
        root->left = insert_leaderboard_tree(root->left, new_group);
    }

    root->height = 1 + max(height_leaderboard(root->left), height_leaderboard(root->right));

    int balance = get_balance_leaderboard(root);


    if (balance > 1 && calculateTotalSteps(new_group) < root->left->total_steps) {
        return right_rotate_leaderboard(root);
    }
    if (balance < -1 && calculateTotalSteps(new_group) > root->right->total_steps) {
        return left_rotate_leaderboard(root);
    }
    if (balance > 1 && calculateTotalSteps(new_group) > root->left->total_steps) {
        root->left = left_rotate_leaderboard(root->left);
        return right_rotate_leaderboard(root);
    }
    if (balance < -1 && calculateTotalSteps(new_group) < root->right->total_steps) {
        root->right = right_rotate_leaderboard(root->right);
        return left_rotate_leaderboard(root);
    }

    return root;
}


LeaderboardNode* leaderboard_tree = NULL;

void inorder_insert_leaderboard_tree(Node_Group* root) {
    if (root != NULL) {
        inorder_insert_leaderboard_tree(root->left);
        leaderboard_tree = insert_leaderboard_tree(leaderboard_tree, root);
        inorder_insert_leaderboard_tree(root->right);
    }
}

void display_leaderboard(LeaderboardNode* root, int* rank) {
    if (root != NULL) {
        display_leaderboard(root->right, rank);
        printf("%d. Group Name: %s, Total Steps: %d\n", *rank, root->group_name, root->total_steps);
        (*rank)++;
        display_leaderboard(root->left, rank);
    }
}

void Generate_leader_board(Node_Group* group_tree) {
    if (group_tree == NULL) {
        printf("No groups found.\n");
        return;
    }


    inorder_insert_leaderboard_tree(group_tree);

    
    printf("Leaderboard:\n");
    int rank = 1;
    display_leaderboard(leaderboard_tree, &rank);
}

void check_individual_reward(Node_Individual * root,int id){
    Node_Individual* top3[3] = {NULL}; 
    inorder_traversal(root, top3); 
    
    int rank=-1;

    for(int i=0;i<3;i++){
        if(top3[i]->id==id){
            rank=i+1;
            break;
        }
    }



    switch (rank) {
        case 1:
            printf("Congratulations! You are ranked 1st and have earned 100 points.\n");
            break;
        case 2:
            printf("Congratulations! You are ranked 2nd and have earned 75 points.\n");
            break;
        case 3:
            printf("Congratulations! You are ranked 3rd and have earned 50 points.\n");
            break;
        default:
            printf("Sorry, you are not in the top 3 individuals.\n");
            break;
    }
}

Node_Individual* find_min(Node_Individual* root) {
    Node_Individual* current = root;
    while (current->left != NULL) {
        current = current->left;
    }
    return current;
}
int deleteFromNodeGroup(Node_Group *group,Node_Individual * toremove) {
    int indexToDelete = -1;
    for (int i = 0; i < group->group_size; i++) {
        if (group->individuals[i] == toremove) {
            indexToDelete = i;
            break;
        }
    }

    if (indexToDelete != -1) {
        for (int i = indexToDelete; i < group->group_size - 1; i++) {
            group->individuals[i] = group->individuals[i + 1];
        }
        group->group_size--;
        return 1;
    } 
    return 0;
}
void searchnremove(Node_Group* group,Node_Individual * toremove)
{
    if(group==NULL)
        return ;
    int ans=deleteFromNodeGroup(group,toremove);
    if (ans==1 )
        return;
    else
    {    
        searchnremove(group->left,toremove);
        searchnremove(group->right,toremove);
    }
}
Node_Individual* delete_individual(Node_Group* group,Node_Individual* root, int id) {
    if (root == NULL) {
        printf("Individual with ID %d not found.\n", id);
        return root;
    }

 
    if (id < root->id) {
        root->left = delete_individual(group,root->left, id);
    } else if (id > root->id) {
        root->right = delete_individual(group,root->right, id);
    } else {
        
        searchnremove(group,root);
        if (root->left == NULL || root->right == NULL) {
            Node_Individual* temp = root->left ? root->left : root->right;
        
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else { 
                *root = *temp; 
            }
            free(temp);
        } else {
       
            Node_Individual* temp = find_min(root->right);
            root->id = temp->id; 
            root->right = delete_individual(group,root->right, temp->id);
        }
    }


    if (root == NULL) {
        return root;
    }


    root->height = 1 + max(height(root->left), height(root->right));

    int balance = balance_factor(root);


    if (balance > 1 && balance_factor(root->left) >= 0) {
        return rotate_right(root);
    }


    if (balance > 1 && balance_factor(root->left) < 0) {
        root->left = rotate_left(root->left);
        return rotate_right(root);
    }

    if (balance < -1 && balance_factor(root->right) <= 0) {
        return rotate_left(root);
    }

    if (balance < -1 && balance_factor(root->right) > 0) {
        root->right = rotate_right(root->right);
        return rotate_left(root);
    }

    return root;
}
void Delete_individual(Node_Individual** root, Node_Group* group_tree, int id) {
    *root = delete_individual(group_tree,*root, id);


    FILE* ind_file = fopen("dspd_file_individual.txt", "w");
    overrideIndividualFile(*root,ind_file);
     write_group_to_file(group_tree);
     
}
Node_Group* find_mini(Node_Group* root) {
    Node_Group* current = root;
    while (current->left != NULL) {
        current = current->left;
    }
    return current;
}
Node_Group* delete_group(Node_Group* root, int group_id) {
    if (root == NULL) {
        printf("Group not found.\n");
        return root;
    }

    if (group_id < root->group_id) {
        root->left = delete_group(root->left, group_id);
    } else if (group_id > root->group_id) {
        root->right = delete_group(root->right, group_id);
    } else {
        for(int i=0;i<root->group_size;i++)
        {
            root->individuals[i]->ingroup=0;
        }
        if (root->left == NULL || root->right == NULL) {
            Node_Group* temp = root->left ? root->left : root->right;
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else { 
                *root = *temp; 
            }
            free(temp);
        } else {

            Node_Group* temp = find_mini(root->right); 
            root->group_id = temp->group_id; 
            root->right = delete_group(root->right, temp->group_id);
        }
    }


    if (root == NULL) {
        return root;
    }


    root->height = 1 + max(height_group(root->left), height_group(root->right));

    int balance = get_balance(root);


    if (balance > 1 && get_balance(root->left) >= 0) {
        return right_rotate(root);
    }


    if (balance > 1 && get_balance(root->left) < 0) {
        root->left = left_rotate(root->left);
        return right_rotate(root);
    }

    if (balance < -1 && get_balance(root->right) <= 0) {
        return left_rotate(root);
    }

    if (balance < -1 && get_balance(root->right) > 0) {
        root->right = right_rotate(root->right);
        return left_rotate(root);
    }

    return root;
}

void Delete_group(Node_Group** root, int group_id) {
    *root = delete_group(*root, group_id);
    write_group_to_file(*root);
}

Node_Group* Merge_groups( Node_Group * group_list,int Group_ID_1, int Group_ID_2) {
    Node_Group *group1 = NULL;
    Node_Group *group2 = NULL;
    Node_Group* groupList=group_list;
    Node_Group* ans = groupList;

    

    group1=search_group_avl_tree(groupList,Group_ID_1);
    group2=search_group_avl_tree(group_list,Group_ID_2);

    if (group1 == NULL || group2 == NULL) {
        printf("One or both of the specified groups not found.\n");
        return ans;
    }

    Node_Group *newGroup = (Node_Group *)malloc(sizeof(Node_Group));
    if (newGroup == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    newGroup->group_id = Group_ID_1;
    newGroup->group_size = group1->group_size + group2->group_size;

    if (newGroup->group_size > 5) {
        printf("Error: Merged group exceeds the maximum allowed size.\n");
        exit(EXIT_FAILURE);
    }

    snprintf(newGroup->group_name, sizeof(newGroup->group_name), "%s%s", group1->group_name, group2->group_name);

    newGroup->Weekly_Group_Goal = group1->Weekly_Group_Goal + group2->Weekly_Group_Goal;

    newGroup->individuals = (Node_Individual **)malloc(newGroup->group_size * sizeof(Node_Individual *));
    if (newGroup->individuals == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    int i;
    for (i = 0; i < group1->group_size; i++) {
        newGroup->individuals[i] = group1->individuals[i];
    }

    for (int j = 0; j < group2->group_size; j++, i++) {
        newGroup->individuals[i] = group2->individuals[j];
    }

    newGroup->left=NULL;
    newGroup->right=NULL;

    Delete_group(&group_list,Group_ID_1);
    group_list=insert_group_avl_tree(group_list,newGroup);
    Delete_group(&group_list,Group_ID_2);
    

    write_group_to_file(group_list);
    printf("Groups merged successfully.\n");

    group_list=ans;
}

void inorder_display_group_range_info(Node_Group* root, int start_id, int end_id, int rank) {
    if (root != NULL) {
        if (root->group_id > start_id) {
            inorder_display_group_range_info(root->left, start_id, end_id, rank);
        }

       
        if (root->group_id >= start_id && root->group_id <= end_id) {
            printf("%d. Group ID: %d, Group Name: %s, Group Size: %d, Weekly Group Goal: %d\n", rank, root->group_id, root->group_name, root->group_size, root->Weekly_Group_Goal);
            printf("   Members:\n");
            for (int i = 0; i < root->group_size; i++) {
                printf("   - Member ID: %d\n", root->individuals[i]->id);
                printf("     Name: %s\n", root->individuals[i]->name);
                printf("     Weekly Step Count: ");
                for (int j = 0; j < 7; j++) {
                    printf("%d ", root->individuals[i]->weekly_step_count[j]);
                }
                printf("\n");
              
            }
        }

        if (root->group_id < end_id) {
            inorder_display_group_range_info(root->right, start_id, end_id, rank + 1);
        }
    }
}

void Display_group_range_info(Node_Group* root, int start_id, int end_id) {
    if (root == NULL) {
        printf("No groups found.\n");
        return;
    }

    inorder_display_group_range_info(root, start_id, end_id, 1);
}
int suggest_daily_top_3(Node_Individual* root) {
    Node_Individual* top3[3] = {NULL}; 
    inorder_traversal(root, top3); 
    int sum=0;
    for(int i=0;i<3;i++){
       sum+=get_sum_of_weekly_steps(top3[i]);
    }
    sum/=3;
    sum/=7;

    return sum;
   
}

int main() {
    Node_Individual* root = NULL;
    read_data_and_store_in_avl_tree("dspd_file_individual.txt", &root);

    FILE* individual_file = fopen("dspd_file_individual.txt", "r");
    FILE* group_file = fopen("dspd_file_group.txt", "r");

    if (individual_file == NULL || group_file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

      Node_Group* group_tree = NULL;

 
    group_tree = readFromGroupFile(group_file,individual_file,root, &group_tree);
    

      int choice;
    do {
        printf("\nMenu:\n");
        printf("1. Add Person\n");
        printf("2. Create Group\n");
        printf("3. Get Top 3 Individuals\n");
        printf("4. Check Group Achievement\n");
        printf("5. Generate Leaderboard\n");
        printf("6. Check Individual Rewards\n");
        printf("7. Delete Individual\n");
        printf("8. Delete Group\n");
        printf("9. Merge Groups\n");
        printf("10. Display Group Info\n");
        printf("11. Suggest Goal Update\n");
        printf("12. Print Individual Info:\n");
        printf("13. Print Group Info\n");
        printf("0. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:{
                addIndividual(&root, individual_file);
                break;
            }
            case 2:{
                create_group(&group_tree,root);
                break;
            }
            case 3:{
                Get_top_3(root);
                break;
            }
            case 4:{
                printf("Enter group_id:\n");
                int g;
                scanf("%d",&g);
                Check_group_achievement(g,group_tree);
                break;
            }
            case 5:{
                Generate_leader_board(group_tree);
                break;
            }
            case 6:{
                int a;
                printf("Enter individual ID: ");
                scanf("%d", &a);
                check_individual_reward(root,a);
                break;
            }
            case 7:{
                
                int idToDeleteindividual;
                printf("Enter individual ID to delete: ");
                scanf("%d", &idToDeleteindividual);
                Delete_individual(&root,group_tree,idToDeleteindividual);
                break;
            }
            case 8:{
                int idToDeletegroup;
                printf("Enter group ID to delete: ");
                scanf("%d", &idToDeletegroup);
                Delete_group(&group_tree,idToDeletegroup);
                break;
            }
            case 9:{
                int groupID1, groupID2;
                printf("Enter first group ID to merge: ");
                scanf("%d", &groupID1);
                printf("Enter second group ID to merge: ");
                scanf("%d", &groupID2);
                Merge_groups(group_tree, groupID1, groupID2);
                break;
            }
            case 10:{
                int groupID1,groupID2;
                printf("Enter group ID 1 to display info: ");
                scanf("%d", &groupID1);
                 printf("Enter group ID 2 to display info: ");
                scanf("%d", &groupID2);
                Display_group_range_info(group_tree,groupID1,groupID2);
                break;
            }
            case 11:{
                printf("%d\n",suggest_daily_top_3(root));
                break;
            }
            case 12:{
                 printf("Individuals are as follows:\n");
                 display_inorder(root);
                 break;
            }
            case 13:{
                printf("Group list is as follows:\n");
                  display_group_members(group_tree);

                break;
            }
            case 0:{
                printf("Exiting program.\n");
                break;
            }
            default:{
                printf("Invalid choice. Please try again.\n");
                break;
            }
        }

    } while (choice != 0);

    return 0;
}
