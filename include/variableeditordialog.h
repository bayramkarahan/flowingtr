#ifndef VARIABLEEDITORDIALOG_H
#define VARIABLEEDITORDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "variable.h"
#include <QComboBox>
#include <QDebug>
#include<QGuiApplication>
#include<QScreen>
class VariableEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VariableEditorDialog(QWidget *parent = nullptr);
    void updateVariablesFromTable();
    void loadVariables();
signals:
    void variableUpdateRecord(VariableRecord oldRec,VariableRecord newRec);
private slots:

    void addVariable();
    void removeSelectedVariable();
    void onTypeChanged(const QString &type);

private:
    QTableWidget *tableWidget;
    QPushButton *addButton;
    QPushButton *removeButton;
    QPushButton *saveButton;
    QString previousValue; // ✅ BU ŞEKİLDE OLMALI
};

#endif // VARIABLEEDITORDIALOG_H
