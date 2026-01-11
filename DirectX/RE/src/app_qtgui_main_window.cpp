#include "app_qtgui_main_window.h"

#include "app_qtgui.h"

namespace dapp::ui {

MainWindow::MainWindow(DeirrorzApp* app, QWidget* parent)
	: QMainWindow(parent), app(app) {

	viewport = new Viewport(&app->getEngine(), this);

	createGUI();
}

void MainWindow::updateFPS(int frameIdx) {

	int fps = max(frameIdx - lastFrameIdx, 0);

	lastFrameIdx = frameIdx;

	statusFPS->setText(tr("FPS: %1").arg(fps));
}

void MainWindow::keyPressEvent(QKeyEvent* event) {

	if (event->key() == Qt::Key::Key_Q) {

		app->getEngine().updateRenderMode();
		app->onResetTimers();

		lastFrameIdx = 0;

		renderArrIdx = (renderArrIdx + 1) % 2;

		leftStatus->setText(tr("Status: %1").arg(renderModeName[renderArrIdx]));
	}
}

void MainWindow::createGUI() {

	central = new QWidget(this);
	setCentralWidget(central);

	mainLayout = new QVBoxLayout(central);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);

	mainLayout->addWidget(viewport, /*stretch*/ 9);

	statusBar = new QWidget(this);
	statusBar->setFixedHeight(30);
	statusLayout = new QHBoxLayout(statusBar);
	statusLayout->setContentsMargins(8, 4, 8, 4);

	leftStatus = new QLabel("", statusBar);
	leftStatus->setText(tr("Status: %1").arg(renderModeName[renderArrIdx]));

	statusFPS = new QLabel("FPS: 0", statusBar);
	statusFPS->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

	statusLayout->addWidget(leftStatus);
	statusLayout->addStretch();
	statusLayout->addWidget(statusFPS);

	mainLayout->addWidget(statusBar, /*stretch*/ 1);

	resize(1280, 720);
	setWindowTitle("Deirrorz Application");
}

}
