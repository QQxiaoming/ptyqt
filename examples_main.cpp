#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QDir>
#include <QProcessEnvironment>
#include <QDebug>
#include "ptyqt.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QMainWindow *mainWindow = new QMainWindow();
    QWidget *centralwidget = new QWidget(mainWindow);
    mainWindow->setCentralWidget(centralwidget);
    QVBoxLayout *verticalLayout = new QVBoxLayout(centralwidget);
    centralwidget->setLayout(verticalLayout);
    QPlainTextEdit *recv = new QPlainTextEdit(mainWindow);
    QLineEdit *send = new QLineEdit(mainWindow);
    QPushButton *sendButton = new QPushButton("send",mainWindow);
    recv->setReadOnly(true);
    verticalLayout->addWidget(recv);
    verticalLayout->addWidget(send);
    verticalLayout->addWidget(sendButton);

    IPtyProcess *localShell = PtyQt::createPtyProcess();
#if defined(Q_OS_LINUX) || defined(Q_OS_MACOS)
    QString shellPath = qEnvironmentVariable("SHELL");
#elif defined(Q_OS_WIN)
    QString shellPath = "c:\\Windows\\system32\\WindowsPowerShell\\v1.0\\powershell.exe";
#endif
    QStringList args;
    localShell->startProcess(shellPath, args, QDir::homePath(), QProcessEnvironment::systemEnvironment().toStringList(),  80, 25);

    QObject::connect(localShell->notifier(), &QIODevice::readyRead, [=](){
        QByteArray data = localShell->readAll();
        if (!data.isEmpty()) {
            recv->appendPlainText(QString::fromUtf8(data));
        }
    });
    QObject::connect(localShell->notifier(), &QIODevice::aboutToClose, [=](){
        if (localShell) {
            QByteArray restOfOutput = localShell->readAll();
            if (!restOfOutput.isEmpty()) {
                recv->appendPlainText(QString::fromUtf8(restOfOutput));
                localShell->notifier()->disconnect();
            }
        }
    });
    QObject::connect(sendButton, &QPushButton::clicked, [=](){
        QString cmd = send->text() + "\n";
        QByteArray data = cmd.toUtf8();
        localShell->write(data);
    });

    mainWindow->show();
    return app.exec();
}
