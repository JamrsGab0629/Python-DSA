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

void deletehead(Node **head){
    if (*head == nullptr){
        return;
    }
    Node *temp = *head;
    *head = (*head)->next;
    delete temp;

}

void deleteatend(Node **head){
    if ((*head)->next == nullptr){
        delete *head;
        *head = nullptr;
        return;
    }
   
    Node *temp = *head;
    while(temp->next->next != nullptr){
        temp = temp->next;
    }
    delete temp->next;
    temp->next = nullptr;
}

void removemid(Node **head){

    if ((*head)->next == nullptr){
        delete *head;
        *head = nullptr;
        return;
    }

    int count = 0;
    Node *temp = *head;
    while(temp != nullptr){
        temp = temp->next;
        count++;
    }
    int mid = count / 2;
    Node *curr = *head;
    for(int i = 0; i < mid - 1; i++){
        curr = curr->next;
    }
    Node *old = curr->next;
    curr->next = old->next;
    delete old;
   
    
}
int main(){
    Node *head = nullptr;
    insertatbeginning(&head , 10);
    insertatend(&head,11);
    insertatend(&head,20);

    printlist(head);
    std::cout << "\n";
    removemid(&head);

    printlist(head);
    return 0;
}