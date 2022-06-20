#include <math.h>
#define PI 3.1415926535897932384626433832795

template <class T>
const T & min(const T & a, const T & b)
{
    return (a < b) ? a : b;
}

template <class T>
const T & max(const T & a, const T & b)
{
    return (a > b) ? a : b;
}

// Parent class for all electronics
class ElectricalBase
{
protected:
    ElectricalBase();

    virtual ~ElectricalBase();

public:
    virtual void setInputVoltage(const double u, const bool isCurrentPersist);
    virtual void setInputCurrent(const double i);
    virtual double getOutputVoltage();
    virtual double getOutputCurrent();

private:
    double InVolt = 0.0;
    double InCurr = 0.0;

    double OutVolt = 0.0;
    double OutCurr = 0.0;
};

ElectricalBase::ElectricalBase()
{
}

void ElectricalBase::setInputVoltage(const double u, const bool isCurrentPersist = true)
{
    this->InVolt = u;
    if (!isCurrentPersist) this->InCurr = INT_MAX;
}

void ElectricalBase::setInputCurrent(const double i)
{
    this->InCurr = i;
}

double ElectricalBase::getOutputVoltage()
{
    return this->OutVolt;
}

double ElectricalBase::getOutputCurrent()
{
    return this->OutCurr;
}

ElectricalBase::~ElectricalBase() {}

// DC source
class DCSource : public ElectricalBase
{
public:
    DCSource(const double u, const bool isCurrentPersist, const double i);

    void setOutputVoltage(const double u);
    void setOutputCurrent(const double i);

    double getOutputVoltage();
    double getOutputCurrent();

    virtual ~DCSource();

private:
    double OutVolt = 0.0;
    double OutCurr = 0.0;
};

DCSource::DCSource(const double u = 0.0, const bool isCurrentPersist = false, const double i = 0.0)
{
    this->OutVolt = u;
    this->OutCurr = (isCurrentPersist) ? i : INT_MAX;
}

void DCSource::setOutputVoltage(const double u)
{
    this->OutVolt = u;
}

void DCSource::setOutputCurrent(const double i)
{
    this->OutCurr = i;
}

double DCSource::getOutputVoltage()
{
    return this->OutVolt;
}

double DCSource::getOutputCurrent()
{
    return this->OutCurr;
}

DCSource::~DCSource() {}

// Simple converter (Inverter) DC->AC [src->INV->eng]
template <class T, class U>
class SimpInverter : public ElectricalBase
{
public:
    SimpInverter(const T * in, const U * out);
    //SimpInverter();

    T * inDevice;
    U * outDevice;

    void setInputVoltage(const double u, const bool isCurrentPersist = true);
    void setInputCurrent(const double i);
    double getOutputVoltage();
    double getOutputCurrent();
    double getOutputFrequency();
    void setProgrammVoltage(const double u);
    void setProgrammCurrent(const double i);
    void setProgrammFrequency(const double f);

    int runSimulation();

    virtual ~SimpInverter();

private:
    // Input values
    double InVolt = 0.0;
    double InCurr = 0.0;

    // Real values
    double Volt = 0.0;
    double Curr = 0.0;
    double Freq = 0.0;

    // Programmed values
    double ProgVolt = 0.0;
    double ProgCurr = 0.0;
    double ProgFreq = 0.0;

    // Output values
    double OutVolt = 0.0;
    double OutCurr = 0.0;
    double OutFreq = 0.0;

    // Sensor values
    double EngRotRate = 0.0;
};

template <class T, class U>
SimpInverter<T,U>::SimpInverter(const T * in, const U * out)
{
    this->inDevice = in;
    this->outDevice = out;
}

/*SimpInverter::SimpInverter()
{
}*/

template <class T, class U>
void SimpInverter<T,U>::setInputVoltage(const double u, const bool isCurrentPersist)
{
    this->InVolt = u;
    if (!isCurrentPersist) this->InCurr = INT_MAX;
}

template <class T, class U>
void SimpInverter<T,U>::setInputCurrent(const double i)
{
    this->InCurr = i;
}

template <class T, class U>
double SimpInverter<T,U>::getOutputVoltage()
{
    return this->OutVolt;
}

template <class T, class U>
double SimpInverter<T,U>::getOutputCurrent()
{
    return this->OutCurr;
}

template <class T, class U>
double SimpInverter<T,U>::getOutputFrequency()
{
    return this->OutFreq;
}

template <class T, class U>
void SimpInverter<T,U>::setProgrammVoltage(const double u)
{
    this->ProgVolt = u;
}

template <class T, class U>
void SimpInverter<T,U>::setProgrammCurrent(const double i)
{
    this->ProgCurr = i;
}

template <class T, class U>
void SimpInverter<T,U>::setProgrammFrequency(const double f)
{
    this->ProgFreq = f;
}

template <class T, class U>
int SimpInverter<T,U>::runSimulation()
{
    this->OutVolt = min(this->ProgVolt, this->InVolt/sqrt(2));
    this->OutCurr = min(this->ProgCurr, this->InCurr/sqrt(2));
    this->OutFreq = this->ProgFreq;

    return 0;
}

template <class T, class U>
SimpInverter<T,U>::~SimpInverter() {}

// Simplified model of AC induction motor [inv->ENG->mech]
class SimpAsyncEngine : public ElectricalBase
{
public:
    SimpAsyncEngine(const unsigned int poles, const double statActResist, const double rotActResist, const bool enableReactConst, const double statReactResist, const double rotReactResist, const double magnetReactResist);

    void setInputVoltage(const double u, const bool isCurrentPersist);
    void setInputCurrent(const double i);
    void setInputFrequency(const double f);
    void setStatorReactiveResistance(const double x);
    void setRotorReactiveResistance(const double x);    // Note: only works, when IsReactConst is true
    void setMagneticReactiveResistance(const double x);

    // true: reactive resistance is dynamic and is calculated by formula
    // false: reactive resistance is static and being constant
    void switchReactiveConst(const bool enable);

    double getOutputMoment();
    double getOutputRotationRate();

    int runSimulation();

    virtual ~SimpAsyncEngine();

private:
    // Input values
    double InVolt = 0.0;
    double InCurr = 0.0;
    double InFreq = 0.0;

    // Stator values
    double StatVolt = 0.0;
    double StatCurr = 0.0;
    double StatFreq = 0.0;

    // Rotor values
    double RotVolt = 0.0;
    double RotCurr = 0.0;
    double RotFreq = 0.0;

    // Motor characteristics (based on transformer as equivalent circuit)
    unsigned int Poles = 0;
    double StatActResist = 0.0;
    double StatReactResist = 0.0;
    double StatInduct = 0.07; // By default: 0.07 (temporarily)
    double RotActResist = 0.0;
    double RotReactResist = 0.0;
    double RotInduct = 2; // By default: 2 (temporarily)
    double MagnetReactResist = 0.0;
    bool IsReactConst = true;

    // Misc values
    double Slope = 0.0;

    // Output values
    double OutMoment = 0.0;
    double OutRotRate = 0.0;
};

SimpAsyncEngine::SimpAsyncEngine(const unsigned int poles, const double statActResist, const double rotActResist, const bool enableReactConst = true, const double statReactResist = 0.0, const double rotReactResist = 0.0, const double magnetReactResist = 0.0)
{
    this->Poles = poles;
    this->StatActResist = statActResist;
    this->RotActResist = rotActResist;

    if (enableReactConst)
    {
        this->StatReactResist = statReactResist;
        this->RotReactResist = rotReactResist;
        this->MagnetReactResist = magnetReactResist;
    }
}

void SimpAsyncEngine::setInputVoltage(const double u, const bool isCurrentPersist = true)
{
    this->InVolt = u;
    if (!isCurrentPersist) this->InCurr = INT_MAX;
}

void SimpAsyncEngine::setInputCurrent(const double i)
{
    this->InCurr = i;
}

void SimpAsyncEngine::setInputFrequency(const double f)
{
    this->InFreq = f;
}

void SimpAsyncEngine::setStatorReactiveResistance(const double x)
{
    this->StatReactResist = x;
}

void SimpAsyncEngine::setRotorReactiveResistance(const double x)
{
    this->RotReactResist = x;
}

void SimpAsyncEngine::switchReactiveConst(const bool enable)
{
    this->IsReactConst = enable;
}

double SimpAsyncEngine::getOutputMoment()
{
    return this->OutMoment;
}

double SimpAsyncEngine::getOutputRotationRate()
{
    return this->OutRotRate;
}

int SimpAsyncEngine::runSimulation()
{
    this->StatVolt = this->InVolt;
    this->StatCurr = this->InCurr;
    this->StatFreq = this->InFreq;
    if (!IsReactConst)
    {
        this->StatReactResist = 2 * PI * this->StatFreq * this->StatInduct;
        this->RotReactResist = 2 * PI * this->RotFreq * this->RotInduct;
    }



    return 0;
}

SimpAsyncEngine::~SimpAsyncEngine() {}
