#ifndef VARIABLEEDITFORM_H
#define VARIABLEEDITFORM_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include "variable.h"
#include<QFormLayout>
#include<QGuiApplication>
#include<QScreen>
#include<QCheckBox>
class VariableEditForm : public QDialog
{
    Q_OBJECT

public:
    explicit VariableEditForm(const VariableRecord &record, QWidget *parent = nullptr);
    VariableRecord getRecord() const;

private:
    QLineEdit *labelEdit;
    QLineEdit *valueEdit;
    QCheckBox *isSecretCB;
    QComboBox *typeCombo;
    QPushButton *okButton;
    QPushButton *cancelButton;

    VariableRecord currentRecord;
};

#endif // VARIABLEEDITFORM_H
