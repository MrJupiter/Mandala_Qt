/** -*- mode: c++ ; c-basic-offset: 3 -*-
 * @file   myQGraphicsView.h
 * @date   March 2019
 *
 * @brief  myQGraphicsView is a QGraphicsView that defines different methods that will help to draw different forms depending on a specified drawing mode: "single" or "mandala"
 *
 * @author Abdelmalik GHOUBIR
 *
 * @copyright Copyright 2018-2019 Abdelmalik GHOUBIR
 * This file is owned by Abdelmalik GHOUBIR.
 */

#ifndef MYQGRAPHICSVIEW_H
#define MYQGRAPHICSVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QMouseEvent>
#include <QStack>

class MyQGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit MyQGraphicsView(QWidget *parent = nullptr);
    ~MyQGraphicsView() override;

    /**
     * @brief Set the size of the pen used to draw (QGraphicsItem *) objects
     * @param The size of the pen
     *
     */
    void setPenSize(int);

    /**
     * @brief Set the color of the pen used to draw (QGraphicsItem *) objects
     * @param The color of the pen
     *
     */
    void setPenColor(QColor);

    /**
     * @brief Set the number of slices we must use to divide the QGraphicsView (the app drawing view) and draw them if the grid mode was activated
     * @param The number of slices
     */
    void setAndDrawSlices(int);

    /**
     * @brief Set the brightness of the lines used to define _slices (when grid button is clicked: grid mode)
     * @param The opacity value that the pen use to draw this dashLines
     *
     */
    void setBrightness(int);

    /**
     * @brief Turn on/off a toggle that help us to know if the user have activated the grid mode (If the user want to see the dashlines that defines the view slices)
     * @param The boolean letting us know if the user wants to activate the grid dashlines or not
     *
     */
    void setGridButtonEnabled(bool);

    /**
     * @brief Turn on/off a toggle that help us to know if the user have activated the mirror mode (If the user want to see the dashlines that defines the view mirror lines)
     * @param The boolean letting us know if the user wants to activate the mirror dashlines or not
     *
     */
    void setMirrorButtonEnabled(bool);

    /**
     * @brief Set the HSV color mode on or off
     * @param The boolean letting us know if the user wants to activate the HSV colors to draw in mandala mode
     *
     */
    void setRainbowMode(bool);

    /**
     * @brief If the user haven't selected a size to the QGraphicsView, he can't draw anything. This method let us know if he chosed a size or not
     * @param The boolean letting us know if the user have set a size to the QGraphicsView or not
     *
     */
    void setPaintEnabled(bool);

    /**
     * @brief Let us clear the QGraphicsScene: delete all drawn QGraphicsScene items (QGraphicsItem)
     * @param The boolean letting us know if we want to clear ou QGraphicsScene or not
     *
     */
    void clearScene(bool);

    /**
     * @brief Let the user undo the last actions
     *
     */
    void undoLastAction();

    /**
     * @brief Let the user redo the last actions
     *
     */
    void redoLastAction();

    /**
     * @brief When the user change the size of the QGraphicsView, this method scales all drawn items to the new QGraphicsView size
     *
     */
    void resizePaintedItems();

    /**
     * @brief Let us know if the undo stack is empty or not
     * @return True if the the undo stack is empty, and false if not
     *
     */
    bool undoStackIsEmpty();

    /**
     * @brief Let the user open an existing image: it will automatically dragged in the QGraphicsView: this method clear our QGraphicsScene!
     * @param The image absolute path
     *
     */
    void openImage(QString);

    /**
     * @brief Push on the undo stack a screenshot of the actual QGraphicsView (without slices dashlines and mirror lines)
     *
     */
    void pushScreenShot();

    /**
     * @brief Empty the undo and redo stacks as it clears the used QGraphicsScene
     *
     */
    void clearAllHistories();

    /**
     * @brief Let us know if the QGraphicsScene items list is empty or not
     * @return True if the the QGraphicsScene items list is empty, and false if not
     *
     */
    bool sceneIsEmpty();

private:
    QGraphicsScene * _scene;
    bool _paintEnabled = false;
    int _penSize = 2;
    QColor _penColor = Qt::white;
    int _slices = 0;
    int _brightness = 255;
    bool _hsvColorToggled = false;
    bool _gridButtonEnabled = false;
    bool _mirrorButtonEnabled = false;

    // _screenshotActivator let us know if we must push a grabed image in our _undoStackCommand or not:
    // we can click on the view without drawing so that won't be counted as an action
    int _screenshotActivator = 0;

    QStack<QPixmap> _undoHistoryStack;
    QStack<QPixmap> _redoHistoryStack;

    QVector<QGraphicsItem *> _slicesAddedHistory;
    QVector<QGraphicsItem *> _mirrorLinesHistory;

    QPointF _previousPoint;
    // _drawLineIndicator will help us to draw lines but whithout remembering the last position of our mouse click if we release the mouse!
    int _drawLineIndicator = 0;

    /**
     * @brief Draw lines that defines the mandala mirror
     *
     */
    void setMirrorLines();

    /**
     * @brief Removes all drawn QGraphicsLineItem objects that defines the QGraphicsView slices (grid lines)
     *
     */
    void removeAllDrawnSlices();

    /**
     * @brief Removes all drawn QGraphicsLineItem objects that defines the QGraphicsView slices (mirror lines)
     *
     */
    void removeAllDrawnMirrorLines();

    /**
     * @brief Let us know if a QGraphicsItem exists already in our QGraphicsScene
     * @param The searched QGraphicsItem
     * @return True if the QGraphicsItem is already drawn in our QGraphicsView (already memorized in our QGraphicsScene), and false if not.
     *
     */
    bool exists(QGraphicsItem *);

    /**
     * @brief If the user activated the "mandala mode", we need to draw in all our view slices the same object but symmetrically to the center of our QGraphicsView.
     * This method use the complex number rotation formula: we change the center of the default reference (0,0) to the middle of our QGraphicsView (width()/2, height()/2),
     * and we draw the QGraphicsItem objects the user drew after defining an angle in which we rotate this objects (the new rotation center will be the point
     * C(width()/2, height()/2), and the rotation ax is Qt::Zaxes
     *
     * @param Two points, because we draw lines not points:
     * We draw a QGraphicsLineItem from two points (the remembered previous mouse clicked points coordinate values, and the new mouse clicked points coordinate values)
     *
     */
    void drawLinesSymmetricallyToSlices(QPointF, QPointF);

    /**
     * @brief This is the overloaded method of drawLinesSymmetricallyToSlices(QPointF, QPointF): this last only use mathemics formula, but this new overloaded method only
     * use QTransform: we create a QTransform().translate(width()/2, height()/2).rotate("the angle of rotation").translate(-width()/2, -height()/2).
     * Both drawLinesSymmetricallyToSlices(QPointF, QPointF) and drawLinesSymmetricallyToSlices(QGraphicsLineItem *) do the same thing, but this one is more optimal.
     *
     * @param The QGraphicsLineItem drawn by the user
     *
     */
    void drawLinesSymmetricallyToSlices(QGraphicsLineItem *);

    /**
     * @brief Do the same job of QTransform().translate(width()/2, height()/2).rotate("the angle of rotation").translate(-width()/2, -height()/2)
     * @param QPointF: The new relative center
     * @param QPointF: The point we want to transform (we can easilly use QTransform, but I used the rotation formula)
     * @param The angle in which we want the point rotate
     * @return The new QPointF after being transformed
     *
     */
    QPointF changeReference(QPointF, QPointF, double);

    /**
     * @brief Let us use HSV color using rotation (thanks to hue())
     * @param QColor: hsvColor
     * @return Tuple<the new hue, the saturation, the value>
     *
     */
    std::tuple<int, int, int> updateHSVColor(QColor);

    /**
     * @brief This method helps us to draw the symmetrical objects of the drawn QGraphicsLineItem, using mirror lines (Mirror effects)
     *
     * @param The QGraphicsLineItem drawn by the user
     *
     */
    void mirrorSymetricDrawing(QGraphicsLineItem *, QColor);

protected:
    void mouseMoveEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;

signals:

public slots:
};

#endif // MYQGRAPHICSVIEW_H
