#pragma once

#include <memory>

class MainWindow final {
    struct Private;
    std::unique_ptr<Private> m_p;
public:
    MainWindow();
    ~MainWindow();

    int run(int argc, char **argv);
};
