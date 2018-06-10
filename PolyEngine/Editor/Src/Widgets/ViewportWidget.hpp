#pragma once

#include "Windows/CustomSDLWindow.hpp"
#include "Rendering/IRenderingDevice.hpp"
#include "Widgets/PolyWidget.hpp"

using namespace Poly;

class ViewportWidget : public PolyWidget
{
public:
	// Viewport widget must have parent at initialization otherwise it will appear without title bar
	explicit ViewportWidget();

	std::unique_ptr<IRenderingDevice> GetRenderingDevice();

protected:
	// overriden events
	void resizeEvent(QResizeEvent* resizeEvent) override;
	void wheelEvent(QWheelEvent* wheelEvent) override;
	void mouseMoveEvent(QMouseEvent* mouseEvent) override;
	void mousePressEvent(QMouseEvent* mouseEvent) override;
	void mouseReleaseEvent(QMouseEvent* mouseREvent) override;
	void keyPressEvent(QKeyEvent* keyEvent) override;
	void keyReleaseEvent(QKeyEvent* keyEvent) override;

private:
	CustomSDLWindow WindowInSDL;
};
