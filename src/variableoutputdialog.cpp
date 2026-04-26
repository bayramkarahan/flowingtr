#include "../include/variableoutputdialog.h"
#include <QHBoxLayout>
#include <QDebug>
#include <QScrollArea>
#include <QWidget>
#include<QApplication>
VariableOutputDialog::VariableOutputDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Çıktı Tanımla");
    resize(700, 300);
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
    connect(addButton, &QPushButton::clicked, this, &VariableOutputDialog::addExpressionRow);

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

QStringList VariableOutputDialog::variableLabels() const
{
    QStringList lists;
    for (const VariableRecord &var : Variable::onlineVariableList) {
        lists.append(var.label);
    }
    return lists;
}

void VariableOutputDialog::addExpressionRow()
{

    OutputRow *row = new OutputRow;

    row->widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(row->widget);
    layout->setContentsMargins(5, 5, 5, 5);


    // İşlem türü combo
    row->operationTypeCombo = new QComboBox(row->widget);
    row->operationTypeCombo->addItems({
        "📥 Değişken Göster (var0)",
        "📥 Değişken Adı İle Göster (var0 = 123)",
        "📥 Sabit Text Göster",
    });
    layout->addWidget(row->operationTypeCombo);

    // OutputMessage edit
    row->outputMessageEdit = new QLineEdit(row->widget);
    row->outputMessageEdit->setFixedWidth(150);
    row->outputMessageEdit->setPlaceholderText("Çıktı Mesajı");
    layout->addWidget(row->outputMessageEdit);


      // var1 combo
    row->varLabelCombo = new QComboBox(row->widget);
    row->varLabelCombo->addItems(variableLabels());
    layout->addWidget(row->varLabelCombo);

      // Sabit sayı için edit
    row->constEdit1 = new QLineEdit(row->widget);
    row->constEdit1->setFixedWidth(150);
    row->constEdit1->setPlaceholderText("Değer");
    layout->addWidget(row->constEdit1);

    // Kaldırma butonu
    row->removeButton = new QPushButton("🗑️", row->widget);
    row->removeButton->setToolTip("İşlemi Sil");
    layout->addWidget(row->removeButton);

    expressionsLayout->insertWidget(expressionsLayout->count() - 1, row->widget);

    outputRows.append(row);

    // İlk widget görünürlüğünü ayarla
    updateExpressionRowWidgets(outputRows.count() - 1);

    // İşlem türü değiştiğinde widgetları güncelle
    connect(row->operationTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int){
        updateExpressionRowWidgets(outputRows.indexOf(row));
    });

    // Kaldırma butonu
    connect(row->removeButton, &QPushButton::clicked, this, [=](){
        int idx = outputRows.indexOf(row);
        if (idx >= 0) {
            outputRows.removeAt(idx);
            expressionsLayout->removeWidget(row->widget);
            row->widget->deleteLater();
            delete row;
        }
    });
}

void VariableOutputDialog::addExpressionRowparametre(const VariableRecord &var)
{
    OutputRow* row = new OutputRow;

    row->widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(row->widget);
    layout->setContentsMargins(5,5,5,5);
    row->operationTypeCombo = new QComboBox(row->widget);
    row->operationTypeCombo->addItems({
        "📥 Değişken Göster (var0)",
        "📥 Değişken Adı İle Göster (var0 = 123)",
        "📥 Sabit Text Göster",
    });
    layout->addWidget(row->operationTypeCombo);

    // OutputMessage edit
    row->outputMessageEdit = new QLineEdit(row->widget);
    row->outputMessageEdit->setFixedWidth(100);
    row->outputMessageEdit->setPlaceholderText("Output Mesage");
    row->outputMessageEdit->setText(var.outputMessage);
    layout->addWidget(row->outputMessageEdit);


    row->varLabelCombo = new QComboBox(row->widget);
    row->varLabelCombo->addItems(variableLabels());
    layout->addWidget(row->varLabelCombo);

    row->constEdit1 = new QLineEdit(row->widget);
    row->constEdit1->setFixedWidth(100);
    row->constEdit1->setPlaceholderText("Değer");
    layout->addWidget(row->constEdit1);


    row->removeButton = new QPushButton("🗑️", row->widget);
    layout->addWidget(row->removeButton);

    expressionsLayout->insertWidget(expressionsLayout->count() - 1, row->widget);

    // İşlem türünü dışarıdan alıyoruz, combo'yu ayarla
    row->operationTypeCombo->setCurrentIndex(var.operationType);

    // expression'u parçalayıp ilgili widgetlara ayırmak yerine direkt dolduralım:

    // "=" ile böl


        int targetIndex = row->varLabelCombo->findText(var.expression.trimmed());
        if(targetIndex >= 0) row->varLabelCombo->setCurrentIndex(targetIndex);

        switch(var.operationType) {
        case 0: // Değişken Değeri Atama (var0)
        {
            int idx = row->varLabelCombo->findText(var.expression.trimmed());
            if (idx >= 0) row->varLabelCombo->setCurrentIndex(idx);
        }
            break;
        case 1: // Değişken Adı ve Değeri(var0 = 5)
        {
            int idx = row->varLabelCombo->findText(var.expression.trimmed());
            if (idx >= 0) row->varLabelCombo->setCurrentIndex(idx);
        }
        break;
        case 2: // Sabit Text Bilgi ()
        {
            row->constEdit1->setText(var.expression.trimmed());
        }
        break;
         }


    connect(row->operationTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int){
        int idx = outputRows.indexOf(row);
        if(idx >= 0) updateExpressionRowWidgets(idx);
    });

    connect(row->removeButton, &QPushButton::clicked, this, [=]() {
        expressionsLayout->removeWidget(row->widget);
        row->widget->deleteLater();
        outputRows.removeOne(row);
    });

    outputRows.append(row);

    updateExpressionRowWidgets(outputRows.indexOf(row));
}



void VariableOutputDialog::updateExpressionRowWidgets(int index)
{
    if (index < 0 || index >= outputRows.count())
        return;

    OutputRow *row = outputRows.at(index);
    int type = row->operationTypeCombo->currentIndex();

    // Önce tüm ek widgetları gizle
    row->varLabelCombo->setVisible(false);
    row->constEdit1->setVisible(false);

    switch (type) {
    case 0: // Sabit Değer: var0
        row->varLabelCombo->setVisible(true);
        break;
    case 1: // Değişken Değer: var0 = 123
        row->varLabelCombo->setVisible(true);
        break;
    case 2: // Sabit text Değer
        row->constEdit1->setVisible(true);
        break;
    }
}

QList<OutputRecord> VariableOutputDialog::getExpressionsWithType() const
{
    QList<OutputRecord> list;
    for (OutputRow *row : outputRows) {
        OutputRecord rec;
        rec.outputType = row->operationTypeCombo->currentIndex();
        switch (rec.outputType) {
        case 0:
            rec.expression  = QString("%1").arg(row->varLabelCombo->currentText());
            rec.label = row->varLabelCombo->currentText();
            /*if(row->outputMessageEdit->text()!="")
                rec.outputMessage="\""+row->outputMessageEdit->text()+"\", ";
            else*/
                rec.outputMessage=row->outputMessageEdit->text();
              break;
        case 1:
            rec.expression  = QString("%1").arg(row->varLabelCombo->currentText());
            rec.label = row->varLabelCombo->currentText();
           /* if(row->outputMessageEdit->text()!="")
                rec.outputMessage="\""+row->outputMessageEdit->text()+"\", ";
            else*/
                rec.outputMessage=row->outputMessageEdit->text();

            break;
        case 2:
            rec.expression = QString("%1")
                       .arg(row->constEdit1->text());
            rec.label = "";

            /*if(row->outputMessageEdit->text()!="")
                rec.outputMessage="\""+row->outputMessageEdit->text()+"\", ";
            else*/
                rec.outputMessage=row->outputMessageEdit->text();

            break;

        }
        list.append(rec);
    }
    return list;
}
