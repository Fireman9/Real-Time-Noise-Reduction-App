#ifndef KALMAN_H
#define KALMAN_H

/// @brief This class implements a Kalman filter for estimating the true state
/// of a system.
class Kalman
{
  private:
    /// @brief Process noise covariance.
    double m_Q;

    /// @brief Measurement noise covariance.
    double m_R;

    /// @brief State estimate.
    double m_x;

    /// @brief State covariance.
    double m_P;

    /// @brief Predicted state estimate.
    double m_x_minus;

    /// @brief Predicted state covariance.
    double m_P_minus;

    /// @brief Kalman gain.
    double m_K;

  public:
    /// @brief Constructor for initializing a Kalman filter object.
    /// @param Q Process noise covariance.
    /// @param R Measurement noise covariance.
    Kalman(double Q, double R)
    {
        m_Q = Q;
        m_R = R;
        m_x = 0;
        m_P = 1;
        m_K = 0;
    }

    /// @brief Updates the state estimate and covariance based on a new
    /// measurement.
    /// @param value The new measurement.
    /// @return The updated state estimate.
    double update(double value)
    {
        // predict
        m_x_minus = m_x;
        m_P_minus = m_P + m_Q;

        // update
        m_K = m_P_minus / (m_P_minus + m_R);
        m_x = m_x_minus + m_K * (value - m_x_minus);
        m_P = (1 - m_K) * m_P_minus;

        return m_x;
    }
};

#endif // KALMAN_H
