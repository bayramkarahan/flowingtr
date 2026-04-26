#include "../include/variableinputdialog.h"
#include <QDebug>

VariableInputDialog::VariableInputDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Değişken Seçimi");
    resize(500, 300);
    // Geçerli ekranı al (çoklu ekran varsa aktif ekranı alır)
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    // Dialog boyutu
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);

    auto mainLayout = new QVBoxLayout(this);

    rowsLayout = new QVBoxLayout();
    mainLayout->addLayout(rowsLayout);


    addButton = new QPushButton("➕ Satır Ekle", this);
    mainLayout->addWidget(addButton);

    connect(addButton, &QPushButton::clicked, this, [this]() {
        addVariableRow();
    });

    // OK ve Cancel butonları
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    // Butonlara erişip metinleri değiştiriyoruz
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    QPushButton *cancelButton = buttonBox->button(QDialogButtonBox::Cancel);
    if (okButton) okButton->setText("Tamam");
    if (cancelButton) cancelButton->setText("Vazgeç");
    mainLayout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    // Başlangıçta 1 satır ekle
   // addVariableRow();
}

void VariableInputDialog::addVariableRow(const VariableRecord &var)
{


    VariableRow *row = new VariableRow;

    row->widget = new QWidget(this);
    QHBoxLayout *hLayout = new QHBoxLayout(row->widget);
    hLayout->setContentsMargins(0, 0, 0, 0);

    // inputMessage edit
    row->inputMessageEdit = new QLineEdit(row->widget);
    row->inputMessageEdit->setPlaceholderText("Girdi Mesajı");
    row->inputMessageEdit->setText(var.inputMessage);
    hLayout->addWidget(row->inputMessageEdit);

    // Değişken seçimi combobox
    row->variableCombo = new QComboBox(row->widget);
    row->variableCombo->addItems(variableLabels());
    if (!var.label.isEmpty()) {
        int idx = row->variableCombo->findText(var.label);
        if (idx >= 0) row->variableCombo->setCurrentIndex(idx);
    }
    hLayout->addWidget(row->variableCombo);

    // Girdi checkbox
    row->inputCheckBox = new QCheckBox("Girdi", row->widget);
    row->inputCheckBox->setChecked(var.isInput);
    hLayout->addWidget(row->inputCheckBox);

    // Value edit
    row->valueEdit = new QLineEdit(row->widget);
    row->valueEdit->setPlaceholderText("Değer");
    row->valueEdit->setText(var.value);
    hLayout->addWidget(row->valueEdit);

    // Sil butonu
    row->removeButton = new QPushButton("🗑️", row->widget);
    hLayout->addWidget(row->removeButton);

    rowsLayout->addWidget(row->widget);
    variableRows.append(row);

    int index = variableRows.size() - 1;
    connect(row->removeButton, &QPushButton::clicked, this, [this, index]() {
        removeVariableRow(index);
    });
}

void VariableInputDialog::removeVariableRow(int index)
{
    if (index < 0 || index >= variableRows.size())
        return;

    VariableRow *row = variableRows.at(index);
    rowsLayout->removeWidget(row->widget);
    row->widget->deleteLater();
    variableRows.removeAt(index);

    // Bağlantıları güncelle (index için)
    for (int i = 0; i < variableRows.size(); ++i) {
        disconnect(variableRows[i]->removeButton, nullptr, nullptr, nullptr);
        connect(variableRows[i]->removeButton, &QPushButton::clicked, this, [this, i]() {
            removeVariableRow(i);
        });
    }
}

QList<VariableRecord> VariableInputDialog::getSelectedVariables() const
{
    QList<VariableRecord> selected;

    for (const VariableRow *row : variableRows) {
        VariableRecord var;
        var.label = row->variableCombo->currentText();
        var.isInput = row->inputCheckBox->isChecked();
        var.value = row->valueEdit->text();

        /*if(row->inputMessageEdit->text()!=""){

            QString strinput = row->inputMessageEdit->text();
            strinput.remove('\"').remove(',').trimmed();
            var.inputMessage="\""+strinput+"\", ";
        var.inputMessage=row->inputMessageEdit->text();
        }
        else*/
            var.inputMessage=row->inputMessageEdit->text();

        // type bilgisi için allVariables'dan eşleştirme
        for (const VariableRecord &v : Variable::onlineVariableList) {
            if (v.label == var.label) {
                var.valueType = v.valueType;
                if(!var.isInput&&var.valueType=="int"&&var.value=="")
                {
                    var.value="0";
                }
                if(!var.isInput&&var.valueType=="string"&&var.value=="")
                {
                    var.value="";
                }
                if(!var.isInput&&var.valueType=="float"&&var.value=="")
                {
                    var.value="0";
                }
                break;
            }
        }

        selected.append(var);
    }
    return selected;
}

QStringList VariableInputDialog::variableLabels() const
{
     QStringList list;

    for (const VariableRecord &var : Variable::onlineVariableList) {
        //labels.insert(var.label);
        list.append(var.label);
    }
    return list;
}
