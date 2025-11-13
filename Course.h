#ifndef COURSE_H
#define COURSE_H
#include <string>
using namespace std;
class Course {
private:
    string code;
    string title;
    int creditHours;
public:

	//Constructors
    Course();
    Course(string code, string title, int creditHours);

	//Getter Functions
    string getCode() const;
    string getTitle() const;
    int getCreditHours() const;

	//Setter Functions
    void setTitle(string newTitle);
    void setCreditHours(int newCH);
};
#endif

