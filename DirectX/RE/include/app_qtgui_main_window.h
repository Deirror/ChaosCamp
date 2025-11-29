#pragma once

#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
//-----------------------------
#include <app_qtgui_viewport.h> 

namespace dapp::re {

class RenderEngine;

}

namespace dapp::ui {

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	MainWindow(re::RenderEngine* engine, QWidget* parent = nullptr);

	HWND getViewportNativeHandle() const { return viewport->getNativeHandle(); }

	void updateFPS(int frameIdx);

private:
	void createGUI();

private:
	QWidget* central;          
	QVBoxLayout* mainLayout;  
	QHBoxLayout* statusLayout;

	Viewport* viewport;      
	QWidget* statusBar;     
	QLabel* statusFPS;     

private:
	int lastFrameIdx = 0;

};


}