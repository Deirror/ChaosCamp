#include <iostream>

#include "Vec3.h"

static void printVector(const crt::Vec3& v) {
	std::cout << "Vector: (" << v.x() << ", " << v.y() << ", " << v.z() << ")\n";
}

void normalAndAreaPrint(const crt::Vec3& A, const crt::Vec3& B, const crt::Vec3& C) {
	printVector(A);
	printVector(B);
	printVector(C);

	crt::Vec3 E1 = B - A;
	crt::Vec3 E2 = C - A;

	std::cout << "Normal vector of the triangle formed by A, B and C: ";

	crt::Vec3 N = cross(E1, E2);

	printVector(N.normalized());

	std::cout << "Area: " << N.length() / 2.0f << "\n\n";
}

void task3() {
	std::cout << "Task 3 is running.\n";

	normalAndAreaPrint(crt::Vec3(-1.75f, -1.75f, -3.0f), crt::Vec3(1.75f, -1.75f, -3.0f), crt::Vec3(0.0f, 1.75f, -3.0f));
	normalAndAreaPrint(crt::Vec3(0.0f, 0.0f, -1.0f), crt::Vec3(1.0f, 0.0f, 1.0f), crt::Vec3(-1.0f, 0.0f, 1.0f));
	normalAndAreaPrint(crt::Vec3(0.56f, 1.11f, 1.23f), crt::Vec3(0.44f, -2.368f, -0.54f), crt::Vec3(-1.56f, 0.15f, -1.92f));

	std::cout << "Task 3 completed.\n";
}
