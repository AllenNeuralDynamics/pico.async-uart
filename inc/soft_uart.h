#ifndef SOFT_UART_H
#define SOFT_UART_H

#include <pico/stdlib.h>

/**
 * \brief an 8N1 Software-based UART implemented with timers.
 */
class SoftUART
{
public:

    enum state_t
    {
        START_BIT,
        BIT_TRANSFER,
        STOP_BIT,
        IDLE
    };

    SoftUART(uint32_t gpio_pin);

    SoftUART(uint32_t gpio_pin, uint32_t baud_rate);

    ~SoftUART();

    void send(uint8_t* data, size_t num_bytes);

    inline void set_baud_rate(uint32_t baud_rate);

    inline bool is_busy();

    inline bool requires_update();

    void update();

    void reset();
    inline void reset_fsm();
    void cleanup();

private:
    uint32_t tx_pin_;
    uint32_t baud_interval_us_;
    state_t state_;
    uint8_t* data_;
    size_t bits_to_transfer_;
    size_t byte_index_;
    size_t bytes_to_transfer_;
    uint32_t next_update_time_us_;

};

// Inline function definitions

void SoftUART::set_baud_rate(uint32_t baud_rate)
{
    baud_interval_us_ = 1'000'000UL / baud_rate;
}

bool SoftUART::requires_update()
{
    if (state_ == IDLE)
        return false;
    uint32_t curr_time_us = time_us_32();
    if (int32_t(curr_time_us - next_update_time_us_) >= 0)
        return true;
    return false;
}

bool SoftUART::is_busy()
{
    return state_ != IDLE;
}
#endif // SOFT_UART_H
