#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

#define MAX_DATA_SIZE 500
#define MAX_USER_SIZE 100
#define MAX_DAYS_SIZE 7

#define SCORE_GOLD 50
#define SCORE_SILVER 30

#define GRADE_NORMAL 0
#define GRADE_GOLD 1
#define GRAGE_SILVER 2

struct Node {
	string w;
	string wk;
}nodes[MAX_DATA_SIZE];

map<string, int> userIds;
int id_cnt = 0;

//dat[사용자ID][요일]
int dat[MAX_USER_SIZE][MAX_DAYS_SIZE];
int points[MAX_USER_SIZE];
int grade[MAX_USER_SIZE];
string names[MAX_USER_SIZE];

int wed[MAX_USER_SIZE];
int weeken[MAX_USER_SIZE];

enum Day { Monday, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday, InvalidDay };

Day stringToDay(const std::string& wk) {
	if (wk == "monday") return Monday;
	else if (wk == "tuesday") return Tuesday;
	else if (wk == "wednesday") return Wednesday;
	else if (wk == "thursday") return Thursday;
	else if (wk == "friday") return Friday;
	else if (wk == "saturday") return Saturday;
	else if (wk == "sunday") return Sunday;
	else return InvalidDay;
}

void getData()
{
	ifstream fin{ "attendance_weekday_500.txt" }; //500개 데이터 입력
	for (int i = 0; i < MAX_DATA_SIZE; i++)
		fin >> nodes[i].w >> nodes[i].wk;
}


static void calculateScore (string w, string wk) {
	//ID 부여
	if (userIds.count(w) == 0) {
		userIds.insert({ w, ++id_cnt });

		if (w == "Daisy") {
			int debug = 1;
		}

		names[id_cnt] = w;
	}
	int id = userIds[w];

	//디버깅용
	if (w == "Daisy") {
		int debug = 1;
	}

	Day day = stringToDay(wk);
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
		break;
	case Thursday:
		points[id]++;
		break;
	case Friday:
		points[id]++;
		break;
	case Saturday:
		points[id] += 2;
		weeken[id] += 1;
		break;
	case Sunday:
		points[id] += 2;
		weeken[id] += 1;
		break;
	default:
		break;
	}
}

void getScore()
{
	for (int i = 0; i < MAX_DATA_SIZE; i++)
		calculateScore(nodes[i].w, nodes[i].wk);
}

void addBonusPoints()
{
	for (int i = 1; i <= id_cnt; i++) {
		if (dat[i][2] > 9) {
			points[i] += 10;
		}

		if (dat[i][5] + dat[i][6] > 9) {
			points[i] += 10;
		}
	}
}

void judgeGrade()
{
	for (int i = 1; i <= id_cnt; i++) {
		if (points[i] >= SCORE_GOLD) {
			grade[i] = GRADE_GOLD;
		}
		else if (points[i] >= SCORE_SILVER) {
			grade[i] = GRAGE_SILVER;
		}
		else {
			grade[i] = GRADE_NORMAL;
		}
	}
}

void displayResults() {
	for (int i = 1; i <= id_cnt; i++) {
		cout << "NAME : " << names[i] << ", ";
		cout << "POINT : " << points[i] << ", ";
		cout << "GRADE : ";

		if (grade[i] == 1) {
			cout << "GOLD" << "\n";
		}
		else if (grade[i] == 2) {
			cout << "SILVER" << "\n";
		}
		else {
			cout << "NORMAL" << "\n";
		}
	}

	std::cout << "\n";
	std::cout << "Removed player\n";
	std::cout << "==============\n";
	for (int i = 1; i <= id_cnt; i++) {

		if (grade[i] != GRADE_GOLD && grade[i] != GRAGE_SILVER && wed[i] == 0 && weeken[i] == 0) {
			std::cout << names[i] << "\n";
		}
	}
}

int main() {
	getData();
	getScore();
	addBonusPoints();
	judgeGrade();
	displayResults();
}
