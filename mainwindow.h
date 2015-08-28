#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "server.h"
#include "client.h"
#include "arduino.h"
#include <cstdint>
#include <QMainWindow>
#include <QTimer>
#include <QVariant>
#include <QKeyEvent>
#include <QLayout>
#include <QHash>
#include <QHashIterator>
#include <QAction>
#include <QMessageBox>
#include <QInputDialog>
#include <iostream>

class Server;
class Client;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    void log(QString message);
    ~MainWindow();

public slots:
    void readyClicked();
    void launchClicked(bool resend = true);
    void startServer();
    void startClient();

protected:
    virtual void keyPressEvent(QKeyEvent* e);

private:
    Ui::MainWindow* ui;
    int8_t countdown;
    bool started;
    int actualCharIndex;
    QString actualSongContent;
    int removedChars;
    int lastRemovedChars;
    QHash<QString, QString>* songsList;

    int maximumEvery300ms;
    QTimer* charsCounterTimer;
    QTimer* countdownTimer;

    bool isClient;
    bool ready;
    bool otherReady;
    bool arduinoIsReady;

    QString generateRichHTMLFromLyrics(bool success);
    void updateScroll(QString part);
    void charsCounter();
    void initSongs();

    Arduino* arduino;
    Client* client;
    Server* server;
    void updateButtons();
    bool requireClient(bool silent = false);
};

#endif // MAINWINDOW_H
