#pragma once

#include <QApplication>
#include <QObject>
#include <QTimer>
//-----------------------------
#include <app_renderer.h> 
#include <app_qtgui_main_window.h> 

namespace dapp {

class DeirrorzApp : public QObject {
	Q_OBJECT

public:
	DeirrorzApp(int& argc, char** argv);

	int run();

public slots:
	void onIdleTick();

	void onQuit() { engine.stopRendering(); }

	void onUpdateStats();

private:
	QApplication app;

	std::unique_ptr<ui::MainWindow> mainWindow = nullptr;

private:
	re::RenderEngine engine;
	
	QTimer* idleTimer = nullptr;
	QTimer* fpsTimer = nullptr;

};

} 

