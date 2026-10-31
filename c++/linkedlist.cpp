#include <iostream>
#include <string>


struct Node {
    int data;
    Node *next;

};

void insertatbeginning(Node **head , int val){
    Node *newnode = new Node{val,*head};
   
   *head = newnode;
}
void printlist(Node *head){
    
    Node *temp = head;
    while(temp != nullptr){
        std::cout << temp->data;
        temp = temp->next;
    }

    
}

void insertatpos(Node **head , int val,int pos){
    Node *newnode = new Node{val,nullptr};

    if (*head == nullptr || pos == 0){
         newnode->next = *head;
        *head = newnode;
        return;
    }
    Node *temp = *head;
    for(int i = 0; i < pos - 1 && temp->next != nullptr; i++) temp = temp->next;
    newnode->next = temp->next;
    temp->next = newnode;


}

void insertatend(Node **head,int val){
    Node *newnode = new Node{val,nullptr};

    if (*head == nullptr){
        *head = newnode;
        return;

    }
    Node *temp = *head;
    while(temp->next != nullptr){
        temp = temp->next;
    }
    temp->next = newnode; 

}
int main(){
    Node *head = nullptr;
    insertatbeginning(&head , 10);
    insertatend(&head,11);
    printlist(head);

    return 0;
}