#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include "sha256.h"
#include "sha256.cpp"

using namespace std;

string mkMerkleRoot(vector<string> transactions) {

	vector<string> merkle;
	int size = (int)transactions.size();

	if (size == 1) return transactions.front();
	if (size % 2 == 1) transactions.push_back(transactions[size - 1]);
	
	for (int i = 0; i < transactions.size() / 2; i++) {
		merkle.push_back(
			sha256(
				sha256(transactions[i * 2]) + sha256(transactions[i * 2 + 1])
			)
		);
		cout << merkle.back() << '\n';
	}
	cout << '\n';

	return mkMerkleRoot(merkle);
}

long mining(string block) {

	long nonce = 0;

	while (true) {
		string element;
		element = block + to_string(nonce);
		string result = sha256(element);
		printf("nonce: %10d:%s\r", nonce, result.c_str());
		if (result.substr(0, 4) == "0000") return nonce;
		nonce++;
	}
}

void runPoW(string merkleRootHash) {

	string block = "version: 1.0, prevBlockHash: abc, timeStamp: 2020.10.15, bits: 99999" + merkleRootHash;
	mining(block);
}

void timePrint(struct tm* pTimeInfo) {

	int year = pTimeInfo->tm_year + 1900;
	int month = pTimeInfo->tm_mon + 1;
	int day = pTimeInfo->tm_mday;
	int hour = pTimeInfo->tm_hour;
	int min = pTimeInfo->tm_min;
	int sec = pTimeInfo->tm_sec;

	printf("%d년 %d월 %d일 %d시 %d분 %d초\n", year, month, day, hour, min, sec);
}

void startPoW(time_t startTime, struct tm* convTime) {

	convTime = localtime(&startTime);
	cout << "시작시간: ";
	timePrint(convTime);
}

void endPoW(time_t endTime, struct tm* convTime) {

	cout << endl;
	convTime = localtime(&endTime);
	cout << "종료시간: ";
	timePrint(convTime);
}

void showRunningTime(time_t startTime, time_t endTime) {
	
	cout << "경과시간: " << (double)(endTime - startTime) << "초";
}


int main() {

	time_t startTime;
	time_t endTime;
	struct tm* convTime;

	vector<string> transaction = {
		"sender:a, receiver:1, amount:1, tid:100",
		"sender:b, receiver:2, amount:1, tid:101",
		"sender:c, receiver:3, amount:1, tid:102",
		"sender:d, receiver:4, amount:1, tid:103",
		"sender:e, receiver:5, amount:1, tid:104",
		"sender:f, receiver:6, amount:1, tid:105",
		"sender:g, receiver:7, amount:1, tid:106",
		"sender:h, receiver:8, amount:1, tid:107"
	};
	
	
	string merkleRootHash = mkMerkleRoot(transaction);

	startTime = time(NULL);
	startPoW(startTime, convTime);

	runPoW(merkleRootHash);

	endTime = time(NULL);
	endPoW(endTime, convTime);

	showRunningTime(startTime, endTime);
}

