#ifndef VARIABLEINPUTDIALOG_H
#define VARIABLEINPUTDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QDialogButtonBox>
#include "variable.h"
#include<QScrollArea>
#include<QGuiApplication>
#include<QScreen>

class VariableInputDialog : public QDialog {
    Q_OBJECT

public:
    explicit VariableInputDialog(QWidget *parent = nullptr);
    QList<VariableRecord> getSelectedVariables() const;
    void addVariableRow(const VariableRecord &var = VariableRecord());

private:
    struct VariableRow {
        QWidget *widget;
        QComboBox *operationTypeCombo;
        QComboBox *variableCombo;
        QCheckBox *inputCheckBox;
        QLineEdit *valueEdit;
        QLineEdit *inputMessageEdit;
        QPushButton *removeButton;
    };

    QVBoxLayout *selectionLayout;
    QVBoxLayout *rowsLayout;
    QPushButton *addButton;

    QList<VariableRow*> variableRows;

     void removeVariableRow(int index);
    QStringList variableLabels() const;
};

#endif // VARIABLEINPUTDIALOG_H
