/** -*- mode: c++ ; c-basic-offset: 3 -*-
 * @file   widgetDrawLinewidth.h
 * @date   March 2019
 *
 * @brief  widgetDrawLineWidth is a widget that let us visualize the line width we draw with: it is promotted to the widget next to the line width.
 *
 * @author Abdelmalik GHOUBIR
 *
 * @copyright Copyright 2018-2019 Abdelmalik GHOUBIR
 * This file is owned by Abdelmalik GHOUBIR.
 */

#ifndef WIDGETDRAWLINEWIDTH_H
#define WIDGETDRAWLINEWIDTH_H

#include <QWidget>

class WidgetDrawLineWidth : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetDrawLineWidth(QWidget *parent = nullptr);

    /**
     * @brief Set the size of the pen used to draw the point defining the pen we use in the QGraphicsView
     * @param The size of the pen
     *
     */
    void setPenSize(int);

    /**
     * @brief Set the color of the pen used to draw the point defining the pen we use in the QGraphicsView
     * @param The color of the pen
     *
     */
    void setPenColor(QColor);

private:
    int _penSize = 2;
    QColor _penColor = Qt::white;

signals:

protected:
    void paintEvent(QPaintEvent *) override;

public slots:
};

#endif // WIDGETDRAWLINEWIDTH_H
