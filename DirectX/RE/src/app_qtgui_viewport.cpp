#include <app_qtgui_viewport.h>

#include <QResizeEvent>
//-----------------------
#include <app_renderer.h>

namespace dapp::ui {

Viewport::Viewport(re::RenderEngine* engine, QWidget* parent)
	: engine(engine), QWidget(parent) {

	setAttribute(Qt::WA_NativeWindow);
	setAttribute(Qt::WA_PaintOnScreen);        
	setAttribute(Qt::WA_NoSystemBackground);  
	setUpdatesEnabled(false);                
}

void Viewport::resizeEvent(QResizeEvent* event) {

	QWidget::resizeEvent(event);

	if (engine != nullptr) {

	     engine->resize(event->size().width(),
	                      event->size().height());
	}
}

}
