#ifndef VARIABLECONDITIONDIALOG_H
#define VARIABLECONDITIONDIALOG_H

#include <QDialog>
#include <QList>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

#include "variable.h"
#include<QGuiApplication>
#include<QScreen>
class VariableConditionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit VariableConditionDialog(QWidget *parent = nullptr);
    // QStringList getExpressions() const;
    void addExpressionRowparametre(int operationType, const QString &expression = QString());

    void addExpressionRow();
    QList<QPair<int, QString>> getExpressionsWithType() const;
private slots:

    void updateExpressionRowWidgets(int index);

private:
    struct ExpressionRow {
        QWidget *widget;
        QComboBox *operationTypeCombo;
        QComboBox *var1Combo;
        QComboBox *var2Combo;
        QComboBox *var3Combo;
        QComboBox *var4Combo;

        QComboBox *operator1Combo;
        QComboBox *operator2Combo;

        QLineEdit *constEdit1;
        QLineEdit *constEdit2;
        QComboBox *logicOperatorCombo;
        QPushButton *removeButton;
    };

    QVBoxLayout *expressionsLayout;
    QList<ExpressionRow*> expressionRows;
    QPushButton *addButton;

    QStringList variableLabels() const;
};

#endif // VARIABLECONDITIONDIALOG_H
