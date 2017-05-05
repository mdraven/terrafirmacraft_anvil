#pragma once

#include <memory>
#include <string>

class ScanImage final {
    struct Private;
    std::unique_ptr<Private> m_p;
public:
    ScanImage();
    ~ScanImage();

    enum class Status {
        initial,
        ok,
        cannot_open,
        is_not_anvil
    };

    bool openImage(const std::string& filename);
    bool getScores(int& red_score, int& green_score) const;
    Status getStatus() const;
    void setScale(int scale);
};
