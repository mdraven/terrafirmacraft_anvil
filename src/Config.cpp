
#include "Config.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

struct Config::Private {
    int m_scale = 0;
    std::string m_screenshots_dir;
    bool m_fail = false;

    Private();
};

Config::Private::Private() {
    namespace pt = boost::property_tree;

    try {
        pt::ptree root;
        pt::read_json("config.json", root);

        m_scale = root.get<int>("scale", 1);
        m_screenshots_dir = root.get<std::string>("screenshots_dir", "");
    }
    catch(const std::exception& e) {
        m_fail = true;
    }
}

Config::Config()
    : m_p(new Private()) {}

Config::~Config() = default;

int Config::getScale() const {
    return m_p->m_scale;
}

const std::string& Config::getScreenshotsDir() const {
    return m_p->m_screenshots_dir;
}

bool Config::getFail() const {
    return m_p->m_fail;
}
