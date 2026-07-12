#include "controller.h"

Controller::Controller() : gaAlg(GeneticAlgotithm()), gui(MainWindow()) {}

void Controller::run() {
    gui.handleClicked();
    std::pair<GAParameters, PolynomData> inputData = gui.inputHandle();

    if (gui.getGenerateClicked()){
        /* генерация через га */
        gui.outHandle(gaParams, polynomData); //Передача данных в гуи

    } else if (gui.getLoadClicked()){
        loadData();
        gui.outHandle(gaParams, polynomData);

    } else if (gui.getStartClicked()){
        std::string polynom = polynomData.polynom;
        bool isDataCorrect = checkInputData();

        if (isDataCorrect){
            std::vector<double> coeffs = polynomParser(polynom);
            std::vector<double> funcValues = calcFunc(coeffs, funcArgs);
            gui.drawPolynomPlot(funcValues, funcArgs);

            /* запуск га */

            size_t snapshotIndex;
            gui.updateResults(generations, locals, maxVal);
            gui.updatePlots(gaHistory, snapshotIndex);
        }
    } // ...
}


Controller::~Controller() {
    gui.setFlagsOff();
}
