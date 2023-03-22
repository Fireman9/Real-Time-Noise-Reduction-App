class AdaptiveKalmanFilter
{
  private:
    float m_x;     // state estimate
    float m_P;     // error covariance estimate
    float m_Q;     // process noise covariance
    float m_R;     // measurement noise covariance
    float m_A;     // state transition matrix
    float m_H;     // observation matrix
    float m_alpha; // forgetting factor for adaptive estimation

  public:
    AdaptiveKalmanFilter(float x, float P, float Q, float R, float alpha) :
        m_x(x), m_P(P), m_Q(Q), m_R(R), m_A(1), m_H(1), m_alpha(alpha)
    {}

    float update(float z)
    {
        // Predict step
        float x_hat = m_A * m_x;
        float P_hat = m_A * m_P * m_A + m_Q;

        // Update step
        float K = P_hat * m_H / (m_H * P_hat * m_H + m_R);
        float x_new = x_hat + K * (z - m_H * x_hat);
        float P_new = (1 - K * m_H) * P_hat;

        // Adaptation step
        float alpha_t = m_alpha / (1 + m_alpha * m_H * P_hat * m_H);
        m_A = m_A + K * (z - m_H * m_A * m_x) * alpha_t;
        m_H = m_H + K * m_A * alpha_t;

        // Update state and error covariance estimates
        m_x = x_new;
        m_P = P_new;

        return m_x;
    }
};
