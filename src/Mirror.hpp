using namespace ci;

class Mirror{
public:
    Mirror(){};
    void    setColor(Color c);
    void    setRotationalPosition(double rp);
    void    setDiffence(double d);
    void    setId(int id);
    Color   getColor();
    double  getRotationalPosition();
    double  getDifference();
    int     getId();
private:
    int mId;
    Color mColor;
    double mRotationalPosition;
    double mDifference;
};