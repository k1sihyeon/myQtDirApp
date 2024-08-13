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
