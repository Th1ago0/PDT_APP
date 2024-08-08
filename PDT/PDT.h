/************************************************************************************************************
* PDT.h                                                                                                     *
* AUTOR: Thiago Silva                                                                                       *
* CRIACAO:            24 JULHO 2024                                                                         *
* ULTIMA MODIFICACAO: 08 AGOSTO 2024                                                                        *
* COMPILADOR:         Microsoft Visual C++ 2022                                                             *
* DESCRICAO: Definicao da classe PDT que representa os valores de um circuito amplificador com transistor.  *
*************************************************************************************************************/

#include <string>
using namespace std;

// POLARIZACAO POR DIVISOR DE TENSAO
class PDT
{
private:
    string method;
    double R1, R2, RC, RE, VCC, BETA;
    double VBB, VBE, VCE, VC, VE, IC, IB, IE, RTH;

    void ExactMethod();
    void SimplifiedMethod();
    void MethodDefiner();
    wstring Regulate(double n);

public:
    PDT(double r1, double r2, double rc, double re, double vcc, double beta);
    void Start();
    
    // Getters
    wstring getR1();
    wstring getR2();
    wstring getRC();
    wstring getRE();
    wstring getRTH();

    wstring getVCC();
    wstring getVCE();
    wstring getVBB();
    wstring getVBE();
    wstring getVC();
    wstring getVE();

    wstring getIE();
    wstring getIC();
    wstring getIB();

    wstring getBETA();
    wstring getMethod();
};