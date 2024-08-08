#include <stdint.h>
#include <memory>
#include <string>

#include "misra2023.h"

#define RULE_19_2_2 "rule_19_2_2.h"
#include RULE_19_2_2

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

static constexpr long double operator"" _km_5_10_1(long double len) noexcept
{
    return len;
}

// User-defined identifiers shall have an appropriate form
static void rule_5_10_1()
{
    const long double len = 10.0_km_5_10_1;
    (void)len;
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

// The "union" keyword shall not be used
static void rule_12_3_1()
{
    union Union_12_3_1
    {
        uint32_t u32;
        float f32;
    };

    Union_12_3_1 u_12_3_1{10};
    (void)u_12_3_1;
}

// The parameters in all declarations or overrides of a function shall either be unnamed or have identical names
static void rule_13_3_3()
{
    struct Base
    {
        virtual ~Base() = default;
        virtual void func(uint32_t value)
        {}
    };

    struct Derived : Base
    {
        void func(uint32_t otherNameForValue) override
        {}
    };

    Derived derived;
    derived.func(1);
}

// Special member functions shall be provided appropriately
static void rule_15_0_1()
{
    class BaseWithPublicVirtualDtor
    {
    public:
        virtual ~BaseWithPublicVirtualDtor() = default;
    };

    class BaseWithProtectedNonVirtualDtor
    {
    protected:
        ~BaseWithProtectedNonVirtualDtor() = default;
    };

    class BaseWithoutAppropriateDtor
    {
    public:
        virtual void work() = 0;
    };

    class DerivedA : public BaseWithPublicVirtualDtor
    {
    };

    class DerivedB : public BaseWithProtectedNonVirtualDtor
    {
    };

    class DerivedC : public BaseWithoutAppropriateDtor
    {
    public:
        void work() override
        {}
    };

    DerivedA a;
    DerivedB b;
    DerivedC c;

    (void)a;
    (void)b;
    (void)c;
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

static void rule_28_6_2_f1(std::string& s)
{
    (void)s;
}

static void rule_28_6_2_f1(std::string&& ss)
{
    (void)ss;
}

template <typename T1, typename T2>
static void rule_28_6_2_f2(T1&& t1, T2& t2)
{
    rule_28_6_2_f1(t1); // non-compliant
    rule_28_6_2_f1(std::forward<T1>(t1));
    rule_28_6_2_f1(std::forward<T2>(t2)); // non-compliant, not a forwarding reference
    rule_28_6_2_f1(std::forward<T2>(t1)); // wrong template argument

    rule_28_6_2_f1(std::move(t1)); // non-compliant
    rule_28_6_2_f1(std::move(t2)); // rule does not apply

    auto lambda = [] (auto&& t) { rule_28_6_2_f1(t); };  // non-compliant
    lambda(std::forward<T1>(t1));
}

template <typename T>
struct Rule_28_6_2
{
    void foo(T&& t)
    {
        std::move(t); // rule does not apply (not a forwarding reference)
    }
};

// "Forwarding references" and "std::forward" shall be used together
static void rule_28_6_2()
{
    std::string s;
    rule_28_6_2_f2(std::string{"hello"}, s);
    rule_28_6_2_f2(s, s);
}

void check_rules()
{
    // Rule 0.1.2 The value returned by a function shall be used
    // - Sonar Rule ID: cpp:M23_007
    // - Issue: https://github.com/ndsev/zserio/issues/607
    rule_0_1_2();

    // Rule 5.10.1 User-defined identifiers shall have an appropriate form
    // - Sonar Rule ID: cpp:S978 (partially matches)
    // - Issue: https://github.com/ndsev/zserio/issues/635
    // - doesn't catch anything, _sv fixed manually, no check in CI
    rule_5_10_1();

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

    // Rule 12.3.1 The "union" keyword shall not be used
    // - Sonar Rule ID: cpp:M23_158
    // Issue: https://github.com/ndsev/zserio/issues/633
    rule_12_3_1();

    // Rule 13.3.3 The parameters in all declarations or overrides of a function shall either be unnamed or have identical names
    // - Sonar Rule ID: cpp:S5319
    // Issue: https://github.com/ndsev/zserio/issues/632
    rule_13_3_3();

    // Rule 15.0.1 Special member functions shall be provided appropriately
    // - Sonar Rule ID: cpp:S1235 (partially matches)
    // - Issue: https://github.com/ndsev/zserio/issues/637
    rule_15_0_1();

    // Rule 15.3.1 Conversion operators and constructors that are callable with a single argument shall be explicit
    // - Sonar Rule ID: cpp:S1709
    // - Issue: https://github.com/ndsev/zserio/issues/593
    rule_15_1_3();

    // Rule 19.2.2 The #include directive shall be followed by either a <filename> or "filename" sequence
    // - Sonar Rule ID: cpp:S956
    // - Issue: https://github.com/ndsev/zserio/issues/639
    rule_19_2_2();

    // Rule 28.6.2 "Forwarding references" and "std::forward" shall be used together
    // - Sonar Rule ID: cpp:M23_279
    // - Issue: https://github.com/ndsev/zserio/issues/634
    rule_28_6_2();
}

} // namespace misra2023
