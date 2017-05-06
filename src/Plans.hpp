#pragma once

#include <memory>
#include <string>

struct Plan final {
    std::string m_item;
    std::uint8_t m_rule1_index;
    std::uint8_t m_rule2_index;
    std::uint8_t m_rule3_index;
};

class Plans final {
    struct Private;
    std::unique_ptr<Private> m_p;
public:
    Plans();
    ~Plans();

    enum class Status {
        ok,
        cannot_open,
        cannot_parse
    };

    Status getStatus() const;

    std::size_t getNumPlans() const;
    const Plan& getPlan(std::size_t index) const;
};
