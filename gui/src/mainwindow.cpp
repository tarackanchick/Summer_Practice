#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    initGUI();
}

void MainWindow::initGUI() {
    btnRun = new QPushButton("Старт", this);
    btnParameters = new QPushButton("Параметры", this);
    btnLoad = new QPushButton("Загрузить", this);
    btnGenerate = new QPushButton("Сгенерировать", this);
    btnNext = new QPushButton(">", this);
    btnPrev = new QPushButton("<", this);
    btnToStart = new QPushButton("<<", this);
    btnToEnd = new QPushButton(">>", this);
    btnSkip = new QPushButton("Пропустить", this);
    btnSave = new QPushButton("Сохранить", this);

    connect(btnRun, &QPushButton::clicked, this, &MainWindow::handleClicked);
    connect(btnLoad, &QPushButton::clicked, this, &MainWindow::handleClicked);
    connect(btnGenerate, &QPushButton::clicked, this, &MainWindow::handleClicked);
    connect(btnNext, &QPushButton::clicked, this, &MainWindow::handleClicked);
    connect(btnPrev, &QPushButton::clicked, this, &MainWindow::handleClicked);
    connect(btnToStart, &QPushButton::clicked, this, &MainWindow::handleClicked);
    connect(btnToEnd, &QPushButton::clicked, this, &MainWindow::handleClicked);
    connect(btnSkip, &QPushButton::clicked, this, &MainWindow::handleClicked);
    connect(btnParameters, &QPushButton::clicked, this, &MainWindow::parametersClicked);

    QLabel *lblTitle = new QLabel("Поиск максимумов полинома с помощью генетического алгоритма", this);
    QLabel *lblPolynom = new QLabel("Полином f(x):", this);
    QLabel *lblInterval = new QLabel("Интервал", this);
    QLabel *lblLeftVal = new QLabel("l:", this);
    QLabel *lblRightVal = new QLabel("r:", this);
    QLabel *lblCntGenerations = new QLabel("Количество поколений:", this);
    QLabel *lblPopulationSize = new QLabel("Размер популяции:", this);
    QLabel *lblSelectionType = new QLabel("Тип селекции:", this);
    QLabel *lblCrossType = new QLabel("Тип скрещивания:", this);
    QLabel *lblMutationType = new QLabel("Тип мутации:", this);
    QLabel *lblCrossProb = new QLabel("Вероятность скрещивания:", this);
    QLabel *lblMutationProb = new QLabel("Вероятность мутации:", this);
    QLabel *lblNicheRadius = new QLabel("Радиус ниши:", this);
    QLabel *lblEps = new QLabel("Порог стагнации:", this);
    QLabel *lblWindow = new QLabel("Окно стагнации:", this);
    QLabel *lblShowGen = new QLabel("Показать поколение:", this);
    QLabel *lblRes = new QLabel("Результат", this);
    QLabel *lblLocalMax = new QLabel("Локальные максимумы:", this);
    QLabel *lblGlobalMax = new QLabel("Глобальный максимум:", this);

    lblStepCounter = new QLabel("0/0", this);
    lblGlobalMaxVal = new QLabel("0", this);

    linePolynom = new QLineEdit(this);

    spinL = new QDoubleSpinBox(this);
    spinR = new QDoubleSpinBox(this);
    spinCrossProb = new QDoubleSpinBox(this);
    spinMutationProb = new QDoubleSpinBox(this);
    spinNicheRadius = new QDoubleSpinBox(this);
    spinEps = new QDoubleSpinBox(this);

    spinCntGenerations = new QSpinBox(this);
    spinPopulationSize = new QSpinBox(this);
    spinWindow = new QSpinBox(this);
    spinShowGen = new QSpinBox(this);

    cmbCrossType = new QComboBox(this);
    cmbCrossType->addItems({"Арифметический", "Смешение"});

    cmbMutationType = new QComboBox(this);
    cmbMutationType->addItems({"Гауссова", "Равномерная"});

    cmbSelectionType = new QComboBox(this);
    cmbSelectionType->addItems({"Турнир", "Рулетка"});

    txtCurrentPopulation = new QTextEdit(this);
    txtCurrentPopulation->setReadOnly(true);
    txtCurrentPopulation->setPlaceholderText("Здесь будет список особей...");
    txtCurrentPopulation->setMaximumHeight(100);

    txtLocalMaxVals = new QTextEdit(this);
    txtLocalMaxVals->setReadOnly(true);
    txtLocalMaxVals->setPlaceholderText("Здесь будут локальные максимумы...");
    txtLocalMaxVals->setMaximumHeight(50);

    plotPlaceholder1 = new QLabel("Место под график функции полинома", this);
    plotPlaceholder1->setAlignment(Qt::AlignCenter);
    plotPlaceholder1->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    plotPlaceholder1->setMinimumSize(400, 150);

    plotPlaceholder2 = new QLabel("Место под график функции качества", this);
    plotPlaceholder2->setAlignment(Qt::AlignCenter);
    plotPlaceholder2->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    plotPlaceholder2->setMinimumSize(300, 180);

    frameAdvanced = new QFrame(this);
    frameAdvanced->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);

    QGridLayout *advancedLayout = new QGridLayout(frameAdvanced);
    advancedLayout->addWidget(lblCntGenerations, 0, 0);
    advancedLayout->addWidget(spinCntGenerations, 0, 1);
    advancedLayout->addWidget(lblPopulationSize, 1, 0);
    advancedLayout->addWidget(spinPopulationSize, 1, 1);
    advancedLayout->addWidget(lblCrossType, 2, 0);
    advancedLayout->addWidget(cmbCrossType, 2, 1);
    advancedLayout->addWidget(lblSelectionType, 3, 0);
    advancedLayout->addWidget(cmbSelectionType, 3, 1);
    advancedLayout->addWidget(lblMutationType, 4, 0);
    advancedLayout->addWidget(cmbMutationType, 4, 1);
    advancedLayout->addWidget(lblCrossProb, 5, 0);
    advancedLayout->addWidget(spinCrossProb, 5, 1);
    advancedLayout->addWidget(lblMutationProb, 6, 0);
    advancedLayout->addWidget(spinMutationProb, 6, 1);
    advancedLayout->addWidget(lblNicheRadius, 7, 0);
    advancedLayout->addWidget(spinNicheRadius, 7, 1);
    advancedLayout->addWidget(lblEps, 8, 0);
    advancedLayout->addWidget(spinEps, 8, 1);
    advancedLayout->addWidget(lblWindow, 9, 0);
    advancedLayout->addWidget(spinWindow, 9, 1);
    advancedLayout->addWidget(btnGenerate, 10, 0);
    advancedLayout->addWidget(btnLoad, 10, 1);

    frameAdvanced->setHidden(true);

    QHBoxLayout *horizLayout1 = new QHBoxLayout();
    horizLayout1->addWidget(lblPolynom);
    horizLayout1->addWidget(linePolynom);
    horizLayout1->addWidget(lblInterval);
    horizLayout1->addWidget(lblLeftVal);
    horizLayout1->addWidget(spinL);
    horizLayout1->addWidget(lblRightVal);
    horizLayout1->addWidget(spinR);
    horizLayout1->addWidget(btnRun);

    btnParameters->setCheckable(true);
    btnParameters->setChecked(false);

    QVBoxLayout *verticalLayout1 = new QVBoxLayout();
    verticalLayout1->addWidget(lblTitle);
    verticalLayout1->addLayout(horizLayout1);
    verticalLayout1->addWidget(btnParameters);
    verticalLayout1->addWidget(frameAdvanced);
    verticalLayout1->addStretch();

    QHBoxLayout *horizLayout4 = new QHBoxLayout();
    horizLayout4->addWidget(lblShowGen);
    horizLayout4->addWidget(spinShowGen);

    QHBoxLayout *horizLayout3 = new QHBoxLayout();
    horizLayout3->addWidget(btnToStart);
    horizLayout3->addWidget(btnPrev);
    horizLayout3->addWidget(lblStepCounter);
    horizLayout3->addWidget(btnNext);
    horizLayout3->addWidget(btnToEnd);
    horizLayout3->addWidget(btnSkip);
    horizLayout3->addWidget(btnSave);

    QVBoxLayout *verticalLayout2 = new QVBoxLayout();
    verticalLayout2->addLayout(horizLayout4);
    verticalLayout2->addWidget(txtCurrentPopulation);
    verticalLayout2->addWidget(lblRes);
    verticalLayout2->addWidget(lblLocalMax);
    verticalLayout2->addWidget(txtLocalMaxVals);
    verticalLayout2->addWidget(lblGlobalMax);
    verticalLayout2->addWidget(lblGlobalMaxVal);

    QHBoxLayout *horizLayout5 = new QHBoxLayout();
    horizLayout5->addWidget(plotPlaceholder2, 1);
    horizLayout5->addLayout(verticalLayout2, 1);

    QWidget *containerWidget = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(containerWidget);
    mainLayout->addLayout(verticalLayout1);
    mainLayout->addWidget(plotPlaceholder1);
    mainLayout->addLayout(horizLayout5);
    mainLayout->addLayout(horizLayout3);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(containerWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    setCentralWidget(scrollArea);
}

void MainWindow::updateResults(std::vector<std::vector<double>> generations, std::vector<double> locals, double maxVal) {
    txtCurrentPopulation->clear();
    txtLocalMaxVals->clear();

    for (auto& generation : generations) {
        QString line = "";
        for (double individ : generation) {
            line += QString("%1 ").arg(individ, 0, 'f', 2);
        }
        txtCurrentPopulation->append(line);
    }

    for (double local : locals) {
        QString line = QString("x = %1").arg(local, 0, 'f', 2);
        txtLocalMaxVals->append(line);
    }

    QString maxValue = QString("%1").arg(maxVal, 0, 'f', 2);
    lblGlobalMaxVal->setText(maxValue);
}

void MainWindow::handleClicked() {
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());

    if (clickedButton == btnRun)           startClicked = true;
    else if (clickedButton == btnLoad)     loadClicked = true;
    else if (clickedButton == btnSave)     saveClicked = true;
    else if (clickedButton == btnGenerate) generateClicked = true;
    else if (clickedButton == btnNext)     nextClicked = true;
    else if (clickedButton == btnPrev)     prevClicked = true;
    else if (clickedButton == btnToStart)  toBeginClicked = true;
    else if (clickedButton == btnToEnd)    toEndClicked = true;
    else if (clickedButton == btnSkip)     skipClicked = true;
}


void MainWindow::arrowHandle(size_t total, size_t cur) {
    QString line = QString("%1/%2").arg(cur).arg(total);
    lblStepCounter->setText(line);
}

std::pair<GAParameters, PolynomData> MainWindow::inputHandle() {
    GAParameters gaParams;
    PolynomData polynomData;

    polynomData.polynom = linePolynom->text().toStdString();
    polynomData.leftBorder = spinL->value();
    polynomData.rightBorder = spinR->value();

    gaParams.maxGeneration = spinCntGenerations->value();
    gaParams.populationSize = spinPopulationSize->value();
    gaParams.crossoverProb = spinCrossProb->value();
    gaParams.mutationProb = spinMutationProb->value();
    gaParams.nicheRadius = spinNicheRadius->value();
    gaParams.stagnationEps = spinEps->value();
    gaParams.stagnationWindow = spinWindow->value();

    if (cmbCrossType->currentText().toStdString() == "Арифметическое")
        gaParams.crossover = CrossoverType::Arithmetic;
    else gaParams.crossover = CrossoverType::BLX;

    if (cmbMutationType->currentText().toStdString() == "Гауссова")
        gaParams.mutation = MutationType::Gaussian;
    else gaParams.mutation = MutationType::Uniform;

    if (cmbSelectionType->currentText().toStdString() == "Турнир")
        gaParams.selection = SelectionType::Tournament;
    else gaParams.selection = SelectionType::Roulette;

    return std::make_pair(gaParams, polynomData);
}

void MainWindow::outHandle(GAParameters gaParams, PolynomData polynomData) {
    linePolynom->setText(QString::fromStdString(polynomData.polynom));
    spinL->setValue(polynomData.leftBorder);
    spinR->setValue(polynomData.rightBorder);

    spinCntGenerations->setValue(gaParams.maxGeneration);
    spinPopulationSize->setValue(gaParams.populationSize);
    spinCrossProb->setValue(gaParams.crossoverProb);
    spinMutationProb->setValue(gaParams.mutationProb);
    spinNicheRadius->setValue(gaParams.nicheRadius);
    spinEps->setValue(gaParams.stagnationEps);
    spinWindow->setValue(gaParams.stagnationWindow);

    if (gaParams.crossover == CrossoverType::Arithmetic)
        cmbCrossType->setCurrentText("Турнир");
    else cmbCrossType->setCurrentText("Рулетка");

    if (gaParams.mutation == MutationType::Gaussian)
        cmbMutationType->setCurrentText("Гауссова");
    else cmbMutationType->setCurrentText("Равномерная");

    if (gaParams.selection == SelectionType::Tournament)
        cmbSelectionType->setCurrentText("Арифметическое");
    else cmbSelectionType->setCurrentText("Смешение");
}

void MainWindow::parametersClicked() {
    bool isHidden = frameAdvanced->isHidden();
    frameAdvanced->setHidden(!isHidden);
}