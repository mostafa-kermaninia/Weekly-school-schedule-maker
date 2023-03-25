#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

#define DEFAULT "z"
#define NOTFOUND 0
#define ALL_WEEK_DAYS 7
#define FIRST_CLASS 1
#define SECOND_CLASS 2

struct Teacher
{
    string name;
    int numOfFreeDays;
    vector<string> freeDays; // it will have (numOfFreeDays) elements
    int numOfCourses;
    vector<string> courses; // it will have (numOfCourses) elements
};
struct Course
{
    string name;
    pair<string, string> days;
    pair<int, int> startAndEndTime;
};
struct PresentedCourse
{
    string courseName;
    string teacherName;
    int classNum;
    pair<int, int> startAndEndTime;
    pair<string, string> days;
};
string showTimeDuration(pair<int, int> startAndEndTime);

//   *** GET INPUT PART ***
string getWord()
{
    string inputedWord;
    cin >> inputedWord;
    return inputedWord;
}
int passedMinutes(string inputedTime)
{
    int PassedMinutes = ((60 * stoi(inputedTime.substr(0, 2))) + stoi(inputedTime.substr(3)));
    return PassedMinutes;
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
    {
        return false;
    }
}

Teacher recieveTeacherInfo()
{
    // <teacher_name> <number_of_free_days> <free_days> <number_of_courses> <courses>
    Teacher inputedTeacher;

    inputedTeacher.name = getWord();
    inputedTeacher.numOfFreeDays = stoi(getWord());
    for (int j = 0; j < inputedTeacher.numOfFreeDays; j++)
    {
        inputedTeacher.freeDays.push_back(getWord());
    }
    inputedTeacher.numOfCourses = stoi(getWord());
    for (int j = 0; j < inputedTeacher.numOfCourses; j++)
    {
        inputedTeacher.courses.push_back(getWord());
    }
    return inputedTeacher;
}
Course recieveCourseInfo()
{
    // <course_name> <day1> <day2> <start_time> <end_time>
    Course inputedCourse;

    inputedCourse.name = getWord();
    inputedCourse.days.first = getWord();
    inputedCourse.days.second = getWord();
    inputedCourse.startAndEndTime.first = passedMinutes(getWord());
    inputedCourse.startAndEndTime.second = passedMinutes(getWord());

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
        while (selectedIndex < teachersList.size() && isFirstWordComesFirst(teachersList[selectedIndex].name, inputedTeacher.name))
        {
            selectedIndex++;
        }
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
        while (selectedIndex < coursesList.size() && isFirstWordComesFirst(coursesList[selectedIndex].name, inputedCourse.name))
        {
            selectedIndex++;
        }
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

//   *** DO PROCESSING PART ***
bool isValidTime(pair<int, int> classTime, pair<int, int> availableTime)
{
    if (classTime.first >= availableTime.first && classTime.second <= availableTime.second)
        return true;
    return false;
}
bool checkIfSelectedBefore(vector<PresentedCourse> presentedCourses, Course courseInfo, Teacher teacherInfo, int classNum, pair<int, int> classTime)
{
    for (int i = 0; i < presentedCourses.size(); i++)
    {
        PresentedCourse curPresented = presentedCourses[i];
        // if teacher can't come or if class has another course on that time
        if ((curPresented.teacherName == teacherInfo.name &&
             curPresented.startAndEndTime == classTime &&
             (curPresented.days.first == courseInfo.days.first || curPresented.days.first == courseInfo.days.second || curPresented.days.second == courseInfo.days.first || curPresented.days.second == courseInfo.days.second)) ||
            (curPresented.classNum == classNum &&
             curPresented.startAndEndTime == classTime &&
             (curPresented.days.first == courseInfo.days.first || curPresented.days.first == courseInfo.days.second || curPresented.days.second == courseInfo.days.first || curPresented.days.second == courseInfo.days.second)))
        {
            return true;
        }
    }
    return false;
}
pair<int, int> presentationTime(Course courseInfo, Teacher teacherInfo, int classNum, vector<PresentedCourse> presentedCourses)
{
    vector<pair<int, int>> classesTimes = {
        {passedMinutes("07:30"), passedMinutes("09:00")},
        {passedMinutes("09:30"), passedMinutes("11:00")},
        {passedMinutes("11:30"), passedMinutes("13:00")},
    };
    pair<int, int> availableTime = {NOTFOUND, NOTFOUND};

    //    if ((find(teacherInfo.freeDays.begin(), teacherInfo.freeDays.end(), courseInfo.days.first) != teacherInfo.freeDays.end()) &&
    //         (find(teacherInfo.freeDays.begin(), teacherInfo.freeDays.end(), courseInfo.days.second) != teacherInfo.freeDays.end()))
    //     {
    //         for (int i = 0; i < classesTimes.size(); i++)
    //         {
    //             bool isSelectedBefore = checkIfSelectedBefore(presentedCourses, courseInfo.name, teacherInfo.name, classNum, {classesTimes[i].first, classesTimes[i].second});
    //             if (!isSelectedBefore && classesTimes[i].first >= courseInfo.startAndEndTime.first && classesTimes[i].second <= courseInfo.startAndEndTime.second)
    //             {
    //                 availableTime = classesTimes[i];
    //                 break;
    //             }
    //         }
    //     }

    if ((find(teacherInfo.freeDays.begin(), teacherInfo.freeDays.end(), courseInfo.days.first) != teacherInfo.freeDays.end()) &&
        (find(teacherInfo.freeDays.begin(), teacherInfo.freeDays.end(), courseInfo.days.second) != teacherInfo.freeDays.end()) &&
        (find(teacherInfo.courses.begin(), teacherInfo.courses.end(), courseInfo.name) != teacherInfo.courses.end()))
    {
        for (int i = 0; i < classesTimes.size(); i++)
        {
            bool isSelectedBefore = checkIfSelectedBefore(presentedCourses, courseInfo, teacherInfo, classNum, {classesTimes[i].first, classesTimes[i].second});
            if (!isSelectedBefore && classesTimes[i].first >= courseInfo.startAndEndTime.first && classesTimes[i].second <= courseInfo.startAndEndTime.second)
            {
                availableTime = classesTimes[i];
                break;
            }
        }
    }

    return availableTime;
}

vector<PresentedCourse> buildSchedule(vector<Teacher> teachersInfo, vector<Course> coursesInfo, vector<PresentedCourse> &presentedCourses, int classNum)
{
    for (int courseCount = 0; courseCount < coursesInfo.size(); courseCount++)
    {
        int minPresentDays = ALL_WEEK_DAYS;
        Teacher selectedTeacher;
        selectedTeacher.name = DEFAULT;
        Course curCourse = coursesInfo[courseCount];

        for (int teacherCount = teachersInfo.size() - 1; teacherCount >= 0; teacherCount--)
        {
            Teacher curTeacher = teachersInfo[teacherCount];
            pair<int, int> classTime = presentationTime(curCourse, curTeacher, classNum, presentedCourses);
            cout << curCourse.name << curTeacher.name << classTime.first << classNum << "kk\n";
            if ((classTime.first != NOTFOUND))
            {
                cout<<"lololo";
                if (teachersInfo[teacherCount].freeDays.size() <= minPresentDays )
                {
                    cout<<"ffffffffffffffuck";
                    minPresentDays = teachersInfo[teacherCount].freeDays.size();
                    selectedTeacher = curTeacher;
                }
            }
        }
        if (selectedTeacher.name != DEFAULT)
        {
            cout << "pollll" << curCourse.name << selectedTeacher.name << showTimeDuration(presentationTime(curCourse, selectedTeacher, classNum, presentedCourses)) << classNum << " polop \n\n";

            presentedCourses.push_back({curCourse.name, selectedTeacher.name, classNum, presentationTime(curCourse, selectedTeacher, classNum, presentedCourses), curCourse.days});
        }
        else
        {
            presentedCourses.push_back({curCourse.name, DEFAULT, classNum, {0, 0}});
        }
    }
    return presentedCourses;
}

//   *** PRINT OUTPUT PART ***
string showTimeDuration(pair<int, int> startAndEndTime)
{
    vector<pair<string, string>> classesTimes = {
        {"07:30", "09:00"},
        {"09:30", "11:00"},
        {"11:30", "13:00"},
    };
    for (int i = 0; i < classesTimes.size(); i++)
    {
        if (passedMinutes(classesTimes[i].first) == startAndEndTime.first)
        {
            return (classesTimes[i].first + " " + classesTimes[i].second);
        }
    }
}
vector<PresentedCourse> sortClassesSchedule(vector<PresentedCourse> allClassesSchedule)
{
    vector<PresentedCourse> schedule = allClassesSchedule;
    PresentedCourse temp;
    // sort on courses' name
    for (int i = 0; i < schedule.size(); i++)
    {
        for (int j = i + 1; j < schedule.size(); j++)
        {
            if (isFirstWordComesFirst(schedule[j].courseName, schedule[i].courseName))
            {
                temp = schedule[j];
                schedule[j] = schedule[i];
                schedule[i] = temp;
            }
        }
    }
    return schedule;
}
void printOutput(vector<PresentedCourse> allClassesSchedule)
{
    vector<PresentedCourse> sortedSchedule = sortClassesSchedule(allClassesSchedule);
    string curCourse;
    for (int courseCount = 0; courseCount < sortedSchedule.size(); courseCount++)
    {
        if (curCourse != sortedSchedule[courseCount].courseName)
        {
            curCourse = sortedSchedule[courseCount].courseName;
            cout << sortedSchedule[courseCount].courseName << endl;
        }
        if (sortedSchedule[courseCount].teacherName != DEFAULT)
        {
            cout << sortedSchedule[courseCount].teacherName << ": " << showTimeDuration(sortedSchedule[courseCount].startAndEndTime) << endl;
        }
        else
        {
            cout << "Not Found\n";
        }
    }
}

int main()
{
    vector<Teacher> teachersInfo;
    vector<Course> coursesInfo;
    vector<PresentedCourse> presentedCourses;

    getInput(teachersInfo, coursesInfo);

    // cout << teachersInfo[1].name;
    // cout << teachersInfo[2].name;
    // cout << teachersInfo[1].numOfFreeDays;
    // cout << teachersInfo[1].numOfFreeDays;
    // cout << teachersInfo[1].numOfCourses;
    // cout << teachersInfo[1].numOfCourses;

    vector<PresentedCourse> firstClassSchedule;
    firstClassSchedule = buildSchedule(teachersInfo, coursesInfo, presentedCourses, FIRST_CLASS);
    vector<PresentedCourse> allClassesSchedule = buildSchedule(teachersInfo, coursesInfo, presentedCourses, SECOND_CLASS);
    printOutput(allClassesSchedule);

    return 0;
}
