#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class QDir;
class QListWidget;
class QListWidgetItem;
class QLineEdit;
class QTextEdit;
class QMenu;
class QContextMenuEvent;

class Widget : public QWidget
{
    Q_OBJECT

protected:
    void contextMenuEvent(QContextMenuEvent*);
    //context menu (우클릭 메뉴) event handler (virtual func) override

private:
    void refreshDir();

    QDir *directory;
    QListWidget *dirListWidget;
    QLineEdit *filenameLineEdit;
    QTextEdit *outputEdit;
    QMenu *menu;

public slots:
    void selectItem(QListWidgetItem*);
    void changeDir();
    void copyFile();
    void makeDir();
    void removeDir();
    void renameDir();

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
};
#endif // WIDGET_H
