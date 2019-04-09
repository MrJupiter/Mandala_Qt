/**
 * @file   myQGraphicsView.cpp
 * @date   March 2019
 *
 * @brief  myQGraphicsView is a QGraphicsView that defines different methods that will help to draw different forms depending on a specified drawing mode: "single" or "mandala"
 *
 * @author Abdelmalik GHOUBIR
 *
 * @copyright Copyright 2018-2019 Abdelmalik GHOUBIR
 * This file is owned by Abdelmalik GHOUBIR.
 */

#include "myQGraphicsView.h"
#include <QDebug>
#include <math.h>
#include <tuple>
#include <functional>

MyQGraphicsView::MyQGraphicsView(QWidget *parent) : QGraphicsView(parent) {
    _scene = new QGraphicsScene();
    _scene->setSceneRect(0,0,width(),height());
    setScene(_scene);
}

MyQGraphicsView::~MyQGraphicsView() {
    delete _scene;
    _slicesAddedHistory.clear();
    _mirrorLinesHistory.clear();
    _undoHistoryStack.clear();
    _redoHistoryStack.clear();
    qDebug() << "Deleted View's Objects!";
}

// Setters:
void MyQGraphicsView::setPenSize(int penSize) {
    _penSize = penSize;
}

void MyQGraphicsView::setPenColor(QColor penColor) {
    _penColor = penColor;
}

void MyQGraphicsView::setBrightness(int brightness) {
    _brightness = 255 - brightness;
    setAndDrawSlices(_slices);
}

void MyQGraphicsView::setRainbowMode(bool hsvColorToggled) {
    _hsvColorToggled = hsvColorToggled;
}

void MyQGraphicsView::setGridButtonEnabled(bool gridButtonEnabled) {
    _gridButtonEnabled = gridButtonEnabled;
    // Delete all the slices, and after if the grid button is enabled we redraw them
    removeAllDrawnSlices();
    if(_gridButtonEnabled) {
        setAndDrawSlices(_slices);
    }
}

void MyQGraphicsView::setMirrorButtonEnabled(bool mirrorButtonEnabled) {
    _mirrorButtonEnabled = mirrorButtonEnabled;
    // Delete all the mirror lines, and after if the mirror button is enabled we redraw them
    removeAllDrawnMirrorLines();
    if(_mirrorButtonEnabled) {
        setMirrorLines();
    }
}

void MyQGraphicsView::setMirrorLines() {
    if(_slices != 0) {
        _mirrorLinesHistory.clear();  // free the vector

        QPen pen(QColor(0, 0, 0, 80), 1, Qt::SolidLine);

        for(int i=1; i<_slices+1; ++i) {
            QLineF angleline;
            /* Set the origin: */
            angleline.setP1(QPointF(width()/2, height()/2));
            angleline.setLength(sqrt(pow(width()/2, 2) + pow(height()/2, 2)));
            angleline.setAngle(i*360/_slices + 180/_slices);

            QGraphicsLineItem * angleLineItem = _scene->addLine(angleline, pen);
            _mirrorLinesHistory.push_back(angleLineItem);

            _scene->update();
        }
    }
}

void MyQGraphicsView::setAndDrawSlices(int slices) {
    _slices = slices;
    if(_gridButtonEnabled) {
        if(_slices != 0) {
            removeAllDrawnSlices();
            _slicesAddedHistory.clear(); // free the vector

            QPen pen(QColor(0, 0, 0, _brightness), 3, Qt::DashLine);
            for(int i=1; i<_slices+1; ++i) {
                QLineF angleline;
                /* Set the origin: */
                angleline.setP1(QPointF(width()/2, height()/2));

                angleline.setLength(sqrt(pow(width()/2, 2) + pow(height()/2, 2)));
                angleline.setAngle(i*360.0/_slices);

                QGraphicsLineItem * angleLineItem = _scene->addLine(angleline, pen);
                _slicesAddedHistory.push_back(angleLineItem);
            }
        }
    }

    _scene->update();
}

void MyQGraphicsView::setPaintEnabled(bool paintEnabled) {
    _paintEnabled = paintEnabled;
}

// Listeners:
void MyQGraphicsView::mouseMoveEvent(QMouseEvent * e) {
    if(_paintEnabled) {
        setMouseTracking(true);
        _screenshotActivator = _scene->items().size();

        if(e->buttons() == Qt::LeftButton) {
            if(_gridButtonEnabled)
                removeAllDrawnSlices();
            if(_mirrorButtonEnabled)
                removeAllDrawnMirrorLines();
            QPointF pt = mapToScene(e->pos());

            if(_drawLineIndicator > 0) {
                QGraphicsLineItem * item = _scene->addLine(_previousPoint.rx(), _previousPoint.ry(), pt.rx(), pt.ry(), QPen(QBrush(_penColor), _penSize, Qt::SolidLine, Qt::RoundCap));

                if(_slices != 0) {
                    if(_mirrorButtonEnabled)
                        mirrorSymetricDrawing(item, _penColor);
                    QPointF p1(width()/2, height()/2);

                    // drawLinesSymmetricallyToSlices(QPointF, QPointF) is a method that helps to draw symetrics lines to slices :
                    // drawLinesSymmetricallyToSlices(QPointF, QPointF) is a first classic method that use pure complex number transdormations

                    // drawLinesSymmetricallyToSlices(_previousPoint, pt);

                    //  drawLinesSymmetricallyToSlices(QGraphicsLineItem *) is the second method to do the same thing: it's an overloaded method that uses QTransform
                    drawLinesSymmetricallyToSlices(item);
                }
            }
            _previousPoint = pt;
            _drawLineIndicator++;

            if(_gridButtonEnabled)
                setAndDrawSlices(_slices);
            if(_mirrorButtonEnabled)
                setMirrorLines();
        }
        _screenshotActivator = _scene->items().size() - _screenshotActivator;

        _scene->update();
    }
}

void MyQGraphicsView::mouseReleaseEvent(QMouseEvent *) {
    _drawLineIndicator = 0;
    if(_screenshotActivator > 0) {
        removeAllDrawnSlices();
        removeAllDrawnMirrorLines();
        // We only screenshot the drawn items without grid slices and mirrors lines!
        _undoHistoryStack.push(grab(QRect(QPoint(0,0), QSize(width(), height()))));
        // We redraw grid slices and mirrors lines if their options are activated!
        if(_gridButtonEnabled)
            setAndDrawSlices(_slices);
        if(_mirrorButtonEnabled)
            setMirrorLines();
    }
}

// Other Useful Methods:
void MyQGraphicsView::undoLastAction() {
    if(!_undoHistoryStack.empty()) {
        _redoHistoryStack.push(_undoHistoryStack.pop());
        _scene->clear();
        if(!_undoHistoryStack.empty()) {
            _scene->addPixmap(_undoHistoryStack.top().scaled(size()));
        }
    }

    if(_gridButtonEnabled)
        setAndDrawSlices(_slices);
    if(_mirrorButtonEnabled)
        setMirrorLines();

    _scene->update();

}

void MyQGraphicsView::redoLastAction() {
    if(!_redoHistoryStack.empty()) {
        _scene->clear();
        QPixmap img = _redoHistoryStack.pop();
        _undoHistoryStack.push(img);
        _scene->addPixmap(img.scaled(size()));
    }

    if(_gridButtonEnabled)
        setAndDrawSlices(_slices);
    if(_mirrorButtonEnabled)
        setMirrorLines();

    _scene->update();
}

void MyQGraphicsView::clearScene(bool clearScene) {
    if(clearScene)
        _scene->clear();

    removeAllDrawnSlices();
    removeAllDrawnMirrorLines();
    if(_gridButtonEnabled)
        setAndDrawSlices(_slices);
    if(_mirrorButtonEnabled)
        setMirrorLines();
}

void MyQGraphicsView::removeAllDrawnSlices() {
    for (auto iter = _slicesAddedHistory.begin(); iter != _slicesAddedHistory.end(); ++iter) {
        //  if we don't insert this condition we'll get the warning message:
        //  QGraphicsScene::removeItem: item 0x20e5cbd0's scene (0x0) is different from this scene (0x20e73000)
        if(exists(*iter))
            _scene->removeItem(*iter);
    }
}

void MyQGraphicsView::removeAllDrawnMirrorLines() {
    for (auto iter = _mirrorLinesHistory.begin(); iter != _mirrorLinesHistory.end(); ++iter)
        if(exists(*iter))
            _scene->removeItem(*iter);
}

bool MyQGraphicsView::exists(QGraphicsItem * item) {
    QList<QGraphicsItem *> items = _scene->items();
    return (std::find(items.begin(), items.end(), item) != items.end());
}

std::tuple<int, int, int> MyQGraphicsView::updateHSVColor(QColor hsvColor) {
    int hue = (hsvColor.hue() + 360/_slices)%360;
    int satura = hsvColor.saturation();
    int value = hsvColor.value();
    return std::make_tuple (hue, satura, value);
}

QPointF MyQGraphicsView::changeReference(QPointF center, QPointF point, double angleInRadian) {
    double cosOfRotation = cos(angleInRadian);
    double sinOfRotation = sin(angleInRadian);

    double xr = point.rx()-center.rx(); // Changement de repére: x relatif
    double yr = point.ry()-center.ry(); // Changement de repére: y relatif

    double x = xr * cosOfRotation - yr * sinOfRotation; // Rotation sur l'axe (Point(width()/2, height()/2), Qt::ZAxes)
    double y = yr * cosOfRotation + xr * sinOfRotation; // Rotation sur l'axe (Point(width()/2, height()/2), Qt::ZAxes)
    x += center.rx(); // Retour au repére d'origine du centre Point(0, 0)
    y += center.ry(); // Retour au repére d'origine
    return QPointF(x,y);
}

void MyQGraphicsView::drawLinesSymmetricallyToSlices(QPointF previousPoint, QPointF currentMousePressPoint) {
    QColor hsvColor = _penColor.convertTo(QColor::Hsv);
    QPointF relatifRefCenter(width()/2, height()/2);

    for(int i=1; i<_slices; ++i) {
        QPointF firstPoint = changeReference(relatifRefCenter, previousPoint, i*2*M_PI/_slices);
        QPointF secondPoint = changeReference(relatifRefCenter, currentMousePressPoint, i*2*M_PI/_slices);
        QGraphicsLineItem * item;
        if(_hsvColorToggled) {
            std::tuple<int, int, int> newHSVColor = updateHSVColor(hsvColor);
            hsvColor = QColor::fromHsv(std::get<0>(newHSVColor), std::get<1>(newHSVColor), std::get<2>(newHSVColor));
            item = _scene->addLine(firstPoint.rx(), firstPoint.ry(), secondPoint.rx(), secondPoint.ry(), QPen(QBrush(hsvColor), _penSize, Qt::SolidLine, Qt::RoundCap));

            if(_mirrorButtonEnabled)
                mirrorSymetricDrawing(item, hsvColor);
        } else {
            item = _scene->addLine(firstPoint.rx(), firstPoint.ry(), secondPoint.rx(), secondPoint.ry(), QPen(QBrush(_penColor), _penSize, Qt::SolidLine, Qt::RoundCap));

            if(_mirrorButtonEnabled)
                mirrorSymetricDrawing(item, _penColor);
        }
    }
}

void MyQGraphicsView::drawLinesSymmetricallyToSlices(QGraphicsLineItem * item) {
    QColor hsvColor = _penColor.convertTo(QColor::Hsv);
    for(int i=1; i<_slices; ++i) {
        QTransform transform = QTransform().translate(width()/2, height()/2).rotate(i*360/_slices).translate(-width()/2, -height()/2);
        QLineF item2 = transform.map(item->line());
        QGraphicsLineItem * item;
        if(_hsvColorToggled) {
            std::tuple<int, int, int> newHSVColor = updateHSVColor(hsvColor);
            hsvColor = QColor::fromHsv(std::get<0>(newHSVColor), std::get<1>(newHSVColor), std::get<2>(newHSVColor));
            item = _scene->addLine(item2, QPen(QBrush(hsvColor), _penSize, Qt::SolidLine, Qt::RoundCap));

            if(_mirrorButtonEnabled)
                mirrorSymetricDrawing(item, hsvColor);
        } else {
            item = _scene->addLine(item2, QPen(QBrush(_penColor), _penSize, Qt::SolidLine, Qt::RoundCap));

            if(_mirrorButtonEnabled)
                mirrorSymetricDrawing(item, _penColor);
        }
    }
}

void MyQGraphicsView::resizePaintedItems() {
    _scene->addPixmap(_undoHistoryStack.top().scaled(size()));

    if(_gridButtonEnabled)
        setAndDrawSlices(_slices);
    if(_mirrorButtonEnabled)
        setMirrorLines();
}

bool MyQGraphicsView::undoStackIsEmpty() {
    return _undoHistoryStack.empty();
}

void MyQGraphicsView::openImage(QString f) {
    _scene->clear();
    QPixmap img = QPixmap(f);
    _undoHistoryStack.push(img);
    _scene->addPixmap(img.scaled(size()));

    if(_gridButtonEnabled)
        setAndDrawSlices(_slices);
    if(_mirrorButtonEnabled)
        setMirrorLines();
}

void MyQGraphicsView::clearAllHistories() {
    _scene->clear();
    _undoHistoryStack.clear();
    _redoHistoryStack.clear();
}

void MyQGraphicsView::pushScreenShot() {
    removeAllDrawnSlices();
    removeAllDrawnMirrorLines();
    // We only screenshot the drawn items without grid slices and mirrors lines!
    _undoHistoryStack.push(grab(QRect(QPoint(0,0), QSize(width(), height()))));
    // We redraw grid slices and mirrors lines if their options are activated!
    if(_gridButtonEnabled)
        setAndDrawSlices(_slices);
    if(_mirrorButtonEnabled)
        setMirrorLines();
}

void MyQGraphicsView::mirrorSymetricDrawing(QGraphicsLineItem * item, QColor color) {
    QTransform transform = QTransform().translate(width()/2, height()/2).rotate(180, Qt::XAxis).translate(-width()/2, -height()/2);
    QLineF item2 = transform.map(item->line());

    _scene->addLine(item2, QPen(QBrush(color), _penSize, Qt::SolidLine, Qt::RoundCap));
}

bool MyQGraphicsView::sceneIsEmpty() {
    return _scene->items().empty();
}
