#pragma once

#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
//-----------------------------
#include <app_qtgui_viewport.h> 

namespace dapp {

class DeirrorzApp;

}

namespace dapp::ui {

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow(DeirrorzApp* app, QWidget* parent = nullptr);

	HWND getViewportNativeHandle() const { return viewport->getNativeHandle(); }

	void updateFPS(int frameIdx);

protected:
	void keyPressEvent(QKeyEvent* event) override;

private:
	void createGUI();

private:
	QWidget* central;          
	QVBoxLayout* mainLayout;  
	QHBoxLayout* statusLayout;

	Viewport* viewport;      
	QWidget* statusBar;     
	QLabel* statusFPS;     

	QLabel* leftStatus;

private:
	static constexpr char const* renderModeName[] = {"Rastering", "RayTracing"};

private:
	int lastFrameIdx = 0;

	int renderArrIdx = 0;

private:
	DeirrorzApp* app = nullptr;

};

}