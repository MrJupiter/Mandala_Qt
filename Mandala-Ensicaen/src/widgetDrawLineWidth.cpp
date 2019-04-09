/**
 * @file   widgetDrawLinewidth.cpp
 * @date   March 2019
 *
 * @brief  widgetDrawLineWidth is a widget that let us visualize the line width we draw with: it is promotted to the widget next to the line width.
 *
 * @author Abdelmalik GHOUBIR
 *
 * @copyright Copyright 2018-2019 Abdelmalik GHOUBIR
 * This file is owned by Abdelmalik GHOUBIR.
 */

#include "widgetDrawLineWidth.h"
#include <QPainter>

WidgetDrawLineWidth::WidgetDrawLineWidth(QWidget *parent) : QWidget(parent)
{
}

void WidgetDrawLineWidth::paintEvent(QPaintEvent *){
    QPainter painter(this);
    this->setStyleSheet("background-color:white;");

    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(_penSize);
    pen.setBrush(_penColor);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(pen);
    painter.fillRect(QRectF(0,0,width(),height()),QBrush(Qt::white));
    painter.drawPoint(width()/2, height()/2);
}

void WidgetDrawLineWidth::setPenSize(int penSize){
    _penSize = penSize;
}

void WidgetDrawLineWidth::setPenColor(QColor penColor){
    _penColor = penColor;
}
