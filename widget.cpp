#include "widget.h"

#include <QDir>
#include <QFile>
#include <QListWidget>
#include <QLineEdit>
#include <QFileInfo>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QMenu>
#include <QContextMenuEvent>
#include <QApplication>
#include <QMessageBox>
#include <QTextStream>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    dir = new QDir(".");

    // define actions
    QAction* mkdirAct = new QAction("&Make Directory", this);
    QAction* cpAct = new QAction("&Copy", this);
    QAction* rmAct = new QAction("Re&move", this);
    QAction* renameAct = new QAction("&Rename", this);
    QAction* quitAct = new QAction("&Quit", this);

    // connect actions
    connect(mkdirAct, SIGNAL(triggered()), SLOT(mkDir()));
    connect(cpAct, SIGNAL(triggered()), SLOT(cpFile()));
    connect(rmAct, SIGNAL(triggered()), SLOT(rmDir()));
    connect(renameAct, SIGNAL(triggered()), SLOT(renameDir()));
    connect(quitAct, SIGNAL(triggered()), SLOT(quit()));

    // add actions to menu
    menu = new QMenu(this);
    menu->addSection("&Directory");
    menu->addAction(mkdirAct);
    menu->addSeparator();
    menu->addSection("&File");
    menu->addAction(cpAct);
    menu->addAction(rmAct);
    menu->addAction(renameAct);
    menu->addSeparator();
    menu->addAction(quitAct);

    // define widgets
    dirListWidget = new QListWidget(this);
    filenameLineEdit = new QLineEdit(this);
    outputEdit = new QTextEdit(this);
    outputEdit->setReadOnly(true);

    //define button, layout
    QPushButton* actionButton = new QPushButton("Action", this);
    actionButton->setMenu(menu);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(dirListWidget);
    layout->addWidget(filenameLineEdit);
    layout->addWidget(actionButton);
    layout->addWidget(outputEdit);

    // connect widgets
    connect(dirListWidget, SIGNAL(itemClicked(QListWidgetItem*)), SLOT(selectItem(QListWidgetItem*)));
    connect(dirListWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), SLOT(changeDir()));
    //changeDir slot에 QListWidgetItem을 같이 넘겨줘도 ok

    refreshDir();
    // show
}

Widget::~Widget() {}

void Widget::refreshDir() {
    dirListWidget->clear();

    for (auto& item : dir->entryList())
        dirListWidget->addItem(item);
}

void Widget::selectItem(QListWidgetItem* item) {
    filenameLineEdit->setText(item->text());
}

void Widget::cpFile() {
    QString filename = dir->absoluteFilePath(dirListWidget->currentItem()->text());
    QFileInfo check(filename);

    if (check.isFile() && filenameLineEdit->text().length()) {
        QString newFile = dir->absoluteFilePath(filenameLineEdit->text());


        if (filename == newFile) {
            QMessageBox::warning(this, "Error", "New file name is same as Existing file");
            filenameLineEdit->clear();
        }
        else
            QFile::copy(filename, newFile);
            //qfile static 멤버 함수 -> singleton?
    }

    dir->refresh();
    refreshDir();
}

// double click slot
void Widget::changeDir() {
    QString filename = dir->absoluteFilePath(dirListWidget->currentItem()->text());
    QFileInfo check(filename);

    if (check.isDir()) {
        dir->cd(filename);
        dir->refresh();
        refreshDir();
    }
    else if (check.isFile()) {
        if (check.isReadable()) {
            QFile file(filename);
            file.open(QFile::ReadOnly);
            QTextStream in(&file);

            outputEdit->setWindowTitle(filename);
            outputEdit->clear();

            QString line;
            while (in.readLineInto(&line))
                outputEdit->append(line);

            file.close();
        }
        else
            QMessageBox::warning(this, "Error", "Cannot Read this file");
    }
}

void Widget::mkDir() {
    if (filenameLineEdit->text().length() > 0) {
        dir->mkdir(filenameLineEdit->text());
        dir->refresh();
        refreshDir();
    }
}

void Widget::rmDir() {
    if ((filenameLineEdit->text().length() > 0) && (dirListWidget->currentItem() != NULL)) {
        QString filename = dir->absoluteFilePath(dirListWidget->currentItem()->text());
        QFileInfo check(filename);

        if (check.isDir())
            dir->rmdir(filenameLineEdit->text());
        else if (check.isFile())
            QFile::remove(filename);

        dir->refresh();
        refreshDir();
    }
}

void Widget::renameDir() {
    if ((filenameLineEdit->text().length() < 0) || (dirListWidget->currentItem() == NULL))
        return;

    dir->rename(dirListWidget->currentItem()->text(), filenameLineEdit->text());
    dir->refresh();
    refreshDir();
}

// override
void Widget::contextMenuEvent(QContextMenuEvent* event) {
    QWidget::contextMenuEvent(event);
    menu->exec(QCursor::pos());
}
