#ifndef VARIABLELOOPDIALOG_H
#define VARIABLELOOPDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QList>
#include <QDialogButtonBox>
#include<QGuiApplication>
#include<QScreen>
struct LoopRow {
    QWidget *widget;
    QComboBox *loopTypeCombo;
    QComboBox *variableLabelCombo;
    QLineEdit *startEdit;
    QLineEdit *endEdit;
    QLineEdit *stepEdit;
    QComboBox *conditionVarCombo;
    QComboBox *conditionOpCombo;
    QLineEdit *conditionValEdit;
    QPushButton *removeButton;
};
struct LoopRecord {
    int loopType;
    QString expression;
    int startValue;
    int endValue;
    int stepValue;
    QString name;
    QString label;
};
class VariableLoopDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VariableLoopDialog(QWidget *parent = nullptr);
    void addLoopRowparametre(int loopType, const QString &expression);
    QList<LoopRecord> getLoopsWithType() const;

private:
    QVBoxLayout *mainLayout;
    QVBoxLayout *loopRowsLayout;
    QList<LoopRow*> loopRows;

    QStringList variableLabels() const;
    void updateLoopRowWidgets(int index);
};

#endif // VARIABLELOOPDIALOG_H
