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
#include <algorithm>



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
    initEdit();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete func;
}

// Widgettien asetteluun voisi melkein laittaa vakiot koordinaateille

// Käyttöliittymän komponenttien luonti
void MainWindow::initNormal()
{
    // TESTI TESTI

    // tän voi melkein jättää lopulliseen
    testButton = new QPushButton(this);
    testButton->setGeometry(1300,0,70,40);
    testButton->setText("Päivitä CB");
    connect(testButton, &QPushButton::clicked, this,
            &MainWindow::updateProductComboBox);



    closeButton = new QPushButton(this);
    closeButton->setGeometry(0,0,60,40);
    closeButton->setText("Sulje");
    connect(closeButton, &QPushButton::clicked, this,
                                &MainWindow::close);


    // Ei päivity vielä jokaisella toiminnolla
    statusBrowser = new QTextBrowser(this);
    statusBrowser->setGeometry(100,0,400,40);
    statusBrowser->setText("Tervetuloa käyttämään annoksen hintalaskuria");
    statusBrowser->setAlignment(Qt::AlignCenter);

}


void MainWindow::initSearch()
{
    // Hakukenttä
    searchLineEdit = new QLineEdit(this);
    searchLineEdit->setGeometry(410,50,200,40);

    // Hakunappi
    searchButton = new QPushButton(this);
    searchButton->setGeometry(610,50,70,40);
    searchButton->setText("Hae");
    connect(searchButton, &QPushButton::clicked, this,
            &MainWindow::searchForProduct);

    // Lisätyn tuotteen kuvaava, syätteeseen perustuva nimi
    descNameLineEdit = new QLineEdit(this);
    descNameLineEdit->setGeometry(60,115,125,40);

    descNameLabel = new QLabel(this);
    descNameLabel->setGeometry(60,85,200,40);
    descNameLabel->setText("Kuvaava nimi tuotteelle: ");

    // Tuotteen valinta ja lisäys -nappi
    addButton = new QPushButton(this);
    addButton->setGeometry(185,115,70,40);
    addButton->setText("Lisää");
    addButton->setDisabled(true);
    connect(addButton, &QPushButton::clicked, this,
            &MainWindow::addChosenProduct);

    // Hakutuloksien esitys CB
    productComboBox = new QComboBox(this);
    productComboBox->setGeometry(60,50,350,40);
}


void MainWindow::initRecipe()
{
    // Kaikki tallennetut tuotteet näytön TableWidgettiin
    std::vector<std::string> allProductNames = func->getAllProducts();
    int productCount = allProductNames.size();

    savedProductsTableWidget = new QTableWidget(this);
    savedProductsTableWidget->setGeometry(60, 180, 615, 700);

    savedProductsTableWidget->setRowCount(productCount);
    savedProductsTableWidget->setColumnCount(2);

    savedProductsTableWidget->setColumnWidth(0,540);
    savedProductsTableWidget->setColumnWidth(1,40);
    savedProductsTableWidget->horizontalHeader()->hide();
    savedProductsTableWidget->verticalHeader()->hide();

    savedProductsTableWidget->setSelectionMode(QAbstractItemView::MultiSelection);


    int row = 0;
    for (const auto& productName : allProductNames)
    {
        // Testailu DESC
        std::string descName = func->getProductDescName(productName);

        QTableWidgetItem *newItem = new QTableWidgetItem(
                                        QString::fromStdString(productName));

        savedProductsTableWidget->setItem(row, 0, newItem);

        // Estää solun valinnan ja sallii vain checkboxin valinnan
        newItem->setFlags((newItem->flags()
                            & ~Qt::ItemIsEditable
                            & ~ Qt::ItemIsSelectable)
                          | Qt::ItemIsUserCheckable
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
    addRecipeButton->setGeometry(260,920,80,40);
    addRecipeButton->setText("Uusi resepti");
    connect(addRecipeButton, &QPushButton::clicked, this,
                                    &MainWindow::addRecipe);

    recipeNameLineEdit = new QLineEdit(this);
    recipeNameLineEdit->setGeometry(60,920,200,40);

    recipeNameLabel = new QLabel(this);
    recipeNameLabel->setGeometry(60,880,200,40);
    recipeNameLabel->setText("Reseptin nimi:");


    // Kaikki tallennetut reseptit QTableWidgettiin
    std::vector<std::string> allRecipeNames = func->getAllRecipes();
    int recipeCount = allRecipeNames.size();

    recipeTableWidget = new QTableWidget(this);
    recipeTableWidget->setGeometry(700, 180, 400, 260);

    recipeTableWidget->setRowCount(recipeCount);
    recipeTableWidget->setColumnCount(2);

    recipeTableWidget->setColumnWidth(0,325);
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

        newItem->setFlags((newItem->flags()
                                & ~Qt::ItemIsEditable
                                & ~Qt::ItemIsSelectable)
                                | Qt::ItemIsSelectable
                                | Qt::ItemIsEnabled);

        QDoubleSpinBox* spinBox = new QDoubleSpinBox();
        spinBox->setRange(1, 10);
        spinBox->setSingleStep(1);
        spinBox->setValue(1.0);
        recipeTableWidget->setCellWidget(recipeRow, 1, spinBox);
        recipeRow++;
    }


    recipePriceBrowser = new QTextBrowser(this);
    recipePriceBrowser->setGeometry(700,500,400,460);
    recipePriceBrowser->setText("Valitse resepti näytön vasemmasta laidasta"
                                " ja aseta syöntikertojen määrä liukusäätimestä");

    calculateRecipeButton = new QPushButton(this);
    calculateRecipeButton->setGeometry(860, 448, 80, 40);
    calculateRecipeButton->setText("Laske hinta");
    connect(calculateRecipeButton, &QPushButton::clicked, this,
            &MainWindow::calculateRecipe);
}


void MainWindow::initEdit()
{
    chackboxInfoLabel = new QLabel(this);
    chackboxInfoLabel->setGeometry(900,0,150,40);
    chackboxInfoLabel->setText("Tietokannan muokkaustila");

    // Tietokannan muokkaustilan valitsin
    databaseEditCheckBox = new QCheckBox(this);
    databaseEditCheckBox->setGeometry(1050,0,40,40);
    databaseEditCheckBox->setChecked(false);

    // Reseptin poistonappi
    deleteRecipePushButton = new QPushButton(this);
    deleteRecipePushButton->setGeometry(1100,0,120,40);
    deleteRecipePushButton->setText("Poista resepti");
    connect(deleteRecipePushButton, &QPushButton::clicked, this,
            &MainWindow::deleteRecipe);

    // Avaa reseptin tuotteet muokkaustilaan
    openRecipeEditPushButton = new QPushButton(this);
    openRecipeEditPushButton->setGeometry(1100,50,120,40);
    openRecipeEditPushButton->setText("Muokkaa reseptiä");
    connect(openRecipeEditPushButton, &QPushButton::clicked, this,
                                &MainWindow::openRecipeEditTableWidget);

    addProductToExistingRecipePushButton = new QPushButton(this);
    addProductToExistingRecipePushButton->setGeometry(940,100,160,40);
    addProductToExistingRecipePushButton->setText("Lisää reseptiin");
    connect(addProductToExistingRecipePushButton, &QPushButton::clicked, this,
                                     &MainWindow::addToExistingRecipe);
    addProductToExistingRecipePushButton->setVisible(false);

    // Muokkaa reseptiä, piilossa kunnes muokkaustila avattu
    editRecipePushButton = new QPushButton(this);
    editRecipePushButton->setGeometry(940,140,160,40);
    editRecipePushButton->setText("Päivitä muutokset ");
    connect(editRecipePushButton, &QPushButton::clicked, this,
                                          &MainWindow::updateRecipe);
    editRecipePushButton->setVisible(false);


    discardRecipeEditPushButton = new QPushButton(this);
    discardRecipeEditPushButton->setGeometry(700,140,160,40);
    discardRecipeEditPushButton->setText("Hylkää muutokset");
    connect(discardRecipeEditPushButton, &QPushButton::clicked, this,
                                        &MainWindow::discardRecipeEdit);
    discardRecipeEditPushButton->setVisible(false);

    // Reseptin tuotteet tablewidget
    recipeEditTableWidget = new QTableWidget(this);
    recipeEditTableWidget->setGeometry(1125, 180, 615, 500);

    recipeEditTableWidget->setRowCount(0);
    recipeEditTableWidget->setColumnCount(2);

    recipeEditTableWidget->setColumnWidth(0,540);
    recipeEditTableWidget->setColumnWidth(1,40);


    recipeEditTableWidget->horizontalHeader()->hide();
    recipeEditTableWidget->verticalHeader()->hide();

    recipeEditTableWidget->setSelectionMode(QAbstractItemView::MultiSelection);

    // Näytetään vasta reseptin muokkaustilassa
    recipeEditTableWidget->setVisible(false);
}



// Päätoiminnallisuus alkaa
void MainWindow::searchForProduct()
{

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


void MainWindow::updateProductComboBox()
{
    // Avaa tiedosto
    std::ifstream file("searchResults.csv");
    if (!file.is_open())
    {
        qDebug() << "searchResults ei aukea... updateProductComboBox()";
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


            // päivittää comboboksin tuote käyttäjän valittavaksi
            productComboBox->addItem(QString::fromStdString(name));


            func->addFoundProduct(name,price,pricePerKg);
            iter++;     // ei taida olla enää tärkeä=??

        }

        else
        {
            qDebug() << "Joku ei toimi rivillä: " << QString::fromStdString(line);
        }
    }



    addButton->setDisabled(false);

    file.close();
}


void MainWindow::addChosenProduct()
{
    // Tuotetta ei voi lisätä ilman kuvaavaa nimeä
    if (descNameLineEdit->text() != "")
    {
        QString selectedText = productComboBox->currentText();
        // qDebug() << "Lisättävä tuote: " << selectedText;

        std::string productName = selectedText.toStdString();
        std::string descName = descNameLineEdit->text().toStdString();

        if(func->addProduct(productName, descName))
        {
           statusBrowser->setText("Tuotteen tallentaminen onnistui");

           // Lisätään tuote käyttöliittymän TableWidgettiin
           int rowCount = savedProductsTableWidget->rowCount();
           savedProductsTableWidget->insertRow(rowCount);

           QTableWidgetItem * newItem = new QTableWidgetItem(
                                            QString::fromStdString(productName));

           savedProductsTableWidget->setItem(rowCount, 0, newItem);

           newItem->setFlags((newItem->flags()
                              & ~Qt::ItemIsEditable
                              & ~ Qt::ItemIsSelectable)
                             | Qt::ItemIsUserCheckable
                             | Qt::ItemIsEnabled);

           newItem->setCheckState(Qt::Unchecked);

           QDoubleSpinBox* spinBox = new QDoubleSpinBox();
           spinBox->setRange(0.1, 3);
           spinBox->setSingleStep(0.1);
           spinBox->setValue(1.0);
           savedProductsTableWidget->setCellWidget(rowCount, 1, spinBox);
        }
        else
        {
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
    if (recipeNameLineEdit->text() == "")
    {
        statusBrowser->setText("Reseptin luominen vaatii reseptin nimen");
        return;
    }

    std::string recipeName = recipeNameLineEdit->text().toStdString();

    // Jos reseptiä ei ole olemassa
    if(!func->recipeExists(recipeName) )
    {
        // Luo allRecipes_ map:iin avaimen reseptin nimellä ja sille tyhjän vektorin
        func->createRecipe(recipeName);

        for (int row = 0; row < savedProductsTableWidget->rowCount(); ++row)
        {
           QTableWidgetItem * item = savedProductsTableWidget->item(row, 0);

           // Valitsee kaikki tuotteet joissa checkbox valittu
           if (item->checkState() == Qt::Checked)
           {
               std::string productName = item->text().toStdString();

               // Haetaan tuotteen määräkerroin spinboxista
               QDoubleSpinBox* spinBox = qobject_cast<QDoubleSpinBox*>(
                                           savedProductsTableWidget->cellWidget(row, 1));

               double amount = spinBox->value();
               func->addProductToRecipe(recipeName, productName, amount);

               // nollataan solun tila
               spinBox->setValue(1.0);
               item->setCheckState(Qt::Unchecked);

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
        std::vector<std::pair<std::string, double>> recipeProducts = func->getRecipeProductPairs(recipeName);
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
        // Selvitetään, mikä resepti on valittu
        QTableWidgetItem* selectedRecipe = selectedItems.at(0);
        std::string recipeName = selectedRecipe->text().toStdString();

        int selectedRow = recipeTableWidget->currentRow();

        // Selvitetään, annosten lukumäärä
        QDoubleSpinBox* spinBox = qobject_cast<QDoubleSpinBox*>(
        recipeTableWidget->cellWidget(selectedRow, 1));

        double timesEaten = spinBox->value();

        // Lasketaan näillä tiedoilla annoshinta
        double pricePerServing = func->calculatePricePerServing(recipeName, timesEaten);

        // Tietojen saattaminen käyttöliittymään
        std::vector<std::pair<std::string, double>> recipeProducts =
                                                    func->getRecipeProductPairs(recipeName);

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


void MainWindow::deleteRecipe()
{
    // Reseptiä ei voi muokata ilman tietokannan muokkaustilaa
    if ( !isRecipeEditModeEnabled() )
    {
        return;
    }

    // Muokkaustilan lisäksi poistettava resepti tulee olla valittuna
    QList<QTableWidgetItem*> selectedItem = recipeTableWidget->selectedItems();
    if (selectedItem.size() == 1)
    {
        QTableWidgetItem* selectedRecipe = selectedItem.at(0);
        std::string recipeName = selectedRecipe->text().toStdString();

        if(func->deleteRecipeData(recipeName))
        {
           statusBrowser->setText("Resepti " +
                                  QString::fromStdString(recipeName)+
                                  " poistettu");
           statusBrowser->setAlignment(Qt::AlignCenter);

           // Päivitetään reseptien tablewidget
           int deletedRecipeRow = selectedItem.first()->row();
           recipeTableWidget->removeRow(deletedRecipeRow);

        }
        else
        {
           statusBrowser->setText("Reseptin " +
                                  QString::fromStdString(recipeName)+
                                  " poistaminen epäonnistui.");
           statusBrowser->setAlignment(Qt::AlignCenter);
        }
    }

    else
    {
        statusBrowser->setText("Valitse ensin poistettava resepti");
        statusBrowser->setAlignment(Qt::AlignCenter);
    }


}


void MainWindow::openRecipeEditTableWidget()
{
    // Reseptiä ei voi muokata ilman tietokannan muokkaustilaa
    if ( !isRecipeEditModeEnabled() )
    {
        return;
    }

    // Muokkaustilan lisäksi muokattavan reseptin tulee olla valittuna
    QList<QTableWidgetItem*> selectedItem = recipeTableWidget->selectedItems();
    if (selectedItem.size() == 1)
    {
        // Nollaa tablewidgetin, clear() ei toiminut
        recipeEditTableWidget->setRowCount(0);

        // Muokkausnapit näkyväksi ja muun toiminnallisuuden rajoitus
        recipeEditTableWidget->setVisible(true);
        editRecipePushButton->setVisible(true);
        discardRecipeEditPushButton->setVisible(true);
        addProductToExistingRecipePushButton->setVisible(true);

        lockFunctionalityForDatabaseEdit();



        QTableWidgetItem* selectedRecipe = selectedItem.at(0);
        std::string recipeName = selectedRecipe->text().toStdString();


        std::vector<std::pair<std::string, double>> recipeProducts;
        recipeProducts = func->getRecipeProductPairs(recipeName);



        // Lisätään tuotteetn käyttöliittymän TableWidgettiin
        int rowCount = 0;

        for (const auto& product : recipeProducts)
        {
           recipeEditTableWidget->insertRow(rowCount);

           std::string productName = product.first;
           double amount = product.second;

           QTableWidgetItem * productNameItem = new QTableWidgetItem(
               QString::fromStdString(productName));

           recipeEditTableWidget->setItem(rowCount, 0, productNameItem);

           productNameItem->setFlags((productNameItem->flags()
                                            & ~Qt::ItemIsEditable
                                            & ~ Qt::ItemIsSelectable)
                                            | Qt::ItemIsUserCheckable
                                            | Qt::ItemIsEnabled);

           productNameItem->setCheckState(Qt::Checked);


           QDoubleSpinBox* spinBox = new QDoubleSpinBox();
           spinBox->setRange(0.1, 10);
           spinBox->setSingleStep(0.1);
           spinBox->setValue(amount);
           recipeEditTableWidget->setCellWidget(rowCount, 1, spinBox);

           rowCount++;
        }
    }
    else
    {
        statusBrowser->setText("Valitse ensin muokattava resepti");
        statusBrowser->setAlignment(Qt::AlignCenter);
    }
}


void MainWindow::updateRecipe()
{
    if ( !isRecipeEditModeEnabled() )
    {
        return;
    }

    // Muokkaustilan lisäksi muokattavan reseptin tulee olla valittuna
    QList<QTableWidgetItem*> selectedItem = recipeTableWidget->selectedItems();
    if (selectedItem.size() == 1)
    {
        QTableWidgetItem * selectedRecipe = selectedItem.at(0);
        std::string recipeName = selectedRecipe->text().toStdString();

        std::vector<std::pair<std::string, double>> updatedRecipeProducts;

        for (int row = 0; row < recipeEditTableWidget->rowCount(); ++row)
        {
           QTableWidgetItem * item = recipeEditTableWidget->item(row, 0);

           // Valitsee kaikki tuotteet joissa checkbox valittu
           if (item->checkState() == Qt::Checked)
           {
               std::string productName = item->text().toStdString();

               // Haetaan tuotteen määräkerroin spinboxista
               QDoubleSpinBox* spinBox = qobject_cast<QDoubleSpinBox*>(
                   recipeEditTableWidget->cellWidget(row, 1));

               double amount = spinBox->value();
               // Lisätään pari tuotteen päivitettyyn reseptiin
               updatedRecipeProducts.push_back(std::make_pair(productName,amount));
           }
        }

        // Päivitetään allRecipes_ map
        if (func->updateRecipeProducts(recipeName,updatedRecipeProducts))
        {
           // Tyhjentää tietokannan ja lataa allRecipes_ map:in sinne takaisin
           func->syncRecipeDataBase();

           statusBrowser->setText("Reseptin " +
                                  QString::fromStdString(recipeName) +
                                  " muokkaaminen onnistui.");

           // Muokkaus onnistui, suljetaan muokkaustila)
           discardRecipeEdit();
        }
        else
        {
           statusBrowser->setText("Reseptin " +
                                  QString::fromStdString(recipeName) +
                                  " muokkaaminen epäonnistui.");
        }
    }
}


void MainWindow::addToExistingRecipe()
{
    // Reseptiä ei voi muokata ilman tietokannan muokkaustilaa
    if ( !isRecipeEditModeEnabled() )
    {
        return;
    }

    QList<QTableWidgetItem*> selectedItem = recipeTableWidget->selectedItems();

    std::string recipeName = selectedItem.at(0)->text().toStdString();

    std::vector<std::string> recipeProducts = func->getRecipeProductNames(recipeName);



    for (int row = 0; row < savedProductsTableWidget->rowCount(); ++row)
    {
        QTableWidgetItem * item = savedProductsTableWidget->item(row, 0);

        // Valitsee kaikki tuotteet joissa checkbox valittu
        if (item->checkState() == Qt::Checked)
        {
           std::string productName = item->text().toStdString();

           // Jos yritetään lisätä jo reseptissä olevaa tuotetta, skipataan tuote
           if (std::find(recipeProducts.begin(), recipeProducts.end(),
                         productName) != recipeProducts.end())
           {
               continue;

           }
           // Tarkistaa recipeEditTableWidgetin, sillä tuote voi olla
           // odottamassa lisäystä reseptiin, eikä vielä map:issa
           else if (isProductAlreadyInrecipeEditTableWidget(productName))
           {
               continue;
           }


           // Haetaan tuotteen määräkerroin spinboxista
           QDoubleSpinBox* spinBox = qobject_cast<QDoubleSpinBox*>(
               savedProductsTableWidget->cellWidget(row, 1));

           double amount = spinBox->value();

           // Luodaan tuotteelle rivi muokkaustilan tablewidgettiin
           int newRow = recipeEditTableWidget->rowCount();
           recipeEditTableWidget->insertRow(newRow);

           QTableWidgetItem *productNameItem = new QTableWidgetItem(
               QString::fromStdString(productName));

           recipeEditTableWidget->setItem(newRow, 0, productNameItem);

           productNameItem->setFlags((productNameItem->flags()
                                      & ~Qt::ItemIsEditable
                                      & ~ Qt::ItemIsSelectable)
                                     | Qt::ItemIsUserCheckable
                                     | Qt::ItemIsEnabled);

           productNameItem->setCheckState(Qt::Checked);

           QDoubleSpinBox* newSpinBox = new QDoubleSpinBox();
           newSpinBox->setRange(0.1, 10);
           newSpinBox->setSingleStep(0.1);
           newSpinBox->setValue(amount);
           recipeEditTableWidget->setCellWidget(newRow, 1, newSpinBox);

           // nollataan TableWidget
           spinBox->setValue(1.0);
           item->setCheckState(Qt::Unchecked);
        }
    }
}


void MainWindow::discardRecipeEdit()
{
    recipeEditTableWidget->setRowCount(0);

    recipeEditTableWidget->setVisible(false);
    discardRecipeEditPushButton->setVisible(false);
    editRecipePushButton->setVisible(false);
    addProductToExistingRecipePushButton->setVisible(false);
    unlockFunctionality();
}


// Aputoiminnallisuus


void MainWindow::lockFunctionalityForDatabaseEdit()
{
    searchButton->setEnabled(false);
    addButton->setEnabled(false);
    addRecipeButton->setEnabled(false);
    calculateRecipeButton->setEnabled(false);
    openRecipeEditPushButton->setEnabled(false);

    recipeTableWidget->setSelectionMode(QAbstractItemView::NoSelection);


}


void MainWindow::unlockFunctionality()
{
    searchButton->setEnabled(true);
    addButton->setEnabled(true);
    addRecipeButton->setEnabled(true);
    calculateRecipeButton->setEnabled(true);
    openRecipeEditPushButton->setEnabled(true);

    recipeTableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

}


bool MainWindow::isRecipeEditModeEnabled() const
{
    // Reseptiä ei voi muokata ilman tietokannan muokkaustilaa
    if (!databaseEditCheckBox->isChecked())
    {
        statusBrowser->setText("Laita tietokannan muokkaustila "
                               "päälle tehdäksesi muutoksia tietokantaan");
        statusBrowser->setAlignment(Qt::AlignCenter);
        return false;
    }
    else
    {
        // Tietokannan muokkaustila pois päältä vahinkojen välttämiseksi
        databaseEditCheckBox->setChecked(false);
        return true;
    }
}


bool MainWindow::isProductAlreadyInrecipeEditTableWidget(const std::string& productName)
{
    qDebug() << QString::number(recipeEditTableWidget->rowCount());

    for (int i = 0; i < recipeEditTableWidget->rowCount(); ++i)
    {
        QTableWidgetItem *existingItem = recipeEditTableWidget->item(i, 0);
        if (existingItem->text().toStdString() == productName)
        {
           return true;
        }
    }
    return false;
}









