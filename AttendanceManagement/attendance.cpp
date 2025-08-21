#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

#define MAX_DATA_SIZE 500
#define MAX_USER_SIZE 100
#define MAX_DAYS_SIZE 7

#define SCORE_GOLD 50
#define SCORE_SILVER 30

#define GRADE_NORMAL 0
#define GRADE_GOLD 1
#define GRADE_SILVER 2  // 기존 GRAGE_SILVER 오타 수정

struct Node {
    string w;
    string wk;
};

enum Day { Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday, InvalidDay };

class AttendanceManager {
private:
    Node nodes[MAX_DATA_SIZE];
    map<string, int> userIds;
    int id_cnt = 0;

    int dat[MAX_USER_SIZE][MAX_DAYS_SIZE] = { 0 };
    int points[MAX_USER_SIZE] = { 0 };
    int grade[MAX_USER_SIZE] = { 0 };
    string names[MAX_USER_SIZE];

    int wed[MAX_USER_SIZE] = { 0 };
    int weeken[MAX_USER_SIZE] = { 0 };

    Day stringToDay(const string& wk) {
        if (wk == "monday") return Monday;
        else if (wk == "tuesday") return Tuesday;
        else if (wk == "wednesday") return Wednesday;
        else if (wk == "thursday") return Thursday;
        else if (wk == "friday") return Friday;
        else if (wk == "saturday") return Saturday;
        else if (wk == "sunday") return Sunday;
        else return InvalidDay;
    }

    struct GradeInfo {
        int gradeId;
        string gradeName;
        int minScore;
    };

    vector<GradeInfo> gradeInfos = {
        {1, "GOLD", SCORE_GOLD},
        {2, "SILVER", SCORE_SILVER},
        {0, "NORMAL", 0}
    };

public:
    int getUserId(const string& username) const {
        auto it = userIds.find(username);
        if (it != userIds.end())
            return it->second;
        return -1; // 없는 사용자면 -1 반환
    }

    int getDat(int userId, Day day) const {
        if (userId > 0 && userId < MAX_USER_SIZE && day >= Monday && day <= Sunday)
            return dat[userId][day];
        return -1;
    }

    int getPoints(int userId) const {
        if (userId > 0 && userId < MAX_USER_SIZE)
            return points[userId];
        return -1;
    }

    int getGrade(int userId) const {
        if (userId > 0 && userId < MAX_USER_SIZE)
            return grade[userId];
        return -1;
    }

    int getWed(int userId) const {
        if (userId > 0 && userId < MAX_USER_SIZE)
            return wed[userId];
        return -1;
    }

    int getWeeken(int userId) const {
        if (userId > 0 && userId < MAX_USER_SIZE)
            return weeken[userId];
        return -1;
    }

    void getData() {
        ifstream fin{ "../AttendanceManagement/attendance_weekday_500.txt" };
        if (!fin) {
            cerr << "Failed to open file\n";
            return;
        }
        for (int i = 0; i < MAX_DATA_SIZE; i++)
            fin >> nodes[i].w >> nodes[i].wk;
        fin.close();
    }

    void calculateScore(const string& w, const string& wk) {
        // ID 부여
        if (userIds.count(w) == 0) {
            userIds.insert({ w, ++id_cnt });
            names[id_cnt] = w;
        }
        int id = userIds[w];

        Day day = stringToDay(wk);
        if (day == InvalidDay) return;  // 유효하지 않은 요일 처리

        dat[id][day] += 1;

        switch (day) {
        case Monday:
            points[id]++;
            break;
        case Tuesday:
            points[id]++;
            break;
        case Wednesday:
            points[id] += 3;
            wed[id]++;
            break;
        case Thursday:
            points[id]++;
            break;
        case Friday:
            points[id]++;
            break;
        case Saturday:
            points[id] += 2;
            weeken[id]++;
            break;
        case Sunday:
            points[id] += 2;
            weeken[id]++;
            break;
        default:
            break;
        }
    }

    void getScore() {
        for (int i = 0; i < MAX_DATA_SIZE; i++)
            calculateScore(nodes[i].w, nodes[i].wk);
    }

    void addBonusPoints() {
        for (int i = 1; i <= id_cnt; i++) {
            if (dat[i][Wednesday] > 9) {
                points[i] += 10;
            }
            if (dat[i][Saturday] + dat[i][Sunday] > 9) {
                points[i] += 10;
            }
        }
    }

    void judgeGrade() {
        sort(gradeInfos.begin(), gradeInfos.end(), [](const GradeInfo& a, const GradeInfo& b) {
            return a.minScore > b.minScore;
            });

        for (int i = 1; i <= id_cnt; i++) {
            grade[i] = GRADE_NORMAL;
            for (const auto& gi : gradeInfos) {
                if (points[i] >= gi.minScore) {
                    grade[i] = gi.gradeId;
                    break;  // 가장 높은 등급 한번만 부여
                }
            }
        }
    }

    void displayResults() {
        for (int i = 1; i <= id_cnt; i++) {
            cout << "NAME : " << names[i] << ", ";
            cout << "POINT : " << points[i] << ", ";
            cout << "GRADE : ";

            if (grade[i] == GRADE_GOLD) {
                cout << "GOLD\n";
            }
            else if (grade[i] == GRADE_SILVER) {
                cout << "SILVER\n";
            }
            else {
                cout << "NORMAL\n";
            }
        }

        cout << "\nRemoved player\n";
        cout << "==============\n";
        for (int i = 1; i <= id_cnt; i++) {
            if (grade[i] != GRADE_GOLD && grade[i] != GRADE_SILVER && wed[i] == 0 && weeken[i] == 0) {
                cout << names[i] << "\n";
            }
        }
    }

    void setGradeThreshold(int gradeId, int newMinScore) {
        for (auto& gi : gradeInfos) {
            if (gi.gradeId == gradeId) {
                gi.minScore = newMinScore;
                break;
            }
        }
    }

    vector<GradeInfo> getGradeInfos()
    {
        return gradeInfos;
    }

    void addGrade(int gradeId, const string& gradeName, int minScore) {
        gradeInfos.push_back({ gradeId, gradeName, minScore });
    }
};

