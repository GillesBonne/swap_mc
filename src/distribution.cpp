#include <stdexcept>

#include "distribution.h"

Distribution::Distribution(const int numSpecies)
{
    // Values specific for the number of species and ratio=2.219.
    // Can be calculated using discrete_polydisperse.nb.
    if(numSpecies==2)
    {
        sigmaMax =  2.01323;
        sigmaMin =  0.907267;
        a =         0.684184;
        b =         sigmaMin;
        c =         1.10596;
    }
    else if(numSpecies==3)
    {
        sigmaMax =  1.85908;
        sigmaMin =  0.837803;
        a =         0.441701;
        b =         sigmaMin;
        c =         0.510641;
    }
    else if(numSpecies==4)
    {
        sigmaMax =  1.78458;
        sigmaMin =  0.804227;
        a =         0.321294;
        b =         sigmaMin;
        c =         0.326784;
    }
    else if(numSpecies==5)
    {
        sigmaMax =  1.74313;
        sigmaMin =  0.785546;
        a =         0.251929;
        b =         sigmaMin;
        c =         0.239395;
    }
    else if(numSpecies==6)
    {
        sigmaMax =  1.71718;
        sigmaMin =  0.773853;
        a =         0.207123;
        b =         sigmaMin;
        c =         0.188665;
    }
    else if(numSpecies==7)
    {
        sigmaMax =  1.69954;
        sigmaMin =  0.765902;
        a =         0.175843;
        b =         sigmaMin;
        c =         0.155606;
    }
    else if(numSpecies==8)
    {
        sigmaMax =  1.6868;
        sigmaMin =  0.760162;
        a =         0.152776;
        b =         sigmaMin;
        c =         0.132377;
    }
    else if(numSpecies==9)
    {
        sigmaMax =  1.67719;
        sigmaMin =  0.755832;
        a =         0.135064;
        b =         sigmaMin;
        c =         0.11517;
    }
    else if(numSpecies==10)
    {
        sigmaMax =  1.66969;
        sigmaMin =  0.752453;
        a =         0.121035;
        b =         sigmaMin;
        c =         0.101916;
    }
    else if(numSpecies==11)
    {
        sigmaMax =  1.66368;
        sigmaMin =  0.749743;
        a =         0.109649;
        b =         sigmaMin;
        c =         0.0913937;
    }
    else if(numSpecies==12)
    {
        sigmaMax =  1.65875;
        sigmaMin =  0.747524;
        a =         0.100223;
        b =         sigmaMin;
        c =         0.0828392;
    }
    else if(numSpecies==13)
    {
        sigmaMax =  1.65465;
        sigmaMin =  0.745672;
        a =         0.0922907;
        b =         sigmaMin;
        c =         0.0757479;
    }
    else if(numSpecies==14)
    {
        sigmaMax =  1.65117;
        sigmaMin =  0.744105;
        a =         0.0855227;
        b =         sigmaMin;
        c =         0.0697742;
    }
    else if(numSpecies==15)
    {
        sigmaMax =  1.64819;
        sigmaMin =  0.742761;
        a =         0.0796802;
        b =         sigmaMin;
        c =         0.0646733;
    }
    else if(numSpecies==16)
    {
        sigmaMax =  1.6456;
        sigmaMin =  0.741597;
        a =         0.0745854;
        b =         sigmaMin;
        c =         0.0602671;
    }
    else if(numSpecies==17)
    {
        sigmaMax =  1.64334;
        sigmaMin =  0.740577;
        a =         0.0701033;
        b =         sigmaMin;
        c =         0.0564227;
    }
    else if(numSpecies==18)
    {
        sigmaMax =  1.64135;
        sigmaMin =  0.739678;
        a =         0.0661297;
        b =         sigmaMin;
        c =         0.0530393;
    }
    else if(numSpecies==19)
    {
        sigmaMax =  1.63957;
        sigmaMin =  0.738879;
        a =         0.0625825;
        b =         sigmaMin;
        c =         0.0500385;
    }
    else if(numSpecies==20)
    {
        sigmaMax =  1.63799;
        sigmaMin =  0.738164;
        a =         0.0593967;
        b =         sigmaMin;
        c =         0.047359;
    }
    else if(numSpecies==30)
    {
        sigmaMax =  1.62814;
        sigmaMin =  0.733728;
        a =         0.0393626;
        b =         sigmaMin;
        c =         0.0308419;
    }
    else if(numSpecies==40)
    {
        sigmaMax =  1.62335;
        sigmaMin =  0.731569;
        a =         0.0294358;
        b =         sigmaMin;
        c =         0.0228662;
    }
    else if(numSpecies==50)
    {
        sigmaMax =  1.62052;
        sigmaMin =  0.730293;
        a =         0.0235078;
        b =         sigmaMin;
        c =         0.0181679;
    }
    else if(numSpecies==60)
    {
        sigmaMax =  1.61865;
        sigmaMin =  0.72945;
        a =         0.0195673;
        b =         sigmaMin;
        c =         0.0150712;
    }
    else if(numSpecies==70)
    {
        sigmaMax =  1.61732;
        sigmaMin =  0.728852;
        a =         0.0167582;
        b =         sigmaMin;
        c =         0.0128764;
    }
    else if(numSpecies==80)
    {
        sigmaMax =  1.61633;
        sigmaMin =  0.728405;
        a =         0.0146545;
        b =         sigmaMin;
        c =         0.0112396;
    }
    else if(numSpecies==90)
    {
        sigmaMax =  1.61556;
        sigmaMin =  0.728059;
        a =         0.0130201;
        b =         sigmaMin;
        c =         0.00997195;
    }
    else if(numSpecies==100)
    {
        sigmaMax =  1.61495;
        sigmaMin =  0.727782;
        a =         0.0117136;
        b =         sigmaMin;
        c =         0.00896128;
    }
    else if(numSpecies==1000)
    {
        sigmaMax =  1.61004;
        sigmaMin =  0.72557;
        a =         0.0011678;
        b =         sigmaMin;
        c =         0.000885355;
    }
    else if(numSpecies==10000)
    {
        sigmaMax =  1.60955;
        sigmaMin =  0.725351;
        a =         0.000116745;
        b =         sigmaMin;
        c =         0.0000884291;
    }
    else if(numSpecies==100000)
    {
        sigmaMax =  1.60951;
        sigmaMin =  0.725329;
        a =         0.0000116742;
        b =         sigmaMin;
        c =         0.00000884185;
    }
    else
    {
        throw std::out_of_range("Parameters of the set number of species are not defined.");
    }
}

double Distribution::GetSigmaMax() const
{
    return sigmaMax;
}
double Distribution::GetSigmaMin() const
{
    return sigmaMin;
}
double Distribution::GetConstA() const
{
    return a;
}
double Distribution::GetConstB() const
{
    return b;
}
double Distribution::GetConstC() const
{
    return c;
}
