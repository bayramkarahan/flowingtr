#include "../include/variableprocessdialog.h"
#include <QHBoxLayout>
#include <QDebug>
#include <QScrollArea>
#include <QWidget>
#include<QApplication>
#include <QRegularExpression>

VariableProcessDialog::VariableProcessDialog(QWidget *parent) : QDialog(parent)
{

    setWindowTitle("İşlem Tanımla");
    resize(650, 300);
    // Geçerli ekranı al (çoklu ekran varsa aktif ekranı alır)
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    // Dialog boyutu
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Scroll alanı içinde expression widget'ları
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    QWidget *scrollWidget = new QWidget(scrollArea);
    scrollArea->setWidget(scrollWidget);

    expressionsLayout = new QVBoxLayout(scrollWidget);
    expressionsLayout->setContentsMargins(5,5,5,5);
    expressionsLayout->setSpacing(10);

    // İlk işlem satırı veya boş başlat
    //addExpressionRow();

    // "Yeni İşlem Ekle" butonu
    addButton = new QPushButton("➕ Yeni İşlem Ekle", this);
    connect(addButton, &QPushButton::clicked, this, &VariableProcessDialog::addExpressionRow);

    // Expressions ve addButton aynı layoutta (scroll içinde)
    expressionsLayout->addWidget(addButton);

    // Ana layouta scrollArea ekle
    mainLayout->addWidget(scrollArea);

    // OK ve Cancel butonları ana layoutun en altında
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    QPushButton *cancelButton = buttonBox->button(QDialogButtonBox::Cancel);
    if (okButton) okButton->setText("Tamam");
    if (cancelButton) cancelButton->setText("Vazgeç");
    mainLayout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}


QStringList VariableProcessDialog::variableLabels() const
{
    QStringList labels;
    for (const VariableRecord &var : Variable::onlineVariableList) {
        labels.append(var.label);
    }
    return labels;
}

void VariableProcessDialog::addExpressionRow()
{
    ProcessRow *row = new ProcessRow;
    row->widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(row->widget);
    layout->setContentsMargins(5, 5, 5, 5);


     // İşlem türü combo
    row->operationTypeCombo = new QComboBox(row->widget);
    row->operationTypeCombo->addItems({
        "📥 Sabit Atama (var = 5)",
        "📥 Değişken Atama (var = var1)",
        "📥 İki Değişkenli İşlem (var = var1 + var2)",
        "📥 Değişken + Sabit İşlem (var = var1 + 5)",
        "📥 Sabit İşlem + Sabit İşlem (var=3 + 5)",
        "📥 Fonksiyon Atama (var = Rasgele_Sayı_Üret())",
    });
    layout->addWidget(row->operationTypeCombo);

     // Hedef değişken combo
    row->variableTargetCombo = new QComboBox(row->widget);
row->variableTargetCombo->addItems(variableLabels());
    layout->addWidget(row->variableTargetCombo);

    // "=" etiketi
    row->equalLabel = new QLabel("=", row->widget);
    layout->addWidget(row->equalLabel);

    // var1 combo
    row->var1Combo = new QComboBox(row->widget);
    row->var1Combo->addItems(variableLabels());
    layout->addWidget(row->var1Combo);


    // Sabit sayı için edit
    row->constEdit1 = new QLineEdit(row->widget);
    row->constEdit1->setFixedWidth(50);
    row->constEdit1->setPlaceholderText("Sayı1");
    layout->addWidget(row->constEdit1);

    // operatör combo
    row->operatorCombo = new QComboBox(row->widget);
    row->operatorCombo->addItems({"+", "-", "*", "/"});
    layout->addWidget(row->operatorCombo);

    // var2 combo
    row->var2Combo = new QComboBox(row->widget);
    row->var2Combo->addItems(variableLabels());
    layout->addWidget(row->var2Combo);



    // Sabit sayı için edit
    row->constEdit2 = new QLineEdit(row->widget);
    row->constEdit2->setFixedWidth(50);
    row->constEdit2->setPlaceholderText("Sayı2");
    layout->addWidget(row->constEdit2);

    // Kaldırma butonu
    row->removeButton = new QPushButton("🗑️", row->widget);
    row->removeButton->setToolTip("İşlemi Sil");
    layout->addWidget(row->removeButton);

    expressionsLayout->insertWidget(expressionsLayout->count() - 1, row->widget);

    processRows.append(row);

    // İlk widget görünürlüğünü ayarla
    updateExpressionRowWidgets(processRows.count() - 1);

    // İşlem türü değiştiğinde widgetları güncelle
    connect(row->operationTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int){
        updateExpressionRowWidgets(processRows.indexOf(row));
    });

    // Kaldırma butonu
    connect(row->removeButton, &QPushButton::clicked, this, [=](){
        int idx = processRows.indexOf(row);
        if (idx >= 0) {
            processRows.removeAt(idx);
            expressionsLayout->removeWidget(row->widget);
            row->widget->deleteLater();
            delete row;
        }
    });
}

void VariableProcessDialog::addExpressionRowparametre(int operationType, const QString &expression)
{

    ProcessRow* row = new ProcessRow;

    row->widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(row->widget);
    layout->setContentsMargins(5,5,5,5);
    row->operationTypeCombo = new QComboBox(row->widget);
    row->operationTypeCombo->addItems({
        "Sabit Atama (var=5)",
        "Değişken Atama (var=var1)",
        "İki Değişkenli İşlem (var=var1 + var2)",
        "Değişken + Sabit İşlem (var=var1 + 5)",
        "Sabit İşlem + Sabit İşlem (var=3 + 5)",
        "📥 Fonksiyon Atama (var = Rasgele_Sayı_Üret())",
    });
    layout->addWidget(row->operationTypeCombo);

    // Hedef değişken combo
    row->variableTargetCombo = new QComboBox(row->widget);
    row->variableTargetCombo->addItems(variableLabels());
    layout->addWidget(row->variableTargetCombo);

    // "=" etiketi
    row->equalLabel = new QLabel("=", row->widget);
    layout->addWidget(row->equalLabel);

    // var1 combo
    row->var1Combo = new QComboBox(row->widget);
    row->var1Combo->addItems(variableLabels());
    layout->addWidget(row->var1Combo);

    // Sabit sayı için edit
    row->constEdit1 = new QLineEdit(row->widget);
    row->constEdit1->setFixedWidth(50);
    row->constEdit1->setPlaceholderText("Sayı1");
    layout->addWidget(row->constEdit1);

    // operatör combo
    row->operatorCombo = new QComboBox(row->widget);
    row->operatorCombo->addItems({"+", "-", "*", "/"});
    layout->addWidget(row->operatorCombo);

    // var2 combo
    row->var2Combo = new QComboBox(row->widget);
    row->var2Combo->addItems(variableLabels());
    layout->addWidget(row->var2Combo);


    row->constEdit2 = new QLineEdit(row->widget);
    row->constEdit2->setFixedWidth(50);
    row->constEdit2->setPlaceholderText("Sayı2");
    layout->addWidget(row->constEdit2);


    row->removeButton = new QPushButton("🗑️", row->widget);
    layout->addWidget(row->removeButton);

    expressionsLayout->insertWidget(expressionsLayout->count() - 1, row->widget);

    // İşlem türünü dışarıdan alıyoruz, combo'yu ayarla
    row->operationTypeCombo->setCurrentIndex(operationType);

    // expression'u parçalayıp ilgili widgetlara ayırmak yerine direkt dolduralım:

    // "=" ile böl
    QStringList parts = expression.split('=');
    if(parts.size() == 2) {
        QString target = parts[0].trimmed();
        QString rightExpr = parts[1].trimmed();

        int targetIndex = row->variableTargetCombo->findText(target);
        if(targetIndex >= 0) row->variableTargetCombo->setCurrentIndex(targetIndex);

        switch(operationType) {
        case 0: // Sabit Atama (var = 5)
            qDebug()<<"atama0";
            row->constEdit1->setText(rightExpr);
            break;
        case 1: // Değişken Atama (var = var1)
        {
            int idx = row->var1Combo->findText(rightExpr);
            if (idx >= 0) row->var1Combo->setCurrentIndex(idx);
        }
        break;
        case 2: // İki Değişkenli İşlem (var = var1 + var2)
        {
            // rightExpr örn: "var1 + var2"
            QStringList parts2 = rightExpr.split(QRegularExpression("\\s*[+\\-*/]\\s*"));
            if(parts2.size() == 2) {
                int idx1 = row->var1Combo->findText(parts2[0]);
                int idx2 = row->var2Combo->findText(parts2[1]);

                if(idx1 >= 0) row->var1Combo->setCurrentIndex(idx1);
                if(idx2 >= 0) row->var2Combo->setCurrentIndex(idx2);

                // operatör bul
                QRegularExpression opRx("[+\\-*/]");
                auto match = opRx.match(rightExpr);

                if(match.hasMatch()) {
                    QString op = match.captured(0);
                    int opIdx = row->operatorCombo->findText(op);
                    if(opIdx >= 0) row->operatorCombo->setCurrentIndex(opIdx);
                }
            }
        }
        break;

        case 3: // Değişken + Sabit İşlem (var = var1 + 5)
        {
            // örnek: "var1 + 6"
            QRegularExpression expRx("^(\\w+)\\s*([+\\-*/])\\s*(\\d+)$");
            auto match = expRx.match(rightExpr);

            if (match.hasMatch()) {
                QString var1 = match.captured(1);
                QString op   = match.captured(2);
                QString num  = match.captured(3);

                int idx1 = row->var1Combo->findText(var1);
                if (idx1 >= 0) row->var1Combo->setCurrentIndex(idx1);

                int opIdx = row->operatorCombo->findText(op);
                if (opIdx >= 0) row->operatorCombo->setCurrentIndex(opIdx);

                row->constEdit2->setText(num);
            }
        }
        break;

        case 4: // Sabit + Sabit İşlem (var = 3 + 5)
        {
            // örnek: "3 + 6"
            QRegularExpression expRx("^(\\d+)\\s*([+\\-*/])\\s*(\\d+)$");
            auto match = expRx.match(rightExpr);

            if (match.hasMatch()) {
                QString num1 = match.captured(1);
                QString op   = match.captured(2);
                QString num2 = match.captured(3);

                int opIdx = row->operatorCombo->findText(op);
                if (opIdx >= 0) row->operatorCombo->setCurrentIndex(opIdx);

                row->constEdit1->setText(num1);
                row->constEdit2->setText(num2);
            }
        }

        break;
        case 5: // Fonksiyon Atama (var = Random())
            qDebug()<<"atama0";
            row->constEdit1->setText(rightExpr);
            break;
        }
    }

    connect(row->operationTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int){
        int idx = processRows.indexOf(row);
        if(idx >= 0) updateExpressionRowWidgets(idx);
    });

    connect(row->removeButton, &QPushButton::clicked, this, [=]() {
        expressionsLayout->removeWidget(row->widget);
        row->widget->deleteLater();
        processRows.removeOne(row);
    });

    processRows.append(row);

    updateExpressionRowWidgets(processRows.indexOf(row));
}

void VariableProcessDialog::updateExpressionRowWidgets(int index)
{
    if (index < 0 || index >= processRows.count())
        return;

    ProcessRow *row = processRows.at(index);
    int type = row->operationTypeCombo->currentIndex();

    // Önce tüm ek widgetları gizle
    row->variableTargetCombo->setVisible(true);
    row->var1Combo->setVisible(false);
    row->operatorCombo->setVisible(false);
    row->var2Combo->setVisible(false);
    row->constEdit1->setVisible(false);
    row->constEdit2->setVisible(false);

    switch (type) {
    case 0: // Sabit Atama: var = 5
        row->constEdit1->setVisible(true);
        break;
    case 1: // Değişken Atama: var = var1
        row->var1Combo->setVisible(true);
        break;
    case 2: // İki Değişkenli İşlem: var = var1 + var2
        row->var1Combo->setVisible(true);
        row->operatorCombo->setVisible(true);
        row->var2Combo->setVisible(true);
        break;
    case 3: // Değişken + Sabit İşlem: var = var1 + 5
        row->var1Combo->setVisible(true);
        row->operatorCombo->setVisible(true);
        row->constEdit2->setVisible(true);
        break;
    case 4: // Değişken + Sabit İşlem: var = 3 + 5
        row->constEdit1->setVisible(true);
        row->operatorCombo->setVisible(true);
        row->constEdit2->setVisible(true);
        break;
    case 5: // Sabit Atama: var = 5
        row->constEdit1->setVisible(true);
        row->constEdit1->setPlaceholderText("Random Araralık (0-100) Gir");
        row->constEdit1->setText("50");
        row->constEdit1->setFixedWidth(200);
        break;
    }
}

QList<ProcessRecord> VariableProcessDialog::getExpressionsWithType() const
{
    QList<ProcessRecord> list;
    for (ProcessRow *row : processRows) {
        ProcessRecord rec;
        QString target = row->variableTargetCombo->currentText();
        rec.processType= row->operationTypeCombo->currentIndex();

        switch (rec.processType) {
        case 0:
            rec.expression = QString("%1 = %2").arg(target).arg(row->constEdit1->text());
            rec.targetLabel = row->variableTargetCombo->currentText();

            break;
        case 1:
            rec.expression = QString("%1 = %2").arg(target).arg(row->var1Combo->currentText());
            rec.targetLabel = row->variableTargetCombo->currentText();

            rec.var1Label = row->var1Combo->currentText();
            break;
        case 2:
            rec.expression = QString("%1 = %2 %3 %4")
                       .arg(target)
                       .arg(row->var1Combo->currentText())
                       .arg(row->operatorCombo->currentText())
                       .arg(row->var2Combo->currentText());
            rec.targetLabel = row->variableTargetCombo->currentText();
            rec.var1Label = row->var1Combo->currentText();
            rec.var2Label = row->var2Combo->currentText();
            break;
        case 3:
            rec.expression = QString("%1 = %2 %3 %4")
                       .arg(target)
                       .arg(row->var1Combo->currentText())
                       .arg(row->operatorCombo->currentText())
                       .arg(row->constEdit2->text());
            rec.targetLabel = row->variableTargetCombo->currentText();
            rec.var1Label = row->var1Combo->currentText();
            break;
        case 4:
            rec.expression = QString("%1 = %2 %3 %4")
                       .arg(target)
                       .arg(row->constEdit1->text())
                       .arg(row->operatorCombo->currentText())
                       .arg(row->constEdit2->text());
            rec.targetLabel = row->variableTargetCombo->currentText();
            break;
        case 5:
            rec.expression = QString("%1 = Rnd( 0 - %2 )").arg(target).arg(row->constEdit1->text());
            rec.targetLabel = row->variableTargetCombo->currentText();

            break;
        }
        list.append(rec);
    }
    return list;
}
