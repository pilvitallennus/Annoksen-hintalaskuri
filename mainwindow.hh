#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include "foodfunctionality.hh"

#include <QMainWindow>
#include <QPushButton>
#include <QString>
#include <QLineEdit>
#include <QLabel>
#include <QTextBrowser>
#include <QListWidget>
#include <QTableWidget>
#include <QComboBox>
#include <vector>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    MainWindow(const MainWindow& other) = delete;
    MainWindow& operator=(const MainWindow& other) = delete;
    ~MainWindow();



private slots:
    // Luo käyttöliittymän elementit, jaoteltu toiminnoittain
    void initSearch();
    void initNormal();
    void initRecipe();

    // Muu toiminnallisuus
    void searchForProduct();
    void updateProductComboBox();
    void addChosenProduct();

    void addRecipe();

    void calculateRecipe();
    // TESTI
    void onTest2ButtonClicked();

private:
    Ui::MainWindow *ui;
    FoodFunctionality* func;

    // väliaikainen nappi, mahdollistaa testaamisen ilman pitkään kestävää selenium-hakua
    QPushButton * testButton;
    // Debug-virtaan tulostava testinappi
    QPushButton * test2Button;


    // Tuotteiden haku ja lisäys
    QLineEdit * searchLineEdit;
    QLabel * descNameLabel;     // Tekeekö desc nimellä mitään? Arvioi lopussa
    QLineEdit * descNameLineEdit;
    QPushButton * searchButton;
    QPushButton * addButton;
    QComboBox * productComboBox;


    // Reseptit
    QTableWidget * savedProductsTableWidget;
    QPushButton * addRecipeButton;
    QLabel * recipeNameLabel;
    QLineEdit * recipeNameLineEdit;

    QTableWidget * recipeTableWidget;
    QTextBrowser * recipePriceBrowser;
    QPushButton * calculateRecipeButton;

    // Muut "yleiset" widgetit
    QPushButton * closeButton;
    QTextBrowser * statusBrowser;

};
#endif // MAINWINDOW_HH
