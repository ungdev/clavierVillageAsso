#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <QVariant>
#include <QKeyEvent>
#include <QLayout>
#include <QHash>
#include <QHashIterator>
#include <QAction>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    songsList = new QHash<QString, QString>();

    songsList->insert(QString("Le Grand Vicaire"),
            QString("Chez nous le vélo, c'est une coutuuuuume"
            "Mon papa, il fait le cadre"
            "Ma maman, elle fait la selle,"
            "Et le curé la pommmpeeee..."
            "Et le grand vicaire toujours par derrière"
            "N'a jamais pu la pomper"
            "Et c'est ce qui l'emmerdeeeee!!!"
            "Chez nous les Renault, c'est une coutuuuuume,"
            "Mon papa, a une Mégane"
            "Ma maman, une Twingo,"
            "Et le curé l' Alpine..."
            "Et le grand vicquaire toujours par derrière"
            "N'a jamais pu la piner"
            "Et c'est ce qui l'emmerdeeeee!!!"
            "Chez nous le jardin, c'est une coutuuuuume,"
            "Mon papa, fait des tomates"
            "Ma maman, fait des salades,"
            "Et le curé laboure..."
            "Et le grand vicquaire toujours par derrière"
            "N'a jamais pu la bourrer"
            "Et c'est ce qui l'emmerdeeeee!!!"));

    initSongs();

    countdown = 3;
    started = false;
    actualCharIndex = 0;
    actualSongContent = "okokokok";
    actualSongContent = actualSongContent.repeated(100);
    removedChars = 0;
    lastRemovedChars = 0;

    charsCounterTimer = new QTimer();
    charsCounterTimer->start(300);
    maximumEvery300ms = 5;

    ui->lyricsViewer->setHtml(generateRichHTMLFromLyrics(false));
    ui->countdown->setText("");
    ui->speed->setText("");
    ui->counter->setText("");

    connect(ui->readyButton, &QPushButton::clicked, this, &MainWindow::readyClicked);
    connect(ui->launchButton, &QPushButton::clicked, this, &MainWindow::launchClicked);
    connect(charsCounterTimer, &QTimer::timeout, this, &MainWindow::charsCounter);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readyClicked()
{
    qDebug() << "Ready";
    ui->launchButton->setEnabled(true);
    ui->readyButton->setEnabled(false);
}

void MainWindow::launchClicked()
{
    qDebug() << "Launch";

    ui->launchButton->setEnabled(false);

    ui->countdown->setText(QVariant(countdown).toString());
    QTimer* countdownTimer = new QTimer();
    connect(countdownTimer, &QTimer::timeout, [=]() {
        --countdown;

        if (countdown == -1) {
            ui->countdown->setText("");
            started = true;
            countdownTimer->stop();
            return;
        }

        if (countdown == 0) {
            ui->countdown->setText("GO!");
            return;
        }

        ui->countdown->setText(QVariant(countdown).toString());
    });
    countdownTimer->start(1000);
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (!started) {
        // return;
    }

    QString typed = e->text().toLower();
    QString actualChar = actualSongContent[actualCharIndex].toLower();

    if (actualChar == " ")
    {
        ++actualCharIndex;
        ++removedChars;
        actualChar = actualSongContent[actualCharIndex].toLower();
    }

    if (typed == actualChar)
    {
        ++actualCharIndex;
    }

    ui->lyricsViewer->setHtml(generateRichHTMLFromLyrics(typed == actualChar));
}

QString MainWindow::generateRichHTMLFromLyrics(bool success)
{
    QString lyrics = QString(actualSongContent);

    if (success)
    {
        ++removedChars;
    }
    lyrics = lyrics.mid(removedChars);

    uint8_t charsToColor = 1;
    if (actualSongContent[actualCharIndex].unicode() == 32)
    {
        ++charsToColor;
    }

    lyrics = lyrics.insert(charsToColor, "</font>");
    lyrics = "<font style=\"color: #2980b9;\">" + lyrics;

    lyrics = "<font style=\"font-size: 25px;\">" + lyrics + "</font>";

    return lyrics;
}

void MainWindow::charsCounter()
{
    uint8_t removedCharsSinceLast = removedChars - lastRemovedChars;

    float percent = (float)removedCharsSinceLast / (float)maximumEvery300ms;

    uint8_t percent100 = (u_int8_t)(percent * 100);
    percent100 = std::min((uint8_t)100, percent100);

    ui->speed->setText(QVariant(removedCharsSinceLast).toString() + " chars.(1/3)s⁻¹");
    ui->counter->setText(QVariant(actualSongContent.length() - removedChars).toString());

    lastRemovedChars = removedChars;
}

void MainWindow::initSongs()
{
    QHash<QString, QString>::const_iterator i = songsList->constBegin();

    while (i != songsList->constEnd())
    {
        qDebug() << "okok";
        QAction* action = new QAction(i.key(), this);
        connect(action, &QAction::triggered, this, [=]() {
            // Ne pas changer de chanson en cours de route
            if (started)
            {
                return;
            }

            countdown = 3;
            started = false;
            actualCharIndex = 0;
            removedChars = 0;
            lastRemovedChars = 0;

            actualSongContent = i.value();

            ui->lyricsViewer->setHtml(generateRichHTMLFromLyrics(false));
            ui->countdown->setText("");
            ui->speed->setText("");
            ui->counter->setText("");
        });
        ++i;
        ui->menuChanson->addAction(action);
    }
}
