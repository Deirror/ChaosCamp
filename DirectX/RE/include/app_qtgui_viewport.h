#pragma once

#include <QWidget>

namespace dapp::re {

class RenderEngine;

}

namespace dapp::ui {

class Viewport : public QWidget {
	Q_OBJECT

public:
	explicit Viewport(re::RenderEngine* engine, QWidget* parent = nullptr);

	HWND getNativeHandle() const { return reinterpret_cast<HWND>(winId()); }

protected:
	void resizeEvent(QResizeEvent*) override;

private:
	re::RenderEngine* engine = nullptr;
	
};

} 
