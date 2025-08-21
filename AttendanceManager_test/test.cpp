#include "pch.h"
#include "../AttendanceManagement/attendance.cpp"
#include <stdexcept>
#include <sstream>
#include <string>

class AttendanceManagement_test : public ::testing::Test {
protected:
    IAttendanceManager* am;

    void SetUp() override {
        am = new AttendanceManager();  // 나중에 MockAttendanceManager로 바꿔도 됨
    }

    void TearDown() override {
        delete am;
    }
};

TEST_F(AttendanceManagement_test, TestCalculateScore) {
    // 새 사용자와 요일 입력
    am->calculateScore("Alice", "monday");
    am->calculateScore("Alice", "wednesday");  // 점수 3점 + wed++
    am->calculateScore("Alice", "saturday");   // 점수 2점 + weekend++

    int id = am->getUserId("Alice");

    EXPECT_EQ(am->getDat(id, Monday), 1);
    EXPECT_EQ(am->getDat(id, Wednesday), 1);
    EXPECT_EQ(am->getDat(id, Saturday), 1);

    EXPECT_EQ(am->getPoints(id), 1 + 3 + 2);
    EXPECT_EQ(am->getWed(id), 1);
    EXPECT_EQ(am->getWeeken(id), 1);
}

TEST_F(AttendanceManagement_test, TestAddBonusPoints) {
    am->calculateScore("Bob", "wednesday");
    int id = am->getUserId("Bob");

    // wednesday를 10번 이상 기록
    for (int i = 0; i < 10; i++) {
        am->calculateScore("Bob", "wednesday");
    }
    // saturday + sunday를 10번 이상 기록
    for (int i = 0; i < 10; i++) {
        am->calculateScore("Bob", "saturday");
    }

    am->addBonusPoints();

    // bonus 10 + 10 추가되었는지 확인
    EXPECT_GE(am->getPoints(id), (3 * 11) + (2 * 10) + 20);
}

TEST_F(AttendanceManagement_test, TestJudgeGrade) {
    am->calculateScore("Charlie", "monday");
    int id = am->getUserId("Charlie");

    // 점수 낮아서 NORMAL
    am->judgeGrade();
    EXPECT_EQ(am->getGrade(id), GRADE_NORMAL);

    for (int i = 0; i < 10; i++)
        am->calculateScore("Charlie", "wednesday");

    // SILVER 점수 넘게 세팅
    am->judgeGrade();
    EXPECT_EQ(am->getGrade(id), GRADE_SILVER);

    // GOLD 점수 넘게 강제로 세팅
    for (int i = 0; i < 5; i++) {
        am->calculateScore("Charlie", "saturday");
        am->calculateScore("Charlie", "sunday");
    }

    am->judgeGrade();
    EXPECT_EQ(am->getGrade(id), GRADE_GOLD);
}

TEST_F(AttendanceManagement_test, TestDisplayResults) {
    am->calculateScore("Daisy", "monday");
    am->judgeGrade();

    testing::internal::CaptureStdout();
    am->displayResults();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_NE(output.find("NAME : Daisy"), std::string::npos);
    EXPECT_NE(output.find("POINT :"), std::string::npos);
    EXPECT_NE(output.find("GRADE :"), std::string::npos);
}

TEST_F(AttendanceManagement_test, TestJudgeGradeUpdateScore) {
    am->calculateScore("Charlie", "monday");
    int id = am->getUserId("Charlie");

    // SILVER if the score is 10 or more
    am->setGradeThreshold(GRADE_SILVER, 10);
    for (int i = 0; i < 10; i++)
        am->calculateScore("Charlie", "monday");
    am->judgeGrade();
    EXPECT_EQ(am->getGrade(id), GRADE_SILVER);

    // GOLD if the score is 10 or more
    am->setGradeThreshold(GRADE_GOLD, 20);
    for (int i = 0; i < 10; i++)
        am->calculateScore("Charlie", "monday");
    am->judgeGrade();
    EXPECT_EQ(am->getGrade(id), GRADE_GOLD);
}

TEST_F(AttendanceManagement_test, AddGradeAddsNewGrade) {
    int initialSize = am->getGradeInfos().size();

    am->addGrade(3, "BRONZE", 10);
    int newSize = am->getGradeInfos().size();
    EXPECT_EQ(newSize, initialSize + 1);

    const auto& grades = am->getGradeInfos();
    bool found = false;
    for (const auto& g : grades) {
        if (g.gradeId == 3 && g.gradeName == "BRONZE" && g.minScore == 10) {
            found = true;
            break;
        }
    }
    EXPECT_TRUE(found);
}