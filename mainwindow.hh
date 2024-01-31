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
#include <QCheckBox>
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
    void initNormal();
    void initSearch();
    void initRecipe();
    void initEdit();

    // Päätoiminnot
    void searchForProduct();
    void updateProductComboBox();
    void addChosenProduct();

    void addRecipe();
    void calculateRecipe();
    void deleteRecipe();

    void openRecipeEditTableWidget();
    void updateRecipe();
    void addToExistingRecipe();

    void discardRecipeEdit();
    void lockFunctionalityForDatabaseEdit();
    void unlockFunctionality();




private:
    Ui::MainWindow *ui;
    FoodFunctionality* func;

    bool isRecipeEditModeEnabled() const;
    bool isProductAlreadyInrecipeEditTableWidget(const std::string& productName);

    // väliaikainen nappi, mahdollistaa testaamisen ilman pitkään kestävää selenium-hakua
    QPushButton * testButton;


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

    // Tietokannan muokkaus
    QCheckBox * databaseEditCheckBox;
    QTableWidget * recipeEditTableWidget;
    QPushButton * deleteRecipePushButton;
    QPushButton * openRecipeEditPushButton;
    QPushButton * editRecipePushButton;
    QPushButton * discardRecipeEditPushButton;
    QPushButton * addProductToExistingRecipePushButton;

    // Muut "yleiset" widgetit
    QPushButton * closeButton;
    QTextBrowser * statusBrowser;
    QLabel * chackboxInfoLabel;




};
#endif // MAINWINDOW_HH
