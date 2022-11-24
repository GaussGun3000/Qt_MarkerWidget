#include "MarkerWidget.h"
#include <qpainter.h>
#include <QWheelEvent>
#include <vector>

enum MarkerWidget::defaultMarkerParameters { dScale = 0, dCM = 16, dMS = 7 };

MarkerWidget::MarkerWidget(QWidget* parent) : QWidget(parent), scale(dScale), coordMultiplyer(dCM), markerSize(dMS)
{
	this->lines = QVector<QLineF>();
}

void MarkerWidget::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.eraseRect(1,1, this->width(), this->height());
	painter.setPen(Qt::black);
	painter.drawLines(this->lines);
}

void MarkerWidget::createMarker(QPointF p)
/**
*	Creates a marker regarding current scale and adds it to drawing list
*/
{
	int x1, x2, y1, y2;
	QPoint offset = this->getOffset();
	x1 = p.x() * coordMultiplyer + offset.x() + markerSize / 2;
	y1 = p.y() * coordMultiplyer + offset.y();
	x2 = x1 - markerSize + 1;
	y2 = y1;
	QLineF hLine(x1, y1, x2, y2);
	
	x1 = p.x() * coordMultiplyer + offset.x();
	y1 = p.y() * coordMultiplyer + offset.y() + markerSize / 2;
	x2 = x1;
	y2 = y1 - markerSize + 1;
	QLineF vLine(x1, y1, x2, y2);

	this->lines.append(hLine);
	this->lines.append(vLine);
}

void MarkerWidget::render(std::vector<QPointF>& points)
/**
*   Render new list of markers
*/
{
	this->lines.clear();
	this->scale = dScale;
	this->coordMultiplyer = dCM;
	this->markerSize = dMS;
	this->points = std::vector<QPointF>(points);
	for (QPointF p : this->points)
		createMarker(p);
	this->update();
	scaleChanged();
}
void MarkerWidget::render()
/**
*   Rerender same list of markers with different scale applied
*/
{
	this->lines.clear();
	for (QPointF p : this->points)
		createMarker(p);
	this->update();
}

std::string MarkerWidget::getScale()
/**
*	Returns a string representing current scale
*/
{
	return std::to_string(static_cast<int>(100.f * this->coordMultiplyer / dCM)) + std::string("%");
}

void MarkerWidget::wheelEvent(QWheelEvent* event)
/**
*	Handle mouse wheel events inside the markerArea
*/
{
	if (event->delta() > 0)
		if (this->increaseScale())
		{
			render();
			scaleChanged();
			event->accept();			
		}
		else event->ignore();
	else if (event->delta() < 0)
	{
		if (this->decreaseScale())
		{
			render();
			scaleChanged();
			event->accept();
		}
			else event->ignore();
	}
	else event->ignore();

}

bool MarkerWidget::increaseScale()
/**
*	Inscrease scale if not at maximal already (returns True if scale was changed)
*/
{
	if (this->scale < maxScaleSteps)
	{
		this->scale++;
		this->coordMultiplyer*=2;
		this->markerSize+=2;
		return true;
	}
	else return false;
}

bool MarkerWidget::decreaseScale()
/**
*	Descrease scale if not at minimal already (returns True if scale was changed)
*/
{
	if (this->scale > -maxScaleSteps)
	{
		this->scale--;
		this->coordMultiplyer /= 2;
		this->markerSize -= 2;
		return true;
	}
	else return false;
}

QPoint MarkerWidget::getOffset()
/**
*	Calculate cooridnates offset to shift (0,0) for markers to the center of the widget
*/
{
	return QPoint(this->width() / 2, this->height() / 2);
}
