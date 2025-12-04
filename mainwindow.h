#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Slot appelé automatiquement quand le texte du combo change
    void on_comboBoxTypeModel_currentTextChanged(const QString &text);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
