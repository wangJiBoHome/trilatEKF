/** @file trilatEKF.h
 *  @brief trilatEKF structs and class definition
 *  @author Yannik Nager
 */

#ifndef TRILAT_EKF_H_
#define TRILAT_EKF_H_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "Eigen/Dense"
#include "kalman.h"
#include "helpers.h"

#define SUNFLOWER_NR 3
#define VAR_Z 0.1 // sunflower meas variance
#define MEAS_NOISE_X 5.0
#define MEAS_NOISE_Y 5.0

using namespace Eigen;

/** @brief Single measurement */
struct Measurement {
    long long timestamp;
    Vector2d sensorloc;
    double distance;
};

/** Trilateration measurement struct */
struct TrilatMeasurement {
    long long timestamp;
    MatrixXd sensorlocs;
    VectorXd distances;
};

/** Trilateration EKF params */
struct TrilatParams {
    float var_z;
    float var_x;
    float var_y;
};

/**
 * TrilatEKF class
 */
class TrilatEKF{
public:
    /** Kalman Filter */
    Kalman ekf_;
    
    /**
     * @brief Constructor
     */
    TrilatEKF(const VectorXd &x_init, const MatrixXd &sensorloc, const TrilatParams &p);
    
    /**
     * @brief Destructor
     */
    ~TrilatEKF();
    
    /**
     * @brief Process measurement of type TrilatMeasurement
     * @param Trilateration measurement package
     * @return void
     */
    void processMeasurements(std::vector<TrilatMeasurement>* trilatMeasurements);
    
    /**
     * @brief calculate jacobian of measurement matrix
     * @param sensorlocs current sensor locations
     * @param x current state estimate
     * @return jacobian matrix
     */
    MatrixXd getJacobian(const MatrixXd &sensorlocs, const VectorXd &x);
    
    /**
     * @brief calculate jacobian of measurement matrix
     * @param m TrilatMeasurement
     * @return jacobian matrix
     */
    MatrixXd getJacobian(const TrilatMeasurement &m);
    
    /**
     * @brief get measurement combinations
     * @param mvec vector containing 6 measurements (2 measurements per sensor)
     * @note measurements must be sorted by sensor
     * @return vector of TrilatMeasurements
     */
    static std::vector<TrilatMeasurement> getCombinations(const std::vector<Measurement> &mvec);
    
    /**
     * @brief Match single measurements into trilat measurement
     * @param m0 distance to object of sensor 0
     * @param m1 distance to object of sensor 1
     * @param m2 distance to object of sensor 2
     * @return TrilatMeasurement
     */
    static TrilatMeasurement toTrilatMeasurement(const Measurement &m0, const Measurement &m1, const Measurement &m2);
    
private:
    
    Eigen::MatrixXd R_;
    Eigen::MatrixXd H_;
    TrilatParams p_;
    long long timestamp_prev_;
    int statesize_;
};

#endif
