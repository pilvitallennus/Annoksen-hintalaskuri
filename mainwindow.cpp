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
#include <QDoubleSpinBox>

#include <QTableWidget>
#include <QHeaderView>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    func = new FoodFunctionality();

    // "Tietokantojen" tuonti tuote-olioiksi
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


// Käyttöliittymän luonti initit:
void MainWindow::initNormal()
{
    // TESTI TESTI

    // tän voi melkein jättää lopulliseen
    testButton = new QPushButton(this);
    testButton->setGeometry(100,0,70,40);
    testButton->setText("Päivitä CB");
    connect(testButton, &QPushButton::clicked, this,
            &MainWindow::updateProductComboBox);

    // tätä ei lopulliseen
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


    // Ei päivity jokaisella kerralla kun pitäisi
    statusBrowser = new QTextBrowser(this);
    statusBrowser->setGeometry(400,0,300,30);
    statusBrowser->setText("Tervetuloa");
}


void MainWindow::initRecipe()
{
    // Kaikki tallennetut tuotteet näytön TableWidgettiin
    std::vector<std::string> allProductNames = func->getAllProducts();
    int productCount = allProductNames.size();

    savedProductsTableWidget = new QTableWidget(this);
    savedProductsTableWidget->setGeometry(700, 40, 600, 900);

    savedProductsTableWidget->setRowCount(productCount);
    savedProductsTableWidget->setColumnCount(2);

    savedProductsTableWidget->setColumnWidth(0,520);
    savedProductsTableWidget->setColumnWidth(1,40);
    savedProductsTableWidget->horizontalHeader()->hide();
    savedProductsTableWidget->verticalHeader()->hide();


    savedProductsTableWidget->setSelectionMode(QAbstractItemView::MultiSelection);


    int row = 0;
    for (const auto& productName : allProductNames)
    {
        // testailua
        std::string descName = func->getProductDescName(productName);

        QTableWidgetItem *newItem = new QTableWidgetItem(
                                        QString::fromStdString(productName));

        savedProductsTableWidget->setItem(row, 0, newItem);

        newItem->setFlags(newItem->flags()
                          | Qt::ItemIsUserCheckable
                          | Qt::ItemIsSelectable
                          | Qt::ItemIsEnabled);

        newItem->setCheckState(Qt::Unchecked);

        QDoubleSpinBox* spinBox = new QDoubleSpinBox();
        spinBox->setRange(0.1, 3);
        spinBox->setSingleStep(0.1);
        spinBox->setValue(1.0);
        savedProductsTableWidget->setCellWidget(row, 1, spinBox);
        row++;
    }


    addRecipeButton = new QPushButton(this);
    addRecipeButton->setGeometry(1500,40,80,40);
    addRecipeButton->setText("Uusi resepti");
    connect(addRecipeButton, &QPushButton::clicked, this,
                                    &MainWindow::addRecipe);

    recipeNameLineEdit = new QLineEdit(this);
    recipeNameLineEdit->setGeometry(1300,40,200,40);

    recipeNameLabel = new QLabel(this);
    recipeNameLabel->setGeometry(1300,10,200,40);
    recipeNameLabel->setText("Reseptin nimi:");


    std::vector<std::string> allRecipeNames = func->getAllRecipes();
    int recipeCount = allRecipeNames.size();

    recipeTableWidget = new QTableWidget(this);
    recipeTableWidget->setGeometry(1300, 300, 600, 900);

    recipeTableWidget->setRowCount(recipeCount);
    recipeTableWidget->setColumnCount(2);

    recipeTableWidget->setColumnWidth(0,520);
    recipeTableWidget->setColumnWidth(1,40);
    recipeTableWidget->horizontalHeader()->hide();
    recipeTableWidget->verticalHeader()->hide();


    recipeTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);


    int recipeRow = 0;
    for (const auto& recipeName : allRecipeNames)
    {
        QTableWidgetItem *newItem = new QTableWidgetItem(
        QString::fromStdString(recipeName));

        recipeTableWidget->setItem(recipeRow, 0, newItem);



        QDoubleSpinBox* spinBox = new QDoubleSpinBox();
        spinBox->setRange(1, 10);
        spinBox->setSingleStep(1);
        spinBox->setValue(1.0);
        recipeTableWidget->setCellWidget(recipeRow, 1, spinBox);
        recipeRow++;
    }


    recipePriceBrowser = new QTextBrowser(this);
    recipePriceBrowser->setGeometry(50,225,600,600);
    recipePriceBrowser->setText("Valitse resepti näytön vasemmasta laidasta"
                                " ja aseta syöntikertojen määrä liukusäätimestä");

    calculateRecipeButton = new QPushButton(this);
    calculateRecipeButton->setGeometry(1300, 260, 70, 40);
    calculateRecipeButton->setText("Laske hinta");
    connect(calculateRecipeButton, &QPushButton::clicked, this,
            &MainWindow::calculateRecipe);
}


void MainWindow::initSearch()
{
    // Hakukenttä
    searchLineEdit = new QLineEdit(this);
    searchLineEdit->setGeometry(400,50,200,40);

    // Hakunappi
    searchButton = new QPushButton(this);
    searchButton->setGeometry(600,50,70,40);
    searchButton->setText("Hae");
    connect(searchButton, &QPushButton::clicked, this,
            &MainWindow::searchForProduct);

    // Lisätyn tuotteen kuvaava, syätteeseen perustuva nimi
    descNameLineEdit = new QLineEdit(this);
    descNameLineEdit->setGeometry(50,115,125,40);

    descNameLabel = new QLabel(this);
    descNameLabel->setGeometry(50,85,200,40);
    descNameLabel->setText("Kuvaava nimi tuotteelle: ");

    // Tuotteen valinta ja lisäys -nappi
    addButton = new QPushButton(this);
    addButton->setGeometry(175,115,70,40);
    addButton->setText("Lisää");
    addButton->setDisabled(true);
    connect(addButton, &QPushButton::clicked, this,
            &MainWindow::addChosenProduct);

    // Hakutuloksien esitys CB
    productComboBox = new QComboBox(this);
    productComboBox->setGeometry(50,50,350,40);
}



// Toiminnallisuus alkaa
void MainWindow::searchForProduct()
{
    qDebug() << "";
    qDebug() << "hakunappia painettu";
    // Ei tyhjiä hakuja
    if (!searchLineEdit->text().isEmpty())
    {
        // Hakee syötteen tekstikentästä
        std::string searchText = searchLineEdit->text().toStdString();

        productComboBox->clear();
        statusBrowser->setText("Hakee tuotteita...");

        // Tyhjennetään hakutietojen tallennus searchResults.csv
        std::ofstream clearFile(
            "searchResults.csv",
                                std::ofstream::out | std::ofstream::trunc);
        clearFile.close();

        // Ei palauta mitään, aktivoi seleniumin, joka  päivittää searchResults.csv
        if (func->gatherFromWeb(searchText))
        {
            statusBrowser->setText("Haku valmis");
        }
        else
        {
            statusBrowser->setText("Haku epäonnistui");
        }

        // Tämän kanssa oli aluksi ongelmia, nyt toimii
        updateProductComboBox();
    }
    else
    {
        statusBrowser->setText("Kirjoita hakusana!");
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

           // Lisätään tuote käyttöliittymän TableWidgettiin
           int rowCount = savedProductsTableWidget->rowCount();
           savedProductsTableWidget->insertRow(rowCount);

           QTableWidgetItem *productNameItem = new QTableWidgetItem(
                                                    QString::fromStdString(productName));

           savedProductsTableWidget->setItem(rowCount, 0, productNameItem);

           productNameItem->setFlags(productNameItem->flags()
                                     | Qt::ItemIsUserCheckable
                                     | Qt::ItemIsSelectable
                                     | Qt::ItemIsEnabled);

           productNameItem->setCheckState(Qt::Unchecked);

           QDoubleSpinBox* spinBox = new QDoubleSpinBox();
           spinBox->setRange(0.1, 3);
           spinBox->setSingleStep(0.1);
           spinBox->setValue(1.0);
           savedProductsTableWidget->setCellWidget(rowCount, 1, spinBox);
        }
        else
        {
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

    qDebug() << "\nLisättävä resepti: " << QString::fromStdString(recipeName) << "\n";;

    // Jos reseptiä ei ole olemassa
    if(!func->recipeExists(recipeName) )
    {
        // Luo allRecipes_ map:iin avaimen reseptin nimellä ja sille tyhjän vektorin
        func->createRecipe(recipeName);

        for (int row = 0; row < savedProductsTableWidget->rowCount(); ++row)
        {
           QTableWidgetItem *item = savedProductsTableWidget->item(row, 0);

           // Valitsee kaikki tuotteet joissa checkbox valittu
           if (item->checkState() == Qt::Checked)
           {
               std::string productName = item->text().toStdString();

               // Haetaan tuotteen määräkerroin spinboxista
               QDoubleSpinBox* spinBox = qobject_cast<QDoubleSpinBox*>(
                                           savedProductsTableWidget->cellWidget(row, 1));

               if (spinBox)
               {
                   double amount = spinBox->value();
                   func->addProductToRecipe(recipeName, productName, amount);

                   // nollataan TableWidget
                   spinBox->setValue(1.0);
                   item->setCheckState(Qt::Unchecked);
               }
           }
           else
           {
               // Spinboxia on voitu säätää vaikka tuotetta ei olisi valittu
               QDoubleSpinBox* spinBox = qobject_cast<QDoubleSpinBox*>(
                                           savedProductsTableWidget->cellWidget(row, 1));

               spinBox->setValue(1.0);
           }
        }

        // Tallentaa reseptin tietokantaan
        std::vector<std::pair<std::string, double>> recipeProducts = func->getRecipeProducts(recipeName);
        func->addRecipeToDatabase(recipeName, recipeProducts);

        // Lisää reseptin reseptin tablewidgettiin

        // Lisätään tuote käyttöliittymän recipeTableWidgettiin
        int recipeRow = recipeTableWidget->rowCount();
        recipeTableWidget->insertRow(recipeRow);

        QTableWidgetItem *recipeNameItem = new QTableWidgetItem(
            QString::fromStdString(recipeName));

        recipeTableWidget->setItem(recipeRow, 0, recipeNameItem);


        QDoubleSpinBox* spinBox = new QDoubleSpinBox();
        spinBox->setRange(1, 10);
        spinBox->setSingleStep(1);
        spinBox->setValue(1.0);
        recipeTableWidget->setCellWidget(recipeRow, 1, spinBox);

    }
}


void MainWindow::calculateRecipe()
{
    QList<QTableWidgetItem*> selectedItems = recipeTableWidget->selectedItems();

    if (!selectedItems.isEmpty()) {
        QTableWidgetItem* selectedRecipe = selectedItems.at(0);
        std::string recipeName = selectedRecipe->text().toStdString();

        int selectedRow = recipeTableWidget->currentRow();
        QDoubleSpinBox* spinBox = qobject_cast<QDoubleSpinBox*>(
        recipeTableWidget->cellWidget(selectedRow, 1));

        double timesEaten = spinBox->value();

        double pricePerServing = func->calculatePricePerServing(recipeName, timesEaten);

        std::vector<std::pair<std::string, double>> recipeProducts =
                                                    func->getRecipeProducts(recipeName);

        recipeTableWidget->clearSelection();

        recipePriceBrowser->clear();
        recipePriceBrowser->append("Reseptin nimi: " +
                                   QString::fromStdString(recipeName) + "\n");


        for (const auto& product : recipeProducts)
        {
           std::string productName = product.first;
           // Vektorista tuotteen määtäkerroin
           double amount = product.second;

           // Haetaan tuotetiedot
           double productPrice =  func->getProductPrice(productName);
           std::string descName = func->getProductDescName(productName);

           // Voidaan vaihtaa productName
           recipePriceBrowser->append(QString::fromStdString(descName) +
                                      " -- yksittäishinta: " +
                                      QString::number(productPrice) +
                                      " x " +
                                      QString::number(amount));


        }
        recipePriceBrowser->append("\nAnnoshinnaksi " +
                                   QString::number(timesEaten) +
                                   " syöntikerralla muodostui: " +
                                   QString::number(pricePerServing, 'f', 2) + " euroa.");

    }
}






// TESTIJUTUT JOTKA LÄHTEE POIS KUITENKIN

void MainWindow::onTest2ButtonClicked()
{
    func->calculatePricePerServing("jauhelihakastike",5);
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


