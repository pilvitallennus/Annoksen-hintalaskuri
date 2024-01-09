#include "mainwindow.hh"
#include "ui_mainwindow.h"
#include "foodfunctionality.hh"

#include <QDebug>
#include <QPalette>

#include <QLabel>
#include <QTextBrowser>
#include <QLineEdit>
#include <QFile>
#include <QTextStream>

#include <fstream>
#include <iostream>
#include <QString>





MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    func = new FoodFunctionality();

    // Tietokannan tiedot tuote-olioiksi
    func->pullProductsFromDatabase();

    func->pullRecipesFromDatabase();

    // Käyttöliittymän luonti
    initNormal();
    initSearch();
    initRecipe();


}

MainWindow::~MainWindow()
{
    delete ui;
    delete func;
}


// Luo käyttöliittymän osia
void MainWindow::initNormal()
{

    testButton = new QPushButton(this);
    testButton->setGeometry(100,0,70,40);
    testButton->setText("Päivitä CB");
    connect(testButton, &QPushButton::clicked, this,
            &MainWindow::updateProductComboBox);

    test2Button = new QPushButton(this);
    test2Button->setGeometry(200,0,70,40);
    test2Button->setText("Debug()");
                        connect(test2Button, &QPushButton::clicked, this,
            &MainWindow::onTest2ButtonClicked);


    closeButton = new QPushButton(this);
    closeButton->setGeometry(0,0,70,40);
    closeButton->setText("Sulje");
    connect(closeButton, &QPushButton::clicked, this,
                                &MainWindow::close);


    statusBrowser = new QTextBrowser(this);
    statusBrowser->setGeometry(400,0,300,30);
    statusBrowser->setText("Tervetuloa");
}


void MainWindow::initRecipe()
{
    // Olemassa olevat tuotteet checkboxi -listana
    // omaksi metodiksi???
    savedProductsListWidget = new QListWidget(this);
    savedProductsListWidget->setGeometry(900,40,400,700);
    savedProductsListWidget->setSelectionMode(QAbstractItemView::MultiSelection);

    std::vector<std::string> allProductNames = func->getAllProducts();
    for (const auto& productName : allProductNames)
    {
        QListWidgetItem *newItem = new QListWidgetItem;
        newItem->setText(QString::fromStdString(productName));
        savedProductsListWidget->addItem(newItem);
        newItem->setFlags(newItem->flags()  | Qt::ItemIsUserCheckable
                                            | Qt::ItemIsSelectable
                                            | Qt::ItemIsEnabled);
        newItem->setCheckState(Qt::Unchecked);
    }





    addRecipeButton = new QPushButton(this);
    addRecipeButton->setGeometry(1500,40,80,40);
    addRecipeButton->setText("Uusi resepti");
    connect(addRecipeButton, &QPushButton::clicked, this,
            &MainWindow::addRecipe);

    recipeNameLineEdit = new QLineEdit(this);
    recipeNameLineEdit -> setGeometry(1300,40,200,40);
}


void MainWindow::initSearch()
{
    // x, y, leveys, korkeus

    // hakukenttä
    searchLineEdit = new QLineEdit(this);
    searchLineEdit -> setGeometry(400,50,200,40);

    // hakunappi
    searchButton = new QPushButton(this);
    searchButton->setGeometry(600,50,70,40);
    searchButton->setText("Hae");
    connect(searchButton, &QPushButton::clicked, this,
            &MainWindow::searchForProduct);


    // lisätyn tuotteen kuvaava, söyteeseen perustuva nimi
    descNameLineEdit = new QLineEdit(this);
    descNameLineEdit -> setGeometry(50,115,125,40);


    // tuotteen valinta ja lisäys -nappi
    addButton = new QPushButton(this);
    addButton->setGeometry(175,115,70,40);
    addButton->setText("Lisää");
    addButton->setDisabled(true);
    connect(addButton, &QPushButton::clicked, this,
            &MainWindow::addChosenProduct);


    // löydetyt tuotteet
    productComboBox = new QComboBox(this);
    productComboBox->setGeometry(50,50,350,40);




}




// Toiminnallisuus
void MainWindow::searchForProduct()
{
    // Jos hakusana on asetettu
    if (!searchLineEdit->text().isEmpty())
    {
        // Hakee syötteen tekstikentästä
        std::string searchText = searchLineEdit->text().toStdString();
        qDebug() << "Haetaan tuotetta: " << searchLineEdit->text();

        productComboBox->clear();

        statusBrowser->setText("Hakee tuotteita...");
        // Tyhjennetään hakutietojen tallennus searchResults.csv
        std::ofstream clearFile(
            "searchResults.csv",
                                std::ofstream::out | std::ofstream::trunc);
        clearFile.close();

        // Ei palauta mitään, aktivoi seleniumin, joka  päivittää searchResults.csv
        func->gatherFromWeb(searchText);

        statusBrowser->setText("Haku valmis");

        // Tämän kanssa oli ongelmia, nyt toimii
        updateProductComboBox();
    }

    else
    {
        statusBrowser->setText("Kirjoita hakusanaa");
    }
}


void MainWindow::addChosenProduct()
{
    // Tuotetta ei voi lisätä ilman kuvaavaa nimeä
    if (descNameLineEdit->text() != "")
    {
        QString selectedText = productComboBox->currentText();
        qDebug() << "Lisättävä tuote: " << selectedText;

        std::string productName = selectedText.toStdString();
        std::string descName = descNameLineEdit->text().toStdString();

        if(func->addProduct(productName, descName))
        {
           statusBrowser->setText("Tuotteen tallentaminen onnistui");

           // lisätään tuote käyttöliittymän QListWidgettiin

           QListWidgetItem *newItem = new QListWidgetItem;
           newItem->setText(QString::fromStdString(productName));
           savedProductsListWidget->addItem(newItem);
           newItem->setFlags(newItem->flags()   | Qt::ItemIsUserCheckable
                                                | Qt::ItemIsSelectable
                                                | Qt::ItemIsEnabled);

           newItem->setCheckState(Qt::Unchecked);




        }
        else{
           qDebug() << "Tuotteen lisääminen ei onnistunut";
           statusBrowser->setText("Tuote on jo tallennetuissa tuotteissa");
           return;
        }




        // "Nollataan haettujen tuotteiden valinta ja lisäys -toiminnot"
        productComboBox->clear();
        addButton->setDisabled(true);
        descNameLineEdit->clear();
    }
    else
    {
        statusBrowser->setText("Anna tuotteelle kuvaava nimi");
    }

}


void MainWindow::addRecipe()
{

    std::string recipeName = recipeNameLineEdit->text().toStdString();

    qDebug();
    qDebug() << "Lisättävä resepti: " << QString::fromStdString(recipeName);
    qDebug();

    // Jos reseptiä ei ole olemassa
    if(!func->recipeExists(recipeName) )
    {
        // Luo reseptin map rakenteeseen reseptin nimellä avaimen ja tyhjän vektorin
        func->createRecipe(recipeName);

        for (int i = 0; i < savedProductsListWidget->count(); ++i)
        {
           QListWidgetItem* item = savedProductsListWidget->item(i);

           // Valitsee kaikki tuotteet joissa checkbox valittu
           if (item->checkState() == Qt::Checked)
           {
               std::string itemName = item->text().toStdString();

               // Lisää valittu tuote reseptiin määrällä 1
               func->addProductToRecipe(recipeName, itemName, 1);
           }
        }
        // Tallentaa reseptin tietokantaan
        std::vector<std::pair<std::string, int>> recipeProducts = func->getRecipeProducts(recipeName);
        func->addRecipeToDatabase(recipeName, recipeProducts);
    }


}


void MainWindow::onTest2ButtonClicked()
{
    func->tulostaRecipes();
}



void MainWindow::updateProductComboBox()
{
    // Avaa tiedosto
    std::ifstream file("searchResults.csv");
    if (!file.is_open())
    {
        qDebug() << "Search file cannot be opened";
        return;
    }

    std::string line;

    int iter = 1;

    while (std::getline(file, line) && iter < 8)
    {
        std::vector<std::string> params = func->split(line, ';');

        if (params.size() == 3)
        {
            std::replace(params[1].begin(), params[1].end(), ',', '.');
            std::replace(params[2].begin(), params[2].end(), ',', '.');

            std::string name = params[0];
            double price = std::stod(params[1]);
            double pricePerKg = std::stod(params[2]);




            // päivittää comboboksin
            productComboBox->addItem(QString::fromStdString(name));


            func->addFoundProduct(name,price,pricePerKg);
            iter++;     // ei taida olla enää tärkeä=??

        }

        else
        {
            qDebug() << "Joku ei toimi: " << QString::fromStdString(line);
        }
    }

    // testi testi
    func->tulostaFoundProductsKoko();

    addButton->setDisabled(false);

    file.close();
}


