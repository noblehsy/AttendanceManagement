#pragma once
#include <string>
#include <vector>

enum Day { Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday, InvalidDay };

struct GradeInfo {
    int gradeId;
    std::string gradeName;
    int minScore;
};

class IAttendanceManager {
public:
    virtual ~IAttendanceManager() {}

    virtual int getUserId(const std::string& username) const = 0;
    virtual int getDat(int userId, Day day) const = 0;
    virtual int getPoints(int userId) const = 0;
    virtual int getGrade(int userId) const = 0;
    virtual int getWed(int userId) const = 0;
    virtual int getWeeken(int userId) const = 0;

    virtual void getData() = 0;
    virtual void calculateScore(const std::string& w, const std::string& wk) = 0;
    virtual void getScore() = 0;
    virtual void addBonusPoints() = 0;
    virtual void judgeGrade() = 0;
    virtual void displayResults() = 0;

    virtual void setGradeThreshold(int gradeId, int newMinScore) = 0;
    virtual void addGrade(int gradeId, const std::string& gradeName, int minScore) = 0;
    virtual std::vector<GradeInfo> getGradeInfos() = 0;
};
