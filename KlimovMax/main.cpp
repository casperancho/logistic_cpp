//
//  main.cpp
//  KlimovMax
//
//  Created by Артем Закиров on 23/09/2019.
//  Copyright © 2019 bmstu. All rights reserved.
//

#include <iostream>
using namespace std;

struct ErrorCell {  //ошибочные и записывает разницу между ними
    int i = -1;
    int j = -1;
    int difference = -1;  //разница между u+v и value
};

struct Shop {  // это все клетки в матрице 5х5
    int i = -1;
    int j = -1;
    int needs = 0;
    int value = -1;
};

struct Potential {  // вычитаемые потенциалы
    int i = -1;
    int j = -1;
    int value = -1;
    int potI = -1;
    int potJ = -1;
};

struct Perspective {  // перспективные клетки на замену
    int i = 0;
    int j = 0;
    int sign = -1;  //меняется -1 и 1
};

Perspective perspective[4]; // массив перспективных точек на замену
int persepctivCounter = 0;
int minPerspectiv = 0;

ErrorCell erCells[25];  //массив клеток, где i+j ошибка
int errorCellsCounter = 0;
ErrorCell maxErrorCell;

Shop places[25]; // все места, магазины
int placesCounter = 0;
int usedCell = 0;

const int m = 5; // кол-во столбцов
const int n = 5; //кол-во строк
int arr [m+1][n+1]; // массив исходных данных
int brr [m+1][n+1]; // массив повторяющий арр

int stocksSum = 0; // сумма запасов
int needsSum = 0; // сумма потребностей

int minElem = 0; // минимальный элемент
int imin = -1; // столбец с мин элементом
int jmin = -1; // строка с мин элементом

Potential potential[m+n-1];  //для запоминания точек и работы впоследствии с ними
int potentialCounter = 0;

void showArray(){  // показ массива
    for (int i=0; i<m+1; i++){
        for (int j=0; j<n+1;j++){
            if ( arr[i][j] == 0 ){ // если 0, то показывает Х , чтобы было как в примере
                cout << " x " ;
            } else {
            cout << arr[i][j] << " ";
            }
        }
        cout << endl;
    }
    cout << "-----------------" << endl;
}

void arrayFilling() {   // заполнение массива
    cout << "Сейчас необходимо будет ввести данные в таблицу 5х5" << endl;
    cout << "Введите первую строку" << endl;
    for (int i=0; i<m; i++) {  //  вводим построчно, изменяя только номер столбца, а номер строки оставляем
        cin >> arr[0][i]; // первая строка и разные столбцы
        brr[0][i] = arr[0][i];
    }
    
    minElem = arr[0][0]; // чтобы минимальный был не мнимый ноль, а первый элемент и можно было адекватно вычислять минимум
    
    cout << "Введите вторую строку" << endl;
    for (int i=0; i<m; i++){
        cin >> arr[1][i]; // вторая строка и разные столбцы
        brr[1][i] = arr[1][i];
    }
    cout << "Введите третью строку" << endl;
    for (int i=0; i<m; i++){
        cin >> arr[2][i]; // третья строка и разные столбцы
        brr[2][i] = arr[2][i];
    }
    cout << "Введите четвертую строку" << endl;
    for (int i=0; i<m; i++){
        cin >> arr[3][i]; // четвертая строка и разные столбцы
        brr[3][i] = arr[3][i];
    }
    cout << "Введите пятую строку" << endl;
    for (int i=0; i<m; i++){
        cin >> arr[4][i]; // пятая строка и разные столбцы
        brr[4][i] = arr[4][i];
    }
    cout << "Введите столбец запасов" << endl;
    for (int i=0; i<n; i++){  // здесь оставляем последний столбец(5 потому что в си++ начало отсчета является 0), меняем только строки
        cin >> arr[i][5]; // последний столбец и разные строки
        brr[i][5] = arr[i][5];
    }
    cout << "Введите строку потребностей" << endl;
    for (int i=0; i<m; i++){  // последняя строка и разные столбцы
        cin >> arr[5][i];
        brr[5][i] = arr[5][i];
    }
    cout << "------------------------------------" << endl;
    
}

void placesFillingAndSort() { //заполнение Shops
    int placesSize = 0;
    for (int i=0; i<m; i++){  //заполнение places
        for (int j=0; j<n; j++){
            places[placesSize].i = i;
            places[placesSize].j = j;
            places[placesSize].value = arr[i][j];
            placesSize++;
        }
    }

    for (int i=0; i<placesSize-1; i++){
        for (int j=i+1; j < placesSize;j++){  // сортировка по убыванию значений
            if (places[i].value > places[j].value) {
                    Shop temp = places[i];
                    places[i] = places[j];
                    places[j] = temp;
            }
        }
    }
    
    for (int i=0; i<placesSize-1; i++){
        for (int j=i+1; j < placesSize;j++){
            if (places[i].value == places[j].value) { // тупо сортировка по значениям
                if ((places[i].i > places[j].i)){ // сортировка по ранжиру
                    Shop temp = places[i];
                    places[i] = places[j];
                    places[j] = temp;
                } else if ((places[i].j > places[j].j) && (places[i].i == places[j].i)){ // тупо сортировка по ранжиру
                    Shop temp = places[i];
                    places[i] = places[j];
                    places[j] = temp;
                }
            }
        }
    }
    
}


void searchMin() {  // поиск минимума
    if (arr[places[placesCounter].i][places[placesCounter].j] == 0) {  // если сейчас ноль тогда смотрим след
        placesCounter++;
        searchMin();
    } else {
        imin = places[placesCounter].i;
        jmin = places[placesCounter].j;
        minElem = places[placesCounter].value;
        placesCounter++;
        if (imin == 0) {
            potential[potentialCounter].potI = 0;
        }
        potential[potentialCounter].i = imin;
        potential[potentialCounter].j = jmin;
        potential[potentialCounter].value= minElem;
        potentialCounter++;
    }
}

bool sumsCounting(){ // расчет суммы потребностей и запросов
    stocksSum = 0;
    needsSum = 0;
    for (int i = 0; i < m; i++){
        stocksSum += arr[5][i];  // stocksSum = stocksSum +  arr[5][i]
        needsSum += arr[i][5];
    }
    return (stocksSum == needsSum) && (needsSum == 0);
}

void changeElem() {  // меняем исходную на нули и ищем потенциалы
    int stocks = arr[imin][5];
    int needs = arr[5][jmin];

    if (stocks > needs) { // если запасы > потребностей
        places[placesCounter-1].needs = needs;
        stocks -= needs;
        needs -= needs;
        // возвращаем обратно
        arr[imin][5] = stocks;
        arr[5][jmin] = needs;
        for (int i = 0; i < m; i++) {
            if (arr[i][jmin] > minElem ){
                arr[i][jmin] = 0; // заменяем всю столбец на 0, можно потом на Х
            }
        }
        arr[imin][jmin] = minElem;
    } else {                          // если запасы < потребностей
        places[placesCounter-1].needs = stocks;
        needs -= stocks;
        stocks -= stocks;
        arr[imin][5] = stocks;
        arr[5][jmin] = needs;
        for (int i = 0; i < m; i++) {
            if (arr[imin][i] > minElem){
            arr[imin][i] = 0; // заменяем весь строка на 0, можно потом на Х
            }
        }
        arr[imin][jmin] = minElem;
    }
    usedCell++;
    showArray();
}

void potntialSort(){  // sorting потенциалов
    for (int i=0; i<m+n-1-1; i++){
        for (int j=i+1;j<m+n-1;j++){
            if (potential[i].i > potential[j].i){
                Potential temp = potential[i];
                potential[i] = potential[j];
                potential[j] = temp;
            } else if ((potential[i].i == potential[j].i) && (potential[i].j > potential[j].j)){
                Potential temp = potential[i];
                potential[i] = potential[j];
                potential[j] = temp;
            }
        }
    }
}
// поиск по j и занос туда данных

void changeIPotential(Potential pot){
    int i = pot.i;
    int iPot = pot.potI;
    for (int s=0; s<m+n-1;s++){
        if (i == potential[s].i) {
            potential[s].potI = iPot;
        }
    }
    //здесь функция меняющая потенциал
}

void changeJPotential(Potential pot){
    int j = pot.j;
    int jPot = pot.potJ;
    for (int s=0; s<m+n-1;s++){
        if (j == potential[s].j) {
            potential[s].potJ = jPot;
        }
    }

}

void referenceFunctionCounting() {  // поиск функции
    int referenceFunc = 0;
    for (int iteration=0;iteration<placesCounter;iteration++){
        if (places[iteration].needs != 0) {
            referenceFunc += places[iteration].needs * places[iteration].value;
        }
    }
    cout << referenceFunc << endl;
}

void potentialCounting(){ // расчет потенциала
    int counter = 0;
    int updated = 0;
    int i = 1;
    int j = 0;
    while (updated < m+n-1){
        if (potential[counter].potI == 0) {
            potential[counter].potJ = potential[counter].value;
            changeJPotential(potential[counter]);
            updated++;
            counter++;
        } else {
            for (;i<m;i++) {
                for (int j = 0; j<m; j++){
                    for (int c=0;c<potentialCounter;c++) {
                        if ((j == potential[c].j) && (potential[c].potJ != -1)) {
                            potential[c].potI = potential[c].value - potential[c].potJ;
                            changeIPotential(potential[c]);
                            updated++;
                        }
                    }
                }
                counter++;
            }
            for (;j<m;j++) {
                for (int i = 0; i<m; i++){
                    for (int c=0; c<potentialCounter; c++) {
                        if ((i == potential[c].i) && (potential[c].potI != -1)) {
                            potential[c].potJ = potential[c].value - potential[c].potI;
                            changeIPotential(potential[c]);
                            updated++;
                        }
                    }
                }
                counter++;
            }
            for (int counter = 0; counter < m+n-1; counter++) {
                if (potential[counter].potJ == -1) {
                    for (int inner = 0; inner<m+n-1; inner++){
                        if ((potential[inner].j == potential[counter].j) && (potential[inner].potJ != -1)) {
                            potential[counter].potJ = potential[inner].potJ;
                            potential[counter].potI = potential[counter].value - potential[counter].potJ;
                        }
                    }
                }
            }
        }
    }
    for (int k = 0; k < potentialCounter; k++) {  //если остались не обновленные
        if (potential[k].potI == -1){
            for (int a=0; a< potentialCounter;a++){
                if (potential[k].i == potential[a].i){
                    potential[k].potI = potential[a].potI;
                } else {
                    potential[k].potI = potential[k].value - potential[k].potJ;
                }
            }
        }
        if (potential[k].potJ == -1){
            for (int a=0; a< potentialCounter;a++){
                if (potential[k].j == potential[a].j){
                    potential[k].potJ = potential[a].potJ;
                } else {
                    potential[k].potJ = potential[k].value - potential[k].potI;
                }
            }
        }
    }
}

void cellsChecking(){  // проверка ячеек на ошибки i+j
    int PotI = 0;
    int PotJ = 0;
    for (int i=0; i<m ;i++) {
        for (int j=0;j<n;j++){
            for (int counter=0; counter<m+n-1; counter++){
                if (potential[counter].i == i) {
                    PotI = potential[counter].potI;
                }
                if (potential[counter].j == j) {
                    PotJ = potential[counter].potJ;
                }
            }
            if ((PotI + PotJ) > brr[i][j]) {
                erCells[errorCellsCounter].difference = PotI + PotJ - brr[i][j];
                erCells[errorCellsCounter].i = i;
                erCells[errorCellsCounter].j = j;
                errorCellsCounter++;
            }
        }
    }
    int max = erCells[0].difference;
    maxErrorCell = erCells[0];
    for (int i=0; i< errorCellsCounter; i++){
        if (erCells[errorCellsCounter].difference > max){
            max = erCells[errorCellsCounter].difference;
            maxErrorCell = erCells[errorCellsCounter];
        }
    }
}

int searchJColumn(int startI, int startJ){  // поиск стобца
    int oldStart = startJ;
    if (m-1-startJ >= startJ) { //влево или вправо
        startJ++;
        if (arr[startI][startJ] != 0) {
            int nonZero = 0;
            for (int i=0;i<m;i++) {
                if (arr[i][startJ] != 0) {
                    nonZero++;
                }
            }
            if (nonZero != 0) {
                return startJ;
            }
        }
    }
    return oldStart;
}

int searchIline(int startI, int startJ) { // поиск строки
//    int oldStart = startI;
    if (m-1-startI >= startI ) {
        
    } else {
//        startI--;
        if (arr[startI][startJ] != 0) {
            int nonZero = 0;
            for (int i = 0; i< startI;i++){
                if (arr[i][startJ] != 0) {
                    nonZero++;
                }
            }
            if (nonZero != 0) {
                return startI;
            }
        }
    }
    return 0;
}

int searchNext(int startI, int startJ) {  // ищем дальше
    int oldI = startI;
    int oldJ = startJ;
    
    for (int i = 0; i<oldI; i++){
        if ((arr[i][maxErrorCell.j] != 0) && (arr[i][oldJ] != 0)){
            
            perspective[persepctivCounter].i = i;
            perspective[persepctivCounter].j = oldJ;
            perspective[persepctivCounter].sign = 1;
            persepctivCounter++;
            
            perspective[persepctivCounter].i = i;
            perspective[persepctivCounter].j = maxErrorCell.j;
            perspective[persepctivCounter].sign = -1;
            persepctivCounter++;
            return i;
        }
    }
    return oldI;
}

//search where until j have non nill
void polygonMaking(){ // поиск квадрата
    int startI = maxErrorCell.i;
    int startJ = maxErrorCell.j;
    int angle = 1;
    
    perspective[persepctivCounter].i = startI;
    perspective[persepctivCounter].j = startJ;
    perspective[persepctivCounter].sign = 1;
    persepctivCounter++;
    
    while (angle != 4) {
        if (startJ == searchJColumn(startI, startJ)) {
            startJ++;
        } else {
            startJ = searchJColumn(startI, startJ);
            if (searchIline(startI, startJ) == 0) {

            } else {
                perspective[persepctivCounter].i = startI;
                perspective[persepctivCounter].j = startJ;
                perspective[persepctivCounter].sign = -1;
                persepctivCounter++;
                
                angle++;
                if (searchNext(startI, startJ) != 0) {
                    angle += 2;
                }
            }
        }
    }
}

void searchMinPerspectiv(){  // ищем минимальную среди перспективных
    for (int i = 0; i< persepctivCounter;i++){
        if (perspective[i].sign == -1) {
            for (int j = 0; j< placesCounter; j++) {
                if ((places[j].i == perspective[i].i) && (places[j].j == perspective[i].j)){
                    if (minPerspectiv > places[j].needs){
                        minPerspectiv = places[j].needs;
                    }
                }
            }
        }
    }
}

void changePotentialCell(int i, int j){  // изменение ячейки потенциала
    for (int pot=0; pot< potentialCounter; pot++){
        if ((potential[pot].i == i) && (potential[pot].j == j)) {
            potential[pot].i = maxErrorCell.i;
            potential[pot].j = maxErrorCell.j;
            potential[pot].potI = -1;
            potential[pot].potJ = -1;
            potential[pot].value = brr[maxErrorCell.i][maxErrorCell.j];
        }
    }
}

void changePerspectiveNeeds(){  // изменение потребности в перспективной 
    for (int i = 0; i< persepctivCounter;i++){
        if (perspective[i].sign == -1) {
            for (int j = 0; j< placesCounter; j++) {
                if ((places[j].i == perspective[i].i) && (places[j].j == perspective[i].j)){
                    places[j].needs -= minPerspectiv;
                    if (places[j].needs == 0){
                        changePotentialCell(perspective[i].i,perspective[i].j); // меняем список потенциалов
                    }
                }
            }
        } else {
            for (int j = 0; j< placesCounter; j++) {
                if ((places[j].i == perspective[i].i) && (places[j].j == perspective[i].j)){
                    places[j].needs += minPerspectiv;
                }
            }
        }
    }
}

int searchInPlaces(int i,int j){
    for (int k =0; k< placesCounter;k++) {
        if ((places[k].i == i) && ((places[k].j == j))){
            return places[k].needs;
        }
    }
    return 0;
}

void finalShow() {  //search in places
    cout << "Финальная матрица" << endl;
    int potCounter = 0;
    for (int i=0; i<m+1; i++){
        for (int j=0; j<n+1;j++){
            int a = potential[potCounter].i;
            int b = potential[potCounter].j;
            if ((a == i) && (b == j)){
                cout << brr[i][j] << "[" << searchInPlaces(a,b) << "] ";
                potCounter++;
            } else {
                cout << brr[i][j] << "  ";
            }
        }
        cout << endl;
    }
    cout << "Распределение грузов" << endl;
    for (int i = 0; i < potentialCounter; i++){
        int a = potential[i].i;
        int b = potential[i].j;
        if (i == 0) {
            cout << "Из " << a+1 << "-го склада необходимо груз направить " ;
            cout << " в " << b+1 << "-й магазин (" << searchInPlaces(a,b)<< " ед.)" << endl;
        } else if (potential[i-1].i == a) {
            cout << ", в " << b+1<< "-й магазин (" << searchInPlaces(a,b) << " ед.)" << endl;
        } else {
            cout << "Из " << a+1 << "-го склада необходимо груз направить в "
                                    << b+1 << "-й магазин (" << searchInPlaces(a,b)<< " ед.)" << endl;
        }
    }
}

int main() {
    arrayFilling();  // заполенение массива
    minPerspectiv = arr[0][n];  //для вычета перспективы
    sumsCounting(); //необходимая правильная сумма
    placesFillingAndSort(); //заполнение place
    
    cout << "Проверим необходимое и достаточное условие разрешимости задачи." << endl;
    cout << "Запасы ∑a = " << stocksSum << endl;
    cout << "Потребности ∑b = " << needsSum << endl;
    if (stocksSum != needsSum ){
        cout << "Ошибка. Условие баланса не соблюдается. Введите данные снова" << endl;
        arrayFilling();
    } else {
        cout << "Условие баланса соблюдается. Запасы равны потребностям. Следовательно, модель транспортной задачи является закрытой." << endl;
    }

    cout << "Ввод завершен. Приступаем к расчетам" << endl;
    
    while (!sumsCounting()) {  // && - and, || - or , ! - not
        searchMin(); // ищем подходящий минимум для работы и делаем загрузку
        changeElem();
    }
    cout << "В результате получен опопрный план, который является допустимым, так как все грузы из баз вывезены, потребность магазинов удовлетворена, а план соответствует системе ограничений транспортной задачи." << endl;
    cout << "Количество занятых клеток таблицы должно быть " << m << " + " << n << " - 1 = " << m+n-1 << endl;
    cout << "Количетсво занятых клеток = " << usedCell << endl;
    if ((m+n-1) == usedCell) {
        cout << "Следовательно, опорный план является невырожденным" << endl;
    }
    
    cout << "Значение целевой функции для этого опорного плана равно: "; // referenceFunctionCounting() x
    referenceFunctionCounting();
    
    //start second step
    potntialSort();
    potentialCounting();
    
    cellsChecking();
    while (maxErrorCell.difference > 0) {
        polygonMaking();
        //находим минимум у - потенциалов и меняем
        searchMinPerspectiv();
        changePerspectiveNeeds();
        
        
        for (int i = 0; i< potentialCounter;i++){
            if (i<2){
                potential[i].potJ = -1;
            } else {
                potential[i].potI = -1;
                potential[i].potJ = -1;
            }
        }
        
        for (int i = 0; i< errorCellsCounter; i++) {
            erCells[i].difference = -1;
            erCells[i].i = -1;
            erCells[i].j = -1;
        }
        
        //start second
        potntialSort();
        potentialCounting();
        
        cellsChecking();
    }
    cout << "Опорный план является оптимальным, так все оценки свободных клеток удовлетворяют условию" << endl;
    cout << "Минимальные затраты составят: ";
    referenceFunctionCounting();
    cout << "Анализ оптимального плана." << endl;
    
    finalShow();
    return 0;
}

//30    40    25    15    20
//10    25    15    20    30
//15    20    15    35    10
//30    40    20    25    15
//25    30    35    20    25
//105 60 90 75 70
//85    65    95    100    55


//  25 20 25 15 20
//0
//-15
// - 10
// -5
// 10




//ручка = 5 шту
//карандаши = 7 штук

//объединение  пенал{
//    ручки
//    карандаши
//}

