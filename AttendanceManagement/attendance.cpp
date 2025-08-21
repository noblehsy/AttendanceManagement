#pragma once

#include "IAttendanceManager.h"
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

#define MAX_DATA_SIZE 500
#define MAX_USER_SIZE 100
#define MAX_DAYS_SIZE 7

#define SCORE_GOLD 50
#define SCORE_SILVER 30

#define GRADE_NORMAL 0
#define GRADE_GOLD 1
#define GRADE_SILVER 2

struct Node {
    std::string w;
    std::string wk;
};

class AttendanceManager : public IAttendanceManager {
private:
    Node nodes[MAX_DATA_SIZE];
    std::map<std::string, int> userIds;
    int id_cnt = 0;

    int dat[MAX_USER_SIZE][MAX_DAYS_SIZE] = { 0 };
    int points[MAX_USER_SIZE] = { 0 };
    int grade[MAX_USER_SIZE] = { 0 };
    std::string names[MAX_USER_SIZE];

    int wed[MAX_USER_SIZE] = { 0 };
    int weeken[MAX_USER_SIZE] = { 0 };

    std::vector<GradeInfo> gradeInfos = {
        { GRADE_GOLD, "GOLD", SCORE_GOLD },
        { GRADE_SILVER, "SILVER", SCORE_SILVER },
        { GRADE_NORMAL, "NORMAL", 0 }
    };

    Day stringToDay(const std::string& wk) {
        if (wk == "monday") return Monday;
        if (wk == "tuesday") return Tuesday;
        if (wk == "wednesday") return Wednesday;
        if (wk == "thursday") return Thursday;
        if (wk == "friday") return Friday;
        if (wk == "saturday") return Saturday;
        if (wk == "sunday") return Sunday;
        return InvalidDay;
    }

public:
    int getUserId(const std::string& username) const override {
        auto it = userIds.find(username);
        return (it != userIds.end()) ? it->second : -1;
    }

    int getDat(int userId, Day day) const override {
        if (userId > 0 && userId < MAX_USER_SIZE && day >= Monday && day <= Sunday)
            return dat[userId][day];
        return -1;
    }

    int getPoints(int userId) const override {
        return (userId > 0 && userId < MAX_USER_SIZE) ? points[userId] : -1;
    }

    int getGrade(int userId) const override {
        return (userId > 0 && userId < MAX_USER_SIZE) ? grade[userId] : -1;
    }

    int getWed(int userId) const override {
        return (userId > 0 && userId < MAX_USER_SIZE) ? wed[userId] : -1;
    }

    int getWeeken(int userId) const override {
        return (userId > 0 && userId < MAX_USER_SIZE) ? weeken[userId] : -1;
    }

    void getData() override {
        std::ifstream fin{ "../AttendanceManagement/attendance_weekday_500.txt" };
        if (!fin) {
            std::cerr << "Failed to open file\n";
            return;
        }
        for (int i = 0; i < MAX_DATA_SIZE; i++)
            fin >> nodes[i].w >> nodes[i].wk;
        fin.close();
    }

    void calculateScore(const std::string& w, const std::string& wk) override {
        if (userIds.count(w) == 0) {
            userIds[w] = ++id_cnt;
            names[id_cnt] = w;
        }
        int id = userIds[w];
        Day day = stringToDay(wk);
        if (day == InvalidDay) return;

        dat[id][day]++;

        switch (day) {
        case Monday: case Tuesday: case Thursday: case Friday: points[id]++; break;
        case Wednesday: points[id] += 3; wed[id]++; break;
        case Saturday: case Sunday: points[id] += 2; weeken[id]++; break;
        default: break;
        }
    }

    void getScore() override {
        for (int i = 0; i < MAX_DATA_SIZE; i++)
            calculateScore(nodes[i].w, nodes[i].wk);
    }

    void addBonusPoints() override {
        for (int i = 1; i <= id_cnt; i++) {
            if (dat[i][Wednesday] > 9) points[i] += 10;
            if (dat[i][Saturday] + dat[i][Sunday] > 9) points[i] += 10;
        }
    }

    void judgeGrade() override {
        std::sort(gradeInfos.begin(), gradeInfos.end(), [](const GradeInfo& a, const GradeInfo& b) {
            return a.minScore > b.minScore;
            });

        for (int i = 1; i <= id_cnt; i++) {
            grade[i] = GRADE_NORMAL;
            for (const auto& gi : gradeInfos) {
                if (points[i] >= gi.minScore) {
                    grade[i] = gi.gradeId;
                    break;
                }
            }
        }
    }

    void displayResults() override {
        for (int i = 1; i <= id_cnt; i++) {
            std::cout << "NAME : " << names[i] << ", ";
            std::cout << "POINT : " << points[i] << ", ";
            std::cout << "GRADE : " << getGradeNameById(grade[i]) << "\n";
        }

        std::cout << "\nRemoved player\n==============\n";
        for (int i = 1; i <= id_cnt; i++) {
            if (grade[i] == GRADE_NORMAL && wed[i] == 0 && weeken[i] == 0)
                std::cout << names[i] << "\n";
        }
    }

    std::string getGradeNameById(int gradeId) {
        for (const auto& g : gradeInfos)
            if (g.gradeId == gradeId)
                return g.gradeName;
        return "UNKNOWN";
    }

    void setGradeThreshold(int gradeId, int newMinScore) override {
        for (auto& gi : gradeInfos)
            if (gi.gradeId == gradeId) {
                gi.minScore = newMinScore;
                break;
            }
    }

    void addGrade(int gradeId, const std::string& gradeName, int minScore) override {
        gradeInfos.push_back({ gradeId, gradeName, minScore });
    }

    std::vector<GradeInfo> getGradeInfos() override {
        return gradeInfos;
    }
};
