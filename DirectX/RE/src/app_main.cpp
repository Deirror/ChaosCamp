#include "app_main.h"

#include "app_qtgui.h"

namespace dapp {

int runApp(int argc, char* argv[]) {

	DeirrorzApp app(argc, argv);

	return app.run();
}

}
