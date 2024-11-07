#ifndef DATASET_H
#define DATASET_H
# include <math.h>
# include <QString>
# include <QStringList>
# include <QFile>
# include <QTextStream>
# include <vector>
# include <iostream>
using namespace std;

/**
 * @brief Data, a double precision vector
 */
typedef vector<double> Data;
/**
 * @brief Matrix, a double precision vector of two dimensions
 */
typedef vector<Data> Matrix;

class Dataset
{
private:
    /**
     * @brief xpoint the xdata of the dataset
     */
    Matrix xpoint;
    /**
     * @brief ypoint the expected values for the dataset
     */
    Data   ypoint;
    /**
     * @brief classVector the list of classes for the dataset
     */
    Data   classVector;
    /**
     * @brief makeClassVector creates the classVector param.
     */
    void    makeClassVector();
public:
    /**
     * @brief Dataset the default construtor
     */
    Dataset();
    /**
     * @brief loadFromDataFile loads the dataset from a "data" file
     * @param filename
     */
    void    loadFromDataFile(QString filename);
    /**
     * @brief loadFromArffFile loads the dataset from an "arff" file
     * @param filename
     */
    void    loadFromArffFile(QString filename);
    /**
     * @brief loadFromCsvFile loads the dataset from an "csv" file
     * @param filename
     */
    void    loadFromCsvFile(QString filename);

    /**
     * @brief getXpoint
     * @param pos
     * @return  the xpoint at position pos
     */
    Data    &getXpoint(int pos);
    /**
     * @brief getYpoint
     * @param pos
     * @return  the expected value at position pos
     */
    double  getYpoint(int pos) const;
    /**
     * @brief dimension
     * @return  the dimension of the given dataset
     */
    int     dimension() const;
    /**
     * @brief count
     * @return  the number of points the set
     */
    int     count() const;
    /**
     * @brief clearPoints removes all points in the set.
     */
    void    clearPoints();
    /**
     * @brief addPoint adds a new point in the set
     * @param x
     * @param y
     */
    void    addPoint(Data &x,double y);
    /**
     * @brief getPoint returns the (x,y) point in the provided position
     * @param pos
     * @param x
     * @param y
     */
    void    getPoint(int pos,Data &x,double y);
    /**
     * @brief getClass
     * @param pos
     * @return  the class for a particular point in the dataset
     */
    double  getClass(int pos);
    /**
     * @brief estimateClass
     * @param value
     * @return  the estimated class for a given value
     */
    double  estimateClass(double value);
    /**
     * @brief getAllXpoint
     * @return  the list of xpoints in the dataset
     */
    Matrix getAllXpoint() const;

    ~Dataset();
};

#endif // DATASET_H
