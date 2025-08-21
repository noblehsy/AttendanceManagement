#include "AttendanceManager.h"
#include <fstream>
#include <iostream>
#include <algorithm>

#define SCORE_GOLD 50
#define SCORE_SILVER 30

#define GRADE_NORMAL 0
#define GRADE_GOLD 1
#define GRADE_SILVER 2

AttendanceManager::AttendanceManager() {
    gradeInfos = {
        { GRADE_GOLD, "GOLD", SCORE_GOLD },
        { GRADE_SILVER, "SILVER", SCORE_SILVER },
        { GRADE_NORMAL, "NORMAL", 0 }
    };
}

AttendanceManager& AttendanceManager::getInstance() {
    static AttendanceManager instance;
    return instance;
}

Day AttendanceManager::stringToDay(const std::string& wk) {
    if (wk == "monday") return Monday;
    if (wk == "tuesday") return Tuesday;
    if (wk == "wednesday") return Wednesday;
    if (wk == "thursday") return Thursday;
    if (wk == "friday") return Friday;
    if (wk == "saturday") return Saturday;
    if (wk == "sunday") return Sunday;
    return InvalidDay;
}

int AttendanceManager::getUserId(const std::string& username) const {
    auto it = userIds.find(username);
    return (it != userIds.end()) ? it->second : -1;
}

int AttendanceManager::getDat(int userId, Day day) const {
    if (userId > 0 && userId < MAX_USER_SIZE && day >= Monday && day <= Sunday)
        return dat[userId][day];
    return -1;
}

int AttendanceManager::getPoints(int userId) const {
    return (userId > 0 && userId < MAX_USER_SIZE) ? points[userId] : -1;
}

int AttendanceManager::getGrade(int userId) const {
    return (userId > 0 && userId < MAX_USER_SIZE) ? grade[userId] : -1;
}

int AttendanceManager::getWed(int userId) const {
    return (userId > 0 && userId < MAX_USER_SIZE) ? wed[userId] : -1;
}

int AttendanceManager::getWeeken(int userId) const {
    return (userId > 0 && userId < MAX_USER_SIZE) ? weeken[userId] : -1;
}

void AttendanceManager::calculateScore(const std::string& w, const std::string& wk) {
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

void AttendanceManager::addBonusPoints() {
    for (int i = 1; i <= id_cnt; i++) {
        if (dat[i][Wednesday] > 9) points[i] += 10;
        if (dat[i][Saturday] + dat[i][Sunday] > 9) points[i] += 10;
    }
}

void AttendanceManager::judgeGrade() {
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

void AttendanceManager::displayResults() {
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

std::string AttendanceManager::getGradeNameById(int gradeId) {
    for (const auto& g : gradeInfos)
        if (g.gradeId == gradeId)
            return g.gradeName;
    return "UNKNOWN";
}

void AttendanceManager::setGradeThreshold(int gradeId, int newMinScore) {
    for (auto& gi : gradeInfos)
        if (gi.gradeId == gradeId) {
            gi.minScore = newMinScore;
            break;
        }
}

void AttendanceManager::addGrade(int gradeId, const std::string& gradeName, int minScore) {
    gradeInfos.push_back({ gradeId, gradeName, minScore });
}

std::vector<GradeInfo> AttendanceManager::getGradeInfos() {
    return gradeInfos;
}