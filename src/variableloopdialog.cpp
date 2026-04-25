#include "../include/variableloopdialog.h"
#include "../include/variable.h"

VariableLoopDialog::VariableLoopDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Tekrar(Döngü) Tanımla");
    resize(500, 200);
    // Geçerli ekranı al (çoklu ekran varsa aktif ekranı alır)
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    // Dialog boyutu
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);

    mainLayout = new QVBoxLayout(this);
    loopRowsLayout = new QVBoxLayout;

    mainLayout->addLayout(loopRowsLayout);

   // QPushButton *addButton = new QPushButton("📥 Satır Ekle");
   // mainLayout->addWidget(addButton);

   /* connect(addButton, &QPushButton::clicked, this, [=]() {
        addLoopRowparametre(0, "");
    });*/

    // OK ve Cancel butonları ana layoutun en altında
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    QPushButton *cancelButton = buttonBox->button(QDialogButtonBox::Cancel);
    if (okButton) okButton->setText("Tamam");
    if (cancelButton) cancelButton->setText("Vazgeç");
    mainLayout->addWidget(buttonBox);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &VariableLoopDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &VariableLoopDialog::reject);

}

QStringList VariableLoopDialog::variableLabels() const {
    QStringList list;
    for (const auto &v : Variable::onlineVariableList)
        list <<v.label;
    return list;
}

void VariableLoopDialog::addLoopRowparametre(int loopType, const QString &expression)
{
    LoopRow *row = new LoopRow;
    row->widget = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(row->widget);
    layout->setContentsMargins(5,5,5,5);

    row->loopTypeCombo = new QComboBox;
    row->loopTypeCombo->addItems({"📥 for(i=0;i<10;i++)",
                                  "📥 for(i=10;i>0;i--)",
                                  "📥 while(i<5)"});
    row->loopTypeCombo->setCurrentIndex(loopType);
    layout->addWidget(row->loopTypeCombo);


    row->variableLabelCombo = new QComboBox;
    row->variableLabelCombo->addItems(variableLabels());

    layout->addWidget(row->variableLabelCombo);
    row->startEdit = new QLineEdit; row->startEdit->setPlaceholderText("start");
    layout->addWidget(row->startEdit);

    row->endEdit = new QLineEdit; row->endEdit->setPlaceholderText("end");
    layout->addWidget(row->endEdit);

    row->stepEdit = new QLineEdit; row->stepEdit->setPlaceholderText("step");
    layout->addWidget(row->stepEdit);

    row->conditionVarCombo = new QComboBox;
    row->conditionVarCombo->addItems(variableLabels());
    layout->addWidget(row->conditionVarCombo);

    row->conditionOpCombo = new QComboBox;
    row->conditionOpCombo->addItems({"<", "<=", ">", ">=", "==", "!="});
    layout->addWidget(row->conditionOpCombo);

    row->conditionValEdit = new QLineEdit;
    row->conditionValEdit->setPlaceholderText("Sayı");
    layout->addWidget(row->conditionValEdit);

    row->removeButton = new QPushButton("🗑️");
    layout->addWidget(row->removeButton);

    loopRowsLayout->addWidget(row->widget);
    loopRows.append(row);

    // parse expression
    QStringList parts = expression.split(' ');
    if (loopType == 0 && parts.size() == 4) { // for i=0 to 10 step 2
        int varIdx = row->variableLabelCombo->findText(parts[0]);
        if (varIdx >= 0) {
            row->variableLabelCombo->setCurrentIndex(varIdx);
        }

        row->startEdit->setText(parts[1]);
        row->endEdit->setText(parts[2]);
        row->stepEdit->setText(parts[3]);
    }
    if (loopType == 1 && parts.size() == 4) { // for i=10 to 0 step 2
        int varIdx = row->variableLabelCombo->findText(parts[0]);
        if (varIdx >= 0) {
            row->variableLabelCombo->setCurrentIndex(varIdx);
        }

        row->startEdit->setText(parts[1]);
        row->endEdit->setText(parts[2]);
        row->stepEdit->setText(parts[3]);
    }
    else if (loopType == 2 && parts.size() == 3) { // while i < 10
        int varIdx = row->conditionVarCombo->findText(parts[0]);
        if (varIdx >= 0) row->conditionVarCombo->setCurrentIndex(varIdx);
        int opIdx = row->conditionOpCombo->findText(parts[1]);
        if (opIdx >= 0) row->conditionOpCombo->setCurrentIndex(opIdx);
        row->conditionValEdit->setText(parts[2]);
    }

    connect(row->loopTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=]() {
        int idx = loopRows.indexOf(row);
        if (idx >= 0)
            updateLoopRowWidgets(idx);
    });



    connect(row->removeButton, &QPushButton::clicked, this, [=]() {
        loopRowsLayout->removeWidget(row->widget);
        row->widget->deleteLater();
        loopRows.removeOne(row);
    });

    updateLoopRowWidgets(loopRows.indexOf(row));
}


void VariableLoopDialog::updateLoopRowWidgets(int index)
{
    if (index < 0 || index >= loopRows.size())
        return;

    LoopRow *row = loopRows.at(index);
    int type = row->loopTypeCombo->currentIndex();
    row->variableLabelCombo->setVisible(false);
    row->startEdit->setVisible(false);
    row->endEdit->setVisible(false);
    row->stepEdit->setVisible(false);
    row->conditionVarCombo->setVisible(false);
    row->conditionOpCombo->setVisible(false);
    row->conditionValEdit->setVisible(false);
    if(type == 0)
    {
   // row->variableNameEdit->setVisible(true);
    row->variableLabelCombo->setVisible(true);
    row->startEdit->setVisible(true);
    row->endEdit->setVisible(true);
    row->stepEdit->setVisible(true);
    }
    if(type == 1)
    {
        // row->variableNameEdit->setVisible(true);
        row->variableLabelCombo->setVisible(true);
        row->startEdit->setVisible(true);
        row->endEdit->setVisible(true);
        row->stepEdit->setVisible(true);
    }
    if(type == 2)
    {
       // row->variableNameEdit->setVisible(true);
        row->conditionVarCombo->setVisible(true);
        row->conditionOpCombo->setVisible(true);
        row->conditionValEdit->setVisible(true);
    }


}

QList<LoopRecord> VariableLoopDialog::getLoopsWithType() const
{
    QList<LoopRecord> list;
    for (LoopRow *row : loopRows) {
        LoopRecord rec;
        rec.loopType = row->loopTypeCombo->currentIndex();

        if (rec.loopType == 0) {
            // for döngüsü
            rec.expression = QString("%1 %2 %3 %4")
                                 .arg(row->variableLabelCombo->currentText())
                                 .arg(row->startEdit->text())
                                 .arg(row->endEdit->text())
                                 .arg(row->stepEdit->text());
            bool ok;
            rec.startValue = row->startEdit->text().toInt(&ok,10);
            rec.endValue = row->endEdit->text().toInt(&ok,10);
            rec.stepValue = row->stepEdit->text().toInt(&ok,10);
            rec.label = row->variableLabelCombo->currentText();
        } else if (rec.loopType == 1){
            // for döngüsü
            rec.expression = QString("%1 %2 %3 %4")
                                 .arg(row->variableLabelCombo->currentText())
                                 .arg(row->startEdit->text())
                                 .arg(row->endEdit->text())
                                 .arg(row->stepEdit->text());
            bool ok;
            rec.startValue = row->startEdit->text().toInt(&ok,10);
            rec.endValue = row->endEdit->text().toInt(&ok,10);
            rec.stepValue = row->stepEdit->text().toInt(&ok,10);
            rec.label = row->variableLabelCombo->currentText();
        }else if (rec.loopType == 2){
            // while
            rec.expression = QString("%1 %2 %3")
                                 .arg(row->conditionVarCombo->currentText())
                                 .arg(row->conditionOpCombo->currentText())
                                 .arg(row->conditionValEdit->text());
            bool ok;
            rec.startValue = row->startEdit->text().toInt(&ok,10);
            rec.endValue = row->conditionValEdit->text().toInt(&ok,10);
            rec.stepValue = row->stepEdit->text().toInt(&ok,10);
            rec.label = row->variableLabelCombo->currentText();

        }

        list.append(rec);
    }
    return list;
}
