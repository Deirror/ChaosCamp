#include "app_qtgui_viewport.h"

#include <QResizeEvent>
//-----------------------
#include "app_renderer.h"

namespace dapp::ui {

Viewport::Viewport(re::RenderEngine* engine, QWidget* parent)
	: engine(engine), QWidget(parent) {

	setMouseTracking(true);

	setAttribute(Qt::WA_NativeWindow);
	setAttribute(Qt::WA_PaintOnScreen);        
	setAttribute(Qt::WA_NoSystemBackground);  
	setUpdatesEnabled(false);                
}

void Viewport::mousePressEvent(QMouseEvent* event) {

	if (event->button() != Qt::LeftButton) {
		return;
	}

	QPointF pos = event->position();

	float offsX = (pos.x() / width()) * 2.f - 1.f;
	float offsY = 1.f - (pos.y() / height()) * 2.f;

	engine->updateOffsFrameData(offsX, offsY);

    QWidget::mousePressEvent(event);
}

void Viewport::resizeEvent(QResizeEvent* event) {

	QWidget::resizeEvent(event);

	if (engine != nullptr) {

	     engine->resize(event->size().width(),
	                      event->size().height());
	}
}

}
