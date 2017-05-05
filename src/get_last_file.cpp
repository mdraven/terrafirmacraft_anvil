
#include "get_last_file.hpp"

#include <boost/filesystem.hpp>
#include <boost/range.hpp>

bool get_last_file(const std::string& dir, std::string& path_to_file) {
    using namespace boost::filesystem;

    path p(dir);

    if(!is_directory(p))
        return false;

    std::time_t max_write_time = 0;

    for(auto& entry : boost::make_iterator_range(directory_iterator(p), {})) {
        std::time_t write_time = last_write_time(entry);
        if(max_write_time < write_time) {
            max_write_time = write_time;
            path_to_file = entry.path().string();
        }
    }

    return true;
}
