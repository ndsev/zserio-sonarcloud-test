#include <stdint.h>
#include <memory>
#include <string>

#include "misra2023.h"

namespace misra2023
{

// The value returned by a function shall be used
static void rule_0_1_2()
{
    struct Rule
    {
        static uint32_t func() { return 0; }
    };

    Rule::func(); // return value not used here
}

// A variable declared in an inner scope shall not hide a variable declared in an outer scope
static void rule_6_4_1()
{
    class Test
    {
    public:
        explicit Test(const std::allocator<uint8_t>& allocator) :
        //                                           ^ hides 'allocator' parameter to the func
                m_allocator(allocator)
        {

        };

        const std::allocator<uint8_t>& allocator() const
        //                             ^ hides 'allocator'parameter to the func
        {
            return m_allocator;
        }

        std::allocator<uint8_t> m_allocator;
    };

    struct Functions
    {
        static void func(const std::allocator<uint8_t>& allocator)
        {

            const Test t = Test(allocator);
            (void)t.allocator();
        }
    };

    Functions::func(std::allocator<uint8_t>());
}

static const std::string RULE_6_7_2 = "MISRA C++ 2024 6.7.2 - Global variables shall not be used";

// Global variables shall not be used
static void rule_6_7_2()
{
    (void)RULE_6_7_2;
}

// Integral promotion or the usual arithmetic conversions shall not change the type signedness
static void rule_7_0_5()
{
    {
        uint16_t a = 0xff;
        uint16_t b = 8;
        const uint16_t c = static_cast<uint16_t>(a << b);
        //                                         ^ integral promotions of a and b happen here, result is int
        (void)c;
    }

    {
        uint32_t a = 0xff;
        float b = 0.0f;
        b = b + a; // uint32_t -> float
        (void)b;
    }
}

// Assignment between numeric types shall be appropriate
static void rule_7_0_6()
{
    {
        // does not work with integral promotions
        uint16_t a = 0x000f;
        uint16_t b = 0x00f0;
        uint16_t c = 0x0f00;
        const uint16_t d = static_cast<uint16_t>(a | b) | c;
        //                                         ^ integral promotions of (a | b) and c happen here, result is int
        (void)d;
    }

    {
        int32_t a = 0x000f;
        uint32_t b = 0x00f0;
        const int32_t c = a + b; // non-appropriate assignment
        (void)c;
    }
}

// C-style casts and functional casts shall not be used.
static void rule_8_2_2()
{
    const uint16_t value = 0xffff;
    const int16_t signed_value = (uint16_t)value;
    //                           ^ c-style cast used here
    const int16_t other_signed_value = uint16_t(value);
    //                                 ^ functional cast used here
    (void)signed_value;
    (void)other_signed_value;
}

// reinterpret_cast shall not be used
static void rule_8_2_5()
{
    const uint32_t value = 0xff00ff00U;
    const uint16_t* u16 = reinterpret_cast<const uint16_t*>(&value);
    //                    ^ reinterpret_cast used here
    (void)u16;
}

// The body of an iteration-statement or a selection-statement shall be a compound-statement
static void rule_9_3_1()
{
    const bool cond = true;
    if (cond)
        return; // not a compound statement here

    for (uint32_t i = 0; i < 10; ++i)
        continue; // not a compound statement here
}

// Conversion operators and constructors that are callable with a single argument shall be explicit
static void rule_15_1_3()
{
    class NoExplicitCtor
    {
    public:
        NoExplicitCtor(uint32_t value) : // not explicit constructor here
                m_value(value)
        {}

        operator uint32_t() const // not explicit conversion operator here
        {
            return m_value;
        }

    private:
        uint32_t m_value;
    };

    struct Functions
    {
        static uint32_t func(const NoExplicitCtor& obj)
        {
            return other_func(obj);
        }

        static uint32_t other_func(uint32_t value)
        {
            return value;
        }
    };

    constexpr uint32_t value = 4;
    (void)Functions::func(value);
}

void check_rules()
{
    // Rule 0.1.2 The value returned by a function shall be used
    // - Sonar Rule ID: cpp:M23_007
    // - Issue: https://github.com/ndsev/zserio/issues/607
    rule_0_1_2();

    // Rule 6.4.1 A variable declared in an inner scope shall not hide a variable declared in an outer scope
    // - Open Issue: https://github.com/ndsev/zserio/issues/609 - wontfix
    // - not covered by any sonar rule, some problems fixed manually, no check in CI
    rule_6_4_1();

    // Rule 6.7.2 Global variables shall not be used
    // - Sonar Rule ID: cpp:M23_388
    // - Issue: https://github.com/ndsev/zserio/issues/611
    rule_6_7_2();

    // Rule 7.0.5 Integral promotion or the usual arithmetic conversions shall not change the type signedness
    // - Sonar Rule ID: cpp:S5276, cpp:S845
    // - Open Issue: https://github.com/ndsev/zserio/issues/605 - wontfix
    // - not covered by any sonar rule
    rule_7_0_5();

    // Rule 7.0.6 Assignment between numeric types shall be appropriate
    // - Sonar Rule ID: cpp:S845
    // - Issue: https://github.com/ndsev/zserio/issues/608
    // - integral promotions not covered by any sonar rule
    rule_7_0_6();

    // Rule 8.2.2 C-style casts and functional notation casts shall not be used
    // - Sonar Rule ID: cpp:M23_089
    // - Issue: https://github.com/ndsev/zserio/issues/610
    rule_8_2_2();

    // Rule 8.2.5 reinterpret_cast shall not be used
    // - Sonar Rule ID: cpp:S3630
    // - Open Issue: https://github.com/ndsev/zserio/issues/586
    rule_8_2_5();

    // Rule 9.3.1 The body of an iteration-statement or a selection-statement shall be a compound-statement
    // - Sonar Rule ID: cpp:S121, cpp:S5305
    // - Issue: https://github.com/ndsev/zserio/issues/606
    rule_9_3_1();

    // Rule 15.3.1 Conversion operators and constructors that are callable with a single argument shall be explicit
    // - Sonar Rule ID: cpp:S1709
    // - Issue: https://github.com/ndsev/zserio/issues/593
    rule_15_1_3();
}

} // namespace misra2023
