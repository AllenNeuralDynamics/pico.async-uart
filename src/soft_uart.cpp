#include <soft_uart.h>

SoftUART::SoftUART(uint32_t tx_pin)
: tx_pin_{tx_pin}
{
    gpio_init(tx_pin_);
    gpio_set_dir(tx_pin_, GPIO_OUT);
    gpio_put(tx_pin_, 1);
    reset(); // Also resets FSM.
}

SoftUART::SoftUART(uint32_t tx_pin, uint32_t baud_rate)
:SoftUART(tx_pin)
{
    set_baud_rate(baud_rate);
}

SoftUART::~SoftUART()
{cleanup();}

void SoftUART::cleanup()
{
    gpio_deinit(tx_pin_);
}

void SoftUART::send(uint8_t* data, size_t num_bytes)
{
    if (is_busy())
    {
#if defined(DEBUG)
        printf("Soft UART is busy.");
#endif
        return;
    }
    reset_fsm();
    data_ = data;
    bytes_to_transfer_ = num_bytes;
    state_ = START_BIT;
    next_update_time_us_ = time_us_32();
    update();
}

void SoftUART::update()
{
    // Update state machine
    state_t next_state{state_};
    switch(state_)
    {
        case START_BIT:
            next_state = BIT_TRANSFER;
            break;
        case BIT_TRANSFER:
            if (bits_to_transfer_ == 1)
                next_state = STOP_BIT;
            break;
        case STOP_BIT:
            if (bytes_to_transfer_ == 0)
                next_state = IDLE;
            else
                next_state = START_BIT;
        case IDLE:
            break;
        default:
            break;
    }

    if (state_ == START_BIT)
        gpio_put(tx_pin_, 0);
    if (state_ == BIT_TRANSFER)
    {   // LSbit to MSbit
        gpio_put(tx_pin_, (0x01 &
                           (data_[byte_index_] >> (8-bits_to_transfer_))));
        bits_to_transfer_ -= 1;
        if (bits_to_transfer_ == 0)
        {
            bits_to_transfer_ = 8;
            bytes_to_transfer_ -= 1;
            byte_index_ += 1;
        }
    }
    if (state_ == STOP_BIT)
    {
        gpio_put(tx_pin_, 1);
    }
    if (state_ == STOP_BIT)
        gpio_put(tx_pin_, 1);
    if (state_ != IDLE)
        next_update_time_us_ += baud_interval_us_;

    // Apply next state.
    state_ = next_state;
}

void SoftUART::reset()
{
    gpio_put(tx_pin_, 1);
    reset_fsm();
}

void SoftUART::reset_fsm()
{
    data_ = nullptr;
    state_ = IDLE;
    bits_to_transfer_ = 8; // 8 bits, 1 start bit, 1 stop bit
    byte_index_ = 0;
}

