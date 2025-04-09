#pragma once

class Player {
public:
    Player(float maxHealth = 100.0f) :
        m_currentHealth(maxHealth),
        m_maxHealth(maxHealth) {}

    float GetHealth() const { return m_currentHealth; }
    float GetMaxHealth() const { return m_maxHealth; }

    void TakeDamage(float damage) {
        m_currentHealth = (m_currentHealth - damage < 0) ? 0 : m_currentHealth - damage;
    }

    void Heal(float amount) {
        m_currentHealth = (m_currentHealth + amount > m_maxHealth) ?
            m_maxHealth : m_currentHealth + amount;
    }

    void ResetHealth() { m_currentHealth = m_maxHealth; }

private:
    float m_currentHealth;
    float m_maxHealth;
};