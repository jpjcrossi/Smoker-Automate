#pragma once
class OffSetModel
{
private:
    /* data */
public:
    OffSetModel();
    ~OffSetModel();
    double OffSet = 0;
    double Temperature=0;
    double Tolerance=0;
    double NaturalFlow =0; 
};

OffSetModel::OffSetModel()
{
}

OffSetModel::~OffSetModel()
{
}
