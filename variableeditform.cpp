#include "variableeditform.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

VariableEditForm::VariableEditForm(const VariableRecord &record, QWidget *parent)
    : QDialog(parent), currentRecord(record)
{
    setWindowTitle("Değişkeni Düzenle");
    resize(230, 120);
    // Geçerli ekranı al (çoklu ekran varsa aktif ekranı alır)
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    // Dialog boyutu
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);

    labelEdit = new QLineEdit(record.label, this);
    valueEdit = new QLineEdit(record.value, this);
    isSecretCB = new QCheckBox(this);
    isSecretCB->setChecked(record.isSecret);
    typeCombo = new QComboBox(this);
    typeCombo->addItems({"number", "text"});
    typeCombo->setCurrentText(record.valueType);

    okButton = new QPushButton("Kaydet", this);
    cancelButton = new QPushButton("İptal", this);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("Değiken Adı:", labelEdit);
    formLayout->addRow("Değeri:", valueEdit);
    formLayout->addRow("Veri Türü:", typeCombo);
    formLayout->addRow("Veriyi Gizle:", isSecretCB);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, [=]() {
        /*******************************************/
        bool findState=false;

            findState=false;
            for (int j = 0; j < Variable::onlineVariableList.size(); ++j) {
                VariableRecord var = Variable::onlineVariableList[j];
                ///qDebug()<<var.label<<currentRecord.label<<labelEdit->text();
                if(labelEdit->text()==var.label&&currentRecord.label!=labelEdit->text())
                {
                    findState=true;
                }
            }
            //return;
            if(findState)
            {
                QDialog dialog;
                 QLabel *label=new QLabel;
                label->setText("Zaten aynı adda değişken var. Farklı bir ad veriniz.");
                dialog.setWindowTitle("Uyarı");
                dialog.resize(200,100);
                // İçerik metni
                // "Tamam" butonu
                QPushButton *button = new QPushButton("Tamam", &dialog);
                QObject::connect(button, &QPushButton::clicked, &dialog, &QDialog::accept);

                // Dikey düzen yerleşimi
                QVBoxLayout *layout = new QVBoxLayout(&dialog);
                layout->addWidget(label);
                layout->addWidget(button);
                layout->setAlignment(button, Qt::AlignRight);

                // Diyaloğu göster
                dialog.exec();
                return;
            }

        /*******************************************/


        currentRecord.label = labelEdit->text();
        currentRecord.value = valueEdit->text();
        currentRecord.valueType = typeCombo->currentText();
        currentRecord.isSecret=isSecretCB->isChecked();
        accept();
    });

    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

VariableRecord VariableEditForm::getRecord() const
{
    return currentRecord;
}
