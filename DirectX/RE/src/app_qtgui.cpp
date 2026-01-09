#include <app_qtgui.h>

namespace dapp {

DeirrorzApp::DeirrorzApp(int& argc, char** argv)
	: app(argc, argv) {

	mainWindow = std::make_unique<ui::MainWindow>(this);

	engine.init(mainWindow->getViewportNativeHandle());
}

int DeirrorzApp::run() {

	mainWindow->show();

	idleTimer = new QTimer(mainWindow.get());
	connect(idleTimer, &QTimer::timeout, this, &DeirrorzApp::onIdleTick);
	idleTimer->start(0);

	fpsTimer = new QTimer(mainWindow.get());
	connect(fpsTimer, &QTimer::timeout, this, &DeirrorzApp::onUpdateStats);
	fpsTimer->start(1000);

	return app.exec();
}

void DeirrorzApp::onUpdateStats() {

	int currFrameIdx = engine.getFrameIdx();
	engine.updateColorChoice();

	mainWindow->updateFPS(currFrameIdx);
}

void DeirrorzApp::onResetTimers() {

	idleTimer->stop();
	fpsTimer->stop();

	idleTimer->start(0);
	fpsTimer->start(1000);
}

void DeirrorzApp::onIdleTick() { 

	engine.renderRedAndGreen();
}

} 
