/**
 * @file   mainWindow.cpp
 * @date   March 2019
 *
 * @brief  the mainWindow of the application that help us to interract with the application tools: connection SIGNALS / SLOTS
 *
 * @author Abdelmalik GHOUBIR
 *
 * @copyright Copyright 2018-2019 Abdelmalik GHOUBIR
 * This file is owned by Abdelmalik GHOUBIR.
 */

#include "mainWindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QCloseEvent>
#include <QColorDialog>
#include <QPixmap>
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QCoreApplication::setAttribute(Qt::AA_DontUseNativeMenuBar);
    ui->setupUi(this);

    this->setWindowIcon(QIcon(":/img/mandala.png"));
    this->window()->setWindowTitle(tr("Mandala-Ensicaen"));

    connectSignalSlots();

    QPixmap squareColor(70,70);
    squareColor.fill(Qt::white);
    ui->colorButton->setIcon(QIcon(squareColor));

    // Fix the graphicsView:
    QRect rcontent = ui->graphicsView->contentsRect();
    ui->graphicsView->setSceneRect(0, 0, rcontent.width(), rcontent.height());

    ui->multiColor->setIcon(QIcon (":/img/rgbColors.png"));
    ui->eraserButton->setIcon(QIcon (":/img/eraser.png"));
    ui->brushButton->setIcon(QIcon (":/img/brush.png"));
    ui->lineWidthWidget->move(8, 8);

    ui->gridSlider->setEnabled(false);
    ui->mirrorCheckBox->setEnabled(false);
    ui->grid->setEnabled(false);
    ui->clearWidget->setEnabled(false);
    ui->spinBox->setEnabled(false);
    ui->lineWidthSlider->setEnabled(false);
    ui->sliceSlider->setEnabled(false);
    ui->colorButton->setEnabled(false);
    ui->multiColor->setEnabled(false);
    ui->brushButton->setEnabled(false);
    ui->eraserButton->setEnabled(false);
    ui->singlePainterActivator->setEnabled(false);

    ui->action_Redo->setEnabled(false);
    ui->action_Undo->setEnabled(false);
    ui->actionSave_As->setEnabled(false);
    ui->action_Open_File->setEnabled(false);

    ui->actionSave_As->setIcon(QIcon(":/img/save_image.png"));
    ui->action_Open_File->setIcon(QIcon(":/img/open_new.png"));
    ui->actionNew_File->setIcon(QIcon(":/img/create_new.png"));
    ui->actionQuit->setIcon(QIcon(":/img/quit.png"));
    ui->action_Undo->setIcon(QIcon(":/img/undo.png"));
    ui->action_Redo->setIcon(QIcon(":/img/redo.png"));
    ui->action_About->setIcon(QIcon(":/img/about.png"));

}

MainWindow::~MainWindow() {
    delete ui;
    qDebug() << "Deleted UI";
}

void MainWindow::connectSignalSlots() {
    // Connect Actions
    connect(ui->actionQuit, SIGNAL(triggered(bool)), this, SLOT(actionExit_triggered()));
    connect(ui->action_About, SIGNAL(triggered(bool)), this, SLOT(actionAbout_triggered()));
    connect(ui->actionSave_As, SIGNAL(triggered(bool)), this, SLOT(actionSaveAs_triggered()));
    connect(ui->action_Redo, SIGNAL(triggered(bool)), this, SLOT(actionRedo_triggered()));
    connect(ui->action_Undo, SIGNAL(triggered(bool)), this, SLOT(actionUndo_triggered()));
    connect(ui->action_Open_File, SIGNAL(triggered(bool)), this, SLOT(actionOpenFile_triggered()));
    connect(ui->actionNew_File, SIGNAL(triggered(bool)), this, SLOT(actionNewFile_triggered()));

    // Connect Sliders
    connect(ui->sliceSlider, SIGNAL(valueChanged(int)), this, SLOT(updateSlicesSpinBox(int )));
    connect(ui->gridSlider, SIGNAL(valueChanged(int)), this, SLOT(setBrightness(int)));
    connect(ui->lineWidthSlider, SIGNAL(valueChanged(int)), this, SLOT(setPenSize(int)));

    // Connect Buttons
    connect(ui->colorButton, SIGNAL(clicked()), this, SLOT(openRGBWindow()));
    connect(ui->eraserButton, SIGNAL(clicked()), this, SLOT(useTheEraser()));
    connect(ui->brushButton, SIGNAL(clicked()), this, SLOT(useTheBrush()));
    connect(ui->clearWidget, SIGNAL(clicked()), this, SLOT(clearPaintWidget()));
    connect(ui->multiColor, SIGNAL(clicked()), this, SLOT(rainbowActivator()));
    connect(ui->singlePainterActivator, SIGNAL(clicked()), this, SLOT(singleModeActivator()));

    // Connect Boxes
    connect(ui->spinBox, SIGNAL(valueChanged(int)), this, SLOT(updateSlicesSlider(int)));
    connect(ui->pixelComboBox, SIGNAL(currentTextChanged(const QString &)), this, SLOT(resizePaintWidget(QString)));

    // Connect CheckBox
    connect(ui->grid, SIGNAL(stateChanged(int)), this, SLOT(activateGrid(int)));
    connect(ui->mirrorCheckBox, SIGNAL(stateChanged(int)), this, SLOT(activateMirror(int)));
}

void MainWindow::actionRedo_triggered() {
    ui->graphicsView->redoLastAction();
}

void MainWindow::actionUndo_triggered() {
    ui->graphicsView->undoLastAction();
}

void MainWindow::actionExit_triggered() {
    close();
}

void MainWindow::actionAbout_triggered()
{
    showMessageBox(QIcon(":/img/mandala.png"), tr("About Mandala-Ensicaen"), tr("Mandala\n\n(c) 2019 Abdelmalik GHOUBIR"), QPixmap(":/img/ensicaen.jpg"), 1);
}

QString MainWindow::showMessageBox(QIcon icon, QString title, QString text, QPixmap pixmap, int buttonsNumber) {
    QMessageBox message;
    message.setWindowIcon(icon);
    message.setWindowTitle(title);
    message.setText(text);
    if(buttonsNumber == 1) {
        message.setStandardButtons(QMessageBox::Ok);
        message.setDefaultButton(QMessageBox::Ok);
    } else if(buttonsNumber == 2) {
        message.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        message.setDefaultButton(QMessageBox::No);
    }

    QSize myRessortSize(100,100);
    QPixmap pixmap1 = pixmap.scaled(myRessortSize,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    message.setIconPixmap(pixmap1);
    message.show();
    message.exec();

    return message.clickedButton()->text();
}

void MainWindow::actionSaveAs_triggered() {
    QString selectedFilter;
    QString fileName = QFileDialog::getSaveFileName(this,
                       tr("Save Image"),
                       QCoreApplication::applicationDirPath(),
                       "BMP (*.bmp);;JPG (*.jpg);;JPEG (*.jpeg);;PNG (*.png)", &selectedFilter);


    qDebug() << selectedFilter;
    // we don't need to save the splices and the mirror lines ;)
    bool gridState = ui->grid->isChecked();
    bool mirrorState = ui->mirrorCheckBox->isChecked();

    ui->grid->setChecked(false);
    ui->mirrorCheckBox->setChecked(false);
    if (!fileName.isEmpty()) {
        QPixmap pixMap = ui->graphicsView->grab();
        pixMap.save(fileName);
    }
    ui->grid->setChecked(gridState);
    ui->mirrorCheckBox->setChecked(mirrorState);

}

void MainWindow::actionOpenFile_triggered() {
    QString file = QFileDialog::getOpenFileName(this, tr("Open An Existing Image"), QString(), "Images (*.png *.jpg *.jpeg *.bmp)");

    if (!file.isEmpty())
        ui->graphicsView->openImage(file);
}

void MainWindow::actionNewFile_triggered() {
    if(!ui->graphicsView->sceneIsEmpty()) {
        QString warningResponse = showMessageBox(QIcon(":/img/mandala.png"), tr("WARNING"),
                                  tr("Are you sure you want to create a new image?\n\nThis will clear all your drawn items history...\n\nSave your image if not done!"),
                                  QPixmap(":/img/ensicaen.jpg"), 2);

        if(warningResponse == "&Yes") {
            _color = Qt::white;
            QPixmap pixmap(70,70);
            pixmap.fill(_color);
            ui->colorButton->setIcon(QIcon(pixmap));
            ui->lineWidthWidget->setPenColor(_color);
            if(!_eraserActive)
                ui->graphicsView->setPenColor(_color);
            _singlePaintMode = true;
            ui->lineWidthSlider->setValue(2);
            ui->pixelComboBox->setCurrentIndex(0);
            ui->singlePainterActivator->setText(tr("Single Mode"));

            ui->graphicsView->clearAllHistories();

            ui->sliceSlider->setValue(2);
            ui->graphicsView->setAndDrawSlices(0);
        }
    }

}

void MainWindow::singleModeActivator() {
    _singlePaintMode = !_singlePaintMode;

    if(_singlePaintMode) {
        ui->graphicsView->setAndDrawSlices(0);
        ui->singlePainterActivator->setText(tr("Single Mode"));
        ui->mirrorCheckBox->setChecked(false);
        ui->mirrorCheckBox->setEnabled(false);

        ui->multiColor->setEnabled(false);
        ui->spinBox->setEnabled(false);
        ui->sliceSlider->setEnabled(false);
        ui->grid->setChecked(false);
        ui->grid->setEnabled(false);
    }
    else {
        ui->graphicsView->setAndDrawSlices(ui->sliceSlider->value());
        ui->singlePainterActivator->setText(tr("Mandala Mode"));
        ui->mirrorCheckBox->setEnabled(true);
        ui->multiColor->setEnabled(true);
        ui->spinBox->setEnabled(true);
        ui->sliceSlider->setEnabled(true);
        ui->grid->setEnabled(true);
    }
}

void MainWindow::rainbowActivator() {
    _hsvActivator = !_hsvActivator;
    ui->graphicsView->setRainbowMode(_hsvActivator);
}

void MainWindow::useTheBrush() {
    _eraserActive = false;
    _brush = QPixmap(":/img/brush.png");
    _brush = _brush.scaled(QSize(50,70), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->graphicsView->setCursor(QCursor(_brush,0,0));
    ui->graphicsView->setPenColor(_color);
}

void MainWindow::useTheEraser() {
    _eraserActive = true;
    _brush = QPixmap(":/img/eraser.png");
    _brush = _brush.scaled(QSize(20,20), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    ui->graphicsView->setCursor(QCursor(_brush, 10, 10));
    ui->graphicsView->setPenColor(Qt::white);
}

void MainWindow::setBrightness(int i) {
    ui->graphicsView->setBrightness(i);
}

void MainWindow::setPenSize(int penSize) {
    ui->lineWidthWidget->setPenSize(penSize);
    ui->graphicsView->setPenSize(penSize);
}

void MainWindow::activateGrid(int i) {
    ui->gridSlider->setEnabled(i!=0);
    ui->graphicsView->setGridButtonEnabled(i!=0);
}

void MainWindow::activateMirror(int i) {
    ui->graphicsView->setMirrorButtonEnabled(i!=0);
}

void MainWindow::clearPaintWidget() {
    ui->graphicsView->setStyleSheet("background-color: rgb(255, 255, 255);border: inherit;border-radius: inherit;");
    int x = (ui->widget->width() - ui->graphicsView->width())/2;
    int y = (ui->widget->height() - ui->graphicsView->height())/2;

    ui->graphicsView->move(x, y);
    ui->graphicsView->clearScene(true);
    ui->graphicsView->pushScreenShot();
}

void MainWindow::resizePaintWidget(QString s) {
    if(s != "Screen Size" && s!="Taille de l'écran") {
        ui->action_Redo->setEnabled(true);
        ui->action_Undo->setEnabled(true);
        ui->actionSave_As->setEnabled(true);
        ui->action_Open_File->setEnabled(true);
        ui->widget->setStyleSheet("background-color:rgb(218,218,218);border-color: rgb(0, 85, 255);border-style: outset;border-width: 2px;border-radius: 10px;");
        if(_eraserActive) {
            _brush = QPixmap(":/img/eraser.png");
            _brush = _brush.scaled(QSize(20,20), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            ui->graphicsView->setCursor(QCursor(_brush, 10, 10));
        } else {
            _brush = QPixmap(":/img/brush.png");
            ui->graphicsView->setCursor(QCursor(_brush,0,0));
            _brush = _brush.scaled(QSize(50,70), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            ui->graphicsView->setCursor(QCursor(_brush,0,0));
        }
        if(!_singlePaintMode) {
            ui->mirrorCheckBox->setEnabled(true);
            ui->multiColor->setEnabled(true);
            ui->spinBox->setEnabled(true);
            ui->sliceSlider->setEnabled(true);
            ui->grid->setEnabled(true);
        }
        ui->graphicsView->setPaintEnabled(true);
        ui->singlePainterActivator->setEnabled(true);
        ui->colorButton->setEnabled(true);
        ui->brushButton->setEnabled(true);
        ui->eraserButton->setEnabled(true);
        ui->clearWidget->setEnabled(true);
        ui->lineWidthSlider->setEnabled(true);

        ui->graphicsView->setStyleSheet("background-color: rgb(255, 255, 255);border: inherit;border-radius: inherit;");
        ui->graphicsView->resize(s.split("x")[0].toInt(), s.split("x")[0].toInt());

        int x = (ui->widget->width() - ui->graphicsView->width())/2;
        int y = (ui->widget->height() - ui->graphicsView->height())/2;

        ui->graphicsView->move(x, y);
        ui->graphicsView->clearScene(true);
        if(!ui->graphicsView->undoStackIsEmpty())
            ui->graphicsView->resizePaintedItems();
    }
    else {
        ui->action_Redo->setEnabled(false);
        ui->action_Undo->setEnabled(false);
        ui->actionSave_As->setEnabled(false);
        ui->action_Open_File->setEnabled(false);
        ui->widget->setStyleSheet("border-color: rgb(218, 218, 218);");

        ui->graphicsView->setCursor(QCursor());
        ui->mirrorCheckBox->setChecked(false);
        ui->mirrorCheckBox->setEnabled(false);

        ui->graphicsView->setPaintEnabled(false);
        ui->singlePainterActivator->setEnabled(false);
        ui->colorButton->setEnabled(false);
        ui->multiColor->setEnabled(false);
        ui->brushButton->setEnabled(false);
        ui->eraserButton->setEnabled(false);
        ui->clearWidget->setEnabled(false);
        ui->spinBox->setEnabled(false);
        ui->lineWidthSlider->setEnabled(false);
        ui->sliceSlider->setEnabled(false);
        ui->grid->setChecked(false);
        ui->grid->setEnabled(false);
        ui->graphicsView->clearScene(true);
        ui->graphicsView->setStyleSheet("background-color: rgb(240, 240, 240);border: inherit;border-radius: inherit;");
    }

}

void MainWindow::updateSlicesSlider(int n) {
    ui->sliceSlider->setValue(n);
}

void MainWindow::updateSlicesSpinBox(int n) {
    ui->spinBox->setValue(n);
    ui->graphicsView->setAndDrawSlices(n);
    ui->graphicsView->setMirrorButtonEnabled(ui->mirrorCheckBox->isChecked());
}

void MainWindow::openRGBWindow() {
    _color = QColorDialog::getColor(Qt::white, this );
    QPixmap pixmap(70,70);
    pixmap.fill(_color);
    ui->colorButton->setIcon(QIcon(pixmap));
    ui->lineWidthWidget->setPenColor(_color);
    if(!_eraserActive)
        ui->graphicsView->setPenColor(_color);
}

void MainWindow::closeEvent (QCloseEvent * event) {
    QString exitResponse = showMessageBox(QIcon(":/img/mandala.png"), tr("You want to quit..."),
                                          tr("Are you sure you want to exit the application?"),
                                          QPixmap(":/img/ensicaen.jpg"), 2);

    (exitResponse != "&Yes")?event->ignore():event->accept();
}

void MainWindow::resizeEvent(QResizeEvent*) {
    QString s = ui->pixelComboBox->currentText();
    ui->graphicsView->resize(s.split("x")[0].toInt(), s.split("x")[0].toInt());
    int x = (ui->widget->width() - ui->graphicsView->width())/2;
    int y = (ui->widget->height() - ui->graphicsView->height())/2;
    ui->graphicsView->move(x, y);
}
