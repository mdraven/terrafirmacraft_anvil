
#include "Plans.hpp"

#include "Rule.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <map>

struct UnknownRuleName : public std::runtime_error {
    UnknownRuleName(const std::string& reason)
        : std::runtime_error(reason) {}
};

struct Plans::Private {
    Status m_status;
    std::map<std::string/*item*/, Plan> m_plans;

    static std::uint8_t getRuleIndex(const std::string& rule_name);

    Private();
};

Plans::Private::Private() {
    namespace pt = boost::property_tree;

    try {
        pt::ptree root;
        pt::read_json("plans.json", root);

        for(const auto& pair : root) {
            const pt::ptree& child = pair.second;

            Plan plan;
            plan.m_item = child.get<std::string>("item");
            plan.m_rule1_index = getRuleIndex(child.get<std::string>("rule1"));
            plan.m_rule2_index = getRuleIndex(child.get<std::string>("rule2"));
            plan.m_rule3_index = getRuleIndex(child.get<std::string>("rule3"));

            m_plans[plan.m_item] = std::move(plan);
        }
    }
    catch(const UnknownRuleName&) {
        m_status = Status::cannot_parse;
        m_plans.clear();
        return;
    }
    catch(const pt::json_parser_error& e) {
        m_status = Status::cannot_open;
        return;
    }

    m_status = Status::ok;
}

std::uint8_t Plans::Private::getRuleIndex(const std::string& rule_name) {
    for(std::size_t i = 0; i < g_rules.size(); ++i)
        if(g_rules[i].getName() == rule_name)
            return i;

    throw UnknownRuleName(rule_name);
}

Plans::Plans()
    : m_p(new Private()) {}

Plans::~Plans() = default;

Plans::Status Plans::getStatus() const {
    return m_p->m_status;
}

std::size_t Plans::getNumPlans() const {
    return m_p->m_plans.size();
}

const Plan& Plans::getPlan(std::size_t index) const {
    auto it = m_p->m_plans.begin();
    std::advance(it, index);

    return it->second;
}
