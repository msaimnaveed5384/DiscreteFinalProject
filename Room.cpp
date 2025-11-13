#include "Room.h"
using namespace std;

// Constructors
Room::Room() 
{
    roomNumber = "";
    capacity = 0;
    type = "";
}

Room::Room(string roomNumber, int capacity, string type) 
{
    this->roomNumber = roomNumber;
    this->capacity = capacity;
    this->type = type;
}

// Getter Functions
string Room::getRoomNumber() const 
{
    return roomNumber;
}

int Room::getCapacity() const 
{
    return capacity;
}

string Room::getType() const 
{
    return type;
}

// Setter Functions
void Room::setCapacity(int newCapacity) 
{
    capacity = newCapacity;
}

void Room::setType(string newType) 
{
    type = newType;
}
