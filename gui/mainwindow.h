#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Types.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QComboBox>
#include <QTextEdit>
#include <QFrame>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <vector>
#include <utility>
#include <QScrollArea>
#include "GeneticAlgorithm.h"
#include <QDebug>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    void updateResults(std::vector<std::vector<double>>& generations, std::vector<std::pair<double, double>>& locals, double& maxVal);
    void updateResults(std::vector<std::vector<double>>& generations, std::vector<double>& locals, double& maxVal);
    void handleClicked();
    void parametersClicked();
    void arrowHandle(size_t& total, size_t& cur);
    std::pair<GAParameters, PolynomData> inputHandle();
    void outHandle(GAParameters& gaParams, PolynomData& polynomData);

    void drawPolynomPlot(std::vector<double>& funcValues, std::vector<double>& funcArgs);
    void updatePlots(std::vector<GenerationSnapshot>& gaHistory, size_t& index);

signals:
    void startRequested();
    void loadRequested();
    void generateRequested();

    void nextRequested();
    void prevRequested();
    void toBeginRequested();
    void toEndRequested();

    void skipRequested();
    void saveRequested();

private:
    void initGUI();

    QChartView *plotPolynom;
    QChartView *plotQuality;

    QPushButton *btnRun;
    QPushButton *btnParameters;
    QPushButton *btnLoad;
    QPushButton *btnGenerate;
    QPushButton *btnNext;
    QPushButton *btnPrev;
    QPushButton *btnToStart;
    QPushButton *btnToEnd;
    QPushButton *btnSkip;
    QPushButton *btnSave;

    QLabel *lblStepCounter;
    QLabel *lblGlobalMaxVal;

    QTextEdit *txtCurrentPopulation;
    QTextEdit *txtLocalMaxVals;

    QLineEdit *linePolynom;
    QDoubleSpinBox *spinL;
    QDoubleSpinBox *spinR;
    QDoubleSpinBox *spinCrossProb;
    QDoubleSpinBox *spinMutationProb;
    QDoubleSpinBox *spinNicheRadius;
    QDoubleSpinBox *spinEps;
    QSpinBox *spinCntGenerations;
    QSpinBox *spinPopulationSize;
    QSpinBox *spinWindow;
    QSpinBox *spinShowGen;
    QComboBox *cmbCrossType;
    QComboBox *cmbMutationType;
    QComboBox *cmbSelectionType;
    QFrame *frameAdvanced;
};

#endif