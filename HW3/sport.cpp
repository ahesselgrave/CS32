#include <iostream>
#include <string>
using namespace std;

class Sport
{
    public:
        Sport(const string& name) :m_name(name) {}
        virtual ~Sport() {};
        string name() const {return m_name;}
        virtual string icon() const = 0;
        virtual bool isOutdoor() const {return true;};

    private:
        string m_name;

};

class Biathlon : public Sport
{
    public:
        Biathlon(const string& name, const double& distance) : Sport(name), m_distance(distance) {}
        virtual ~Biathlon() {cout << "Destroying the Biathlon object named " << name() << ", distance " << m_distance << " km." << endl;}
        virtual string icon() const {return "a skier with a rifle";}

    private: 
        double m_distance;

};

class Snowboarding : public Sport
{
    public:
        Snowboarding(const string& name) : Sport(name) {}
        virtual ~Snowboarding() {cout << "Destroying the Snowboarding object named " << name() << "." << endl;}
        virtual string icon() const {return "a descending snowboarder";}
};

class FigureSkating : public Sport
{
    public:
        FigureSkating(const string& name) : Sport(name) {}
        virtual ~FigureSkating() {cout << "Destroying the FigureSkating object named " << name() << "." << endl;}
        virtual string icon() const {return "a skater in the Biellmann position";}
        virtual bool isOutdoor() const {return false;}
};

