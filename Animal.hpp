#include <iostream>
#include <string>

using namespace std;

class Animal{
    protected:
        size_t age;
        const string name;
    public:
        Animal(size_t age, string new_name) : name(new_name){
            this->age = age;    
        }
        size_t get_age() {return age;}
        
        string get_name() {return name;}
    
        friend ostream& operator<<(ostream& output, const Animal& myanimal) {
            output << "Name: " << myanimal.name << ". Age: " << myanimal.age << endl;
            return output;
        }

        bool operator< (const Animal& animal1) const{
            return age < animal1.age;
        }
        bool operator== (const Animal& animal1) const{
            return age == animal1.age && name == animal1.name;
        }
        bool operator<= (const Animal& animal1) const{
            return age <= animal1.age;
        }
        bool operator!=(const Animal& animal1) const{
            return age == animal1.age && name == animal1.name;
        }
        bool operator> (const Animal& animal1) const{
            return age > animal1.age;
        }
        bool operator>= (const Animal& animal1) const{
            return age >= animal1.age;
        }
        Animal operator=(const Animal& animal1) const{
            return Animal(animal1.age, animal1.name);
        }
};