#include <stdio.h>						//This program for simply e-marketing program which does adding customer,removing customer,adding basket,and products.
#include <stdlib.h>
#include<string.h>

typedef struct Product{					//Structure for Product which carries data about it.
    int id;
    char* name;
    char* category;
    int price;
    struct Product* next;				//Pointer to product Linked list.
} pro;


typedef struct Basket{					//Structure for basket which carries data about it.
    int id;
    int amount;
    struct Product* product_list;		
    struct Basket* next;				//Pointer to basket linked list.
} basket;

typedef struct Customer{				//Structure for Customer which carries data about it.
    int id;
    char* name;
    char* surname;
    struct Basket* basket_list;
    struct Customer* next;				//Pointer to the customer linked list.
} custom;

custom* head = NULL;
pro *p_head = NULL;

void printMenu();						//Some pre-initializations for functions.
void listCustomers();
void listProducts();
void listBuyers(int);
void listAmounts();


void addCustomer(int id, char* name, char* surname) {	//Function that adds customer to the e-market.
    custom *t, *temp;
    t = (custom*)malloc(sizeof(custom));
    t->id = id;
    t->name = malloc(strlen(name)+1);					//Allocating memory for LL.
    t->surname = malloc(strlen(surname)+1);
    strcpy(t->name,name);
    strcpy(t->surname,surname);

    t->basket_list = NULL;

    if (head == NULL) {				//If LL has no customers.
        head = t;
        head->next = NULL;
        return;
    }

    temp = head;

    while (temp->next != NULL)
        temp = temp->next;

    temp->next = t;
    t->next  = NULL;
}

void addPro(int id,char *name,char *cat,int price){		//Function that adds product to the product linked list.
    pro *p, *temp, *prev;
    p = (pro*)malloc(sizeof(pro));
    p->id = id;
    p->price = price;
    p->name = malloc(strlen(name)+1);
    p->category = malloc(strlen(cat)+1);
    strcpy(p->name,name);
    strcpy(p->category,cat);

    if (p_head == NULL) {								//If the product list is empty.
        p_head = p;
        p_head->next = NULL;
        return;
    }

    temp = p_head;

    while(temp){
        if (strcmp(temp->name,p->name) == 1){				//Adds products alphabetically ordered.
            if(temp == p_head){
                p->next = temp;
                p_head = p;
                break;
            }
            prev->next = p;
            p->next = temp;
            break;
        }
        prev = temp;
        temp = temp->next;
    }

    if (!temp){
        prev->next = p;
        p->next   = NULL;
    }
}

void addBasket(basket** basketlist,int id){		//Function that adds basket for customers.
    basket *b,*temp;
    b = (basket*)malloc(sizeof(basket));		//Memory allocating for LL.
    b->id=id;
    b->product_list=NULL;
    b->next=NULL;
    b->amount=0;

    if (*basketlist == NULL) {				//If the basketlist is empty.
        *basketlist = b;
        return;
    }

    temp = *basketlist;

    while (temp->next != NULL)				//Exploring in the LinkedList.
        temp = temp->next;

    temp->next = b;
    return;

}

int addProduct(pro** prolist, int i){		//This function adds product to the basket 
    pro *p,*temp,*tra;
    tra = p_head;
    while (tra->id != i ){
        tra = tra->next;
    }
    p = (pro*)malloc(sizeof(pro));
    p->name = malloc(strlen(tra->name)+1);				//Memory allocating for the Linked List.
    p->category = malloc(strlen(tra->category)+1);
    strcpy(p->name,tra->name);
    strcpy(p->category,tra->category);
    p->id = tra->id;
    p->price = tra->price;
    p->next=NULL;

    if (*prolist == NULL) {							//If list is empty
        *prolist = p;
        return p->price;
    }

    temp = *prolist;

    while (temp->next != NULL)						//Exploring in the Linked list.
        temp = temp->next;

    temp->next = p;
    return p->price;
}

void removeProducts(pro ** prolist){	//Function that removes products after exiting the system for do not memory leak.
    
    while (*prolist){
        pro *p;
        p = *prolist;
        *prolist  = p->next;
        free(p->name);					//Frees the memory
        free(p->category);
        free(p);
    }

}

void removeBaskets(basket ** basketlist){		//Function that removes baskets after exiting the system for do not memory leak.
    while (*basketlist){
        basket* b;
        b = *basketlist;
        *basketlist = b->next;
        removeProducts(&(b->product_list));
        free(b);								//Frees the memory
    }
}

void removeCustomer(char *name,char *surname){				//Function that removes Customer from the market.
    custom *t,*prev;
    t = head;
    while(t){                                   //find the wanted land to remove
        if(!strcmp(t->name,name) && !(strcmp(t->surname,surname))){
            if(t == head){       // remove from the head
                head = head->next;
                removeBaskets(&(t->basket_list));
            }
            else if(!t->next){       // remove from the end
                prev->next = NULL;
                removeBaskets(&(t->basket_list));
            }
            else{                           // remove from between
                prev->next = t->next;
                removeBaskets(&(t->basket_list));
            }	
            free(t->name);					//Frees the memory
            free(t->surname);
            free(t);
            break;
        }
        prev = t;
        t = t->next;
    }
    
}

void removeAll(){		//Removes all features after exiting the system for do not memory leak.
    custom *t;
    while (head) {
        t = head;
        head = head->next;
        removeBaskets(&(t->basket_list));
        free(t->name);
        free(t->surname);
        free(t);
    }
    removeProducts(&p_head);
}



int main(){										
    FILE *file = fopen("customer.txt","r+");				//Reading Customers and add to linked list
    char line[50];
    while (fgets(line,50,file) != NULL){
        char name[10],surname[10];
        int id;
        sscanf(line,"%d%s%s",&id,name,surname);
        addCustomer(id,name,surname);
    }
    FILE *file1 = fopen("product.txt","r+");				//Reading products and adding to the linked list
    char line1[50];
    int index=0;
    while (fgets(line1,50,file1) != NULL){
        char name[10],cat[10];
        int id,price;
        sscanf(line1,"%d%s%s%d",&id,name,cat,&price);
        addPro(id,name,cat,price);
    }
    FILE *file2 = fopen("basket.txt","r+");					//Reading baskets and adding to the linked list
    char line2[50];
    int i = 0;
    while (fgets(line2,50,file2) != NULL){
        int cos_id,id,j;
        sscanf(line2,"%d%d%d",&cos_id,&id,&j);

        custom *t;
        t = head;
        while(t->id != cos_id)
            t = t->next;
        
        if(!t->basket_list || i!=id){							
            if(!t->basket_list)
            i = 0;
            addBasket(&(t->basket_list),id);
            basket *b;
            b = t->basket_list;
            while (b->id != id)
                b = b->next;
            b->amount+=addProduct(&(b->product_list),j);
            i++;
        }
        else {
            basket *b;
            b = t->basket_list;
            while (b->id != id)
                b = b->next;
            b->amount+=addProduct(&(b->product_list),j);
        }
    }

    
    int choice = 0;
    while (choice != 6) {						//Shows menu and asks to user which option she/he will use.
        printMenu();
        scanf("%d",&choice);
        switch (choice) {
        case 1: {								//If wants to add customer
            char name[10],surname[10];
            int cus_n = 1;
            custom *t;
            t = head;
            while (t){
                t = t->next;
                cus_n++;
            }
            printf("Please enter Name Surname with a space in between\n");
            scanf("%s %s",name,surname);
            addCustomer(cus_n,name,surname);
            listCustomers();
            break;
        }
        case 2: {							//If wants to add basket and suboperations
            int cus,pro=0,id=1,amount=0;
            listCustomers();
            printf("Please enter a customer id:\n");
            scanf("%d",&cus);
            custom *t;
            t = head;
            while(t->id != cus)				//Searching for customer
                t = t->next;
            basket *b;
            b = t->basket_list;
            while (b){
                b = b->next;
                id++;
            }
            
            addBasket(&(t->basket_list),id);		//Used addBasket function
            b = t->basket_list;
            while (b->id != id)
                b = b->next;
            while(1){
                listProducts();
                printf("Please enter a product id:\n");
                scanf("%d",&pro);
                if (pro == -1) {break;}
                amount+=addProduct(&(b->product_list),pro);;
                
            }
            b->amount = amount;
            //list();
            
            break;
        }
        case 3:						//If wants to remove a customer. 
            listCustomers();
            char name[10],surname[10];
            printf("Please enter Name Surname with a space in between\n");
            scanf("%s %s",name,surname);
            removeCustomer(name,surname);
            printf("\n");
            listCustomers();      
            break;
        case 4:					// If wants to list the customers who bought a specific product
            listProducts();
            int pick = 0;
            printf("\nPlease enter a Product id\n");
            scanf("%d",&pick);				//Gets product id.
            listBuyers(pick);
            break;
        case 5:					//If wants to list the total shopping amounts of each customer
            listAmounts();		//Used list amount function	
            break;
        case 6:					//If user wants to exit the system.
            removeAll();		// Used removeAll function in order not to memory leak after exiting. 
            return 0;
        default:
            printf("You entered an invalid value. Try again.");
        }
    }
}

void printMenu() {						//Function that prints menu
    printf("\n1. Add a customer\n");
    printf("2. Add basket\n");
    printf("3. Remove  customer\n");
    printf("4. List  the customers  who  bought  a  specific  product\n");
    printf("5. List  the  total  shopping  amounts  of  each  customer\n");
    printf("6. Exit\n");
    printf("\n");
}


void listCustomers(){					//Function that lists all customer in the linked list(market).
    custom *tra;
    tra = head;
    while (tra){						//To the end of the Linked List.
        printf("%d %s %s\n",tra->id,tra->name,tra->surname);
        tra = tra->next;
    }
}
void listProducts(){					//Function that lists products.
    pro *tra;
    tra = p_head;	
    while (tra != NULL){				//To the end of the LinkedList.
        printf("%d %s %s %d\n",tra->id,tra->name,tra->category,tra->price);
        tra = tra->next;
    }
}


void listBuyers(int id){				//Function that lists buyers for option 4 in the menu.
    custom *t;
    basket *b;
    pro *p;
    t = head;
    int once;
    while (t){							//To the end of the linked list.
        b = t->basket_list;
        once = 0;
        while (b && !once){
            p=b->product_list;
            while (p){
                if (p->id == id){		// If ids matched.
                    printf("%s %s bought %s\n",t->name,t->surname,p->name);
                    once = 1;
                    break;
                }
                p = p->next;
            }
            b = b->next;
        }
        t = t->next;
    }
}
void listAmounts(){							//Function that lists amounts of each customer for option 5 in the menu.
    custom *t;
    basket *b;
    t = head;
    while (t){								//To the end of the linkedlist.
        int total_amount=0;
        b=t->basket_list;
        while (b){
            total_amount += b->amount;
            b = b->next;
        }
        printf("%s %s's total amount %d\n",t->name,t->surname,total_amount);	//Prints total amount.
        t = t->next;
    }
}


