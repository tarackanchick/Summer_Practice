#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    initGUI();

    GAParameters params;
    PolynomData poly;

    outHandle(params, poly);
    
    resize(1200, 800);
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
    btnSave = new QPushButton("Сохранить", this);

    connect(btnRun, &QPushButton::clicked, this, &MainWindow::handleClicked);
    connect(btnLoad, &QPushButton::clicked, this, &MainWindow::handleClicked);
    connect(btnGenerate, &QPushButton::clicked, this, &MainWindow::handleClicked);
    connect(btnNext, &QPushButton::clicked, this, &MainWindow::handleClicked);
    connect(btnPrev, &QPushButton::clicked, this, &MainWindow::handleClicked);
    connect(btnToStart, &QPushButton::clicked, this, &MainWindow::handleClicked);
    connect(btnToEnd, &QPushButton::clicked, this, &MainWindow::handleClicked);
    connect(btnSave, &QPushButton::clicked, this, &MainWindow::handleClicked);
    connect(btnParameters, &QPushButton::clicked, this, &MainWindow::parametersClicked);

    QList<QPushButton*> buttons = {btnToStart, btnPrev, btnNext,
        btnToEnd, btnSave, btnGenerate, btnLoad};

    for (QPushButton* btn : buttons) {
        btn->setMaximumHeight(30);
        if (btn == btnSave) {
            btn->setFixedWidth(100);
        } else if (btn == btnNext || btn == btnPrev) {
            btn->setFixedWidth(30);
        } else if (btn == btnToStart || btn == btnToEnd){
            btn->setFixedWidth(40);
        }
    }

    QLabel *lblTitle = new QLabel("Поиск максимумов полинома с помощью генетического алгоритма", this);
    lblTitle->setAlignment(Qt::AlignCenter);
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
    QLabel *lblShowGen = new QLabel("Текущее поколение:", this);
    QLabel *lblRes = new QLabel("Результат", this);
    QLabel *lblLocalMax = new QLabel("Локальные максимумы:", this);
    QLabel *lblGlobalMax = new QLabel("Глобальный максимум:", this);

    lblStepCounter = new QLabel("0/0", this);
    lblGlobalMaxVal = new QLabel("0", this);
    lblStepCounter->setAlignment(Qt::AlignCenter);

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

    cmbCrossType = new QComboBox(this);
    cmbCrossType->addItems({"Арифметическое", "Смешение"});
    cmbCrossType->setCurrentIndex(1);

    cmbMutationType = new QComboBox(this);
    cmbMutationType->addItems({"Гауссова", "Равномерная"});

    cmbSelectionType = new QComboBox(this);
    cmbSelectionType->addItems({"Турнир", "Рулетка"});

    txtCurrentPopulation = new QTextEdit(this);
    txtCurrentPopulation->setReadOnly(true);
    txtCurrentPopulation->setPlaceholderText("Здесь будет список особей...");
    txtCurrentPopulation->setMaximumHeight(150);

    txtLocalMaxVals = new QTextEdit(this);
    txtLocalMaxVals->setReadOnly(true);
    txtLocalMaxVals->setPlaceholderText("Здесь будут локальные максимумы...");
    txtLocalMaxVals->setMaximumHeight(100);

    plotPolynom = new QChartView(centralWidget());
    plotQuality = new QChartView(centralWidget());
    plotPolynom->setChart(new QChart());
    plotQuality->setChart(new QChart());

    QList<QDoubleSpinBox*> doubleSpins = {spinL, spinR, spinCrossProb,
        spinMutationProb, spinNicheRadius, spinEps};
    QList<QSpinBox*> spins = {spinCntGenerations, spinPopulationSize, spinWindow};

    for (QDoubleSpinBox *dSpin : doubleSpins){
        dSpin->setMaximumWidth(200);
        dSpin->setDecimals(6);
        dSpin->setSingleStep(0.000001);

        if (dSpin == spinL || dSpin == spinR){
            spinL->setRange(-1000.0, 1000.0);
            spinR->setRange(-1000.0, 1000.0);
        }
    }

    for (QSpinBox *spin : spins){
        spin->setMaximum(10000);
        spin->setMaximumWidth(200);
    }

    frameAdvanced = new QFrame(centralWidget());
    frameAdvanced->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);

    QHBoxLayout *horizBtnLayout = new QHBoxLayout();
    horizBtnLayout->addStretch(1);
    horizBtnLayout->addWidget(btnGenerate);
    horizBtnLayout->addWidget(btnLoad);

    QGridLayout *advancedLayout = new QGridLayout(frameAdvanced);
    advancedLayout->addWidget(lblCntGenerations, 0, 0);
    advancedLayout->addWidget(spinCntGenerations, 0, 5);
    advancedLayout->addWidget(lblPopulationSize, 1, 0);
    advancedLayout->addWidget(spinPopulationSize, 1, 5);
    advancedLayout->addWidget(lblCrossType, 2, 0);
    advancedLayout->addWidget(cmbCrossType, 2, 5);
    advancedLayout->addWidget(lblSelectionType, 3, 0);
    advancedLayout->addWidget(cmbSelectionType, 3, 5);
    advancedLayout->addWidget(lblMutationType, 4, 0);
    advancedLayout->addWidget(cmbMutationType, 4, 5);
    advancedLayout->addWidget(lblCrossProb, 5, 0);
    advancedLayout->addWidget(spinCrossProb, 5, 5);
    advancedLayout->addWidget(lblMutationProb, 6, 0);
    advancedLayout->addWidget(spinMutationProb, 6, 5);
    advancedLayout->addWidget(lblNicheRadius, 7, 0);
    advancedLayout->addWidget(spinNicheRadius, 7, 5);
    advancedLayout->addWidget(lblEps, 8, 0);
    advancedLayout->addWidget(spinEps, 8, 5);
    advancedLayout->addWidget(lblWindow, 9, 0);
    advancedLayout->addWidget(spinWindow, 9, 5);
    advancedLayout->addLayout(horizBtnLayout, 10, 0, 1, 6);

    frameAdvanced->setFixedSize(380, 380);
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

    QHBoxLayout *horizLayout4 = new QHBoxLayout();
    horizLayout4->addWidget(lblShowGen);

    QHBoxLayout *horizLayout3 = new QHBoxLayout();
    horizLayout3->setSpacing(4);
    horizLayout3->setAlignment(Qt::AlignRight);
    horizLayout3->addWidget(btnToStart);
    horizLayout3->addWidget(btnPrev);
    horizLayout3->addWidget(lblStepCounter);
    horizLayout3->addWidget(btnNext);
    horizLayout3->addWidget(btnToEnd);

    QHBoxLayout *horizLayout5 = new QHBoxLayout();
    horizLayout5->setAlignment(Qt::AlignRight);
    horizLayout5->addWidget(btnSave);

    QHBoxLayout *horizLayout6 = new QHBoxLayout();
    horizLayout6->addWidget(lblGlobalMax);
    horizLayout6->addWidget(lblGlobalMaxVal);
    horizLayout6->addStretch(1);

    QVBoxLayout *verticalLayout2 = new QVBoxLayout();
    verticalLayout2->addLayout(horizLayout4);
    verticalLayout2->addWidget(txtCurrentPopulation);
    verticalLayout2->addStretch(1);
    verticalLayout2->addWidget(lblRes);
    verticalLayout2->addWidget(lblLocalMax);
    verticalLayout2->addWidget(txtLocalMaxVals);
    verticalLayout2->addLayout(horizLayout6);
    verticalLayout2->addLayout(horizLayout5);

    QVBoxLayout *verticalLayout3 = new QVBoxLayout();
    verticalLayout3->addWidget(plotQuality, 1);
    verticalLayout3->addLayout(horizLayout3);

    QHBoxLayout *horizLayout7 = new QHBoxLayout();
    horizLayout7->addLayout(verticalLayout3, 1);
    horizLayout7->addLayout(verticalLayout2, 1);

    QWidget *containerWidget = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(containerWidget);
    mainLayout->addLayout(verticalLayout1, 0);
     mainLayout->addWidget(plotPolynom, 1);
    mainLayout->addLayout(horizLayout7, 1);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(containerWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    setCentralWidget(scrollArea);
}

void MainWindow::updateResults(std::vector<std::vector<double>>& generations, std::vector<std::pair<double, double>>& locals, double& maxVal) {
    txtCurrentPopulation->clear();
    txtLocalMaxVals->clear();

    size_t cnt = 1;
    for (auto& generation : generations) {
        QString line = "";
        for (double individ : generation) {
            line += QString("%1. %2").arg(cnt).arg(individ, 0, 'f', 2);
            cnt++;
        }
        txtCurrentPopulation->append(line);
    }

    for (const auto& local : locals) {
        QString line = QString("x = %1, y = %2").arg(local.first, 0, 'f', 2).arg(local.second, 0, 'f', 2);
        txtLocalMaxVals->append(line);
    }

    QString maxValue = QString("%1").arg(maxVal, 0, 'f', 2);
    lblGlobalMaxVal->setText(maxValue);
}

void MainWindow::handleClicked() {
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());

    if (clickedButton == btnRun) emit startRequested();
    else if (clickedButton == btnLoad) emit loadRequested();
    else if (clickedButton == btnGenerate) emit generateRequested();
    else if (clickedButton == btnNext) emit nextRequested();
    else if (clickedButton == btnPrev) emit prevRequested();
    else if (clickedButton == btnToStart) emit toBeginRequested();
    else if (clickedButton == btnToEnd) emit toEndRequested();
    else if (clickedButton == btnSave) emit saveRequested();
}

void MainWindow::arrowHandle(size_t& total, size_t& cur) {
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

void MainWindow::outHandle(GAParameters& gaParams, PolynomData& polynomData) {
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
        cmbCrossType->setCurrentText("Арифметическое");
    else cmbCrossType->setCurrentText("Смешение");

    if (gaParams.mutation == MutationType::Gaussian)
        cmbMutationType->setCurrentText("Гауссова");
    else cmbMutationType->setCurrentText("Равномерная");

    if (gaParams.selection == SelectionType::Tournament)
        cmbSelectionType->setCurrentText("Турнир");
    else cmbSelectionType->setCurrentText("Рулетка");
}

void MainWindow::drawPolynomPlot(std::vector<double>& funcValues, std::vector<double>& funcArgs){
    QChart *chart = plotPolynom->chart();
    chart->removeAllSeries();

    QLineSeries *series = new QLineSeries();
    int i = 0;

    while (i < funcValues.size()){
        series->append(funcArgs[i], funcValues[i]);
        ++i;
    }

    series->setName("Функция f(x)");
    chart->addSeries(series);
    chart->createDefaultAxes();
    plotPolynom->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::updatePlots(std::vector<GenerationSnapshot>& gaHistory, size_t& index){      //Обновление ф-и качества и максимумов на графике f(x)
    if (index < gaHistory.size()){
        QChart *chartQ = plotQuality->chart();
        QChart *chartP = plotPolynom->chart();

        chartQ->removeAllSeries();

        QLineSeries *seriesQBest = new QLineSeries();
        QLineSeries *seriesQAvg = new QLineSeries();
        seriesQBest->setName("Лучшая особь");
        seriesQAvg->setName("Среднее по популяции");

        for (int i = 0; i <= index; ++i){
            seriesQBest->append(gaHistory[i].generationNumber, gaHistory[i].bestFitness);
            seriesQAvg->append(gaHistory[i].generationNumber, gaHistory[i].avgFitness);
        }

        chartQ->addSeries(seriesQBest);
        chartQ->addSeries(seriesQAvg);
        chartQ->createDefaultAxes();

        for (QAbstractSeries *oldSeries : chartP->series()) {
            if (oldSeries->name() == "Максимумы популяции") {
                chartP->removeSeries(oldSeries);
                break;
            }
        }

        QScatterSeries *seriesP = new QScatterSeries();
        seriesP->setName("Максимумы популяции");
        seriesP->setMarkerShape(QScatterSeries::MarkerShapeCircle);
        seriesP->setMarkerSize(10.0);
        seriesP->setColor(Qt::red);
        seriesP->setBorderColor(Qt::darkRed);

        GenerationSnapshot snap = gaHistory[index];
        for (int i = 0; i < snap.maxima.size(); ++i) {
            seriesP->append(snap.maxima[i].x, snap.maxima[i].rawFitness);
        }

        chartP->addSeries(seriesP);
        chartP->createDefaultAxes();
        plotQuality->setRenderHint(QPainter::Antialiasing);
        plotPolynom->setRenderHint(QPainter::Antialiasing);
    }
}

void MainWindow::parametersClicked() {
    bool isHidden = frameAdvanced->isHidden();
    frameAdvanced->setHidden(!isHidden);
}