#include <app_qtgui_main_window.h>

#include <app_renderer.h>

namespace dapp::ui {

MainWindow::MainWindow(re::RenderEngine* engine, QWidget* parent)
	: QMainWindow(parent) {

	viewport = new Viewport(engine, this);

	createGUI();
}

void MainWindow::updateFPS(int frameIdx) {

	int fps = max(frameIdx - lastFrameIdx, 0);

	lastFrameIdx = frameIdx;

	statusFPS->setText(tr("FPS: %1").arg(fps));
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

	QLabel* leftStatus = new QLabel("Status: Rendering", statusBar);

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
