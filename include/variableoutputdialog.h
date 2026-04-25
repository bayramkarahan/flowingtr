#ifndef VARIABLEOUTPUTDIALOG_H
#define VARIABLEOUTPUTDIALOG_H

#include <QDialog>
#include <QList>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include<QGuiApplication>
#include<QScreen>
#include "variable.h"
struct OutputRecord {
    int outputType;
    QString expression;
    QString label;
    QString outputMessage;
};
class VariableOutputDialog : public QDialog
{
    Q_OBJECT
public:
    explicit VariableOutputDialog(QWidget *parent = nullptr);
    void addExpressionRowparametre(const VariableRecord &var);
    void addExpressionRow();
    QList<OutputRecord> getExpressionsWithType() const;
private slots:
    void updateExpressionRowWidgets(int index);
private:
    struct OutputRow {
        QWidget *widget;
        QLineEdit *outputMessageEdit;
        QComboBox *varLabelCombo;
        QComboBox *operationTypeCombo;
        QLineEdit *constEdit1;
        QPushButton *removeButton;
    };

    QVBoxLayout *expressionsLayout;
    QList<OutputRow*> outputRows;
    QPushButton *addButton;

    QStringList variableLabels() const;
};

#endif // VARIABLEOUTPUTDIALOG_H
