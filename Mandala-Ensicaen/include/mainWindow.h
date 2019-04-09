/** -*- mode: c++ ; c-basic-offset: 3 -*-
 * @file   mainWindow.h
 * @date   March 2019
 *
 * @brief  the mainWindow of the application that help us to interract with the application tools: connection SIGNALS / SLOTS
 *
 * @author Abdelmalik GHOUBIR
 *
 * @copyright Copyright 2018-2019 Abdelmalik GHOUBIR
 * This file is owned by Abdelmalik GHOUBIR.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    /**
     * @brief Estabish the connection SIGNAL / SLOTS
     *
     */
    void connectSignalSlots();

protected:
    /**
     * @brief When trying to close the application, you will get a warning message telling you to confirm your action
     * @param QCloseEvent
     *
     */
    void closeEvent (QCloseEvent *) override;
    /**
     * @brief When resizing the application, this method let the QGraphicsView in which we draw centered in our application
     * @param QResizeEvent
     *
     */
    void resizeEvent(QResizeEvent *) override;

private:
    Ui::MainWindow *ui;

    // _color defines the color we use to draw
    QColor _color;

    // _eraserActive defines if we will use the eraser or the brush:
    // If this last is set to false then when we use the brush and if it is set to true when we use the eraser
    bool _eraserActive = false;

    // _hsvActivator defines if we will use the HSV colors to draw or not:
    // If it is set to false then we only use one color to draw in grid mandala mode and if it is set to true we use HSV Color
    bool _hsvActivator = false;

    // _brush defines the icon of the cursor we will use to draw or to erase
    QPixmap _brush;

    // _singlePaintMode defines if we will draw using classic painting or with mandala effects:
    // If it is set to true, we only draw one form while mouseMoveEvent, and if it is set to false we draw using mandala effects
    bool _singlePaintMode = true;

    /**
     * @brief Personnalize a QMessageBOX and show it
     * @param The QMessageBOX window icon
     * @param The QMessageBOX title
     * @param The QMessageBOX text
     * @param The QMessageBOX icon
     * @param The buttons number of the  QMessageBOX, if 1 then display only QMessageBox:OK if 2 it'll display QMessageBox::Yes and QMessageBox::No
     * @return The text of the chosen button
     *
     */
    QString showMessageBox(QIcon icon, QString title, QString text, QPixmap pixmap, int buttonsNumber);

private slots:
    void actionExit_triggered();
    void actionAbout_triggered();
    void actionSaveAs_triggered();
    void actionRedo_triggered();
    void actionUndo_triggered();
    void actionOpenFile_triggered();
    void actionNewFile_triggered();

    void updateSlicesSpinBox(int);
    void updateSlicesSlider(int);
    void openRGBWindow();
    void resizePaintWidget(QString);
    void clearPaintWidget();
    void activateGrid(int);
    void activateMirror(int);
    void setPenSize(int);
    void useTheEraser();
    void useTheBrush();
    void setBrightness(int);
    void rainbowActivator();
    void singleModeActivator();
};

#endif // MAINWINDOW_H
