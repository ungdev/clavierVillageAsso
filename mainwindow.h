#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <cstdint>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void readyClicked();
    void launchClicked();

protected:
    virtual void keyPressEvent(QKeyEvent *e);

private:
    Ui::MainWindow *ui;
    int8_t countdown;
    bool started;
    int actualCharIndex;
    QString actualSongContent;
    bool hasFailed;
    int removedChars;
    int lastRemovedChars;
    QHash<QString, QString>* songsList;

    int maximumEvery300ms;

    QTimer* charsCounterTimer;

    QString generateRichHTMLFromLyrics(bool success);
    void updateScroll(QString part);
    void charsCounter();
    void initSongs();
};

#endif // MAINWINDOW_H
