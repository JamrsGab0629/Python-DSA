#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <stdexcept>


class Todo{
   private:
    std::vector<std::string> tasks;

   public:
   
   
   void add_task(std::string task){
      
      tasks.push_back(task);      

   }
   void remove(size_t index){
      if (index > tasks.size()) throw std::out_of_range("The index task was not existing");
         tasks.erase(tasks.begin() + index); 
   }
   void show_all() const{

      if (tasks.empty()){
         std::cout << "no tasks yet";
         return;
      }
      for(std::string i : tasks){
            std::cout << i << "\n";

      }
   }
   
   void menu(){
      std::cout << "Menu \n";

      std::cout << "1.ADD \n";
      std::cout << "2.Remove \n";
      std::cout << "3.See all \n";
      std::cout << "4.Exit \n";

   }
   size_t size () const  {
      return tasks.size();
   }

};
int main(){
   Todo todo;  
   int choice = 0;
   do{
     
      todo.menu();
      std::cout << "Enter your choice";
      std::cin >> choice;
      if (choice == 1){
         std::string task;
         std::cin >> task;
         todo.add_task(task);
      }else if (choice == 2){
       size_t index;
       std::cout << "enter what index you want to remove";
       std::cin >> index;

       todo.remove(index - 1);
      }else if (choice == 3){
         todo.show_all();
      }else if (choice > 4){
         throw std::invalid_argument("Just enter the choice on the menu");
      }
      }while(choice != 4);
     
}