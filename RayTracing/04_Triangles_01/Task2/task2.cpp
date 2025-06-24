#include <iostream>

#include "Vec3.h"

static void printVector(const crt::Vec3& v) {
	std::cout << "Vector: (" << v.x() << ", " << v.y() << ", " << v.z() << ")\n";
}

void crossProductPrint(const crt::Vec3& A, const crt::Vec3& B) {
	printVector(A);
	printVector(B);

	std::cout << "Cross product of A and B:";

	printVector(cross(A,B));

	std::cout << '\n';
}

void areaPrint(const crt::Vec3& A, const crt::Vec3& B) {
	printVector(A);
	printVector(B);

	std::cout << "Area of the parallelogram formed by A and B: " 
	          << cross(A, B).length() << "\n\n";
}

void task2() {
	std::cout << "Task 2 is running.\n";
	
	crossProductPrint(crt::Vec3(3.5f, 0.0f, 0.0f), crt::Vec3(1.75f, 3.5f, 0.0f));
	crossProductPrint(crt::Vec3(3.0f, -3.0f, 1.0f), crt::Vec3(4.0f, 9.0f, 3.0f));

	areaPrint(crt::Vec3(3.0f, -3.0f, 1.0f), crt::Vec3(4.0f, 9.0f, 3.0f));
	areaPrint(crt::Vec3(3.0f, -3.0f, 1.0f), crt::Vec3(-12.0f, 12.0f, -4.0f));

	std::cout << "Task 2 completed.\n";
}