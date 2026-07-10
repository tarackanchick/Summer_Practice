#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Types.h"
#include "ui_mainwindow.h"
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

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    void initGUI();
    void updateResults(std::vector<std::vector<double>> generations, std::vector<double> locals, double maxVal);
    void handleClicked();
    void parametersClicked();
    void arrowHandle(size_t total, size_t cur);
    std::pair<GAParameters, PolynomData> inputHandle();
    void outHandle(GAParameters gaParams, PolynomData polynomData);

private:
    Ui::MainWindow *ui;

    QLabel *plotPlaceholder1;
    QLabel *plotPlaceholder2;

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

    bool startClicked = false;
    bool generateClicked = false;
    bool loadClicked = false;
    bool saveClicked = false;
    bool skipClicked = false;
    bool toBeginClicked = false;
    bool toEndClicked = false;
    bool nextClicked = false;
    bool prevClicked = false;
};

#endif
