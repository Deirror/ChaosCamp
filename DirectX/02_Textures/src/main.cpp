#include <cdxr_incl.h>

// Non-working demo as we need a real hwnd
int main() {

	cdxr::CDXRenderer engine; 
	engine.init(nullptr);

	engine.testClearAndSetRT();

	return 0;
}