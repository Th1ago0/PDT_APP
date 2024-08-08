/************************************************************************************************************
* PDT.cpp                                                                                                   *
* AUTOR: Thiago Silva                                                                                       *
* CRIACAO:            24 JULHO 2024                                                                         *
* ULTIMA MODIFICACAO: 08 AGOSTO 2024                                                                        *
* COMPILADOR:         Microsoft Visual C++ 2022                                                             *
* DESCRICAO: Implementacao da classe PDT, esta classe foi criada para fazer uma analise CC de               *
* um circuito de polarizacao por divisor de tensao.                                                         *
*************************************************************************************************************/

#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include "PDT.h"
using namespace std;


// POLARIZACAO POR DIVISOR DE TENSAO
PDT::PDT(double r1, double r2, double rc, double re, double vcc, double beta)
    : R1(r1), R2(r2), RC(rc), RE(re), VCC(vcc), BETA(beta)
{
    VBE = 0.7;
    MethodDefiner();
}

// Comeca a Analise
void PDT::Start()
{
    // Chama o metodo
    if (method == "exact")
    {
        ExactMethod();
    }
    else
    {
        SimplifiedMethod();
    }
}

// Define qual metodo usar com base em R1 || R2 <= 10% * BETA * RE
void PDT::MethodDefiner()
{
    RTH = (R1 * R2) / (R1 + R2);
    if (RTH <= (0.1 * BETA * RE))
    {
        method = "simplified";
    }
    else
    {
        method = "exact";
    }
}

void PDT::ExactMethod()
{
    // Tensao na Base
    VBB = (R2 / (R1 + R2)) * VCC;
    // Corrente na Base
    IB = (VBB - VBE) / (RTH + RE * (BETA + 1));
    // Corrente no Coletor
    IC = BETA * IB;
    // Tensao no Coletor
    VC = VCC - (RC * IC);
    // Corrente no Emissor
    IE = IC;
    // Tensao no Emissor
    VE = RE * IE;
    // Tensao Coletor Emissor
    VCE = VC - VE;

}

// Metodo Simplificado
void PDT::SimplifiedMethod()
{
    // Tensao da base
    VBB = (R2 / (R1 + R2)) * VCC;
    // Tensao no Emissor
    VE = VBB - VBE;
    // Corrente no Emissor
    IE = VE / RE;
    // Corrente no Coletor
    IC = IE;
    // Corrente na Base
    IB = IC / BETA;
    // Tensao no Coletor
    VC = VCC - (RC * IC);
    // Tensao Coletor - Emissor
    VCE = VC - VE;
}

// Regulariza os dados e retorna uma string com o multiplo embutido
wstring PDT::Regulate(double n)
{
    wstringstream text;
    text << setprecision(2) << fixed;

    // Mega
    if (n >= 1000000)
    {
        text << n / 1000000 << L" M";
    }
    // Kilo
    else if (n >= 1000)
    {
        text << n / 1000 << L" k";
    }
    // Normal
    else if (n >= 1)
    {
        text << n << L" ";
    }
    // Mili
    else if (n >= 0.001)
    {
        text << n / 0.001 << L" m";
    }
    // Micro
    else if (n >= 0.000001)
    {
        text << n / 0.000001 << L" u";
    }
    // Nano
    else if (n >= 0.000000001)
    {
        text << n / 0.000000001 << L" n";
    }

    // Valor com seu multiplo
    return text.str();
}

/*       GETTERS       */

// Resistencias
wstring PDT::getR1()
{
    return Regulate(R1);
}

wstring PDT::getR2()
{
    return Regulate(R2);
}

wstring PDT::getRC()
{
    return Regulate(RC);
}

wstring PDT::getRE()
{
    return Regulate(RE);
}

wstring PDT::getRTH()
{
    return Regulate(RTH);
}


// Tensoes
wstring PDT::getVCC()
{
    return Regulate(VCC);
}

wstring PDT::getVCE()
{
    return Regulate(VCE);
}

wstring PDT::getVBB()
{
    return Regulate(VBB);
}

wstring PDT::getVBE()
{
    return Regulate(VBE);
}

wstring PDT::getVC()
{
    return Regulate(VC);
}

wstring PDT::getVE()
{
    return Regulate(VE);
}

// Correntes
wstring PDT::getIE()
{
    return Regulate(IE);
}

wstring PDT::getIC()
{
    return Regulate(IC);
}

wstring PDT::getIB()
{
    return Regulate(IB);
}

wstring PDT::getMethod()
{
    if (method == "exact")
    {
        return L"Metodo Exato";
    }
    else
    {
        return L"Metodo Simplificado";
    }
}

wstring PDT::getBETA()
{
    return Regulate(BETA);
}