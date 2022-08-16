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
//template <template <class> class U, class T>
class DCSource : public ElectricalBase
{
public:
    DCSource(const double u, const bool isCurrentPersist = false, const double i = 0.0);

    //typename T * InDevice;
    //U * OutDevice;

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

//template <template <class> class U, class T>
DCSource::DCSource(const double u, const bool isCurrentPersist, const double i)
{
    //this->InDevice = in;
    //this->OutDevice = out;
    this->OutVolt = u;
    this->OutCurr = (isCurrentPersist) ? i : INT_MAX;
}

//template <template <class> class U, class T>
void DCSource::setOutputVoltage(const double u)
{
    this->OutVolt = u;
}

//template <template <class> class U, class T>
void DCSource::setOutputCurrent(const double i)
{
    this->OutCurr = i;
}

//template <template <class> class U, class T>
double DCSource::getOutputVoltage()
{
    return this->OutVolt;
}

//template <template <class> class U, class T>
double DCSource::getOutputCurrent()
{
    return this->OutCurr;
}

////template <template <class> class U, class T>
int DCSource::runSimulation(const double dt)
{
    //this->OutDevice->setInputVoltage(this->OutVolt, (this->OutCurr == INT_MAX) ? false : true);

    return 0;
}

//template <template <class> class U, class T>
DCSource::~DCSource()
{
    //this->InDevice = NULL;
    //this->OutDevice = NULL;
}


// Simple converter (Inverter) DC->AC [src->INV->eng]
//template <class T, class U>
class SimpInverter : public ElectricalBase
{
public:
    SimpInverter();
    //SimpInverter();

    //T * InDevice;
    //U * OutDevice;

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

//template <class T, class U>
SimpInverter::SimpInverter()
{
    //this->InDevice = in;
    //this->OutDevice = out;
}

//template <class T, class U>
void SimpInverter::setInputVoltage(const double u, const bool isCurrentPersist)
{
    this->InVolt = u;
    if (!isCurrentPersist) this->InCurr = INT_MAX;
}

//template <class T, class U>
void SimpInverter::setInputCurrent(const double i)
{
    this->InCurr = i;
}

//template <class T, class U>
double SimpInverter::getOutputVoltage()
{
    return this->OutVolt;
}

//template <class T, class U>
double SimpInverter::getOutputCurrent()
{
    return this->OutCurr;
}

//template <class T, class U>
double SimpInverter::getOutputFrequency()
{
    return this->OutFreq;
}

//template <class T, class U>
void SimpInverter::setProgrammVoltage(const double u)
{
    this->ProgVolt = u;
}

//template <class T, class U>
void SimpInverter::setProgrammCurrent(const double i)
{
    this->ProgCurr = i;
}

//template <class T, class U>
void SimpInverter::setProgrammFrequency(const double f)
{
    this->ProgFreq = f;
}

//template <class T, class U>
int SimpInverter::runSimulation(const double dt)
{
    this->OutVolt = min(this->ProgVolt, this->InVolt/sqrt(2));
    this->OutCurr = min(this->ProgCurr, this->InCurr/sqrt(2));
    this->OutFreq = this->ProgFreq;

    //this->OutDevice->setInputVoltage(this->OutVolt);
    //this->OutDevice->setInputCurrent(this->OutCurr);
    //this->OutDevice->setInputFrequency(this->OutFreq);


    return 0;
}

//template <class T, class U>
SimpInverter::~SimpInverter()
{
    //this->InDevice = NULL;
    //this->OutDevice = NULL;
}


// Simplified model of AC induction motor [inv->ENG->mech]
//template <class U>
class SimpAsyncEngine : public ElectricalBase
{
public:
    SimpAsyncEngine(const unsigned int poles, const double statActResist, const double rotActResist, const double airActResist,
                    const bool enableReactConst = true, const double statReactResist = 0.0, const double rotReactResist = 0.0,
                    const double magnetReactResist = 0.0);

    //T * InDevice;
    //U * OutDevice;

    void setInputVoltage(const double u, const bool isCurrentPersist = true);
    void setInputCurrent(const double i);
    void setInputFrequency(const double f);
    void setStatorReactiveResistance(const double x);
    void setRotorReactiveResistance(const double x);    // Note: only works, when IsReactConst is true
    void setMagneticReactiveResistance(const double x);
    void setEfficiency(const double n);
    void setRotationRate(const double n);

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

//template <class U>
SimpAsyncEngine::SimpAsyncEngine(const unsigned int poles, const double statActResist, const double rotActResist,
                                 const double airActResist, const bool enableReactConst, const double statReactResist, const double rotReactResist,
                                 const double magnetReactResist)
{
    //this->InDevice = in;
    //this->OutDevice = out;

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

//template <class U>
void SimpAsyncEngine::setInputVoltage(const double u, const bool isCurrentPersist)
{
    this->InVolt = u;
    if (!isCurrentPersist) this->InCurr = INT_MAX;
}

//template <class U>
void SimpAsyncEngine::setInputCurrent(const double i)
{
    this->InCurr = i;
}

//template <class U>
void SimpAsyncEngine::setInputFrequency(const double f)
{
    this->InFreq = f;
}

//template <class U>
void SimpAsyncEngine::setStatorReactiveResistance(const double x)
{
    this->StatReactResist = x;
}

//template <class U>
void SimpAsyncEngine::setRotorReactiveResistance(const double x)
{
    this->RotReactResist = x;
}

//template <class U>
void SimpAsyncEngine::setMagneticReactiveResistance(const double x)
{
    this->MagnetReactResist = x;
}

//template <class U>
void SimpAsyncEngine::setEfficiency(const double n)
{
    this->Efficiency = comp(0.0, 1.0, n);
}

//template <class U>
void SimpAsyncEngine::setRotationRate(const double n)
{
    this->RotRate = n;
}

//template <class U>
void SimpAsyncEngine::switchReactiveConst(const bool enable)
{
    this->IsReactConst = enable;
}

//template <class U>
double SimpAsyncEngine::getOutputTorque()
{
    return this->Torque;
}

//template <class U>
double SimpAsyncEngine::getOutputRotationRate()
{
    return this->RotRate;
}


// NOTE: This model is very very simplified. Many values are unused.
// For now I need it just to make sure, that the program will run fine
//template <class U>
int SimpAsyncEngine::runSimulation(const double dt)
{
    //this->StatVolt = this->InVolt;
    //this->StatCurr = this->InCurr;
    this->StatFreq = this->InFreq;
    //this->RotRate = this->OutDevice->getRotRate();
    if (!IsReactConst)
    {
        this->StatReactResist = 2 * PI * this->StatFreq * this->StatInduct;
        this->RotReactResist = 2 * PI * this->RotFreq * this->RotInduct;
    }

    try
    {
        this->MagnetRotRate = (120 * this->InFreq) / this->Poles;
        this->Slip = (this->MagnetRotRate - this->RotRate / 60) / this->MagnetRotRate;
        this->StatCurr = ( this->InVolt / ( sqrt(sqr(this->StatActResist + this->RotActResist) + sqr(this->StatReactResist + this->RotReactResist)) ) ) + this->InVolt / this->AirActResist;
        this->Torque = ( (3 * sqr(this->InVolt) * (this->RotActResist / this->Slip)) / (2 * PI * this->InFreq * (sqr(this->StatActResist + this->RotActResist/this->Slip) + sqr(this->StatReactResist + this->RotReactResist))) ) * this->Efficiency;
    }
    catch (_exception e)
    {
        return 1;
    }

    return 0;
}

//template <class U>
SimpAsyncEngine::~SimpAsyncEngine()
{
    //this->InDevice = NULL;
    //this->OutDevice = NULL;
}


// Mechanical entity
// In this case it will be a train wheel on rails
//template <class T>
class TrainWheel
{
public:
    TrainWheel();

    //T * InDevice;

    void setTorque(const double m);
    void setRotationRate(const double n);

    double getRotationRate();

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

//template <class T>
TrainWheel::TrainWheel()
{
    //this->InDevice = in;
}

//template <class T>
void TrainWheel::setTorque(const double m)
{
    this->Torque = m;
}

//template <class T>
void TrainWheel::setRotationRate(const double n)
{
    this->RotRate = n;
}

//template <class T>
double TrainWheel::getRotationRate()
{
    return this->RotRate;
}

//template <class T>
int TrainWheel::runSimulation(const double dt)
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

//template <class T>
TrainWheel::~TrainWheel()
{
    //this->InDevice = NULL;
}
