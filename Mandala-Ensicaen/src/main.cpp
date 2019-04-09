/**
 * @file   main.cpp
 * @date   March 2019
 *
 * @brief  this class helps us to execute our application, as it lets the application run in the local OS language
 *
 * @author Abdelmalik GHOUBIR
 *
 * @copyright Copyright 2018-2019 Abdelmalik GHOUBIR
 * This file is owned by Abdelmalik GHOUBIR.
 */

#include "mainWindow.h"
#include <QApplication>
#include <QTranslator>
#include <QInputDialog>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setOrganizationDomain("ensicaen.fr");
    QApplication::setOrganizationName("ENSICAEN");
    QApplication::setApplicationName("Mandala");

    QLocale locale;
    QStringList languages = locale.uiLanguages();

    QTranslator translator;

    if(languages[0].split("-")[0] ==  "fr"){
        translator.load(":/translations/french.qm");
    }else if (languages[0].split("-")[0]  == "ar"){
        translator.load(":/translations/arabic.qm");
    }

    if(languages[0].split("-")[0] != "en"){
        a.installTranslator(&translator);
    }

    MainWindow w;
    w.show();

    return a.exec();
}
