#ifndef CHANGE_COMBO_H
#define CHANGE_COMBO_H
#include <QDialog>
class QComboBox;
class QPushButton;
class QLineEdit;
class QLabel;

class ChangeComboDialog : public QDialog
{
public:
    ChangeComboDialog(QWidget *parent = 0);
    QComboBox * Store_Value() const { return store_combo; }
    QComboBox * Layer_Value() const { return layer_combo; }
    QLineEdit * Goods_Value() const { return storage_lineEdit; }
private:
    QComboBox *store_combo;
    QComboBox *layer_combo;
    QLineEdit *storage_lineEdit;
    QLabel *label1;
    QLabel *label2;
    QLabel *label3;
    QPushButton *okButton;
    QPushButton *cancelButton;
};

#endif // CHANGE_COMBO_H
