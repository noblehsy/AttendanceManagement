#pragma once
#include "IAttendanceManager.h"
#include <map>
#include <string>
#include <vector>

#define MAX_DATA_SIZE 500
#define MAX_USER_SIZE 100
#define MAX_DAYS_SIZE 7

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

    std::vector<GradeInfo> gradeInfos;

    AttendanceManager();
public:
    static AttendanceManager& getInstance();
    Day stringToDay(const std::string& wk);

    int getUserId(const std::string& username) const override;
    int getDat(int userId, Day day) const override;
    int getPoints(int userId) const override;
    int getGrade(int userId) const override;
    int getWed(int userId) const override;
    int getWeeken(int userId) const override;

    void calculateScore(const std::string& w, const std::string& wk) override;
    void addBonusPoints() override;
    void judgeGrade() override;
    void displayResults() override;

    void setGradeThreshold(int gradeId, int newMinScore) override;
    void addGrade(int gradeId, const std::string& gradeName, int minScore) override;
    std::vector<GradeInfo> getGradeInfos() override;

    std::string getGradeNameById(int gradeId) override;
};