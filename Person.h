#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <iostream>

class Person {
private:
    int id;
    std::string firstName;
    std::string lastName;
    int birthYear;

public:
    Person() : id(0), firstName(""), lastName(""), birthYear(0) {}
    
    Person(int id, const std::string& first, const std::string& last, int year) : id(id), firstName(first), lastName(last), birthYear(year) {}
    
    int GetId() const { 
        return id; 
    }
    
    std::string GetFirstName() const { 
        return firstName; 
    }

    std::string GetLastName() const { 
        return lastName; 
    }

    int GetBirthYear() const { 
        return birthYear; 
    }
    
    bool operator==(const Person& other) const {
        return id == other.id;
    }
    
    bool operator!=(const Person& other) const {
        return id != other.id;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Person& p) {
        os << p.firstName << " " << p.lastName << " (ID:" << p.id << ")";
        return os;
    }
};

class Student : public Person {
private:
    int studentId;
    std::string group;
    
public:
    Student() : Person(), studentId(0), group("") {}
    
    Student(int id, const std::string& first, const std::string& last, int year, int studentId, const std::string& group)
        : Person(id, first, last, year), studentId(studentId), group(group) {}
    
    int GetStudentId() const { 
        return studentId; 
    }
    std::string GetGroup() const { 
        return group; 
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Student& s) {
        os << s.GetFirstName() << " " << s.GetLastName() 
           << " (Student ID:" << s.GetStudentId() << ", Group:" << s.GetGroup() << ")";
        return os;
    }
};

class Teacher : public Person {
private:
    int teacherId;
    std::string department;
    
public:
    Teacher() : Person(), teacherId(0), department("") {}
    
    Teacher(int id, const std::string& first, const std::string& last, int year, int teacherId, const std::string& department)
        : Person(id, first, last, year), teacherId(teacherId), department(department) {}
    
    int GetTeacherId() const { 
        return teacherId; 
    }
    
    std::string GetDepartment() const { 
        return department; 
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Teacher& t) {
        os << t.GetFirstName() << " " << t.GetLastName() 
           << " (Teacher ID:" << t.GetTeacherId() << ", Dept:" << t.GetDepartment() << ")";
        return os;
    }
};

#endif