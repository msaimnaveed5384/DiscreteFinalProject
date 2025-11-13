#ifndef ROOM_H
#define ROOM_H
#include <string>
using namespace std;
class Room {
private:
    string roomNumber;
    int capacity;
    string type;  // Lecture room, Lab, Tutorial room etc.
public:
    //Constructors
    Room();
    Room(string roomNumber, int capacity, string type);
    //Getter Functions 
    string getRoomNumber() const;
    int getCapacity() const;
    string getType() const;

    //Setter Functions
    void setCapacity(int newCapacity);
    void setType(string newType);
};
#endif
