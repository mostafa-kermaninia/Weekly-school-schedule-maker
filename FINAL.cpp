#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

#define NOT_FOUND ""
#define ALL_WEEK_DAYS 7
#define FIRST_CLASS 1
#define SECOND_CLASS 2
#define NUM_OF_CLASSES 2

struct Teacher
{
    string name;
    vector<string> freeDays;
    vector<string> coursesNames;
};
struct Course
{
    string name;
    pair<string, string> days;
    pair<int, int> validTime;
};
struct PresentedCourse
{
    Course course;
    pair<int, int> startAndEndTime;
    string teacherName;
    int classNum;
};

//   *** GET INPUT ***
string getWord()
{
    string inputedWord;
    cin >> inputedWord;
    return inputedWord;
}
int passedMinutes(string inputedTime)
{
    int passedHours = stoi(inputedTime.substr(0, 2));
    int remainMinutes = stoi(inputedTime.substr(3));
    int passedMinutes = ((60 * passedHours) + remainMinutes);
    return passedMinutes;
}
bool isFirstWordComesFirst(string word1, string word2)
{
    for (int i = 0; i < min(word1.size(), word2.size()); i++)
    {
        if (word1[i] < word2[i])
            return true;
        else if (word1[i] > word2[i])
            return false;
    }
    if (word1.size() > word2.size())
        return false;
    else if (word1.size() < word2.size())
        return true;
    else
        return false;
}
Teacher recieveTeacherInfo()
{
    // <teacher_name> <number_of_free_days> <free_days> <number_of_courses> <courses>
    Teacher inputedTeacher;

    inputedTeacher.name = getWord();
    int numOfFreeDays = stoi(getWord());
    for (int i = 0; i < numOfFreeDays; i++)
        inputedTeacher.freeDays.push_back(getWord());
    int numOfCourses = stoi(getWord());
    for (int i = 0; i < numOfCourses; i++)
        inputedTeacher.coursesNames.push_back(getWord());

    return inputedTeacher;
}
Course recieveCourseInfo()
{
    // <course_name> <day1> <day2> <start_time> <end_time>
    Course inputedCourse;

    inputedCourse.name = getWord();
    inputedCourse.days.first = getWord();
    inputedCourse.days.second = getWord();
    inputedCourse.validTime.first = passedMinutes(getWord());
    inputedCourse.validTime.second = passedMinutes(getWord());

    return inputedCourse;
}
vector<Teacher> buildTeachersList(int numOfTeachers)
{
    vector<Teacher> teachersList;

    for (int i = 0; i < numOfTeachers; i++)
    {
        Teacher inputedTeacher = recieveTeacherInfo();
        // Put new teacher in alphabetical order
        int selectedIndex = 0;
        while (selectedIndex < teachersList.size() &&
               isFirstWordComesFirst(teachersList[selectedIndex].name, inputedTeacher.name))
            selectedIndex++;
        teachersList.insert(teachersList.begin() + selectedIndex, inputedTeacher);
    }

    return teachersList;
}
vector<Course> buildCoursesList(int numOfCourses)
{
    vector<Course> coursesList;

    for (int i = 0; i < numOfCourses; i++)
    {
        Course inputedCourse = recieveCourseInfo();
        // Put new teacher in alphabetical order
        int selectedIndex = 0;
        while (selectedIndex < coursesList.size() &&
               isFirstWordComesFirst(coursesList[selectedIndex].name, inputedCourse.name))
            selectedIndex++;
        coursesList.insert(coursesList.begin() + selectedIndex, inputedCourse);
    }

    return coursesList;
}
void getInput(vector<Teacher> &teachersInfo, vector<Course> &coursesInfo)
{
    int numOfTeachers, numOfCourses;
    cin >> numOfTeachers;
    teachersInfo = buildTeachersList(numOfTeachers);
    cin >> numOfCourses;
    coursesInfo = buildCoursesList(numOfCourses);
}

//   *** DO PROCESS ***
bool isValidTime(pair<int, int> classTime, pair<int, int> availableTime)
{
    if (classTime.first >= availableTime.first && classTime.second <= availableTime.second)
        return true;
    return false;
}
bool areValidDaysAndTime(pair<string, string> days, pair<int, int> consideredClassTime, int classNum, vector<PresentedCourse> presentedCourses)
{
    for (PresentedCourse pCourse : presentedCourses)
        if (pCourse.classNum == classNum &&
            (days.first == pCourse.course.days.first ||
             days.first == pCourse.course.days.second ||
             days.second == pCourse.course.days.first ||
             days.second == pCourse.course.days.second) &&
            consideredClassTime == pCourse.startAndEndTime)
            return false;
    return true;
}
bool checkCourseAndClassValidity(Course course, string consideredDay, pair<int, int> consideredClassTime, vector<PresentedCourse> presentedCourses, int classNum)
{
    // check if the course can be presented in considered day and time
    if (isValidTime(consideredClassTime, course.validTime))
        if (course.days.first == consideredDay || course.days.second == consideredDay)
            if (areValidDaysAndTime(course.days, consideredClassTime, classNum, presentedCourses))
                return true;
    return false;
}
bool isFreeTimeAndDidntPresentBefore(pair<int, int> consideredClassTime, pair<string, string> consideredDays, Teacher teacher, Course course, vector<PresentedCourse> presentedCourses, int classNum)
{
    for (PresentedCourse pCourse : presentedCourses)
        if (teacher.name == pCourse.teacherName &&
                ((consideredDays.first == pCourse.course.days.first ||
                  consideredDays.first == pCourse.course.days.second ||
                  consideredDays.second == pCourse.course.days.first ||
                  consideredDays.second == pCourse.course.days.second) &&
                 consideredClassTime == pCourse.startAndEndTime) ||
            (classNum == pCourse.classNum && course.name == pCourse.course.name))
            return false;
    return true;
}
bool checkTimeAndTeacherValidity(Teacher teacher, Course course, pair<string, string> consideredDays, pair<int, int> consideredClassTime, vector<PresentedCourse> presentedCourses, int classNum)
{
    // check if the teacher 1-can teach selected course 2-can come in considered day and time  3-its not reserved before 4-he didnt presente that course in that class before
    if ((find(teacher.freeDays.begin(), teacher.freeDays.end(), consideredDays.first) != teacher.freeDays.end()) &&
        (find(teacher.freeDays.begin(), teacher.freeDays.end(), consideredDays.second) != teacher.freeDays.end()))
        if (find(teacher.coursesNames.begin(), teacher.coursesNames.end(), course.name) != teacher.coursesNames.end())
            if (isFreeTimeAndDidntPresentBefore(consideredClassTime, consideredDays, teacher, course, presentedCourses, classNum))
                return true;
    return false;
}
PresentedCourse FindCourseAndTeacher(string consideredDay, pair<int, int> consideredClassTime, vector<Teacher> teachersInfo, vector<Course> coursesInfo, vector<PresentedCourse> presentedCourses, int classNum)
{
    PresentedCourse newValidCourse;
    for (Course course : coursesInfo)
        if (checkCourseAndClassValidity(course, consideredDay, consideredClassTime, presentedCourses, classNum))
        {
            // check if we have a teacher in that time
            Teacher selectedTeacher;
            selectedTeacher.name = NOT_FOUND;
            int minFreeDays = ALL_WEEK_DAYS;
            for (Teacher teacher : teachersInfo)
            {
                pair<string, string> consideredDays = course.days;
                if (checkTimeAndTeacherValidity(teacher, course, consideredDays, consideredClassTime, presentedCourses, classNum))
                    if (teacher.freeDays.size() < minFreeDays)
                    {
                        selectedTeacher = teacher;
                        minFreeDays = teacher.freeDays.size();
                    }
            }
            if (selectedTeacher.name != NOT_FOUND)
            {
                newValidCourse = {course, consideredClassTime, selectedTeacher.name, classNum};
                return newValidCourse;
            }
        }
    newValidCourse.teacherName = NOT_FOUND;
    return newValidCourse;
}
vector<PresentedCourse> buildSchedule(vector<Teacher> teachersInfo, vector<Course> coursesInfo, vector<PresentedCourse> &presentedCourses)
{
    vector<pair<int, int>> classTimes = {
        {passedMinutes("07:30"), passedMinutes("09:00")},
        {passedMinutes("09:30"), passedMinutes("11:00")},
        {passedMinutes("11:30"), passedMinutes("13:00")},
    };
    vector<string> weekDays = {"Saturday", "Sunday", "Monday", "Tuesday", "Wednesday"};

    for (int classNum = FIRST_CLASS; classNum <= NUM_OF_CLASSES; classNum++)
        for (string day : weekDays)
            for (pair<int, int> classTime : classTimes)
            {
                // search for valid teacher and course
                PresentedCourse newValidCourse = FindCourseAndTeacher(day, classTime, teachersInfo, coursesInfo, presentedCourses, classNum);
                if (newValidCourse.teacherName != NOT_FOUND)
                    presentedCourses.push_back(newValidCourse);
            }
    return presentedCourses;
}

//   *** PRINT OUTPUT ***
string showTimeDuration(pair<int, int> startAndEndTime)
{
    vector<pair<string, string>> classesTimes = {
        {"07:30", "09:00"},
        {"09:30", "11:00"},
        {"11:30", "13:00"},
    };
    for (int i = 0; i < classesTimes.size(); i++)
        if (passedMinutes(classesTimes[i].first) == startAndEndTime.first)
            return (classesTimes[i].first + " " + classesTimes[i].second);
}
vector<PresentedCourse> sortClassesSchedule(vector<PresentedCourse> schedule)
{
    PresentedCourse temp;
    // sort on courses' name
    for (int i = 0; i < schedule.size(); i++)
        for (int j = i + 1; j < schedule.size(); j++)
            if (isFirstWordComesFirst(schedule[j].course.name, schedule[i].course.name))
            {
                temp = schedule[j];
                schedule[j] = schedule[i];
                schedule[i] = temp;
            }
    return schedule;
}
string findTeacherAndTimeOfCourse(Course curCourse, vector<PresentedCourse> schedule, int classNum)
{
    string result = "Not Found";
    for (PresentedCourse pCourse : schedule)
        if (pCourse.course.name == curCourse.name && pCourse.classNum == classNum)
            result = pCourse.teacherName + ": " + showTimeDuration(pCourse.startAndEndTime);
    return result;
}
void printOutput(vector<PresentedCourse> schedule, vector<Course> coursesInfo)
{
    vector<PresentedCourse> sortedSchedule = sortClassesSchedule(schedule);
    string curCourseName;
    for (Course Course : coursesInfo)
    {
        cout << Course.name << endl;
        cout << findTeacherAndTimeOfCourse(Course, schedule, FIRST_CLASS) << endl;
        cout << findTeacherAndTimeOfCourse(Course, schedule, SECOND_CLASS) << endl;
    }
}

//  *** MAIN ***
int main()
{
    vector<Teacher> teachersInfo;
    vector<Course> coursesInfo;
    vector<PresentedCourse> presentedCourses;

    getInput(teachersInfo, coursesInfo);
    vector<PresentedCourse> Schedule = buildSchedule(teachersInfo, coursesInfo, presentedCourses);
    printOutput(Schedule, coursesInfo);

    return 0;
}
