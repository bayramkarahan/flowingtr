#ifndef VARIABLEEXPRESSIONDIALOG_H
#define VARIABLEEXPRESSIONDIALOG_H

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
struct ProcessRecord {
    int processType;
    QString expression;
    QString targetLabel;
    QString var1Label;
    QString var2Label;
};
class VariableProcessDialog : public QDialog
{
    Q_OBJECT
public:
    explicit VariableProcessDialog(QWidget *parent = nullptr);
   // QStringList getExpressions() const;
     void addExpressionRowparametre(int operationType, const QString &expression = QString());

    void addExpressionRow();
     QList<ProcessRecord> getExpressionsWithType() const;
private slots:

    void updateExpressionRowWidgets(int index);
 private:
    struct ProcessRow {
        QWidget *widget;
        QComboBox *variableTargetCombo;
        QLabel *equalLabel;
        QComboBox *operationTypeCombo;
        QComboBox *var1Combo;
        QComboBox *operatorCombo;
        QComboBox *var2Combo;
        QLineEdit *constEdit1;
         QLineEdit *constEdit2;
        QPushButton *removeButton;
    };

    QVBoxLayout *expressionsLayout;
    QList<ProcessRow*> processRows;
    QPushButton *addButton;

    QStringList variableLabels() const;
};

#endif // VARIABLEEXPRESSIONDIALOG_H
