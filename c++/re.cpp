
#include <iostream>

struct  Node {
    int data;
    Node *next; 
};

void insertathead(Node **head,int val){
    Node *newnode = new Node{val,*head};

    newnode->next = *head;
   *head = newnode;
    
}
void printlist(Node *head){
    Node *temp = head;
    while(temp != nullptr){
        std::cout << temp->data << "->";
        temp = temp->next;
    }
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

    insertatend(&head,10);
    insertatend(&head,20);
    printlist(head);

    return 0;
}