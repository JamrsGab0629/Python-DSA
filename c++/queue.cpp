#include <iostream>
#include <stdexcept>
struct Node 
{
    int data;
    Node *next;
};

void queue(Node **first,int val){
    Node *newnode = new Node{val,nullptr};

    if (*first == nullptr){
        newnode->next = *first;
        *first = newnode;
        return;
    }
    Node *temp = *first;
    while(temp->next != nullptr){
        temp = temp->next;
    }
    temp->next = newnode;
   
}

void dequeue(Node **first){
    if (*first == nullptr){
        return;
    }
    Node *temp = *first;
    *first = (*first)->next;
    delete temp;
    temp = nullptr;

}

void printqueue(Node *first){
    if(first == nullptr)
    {
        return;
    }

    Node *temp = first;
    while (temp != nullptr)
    {
        std::cout << temp->data;
        temp = temp->next;
    }
    
}
int peek(Node *first){
    if (first == nullptr){
        return 1;
    }
    return first->data;
}

int main(){

    Node *first = nullptr;

    queue(&first,10);
    queue(&first,20);
   
   
   std::cout << peek(first);
    return 0;
}