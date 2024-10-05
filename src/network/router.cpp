#include "include/router.hpp"

namespace aether {
    void Router::setDefaultCallback(Callback func) {
        default_callback_ = func;
    }

    void Router::addCallback(const std::string_view& path, Callback func) {
        callbacks_[path] = func;
    }

    Callback Router::getCallback(const std::string_view& path) {
        if (auto it = callbacks_.find(path); it != callbacks_.end()) {
            return it->second;
        }
        return default_callback_;
    }
};