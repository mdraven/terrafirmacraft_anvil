#pragma once

#include <memory>
#include <string>

class Config final {
    struct Private;
    std::unique_ptr<Private> m_p;
public:
    Config();
    ~Config();

    int getScale() const;
    const std::string& getScreenshotsDir() const;
    bool getFail() const;
};
