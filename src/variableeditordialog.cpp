#include "../include/variableeditordialog.h"
#include <QHeaderView>
#include <QMessageBox>
#include "../include/variableeditform.h"

VariableEditorDialog::VariableEditorDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Variable Editor");
    resize(220, 300);
    // Geçerli ekranı al (çoklu ekran varsa aktif ekranı alır)
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    // Dialog boyutu
    int x = (screenGeometry.width() - width()) / 2;
    int y = (screenGeometry.height() - height()) / 2;
    move(x, y);

    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(3);

    tableWidget->setHorizontalHeaderLabels(QStringList() << "Adı" << "Değer" << "Veri Türü");
    tableWidget->horizontalHeader()->setStretchLastSection(true);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked);
    // Sütun başlıklarını sabit tutmak istiyorsanız:
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    tableWidget->setColumnWidth(0, 70); // Label
    tableWidget->setColumnWidth(1, 40); // Value
    tableWidget->setColumnWidth(2, 40); // Type
    //tableWidget->setColumnWidth(3, 40); // Type
    connect(tableWidget, &QTableWidget::cellDoubleClicked, this, [=](int row, int) {
        if (row < 0 || row >= tableWidget->rowCount()) return;

        VariableRecord rec;
        rec.label = tableWidget->item(row, 0)->text();
        //rec.value = tableWidget->item(row, 1)->text();
        rec.valueType = tableWidget->item(row, 2)->text();
    for (int i=0;i<Variable::onlineVariableList.size();i++) {
        if(Variable::onlineVariableList[i].label==rec.label)
        {
            rec.value=Variable::onlineVariableList[i].value;
            rec.isSecret=Variable::onlineVariableList[i].isSecret;
        }
    }
         // rec.isSecret = tableWidget->item(row, 3)->;
        VariableEditForm dialog(rec, this);
        if (dialog.exec() == QDialog::Accepted) {
            VariableRecord updated = dialog.getRecord();
            tableWidget->item(row, 0)->setText(updated.label);
            tableWidget->item(row, 1)->setText(updated.value);
            tableWidget->item(row, 2)->setText(updated.valueType);
            qDebug()<<"..."<<updated.label<<updated.value<<updated.valueType<<updated.isSecret;
            //güncelliyor

            if(!updated.isInput&&updated.valueType=="int"&&
                    (updated.value==""||updated.value=="0"))
            {
                updated.value="0";
            }
            if(!updated.isInput&&updated.valueType=="string"&&
                    (updated.value==""||updated.value=="0"))
            {
                updated.value="";
            }
            if(!updated.isInput&&updated.valueType=="float"&&
                    (updated.value==""||updated.value=="0"))
            {
                updated.value="0";
            }




            emit variableUpdateRecord(rec,updated);

            // güncel halini listeliyor
           /* for (const VariableRecord &var : Variable::onlineVariableList) {
                qDebug()<<var.label<<var.value<<var.type;
            }*/
            //readonly yapıyor
         /*   for (int row = 0; row < tableWidget->rowCount(); ++row) {
                for (int col = 0; col < tableWidget->columnCount(); ++col) {
                    QTableWidgetItem* item = tableWidget->item(row, col);
                    if (!item) {
                        item = new QTableWidgetItem();
                        tableWidget->setItem(row, col, item);
                    }
                    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                }
            }*/
        }
         ////if (dialog.exec() == QDialog::Rejected) {
            //qDebug()<<"vazgeç";
            //readonly yapıyor
            for (int row = 0; row < tableWidget->rowCount(); ++row) {
                for (int col = 0; col < tableWidget->columnCount(); ++col) {
                    QTableWidgetItem* item = tableWidget->item(row, col);
                    if (!item) {
                        item = new QTableWidgetItem();
                        tableWidget->setItem(row, col, item);
                    }
                    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                }
            }
         ///}
        loadVariables();
    });

    addButton = new QPushButton("➕ Ekle", this);
    removeButton = new QPushButton("🗑️ Sil", this);
    //saveButton = new QPushButton("Kaydet", this);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(removeButton);
    buttonLayout->addStretch();
   // buttonLayout->addWidget(saveButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0); // Genel boşluk yok
    mainLayout->addWidget(tableWidget);
    mainLayout->addLayout(buttonLayout);

    connect(addButton, &QPushButton::clicked, this, &VariableEditorDialog::addVariable);
    connect(removeButton, &QPushButton::clicked, this, &VariableEditorDialog::removeSelectedVariable);
    //connect(saveButton, &QPushButton::clicked, this, &VariableEditorDialog::updateVariablesFromTable);

    loadVariables();
}

void VariableEditorDialog::loadVariables()
{
    tableWidget->setRowCount(0);
    for (const VariableRecord &var : Variable::onlineVariableList) {
        int row = tableWidget->rowCount();
        tableWidget->insertRow(row);
        tableWidget->setItem(row, 0, new QTableWidgetItem(var.label));
        if(var.isSecret)
            tableWidget->setItem(row, 1, new QTableWidgetItem("***"));
        else
            tableWidget->setItem(row, 1, new QTableWidgetItem(var.value));

        tableWidget->setItem(row, 2, new QTableWidgetItem(var.valueType));

     }
    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        for (int col = 0; col < tableWidget->columnCount(); ++col) {
            QTableWidgetItem* item = tableWidget->item(row, col);
            if (!item) {
                item = new QTableWidgetItem();
                tableWidget->setItem(row, col, item);
            }
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        }
    }
}

void VariableEditorDialog::addVariable()
{

    int row = tableWidget->rowCount();
    int varRow=row;
    // Listeye boş VariableRecord da ekle
    VariableRecord rec;
    rec.label ="var"+QString::number(varRow);
    /*******************************************/
    bool findState=true;
    while(findState)
    {
        findState=false;
        for (int j = 0; j < Variable::onlineVariableList.size(); ++j) {
            VariableRecord var = Variable::onlineVariableList[j];
            if(rec.label==var.label)
            {
                findState=true;
                varRow++;
                rec.label ="var"+QString::number(varRow);
            }
        }
    }

    /*******************************************/

    rec.value = "0";
    rec.valueType = "int";

    Variable::onlineVariableList.append(rec);

    tableWidget->insertRow(row);
    tableWidget->setItem(row, 0, new QTableWidgetItem(rec.label));
    tableWidget->setItem(row, 1, new QTableWidgetItem(rec.value));
    tableWidget->setItem(row, 2, new QTableWidgetItem(rec.valueType));
    tableWidget->hideColumn(3); // 2. sütunu (3. sütun) gizler
  }

void VariableEditorDialog::removeSelectedVariable()
{
    int row = tableWidget->currentRow();
    if (row >= 0)
    {
        VariableRecord rec;
        rec.label = tableWidget->item(row, 0)->text();
        rec.value = tableWidget->item(row, 1)->text();

        tableWidget->removeRow(row);
    qDebug()<<"silinen kayıt"<<rec.label<<rec.value;

        for (int j = 0; j < Variable::onlineVariableList.size(); ++j) {
            VariableRecord var = Variable::onlineVariableList[j];
            if(rec.label==var.label)
            {
                    //variableWidget->loadVariables();
                Variable::onlineVariableList.removeAt(j);
            }
        }
    }
}

void VariableEditorDialog::updateVariablesFromTable()
{

    Variable::onlineVariableList.clear();

    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        VariableRecord rec;
        rec.label = tableWidget->item(row, 0)->text();
        rec.value = tableWidget->item(row, 1)->text();
        rec.valueType = tableWidget->item(row, 2)->text();
        Variable::onlineVariableList.append(rec);
        }



   // QMessageBox::information(this, "Bilgi", "Değişken listesi güncellendi.");
}

void VariableEditorDialog::onTypeChanged(const QString &type)
{
    qDebug()<<"güncellendi..";
    updateVariablesFromTable();
   /* QComboBox *combo = qobject_cast<QComboBox *>(sender());
    if (!combo)
        return;

    int row = -1;
    for (int i = 0; i < tableWidget->rowCount(); ++i) {
        if (tableWidget->cellWidget(i, 2) == combo) {
            row = i;
            break;
        }
    }

    if (row != -1 && row < Variable::onlineVariableList.size()) {
        Variable::onlineVariableList[row].type = type;
    }*/
}

