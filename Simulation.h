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

template <class T>
const T & comp(const T & mn, const T & mx, const T & a)
{
    return min(mx, max(mn, a));
}

template <class T>
const T & sqr(const T & n)
{
    return n * n;
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

    virtual int runSimulation(const double dt);

private:
    double InVolt = 0.0;
    double InCurr = 0.0;

    double OutVolt = 0.0;
    double OutCurr = 0.0;
};

ElectricalBase::ElectricalBase() {}

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

int ElectricalBase::runSimulation(const double dt)
{
    return 0;
}

ElectricalBase::~ElectricalBase() {}


// DC source
template <class T, class U>
class DCSource : public ElectricalBase
{
public:
    DCSource(const T * in, const U * out, const double u, const bool isCurrentPersist = false, const double i = 0.0);

    T * InDevice;
    U * OutDevice;

    void setOutputVoltage(const double u);
    void setOutputCurrent(const double i);

    double getOutputVoltage();
    double getOutputCurrent();

    int runSimulation(const double dt);

    virtual ~DCSource();

private:
    double OutVolt = 0.0; // V
    double OutCurr = 0.0; // A
};

template <class T, class U>
DCSource<T,U>::DCSource(const T * in, const U * out, const double u, const bool isCurrentPersist, const double i)
{
    this->InDevice = in;
    this->OutDevice = out;
    this->OutVolt = u;
    this->OutCurr = (isCurrentPersist) ? i : INT_MAX;
}

template <class T, class U>
void DCSource<T,U>::setOutputVoltage(const double u)
{
    this->OutVolt = u;
}

template <class T, class U>
void DCSource<T,U>::setOutputCurrent(const double i)
{
    this->OutCurr = i;
}

template <class T, class U>
double DCSource<T,U>::getOutputVoltage()
{
    return this->OutVolt;
}

template <class T, class U>
double DCSource<T,U>::getOutputCurrent()
{
    return this->OutCurr;
}

template <class T, class U>
int DCSource<T,U>::runSimulation(const double dt)
{
    this->OutDevice->setInputVoltage(this->OutVolt, (this->OutCurr == INT_MAX) ? false : true);

    return 0;
}

template <class T, class U>
DCSource<T,U>::~DCSource()
{
    this->InDevice = NULL;
    this->OutDevice = NULL;
}


// Simple converter (Inverter) DC->AC [src->INV->eng]
template <class T, class U>
class SimpInverter : public ElectricalBase
{
public:
    SimpInverter(const T * in, const U * out);
    //SimpInverter();

    T * InDevice;
    U * OutDevice;

    void setInputVoltage(const double u, const bool isCurrentPersist = true);
    void setInputCurrent(const double i);

    double getOutputVoltage();
    double getOutputCurrent();
    double getOutputFrequency();

    void setProgrammVoltage(const double u);
    void setProgrammCurrent(const double i);
    void setProgrammFrequency(const double f);

    int runSimulation(const double dt);

    virtual ~SimpInverter();

private:
    // Input values
    double InVolt = 0.0;
    double InCurr = 0.0;

    // Real values
    double Volt = 0.0; // V
    double Curr = 0.0; // A
    double Freq = 0.0; // hz

    // Programmed values
    double ProgVolt = 0.0;
    double ProgCurr = 0.0;
    double ProgFreq = 0.0;

    // Output values
    double OutVolt = 0.0;
    double OutCurr = 0.0;
    double OutFreq = 0.0;

    // Sensor values
    double EngRotRate = 0.0; // hz
};

template <class T, class U>
SimpInverter<T,U>::SimpInverter(const T * in, const U * out)
{
    this->InDevice = in;
    this->OutDevice = out;
}

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
int SimpInverter<T,U>::runSimulation(const double dt)
{
    this->OutVolt = min(this->ProgVolt, this->InVolt/sqrt(2));
    this->OutCurr = min(this->ProgCurr, this->InCurr/sqrt(2));
    this->OutFreq = this->ProgFreq;

    this->OutDevice->setInputVoltage(this->OutVolt);
    this->OutDevice->setInputCurrent(this->OutCurr);
    this->OutDevice->setInputFrequency(this->OutFreq);


    return 0;
}

template <class T, class U>
SimpInverter<T,U>::~SimpInverter()
{
    this->InDevice = NULL;
    this->OutDevice = NULL;
}


// Simplified model of AC induction motor [inv->ENG->mech]
template <class T, class U>
class SimpAsyncEngine : public ElectricalBase
{
public:
    SimpAsyncEngine(const T * in, const U * out, const unsigned int poles, const double statActResist, const double rotActResist,
                    const double airActResist, const bool enableReactConst = true, const double statReactResist = 0.0, const double rotReactResist = 0.0,
                    const double magnetReactResist = 0.0);

    T * InDevice;
    U * OutDevice;

    void setInputVoltage(const double u, const bool isCurrentPersist = true);
    void setInputCurrent(const double i);
    void setInputFrequency(const double f);
    void setStatorReactiveResistance(const double x);
    void setRotorReactiveResistance(const double x);    // Note: only works, when IsReactConst is true
    void setMagneticReactiveResistance(const double x);
    void setEfficiency(const double n);

    // true: reactive resistance is dynamic and is calculated by formula
    // false: reactive resistance is static and being constant
    void switchReactiveConst(const bool enable);

    double getOutputTorque();
    double getOutputRotationRate();

    int runSimulation(const double dt);

    virtual ~SimpAsyncEngine();

private:
    // Input values
    double InVolt = 0.0; // V
    double InCurr = 0.0; // A
    double InFreq = 0.0; // hz

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
    double StatActResist = 0.0; // ohm
    double StatReactResist = 0.0;
    double StatInduct = 0.07; // H // By default: 0.07 (temporarily)
    double RotActResist = 0.0;
    double RotReactResist = 0.0;
    double RotInduct = 2; // By default: 2 (temporarily)
    double MagnetReactResist = 0.0;
    double AirActResist = 0.0;
    bool IsReactConst = true;

    // Misc values
    double Slip = 0.0;
    double Efficiency = 1.0; // We can change this value between 0 and 1
    double MagnetRotRate = 0.0;

    // Mechanical (output) values
    double Torque = 0.0; // N*m
    double RotRate = 0.0; // rpm
};

template <class T, class U>
SimpAsyncEngine<T,U>::SimpAsyncEngine(const T * in, const U * out, const unsigned int poles, const double statActResist, const double rotActResist,
                                 const double airActResist, const bool enableReactConst, const double statReactResist, const double rotReactResist,
                                 const double magnetReactResist)
{
    this->InDevice = in;
    this->OutDevice = out;

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

template <class T, class U>
void SimpAsyncEngine<T,U>::setInputVoltage(const double u, const bool isCurrentPersist)
{
    this->InVolt = u;
    if (!isCurrentPersist) this->InCurr = INT_MAX;
}

template <class T, class U>
void SimpAsyncEngine<T,U>::setInputCurrent(const double i)
{
    this->InCurr = i;
}

template <class T, class U>
void SimpAsyncEngine<T,U>::setInputFrequency(const double f)
{
    this->InFreq = f;
}

template <class T, class U>
void SimpAsyncEngine<T,U>::setStatorReactiveResistance(const double x)
{
    this->StatReactResist = x;
}

template <class T, class U>
void SimpAsyncEngine<T,U>::setRotorReactiveResistance(const double x)
{
    this->RotReactResist = x;
}

template <class T, class U>
void SimpAsyncEngine<T,U>::setEfficiency(const double n)
{
    this->Efficiency = comp(0.0, 1.0, n);
}

template <class T, class U>
void SimpAsyncEngine<T,U>::switchReactiveConst(const bool enable)
{
    this->IsReactConst = enable;
}

template <class T, class U>
double SimpAsyncEngine<T,U>::getOutputTorque()
{
    return this->Torque;
}

template <class T, class U>
double SimpAsyncEngine<T,U>::getOutputRotationRate()
{
    return this->RotRate;
}


// NOTE: This model is very very simplified. Many values are unused.
// For now I need it just to make sure, that the program will run fine
template <class T, class U>
int SimpAsyncEngine<T,U>::runSimulation(const double dt)
{
    //this->StatVolt = this->InVolt;
    //this->StatCurr = this->InCurr;
    this->StatFreq = this->InFreq;
    this->RotRate = this->OutDevice->getRotRate();
    if (!IsReactConst)
    {
        this->StatReactResist = 2 * PI * this->StatFreq * this->StatInduct;
        this->RotReactResist = 2 * PI * this->RotFreq * this->RotInduct;
    }

    try
    {
        this->MagnetRotRate = (120 * this->InFreq) / this->Poles;
        this->Slip = (this->MagnetRotRate - this->RotRate / 60) / this->MagnetRotRate;
        this->StatorCurr = ( this->InVolt / ( sqrt(sqr(this->StatActResist + this->RotActResist) + sqr(this->StatReactResist + this->RotReactResist)) ) ) + this->InVolt / this->AirActResist;
        this->Torque = ( (3 * sqr(this->InVolt) * (this->RotActResist / this->Slip)) / (2 * PI * this->InFreq * (sqr(this->StatActResist + this->RotActResist/this->Slip) + sqr(this->StatReactResist + this->RotReactResist))) ) * this->Efficiency;
    }
    catch (_exception e)
    {
        return 1;
    }

    return 0;
}

template <class T, class U>
SimpAsyncEngine<T,U>::~SimpAsyncEngine()
{
    this->InDevice = NULL;
    this->OutDevice = NULL;
}


// Mechanical entity
// In this case it will be a train wheel on rails
template <class T>
class TrainWheel
{
public:
    TrainWheel(const T * in);

    T * InDevice;

    void setTorque(const double m);
    void setRotRate(const double n);

    double getRotRate();

    int runSimulation(const double dt);

    virtual ~TrainWheel();

private:
    // Wheel kinetic values
    double RotRate = 0.0; // rpm
    double Torque = 0.0; // N*m

    // Wheel characteristics
    double Mass = 1800; // kg
    double Radius = 0.86; // m
};

template <class T>
TrainWheel<T>::TrainWheel(const T * in)
{
    this->InDevice = in;
}

template <class T>
void TrainWheel<T>::setTorque(const double m)
{
    this->Torque = m;
}

template <class T>
void TrainWheel<T>::setRotRate(const double n)
{
    this->RotRate = n;
}

template <class T>
double TrainWheel<T>::getRotRate()
{
    return this->RotRate;
}

template <class T>
int TrainWheel<T>::runSimulation(const double dt)
{
    try
    {
        this->RotRate = (this->Torque * dt) / (PI * this->Mass * this->Radius);
    }
    catch (_exception e)
    {
        return 1;
    }

    return 0;
}

template <class T>
TrainWheel<T>::~TrainWheel()
{
    this->InDevice = NULL;
}
